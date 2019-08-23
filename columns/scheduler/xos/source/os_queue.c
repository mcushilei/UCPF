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

#if (OS_QUEUE_EN > 0u) && (OS_MAX_QUEUES > 0u)

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
OS_ERR osQueueCreate(OS_HANDLE *hQueue, void *buffer, UINT16 queueSize)
{
    OS_QUEUE   *pqueue;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (0u == queueSize || NULL == buffer) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }

    OSEnterCriticalSection();
    pqueue = OS_ObjPoolNew(&osQueueFreeList);
    if (pqueue == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OBJ_DEPLETED;
    }
    OSExitCriticalSection();

    //! Set object type.
    //! Init wait list head.
    //! Init queue values.
    pqueue->OSQueueObjHead.OSObjType =  OS_OBJ_TYPE_SET(OS_OBJ_TYPE_QUEUE)
                                     | OS_OBJ_TYPE_WAITABLE_MSK
                                     | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_LIST);
    os_list_init_head(&pqueue->OSQueueEnqueueWaitList);
    os_list_init_head(&pqueue->OSQueueDequeueWaitList);
    pqueue->OSQueueBuffer       = buffer;
    pqueue->OSQueueSize         = queueSize;
    pqueue->OSQueueHead         = 0;
    pqueue->OSQueueTail         = 0;
    pqueue->OSQueueLength       = 0;
    pqueue->OSQueueWriteToken   = queueSize;
    pqueue->OSQueueReadToken    = 0;
    
    *hQueue = pqueue;
    
    return OS_ERR_NONE;
}


OS_ERR osQueueDelete(OS_HANDLE *hQueue, UINT16 opt)
{
    OS_QUEUE   *pqueue = (OS_QUEUE *)*hQueue;
    BOOL        taskPend;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... can't DELETE from an ISR
    }
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }


    OSEnterCriticalSection();
    if (!OS_LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList) || !OS_LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) {     //!< check wait list if it's empty.
        taskPend    = TRUE;
    } else {
        taskPend    = FALSE;
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (pqueue->OSQueueLength != 0u) {
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
    while (!OS_LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList)) {  //!< Ready ALL suspended tasks.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueEnqueueWaitList, OS_STAT_PEND_ABORT);
    }
    while (!OS_LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) {  //!< Ready ALL suspended tasks.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueDequeueWaitList, OS_STAT_PEND_ABORT);
    }
    
    pqueue->OSQueueObjHead.OSObjType = OS_OBJ_TYPE_UNUSED;
    OS_ObjPoolFree(&osQueueFreeList, pqueue);
    OSExitCriticalSection();
    
    if (taskPend) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}

OS_ERR osQueueWrite(OS_HANDLE hQueue, void const *buffer, UINT32 timeout)
{
    OS_QUEUE       *pqueue = (OS_QUEUE *)hQueue;
    OS_WAIT_NODE    node;
    OS_ERR          err;
    

#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (NULL == buffer) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from an ISR.
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u) {           //!< See if called with scheduler locked.
        return OS_ERR_PEND_LOCKED;
    }
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }


    OSEnterCriticalSection();
    if (pqueue->OSQueueWriteToken == 0u) {     //!< is there any write token avalible?
        if (timeout == 0u) {
            OSExitCriticalSection();
            return OS_ERR_TIMEOUT;
        }
        
        OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)pqueue,            //!< Suspend current task until event or timeout occurs
                              &node,
                              &pqueue->OSQueueEnqueueWaitList,
                              timeout);
        OSExitCriticalSection();
        OS_SchedulerRunNext();
        
        OSEnterCriticalSection();
        if (node.OSWaitNodeRes != OS_STAT_PEND_OK) {
            switch (node.OSWaitNodeRes) {
                case OS_STAT_PEND_ABORT:
                     err = OS_ERR_PEND_ABORT;
                     break;

                case OS_STAT_PEND_TO:
                default:
                     err = OS_ERR_TIMEOUT;
                     break;
            }
            OSExitCriticalSection();
            return err;
        }
    } else {
        pqueue->OSQueueWriteToken--;    
    }

    pqueue->OSQueueBuffer[pqueue->OSQueueTail] = buffer;
    pqueue->OSQueueTail++;
    if (pqueue->OSQueueTail >= pqueue->OSQueueSize) {
        pqueue->OSQueueTail = 0;
    }
    pqueue->OSQueueLength++;
    pqueue->OSQueueReadToken++;
    if (!OS_LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) { //!< Is any task waiting for reading?
        pqueue->OSQueueReadToken--;
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueDequeueWaitList, OS_STAT_PEND_OK);
        OSExitCriticalSection();
        OS_SchedulerRunPrio();
        return OS_ERR_NONE;
    }
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

