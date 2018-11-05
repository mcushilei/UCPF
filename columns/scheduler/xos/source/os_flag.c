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
#define  __OS_FLAG_C__

/*============================ INCLUDES ======================================*/
#include ".\os.h"

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)

/*============================ MACROS ========================================*/
#define OS_FLAG_STATUS_BIT              (0x01u)
#define OS_FLAG_MANUAL_RESET_BIT        (0x80u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*!
 *! \Brief       CREATE AN EVENT FLAG
 *!
 *! \Description This function is called to create an flag object.
 *!
 *! \Arguments   pFlagHandle    Pointer to the handle of flag.
 *!
 *!              initValue      Initial value of flag. If this value is TRUE, the initial state of the
 *!                             flag is SET.
 *!
 *!              manualReset    If this flag is auto reset. It can be:
 *!                             manualReset == TRUE     The flag will not be reset until you reset it
 *!                                                     by calling osFlagReset. 
 *!                             manualReset == FALSE    The flag will be reset once a waiting task has
 *!                                                     been release. 
 *!
 *! \Returns     OS_ERR_NONE            If the call was successful.
 *!              OS_ERR_USE_IN_ISR      If you attempted to create an Event Flag from an ISR.
 *!              OS_ERR_OBJ_DEPLETED    If there are no more event flag control block
 */
OS_ERR osFlagCreate(OS_HANDLE *pFlagHandle, BOOL initValue, BOOL manualReset)
{
    OS_FLAG    *pflag;
    UINT16      flags = 0;
#if OS_CRITICAL_METHOD == 3u
    CPU_SR   cpu_sr = 0u;            //!< Allocate storage for CPU status register
#endif


#if OS_ARG_CHK_EN > 0u
    if (pFlagHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }

    if (initValue != FALSE) {
        flags |= OS_FLAG_STATUS_BIT;
    }
    if (manualReset == FALSE) {
        flags |= OS_FLAG_MANUAL_RESET_BIT;
    }
    
    //! malloc ECB from pool.
    OSEnterCriticalSection(cpu_sr);
    pflag = OS_ObjPoolNew(&osFlagFreeList);
    if (pflag == NULL) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection(cpu_sr);

    //! set object type.
    //! initial flag's property.
    //! initial flag's wait list.
    pflag->OSFlagObjHeader.OSObjType      = OS_OBJ_TYPE_SET(OS_OBJ_TYPE_FLAG)
                          | OS_OBJ_TYPE_WAITABLE_MSK
                          | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_LIST);
    pflag->OSFlagFlags    = flags;
    os_list_init_head(&pflag->OSFlagWaitList);
    
    *pFlagHandle = pflag;
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       DELETE A FLAG
 *!
 *! \Description This function deletes a flag and readies all tasks pending on it.
 *!
 *! \Arguments   pFlagHandle    is a pointer to the handle of flag.
 *!
 *!              opt            determines delete options, one of follows:
 *!                             opt == OS_DEL_NOT_IN_USE    Deletes the flag ONLY if no task pending
 *!                                                         for it.
 *!                             opt == OS_DEL_ALWAYS        Deletes the flag even if tasks are pending 
 *!                                                         for it.  In this case, all the pending tasks
 *!                                                         will be readied.
 *!
 *! \Returns     OS_ERR_NONE            The flag was deleted successfully.
 *!              OS_ERR_USE_IN_ISR      If you attempted to delete the flag from an ISR.
 *!              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a flag object.
 *!              OS_ERR_INVALID_OPT     An invalid option was specified.
 *!              OS_ERR_TASK_WAITING    One or more tasks were waiting on the flag.
 *!
 *!
 *! \Notes       1) This function must be used with care. Tasks that would normally expect
 *!                 the presence of the flag MUST check the return code of osFlagPend().
 *!              2) This call can potentially disable interrupts for a long time. The interrupt
 *!                 disable time is directly proportional to the number of tasks waiting on
 *!                 the flag.
 *!              3) All tasks that were waiting for the event flag will be readied and returned an
 *!                 OS_ERR_PEND_ABORT if osFlagDelete() was called with OS_DEL_ALWAYS.
 */
#if OS_FLAG_DEL_EN > 0u
OS_ERR osFlagDelete(OS_HANDLE *pFlagHandle, UINT8 opt)
{
    OS_FLAG    *pflag;
    BOOL        taskPend;
    BOOL        taskSched = FALSE;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pFlagHandle == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< Can't DELETE from an ISR
        return OS_ERR_USE_IN_ISR;
    }
    if (*pFlagHandle == NULL) {         //!< Validate handle
        return OS_ERR_INVALID_HANDLE;
    }
    pflag = (OS_FLAG *)*pFlagHandle;
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (pflag->OSFlagWaitList.Next != &pflag->OSFlagWaitList) {     //!< See if any tasks taskPend on this flag...
        taskPend    = TRUE;                                         //!< ...Yes
        taskSched   = TRUE;
    } else {
        taskPend    = FALSE;                                        //!< ...No
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (taskPend != FALSE) {
                OSExitCriticalSection(cpu_sr);
                return OS_ERR_TASK_WAITING;
            }
            break;

        case OS_DEL_ALWAYS:
            break;

        default:
             OSExitCriticalSection(cpu_sr);
             return OS_ERR_INVALID_OPT;
    }
    
    while (pflag->OSFlagWaitList.Next != &pflag->OSFlagWaitList) {  //!< Ready ALL tasks task pend for this flag.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pflag, OS_STAT_PEND_ABORT);
    }
    pflag->OSFlagObjHeader.OSObjType      = OS_OBJ_TYPE_UNUSED;
    pflag->OSFlagFlags    = 0u;
    OS_ObjPoolFree(&osFlagFreeList, pflag);
    OSExitCriticalSection(cpu_sr);
    
    if (taskSched) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}
