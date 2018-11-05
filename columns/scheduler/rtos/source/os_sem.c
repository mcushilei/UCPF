
#include ".\os.h"

#if OS_SEM_EN > 0u

/*********************************************************************************************************
*                                         CREATE A SEMAPHORE
*
* Description: This function creates a semaphore.
*
* Arguments  : ppevent       is a pointer to a pointer of the event control block
*
*              cnt           is the initial value for the semaphore.  If the value is 0, no resource is
*                            available (or no event has occurred).  You initialize the semaphore to a
*                            non-zero value to specify how many resources are available (e.g. if you have
*                            10 resources, you would initialize the semaphore to 10).
*
* Returns    : OS_ERR_NONE              If the call was successful.
*              OS_ERR_CREATE_ISR        If you attempted to create a MUTEX from an ISR
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a NULL pointer.
*              OS_ERR_EVENT_DEPLETED    No more event control blocks available.
*********************************************************************************************************/

OS_ERR   osSemCreate   (OS_HANDLE  *pSemaphoreHandle,
                        UINT16      cnt)
{
    OS_EVENT   *pevent;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                            //!< See if called from ISR ...
        return OS_ERR_CREATE_ISR;                       //!< ... can't CREATE from an ISR
    }
#if OS_ARG_CHK_EN > 0u
    if (pSemaphoreHandle == NULL) {                  //!< Validate handle
        return OS_ERR_INVALID_HANDLE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    pevent = osEventFreeList;                           //!< Get next free event control block
    if (pevent == NULL) {                            //!< See if pool of free ECB pool was empty
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_EVENT_DEPLETED;                   //!< No more event control blocks
    }
    osEventFreeList = (OS_EVENT *)osEventFreeList->OSEventPtr; //!< Adjust the free list
    OSExitCriticalSection(cpu_sr);
    
    pevent->OSObjType   = OS_OBJ_TYPE_SEM
                        | OS_OBJ_WAITABLE
                        | OS_OBJ_PRIO_TYPE_BITMAP;
    pevent->OSEventCnt  = cnt;                          //!< Set semaphore value
    pevent->OSEventPtr  = NULL;                      //!< Unlink from ECB free list
    OS_EventWaitTableInit(pevent);                      //!< Initialize to 'nobody waiting' on sem.
    *pSemaphoreHandle = pevent;
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                         DELETE A SEMAPHORE
*
* Description: This function deletes a semaphore and readies all tasks pending on the semaphore.
*
* Arguments  : hSemaphore    is a handle to the event control block associated with the desired
*                            semaphore.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete semaphore ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the semaphore even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
* Returns    : OS_ERR_NONE              The call was successful and the semaphore was deleted
*              OS_ERR_DEL_ISR           If you attempted to delete the semaphore from an ISR
*              OS_ERR_INVALID_OPT       An invalid option was specified
*              OS_ERR_TASK_WAITING      One or more tasks were waiting on the semaphore
*              OS_ERR_EVENT_TYPE        If you didn't pass a pointer to a semaphore
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a invalid handle.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the semaphore MUST check the return code of osSemPend().
*              2) OSSemAccept() callers will not know that the intended semaphore has been deleted unless
*                 they check 'pevent' to see that it's a NULL pointer.
*              3) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the semaphore.
*              4) Because ALL tasks pending on the semaphore will be readied, you MUST be careful in
*                 applications where the semaphore is used for mutual exclusion because the resource(s)
*                 will no longer be guarded by the semaphore.
*              5) All tasks that were waiting for the semaphore will be readied and returned an
*                 OS_ERR_PEND_ABORT if osSemDelete() was called with OS_DEL_ALWAYS
*********************************************************************************************************/

#if OS_SEM_DEL_EN > 0u
OS_ERR   osSemDelete   (OS_HANDLE  hSemaphore,
                        UINT8      opt)
{
    OS_EVENT   *pevent = (OS_EVENT *)hSemaphore;
    BOOL        tasksWaiting;
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                    //!< See if called from ISR ...
        return OS_ERR_DEL_ISR;                  //!< ... can't DELETE from an ISR
    }
