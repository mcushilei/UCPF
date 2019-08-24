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
#define __OS_SEM_C__

/*============================ INCLUDES ======================================*/
#include ".\os_private.h"
#include ".\os_port.h"

#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*!
 *! \Brief       CREATE A SEMAPHORE
 *!
 *! \Description This function creates a semaphore.
 *!
 *! \Arguments   pSemHandle     is a pointer to a handle of the semaphore.
 *!
 *!              initCnt        is the initial value for the semaphore. If the value is 0, no
 *!                             resource is available. You initialize the semaphore to a non-zero 
 *!                             value to specify how many resources are available.
 *!
 *! \Returns     OS_ERR_NONE            If the call was successful.
 *!              OS_ERR_USE_IN_ISR      If you attempted to create a MUTEX from an ISR
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is a NULL pointer.
 *!              OS_ERR_OBJ_DEPLETED    No more event control blocks available.
 */
OS_ERR osSemCreate(OS_HANDLE *pSemHandle, UINT16 initCnt)
{
    OS_SEM     *psem;


#if OS_ARG_CHK_EN > 0u
    if (pSemHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }

    //! malloc a ECB from pool.
    OSEnterCriticalSection();
    psem = pool_new(&osSemFreePool);
    if (psem == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection();

    //! Set object type.
    //! Init semaphore value.
    //! Init wait list head.
    psem->OSSemObjHead.OSObjType =    OS_OBJ_TYPE_SET(OS_OBJ_TYPE_SEM)
                                    | OS_OBJ_TYPE_WAITABLE_MSK
                                    | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_PRIO_LIST);
    psem->OSSemToken    = initCnt;
    list_init(&psem->OSSemWaitList);
    
    *pSemHandle = psem;
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       DELETE A SEMAPHORE
 *!
 *! \Description This function deletes a semaphore and readies all tasks pending on the semaphore.
 *!
 *! \Arguments   pSemHandle     is a pointer to a handle of the semaphore.
 *!
 *!              opt            determines delete options as follows:
 *!                             opt == OS_DEL_NOT_IN_USE    Delete semaphore ONLY if no task pending
 *!                             opt == OS_DEL_ALWAYS        Deletes the semaphore even if tasks are
 *!                                                         waiting. In this case, all the tasks pending
 *!                                                         will be readied.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the semaphore was deleted
 *!              OS_ERR_USE_IN_ISR      If you attempted to delete the semaphore from an ISR
 *!              OS_ERR_INVALID_OPT     An invalid option was specified
 *!              OS_ERR_TASK_WAITING    One or more tasks were waiting on the semaphore
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a semaphore object.
 *!
 *! \Notes       1) This function must be used with care.  Tasks that would normally expect the
 *!                 presence of the semaphore MUST check the return code of osSemPend().
 *!              2) OSSemAccept() callers will not know that the intended semaphore has been deleted
 *!                 unless they check 'psem' to see that it's a NULL pointer.
 *!              3) This call can potentially disable interrupts for a long time.  The interrupt
 *!                 disable time is directly proportional to the number of tasks waiting on the semaphore.
 *!              4) Because ALL tasks pending on the semaphore will be readied, you MUST be careful
 *!                 in applications where the semaphore is used for mutual exclusion because the
 *!                 resource(s) will no longer be guarded by the semaphore.
 *!              5) All tasks that were waiting for the semaphore will be readied and returned an
 *!                 OS_ERR_PEND_ABORT if osSemDelete() was called with OS_DEL_ALWAYS
 */
#if OS_SEM_DEL_EN > 0u
OS_ERR osSemDelete(OS_HANDLE *pSemHandle, UINT16 opt)
{
    OS_SEM     *psem = (OS_SEM *)*pSemHandle;
    BOOL        taskPend;


#if OS_ARG_CHK_EN > 0u
    if (pSemHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... can't DELETE from an ISR
    }
    if (*pSemHandle == NULL) {          //!< Validate handle
        return OS_ERR_INVALID_HANDLE;
    }
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    if (psem->OSSemWaitList.Next != &psem->OSSemWaitList) {     //!< check wait list if it's empty.
        taskPend    = TRUE;
    } else {
        taskPend    = FALSE;
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (taskPend != FALSE) {
                OSExitCriticalSection();
                return OS_ERR_TASK_WAITING;
            }
            break;

        case OS_DEL_ALWAYS:
            break;

        default:
            OSExitCriticalSection();
            return OS_ERR_INVALID_OPT;
    }
    
    while (!LIST_IS_EMPTY(psem->OSSemWaitList)) {  //!< Ready ALL suspended tasks.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)psem, &psem->OSSemWaitList, OS_STAT_PEND_ABORT);
    }
    psem->OSSemObjHead.OSObjType = OS_OBJ_TYPE_UNUSED;
    pool_free(&osSemFreePool, psem);
    OSExitCriticalSection();
    
    if (taskPend) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}
