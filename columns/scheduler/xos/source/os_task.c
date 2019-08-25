/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include ".\os_private.h"
#include ".\os_port.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void os_task_del        (void);

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr    (CPU_STK        *pbos,
                                UINT32          size,
                                UINT16          opt);
#endif

static void os_task_return     (void           *arg);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*
 *  \brief      CREATE A TASK
 * 
 *  \remark     This function is used to have OS manage the execution of a task.  Tasks can either
 *              be created prior to the start of multitasking or by a running task.  A task cannot be
 *              created by an ISR.
 * 
 * 
 *              the thread entry function shall like this:
 *                  int task (void *parg)
 *                  {
 *                      if () {
 *                         return -1;
 *                      }
 *                      
 *                      for (;;) {
 *                         Task code;
 *                      }
 *                      
 *                      return 0;
 *                  }
 * 
 *  \param      pHandle    is a pointer to the task's handle
 * 
 *              stack      point to the array stack[].
 * 
 *              stackSize  The size of the stack. The value is the length of the stack[], NOT sizeof(stack).
 * 
 *              priority   The value should be LESS than OS_TASK_LOWEST_PRIO.
 * 
 *              options    contains additional information (or options) about the behavior of the task.  The
 *                         LOWER 8-bits are reserved by OS while the upper 8 bits can be application
 *                         specific.  See OS_TASK_OPT_??? in OS.H.  Current choices are:
 * 
 *                         OS_TASK_OPT_STK_CHK     Stack checking to be allowed for the task
 *                         OS_TASK_OPT_STK_CLR     Clear the stack when the task is created
 *                         OS_TASK_OPT_SAVE_FP     If the CPU has floating-point registers, save them
 *                                                 during a context switch.
 * 
 *  \return     OS_ERR_NONE            if the function was successful.
 *              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum
 *              OS_ERR_USE_IN_ISR      if you tried to create a task from an ISR.
 */

OS_ERR  osTaskCreate(   OS_HANDLE      *pHandle,
                        OS_TASK        *entry,
                        void           *argument,
                        CPU_STK        *stack,
                        UINT32          stackSize,
                        UINT16          options,
                        UINT8           priority)
{
    OS_TCB     *ptcb;
    CPU_STK    *psp;


#if OS_ARG_CHK_EN > 0u
    if (entry == NULL) {
        return OS_ERR_NULL_POINTER;
    }
#if OS_TASK_STACK_ON_HEAP_EN == 0
    if (stack == NULL) {
        return OS_ERR_NULL_POINTER;
    }
#endif
    if (priority >= OS_TASK_LOWEST_PRIO) {  //!< Make sure priority is within allowable range
        return OS_ERR_INVALID_PRIO;
    }
#endif
    if (osIntNesting > 0u) {                //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;           //!< ... Should not create object from an ISR.
    }
    
    //! Get a TCB object.
    OSEnterCriticalSection();
    ptcb = pool_new(&osTCBFreePool);
    if (ptcb == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OUT_OF_MEMORY;
    }
    options &= ~OS_TASK_OPT_STK_HEAP;
#if OS_TASK_STACK_ON_HEAP_EN > 0
    if (stack == NULL) {
        stack = OSHeapAlloc(stackSize * sizeof(CPU_STK));
        if (stack == NULL) {
            pool_free(&osTCBFreePool, ptcb);
            OSExitCriticalSection();
            return OS_ERR_OUT_OF_MEMORY;
        }
        options |= OS_TASK_OPT_STK_HEAP;
    }
#endif
    OSExitCriticalSection();
    
    //! initial TCB.
#if (OS_STAT_TASK_STK_CHK_EN > 0u)
    os_task_stk_clr(stack, stackSize, options);
#endif
#if OS_CPU_STK_GROWTH_DOWN == 1u
    if (stackSize != 0u) {
        psp = OSTaskStkInit(stack + stackSize - 1u, (void *)&os_task_return, (void *)entry, argument);
    } else {
        psp = OSTaskStkInit(stack, (void *)&os_task_return, (void *)entry, argument);
    }
#else
    psp = OSTaskStkInit(stack, (void *)&os_task_return, (void *)entry, argument);
#endif
    OS_TCBInit(ptcb, priority, psp, stack, stackSize, options);
    
#if OS_HOOKS_EN > 0u
    OSTaskCreateHook(ptcb);
#endif

    //! give this task's TCB to scheduler.
    OSEnterCriticalSection();
    OS_SchedulerReadyTask(ptcb);
    OSExitCriticalSection();
    
    if (pHandle != NULL) {
        *pHandle = ptcb;
    }
    
    if (osRunning != FALSE) {           //!< try scheduling only when the os has been running.
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}

#if OS_TASK_DEL_EN > 0u
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
static void os_unlock_mutex(OS_MUTEX *pmutex)
{
    OS_TCB     *ptcb;
    
    
    pmutex->OSMutexCnt = 0u;

    list_remove(&pmutex->OSMutexOvlpList);
    
    if (!LIST_IS_EMPTY(pmutex->OSMutexWaitList)) {                  //!< Is any task waiting for the mutex?
        ptcb = OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pmutex,     //!< Yes, Make HPT waiting for mutex ready
                                    &pmutex->OSMutexWaitList,
                                    OS_STAT_PEND_OK);
        pmutex->OSMutexOwnerTCB  = ptcb;
        pmutex->OSMutexOwnerPrio = ptcb->OSTCBPrio;
    } else {                                                        //!< No.
        pmutex->OSMutexOwnerTCB  = NULL;
        pmutex->OSMutexOwnerPrio = 0u;
    }
}
#endif

