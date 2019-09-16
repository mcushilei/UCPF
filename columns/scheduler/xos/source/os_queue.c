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
#include <string.h>

#if OS_QUEUE_EN > 0u

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
/*
 *  \brief      CREATE A QUEUE
 * 
 *  \param      hQueue      the pointer to a handle to the queue.
 * 
 *              buffer      the buffer of the queue. If it is NULL and OS_QUEUE_BUFFER_ON_HEAP_EN
 *                          is enabled, the buffer will be allocated on the heap.
 * 
 *              queueSize   the maximum LENGTH of the queue. This is NOT the SIZEOF() the buffer.
 * 
 *              elementSize The size of the element stored in the queue.
 * 
 *  \return     OS_ERR_NONE            The call was successful.
 *              OS_ERR_INVALID_OPT     If 'queueSize' is 0.
 *              OS_ERR_NULL_POINTER    If 'buffer' is NULL when OS_QUEUE_BUFFER_ON_HEAP_EN is disabled.
 *              OS_ERR_USE_IN_ISR      Being called in an ISR.
 *              OS_ERR_OUT_OF_MEMORY   There is no queue object to use or the heap cannot get enough memory for the buffer.
 */
OS_ERR osQueueCreate(OS_HANDLE *hQueue, void *buffer, UINT16 queueSize, size_t elementSize)
{
    OS_QUEUE   *pqueue;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#if OS_QUEUE_BUFFER_ON_HEAP_EN == 0u
    if (NULL == buffer) {
        return OS_ERR_NULL_POINTER;
    }
#endif
    if (0u == queueSize) {
        return OS_ERR_INVALID_OPT;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... Should not create object from an ISR.
    }

    OSEnterCriticalSection();
    pqueue = pool_new(&osQueueFreePool);
    if (pqueue == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OUT_OF_MEMORY;
    }
    pqueue->OSQueueOpt = 0;
#if OS_QUEUE_BUFFER_ON_HEAP_EN > 0u
    if (NULL == buffer) {
        buffer = OSHeapAlloc(elementSize * queueSize);
        if (buffer == NULL) {
            pool_free(&osQueueFreePool, pqueue);
            OSExitCriticalSection();
            return OS_ERR_OUT_OF_MEMORY;
        }
        pqueue->OSQueueOpt |= OS_QUEUE_BUFFER_ON_HEAP;
    }
#endif
    OSExitCriticalSection();

    //! Set object type.
    //! Init wait list head.
    //! Init queue values.
    pqueue->OSQueueObjHead.OSObjType = OS_OBJ_TYPE_SET(OS_OBJ_TYPE_QUEUE)
                                     | OS_OBJ_TYPE_WAITABLE_MSK
                                     | OS_OBJ_PRIO_TYPE_SET(OS_OBJ_PRIO_TYPE_LIST);
    list_init(&pqueue->OSQueueEnqueueWaitList);
    list_init(&pqueue->OSQueueDequeueWaitList);
    pqueue->OSQueueElementSize  = elementSize;
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

/*
 *  \brief      STOP A QUEUE
 * 
 *  \remark     This will stop the queue. Any osQueueWrite() after this will return
 *              OS_ERR_PEND_ABORT but osQueueRead() will not be effected. To elegantly
 *              delete a queue:
 *              1) the producer(s) should stop writing the queue;
 *              2) the consumer shall call osQueueStopEnqueue() to stop the queue;
 *              3) the consumer may optionally call osQueueRead() till the queue becomes empty in
 *                 order to free the resource in the buffer.
 *              4) delete the queue by calling osQueueDelete().
 *              CAUTION: once a queue is stopped, there is no way to rturn it to normal status.
 * 
 *  \param      hQueue      the handle to the queue.
 * 
 *  \return     OS_ERR_NONE            The call was successful.
 *              OS_ERR_INVALID_HANDLE  If 'hQueue' is an invalid handle.
 *              OS_ERR_OBJ_TYPE        If you didn't pass a queue object.
 */
#if OS_QUEUE_DEL_EN > 0
OS_ERR osQueueStopEnqueue(OS_HANDLE *hQueue)
{
    OS_QUEUE   *pqueue = (OS_QUEUE *)*hQueue;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... can't use from an ISR
    }


    OSEnterCriticalSection();
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        OSExitCriticalSection();
        return OS_ERR_OBJ_TYPE;
    }
    pqueue->OSQueueOpt |= OS_QUEUE_STOP_ENQUEUE;
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}

