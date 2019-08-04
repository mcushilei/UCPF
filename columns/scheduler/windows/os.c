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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\os.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
CRITICAL_SECTION __globalCriticalSection;

/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
void        osInit                 (void)
{
    InitializeCriticalSectionAndSpinCount(&__globalCriticalSection, 0x00000400);
}

void        osTaskDelay            (UINT32          timeout)
{
    Sleep(MS_PER_TICK * timeout);
    return OS_ERR_NONE;
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
