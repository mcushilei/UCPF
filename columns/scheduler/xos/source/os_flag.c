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

#if OS_FLAG_EN > 0u

/*============================ MACROS ========================================*/
#define OS_FLAG_STATUS_BIT              (0x01u)
#define OS_FLAG_MANUAL_RESET_BIT        (0x80u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*
 *  \brief      CREATE AN EVENT FLAG
 * 
 *  \remark     This function is called to create an flag object.
 * 
 *  \param      pFlagHandle     Pointer to the handle of flag.
 * 
 *              initValue       Initial value of flag. If this value is TRUE, the initial state of the
 *                              flag is SET.
 * 
 *              manualReset     If this flag is auto reset. It can be:
 *                              manualReset == TRUE     The flag will not be reset until you reset it
 *                                                      by calling osFlagReset. 
 *                              manualReset == FALSE    The flag will be reset once a waiting task has
 *                                                      been release. 
 * 
 *  \return     OS_ERR_NONE            If the call was successful.
 *              OS_ERR_USE_IN_ISR      If you attempted to create an Event Flag from an ISR.
 *              OS_ERR_OBJ_DEPLETED    If there are no more event flag control block
 */
OS_ERR osFlagCreate(OS_HANDLE *pFlagHandle, BOOL initValue, BOOL manualReset)
{
    OS_FLAG    *pflag;
    UINT16      flags = 0;


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
    OSEnterCriticalSection();
    pflag = pool_new(&osFlagFreePool);
    if (pflag == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection();

    //! set object type.
    //! initial flag's property.
    //! initial flag's wait list.
    pflag->OSFlagObjHeader.OSObjType = OS_OBJ_TYPE_SET(OS_OBJ_TYPE_FLAG)
                                     | OS_OBJ_TYPE_WAITABLE_MSK
                                     | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_LIST);
    pflag->OSFlagFlags    = flags;
    list_init(&pflag->OSFlagWaitList);
    
    *pFlagHandle = pflag;
    
    return OS_ERR_NONE;
}

/*
 *  \brief      DELETE A FLAG
 * 
 *  \remark     This function deletes a flag and readies all tasks pending on it.
 * 
 *  \param      pFlagHandle     is a pointer to the handle of flag.
 * 
 *              opt             determines delete options, one of follows:
 *                              opt == OS_DEL_NOT_IN_USE    Deletes the flag ONLY if no task pending
 *                                                          for it.
 *                              opt == OS_DEL_ALWAYS        Deletes the flag even if tasks are pending 
 *                                                          for it.  In this case, all the pending tasks
 *                                                          will be readied.
 * 
 *  \return     OS_ERR_NONE             The flag was deleted successfully.
 *              OS_ERR_USE_IN_ISR       If you attempted to delete the flag from an ISR.
 *              OS_ERR_INVALID_HANDLE   If 'hFlag' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a flag object.
 *              OS_ERR_INVALID_OPT      An invalid option was specified.
 *              OS_ERR_DELETE_IN_USE    One or more tasks were waiting on the flag.
 * 
 * 
 *  \note       1) This function must be used with care. Tasks that would normally expect
 *                 the presence of the flag MUST check the return code of osFlagPend().
 *              2) This call can potentially disable interrupts for a long time. The interrupt
 *                 disable time is directly proportional to the number of tasks waiting on
 *                 the flag.
 *              3) All tasks that were waiting for the event flag will be readied and returned an
 *                 OS_ERR_PEND_ABORT if osFlagDelete() was called with OS_DEL_ALWAYS.
 */
#if OS_FLAG_DEL_EN > 0u
OS_ERR osFlagDelete(OS_HANDLE hFlag, UINT16 opt)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
    BOOL        taskPend;
    BOOL        taskSched = FALSE;


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< Can't DELETE from an ISR
        return OS_ERR_USE_IN_ISR;
    }
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    if (!LIST_IS_EMPTY(pflag->OSFlagWaitList)) {    //!< Is any tasks taskPend on this flag...
        taskPend    = TRUE;                         //!< ...Yes
        taskSched   = TRUE;
    } else {
        taskPend    = FALSE;                        //!< ...No
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (taskPend != FALSE) {
                OSExitCriticalSection();
                return OS_ERR_DELETE_IN_USE;
            }
            break;

        case OS_DEL_ALWAYS:
            break;

        default:
             OSExitCriticalSection();
             return OS_ERR_INVALID_OPT;
    }
    
    while (!LIST_IS_EMPTY(pflag->OSFlagWaitList)) {  //!< Ready ALL tasks task pend for this flag.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pflag, &pflag->OSFlagWaitList, OS_STAT_PEND_ABORT);
    }
    pflag->OSFlagObjHeader.OSObjType      = OS_OBJ_TYPE_UNUSED;
    pflag->OSFlagFlags    = 0u;
    pool_free(&osFlagFreePool, pflag);
    OSExitCriticalSection();
    
    if (taskSched) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}
#endif