/*
 *  \brief      DELETE CURRENT TASK
 * 
 *  \remark     This function delete current running task. The deleted task is returned to the dormant
 *              state and can be re-activated by creating the deleted task again. This function is
 *              internal to OS. Your task should be terminated by a return.
 *               
 * 
 *  \param      none
 * 
 *  \return     none
 */
static void os_task_del(void)
{
    OS_TCB         *ptcb;
    OS_MUTEX       *pmutex;


    OSEnterCriticalSection();
    ptcb = osTCBCur;
    
    //! set free from any object that it suspend for.
    if (ptcb->OSTCBWaitNode != NULL) {      //!< Is this task suspend for any object?
        OS_WaitNodeRemove(ptcb);            //!< Yes, set it free from that object.
    } else {                                //!< NO. It's owned by scheduler ...
        OS_SchedulerUnreadyTask(ptcb);      //!< ... set it free from scheduler.
    }

    //! to ensure task releases all mutex(es) that it has had got. This should be a fatal error???
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
    while (!LIST_IS_EMPTY(ptcb->OSTCBOwnMutexList)) {
        OS_LIST_NODE *list = ptcb->OSTCBOwnMutexList.Next;
        pmutex = CONTAINER_OF(list, OS_MUTEX, OSMutexOvlpList);
        os_unlock_mutex(pmutex);
    }
#endif
    
#if OS_TASK_STACK_ON_HEAP_EN > 0
    if (ptcb->OSTCBOpt & OS_TASK_OPT_STK_HEAP) {
        OSHeapFree(ptcb->OSTCBStkBase);
    }
#endif
    pool_free(&osTCBFreePool, ptcb);   //!< Return TCB object to free TCB pool.
    osTCBCur = NULL;
    OSExitCriticalSection();
    
    OS_SchedulerRunNext();
}
#endif

/*
 *  \brief      CHANGE PRIORITY OF A TASK
 * 
 *  \remark     This function allows you to change the priority of a task dynamically.  Note that the new
 *              priority MUST be available.
 * 
 *  \param      ptcb     pointer to tcb
 * 
 *              newp     is the new priority
 * 
 *  \return     OS_ERR_NONE             is the call was successful
 *              OS_ERR_INVALID_HANDLE   ptcb is NULL.
 *              OS_ERR_INVALID_PRIO     if the priority you specify is higher that the maximum allowed
 *              OS_ERR_TASK_NOT_EXIST   there is no task with the specified OLD priority (i.e. the OLD task does
 *                                      not exist.
 */