#endif

/*!
 *! \Brief       PEND ON SEMAPHORE
 *!
 *! \Description This function waits for a semaphore.
 *!
 *! \Arguments   hSemaphore     is a handle to the semaphore.
 *!
 *!              timeout        is an optional timeout period (in clock ticks).  If non-zero, your
 *!                             task will wait for the resource up to the amount of time specified
 *!                             by this argument. If you specify 0, however, your task will wait
 *!                             forever at the specified semaphore or, until the resource becomes
 *!                             available (or the event occurs).
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and your task owns the resource
 *!                                     or, the event you are waiting for occurred.
 *!              OS_ERR_TIMEOUT         The semaphore was not received within the specified
 *!                                     'timeout'.
 *!              OS_ERR_PEND_ABORT      The wait on the semaphore was aborted.
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a semaphore object.
 *!              OS_ERR_USE_IN_ISR      If you called this function from an ISR and the result
 *!              OS_ERR_PEND_LOCKED     If you called this function when the scheduler is locked
 *!                                     would lead to a suspension.
 */
OS_ERR osSemPend(OS_HANDLE hSemaphore, UINT32 timeout)
{
    OS_SEM         *psem = (OS_SEM *)hSemaphore;
    OS_WAIT_NODE    node;
    OS_ERR          err;


#if OS_ARG_CHK_EN > 0u
    if (hSemaphore == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) {     //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }
    if (osIntNesting > 0u) {            //!< See if called from an ISR.
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u) {           //!< See if called with scheduler locked.
        return OS_ERR_PEND_LOCKED;
    }

    OSEnterCriticalSection();
    if (psem->OSSemToken > 0u) {
        psem->OSSemToken--;
        OSExitCriticalSection();
        return OS_ERR_NONE;
    }

    if (timeout == 0u) {
        OSExitCriticalSection();
        return OS_ERR_TIMEOUT;
    }

    OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)psem, &node, &psem->OSSemWaitList, timeout);    //!< Suspend current task until event or timeout occurs
    OSExitCriticalSection();
    OS_SchedulerRunNext();

    switch (node.OSWaitNodeRes) {
        case OS_STAT_PEND_OK:
             err = OS_ERR_NONE;
             break;

        case OS_STAT_PEND_ABORT:
             err = OS_ERR_PEND_ABORT;
             break;

        case OS_STAT_PEND_TO:
        default:
             err = OS_ERR_TIMEOUT;
             break;
    }
    
    return err;
}

/*!
 *! \Brief       POST TO A SEMAPHORE
 *!
 *! \Description This function signals a semaphore. You can post multiple resource at once, so if
 *!              there are several tasks pend for this semaphore, tasks will be ready by their
 *!              priority till all resource exhausted.
 *!
 *! \Arguments   hSemaphore     is a handle to the semaphore.
 *!
 *!              cnt            is the value for the semaphore count to add.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the semaphore was signaled.
 *!              OS_ERR_SEM_OVF         If the semaphore count exceeded its limit. In other words,
 *!                                     you have signaled the semaphore more often than its capable.
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a event semaphore object.
 */
OS_ERR osSemPost(OS_HANDLE hSemaphore, UINT16 cnt)
{
    OS_SEM     *psem = (OS_SEM *)hSemaphore;
    OS_ERR      err;


#if OS_ARG_CHK_EN > 0u
    if (hSemaphore == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) {     //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }
    
    if (cnt == 0u) {
        return OS_ERR_NONE;
    }

    OSEnterCriticalSection();
    if (cnt <= (65535u - psem->OSSemToken)) {    //!< Make sure semaphore will not overflow
        psem->OSSemToken += cnt;                 //!< Increment semaphore count to register event
        if (!LIST_IS_EMPTY(psem->OSSemWaitList)) {     //!< See if any tasks waiting for semaphore
            while (!LIST_IS_EMPTY(psem->OSSemWaitList)) {
                psem->OSSemToken--;                                   //!< decrement semaphore count...
                OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)psem, &psem->OSSemWaitList, OS_STAT_PEND_OK);    //!< ...and ready HPT waiting on event
            }
            OSExitCriticalSection();
            OS_SchedulerRunPrio();
        } else {
            OSExitCriticalSection();
        }
        err = OS_ERR_NONE;
    } else {                                    //!< Not change semaphore count.
        OSExitCriticalSection();
        err = OS_ERR_SEM_OVF;
    }
    
    return err;
}