/*
 *  \brief      WAIT ON A FLAG
 * 
 *  \remark     This function is called to wait for a flag to be set.
 * 
 *  \param      hFlag       is a handle to the desired flag.
 * 
 *              timeout     is an optional timeout (in clock ticks) that your task will wait for
 *                          the desired bit combination.  If you specify OS_INFINITE, this function
 *                          will not return until the flag is set.
 *                          If you specify 0, this call will return immediately even if the flag
 *                          was not set.
 * 
 *   \return    OS_ERR_NONE             The flag have been set within the specified 'timeout'.
 *              OS_ERR_USE_IN_ISR       If you tried to PEND from an ISR.
 *              OS_ERR_PEND_LOCKED      If you called this function when the scheduler is locked.
 *              OS_ERR_INVALID_HANDLE   If 'hFlag' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a flag object.
 *              OS_ERR_TIMEOUT          The flag have not been set in the specified 'timeout'.
 *              OS_ERR_PEND_ABORT       The wait on the flag was aborted.
 */
OS_ERR osFlagPend(OS_HANDLE hFlag, UINT32 timeout)
{
    OS_FLAG        *pflag = (OS_FLAG *)hFlag;
    OS_WAIT_NODE    node;
    UINT16          ready;
    UINT16          consume;
    OS_ERR          err;


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {                    //!< Can't PEND from an ISR
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u && timeout != 0u) {  //!< Can't PEND when locked
        return OS_ERR_PEND_LOCKED;
    }
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    consume = pflag->OSFlagFlags & OS_FLAG_MANUAL_RESET_BIT;
    ready   = pflag->OSFlagFlags & OS_FLAG_STATUS_BIT;
    if (ready != 0u) {                          //!< See if flag has benn set.
        if (consume != 0u) {                    //!< Yes. See if we need to consume the flags.
            pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;       //!< Yes. Reset the flag.
        }
        OSExitCriticalSection();
        return OS_ERR_NONE;
    }

    if (timeout == 0u) {
        OSExitCriticalSection();
        return OS_ERR_TIMEOUT;
    }

    OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)pflag, &node, &pflag->OSFlagWaitList, timeout);    //!< Suspend task until event occur or timeout
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

/*
 *  \brief      SET A FLAG
 * 
 *  \remark     This function is called to set a flag.
 * 
 *  \param      hFlag          is a handle to the desired flag.
 * 
 *  \return     OS_ERR_NONE            The call was successfull
 *              OS_ERR_INVALID_HANDLE  If 'hFlag' is an invalid handle.
 *              OS_ERR_OBJ_TYPE        If you didn't pass a event flag object.
 * 
 *  \note       1) The execution time of this function depends on the number of tasks waiting on
 *                 the event flag.
 *              2) The amount of time interrupts are DISABLED depends on the number of tasks
 *                 waiting on the event flag.
 */
OS_ERR osFlagSet(OS_HANDLE hFlag)
{
    OS_FLAG      *pflag = (OS_FLAG *)hFlag;


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    pflag->OSFlagFlags |= OS_FLAG_STATUS_BIT;               //!< Set the flags.
    if (!LIST_IS_EMPTY(pflag->OSFlagWaitList)) {            //!< See if any task is waiting for this flag.
        while (!LIST_IS_EMPTY(pflag->OSFlagWaitList)) {     //!< Yes, Ready ALL tasks waiting for this flag.
            OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pflag, &pflag->OSFlagWaitList, OS_STAT_PEND_OK);
        }
        if (pflag->OSFlagFlags & OS_FLAG_MANUAL_RESET_BIT) {    //!< Is this a auto-reset flag?
            pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;          //!< Yes, Reset the flag.
        }
    }
    OSExitCriticalSection();
    OS_SchedulerRunPrio();
    
    return OS_ERR_NONE;
}

/*
 *  \brief      RESET A FLAG
 * 
 *  \remark     This function is called to reset a flag.
 * 
 *  \param      hFlag          is a handle to the desired flag.
 * 
 *  \return     OS_ERR_NONE             The call was successfull
 *              OS_ERR_INVALID_HANDLE   If 'hFlag' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a flag object.
 */
OS_ERR osFlagReset(OS_HANDLE hFlag)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;


#if OS_ARG_CHK_EN > 0u
    if (hFlag == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (OS_OBJ_TYPE_GET(pflag->OSFlagObjHeader.OSObjType) != OS_OBJ_TYPE_FLAG) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    pflag->OSFlagFlags &= ~OS_FLAG_STATUS_BIT;       //!< Reset the flags
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}

/*
 *  \brief      QUERY A FLAG
 * 
 *  \remark     This function is used to get the information of the flag. This is intent on statistic use, do not
 *              use in your application.
 * 
 *  \param      hFlag          is a handle to the desired flag.
 *              pInfo          a pointer to a OS_FLAG_INFO struct to store the information.
 * 
 *  \return     OS_ERR_NONE             The call was successfull
 *              OS_ERR_INVALID_HANDLE   If 'hFlag' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a event flag object.
 */
#if OS_FLAG_QUERY_EN > 0u
OS_ERR osFlagQuery(OS_HANDLE hFlag, OS_FLAG_INFO *pInfo)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
    UINT16      flag;


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

    OSEnterCriticalSection();
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
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}
#endif

#endif      //!< #if OS_FLAG_EN > 0u