#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                       //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) {         //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventGrp != 0u) {             //!< See if any tasks waiting on semaphore
        tasksWaiting = TRUE;                    //!< Yes
    } else {
        tasksWaiting = FALSE;                   //!< No
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                    //!< Delete semaphore only if no task waiting
             if (tasksWaiting) {
                 OSExitCriticalSection(cpu_sr);
                 return OS_ERR_TASK_WAITING;
             }
             break;

        case OS_DEL_ALWAYS:                     //!< Always delete the semaphore
             while (pevent->OSEventGrp != 0u) { //!< Ready ALL tasks waiting for semaphore
                 (void)OS_EventTaskRdy(pevent, OS_STAT_PEND_ABORT);
             }
             break;

        default:
             OSExitCriticalSection(cpu_sr);
             return OS_ERR_INVALID_OPT;
    }
    
    pevent->OSObjType   = OS_OBJ_TYPE_UNUSED;
    pevent->OSEventPtr  = osEventFreeList;      //!< Return Event Control Block to free list
    pevent->OSEventCnt  = 0u;
    osEventFreeList     = pevent;               //!< Get next free event control block
    OSExitCriticalSection(cpu_sr);
    
    if (tasksWaiting) {                         //!< Reschedule only if task(s) were waiting
        OS_Schedule();                          //!< Find highest priority task ready to run
    }
    
    return OS_ERR_NONE;
}
#endif

/*********************************************************************************************************
*                                          PEND ON SEMAPHORE
*
* Description: This function waits for a semaphore.
*
* Arguments  : hSemaphore    is a handle to the event control block associated with the desired
*                            semaphore.
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for the resource up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever at the specified
*                            semaphore or, until the resource becomes available (or the event occurs).
*
* Returns    : OS_ERR_NONE              The call was successful and your task owns the resource
*                                       or, the event you are waiting for occurred.
*              OS_ERR_TIMEOUT           The semaphore was not received within the specified
*                                       'timeout'.
*              OS_ERR_PEND_ABORT        The wait on the semaphore was aborted.
*              OS_ERR_EVENT_TYPE        If you didn't pass a pointer to a semaphore.
*              OS_ERR_PEND_ISR          If you called this function from an ISR and the result
*              OS_ERR_PEND_LOCKED       If you called this function when the scheduler is locked
*                                       would lead to a suspension.
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a invalid handle.
*********************************************************************************************************/

OS_ERR   osSemPend (OS_HANDLE  hSemaphore,
                    UINT32     timeout)
{
    OS_EVENT       *pevent = (OS_EVENT *)hSemaphore;
    OS_WAIT_NODE    node;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR       cpu_sr = 0u;
#endif
    UINT8           err;


    if (osIntNesting > 0u) {                        //!< See if called from ISR ...
        return OS_ERR_PEND_ISR;                     //!< ... can't PEND from an ISR
    }
    if (osLockNesting > 0u) {                       //!< See if called with scheduler locked ...
        return OS_ERR_PEND_LOCKED;                  //!< ... can't PEND when locked
    }
#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                        //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) {     //!< Validate event type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventCnt > 0u) {                  //!< If sem. is positive, resource available ...
        pevent->OSEventCnt--;                       //!< ... decrement semaphore only if positive.
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }

    if (timeout == 0u) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TIMEOUT;
    }

    OS_EventTaskWait(pevent, &node, timeout);       //!< Suspend current task until event or timeout occurs
    OSExitCriticalSection(cpu_sr);
    OS_Schedule();                                  //!< Find next highest priority task ready

    switch (node.OSWaitNodeRes) {                   //!< See if we timed-out or aborted
        case OS_STAT_PEND_OK:
             err = OS_ERR_NONE;
             break;

        case OS_STAT_PEND_ABORT:
             err = OS_ERR_PEND_ABORT;               //!< Indicate that we aborted
             break;

        case OS_STAT_PEND_TO:
        default:
             err = OS_ERR_TIMEOUT;                  //!< Indicate that we didn't get event within TO
             break;
    }
    return err;
}

