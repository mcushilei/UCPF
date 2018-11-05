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


//! \note do not move this pre-processor statement to other places
#define  __OS_MUTEX_C__


/*============================ INCLUDES ======================================*/
#include ".\os.h"

#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES >0u)

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

//! How does task own mutex? from time zone:
//! overlap enabled:
//!                         .....
//!                 |--------- mutex 3 ---------|
//!       |------------------ mutex 2 -------------|
//!     |------------------ mutex 1 ----------------------|
//! |------------------ mutex 0 -----------------------------|   ...
//! 
//! overlap disabled:
//! |--- mutex 0 ---|   |------ mutex 1 ------|         |--- mutex 2 ---| ...
//!

/*!
 *! \Brief       CREATE A MUTEX
 *!
 *! \Description This function creates a mutex.
 *!
 *! \Arguments   pMutexHandle  is a pointer to the handle to the event control block associated
 *!                            with the desired mutex.
 *!
 *!              ceilingPrio   ceiling priority of the mutex. You can disable this function of
 *!                            the mutex by setting this value to OS_TASK_LOWEST_PRIO.
 *!
 *! \Returns     OS_ERR_NONE            If the call was successful.
 *!              OS_ERR_USE_IN_ISR      If you attempted to create a mutex from an ISR.
 *!              OS_ERR_INVALID_HANDLE  If 'pMutexHandle' is a invalid handle.
 *!              OS_ERR_OBJ_DEPLETED    If No more objects available.
 */
OS_ERR osMutexCreate(OS_HANDLE *pMutexHandle, UINT8 ceilingPrio)
{
    OS_MUTEX   *pmutex;
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pMutexHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#if OS_TASK_LOWEST_PRIO < 255u
    if (ceilingPrio > OS_TASK_LOWEST_PRIO) {    //!< Make sure priority is within allowable range.
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif
    if (osIntNesting > 0u) {                    //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;               //!< ... Should not create object from an ISR.
    }

    //! malloc an ECB from pool.
    OSEnterCriticalSection(cpu_sr);
    pmutex = OS_ObjPoolNew(&osMutexFreeList);
    if (pmutex == NULL) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection(cpu_sr);

    //! Set object type.
    //! init mutex property.
    //! init mutex wait list head.
    pmutex->OSMutexObjHeader.OSObjType =  OS_OBJ_TYPE_SET(OS_OBJ_TYPE_MUTEX)
                                        | OS_OBJ_TYPE_WAITABLE_MSK
                                        | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_PRIO_LIST);
    pmutex->OSMutexCnt          = 0u;
    pmutex->OSMutexCeilingPrio  = ceilingPrio;
    pmutex->OSMutexOwnerPrio    = 0u;
#if OS_MUTEX_OVERLAP_EN > 0u
    os_list_init_head(&pmutex->OSMutexOvlpList);
#endif
    pmutex->OSMutexOwnerTCB     = NULL;
    os_list_init_head(&pmutex->OSMutexWaitList);
    
    *pMutexHandle = pmutex;
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       DELETE A MUTEX
 *!
 *! \Description This function deletes a mutex and readies all tasks pending 
 *!              on the it.
 *!
 *! \Arguments   hMutex         is a handle to the event control block associated with the
 *!                             desired mutex.
 *!
 *!              opt            determines delete options as follows:
 *!                             opt == OS_DEL_NOT_IN_USE    Delete mutex ONLY if no task pending
 *!                             opt == OS_DEL_ALWAYS        Deletes the mutex if tasks are waiting.
 *!                                                         In this case, all the tasks pending will
 *!                                                         be readied.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the mutex was deleted
 *!              OS_ERR_INVALID_HANDLE  If 'hMutex' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a event mutex object.
 *!              OS_ERR_USE_IN_ISR      If you attempted to delete the MUTEX from an ISR
 *!              OS_ERR_INVALID_OPT     An invalid option was specified
 *!              OS_ERR_TASK_WAITING    One or more tasks were waiting on the mutex
 *!              OS_ERR_MUTEX_IS_OWNED  Mutex is owned by a task.
 *!
 *! \Notes       1) This function must be used with care.  Tasks that would normally expect the
 *!                 presence of the mutex MUST check the return code of osMutexPend().
 *!              2) This call can potentially disable interrupts for a long time.  The interrupt
 *!                 disable time is directly proportional to the number of tasks waiting on the mutex.
 *!              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful
 *!                 because the resource(s) will no longer be guarded by the mutex.
 *!              4) IMPORTANT: In the 'OS_DEL_ALWAYS' case, we assume that the owner of the Mutex
 *!                            (if there is one) is ready-to-run and is thus NOT pending on another
 *!                            kernel object or has delayed itself.  In other words, if a task owns
 *!                            the mutex being deleted, that task will be made ready-to-run at its
 *!                            original priority.
 */
