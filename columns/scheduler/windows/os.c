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
#include "./app_cfg.h"
#include "./os.h"
#include "../../timer/timer.h"
#include "../../clock/clock.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void os_timer_timeout_callback(timer_t *timer);

/*============================ GLOBAL VARIABLES ==============================*/
CRITICAL_SECTION __globalCriticalSection;

/*============================ LOCAL VARIABLES ===============================*/
static HANDLE hTimer = NULL;
static HANDLE hTimerQueue = NULL;

/*============================ IMPLEMENTATION ================================*/


static VOID CALLBACK timer_routine(PVOID lpParam, BOOLEAN isTimeOut)
{
    if (isTimeOut) {
        timer_tick();
    }
}

static VOID CALLBACK clock_routine(PVOID lpParam, BOOLEAN isTimeOut)
{
    if (isTimeOut) {
        clock_tick_tock();
    }
}


static bool os_tick_start(void)
{
    // Create the timer queue.
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue) {
        return false;
    }

    // Set a timer to call the timer routine every 10 milliseconds.
    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, timer_routine, NULL, 10, 10, 0)) {
        return false;
    }

    // Set a timer to call the timer routine every 1 second.
    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, clock_routine, NULL, 1000, 1000, 0)) {
        return false;
    }

    return true;
}

static bool os_tick_stop(void)
{
    // Delete all timers in the timer queue and the queue itself.
    if (!DeleteTimerQueueEx(hTimerQueue, NULL)) {
        printf("DeleteTimerQueue failed (%d)\n", GetLastError());
    }

    return true;
}

bool osInit (void)
{
    //! init critical section.
    InitializeCriticalSectionAndSpinCount(&__globalCriticalSection, 0x00000400);

    //! init system timer.
    if (!timer_init(&os_timer_timeout_callback)) {
        return false;
    }

    //! start system tick-clock.
    if (!os_tick_start()) {
        return false;
    }

    return true;
}




OS_ERR osTaskDelay (UINT32 timeMS)
{
    Sleep(timeMS);
    return OS_ERR_NONE;
}

OS_ERR osTaskCreat(OS_HANDLE *pTaskHandle, void *entry, void *arg)
{
    OS_HANDLE taskHandle = NULL;
    taskHandle = CreateThread( NULL, 0, entry, arg, 0, NULL);
    *pTaskHandle = taskHandle;
    if (taskHandle == NULL) {
        return OS_ERR_OUT_OF_MEMORY;
    }
    CloseHandle(taskHandle);

    return OS_ERR_NONE;
}




OS_ERR osSemCreate (OS_HANDLE *pSemaphoreHandle, UINT16 initialCount)
{
    HANDLE hSem;

    hSem = CreateSemaphore(NULL, initialCount, 0x7FFFFFFFu, NULL);
    if (hSem == NULL) {
        *pSemaphoreHandle = NULL;
        return OS_ERR_OUT_OF_MEMORY;
    } else {
        *pSemaphoreHandle = hSem;
        return OS_ERR_NONE;
    }
}

OS_ERR osSemDelete (OS_HANDLE hSemaphore, UINT8 opt)
{
    CloseHandle(hSemaphore);
    return OS_ERR_NONE;
}

OS_ERR osSemPost (OS_HANDLE hSemaphore, UINT16 cnt)
{
    ReleaseSemaphore(hSemaphore, cnt, NULL);
    return OS_ERR_NONE;
}

OS_ERR osSemPend (OS_HANDLE hSemaphore, UINT32 timeMS)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hSemaphore, timeMS);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}




OS_ERR osMutexCreate (OS_HANDLE *pMutexHandle, UINT8 ceilingPrio)
{
    OS_HANDLE hMutex;

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        *pMutexHandle = NULL;
        return OS_ERR_OUT_OF_MEMORY;
    } else {
        *pMutexHandle = hMutex;
        return OS_ERR_NONE;
    }
}

OS_ERR osMutexDelete (OS_HANDLE hMutex, UINT8 opt)
{
    CloseHandle(hMutex);
    return OS_ERR_NONE;
}

