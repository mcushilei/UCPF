/*******************************************************************************
 *  Copyright(C)2017-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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

/*!
 *! \Brief functions for task manage.
 *!
 */


//! \note do not move this pre-processor statement to other places
#define  __OS_TASK_C__

/*============================ INCLUDES ======================================*/
#include ".\os.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void os_task_del        (void);

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr    (CPU_STK         *pbos,
                                UINT32          size,
                                UINT8           opt);
#endif

static void os_task_wrapper    (void           *ptask,
                                void           *parg);

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
 *!              parg     is a pointer to an optional data area which can be used to pass parameters to
 *!                        the task when the task first executes.  Where the task is concerned it thinks
 *!                        it was invoked and passed the argument 'parg' as follows:
 *!
 *!                            void task (void *parg)
 *!                            {
 *!                                for (;;) {
 *!                                    Task code;
 *!                                }
 *!                            }
 *!
 *!              prio      is the task's priority.  A unique priority MUST be assigned to each task and the
 *!                        lower the number, the higher the priority.
 *!
 *!              pstk      point to the LOWEST (valid) memory location of the stack.
 *!
 *!              stkSize   is the size of the stack in number of elements.  If CPU_STK is set to UINT8,
 *!                        'stkSize' corresponds to the number of bytes available.  If CPU_STK is set to
 *!                        UINT32, 'stkSize' contains the number of 32-bit entries available on the stack.
 *!
 *!              opt       contains additional information (or options) about the behavior of the task.  The
 *!                        LOWER 8-bits are reserved by OS while the upper 8 bits can be application
 *!                        specific.  See OS_TASK_OPT_??? in OS.H.  Current choices are:
 *!
 *!                        OS_TASK_OPT_STK_CHK      Stack checking to be allowed for the task
 *!                        OS_TASK_OPT_STK_CLR      Clear the stack when the task is created
 *!                        OS_TASK_OPT_SAVE_FP      If the CPU has floating-point registers, save them
 *!                                                 during a context switch.
 *!
 *! \Returns     OS_ERR_NONE            if the function was successful.
 *!              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum
 *!              OS_ERR_USE_IN_ISR      if you tried to create a task from an ISR.
 */

OS_ERR  osTaskCreate(   OS_HANDLE  *pHandle,
                        OS_TASK    *task,
                        void       *parg,
                        UINT8       prio,
                        CPU_STK     *pstk,
                        UINT32      stkSize,
                        UINT8       opt)
{
    OS_TCB     *ptcb;
    CPU_STK     *psp;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }
