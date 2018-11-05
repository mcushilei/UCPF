
#include ".\os.h"

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)

/*********************************************************************************************************
*                                        CREATE AN EVENT FLAG
*
* Description: This function is called to create an event flag group.
*
* Arguments  : pFlagHandle          Pointer to the handle of flag.
*              init                 Initial value of flag.
*              manual               
*
* Returns    : OS_ERR_NONE               if the call was successful.
*              OS_ERR_CREATE_ISR         if you attempted to create an Event Flag from an ISR.
*              OS_ERR_FLAG_DEPLETED      if there are no more event flag control block
*
* Called from: Task ONLY
*********************************************************************************************************/

OS_ERR   osFlagCreate  (OS_HANDLE  *pFlagHandle,
                        BOOL     init,
                        BOOL     manual)
{
    OS_FLAG    *pflag;
#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR   cpu_sr = 0u;                    //!< Allocate storage for CPU status register
#endif
    UINT16      flags = 0;


    if (osIntNesting > 0u) {                    //!< Should not CREATE from an ISR
        return OS_ERR_CREATE_ISR;
    }
#if OS_ARG_CHK_EN > 0u
    if (pFlagHandle == NULL) {               //!< Validate handle
        return OS_ERR_INVALID_HANDLE;
    }
#endif

    if (init != FALSE) {
        flags |= 0x01u;
    }
    if (manual == FALSE) {
        flags |= 0x80u;
    }

    OSEnterCriticalSection(cpu_sr);             //!< Get ECB from free list.
    pflag = osFlagFreeList;                         
    if (pflag == NULL) {                     //!< See if we have ECB available.
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_FLAG_DEPLETED;
    }
    osFlagFreeList = osFlagFreeList->OSFlagPtr;
    OSExitCriticalSection(cpu_sr);
    
    pflag->OSObjType      = OS_OBJ_TYPE_FLAG
                          | OS_OBJ_WAITABLE
                          | OS_OBJ_PRIO_TYPE_LIST;
    pflag->OSFlagFlags    = flags;
    pflag->OSFlagWaitList = NULL;
    *pFlagHandle = pflag;
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                     DELETE AN EVENT FLAG GROUP
*
* Description: This function deletes an event flag group and readies all tasks pending on the event flag
*              group.
*
* Arguments  : pflag         is a pointer to the desired event flag group.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Deletes the event flag group ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the event flag group even if tasks are
*                                                    waiting.  In this case, all the tasks pending will be
*                                                    readied.
*
* Returns    : OS_ERR_NONE              The call was successful and the event flag group was deleted
*              OS_ERR_DEL_ISR           If you attempted to delete the event flag group from an ISR
*              OS_ERR_INVALID_HANDLE   If 'pflag' is a NULL pointer.
*              OS_ERR_EVENT_TYPE        If you didn't pass a pointer to an event flag group
*              OS_ERR_INVALID_OPT       An invalid option was specified
*              OS_ERR_TASK_WAITING      One or more tasks were waiting on the event flag group.
*
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the event flag group MUST check the return code of OSFlagAccept() and osFlagPend().
*              2) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the event flag group.
*              3) All tasks that were waiting for the event flag will be readied and returned an
*                 OS_ERR_PEND_ABORT if osFlagDelete() was called with OS_DEL_ALWAYS
*
* Called from: Task ONLY
*********************************************************************************************************/