/*********************************************************************************************************
*                                    ABORT WAITING ON A SEMAPHORE
*
* Description: This function aborts & readies any tasks currently waiting on a semaphore.  This function
*              should be used to fault-abort the wait on the semaphore, rather than to normally signal
*              the semaphore via osSemPost().
*
* Arguments  : hSemaphore    is a handle to the event control block associated with the desired
*                            semaphore.
*
*              opt           determines the type of ABORT performed:
*                            OS_PEND_OPT_NONE         ABORT wait for the highest priority task (HPT) that
*                                                     is waiting on the semaphore
*                            OS_PEND_OPT_BROADCAST    ABORT wait for ALL tasks that are  waiting on the
*                                                     semaphore
*
* Returns    : OS_ERR_NONE              No tasks were waiting on the semaphore.
*              OS_ERR_PEND_ABORT        At least one task waiting on the semaphore was readied
*                                       and informed of the aborted wait; check return value
*                                       for the number of tasks whose wait on the semaphore
*                                       was aborted.
*              OS_ERR_EVENT_TYPE        If you didn't pass a pointer to a semaphore.
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a invalid handle.
*********************************************************************************************************/

#if OS_SEM_PEND_ABORT_EN > 0u
OS_ERR  osSemPendAbort (OS_HANDLE  hSemaphore,
                        UINT8      opt)
{
    OS_EVENT   *pevent = (OS_EVENT *)hSemaphore;
    UINT8       err;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                            //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) {         //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventGrp != 0u) {                     //!< See if any task waiting on semaphore?
        switch (opt) {
            case OS_PEND_OPT_BROADCAST:                 //!< Do we need to abort ALL waiting tasks?
                 while (pevent->OSEventGrp != 0u) {     //!< Yes, ready ALL tasks waiting on semaphore
                     (void)OS_EventTaskRdy(pevent, OS_STAT_PEND_ABORT);
                 }
                 break;

            case OS_PEND_OPT_NONE:
            default:                                    //!< No, ready HPT waiting on semaphore
                 OS_EventTaskRdy(pevent, OS_STAT_PEND_ABORT);
                 break;
        }
        OSExitCriticalSection(cpu_sr);
        OS_Schedule();                                  //!< Find HPT ready to run
        err = OS_ERR_PEND_ABORT;
    } else {
        OSExitCriticalSection(cpu_sr);
        err = OS_ERR_NONE;
    }
    return err;
}
#endif

/*********************************************************************************************************
*                                         POST TO A SEMAPHORE
*
* Description: This function signals a semaphore. You can post multiple resource at once, so if there are 
*              several tasks pend for this semaphore, tasks will be ready by their priority till all
*              resource exhausted.
*
* Arguments  : hSemaphore    is a handle to the event control block associated with the desired
*                            semaphore.
*
*              cnt           is the value for the semaphore count to add.
*
* Returns    : OS_ERR_NONE         The call was successful and the semaphore was signaled.
*              OS_ERR_SEM_OVF      If the semaphore count exceeded its limit. In other words, you have
*                                  signaled the semaphore more often than its capable.
*              OS_ERR_EVENT_TYPE   If you didn't pass a pointer to a semaphore
*              OS_ERR_INVALID_HANDLE  If hSemaphore is a invalid handle.
*********************************************************************************************************/