/*
 *  \brief      DELETE A QUEUE
 * 
 *  \remark     This function delete the queue. The data in the buffer, if any, will be lost. In the situration
 *              where this is a matter, osQueueStopEnqueue() would be called before calling osQueueDelete(). 
 * 
 *  \param      hQueue      the handle to the queue.
 * 
 *              opt         determines delete options, one of follows:
 *                          opt == OS_DEL_NOT_IN_USE    Delete the queue only if it is empyt.
 *                          opt == OS_DEL_ALWAYS        Delete the queue no matter the queue is empty or not.
 * 
 *  \return     OS_ERR_NONE             The call was successful.
 *              OS_ERR_INVALID_HANDLE   If 'hQueue' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a queue object.
 *              OS_ERR_DELETE_IN_USE    One or more tasks are waiting on the queue.
 */
OS_ERR osQueueDelete(OS_HANDLE hQueue, UINT16 opt)
{
    OS_QUEUE   *pqueue = (OS_QUEUE *)hQueue;
    BOOL        taskPend;


#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
#endif
    if (osIntNesting > 0u) {            //!< See if called from ISR ...
        return OS_ERR_USE_IN_ISR;       //!< ... can't DELETE from an ISR
    }


    OSEnterCriticalSection();
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< make sure that it is a queue.
        OSExitCriticalSection();
        return OS_ERR_OBJ_TYPE;
    }
    if (!LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList) || !LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) {     //!< check wait list if it's empty.
        taskPend    = TRUE;
    } else {
        taskPend    = FALSE;
    }
    switch (opt) {
        case OS_DEL_NOT_IN_USE:
            if (pqueue->OSQueueLength != 0u) {
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
    while (!LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList)) {  //!< Ready ALL suspended tasks.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueEnqueueWaitList, OS_STAT_PEND_ABORT);
    }
    while (!LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) {  //!< Ready ALL suspended tasks.
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueDequeueWaitList, OS_STAT_PEND_ABORT);
    }
    
    pqueue->OSQueueObjHead.OSObjType = OS_OBJ_TYPE_UNUSED;
#if OS_QUEUE_BUFFER_ON_HEAP_EN > 0u
    if (pqueue->OSQueueOpt & OS_QUEUE_BUFFER_ON_HEAP) {
        OSHeapFree(pqueue->OSQueueBuffer);
    }
#endif
    pool_free(&osQueueFreePool, pqueue);
    OSExitCriticalSection();
    
    if (taskPend) {
        OS_SchedulerRunPrio();
    }
    
    return OS_ERR_NONE;
}
#endif

/*
 *  \brief      ENQUEUE
 * 
 *  \param      hQueue      the handle to the queue.
 *              buffer      the buffer to store the enqueued data.
 *              timeout     the time trying to wait if queue is full.
 * 
 *  \return     OS_ERR_NONE             The call was successful.
 *              OS_ERR_INVALID_HANDLE   If 'hQueue' is an invalid handle.
 *              OS_ERR_OBJ_TYPE         If you didn't pass a queue object.
 *              OS_ERR_PEND_LOCKED      If you called this function when the scheduler is locked.
 *              OS_ERR_PEND_ABORT       The queue has been shutdown.
 *              OS_ERR_TIMEOUT          The queue is full till 'timeout' has expired.
 */
