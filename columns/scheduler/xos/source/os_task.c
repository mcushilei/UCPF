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


//! \note do not move this pre-processor statement to other places
#define __OS_TASK_C__

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

/*!
 *! \Brief       CREATE A TASK
 *!
 *! \Description This function is used to have OS manage the execution of a task.  Tasks can either
 *!              be created prior to the start of multitasking or by a running task.  A task cannot be
 *!              created by an ISR.
 *!
 *! \Arguments   pHandle  is a pointer to the task's handle
 *!
 *!              task     is a pointer to the task's code
 *!
 *!
 *! \Returns     OS_ERR_NONE            if the function was successful.
 *!              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum
 *!              OS_ERR_USE_IN_ISR      if you tried to create a task from an ISR.
 */

OS_ERR  osTaskCreate(   OS_HANDLE       *pHandle,
                        OS_TASK_CFG     *cfg)
{
    OS_TCB     *ptcb;
    CPU_STK    *psp;


    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }
#if OS_ARG_CHK_EN > 0u
    if (cfg == NULL) {
        return OS_ERR_NULL_POINTER;
    }
#if OS_TASK_LOWEST_PRIO < 255u
    if (cfg->Priority > OS_TASK_LOWEST_PRIO) {   //!< Make sure priority is within allowable range
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif
    
    //! Get a TCB object.
    OSEnterCriticalSection();
    ptcb = OS_ObjPoolNew(&osTCBFreeList);
    if (ptcb == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection();
    
    //! initial TCB.
#if (OS_STAT_TASK_STK_CHK_EN > 0u)
    os_task_stk_clr(cfg->Stack, cfg->StackSize, cfg->Options);
#endif
#if OS_CPU_STK_GROWTH_DOWN == 1u
    if (cfg->StackSize != 0u) {
        psp = OSTaskStkInit(cfg->Stack + cfg->StackSize - 1u, (void *)&os_task_return, (void *)cfg->Entry, cfg->Argument);
    } else {
        psp = OSTaskStkInit(cfg->Stack + cfg->StackSize,      (void *)&os_task_return, (void *)cfg->Entry, cfg->Argument);
    }
#else
    psp = OSTaskStkInit(cfg->Stack, (void *)&os_task_return, (void *)cfg->Entry, cfg->Argument);
#endif
    OS_TCBInit(ptcb, cfg->Priority, psp, cfg->Stack, cfg->StackSize, cfg->Options);
    
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

    os_list_del(&pmutex->OSMutexOvlpList);
    
    if (!OS_LIST_IS_EMPTY(pmutex->OSMutexWaitList)) {               //!< Any task waiting for the mutex?
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

/*!
 *! \Brief       DELETE CURRENT TASK
 *!
 *! \Description This function delete current running task. The deleted task is returned to the dormant
 *!              state and can be re-activated by creating the deleted task again. This function is
 *!              internal to OS. Your task should be terminated by a return.
 *!              
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
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
    while (!OS_LIST_IS_EMPTY(ptcb->OSTCBOwnMutexList)) {
        OS_LIST_NODE *list = ptcb->OSTCBOwnMutexList.Next;
        pmutex = OS_CONTAINER_OF(list, OS_MUTEX, OSMutexOvlpList);
        os_unlock_mutex(pmutex);
    }
#endif
    
    OS_ObjPoolFree(&osTCBFreeList, ptcb);   //!< Return TCB object to free TCB pool.
    osTCBCur = NULL;
    OSExitCriticalSection();
    
    OS_SchedulerRunNext();
}
#endif

/*!
 *! \Brief       CHANGE PRIORITY OF A TASK
 *!
 *! \Description This function allows you to change the priority of a task dynamically.  Note that the new
 *!              priority MUST be available.
 *!
 *! \Arguments   ptcb     pointer to tcb
 *!
 *!              newp     is the new priority
 *!
 *! \Returns     OS_ERR_NONE            is the call was successful
 *!              OS_ERR_INVALID_HANDLE  ptcb is NULL.
 *!              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum allowed
 *!              OS_ERR_TASK_NOT_EXIST  there is no task with the specified OLD priority (i.e. the OLD task does
 *!                                     not exist.
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
    if (!OS_LIST_IS_EMPTY(ptcb->OSTCBOwnMutexList)) {   //!< See if the task ownes any mutex.
                                                        //!< Yes. Update the priority store in the mutex(es).
        for (OS_LIST_NODE *iterate = ptcb->OSTCBOwnMutexList.Next; iterate != &ptcb->OSTCBOwnMutexList; iterate = iterate->Next) {
            OS_MUTEX *pmutex = OS_CONTAINER_OF(iterate, OS_MUTEX, OSMutexOvlpList);
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

/*!
 *! \Brief       CATCH ACCIDENTAL TASK RETURN
 *!
 *! \Description This function is wrapper of a task.  It mainly handles the return of the task and deletes
 *!              it after the task function returns.
 *!
 *! \Arguments   parg          this value is returned by the task.
 *!
 *! \Returns     none
 *!
 *! \Notes       This function is INTERNAL to OS and your application should not call it.
 */

static void os_task_return(void *parg)
{
#if OS_HOOKS_EN > 0
    OSTaskReturnHook(osTCBCur, parg);    //!< Call hook to let user decide on what to do
#endif

#if OS_TASK_DEL_EN > 0u
    os_task_del();                      //!< Delete task if it returns!
#else
    //! this should be a fatal error!
    OSEnterCriticalSection();
    while (1);
#endif
}

/*!
 *! \Brief       CLEAR TASK STACK
 *!
 *! \Description This function is used to clear the stack of a task (i.e. write all zeros)
 *!
 *! \Arguments   pbos     is a pointer to the task's BOTTOM of stack.  If the configuration constant
 *!                       OS_CPU_STK_GROWTH_DOWN is set to 1, the stack is assumed to grow downward (i.e. from high
 *!                       memory to low memory).  'pbos' will thus point to the lowest (valid) memory
 *!                       location of the stack.  If OS_CPU_STK_GROWTH_DOWN is set to 0, 'pbos' will point to the
 *!                       highest memory location of the stack and the stack will grow with increasing
 *!                       memory locations.  'pbos' MUST point to a valid 'free' data item.
 *!
 *!              size     is the number of 'stack elements' to clear.
 *!
 *!              opt      contains additional information (or options) about the behavior of the task.  The
 *!                       LOWER 8-bits are reserved by OS while the upper 8 bits can be application
 *!                       specific.  See OS_TASK_OPT_??? in OS.H.
 *!
 *! \Returns     none
 */

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr(CPU_STK  *pstk,
                            UINT32   size,
                            UINT16   opt)
{
    if ((opt & OS_TASK_OPT_STK_CHK) != 0x00u) {      //!< See if stack checking has been enabled
        if ((opt & OS_TASK_OPT_STK_CLR) != 0x00u) {  //!< See if stack needs to be cleared
            while (size > 0u) {
                size--;
                *pstk++ = (CPU_STK)0;
            }
        }
    }
}
#endif