#if OS_MUTEX_DEL_EN > 0u
OS_ERR osMutexDelete(OS_HANDLE *pMutexHandle, UINT8 opt)
{
    OS_MUTEX   *pmutex;
    OS_TCB     *powner;
    BOOL        taskPend;
    BOOL        taskSched = FALSE;
#if OS_CRITICAL_METHOD == 3u                //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pMutexHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {                //!< Should not delete object from an ISR.
        return OS_ERR_USE_IN_ISR;
    }
    if (*pMutexHandle == NULL) {            //!< Validate handle.
        return OS_ERR_INVALID_HANDLE;
    }
    pmutex = (OS_MUTEX *)*pMutexHandle;
    if (OS_OBJ_TYPE_GET(pmutex->OSMutexObjHeader.OSObjType) != OS_OBJ_TYPE_MUTEX) {   //!< Validate event block type.
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (pmutex->OSMutexWaitList.Next != &pmutex->OSMutexWaitList) { //!< See if any tasks suspend for this mutex.
        taskPend    = TRUE;                                         //!< Yes
        taskSched   = TRUE;
    } else {
        taskPend    = FALSE;                                        //!< No
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (taskPend != FALSE) {
                OSExitCriticalSection(cpu_sr);
                return OS_ERR_TASK_WAITING;
            }
            if (pmutex->OSMutexOwnerTCB != NULL) {
                OSExitCriticalSection(cpu_sr);
                return OS_ERR_MUTEX_IS_OWNED;
            }
            break;

        case OS_DEL_ALWAYS:
            break;

        default:
             OSExitCriticalSection(cpu_sr);
             return OS_ERR_INVALID_OPT;
    }
    
    //! remove the owner if there is any.
    powner = pmutex->OSMutexOwnerTCB;
    if (powner != NULL) {
#if OS_MUTEX_OVERLAP_EN > 0u
        os_list_del(&pmutex->OSMutexOvlpList);
#else
        powner->OSTCBOwnMutex = NULL;
#endif
        if (pmutex->OSMutexOwnerPrio != powner->OSTCBPrio) {        //!< If this task's prio has been changed,
            OS_ChangeTaskPrio(powner, pmutex->OSMutexOwnerPrio);    //!< Yes, restore task's prio.
            taskSched = TRUE;
        }
    }
    
    //! Ready ALL tasks are suspending for this mutex.
    while (pmutex->OSMutexWaitList.Next != &pmutex->OSMutexWaitList) {
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pmutex, OS_STAT_PEND_ABORT);
    }
    
    pmutex->OSMutexObjHeader.OSObjType = OS_OBJ_TYPE_UNUSED;
    pmutex->OSMutexCnt          = 0u;
    pmutex->OSMutexCeilingPrio  = 0u;
    pmutex->OSMutexOwnerPrio    = 0u;
    pmutex->OSMutexOwnerTCB     = NULL;
    OS_ObjPoolFree(&osMutexFreeList, pmutex);
    OSExitCriticalSection(cpu_sr);
    
    if (taskSched) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}
#endif

/*!
 *! \Brief       PEND ON MUTEX
 *!
 *! \Description This function waits for a mutex.
 *!
 *! \Arguments   hMutex        is a handle to the mutex.
 *!
 *!              timeout       is an optional timeout period (in clock ticks).  If non-zero, your
 *!                            task will wait for the resource up to the amount of time specified
 *!                            by this argument. If you specify OS_INFINITE, however, your task
 *!                            will wait forever at the specified mutex or, until the resource
 *!                            becomes available. If you specify 0, however, your task will NOT wait
 *!                            and return OS_ERR_TIMEOUT if the specified mutex is not avalible.
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and your task owns the mutex.
 *!              OS_ERR_INVALID_HANDLE  If 'hMutex' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If object was not a mutex.
 *!              OS_ERR_USE_IN_ISR      If this function was called from an ISR and the result
 *!              OS_ERR_PEND_LOCKED     If this function was called when the scheduler is locked.
 *!              OS_ERR_TIMEOUT         The mutex was not available within the specified 'timeout'.
 *!              OS_ERR_PEND_ABORT      The wait on the mutex was aborted.
 *!                                     would lead to a suspension.
 *!              OS_ERR_MUTEX_OVERFLOW  The task has recursively use the same mutex too many times.
 *!              OS_ERR_OVERLAP_MUTEX   Current task try getting the mutex while it has gotten another one.
 *!                                     User should release the mutex before try getting it again.
 *!
 *! \Notes       1) The task that has owned one mutex could NOT try to own other one. It's say it
 *!                 could not be overlapped if OS_MUTEX_OVERLAP_EN = 0.
 *!              2) If OS_MUTEX_OVERLAP_EN != 0, 
 */
