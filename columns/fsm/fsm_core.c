/*******************************************************************************
 *  Copyright(C)2016-2018 by Dreistein<mcu_shilei@hotmail.com>                *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not, see http://www.gnu.org/licenses/.        *
*******************************************************************************/



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fsm_core.h"

/*============================ MACROS ========================================*/
//! \brief task pool size
#if ((!defined(FSM_MAX_TASKS)) || (FSM_MAX_TASKS < 1))
#   define FSM_MAX_TASKS           (1u)
#   warning "FSM_MAX_TASKS is invalid, use default value 1u."
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static bool         fsm_task_stack_push        (fsm_tcb_t          *task,
                                                fsm_state_t        *state,
                                                void               *arg);
static bool         fsm_task_stack_pop         (fsm_tcb_t          *task);
static fsm_tcb_t   *fsm_tcb_new                (void);
static void         fsm_tcb_free               (fsm_tcb_t          *task);
static void         fsm_tcb_init               (fsm_tcb_t          *pTCB,
                                                fsm_state_t        *state,
                                                void               *arg,
                                                task_stack_t       *stack,
                                                uint8_t             stackSize);

static bool         fsm_task_enqueue           (list_node_t    *pTaskQueue,
                                                fsm_tcb_t      *pTask);
static fsm_tcb_t   *fsm_task_dequeue           (list_node_t    *pTaskQueue);

static fsm_tcb_t   *fsm_get_next_ready_task    (void);
static void         fsm_set_task_pend          (uint32_t            timeDelay);

static void         fsm_set_task_ready         (fsm_tcb_t          *pTask,
                                                uint8_t             pendStat);

/*============================ LOCAL VARIABLES ===============================*/
static fsm_tcb_t   *fsmTCBFreeList;                 //! TCB
static fsm_tcb_t    fsmTCBTbl[FSM_MAX_TASKS];       //! TCB pool

/*============================ GLOBAL VARIABLES ==============================*/
uint8_t             fsmIntNesting;
scheduler_t         fsmScheduler;

/*============================ IMPLEMENTATION ================================*/
void fsm_int_enter(void)
{
    //! there is no need atom protecting.
    if (fsmIntNesting < 255u) {
        fsmIntNesting++;
    }
}
                
void fsm_int_exit(void)
{
    if (fsmIntNesting != 0) {
        fsmIntNesting--;
    }
}
                
/*! \brief try to push a routine into stack
 *  \param task
 *  \param state routine
 *  \param arg argument for routine
 *  \retval true succeeded in pushing task routine into common return stack.
 *  \retval false failed to push task routine into common return stack
 */
static bool fsm_task_stack_push(
        fsm_tcb_t      *task,
        fsm_state_t    *state,
        void           *arg)
{
    task_stack_t *ptStack;
    
    //! if stack full
    if ((task->StackPoint + 1) >= task->StackSize) {
        return false;
    }

    task->StackPoint++;
    ptStack = task->Stack + task->StackPoint;
    ptStack->State = state;
    ptStack->Arg   = arg;

    return true;
}

/*! \brief  try to pop a routine from stack
 *  \param  task
 *  \retval true succeeded in pushing task routine into common return stack.
 *  \retval false failed to push task routine into common return stack
 */
static bool fsm_task_stack_pop(fsm_tcb_t *task)
{
    //! if stack empty
    if (0 == task->StackPoint) {
        return false;
    }

    task->StackPoint--;

    return true;
}

/*! \brief perform a state transfer.
 *  \param state    target state. Transfering to a NULL state will terminal this FSM.
 *  \param arg a    argument to the task.
 *  \retval false   invalid param
 *  \retval true    succeeded to transfer state
 */
bool fsm_state_transfer(fsm_state_t *state, void *arg)
{
    fsm_tcb_t      *pTCB      = fsmScheduler.CurrentTask;
    task_stack_t   *ptRoutine = pTCB->Stack + pTCB->StackPoint;

    ptRoutine->State = state;
    ptRoutine->Arg   = arg;

    return true;
}

/*! \brief  call a sub routine and return CURRENT state when sub fsm complete.
 *  \param  state target routine
 *  \param  arg a pointer of argument control block
 *  \retval false invalid param or stack overflow
 *  \retval true succeeded to call sub FSM
 */
fsm_err_t fsm_call_sub(fsm_state_t *state, void *arg)
{
    fsm_tcb_t      *task = fsmScheduler.CurrentTask;

    if (!fsm_task_stack_push(task, state, arg)) {
        return FSM_ERR_TASK_STACK_FULL;
    }

    return FSM_ERR_NONE;
}

static void fsm_tcb_init(
        fsm_tcb_t          *pTCB,
        fsm_state_t        *state,
        void               *arg,
        task_stack_t       *stack,
        uint8_t             stackSize)
{
    list_init(&pTCB->ListNode);
    
    pTCB->Flag              = 0;
    pTCB->Status            = FSM_TASK_STATUS_READY;

    pTCB->Stack             = stack;        //!< set stack buffer
    pTCB->StackSize         = stackSize;   //!< set stack size
    pTCB->StackPoint        = 0;             //!< set stack point
    pTCB->Stack[0].State    = state;       //!< set task routine
    pTCB->Stack[0].Arg      = arg;          //!< set argument

#if SAFE_TASK_THREAD_SYNC == ENABLED
    pTCB->Object            = NULL;
    list_init(&pTCB->WaitNode);
#endif
}