OS_ERR osMutexPend (OS_HANDLE hMutex, UINT32 timeMS)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hMutex, timeMS);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}

OS_ERR osMutexPost (OS_HANDLE hMutex)
{
    ReleaseMutex(hMutex);
    return OS_ERR_NONE;
}




OS_ERR osFlagCreate (OS_HANDLE *pFlagHandle, BOOLEAN init, BOOLEAN manual)
{
    OS_HANDLE hFlag;

    hFlag = CreateEvent(NULL, manual, init, NULL);
    if (hFlag == NULL) {
        *pFlagHandle = NULL;
        return OS_ERR_OUT_OF_MEMORY;
    } else {
        *pFlagHandle = hFlag;
        return OS_ERR_NONE;
    }
}

OS_ERR osFlagDelete (OS_HANDLE hFlag, UINT8 opt)
{
    CloseHandle(hFlag);
    return OS_ERR_NONE;
}

OS_ERR osFlagPend (OS_HANDLE hFlag, UINT32 timeMS)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hFlag, timeMS);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        retValue = GetLastError();
        return OS_ERR_PEND_ABORT;
    }
}

OS_ERR osFlagSet (OS_HANDLE hFlag)
{
    SetEvent(hFlag);
    return OS_ERR_NONE;
}

OS_ERR osFlagReset (OS_HANDLE hFlag)
{
    ResetEvent(hFlag);
    return OS_ERR_NONE;
}




OS_ERR osQueueCreate (OS_HANDLE *pQueueHandle, size_t size, size_t elementSize)
{
    HANDLE hSem;
    OS_ERR retVal = OS_ERR_NONE;
    os_queue_t *pQueue;

    pQueue = malloc(sizeof(os_queue_t));
    if (NULL == pQueue) {
        return OS_ERR_OUT_OF_MEMORY;
    }

    hSem = CreateSemaphore(NULL, size, 6000u, NULL);
    if (hSem == NULL) {
        retVal = OS_ERR_OUT_OF_MEMORY;
        goto __writesem_err_exit;
    } else {
        pQueue->WriteSem = hSem;
    }

    hSem = CreateSemaphore(NULL, 0, 6000u, NULL);
    if (hSem == NULL) {
        retVal = OS_ERR_OUT_OF_MEMORY;
        goto __readsem_err_exit;
    } else {
        pQueue->ReadSem = hSem;
    }

    void *buffer = malloc(size * elementSize);
    if (buffer == NULL) {
        retVal = OS_ERR_OUT_OF_MEMORY;
        goto __malloc_err_exit;
    }

    pQueue->QueueHead   = 0;
    pQueue->QueueTail   = 0;
    pQueue->QueueBuffer = buffer;
    pQueue->QueueSize   = size;
    pQueue->QueueLength = 0;
    pQueue->QueueElementSize = elementSize;


    *pQueueHandle = pQueue;
    return OS_ERR_NONE;

__malloc_err_exit:
    CloseHandle(pQueue->ReadSem);
__readsem_err_exit:
    CloseHandle(pQueue->WriteSem);
__writesem_err_exit:
    return retVal;
}

OS_ERR osQueueDelete (OS_HANDLE hQueue)
{
    os_queue_t *pQueue = (os_queue_t *)hQueue;
    OS_CRITICAL_SECTION_BEGIN();
    CloseHandle(pQueue->ReadSem);
    CloseHandle(pQueue->WriteSem);
    pQueue->QueueHead = 0;
    pQueue->QueueTail = 0;
    pQueue->QueueBuffer = NULL;
    pQueue->QueueSize = 0;
    pQueue->QueueLength = 0;
    pQueue->QueueElementSize = 0;
    OS_CRITICAL_SECTION_END();
    return OS_ERR_NONE;
}

