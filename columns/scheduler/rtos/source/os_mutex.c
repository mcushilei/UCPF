
#include ".\os.h"

#if OS_MUTEX_EN > 0u

/*********************************************************************************************************
*                                 CREATE A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function creates a mutual exclusion semaphore.
*
* Arguments  : pMutexHandle  is a pointer to the handle to the event control block associated with the
*                            desired mutex.
*
*              ceilingPrio   ceiling priority of the mutex. You can disable this function of the mutex by
*                            setting this value to OS_TASK_LOWEST_PRIO.
*
* Returns    : OS_ERR_NONE              If the call was successful.
*              OS_ERR_CREATE_ISR        If you attempted to create a MUTEX from an ISR
*              OS_ERR_INVALID_HANDLE    If 'ppevent' is a NULL pointer.
*              OS_ERR_EVENT_DEPLETED    No more event control blocks available.
*              OS_ERR_TASK_EXIST        The ceiling priority has been used by task.
*********************************************************************************************************/

OS_ERR   osMutexCreate (OS_HANDLE  *pMutexHandle,
                        UINT8       ceilingPrio)
{
    OS_EVENT   *pevent;
#if OS_CRITICAL_METHOD == 3u                                //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                                //!< See if called from ISR ...
        return OS_ERR_CREATE_ISR;                           //!< ... can't CREATE mutex from an ISR
    }
#if OS_ARG_CHK_EN > 0u
    if (pMutexHandle == NULL) {                          //!< Validate handle
        return OS_ERR_INVALID_HANDLE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (ceilingPrio < OS_TASK_IDLE_PRIO) {
        if (osPrioTCBTbl[ceilingPrio] != NULL) {
            OSExitCriticalSection(cpu_sr);
            return OS_ERR_TASK_EXIST;
        }
    }
    if (osEventFreeList == NULL) {                   //!< See if an ECB was available
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_EVENT_DEPLETED;                   //!< No more event control blocks
    }
    pevent          = osEventFreeList;                  //!< Get next free event control block
    osEventFreeList = osEventFreeList->OSEventPtr;      //!< Adjust the free list
    OSExitCriticalSection(cpu_sr);

    pevent->OSObjType           = OS_OBJ_TYPE_MUTEX
                                | OS_OBJ_WAITABLE
                                | OS_OBJ_PRIO_TYPE_BITMAP;
    pevent->OSEventCeilingPrio  = ceilingPrio;
    pevent->OSEventOwnerPrio    = OS_TASK_IDLE_PRIO;
    pevent->OSEventOwnerTCB     = NULL;              //!< No task owning the mutex
    OS_EventWaitTableInit(pevent);                      //!< Initialize to 'nobody waiting' on mutex.
    *pMutexHandle = pevent;
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                           DELETE A MUTEX
*
* Description: This function deletes a mutual exclusion semaphore and readies all tasks pending on the it.
*
* Arguments  : hMutex        is a handle to the event control block associated with the desired mutex.
*
*              opt           determines delete options as follows:
*                            opt == OS_DEL_NO_PEND   Delete mutex ONLY if no task pending
*                            opt == OS_DEL_ALWAYS    Deletes the mutex even if tasks are waiting.
*                                                    In this case, all the tasks pending will be readied.
*
* Returns    : OS_ERR_NONE             The call was successful and the mutex was deleted
*              OS_ERR_DEL_ISR          If you attempted to delete the MUTEX from an ISR
*              OS_ERR_INVALID_OPT      An invalid option was specified
*              OS_ERR_TASK_WAITING     One or more tasks were waiting on the mutex
*              OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a mutex
*              OS_ERR_INVALID_HANDLE      If 'pevent' is a NULL pointer.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of
*                 the mutex MUST check the return code of osMutexPend().
*
*              2) This call can potentially disable interrupts for a long time.  The interrupt disable
*                 time is directly proportional to the number of tasks waiting on the mutex.
*
*              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful because the
*                 resource(s) will no longer be guarded by the mutex.
*
*              4) IMPORTANT: In the 'OS_DEL_ALWAYS' case, we assume that the owner of the Mutex (if there
*                            is one) is ready-to-run and is thus NOT pending on another kernel object or
*                            has delayed itself.  In other words, if a task owns the mutex being deleted,
*                            that task will be made ready-to-run at its original priority.
*********************************************************************************************************/

#if OS_MUTEX_DEL_EN > 0u
OS_ERR   osMutexDelete  (OS_HANDLE  hMutex,
                         UINT8      opt)
{
    OS_EVENT   *pevent = (OS_EVENT *)hMutex;
    OS_TCB     *powner;
    BOOL        tasksWaiting;
    BOOL        taskSched = FALSE;
#if OS_CRITICAL_METHOD == 3u                                //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                                //!< Can't DELETE from an ISR.
        return OS_ERR_DEL_ISR;
    }