#if OS_FLAG_DEL_EN > 0u
OS_ERR   osFlagDelete   (OS_HANDLE   hFlag,
                         UINT8       opt)
{
    OS_FLAG      *pflag = (OS_FLAG *)hFlag;
    BOOL       waiting;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR     cpu_sr = 0u;
#endif
    UINT8         err;


    if (osIntNesting > 0u) {                            //!< Can't DELETE from an ISR
        return OS_ERR_DEL_ISR;
    }
#if OS_ARG_CHK_EN > 0u
    if (pflag == NULL) {                             //!< Validate 'pflag'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pflag->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_FLAG) { //!< Validate event type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pflag->OSFlagWaitList != NULL) {             //!< See if any tasks waiting on event flags
        waiting = TRUE;                              //!< Yes
    } else {
        waiting = FALSE;                             //!< No
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                                //!< Delete if no task waiting
             if (waiting == FALSE) {
                 pflag->OSObjType      = OS_OBJ_TYPE_UNUSED;
                 pflag->OSFlagFlags    = 0u;
                 pflag->OSFlagPtr      = osFlagFreeList;    //!< Return ECB to free list
                 osFlagFreeList        = pflag;
                 OSExitCriticalSection(cpu_sr);
                 err = OS_ERR_NONE;
             } else {
                 OSExitCriticalSection(cpu_sr);
                 err = OS_ERR_TASK_WAITING;
             }
             break;

        case OS_DEL_ALWAYS:                                 //!< Always delete the event flag
            while (pflag->OSFlagWaitList != NULL) {      //!< Ready ALL tasks waiting for this flag.
                 (void)OS_EventTaskRdy(pflag, OS_STAT_PEND_ABORT);
            }
            pflag->OSObjType      = OS_OBJ_TYPE_UNUSED;
            pflag->OSFlagFlags    = 0u;
            pflag->OSFlagPtr      = osFlagFreeList;         //!< Return flag to free list
            osFlagFreeList        = pflag;
            OSExitCriticalSection(cpu_sr);
            if (waiting) {                       //!< Reschedule only if task(s) were waiting
                OS_Schedule();
            }
            err = OS_ERR_NONE;
            break;

        default:
             OSExitCriticalSection(cpu_sr);
             err = OS_ERR_INVALID_OPT;
             break;
    }
    return err;
}
#endif

/*********************************************************************************************************
*                                     WAIT ON AN EVENT FLAG
*
* Description: This function is called to wait for a flag to be set.
*
* Arguments  : hFlag         is a handle to the desired event flag handle.
*
*              timeout       is an optional timeout (in clock ticks) that your task will wait for the
*                            desired bit combination.  If you specify 0, however, your task will wait
*                            forever at the specified event flag group or, until a message arrives.
*
*  Returns    : OS_ERR_NONE               The desired bits have been set within the specified 'timeout'.
*               OS_ERR_PEND_ISR           If you tried to PEND from an ISR
*               OS_ERR_PEND_LOCKED        If you called this function when the scheduler is locked
*               OS_ERR_INVALID_HANDLE     If 'pflag' is a NULL pointer.
*               OS_ERR_EVENT_TYPE         You are not pointing to an event flag
*               OS_ERR_TIMEOUT            The bit(s) have not been set in the specified 'timeout'.
*               OS_ERR_PEND_ABORT         The wait on the flag was aborted.
*
* Called from: Task ONLY
*
* Note(s)    : 1) IMPORTANT, the behavior of this function has changed from PREVIOUS versions.  The
*                 function NOW returns the flags that were ready INSTEAD of the current state of the
*                 event flags.
*********************************************************************************************************/

OS_ERR   osFlagPend (OS_HANDLE       hFlag,
                     UINT32          timeout)
{
    OS_FLAG        *pflag = (OS_FLAG *)hFlag;
    OS_WAIT_NODE    node;
    UINT16          ready;
    UINT16          consume;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR       cpu_sr = 0u;
#endif
    UINT8           err;


    if (osIntNesting > 0u) {                        //!< Can't PEND from an ISR
        return OS_ERR_PEND_ISR;
    }
    if (osLockNesting > 0u) {                       //!< Can't PEND when locked
        return OS_ERR_PEND_LOCKED;
    }
#if OS_ARG_CHK_EN > 0u
    if (pflag == NULL) {                         //!< Validate 'pflag'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pflag->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_FLAG) { //!< Validate event type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    consume = pflag->OSFlagFlags & 0x80u;
    ready   = pflag->OSFlagFlags & 0x01u;
    if (ready != 0u) {                              //!< See if flag set
        if (consume != 0u) {                        //!< Yes. See if we need to consume the flags
            pflag->OSFlagFlags &= ~0x01u;           //!< Yes. Clear flag
        }
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }

    if (timeout == 0u) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TIMEOUT;
    }

    OS_EventTaskWait(pflag, &node, timeout);        //!< Suspend task until event occur or timeout
    OSExitCriticalSection(cpu_sr);
    OS_Schedule();

    switch (node.OSWaitNodeRes) {
        case OS_STAT_PEND_OK:
            err = OS_ERR_NONE;
            break;
            
        case OS_STAT_PEND_ABORT:
            err = OS_ERR_PEND_ABORT;                //!< Indicate that we aborted waiting
            break;
            
        case OS_STAT_PEND_TO:
        default:
            err = OS_ERR_TIMEOUT;                   //!< Indicate that we timed-out waiting
            break;
    }
    return err;
}