OS_ERR osQueueWrite (OS_HANDLE hQueue, void *buffer, UINT32 timeMS)
{
    os_queue_t *pQueue = (os_queue_t *)hQueue;
    DWORD retValue;

    retValue = WaitForSingleObject(pQueue->WriteSem, timeMS);
    if (retValue == WAIT_OBJECT_0) {
        OS_CRITICAL_SECTION_BEGIN();
        memcpy((char*)pQueue->QueueBuffer + pQueue->QueueTail * pQueue->QueueElementSize,
               buffer,
               pQueue->QueueElementSize);
        pQueue->QueueTail++;
        if (pQueue->QueueTail >= pQueue->QueueSize) {
            pQueue->QueueTail = 0;
        }
        pQueue->QueueLength++;
        ReleaseSemaphore(pQueue->ReadSem, 1, NULL);
        OS_CRITICAL_SECTION_END();
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}

OS_ERR osQueueRead (OS_HANDLE hQueue, void *buffer, UINT32 timeMS)
{
    os_queue_t *pQueue = (os_queue_t *)hQueue;
    DWORD retValue;

    retValue = WaitForSingleObject(pQueue->ReadSem, timeMS);
    if (retValue == WAIT_OBJECT_0) {
        OS_CRITICAL_SECTION_BEGIN();
        memcpy(buffer,
               (char*)pQueue->QueueBuffer + pQueue->QueueHead * pQueue->QueueElementSize,
               pQueue->QueueElementSize);
        pQueue->QueueHead++;
        if (pQueue->QueueHead >= pQueue->QueueSize) {
            pQueue->QueueHead = 0;
        }
        pQueue->QueueLength--;
        ReleaseSemaphore(pQueue->WriteSem, 1, NULL);
        OS_CRITICAL_SECTION_END();
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}



typedef struct os_timer_t OS_TIMER;

struct os_timer_t {
    timer_t             OSTimerData;
    OS_TIMER_ROUTINE   *OSTimerRoutine;
    void               *OSTimerRoutineArg;
    UINT16              OSTimerOpt;
};

static void os_timer_timeout_callback(timer_t *timer)
{
    OS_TIMER *osTimer;
    
    osTimer = CONTAINER_OF(timer, OS_TIMER, OSTimerData);

    if (NULL != osTimer->OSTimerRoutine) {
        osTimer->OSTimerRoutine(osTimer->OSTimerRoutineArg);
    }
    
    if (osTimer->OSTimerOpt & OS_TIMER_OPT_AUTO_DELETE) {
        free(osTimer);
    }
}

OS_ERR osTimerCreat(OS_HANDLE          *pTimerHandle,
                    UINT32		        initValue,
                    UINT32		        reloadValue,
                    OS_TIMER_ROUTINE   *fnRoutine,
                    void               *RoutineArg,
                    UINT16              opt)
{
    OS_TIMER *timer = NULL;

    timer = malloc(sizeof(OS_TIMER));
    if (timer == NULL) {
        return OS_ERR_OUT_OF_MEMORY;
    }

    initValue /= 10u;
    reloadValue /= 10u;

    timer->OSTimerOpt = 0;
    if (reloadValue == 0u) {
        if (opt & OS_TIMER_OPT_AUTO_DELETE) {
            timer->OSTimerOpt |= OS_TIMER_OPT_AUTO_DELETE;
        }
    }
    timer->OSTimerRoutine       = fnRoutine;
    timer->OSTimerRoutineArg    = RoutineArg;
    OS_CRITICAL_SECTION_BEGIN();
    timer_config(&timer->OSTimerData, initValue, reloadValue);
    OS_CRITICAL_SECTION_END();
    *pTimerHandle = timer;

    return OS_ERR_NONE;
}

OS_ERR osTimerDelete(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;

    OS_CRITICAL_SECTION_BEGIN();
    timer_stop(&timer->OSTimerData);
    free(timer);
    OS_CRITICAL_SECTION_END();

    return OS_ERR_NONE;
}

OS_ERR osTimerStart(OS_HANDLE hTimer, UINT32 timeMS)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    timeMS /= 10u;

    OS_CRITICAL_SECTION_BEGIN();
    timer_start(&timer->OSTimerData, timeMS);
    OS_CRITICAL_SECTION_END();
    
    return OS_ERR_NONE;
}

OS_ERR osTimerStop(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    OS_CRITICAL_SECTION_BEGIN();
    timer_stop(&timer->OSTimerData);
    OS_CRITICAL_SECTION_END();
    
    return OS_ERR_NONE;
}


/* EOF */