OS_ERR osQueueRead(OS_HANDLE hQueue, void *buffer, UINT32 timeout)
{
    OS_QUEUE       *pqueue = (OS_QUEUE *)hQueue;
    OS_WAIT_NODE    node;
    OS_ERR          err;
    

#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (NULL == buffer) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from an ISR.
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u) {           //!< See if called with scheduler locked.
        return OS_ERR_PEND_LOCKED;
    }
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    if (pqueue->OSQueueReadToken == 0u) {     //!< queue is empty.
        if (timeout == 0u) {
            OSExitCriticalSection();
            return OS_ERR_TIMEOUT;
        }
        
        OS_WaitableObjAddTask((OS_WAITABLE_OBJ *)pqueue,
                              &node,
                              &pqueue->OSQueueDequeueWaitList,
                              timeout);
        OSExitCriticalSection();
        OS_SchedulerRunNext();
        
        OSEnterCriticalSection();
        if (node.OSWaitNodeRes != OS_STAT_PEND_OK) {
            switch (node.OSWaitNodeRes) {
                case OS_STAT_PEND_ABORT:
                     err = OS_ERR_PEND_ABORT;
                     break;

                case OS_STAT_PEND_TO:
                default:
                     err = OS_ERR_TIMEOUT;
                     break;
            }
            OSExitCriticalSection();
            return err;
        }
    } else {
        pqueue->OSQueueReadToken--;  
    }

    *(void const **)buffer = pqueue->OSQueueBuffer[pqueue->OSQueueHead];
    pqueue->OSQueueHead++;
    if (pqueue->OSQueueHead >= pqueue->OSQueueSize) {
        pqueue->OSQueueHead = 0;
    }
    pqueue->OSQueueLength--;
    pqueue->OSQueueWriteToken++;
    if (!OS_LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList)) { //!< Is any task waiting for writing?
        pqueue->OSQueueWriteToken--;
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueEnqueueWaitList, OS_STAT_PEND_OK);
        OSExitCriticalSection();
        OS_SchedulerRunPrio();
        return OS_ERR_NONE;
    }
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

#if OS_QUEUE_QUERY_EN > 0u
OS_ERR osQueueQuery(OS_HANDLE hQueue, OS_QUEUE_INFO *pInfo)
{
    OS_QUEUE     *pqueue = (OS_QUEUE *)hQueue;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (pInfo == NULL) {
        return OS_ERR_PDATA_NULL;
    }
#endif
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) {    //!< Validate object's type
        return OS_ERR_OBJ_TYPE;
    }

    OSEnterCriticalSection();
    pInfo->OSQueueInfoLength    = pqueue->OSQueueLength;
    pInfo->OSQueueInfoHead      = pqueue->OSQueueHead;
    pInfo->OSQueueInfoTail      = pqueue->OSQueueTail;
    pInfo->OSQueueInfoSize      = pqueue->OSQueueSize;
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}
#endif      //!< #if OS_QUEUE_QUERY_EN > 0u

#endif      //!< #if (OS_QUEUE_EN > 0u) && (OS_MAX_QUEUES > 0u)