/*!
 *! \Brief       ABORT WAITING ON A SEMAPHORE
 *!
 *! \Description This function aborts & readies any tasks currently waiting on a semaphore.
 *!              This function should be used to fault-abort the wait on the semaphore, rather
 *!              than to normally signal the semaphore via osSemPost().
 *!
 *! \Arguments   hSemaphore     is a handle to the semaphore.
 *!
 *! \Returns     OS_ERR_NONE            No tasks were waiting on the semaphore.
 *!              OS_ERR_PEND_ABORT      At least one task waiting on the semaphore was readied
 *!                                     and informed of the aborted wait; check return value
 *!                                     for the number of tasks whose wait on the semaphore
 *!                                     was aborted.
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a handle of a semaphore object.
 */
#if OS_SEM_PEND_ABORT_EN > 0u
OS_ERR osSemPendAbort(OS_HANDLE hSemaphore)
{
    OS_SEM     *psem = (OS_SEM *)hSemaphore;
    OS_ERR      err;


#if OS_ARG_CHK_EN > 0u
    if (hSemaphore == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    if (!LIST_IS_EMPTY(psem->OSSemWaitList)) {           //!< See if any task waiting on semaphore?
        while (!LIST_IS_EMPTY(psem->OSSemWaitList)) {    //!< Yes, ready ALL tasks waiting on semaphore
            OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)psem, &psem->OSSemWaitList, OS_STAT_PEND_ABORT);
        }
        OSExitCriticalSection();
        OS_SchedulerRunPrio();
        err = OS_ERR_PEND_ABORT;
    } else {
        OSExitCriticalSection();
        err = OS_ERR_NONE;
    }
    
    return err;
}
#endif

/*!
 *! \Brief       SET SEMAPHORE
 *!
 *! \Description This function sets the semaphore count to the value specified as an argument.
 *!              Typically, this value would be 0.
 *!              You would typically use this function when a semaphore is used as a signaling
 *!              mechanism and, you want to reset the count value.
 *!              You'd like to use osSemPendAbort() first.
 *!
 *!
 *! \Arguments   hSemaphore     is a handle to the semaphore.
 *!
 *!              cnt            is the new value for the semaphore count.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the semaphore value was set.
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE      If you didn't pass a event semaphore object.
 *!              OS_ERR_TASK_WAITING    If tasks are waiting on the semaphore.
 */
#if OS_SEM_SET_EN > 0u
OS_ERR osSemSet(OS_HANDLE hSemaphore, UINT16 cnt)
{
    OS_SEM     *psem = (OS_SEM *)hSemaphore;
    OS_ERR      err;


#if OS_ARG_CHK_EN > 0u
    if (hSemaphore == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    err = OS_ERR_NONE;
    OSEnterCriticalSection();
    if (!LIST_IS_EMPTY(psem->OSSemWaitList)) {       //!< See if task(s) waiting?
        err = OS_ERR_TASK_WAITING;
    } else {
        psem->OSSemToken = cnt;                           //!< No, OK to set the value
    }
    OSExitCriticalSection();
    
    return err;
}
#endif

/*!
 *! \Brief       QUERY A SEMAPHORE
 *!
 *! \Description This function obtains information about a semaphore
 *!
 *! \Arguments   hSemaphore     is a handle to the semaphore.
 *!
 *!              pInfo          is a pointer to a structure that will contain information about the
 *!                             semaphore.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the message was sent
 *!              OS_ERR_INVALID_HANDLE  If 'hSemaphore' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a event semaphore object.
 *!              OS_ERR_PDATA_NULL      If 'pInfo' is a NULL pointer
 */
#if OS_SEM_QUERY_EN > 0u
OS_ERR osSemQuery(OS_HANDLE hSemaphore, OS_SEM_INFO *pInfo)
{
    OS_SEM     *psem = (OS_SEM *)hSemaphore;


#if OS_ARG_CHK_EN > 0u
    if (hSemaphore == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (OS_OBJ_TYPE_GET(psem->OSSemObjHead.OSObjType) != OS_OBJ_TYPE_SEM) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    pInfo->OSCnt = psem->OSSemToken;    //!< Get semaphore count
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}
#endif      //!< #if OS_SEM_QUERY_EN > 0u

#endif      //!< #if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)