OS_ERR  osSemPost  (OS_HANDLE hSemaphore,
                    UINT16    cnt)
{
    OS_EVENT   *pevent = (OS_EVENT *)hSemaphore;
    UINT8       err;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                            //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) { //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif
    
    if (cnt == 0u) {
        return OS_ERR_NONE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (cnt <= (65535u - pevent->OSEventCnt)) {         //!< Make sure semaphore will not overflow
        pevent->OSEventCnt += cnt;                      //!< Increment semaphore count to register event
        if (pevent->OSEventGrp != 0u) {                 //!< See if any tasks waiting for semaphore
            while (pevent->OSEventGrp != 0u && pevent->OSEventCnt != 0u) {
                pevent->OSEventCnt--;                               //!< decrement semaphore count...
                OS_EventTaskRdy(pevent, OS_STAT_PEND_OK);           //!< ...and ready HPT waiting on event
            }
            OSExitCriticalSection(cpu_sr);
            OS_Schedule();                              //!< Find HPT ready to run.
        } else {
            OSExitCriticalSection(cpu_sr);
        }
        err = OS_ERR_NONE;
    } else {                                            //!< Not change semaphore count.
        OSExitCriticalSection(cpu_sr);
        err = OS_ERR_SEM_OVF;
    }
    return err;
}

/*********************************************************************************************************
*                                            SET SEMAPHORE
*
* Description: This function sets the semaphore count to the value specified as an argument.  Typically,
*              this value would be 0.
*
*              You would typically use this function when a semaphore is used as a signaling mechanism
*              and, you want to reset the count value.
*
* Arguments  : hSemaphore   is a pointer to the event control block
*
*              cnt          is the new value for the semaphore count.  You would pass 0 to reset the
*                           semaphore count.
*
* Returns    : OS_ERR_NONE              The call was successful and the semaphore value was set.
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a invalid handle.
*              OS_ERR_EVENT_TYPE        If you didn't pass a handle of a semaphore.
*              OS_ERR_TASK_WAITING      If tasks are waiting on the semaphore.
*********************************************************************************************************/

#if OS_SEM_SET_EN > 0u
OS_ERR   osSemSet   (OS_HANDLE  hSemaphore,
                     UINT16     cnt)
{
    OS_EVENT  *pevent = (OS_EVENT *)hSemaphore;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif
    UINT8      err;


#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                            //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) {         //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    err = OS_ERR_NONE;
    if (pevent->OSEventCnt > 0u) {                      //!< See if semaphore already has a count
        pevent->OSEventCnt = cnt;                       //!< Yes, set it to the new value specified.
    } else {                                            //!< No
        if (pevent->OSEventGrp == 0u) {                 //!< See if task(s) waiting?
            pevent->OSEventCnt = cnt;                   //!< No, OK to set the value
        } else {
            err = OS_ERR_TASK_WAITING;
        }
    }
    OSExitCriticalSection(cpu_sr);
    return err;
}
#endif

/*********************************************************************************************************
*                                          QUERY A SEMAPHORE
*
* Description: This function obtains information about a semaphore
*
* Arguments  : hSemaphore    is a pointer to the event control block associated with the desired
*                            semaphore
*
*              pInfo    is a pointer to a structure that will contain information about the
*                            semaphore.
*
* Returns    : OS_ERR_NONE              The call was successful and the message was sent
*              OS_ERR_EVENT_TYPE        If you are attempting to obtain data from a non semaphore.
*              OS_ERR_INVALID_HANDLE    If 'hSemaphore' is a invalid handle.
*              OS_ERR_PDATA_NULL        If 'pInfo' is a NULL pointer
*********************************************************************************************************/

#if OS_SEM_QUERY_EN > 0u
OS_ERR  osSemQuery (OS_HANDLE    hSemaphore,
                    OS_SEM_INFO *pInfo)
{
    OS_EVENT       *pevent = (OS_EVENT *)hSemaphore;
    OS_PRIO        *psrc;
    OS_PRIO        *pdest;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR       cpu_sr = 0u;
#endif
    UINT8           i;


#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                        //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_SEM) {         //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
    if (pInfo == NULL) {                         //!< Validate 'pInfo'
        return OS_ERR_PDATA_NULL;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    pInfo->OSEventGrp   = pevent->OSEventGrp;       //!< Copy wait list
    psrc    = &pevent->OSEventTbl[0];
    pdest   = &pInfo->OSEventTbl[0];
    for (i = 0u; i < OS_BITMAP_TBL_SIZE; i++) {
        *pdest++ = *psrc++;
    }
    pInfo->OSCnt = pevent->OSEventCnt;              //!< Get semaphore count
    OSExitCriticalSection(cpu_sr);
    return OS_ERR_NONE;
}
#endif      //!< OS_SEM_QUERY_EN

#endif      //!< OS_SEM_EN