/*! \brief  create a new task control block
 *  \param  state task routine
 *  \param  fnReturnTo a routine which will be called when task routine return
 *          false.
 *  \param  arg a pointer of a argument control block
 *  \param  stack a return stack buffer
 *  \param  stackSize return stack size
 *  \return task handle
 */
fsm_err_t fsm_task_create(
        fsm_handle_t       *pptTask,
        fsm_state_t        *state,
        void               *arg,
        task_stack_t       *stack,
        uint8_t             stackSize)
{
    fsm_tcb_t *ptTask;
    
    //! validate parameters.
    if ((NULL == state)
    ||  (NULL == stack) 
    ||  (0 == stackSize)) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    ptTask = fsm_tcb_new();         /*! try to get a TCB */
    if (NULL == ptTask) {
        return FSM_ERR_TASK_NO_MORE_TCB;
    }
    
    fsm_tcb_init(ptTask, state, arg, stack, stackSize);
    
    fsm_set_task_ready(ptTask, FSM_TASK_STATUS_READY);     /*! Let this task to run. */

    if (NULL != pptTask) {
        *pptTask = ptTask;
    }
    
    return FSM_ERR_NONE;
}

fsm_err_t fsm_task_delay(uint32_t wTimeout)
{
    uint8_t         chResult;
    fsm_tcb_t      *pTask = fsmScheduler.CurrentTask;

    if (fsmIntNesting != 0) {
        return FSM_ERR_PEND_ISR;
    }
    
    switch (pTask->Status) {
        case FSM_TASK_STATUS_READY:
            if (wTimeout == 0u) {
                chResult = FSM_ERR_TASK_PEND_TIMEOUT;
            } else {
                //! add task to the object's wait queue.
                pTask->Object = NULL;
                fsm_set_task_pend(wTimeout);
                chResult = FSM_ERR_OBJ_NOT_SINGLED;
            }
            break;
            
        case FSM_TASK_STATUS_PEND_OK:
        case FSM_TASK_STATUS_PEND_TIMEOUT:
            pTask->Status    = FSM_TASK_STATUS_READY;
            chResult         = FSM_ERR_TASK_PEND_TIMEOUT;
            break;
        
        case FSM_TASK_STATUS_PEND:
        default:
            chResult = FSM_ERR_OBJ_NOT_SINGLED;
            break;
    }

    return chResult;
}

void fsm_tick(void)
{
    fsm_tcb_t      *pTask;
    list_node_t    *pNode;
    
    if (fsmScheduler.Running == false) {
        return;
    }
        
    FSM_SAFE_ATOM_CODE(
        //! there would not be lots of tasks to pend, so this does things well enough.
        for (pNode = fsmScheduler.PendList.Next; pNode != &fsmScheduler.PendList; ) {
            pTask = CONTAINER_OF(pNode, fsm_tcb_t, ListNode);
            pNode = pNode->Next;
            if ((pTask->Delay != 0) && (pTask->Delay != FSM_INFINITE)) {
               pTask->Delay--;
               if (pTask->Delay == 0) {
#if SAFE_TASK_THREAD_SYNC == ENABLED
                   if (pTask->Object != NULL) {
                       fsm_waitable_obj_remove_task(pTask->Object, pTask);
                   }
#endif
                   fsm_set_task_ready(pTask, FSM_TASK_STATUS_PEND_TIMEOUT);
               }
            }
        }
    )
}


/*! \brief add a task control block to a specified task control queue
 *  \param pTaskQueue a pointer of a task queue control block
 *  \param task a pointer of a task control block
 *  \retval false failed to add task to queue
 *  \retval true succeeded in adding task to queue
 */
static bool fsm_task_enqueue(list_node_t *pTaskQueue, fsm_tcb_t *pTask)
{
    list_insert(&pTask->ListNode, pTaskQueue->Prev);

    return true;
}

/*! \brief  remove a task control block from a specified task control queue
 *  \param  pTaskQueue a pointer of a task queue control block
 *  \param  task a pointer of a task control block
 *  \retval NULL failed to get a task from queue
 *  \retval true succeeded to get a task from queue
 */
static fsm_tcb_t *fsm_task_dequeue(list_node_t *pTaskQueue)
{
    fsm_tcb_t *pTask;
    
    if (LIST_IS_EMPTY(pTaskQueue)) {
        return NULL;
    }
    
    pTask = CONTAINER_OF(pTaskQueue->Next, fsm_tcb_t, ListNode);
    list_remove(&pTask->ListNode);

    return pTask;
}

/*! \brief  add a task to ready list queue.
 *  \param  task task control block
 */