OS_ERR osQueueWrite(OS_HANDLE hQueue, const void *buffer, UINT32 timeout)
{
    OS_QUEUE       *pqueue = (OS_QUEUE *)hQueue;
    OS_WAIT_NODE    node;
    OS_ERR          err;
    

#if OS_ARG_CHK_EN > 0u
    if (hQueue == NULL) {
        return OS_ERR_INVALID_HANDLE;
    }
    if (NULL == buffer) {
        return OS_ERR_NULL_POINTER;
    }
#endif
    if (osIntNesting > 0u && timeout != 0u) {       //!< it's not possible to pend a ISR.
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u && timeout != 0u) {      //!< See if it tries to pend ourself when scheduler is locked.
        return OS_ERR_PEND_LOCKED;
    }


    OSEnterCriticalSection();
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        OSExitCriticalSection();
        return OS_ERR_OBJ_TYPE;
    }
    if (pqueue->OSQueueOpt & OS_QUEUE_STOP_ENQUEUE) {
        OSExitCriticalSection();
        return OS_ERR_PEND_ABORT;
    }
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

    memcpy((char *)pqueue->OSQueueBuffer + pqueue->OSQueueElementSize * pqueue->OSQueueTail, buffer, pqueue->OSQueueElementSize);
    pqueue->OSQueueTail++;
    if (pqueue->OSQueueTail >= pqueue->OSQueueSize) {
        pqueue->OSQueueTail = 0u;
    }
    pqueue->OSQueueLength++;
    pqueue->OSQueueReadToken++;
    if (!LIST_IS_EMPTY(pqueue->OSQueueDequeueWaitList)) { //!< Is any task waiting for reading?
        pqueue->OSQueueReadToken--;
        OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pqueue, &pqueue->OSQueueDequeueWaitList, OS_STAT_PEND_OK);
        OSExitCriticalSection();
        OS_SchedulerRunPrio();
        return OS_ERR_NONE;
    }
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

/*
 *  \brief      DEQUEUE
 * 
 *  \param      hQueue      the handle to the queue.
 *              buffer      the buffer to store the dequeued data.
 *              timeout     the time trying to wait if queue is empty.
 * 
 *  \return     OS_ERR_NONE             The call was successful.
 *              OS_ERR_INVALID_HANDLE   hQueue is an invalid handle.
 *              OS_ERR_OBJ_TYPE         hQueue is not a handle to a queue object.
 *              OS_ERR_USE_IN_ISR       If you called this function from an ISR.
 *              OS_ERR_PEND_LOCKED      If you called this function when the scheduler is locked.
 *              OS_ERR_PEND_ABORT       The queue has been shutdown.
 *              OS_ERR_TIMEOUT          The queue is empty till 'timeout' has expired.
 */
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
        return OS_ERR_NULL_POINTER;
    }
#endif
    if (osIntNesting > 0u) {                    //!< ISRs should not be consumers.
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting > 0u && timeout != 0u) {  //!< See if we try to be suspended with scheduler locked.
        return OS_ERR_PEND_LOCKED;
    }

    
    OSEnterCriticalSection();
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) { //!< Validate object's type
        OSExitCriticalSection();
        return OS_ERR_OBJ_TYPE;
    }
    if (pqueue->OSQueueReadToken == 0u) {       //!< queue is empty.
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

    memcpy(buffer, (char *)pqueue->OSQueueBuffer + pqueue->OSQueueElementSize * pqueue->OSQueueHead, pqueue->OSQueueElementSize);
    pqueue->OSQueueHead++;
    if (pqueue->OSQueueHead >= pqueue->OSQueueSize) {
        pqueue->OSQueueHead = 0u;
    }
    pqueue->OSQueueLength--;
    pqueue->OSQueueWriteToken++;
    if (!LIST_IS_EMPTY(pqueue->OSQueueEnqueueWaitList)) { //!< Is any task waiting for writing?
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
        return OS_ERR_NULL_POINTER;
    }
#endif

    OSEnterCriticalSection();
    if (OS_OBJ_TYPE_GET(pqueue->OSQueueObjHead.OSObjType) != OS_OBJ_TYPE_QUEUE) {    //!< Validate object's type
        OSExitCriticalSection();
        return OS_ERR_OBJ_TYPE;
    }
    pInfo->OSQueueInfoLength    = pqueue->OSQueueLength;
    pInfo->OSQueueInfoHead      = pqueue->OSQueueHead;
    pInfo->OSQueueInfoTail      = pqueue->OSQueueTail;
    pInfo->OSQueueInfoSize      = pqueue->OSQueueSize;
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}
#endif      //!< #if OS_QUEUE_QUERY_EN > 0u

#endif      //!< #if OS_QUEUE_EN > 0u