#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                                //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_MUTEX) {   //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventGrp != 0u) {                         //!< See if any tasks waiting on mutex
        tasksWaiting = TRUE;                            //!< Yes
        taskSched   = TRUE;
    } else {
        tasksWaiting = FALSE;                           //!< No
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                                //!< Delete mutex if NO task waiting.
             if (tasksWaiting) {
                 OSExitCriticalSection(cpu_sr);
                 return OS_ERR_TASK_WAITING;
             }
             break;

        case OS_DEL_ALWAYS:                                 //!< ALWAYS delete mutex.
             while (pevent->OSEventGrp != 0u) {             //!< Ready ALL tasks waiting for mutex
                 (void)OS_EventTaskRdy(pevent, OS_STAT_PEND_ABORT);
             }
             break;

        default:
             OSExitCriticalSection(cpu_sr);
             return OS_ERR_INVALID_OPT;
    }
    
    powner = pevent->OSEventOwnerTCB;
    if (powner != NULL) {                                           //!< See if this mutex has been owned by any task.
        powner->OSTCBOwnMutex = NULL;                               //!< Yes.
        if (pevent->OSEventCeilingPrio == powner->OSTCBPrio) {      //!< If this task's prio has been changed,
            OS_TaskChangePrio(powner, pevent->OSEventOwnerPrio);    //!< then restore task's prio.
            taskSched = TRUE;
        }
    }

    pevent->OSObjType     = OS_OBJ_TYPE_UNUSED;
    pevent->OSEventPtr    = osEventFreeList;   //!< Return Event Control Block to free list
    pevent->OSEventCnt    = 0u;
    osEventFreeList       = pevent;
    OSExitCriticalSection(cpu_sr);
    
    if (taskSched) {                //!< Reschedule only if task(s) were waiting
        OS_Schedule();
    }

    return OS_ERR_NONE;
}
#endif

/*********************************************************************************************************
*                                 PEND ON MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function waits for a mutual exclusion semaphore.
*
* Arguments  : hMutex        is a handle to the event control block associated with the desired
*                            mutex.
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will
*                            wait for the resource up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever at the specified
*                            mutex or, until the resource becomes available.
*
* Returns    : OS_ERR_NONE        The call was successful and your task owns the mutex
*              OS_ERR_TIMEOUT     The mutex was not available within the specified 'timeout'.
*              OS_ERR_PEND_ABORT  The wait on the mutex was aborted.
*              OS_ERR_PEND_ISR    If you called this function from an ISR and the result
*                                 would lead to a suspension.
*              OS_ERR_PEND_LOCKED If you called this function when the scheduler is locked
*              OS_ERR_EVENT_TYPE  If you didn't pass a pointer to a mutex
*              OS_ERR_INVALID_HANDLE 'pevent' is a NULL pointer
*              OS_ERR_HAS_OWN_MUTEX  Current task has owned this mutex.
*
* Note(s)    : 1) The task that owns the mutex MUST NOT pend on any other event while it owns the mutex.
*              2) You MUST NOT change the priority of the task that owns the mutex
*********************************************************************************************************/

OS_ERR  osMutexPend (OS_HANDLE  hMutex,
                     UINT32     timeout)
{
    OS_EVENT       *pevent = (OS_EVENT *)hMutex;
    OS_WAIT_NODE    node;
#if OS_CRITICAL_METHOD == 3u                                //!< Allocate storage for CPU status register
    OS_CPU_SR       cpu_sr = 0u;
#endif
    UINT8           err;


    if (osIntNesting > 0u) {                                //!< See if called from ISR ...
        return OS_ERR_PEND_ISR;                             //!< ... can't PEND from an ISR
    }
    if (osLockNesting > 0u) {                               //!< See if called with scheduler locked ...
        return OS_ERR_PEND_LOCKED;                          //!< ... can't PEND when locked
    }
#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                                //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_MUTEX) {   //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventOwnerTCB == NULL) {               //!< Is Mutex available?
        pevent->OSEventOwnerPrio = osTCBCur->OSTCBPrio;     //!< Yes,  current task own this mutex.
        pevent->OSEventOwnerTCB  = osTCBCur;                //!< ... save current task's TCB and prio to mutex.
        osTCBCur->OSTCBOwnMutex  = pevent;
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NONE;
    }
    
    if (timeout == 0u) {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TIMEOUT;
    }

    if (pevent->OSEventOwnerTCB->OSTCBPrio > pevent->OSEventCeilingPrio) {      //!< Owner's prio lower than ceiling.
        OS_TaskChangePrio(pevent->OSEventOwnerTCB, pevent->OSEventCeilingPrio); //!< change owner's prio to ceiling.
    }
    
    OS_EventTaskWait(pevent, &node, timeout);               //!< Suspend current task until event or timeout occurs
    OSExitCriticalSection(cpu_sr);
    OS_Schedule();

    switch (node.OSWaitNodeRes) {                           //!< See if we timed-out or aborted
        case OS_STAT_PEND_OK:
             err = OS_ERR_NONE;                             //!< Indicate current has owned this mutex.
             break;

        case OS_STAT_PEND_ABORT:
             err = OS_ERR_PEND_ABORT;                       //!< Indicate that we aborted getting mutex.
             break;

        case OS_STAT_PEND_TO:
        default:
             err = OS_ERR_TIMEOUT;                          //!< Indicate that we didn't get mutex within TO.
             break;
    }
    return err;
}

