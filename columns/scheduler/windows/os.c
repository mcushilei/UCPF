/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#define __OS_WINDOWS_OS_C__

/*============================ INCLUDES ======================================*/
#include ".\api\os.h"
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define MS_PER_TICK                 (10u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
enum {
    OS_ERR_NONE                     = 0u,

    OS_ERR_EVENT_TYPE               = 1u,
    OS_ERR_PDATA_NULL               = 2u,
    OS_ERR_INVALID_HANDLE           = 3u,
    OS_ERR_INVALID_OPT              = 4u,
    OS_ERR_DEL_ISR                  = 5u,
    OS_ERR_CREATE_ISR               = 6u,
    OS_ERR_INVALID_TASK_HANDLE      = 7u,

    OS_ERR_TIMEOUT                  = 30u,
    OS_ERR_PEND_ISR                 = 31u,
    OS_ERR_PEND_LOCKED              = 32u,
    OS_ERR_PEND_ABORT               = 33u,
    OS_ERR_POST_ISR                 = 34u,
    OS_ERR_TASK_WAITING             = 35u,

    OS_ERR_TASK_DEPLETED            = 60u,
    OS_ERR_TASK_OPT                 = 61u,
    OS_ERR_TASK_EXIST               = 62u,
    OS_ERR_TASK_NOT_EXIST           = 63u,
    OS_ERR_INVALID_PRIO             = 64u,

    OS_ERR_FLAG_DEPLETED            = 80u,

    OS_ERR_EVENT_DEPLETED           = 90u,
    OS_ERR_SEM_OVF                  = 91u,
    OS_ERR_NOT_MUTEX_OWNER          = 92u,
    OS_ERR_HAS_OWN_MUTEX            = 93u,
};


typedef uint8_t OS_ERR;
typedef HANDLE  OS_HANDLE;

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
CRITICAL_SECTION __globalCriticalSection;

/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
void        osInit                 (void)
{
    InitializeCriticalSectionAndSpinCount(&__globalCriticalSection, 0x00000400);
}

void        osTimeDelay            (UINT32          ticks)
{
    Sleep(MS_PER_TICK * ticks);
}


OS_ERR      osSemCreate            (OS_HANDLE      *pSemaphoreHandle,
                                    UINT16          initialCount)
{
    HANDLE hSem;

    hSem = CreateSemaphore(NULL, initialCount, 0x7FFFFFFFu, NULL);
    if (hSem == NULL) {
        *pSemaphoreHandle = NULL;
        return OS_ERR_EVENT_DEPLETED;
    } else {
        *pSemaphoreHandle = hSem;
        return OS_ERR_NONE;
    }
}

OS_ERR      osSemDelete            (OS_HANDLE       hSemaphore,
                                    UINT8           opt)
{
    CloseHandle(hSemaphore);
    return OS_ERR_NONE;
}

OS_ERR      osSemPost              (OS_HANDLE       hSemaphore,
                                    UINT16          cnt)
{
    ReleaseSemaphore(hSemaphore, cnt, NULL);
    return OS_ERR_NONE;
}

OS_ERR      osSemPend              (OS_HANDLE       hSemaphore,
                                    UINT32          timeout)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hSemaphore, MS_PER_TICK * timeout);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}




OS_ERR      osMutexCreate          (OS_HANDLE      *pMutexHandle,
                                    UINT8           ceilingPrio)
{
    OS_HANDLE hMutex;

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        *pMutexHandle = NULL;
        return OS_ERR_EVENT_DEPLETED;
    } else {
        *pMutexHandle = hMutex;
        return OS_ERR_NONE;
    }
}

OS_ERR      osMutexDelete          (OS_HANDLE       hMutex,
                                    UINT8           opt)
{
    CloseHandle(hMutex);
    return OS_ERR_NONE;
}

OS_ERR      osMutexPend            (OS_HANDLE       hMutex,
                                    UINT32          timeout)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hMutex, MS_PER_TICK * timeout);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        return OS_ERR_PEND_ABORT;
    }
}

OS_ERR      osMutexPost            (OS_HANDLE       hMutex)
{
    ReleaseMutex(hMutex);
    return OS_ERR_NONE;
}




OS_ERR      osFlagCreate           (OS_HANDLE      *pFlagHandle,
                                    BOOLEAN         init,
                                    BOOLEAN         manual)
{
    OS_HANDLE hFlag;

    hFlag = CreateEvent(NULL, manual, init, NULL);
    if (hFlag == NULL) {
        *pFlagHandle = NULL;
        return OS_ERR_EVENT_DEPLETED;
    } else {
        *pFlagHandle = hFlag;
        return OS_ERR_NONE;
    }
}

OS_ERR      osFlagDelete           (OS_HANDLE       hFlag,
                                    UINT8           opt)
{
    CloseHandle(hFlag);
    return OS_ERR_NONE;
}

OS_ERR      osFlagPend             (OS_HANDLE       hFlag,
                                    UINT32          timeout)
{
    DWORD retValue;

    retValue = WaitForSingleObject(hFlag, MS_PER_TICK * timeout);
    if (retValue == WAIT_OBJECT_0) {
        return OS_ERR_NONE;
    } else if (retValue == WAIT_TIMEOUT) {
        return OS_ERR_TIMEOUT;
    } else {
        retValue = GetLastError();
        return OS_ERR_PEND_ABORT;
    }
}

OS_ERR      osFlagSet              (OS_HANDLE       hFlag)
{
    SetEvent(hFlag);
    return OS_ERR_NONE;
}

OS_ERR      osFlagReset            (OS_HANDLE       hFlag)
{
    ResetEvent(hFlag);
    return OS_ERR_NONE;
}

/* EOF */