OS_ERR osMutexPend(OS_HANDLE hMutex, UINT32 timeout)
{
    OS_MUTEX       *pmutex = (OS_MUTEX *)hMutex;
    OS_WAIT_NODE    node;
    OS_ERR          err;
    UINT8           prio;
#if OS_CRITICAL_METHOD == 3u                //!< Allocate storage for CPU status register
    CPU_SR       cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hMutex == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {                //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;           //!< ... mutex can't be used from an ISR.
    }
    if (osLockNesting > 0u) {               //!< See if called with scheduler locked ...
        return OS_ERR_PEND_LOCKED;          //!< ... can't PEND when locked
    }
    if (OS_OBJ_TYPE_GET(pmutex->OSMutexObjHeader.OSObjType) != OS_OBJ_TYPE_MUTEX) {  //!< Validate object's type.
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (pmutex->OSMutexOwnerTCB == NULL) {              //!< Has mutex been possessed by any other task?...
#if OS_MUTEX_OVERLAP_EN > 0u                            //!  ...No.
        os_list_add(&pmutex->OSMutexOvlpList, osTCBCur->OSTCBOwnMutexList.Prev);
#else
        if (osTCBCur->OSTCBOwnMutex != NULL) {
            OSExitCriticalSection(cpu_sr);
            return OS_ERR_OVERLAP_MUTEX;
        }
        osTCBCur->OSTCBOwnMutex  = pmutex;
#endif
        pmutex->OSMutexCnt       = 0u;
        pmutex->OSMutexOwnerPrio = osTCBCur->OSTCBPrio;
        pmutex->OSMutexOwnerTCB  = osTCBCur;
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }
    
    if (pmutex->OSMutexOwnerTCB == osTCBCur) {          //!< Is mutex owned by CURRENT task?...
        if (pmutex->OSMutexCnt < 255u) {                //! ...Yes.
            pmutex->OSMutexCnt++;
            OSExitCriticalSection(cpu_sr);
            return OS_ERR_NONE;
        } else {
            OSExitCriticalSection(cpu_sr);              //!< this should be fatal error!
            return OS_ERR_MUTEX_OVERFLOW;
        }
    }
    
    if (timeout == 0u) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TIMEOUT;
    }

                                                            //! anti prio-reserve mechanism: ceiling AND bubbling
    if (pmutex->OSMutexCeilingPrio < osTCBCur->OSTCBPrio) { //! compare and get the highest prio of ...
        prio = pmutex->OSMutexCeilingPrio;
    } else {
        prio = osTCBCur->OSTCBPrio;
    }
    if (pmutex->OSMutexOwnerTCB->OSTCBPrio > prio) {        //!< Does owner have a lower priority?
        OS_ChangeTaskPrio(pmutex->OSMutexOwnerTCB, prio);   //!< Yes. Rise owner's priority.
    }

    OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)pmutex, &node, timeout);   //!< Suspend current task.
    OSExitCriticalSection(cpu_sr);
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
 *! \Brief       POST TO A MUTEX
 *!
 *! \Description This function signals a mutex
 *!
 *! \Arguments   hMutex         is a handle to the mutex.
 *!
 *! \Returns     OS_ERR_NONE                The call was successful and the mutex was signaled.
 *!              OS_ERR_USE_IN_ISR          Attempted to post from an ISR (not valid for MUTEXes)
 *!              OS_ERR_INVALID_HANDLE      If 'hMutex' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE            If you didn't pass a event mutex object.
 *!              OS_ERR_NOT_MUTEX_OWNER     The task that did the post is NOT the owner of the MUTEX.
 *!
 *! \Notes       1) The mutex can ONLY be released by it's owner.
 */