/*********************************************************************************************************
*                                       SET EVENT FLAG BIT(S)
*
* Description: This function is called to set an event flag.
*
* Arguments  : pflag          is a pointer to the desired event flag group.
*
* Returns    : OS_ERR_NONE                The call was successfull
*              OS_ERR_INVALID_HANDLE     You passed a NULL pointer
*              OS_ERR_EVENT_TYPE          You are not pointing to an event flag group
*
* Called From: Task or ISR
*
* WARNING(s) : 1) The execution time of this function depends on the number of tasks waiting on the event
*                 flag group.
*              2) The amount of time interrupts are DISABLED depends on the number of tasks waiting on
*                 the event flag group.
*********************************************************************************************************/

OS_ERR   osFlagSet   (OS_HANDLE  hFlag)
{
    OS_FLAG      *pflag = (OS_FLAG *)hFlag;
    BOOL       sched;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR     cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pflag == NULL) {                             //!< Validate 'pflag'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pflag->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_FLAG) {         //!< Make sure we are pointing to an event flag
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    pflag->OSFlagFlags |= 0x01u;                            //!< Set the flags.
    sched = FALSE;                                       //!< Indicate that we don't need rescheduling
    if (pflag->OSFlagWaitList != NULL) {
        sched = TRUE;                                    //!< When done we will reschedule
        while (pflag->OSFlagWaitList != NULL) {          //!< Ready ALL tasks waiting for this flag.
            (void)OS_EventTaskRdy(pflag, OS_STAT_PEND_OK);
        }
        if (pflag->OSFlagFlags & 0x80u) {                   //!< Is this a auto-reset flag?
            pflag->OSFlagFlags &= ~0x01u;                   //!< Yes, Reset the flag.
        }
    }
    OSExitCriticalSection(cpu_sr);
    if (sched) {
        OS_Schedule();
    }
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                       RESET EVENT FLAG BIT(S)
*
* Description: This function is called to reset an event flag.
*
* Arguments  : pflag          is a pointer to the desired event flag.
*
* Returns    : OS_ERR_NONE              The call was successfull
*              OS_ERR_INVALID_HANDLE    You passed a NULL pointer
*              OS_ERR_EVENT_TYPE        You are not pointing to an event flag
*
* Called From: Task or ISR
*********************************************************************************************************/

OS_ERR   osFlagReset (OS_HANDLE  hFlag)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pflag == NULL) {                         //!< Validate 'pflag'.
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pflag->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_FLAG) { //!< Validate event type.
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    pflag->OSFlagFlags &= ~0x01u;                   //!< Reset the flags
    OSExitCriticalSection(cpu_sr);
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                          QUERY EVENT FLAG
*
* Description: This function is used to check the value of the event flag group.
*
* Arguments  : pflag         is a pointer to the desired event flag group.
*
* Returns    : OS_ERR_NONE              The call was successfull
*              OS_ERR_INVALID_HANDLE    You passed a NULL pointer
*              OS_ERR_EVENT_TYPE        You are not pointing to an event flag
*
* Called From: Task or ISR
*********************************************************************************************************/

#if OS_FLAG_QUERY_EN > 0u
OS_ERR   osFlagQuery   (OS_HANDLE      hFlag,
                        OS_FLAG_INFO  *pInfo)
{
    OS_FLAG    *pflag = (OS_FLAG *)hFlag;
    UINT16      flag;
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pflag == NULL) {                     //!< Validate 'pflag'
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {               //!< Validate 'pInfo'
        return OS_ERR_PDATA_NULL;
    }
    if ((pflag->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_FLAG) { //!< Validate event type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    flag = pflag->OSFlagFlags;
    if (flag & 0x80 != 0) {                     //!< Is this a auto-rest flag?
        pInfo->OSFlagAutoReset = TRUE;
    } else {
        pInfo->OSFlagAutoReset = FALSE;
    }
    if (flag & 0x01 != 0) {                     //!< Is this flag set?
        pInfo->OSFlagStatus = TRUE;
    } else {
        pInfo->OSFlagStatus = FALSE;
    }
    OSExitCriticalSection(cpu_sr);
    return OS_ERR_NONE;
}
#endif



#endif      //!< (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