#if OS_ARG_CHK_EN > 0u
#if OS_TASK_LOWEST_PRIO < 255u
    if (prio > OS_TASK_LOWEST_PRIO) {   //!< Make sure priority is within allowable range
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif
    
    //! Get a TCB object.
    OSEnterCriticalSection(cpu_sr);
    ptcb = OS_ObjPoolNew(&osTCBFreeList);
    if (ptcb == NULL) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection(cpu_sr);
    
    //! initial TCB.
#if (OS_STAT_TASK_STK_CHK_EN > 0u)
    os_task_stk_clr(pstk, stkSize, opt);                //!< Clear the task's stack
#endif
#if OS_STK_GROWTH_DOWN == 1u                            //!< Initialize the task's stack
    if (stkSize != 0u) {
        psp = OSTaskStkInit(pstk + stkSize - 1u, (void *)&os_task_wrapper, (void *)task, parg);
    } else {
        psp = OSTaskStkInit(pstk + stkSize,      (void *)&os_task_wrapper, (void *)task, parg);
    }
#else
    psp = OSTaskStkInit(pstk, (void *)&os_task_wrapper, (void *)task, parg);
#endif
    OS_TCBInit(ptcb, prio, psp, pstk, stkSize, opt);    //!< Initialize the TCB object.
    
#if OS_HOOKS_EN > 0u
    OSTaskCreateHook(ptcb);
#endif

    //! give this task's TCB to scheduler.
    OSEnterCriticalSection(cpu_sr);
    OS_SchedulerReadyTask(ptcb);
    OSExitCriticalSection(cpu_sr);
    
    if (pHandle != NULL) {
        *pHandle = ptcb;
    }
    
    if (osRunning != FALSE) {           //!< try scheduling only the os has been running.
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

#if OS_MUTEX_OVERLAP_EN > 0u
    os_list_del(&pmutex->OSMutexOvlpList);
#else
    osTCBCur->OSTCBOwnMutex = NULL;
#endif
    
    if (pmutex->OSMutexWaitList.Next != &pmutex->OSMutexWaitList) {                 //!< Any task waiting for the mutex?
        ptcb = OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pmutex, OS_STAT_PEND_OK);   //!< Yes, Make HPT waiting for mutex ready
        pmutex->OSMutexOwnerTCB  = ptcb;
        pmutex->OSMutexOwnerPrio = ptcb->OSTCBPrio;
    } else {                                                                        //!< No.
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
 *!              internal to OS and called by os_task_wrapper. Your task should be terminated by a return.
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
#if OS_CRITICAL_METHOD == 3u
    CPU_SR       cpu_sr = 0u;
#endif


    OSEnterCriticalSection(cpu_sr);
    ptcb = osTCBCur;
    
    //! set free from any object that it suspend for.
    if (ptcb->OSTCBWaitNode != NULL) {      //!< Is this task suspend for any object?
        OS_WaitNodeRemove(ptcb);            //!< Yes, set it free from that object.
    } else {                                //!< NO. It's owned by scheduler ...
        OS_SchedulerUnreadyTask(ptcb);      //!< ... set it free from scheduler.
    }

    //! to ensure task releases all mutex(es) that it has had got. This should be a fatal error???
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
#if OS_MUTEX_OVERLAP_EN > 0u
    for (OS_LIST_NODE *list = ptcb->OSTCBOwnMutexList.Next; list != &ptcb->OSTCBOwnMutexList;) {
        pmutex = OS_CONTAINER_OF(list, OS_MUTEX, OSMutexOvlpList);
        list = list->Next;
        os_unlock_mutex(pmutex);
    }
#else
    pmutex = ptcb->OSTCBOwnMutex;
    if (pmutex != NULL) {
        pmutex->OSMutexCnt = 0u;
        os_unlock_mutex(pmutex);
    }
#endif
#endif
    
    OS_ObjPoolFree(&osTCBFreeList, ptcb);   //!< Return TCB object to free TCB pool.
    osTCBCur = NULL;
    OSExitCriticalSection(cpu_sr);
    
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
#if OS_CRITICAL_METHOD == 3u
    CPU_SR   cpu_sr = 0u;
#endif
    
    
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

    OSEnterCriticalSection(cpu_sr);
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
#   if OS_MUTEX_OVERLAP_EN > 0u
    if (ptcb->OSTCBOwnMutexList.Next != &ptcb->OSTCBOwnMutexList) {
        OS_MUTEX *pmutex = OS_CONTAINER_OF(ptcb->OSTCBOwnMutexList.Next, OS_MUTEX, OSMutexOvlpList);
        pmutex->OSMutexOwnerPrio = newprio;
        if (newprio < ptcb->OSTCBPrio) {
            OS_ChangeTaskPrio(ptcb, newprio);
        }
#   else
    if (ptcb->OSTCBOwnMutex != NULL) {                  //!< See if the task has owned a mutex.
        ptcb->OSTCBOwnMutex->OSMutexOwnerPrio = newprio;//!< Yes. Update the priority store in the mutex ...
        if (newprio < ptcb->OSTCBPrio) {                //!< Change the priority only if the new one ...
                                                        //! ... is higher than the task's current,
            OS_ChangeTaskPrio(ptcb, newprio);           //! ... else the task will use the new priority
        }                                               //! ... after it release the mutex.
#   endif
    } else {
#else
    {
#endif
        OS_ChangeTaskPrio(ptcb, newprio);
    }
    OSExitCriticalSection(cpu_sr);
    
    if (osRunning != FALSE) {                           //!< try scheduling only the os has been running.
        OS_SchedulerRunPrio();
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
 *! \Arguments   ptask         is a pointer to the task function.
 *!
 *!              parg          is a pointer to a user supplied data area that will be passed to the task
 *!                            when the task first executes.
 *!
 *! \Returns     none
 *!
 *! \Notes       This function is INTERNAL to OS and your application should not call it.
 */

static void os_task_wrapper(void *ptask, void *parg)
{
    (void)((OS_TASK *)ptask)(parg);
    
#if OS_HOOKS_EN > 0
    OSTaskReturnHook(osTCBCur);     //!< Call hook to let user decide on what to do
#endif

#if OS_TASK_DEL_EN > 0u
    os_task_del();                  //!< Delete task if it returns!
#else
    //! this may be a fatal error!
    osTaskSleep(OS_INFINITE);
#endif
}

/*!
 *! \Brief       CLEAR TASK STACK
 *!
 *! \Description This function is used to clear the stack of a task (i.e. write all zeros)
 *!
 *! \Arguments   pbos     is a pointer to the task's BOTTOM of stack.  If the configuration constant
 *!                       OS_STK_GROWTH_DOWN is set to 1, the stack is assumed to grow downward (i.e. from high
 *!                       memory to low memory).  'pbos' will thus point to the lowest (valid) memory
 *!                       location of the stack.  If OS_STK_GROWTH_DOWN is set to 0, 'pbos' will point to the
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
                            UINT8    opt)
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