/*********************************************************************************************************
*                                POST TO A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function signals a mutual exclusion semaphore
*
* Arguments  : pevent              is a pointer to the event control block associated with the desired
*                                  mutex.
*
* Returns    : OS_ERR_NONE             The call was successful and the mutex was signaled.
*              OS_ERR_EVENT_TYPE       If you didn't pass a pointer to a mutex
*              OS_ERR_INVALID_HANDLE   'hMutex' is a NULL pointer
*              OS_ERR_POST_ISR         Attempted to post from an ISR (not valid for MUTEXes)
*              OS_ERR_NOT_MUTEX_OWNER  The task that did the post is NOT the owner of the MUTEX.
*
* Note(s)    : 1) The mutex can ONLY be released by it's owner.
*********************************************************************************************************/

OS_ERR   osMutexPost (OS_HANDLE  hMutex)
{
    OS_EVENT   *pevent = (OS_EVENT *)hMutex;
    BOOL        taskSched = FALSE;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                            //!< See if called from ISR ...
        return OS_ERR_POST_ISR;                         //!< ... can't POST mutex from an ISR
    }
#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                            //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_MUTEX) {   //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }
#endif

    OSEnterCriticalSection(cpu_sr);
    if (osTCBCur != pevent->OSEventOwnerTCB) {          //!< See if this task owns the MUTEX
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_NOT_MUTEX_OWNER;
    }
    
    osTCBCur->OSTCBOwnMutex = NULL;
    if (pevent->OSEventOwnerPrio != osTCBCur->OSTCBPrio) {      //!< If this task's prio has been changed...
        OS_TaskChangePrio(osTCBCur, pevent->OSEventOwnerPrio);  //!< ... restore task's prio.
        taskSched = TRUE;
    }
    
    if (pevent->OSEventGrp != 0u) {                             //!< Any task waiting for the mutex?
        OS_TCB *ptcb = OS_EventTaskRdy(pevent, OS_STAT_PEND_OK);//!< Yes, Make HPT waiting for mutex ready
        pevent->OSEventOwnerTCB  = ptcb;
        pevent->OSEventOwnerPrio = ptcb->OSTCBPrio;
        taskSched = TRUE;
    } else {
        pevent->OSEventOwnerTCB  = NULL;
        pevent->OSEventOwnerPrio = OS_TASK_IDLE_PRIO;
    }
    OSExitCriticalSection(cpu_sr);
    
    if (taskSched) {
        OS_Schedule();
    }
    
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                 QUERY A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function obtains information about a mutex
*
* Arguments  : pevent       is a pointer to the event control block associated with the desired mutex
*
*              pInfo        is a pointer to a structure that will contain information about the mutex
*
* Returns    : OS_ERR_NONE          The call was successful and the message was sent
*              OS_ERR_INVALID_HANDLE   If 'pevent'       is a NULL pointer
*              OS_ERR_PDATA_NULL    If 'pInfo' is a NULL pointer
*              OS_ERR_EVENT_TYPE    If you are attempting to obtain data from a non mutex.
*********************************************************************************************************/

#if OS_MUTEX_QUERY_EN > 0u
OS_ERR  osMutexQuery   (OS_HANDLE      hMutex,
                        OS_MUTEX_INFO *pInfo)
{
    OS_EVENT       *pevent = (OS_EVENT *)hMutex;
    OS_PRIO        *psrc;
    OS_PRIO        *pdest;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR       cpu_sr = 0u;
#endif
    uint_fast8_t    i;


#if OS_ARG_CHK_EN > 0u
    if (pevent == NULL) {                            //!< Validate 'pevent'
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {                      //!< Validate 'pInfo'
        return OS_ERR_PDATA_NULL;
    }
#endif
    if ((pevent->OSObjType & OS_OBJ_TYPE_MSK) != OS_OBJ_TYPE_MUTEX) {       //!< Validate event block type
        return OS_ERR_EVENT_TYPE;
    }

    OSEnterCriticalSection(cpu_sr);
    if (pevent->OSEventOwnerTCB == NULL) {                //!< Any task own this mutex?
        pInfo->OSValue       = TRUE;
        pInfo->OSOwnerPrio   = 0u;
        pInfo->OSOwnerTCB    = NULL;
    } else {
        pInfo->OSValue       = FALSE;
        pInfo->OSOwnerTCB    = pevent->OSEventOwnerTCB;
        pInfo->OSOwnerPrio   = pevent->OSEventOwnerTCB->OSTCBPrio;
    }
    pInfo->OSCeilingPrio     = pevent->OSEventCeilingPrio;
    pInfo->OSEventGrp        = pevent->OSEventGrp;     //!< Copy wait list
    psrc  = &pevent->OSEventTbl[0];
    pdest = &pInfo->OSEventTbl[0];
    for (i = 0u; i < OS_BITMAP_TBL_SIZE; i++) {
        *pdest++ = *psrc++;
    }
    OSExitCriticalSection(cpu_sr);
    return OS_ERR_NONE;
}
#endif      //!< OS_MUTEX_QUERY_EN

#endif      //!< OS_MUTEX_EN