#endif

/*!
 *! \Brief       WAIT ON A FLAG
 *!
 *! \Description This function is called to wait for a flag to be set.
 *!
 *! \Arguments   hFlag          is a handle to the desired flag.
 *!
 *!              timeout        is an optional timeout (in clock ticks) that your task will wait for
 *!                             the desired bit combination.  If you specify OS_INFINITE, this function
 *!                             will not return until the flag is set.
 *!                             If you specify 0, this call will return immediately even if the flag
 *!                             was not set.
 *!
 *!  \Returns    OS_ERR_NONE            The flag have been set within the specified 'timeout'.
 *!              OS_ERR_USE_IN_ISR      If you tried to PEND from an ISR.
 *!              OS_ERR_PEND_LOCKED     If you called this function when the scheduler is locked.
 *!              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE      If you didn't pass a flag object.
 *!              OS_ERR_TIMEOUT         The flag have not been set in the specified 'timeout'.
 *!              OS_ERR_PEND_ABORT      The wait on the flag was aborted.
 */
OS_ERR osFlagPend(OS_HANDLE hFlag, UINT32 timeout)
{
    OS_FLAG        *pflag = (OS_FLAG *)hFlag;
    OS_WAIT_NODE    node;
    UINT16          ready;
    UINT16          consume;
    OS_ERR          err;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR       cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< Can't PEND from an ISR
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u) {           //!< Can't PEND when locked
        return OS_ERR_PEND_LOCKED;
    }
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    consume = pflag->OSFlagFlags & OS_FLAG_MANUAL_RESET_BIT;
    ready   = pflag->OSFlagFlags & OS_FLAG_STATUS_BIT;
    if (ready != 0u) {                          //!< See if flag has benn set.
        if (consume != 0u) {                    //!< Yes. See if we need to consume the flags.
            pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;       //!< Yes. Reset the flag.
        }
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }

    if (timeout == 0u) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TIMEOUT;
    }

    OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)pflag, &node, timeout);    //!< Suspend task until event occur or timeout
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
 *! \Brief       SET A FLAG
 *!
 *! \Description This function is called to set a flag.
 *!
 *! \Arguments   hFlag          is a handle to the desired flag.
 *!
 *! \Returns     OS_ERR_NONE            The call was successfull
 *!              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE        If you didn't pass a event flag object.
 *!
 *! \Notes       1) The execution time of this function depends on the number of tasks waiting on
 *!                 the event flag.
 *!              2) The amount of time interrupts are DISABLED depends on the number of tasks
 *!                 waiting on the event flag.
 */
OS_ERR osFlagSet(OS_HANDLE hFlag)
{
    OS_FLAG      *pflag = (OS_FLAG *)hFlag;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR     cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    pflag->OSFlagFlags |= OS_FLAG_STATUS_BIT;                   //!< Set the flags.
    if (pflag->OSFlagWaitList.Next != &pflag->OSFlagWaitList) {         //!< See if any task is waiting for this flag.
        while (pflag->OSFlagWaitList.Next != &pflag->OSFlagWaitList) {  //!< Yes, Ready ALL tasks waiting for this flag.
            OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pflag, OS_STAT_PEND_OK);
        }
        if (pflag->OSFlagFlags & OS_FLAG_MANUAL_RESET_BIT) {    //!< Is this a auto-reset flag?
            pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;          //!< Yes, Reset the flag.
        }
    }
    OSExitCriticalSection(cpu_sr);
    OS_SchedulerRunPrio();
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       RESET A FLAG
 *!
 *! \Description This function is called to reset a flag.
 *!
 *! \Arguments   hFlag          is a handle to the desired flag.
 *!
 *! \Returns     OS_ERR_NONE            The call was successfull
 *!              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE      If you didn't pass a flag object.
 */
OS_ERR osFlagReset(OS_HANDLE hFlag)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;       //!< Reset the flags
    OSExitCriticalSection(cpu_sr);
    
    return OS_ERR_NONE;
}

/*!
 *! \Brief       QUERY A FLAG
 *!
 *! \Description This function is used to get the information of the flag. This is intent on statistic use, do not
 *!              use in your application.
 *!
 *! \Arguments   hFlag          is a handle to the desired flag.
 *!              pInfo          a pointer to a OS_FLAG_INFO struct to store the information.
 *!
 *! \Returns     OS_ERR_NONE            The call was successfull
 *!              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *!              OS_ERR_OBJ_TYPE      If you didn't pass a event flag object.
 */
#if OS_FLAG_QUERY_EN > 0u
OS_ERR osFlagQuery(OS_HANDLE hFlag, OS_FLAG_INFO *pInfo)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
    UINT16      flag;
#if OS_CRITICAL_METHOD == 3u            //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    flag = pflag->OSFlagFlags;
    if (flag & OS_FLAG_MANUAL_RESET_BIT) {      //!< Is this a manual-rest flag?
        pInfo->OSFlagManualReset = FALSE;
    } else {
        pInfo->OSFlagManualReset = TRUE;
    }
    if (flag & OS_FLAG_MANUAL_RESET_BIT) {      //!< Is this flag set?
        pInfo->OSFlagStatus = TRUE;
    } else {
        pInfo->OSFlagStatus = FALSE;
    }
    pInfo->OSWaitList = pflag->OSFlagWaitList;  //!< Copy wait list
    OSExitCriticalSection(cpu_sr);
    
    return OS_ERR_NONE;
}
#endif

#endif      //!< #if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