#if OS_TASK_CHANGE_PRIO_EN > 0u
OS_ERR osTaskChangePrio(OS_HANDLE taskHandle, UINT8 newprio)
{
    OS_TCB     *ptcb = (OS_TCB *)taskHandle;
    
    
#if OS_ARG_CHK_EN > 0u
    if (taskHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#if OS_TASK_LOWEST_PRIO < 255
    if (newprio > OS_TASK_LOWEST_PRIO) {
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif
    if (OS_OBJ_TYPE_TCB != OS_OBJ_TYPE_GET(ptcb->OSTCBObjHeader.OSObjType)) {
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
    if (!LIST_IS_EMPTY(ptcb->OSTCBOwnMutexList)) {      //!< See if the task ownes any mutex.
                                                        //!< Yes. Update the priority store in the mutex(es).
        for (OS_LIST_NODE *iterate = ptcb->OSTCBOwnMutexList.Next; iterate != &ptcb->OSTCBOwnMutexList; iterate = iterate->Next) {
            OS_MUTEX *pmutex = CONTAINER_OF(iterate, OS_MUTEX, OSMutexOvlpList);
            pmutex->OSMutexOwnerPrio = newprio;
        }
        if (newprio < ptcb->OSTCBPrio) {                //!< Re-shedule only if the new priority is higher than the task's current.
            OS_ChangeTaskPrio(ptcb, newprio);           //! ... the task will use the new priority after it release the mutex.
        }                                               //! ... because the anti-priority-reverse mechanism meight need to rise it's priority.
    } else {
#else
    {
#endif
        OS_ChangeTaskPrio(ptcb, newprio);
    }
    OSExitCriticalSection();
    
    if (osRunning != FALSE) {                           //!< try scheduling only the os has been running ...
        OS_SchedulerRunPrio();                          //!  in case this is called before os has started.
    }
    
    return OS_ERR_NONE;
}
#endif

/*
 *  \brief      CATCH ACCIDENTAL TASK RETURN
 * 
 *  \remark     This function is wrapper of a task.  It mainly handles the return of the task and deletes
 *              it after the task function returns.
 * 
 *  \param      parg          this value is returned by the task.
 * 
 *  \return     none
 * 
 *  \note       This function is INTERNAL to OS and your application should not call it.
 */

static void os_task_return(void *parg)
{
#if OS_HOOKS_EN > 0
    OSTaskReturnHook(osTCBCur, parg);   //!< Call hook to let user decide on what to do
#endif

#if OS_TASK_DEL_EN > 0u
    os_task_del();                      //!< Delete task if it returns!
#else
    //! this should be a fatal error!
    OSEnterCriticalSection();
    while (1);
#endif
}

/*
 *  \brief      CLEAR TASK STACK
 * 
 *  \remark     This function is used to clear the stack of a task (i.e. write all zeros)
 * 
 *  \param      pbos    is a pointer to the task's BOTTOM of stack.  If the configuration constant
 *                      OS_CPU_STK_GROWTH_DOWN is set to 1, the stack is assumed to grow downward (i.e. from high
 *                      memory to low memory).  'pbos' will thus point to the lowest (valid) memory
 *                      location of the stack.  If OS_CPU_STK_GROWTH_DOWN is set to 0, 'pbos' will point to the
 *                      highest memory location of the stack and the stack will grow with increasing
 *                      memory locations.  'pbos' MUST point to a valid 'free' data item.
 * 
 *              size    is the number of 'stack elements' to clear.
 * 
 *              opt     contains additional information (or options) about the behavior of the task.  The
 *                      LOWER 8-bits are reserved by OS while the upper 8 bits can be application
 *                      specific.  See OS_TASK_OPT_??? in OS.H.
 * 
 *  \return     none
 */

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr(CPU_STK  *pstk,
                            UINT32   size,
                            UINT16   opt)
{
    if ((opt & OS_TASK_OPT_STK_CHK) != 0u) {        //!< stack checking has been enabled.
        while (size > 0u) {
            size--;
            *pstk++ = (CPU_STK)0;
        }
    } else if ((opt & OS_TASK_OPT_STK_CLR) != 0u) { //!< stack needs to be cleared.
        while (size > 0u) {
            size--;
            *pstk++ = (CPU_STK)0;
        }
    }
}
#endif