OS_ERR osMutexPost(OS_HANDLE hMutex)
{
    OS_MUTEX   *pmutex = (OS_MUTEX *)hMutex;
    OS_TCB     *ptcb;
    BOOL        taskSched = FALSE;
#if OS_CRITICAL_METHOD == 3u                //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hMutex == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {                //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;           //!< ... mutex can't be used from an ISR.
    }
    if (OS_OBJ_TYPE_GET(pmutex->OSMutexObjHeader.OSObjType) != OS_OBJ_TYPE_MUTEX) {   //!< Validate event block type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (osTCBCur != pmutex->OSMutexOwnerTCB) {      //!< See if the mutex owned by current task.
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NOT_MUTEX_OWNER;
    }
#if OS_MUTEX_OVERLAP_EN > 0u
#else
    osTCBCur->OSTCBOwnMutex = NULL;
#endif
    
    if (pmutex->OSMutexCnt != 0u) {                 //!< Does current task own this mutex recursively?
        pmutex->OSMutexCnt--;
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }
    
    if (pmutex->OSMutexOwnerPrio != osTCBCur->OSTCBPrio) {      //!< If this task's prio has been changed...
        OS_ChangeTaskPrio(osTCBCur, pmutex->OSMutexOwnerPrio);  //!< ... Yes, restore task's prio.
        taskSched = TRUE;
    }
#if OS_MUTEX_OVERLAP_EN > 0u
    os_list_del(&pmutex->OSMutexOvlpList);
#else
    osTCBCur->OSTCBOwnMutex = NULL;
#endif
    
    if (pmutex->OSMutexWaitList.Next != &pmutex->OSMutexWaitList) {                 //!< Is any task waiting for the mutex?...
        ptcb = OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pmutex, OS_STAT_PEND_OK);   //!< ... Yes, Make the HPT waiting for the mutex ready
        pmutex->OSMutexOwnerTCB  = ptcb;
        pmutex->OSMutexOwnerPrio = ptcb->OSTCBPrio;
#if OS_MUTEX_OVERLAP_EN > 0u
        os_list_add(&pmutex->OSMutexOvlpList, ptcb->OSTCBOwnMutexList.Prev);
#else
        ptcb->OSTCBOwnMutex      = pmutex;
#endif
        taskSched = TRUE;
    } else {                                                                        //!< ... No.
        pmutex->OSMutexOwnerTCB  = NULL;
        pmutex->OSMutexOwnerPrio = 0u;
    }
    OSExitCriticalSection(cpu_sr);
    
    if (taskSched) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       QUERY A MUTEX
 *!
 *! \Description This function obtains information about a mutex
 *!
 *! \Arguments   hMutex         is a handle to the mutex.
 *!
 *!              pInfo          is a pointer to a structure that will contain information about
 *!                             the mutex
 *!
 *! \Returns     OS_ERR_NONE            The call was successful and the message was sent
 *!              OS_ERR_INVALID_HANDLE  If 'hMutex' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a mutex object.
 *!              OS_ERR_PDATA_NULL      If 'pInfo' is a NULL pointer
 */
#if OS_MUTEX_QUERY_EN > 0u
OS_ERR osMutexQuery(OS_HANDLE hMutex, OS_MUTEX_INFO *pInfo)
{
    OS_MUTEX       *pmutex = (OS_MUTEX *)hMutex;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR       cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hMutex == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (OS_OBJ_TYPE_GET(pmutex->OSMutexObjHeader.OSObjType) != OS_OBJ_TYPE_MUTEX) {  //!< Validate objext's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (pmutex->OSMutexOwnerTCB == NULL) {                  //!< Does any task own this mutex?
        pInfo->OSOwnerTCB    = NULL;
        pInfo->OSOwnerPrio   = 0u;
    } else {
        pInfo->OSOwnerTCB    = pmutex->OSMutexOwnerTCB;
        pInfo->OSOwnerPrio   = pmutex->OSMutexOwnerPrio;
    }
    pInfo->OSCeilingPrio     = pmutex->OSMutexCeilingPrio;
    pInfo->OSWaitList        = pmutex->OSMutexWaitList;     //!< Copy wait list
    OSExitCriticalSection(cpu_sr);
    
    return OS_ERR_NONE;
}
#endif      //!< #if OS_MUTEX_QUERY_EN > 0u

#endif      //!< #if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES >0u)