void fsm_set_task_ready(fsm_tcb_t *pTask, uint8_t pendStat)
{
    FSM_SAFE_ATOM_CODE(
        pTask->Status = pendStat;
        pTask->Object = NULL;
        list_remove(&pTask->ListNode);
        fsm_task_enqueue(&fsmScheduler.ReadyList, pTask);
    )
}

static void fsm_set_task_pend(uint32_t timeDelay)
{
    FSM_SAFE_ATOM_CODE(
        fsmScheduler.CurrentTask->Delay    = timeDelay;
        fsmScheduler.CurrentTask->Status   = FSM_TASK_STATUS_PEND;
        fsm_task_enqueue(&fsmScheduler.PendList, fsmScheduler.CurrentTask);
    )
}

static fsm_tcb_t *fsm_get_next_ready_task(void)
{
    fsm_tcb_t *pTask;
    
    FSM_SAFE_ATOM_CODE(
        pTask = fsm_task_dequeue(&fsmScheduler.ReadyList);
    )
        
    return pTask;
}


void fsm_waitable_obj_pnd_task(fsm_waitable_obj_t *pObj, uint32_t timeDelay)
{
    fsm_tcb_t *pTask = fsmScheduler.CurrentTask;
    
    pTask->Object = pObj;
    list_insert(&pTask->WaitNode, &pObj->TaskQueue);
    fsm_set_task_pend(timeDelay);
}

fsm_tcb_t *fsm_waitable_obj_rdy_task(fsm_waitable_obj_t *pObj, uint8_t pendStat)
{
    fsm_tcb_t *pTask;
    
    if (LIST_IS_EMPTY(&pObj->TaskQueue)) {
        return NULL;
    }
    
    pTask = CONTAINER_OF(pObj->TaskQueue.Next, fsm_tcb_t, WaitNode);
    list_remove(&pTask->WaitNode);
    fsm_set_task_ready(pTask, pendStat);

    return pTask;
}

void fsm_waitable_obj_remove_task(fsm_waitable_obj_t *pObj, fsm_tcb_t *pTask)
{
    list_remove(&pTask->WaitNode);
}

void fsm_start(void)
{
    fsmScheduler.Running = true;
}

/*! \brief  fsm_scheduler function
 *  \retval true system is busy
 *  \retval false system is idle, no task to run.
 */
bool fsm_scheduler(void)
{  
    fsm_tcb_t      *pTask;
    task_stack_t   *ptRoutine;

    pTask = fsm_get_next_ready_task();
    if (NULL == pTask) {
        return false;
    }
    fsmScheduler.CurrentTask = pTask;

    ptRoutine = pTask->Stack + pTask->StackPoint;
    ptRoutine->State(ptRoutine->Arg);                   //!< run state routine.

    do {
        if (pTask->Status == FSM_TASK_STATUS_PEND) {    //!< task has been pended?
            break;                                      //!< Yes, this task has been moved to pend list.
        }
                                                        //!< No.
        if (ptRoutine->State != NULL) {                         //!< Whether this fsm is complete.
            fsm_set_task_ready(pTask, FSM_TASK_STATUS_READY);   //!< No.
        } else if (fsm_task_stack_pop(pTask)) {                 //!< Yes, Has parent fsm?
            fsm_set_task_ready(pTask, FSM_TASK_STATUS_READY);   //!< Yes, re-add this task to queue
        } else {
            fsm_tcb_free(pTask);                                //!< No, this task finished, free task
        }
    } while (0);

    return true;
}

/*! \brief get a new task control block from pool and initial it
 *  \param state task's first routine
 *  \param arg a pointer of a argument
 *  \param stack a return stack buffer
 *  \param stackSize return stack size
 *  \retval NULL failed to create a new task control block
 *  \retval a pointer for a initialized task control block
 */
static fsm_tcb_t *fsm_tcb_new(void)
{
    fsm_tcb_t *pTCB;
    
    if (NULL == fsmTCBFreeList) {
        return NULL;
    }
    pTCB           = fsmTCBFreeList;
    fsmTCBFreeList = (fsm_tcb_t *)pTCB->ObjNext;

    return pTCB;
    
}

/*! \brief  free a task control blcok to pool
 *  \param  pTCB a pointer of a task control block
 *  \return none
 */
static void fsm_tcb_free(fsm_tcb_t *pTCB)
{
    pTCB->ObjNext  = fsmTCBFreeList;
    fsmTCBFreeList = pTCB;
}

/*! \brief  add task control blocks to the task pool
 *  \param  pHead task heap buffer
 *  \param  hwSize heap size
 *  \return access result
 */
void fsm_tcb_pool_init(void)
{
    uint_fast16_t n;
    fsm_tcb_t **p;

    p = &fsmTCBFreeList;
    
    //! add mutex OCBs to the free list
    for (n = 0; n < ARRAY_LENGTH(fsmTCBTbl); n++) {
        *p = &fsmTCBTbl[n];
        p = (fsm_tcb_t **)&((*p)->ObjNext);
    }
}

/* EOF */
