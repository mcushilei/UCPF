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


#ifndef __OS_WINDOWS_OS_H__
#define __OS_WINDOWS_OS_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define OS_CRITICAL_SECTION_DEFINE()    
#define OS_CRITICAL_SECTION_BEGIN()         EnterCriticalSection(&__globalCriticalSection)
#define OS_CRITICAL_SECTION_END()           LeaveCriticalSection(&__globalCriticalSection)


#define OS_TIMER_TYPE                       OS_HANDLE
#define OS_TIMER_CREATE(__TMR, __INIT, __RELOAD, __ROUTINE, __ARG)\
    osTimerCreat(&(__TMR), __INIT, __RELOAD, __ROUTINE, (void *)(__ARG), OS_TIMER_OPT_AUTO_DELETE)
#define OS_TIMER_CREATE_EX(__TMR, __INIT, __RELOAD, __ROUTINE, __ARG, __OP)\
    osTimerCreat(&(__TMR), __INIT, __RELOAD, __ROUTINE, (void *)(__ARG), __OP)
#define OS_TIMER_DELETE(__TMR)              osTimerDelete(__TMR)
#define OS_TIMER_START(__TMR, __TIME)       osTimerStart(__TMR, __TIME)
#define OS_TIMER_STOP(__TMR)                osTimerStop(__TMR)


#define OS_QUEUE_TYPE                       OS_HANDLE
#define OS_QUEUE_CREATE(__Q, __L, __SIZE)   osQueueCreate(&(__Q), __L, __SIZE)
#define OS_QUEUE_DELETE(__Q)                osSemDelete(__Q)
#define OS_QUEUE_WRITE(__Q, __BUF, __TIME)  osQueueWrite(__Q, __BUF, __TIME)
#define OS_QUEUE_READ(__Q, __BUF, __TIME)   osQueueRead(__Q, __BUF, __TIME)


#define OS_SEMAPHORE_TYPE                   OS_HANDLE
#define OS_SEMAPHORE_CREATE(__SEM, __CNT)   osSemCreate(&(__SEM), __CNT)
#define OS_SEMAPHORE_DELETE(__SEM)          osSemDelete(__SEM, 0)
#define OS_SEMAPHORE_RELEASE(__SEM)         osSemPost(__SEM, 1u)
#define OS_SEMAPHORE_WAIT(__SEM, __TIME)    osSemPend(__SEM, __TIME)


#define OS_MUTEX_TYPE                       OS_HANDLE
#define OS_MUTEX_CREATE(__MUTEX)            osMutexCreate(&(__MUTEX), 0)
#define OS_MUTEX_DELETE(__MUTEX)            osMutexDelete(__MUTEX, 0)
#define OS_MUTEX_RELEASE(__MUTEX)           osMutexPost(__MUTEX)
#define OS_MUTEX_WAIT(__MUTEX, __TIME)      osMutexPend(__MUTEX, __TIME)


#define OS_FLAG_TYPE                        OS_HANDLE
#define OS_FLAG_CREATE(__FLAG, __BMANUAL, __BINITVAL) osFlagCreate(&(__FLAG), __BINITVAL, __BMANUAL)
#define OS_FLAG_DELETE(__FLAG)              osFlagDelete(__FLAG, 0)
#define OS_FLAG_SET(__FLAG)                 osFlagSet(__FLAG)
#define OS_FLAG_RESET(__FLAG)               osFlagReset(__FLAG)
#define OS_FLAG_WAIT(__FLAG, __TIME)        osFlagPend(__FLAG, __TIME)


#define OS_TASK_CREATE(__HANDLE, __NAME, __ENTRY, __ARG,  __STACK_SIZE, __RIO, __OPT)  \
    osTaskCreat(&(__HANDLE), __ENTRY, (void *)(__ARG))
#define OS_TASK_ENTRY(__TASK)               DWORD WINAPI __TASK(void *pArg)
#define OS_TASK_ARG                         (pArg)
#define OS_TASK_SLEEP(__T)                  osTaskDelay(__T)
#define OS_TASK_EXIT(__V)                   return (DWORD)(__V)


#define OS_INFINITE                         INFINITE


#define OS_TIMER_OPT_AUTO_DELETE        (0x0001)

/*============================ TYPES =========================================*/
enum {
    OS_ERR_NONE                     = 0u,

    OS_ERR_INVALID_HANDLE,
    OS_ERR_INVALID_OBJ,
    OS_ERR_INVALID_OPT,
    OS_ERR_OUT_OF_MEMORY,

    OS_ERR_TIMEOUT                  = 30u,
    OS_ERR_IN_ISR,
    OS_ERR_IN_LOCKED,
    OS_ERR_IN_CRITICAL_SECTION,
    
    OS_ERR_PEND_ABORT               = 40u,

    OS_ERR_SEM_OVERFLOW             = 91u,
    OS_ERR_NOT_MUTEX_OWNER          = 92u,
};

typedef uint8_t OS_ERR;
typedef HANDLE  OS_HANDLE;

typedef struct {
    HANDLE  WriteSem;
    HANDLE  ReadSem;

    size_t  QueueHead;
    size_t  QueueTail;
    size_t  QueueSize;
    size_t  QueueLength;
    size_t  QueueElementSize;
    void    *QueueBuffer;
} os_queue_t;

typedef void OS_TIMER_ROUTINE(void *arg);

/*============================ PUBLIC VARIABLES ==============================*/
extern CRITICAL_SECTION __globalCriticalSection;

/*============================ PUBLIC PROTOTYPES =============================*/
extern bool   osInit        (void);

extern OS_ERR osTaskDelay   (UINT32 timeMS);
extern OS_ERR osTaskCreat   (OS_HANDLE *pTaskHandle, void *entry, void *arg);


extern OS_ERR osFlagCreate  (OS_HANDLE *pFlagHandle, BOOLEAN init, BOOLEAN manual);
extern OS_ERR osFlagDelete  (OS_HANDLE hFlag, UINT8 opt);
extern OS_ERR osFlagPend    (OS_HANDLE hFlag, UINT32 timeMS);
extern OS_ERR osFlagSet     (OS_HANDLE hFlag);
extern OS_ERR osFlagReset   (OS_HANDLE hFlag);


extern OS_ERR osMutexCreate (OS_HANDLE *pMutexHandle, UINT8 ceilingPrio);
extern OS_ERR osMutexDelete (OS_HANDLE hMutex, UINT8 opt);
extern OS_ERR osMutexPend   (OS_HANDLE hMutex, UINT32 timeMS);
extern OS_ERR osMutexPost   (OS_HANDLE hMutex);


extern OS_ERR osSemCreate   (OS_HANDLE *pSemaphoreHandle, UINT16 cnt);
extern OS_ERR osSemDelete   (OS_HANDLE hSemaphore, UINT8 opt);
extern OS_ERR osSemPend     (OS_HANDLE hSemaphore, UINT32 timeMS);
extern OS_ERR osSemPost     (OS_HANDLE hSemaphore, UINT16 cnt);


extern OS_ERR osQueueCreate (OS_HANDLE *pQueueHandle, size_t size, size_t elementSize);
extern OS_ERR osQueueDelete (OS_HANDLE hQueue);
extern OS_ERR osQueueWrite  (OS_HANDLE hQueue, void *buffer, UINT32 timeMS);
extern OS_ERR osQueueRead   (OS_HANDLE hQueue, void *buffer, UINT32 timeMS);


extern OS_ERR osTimerCreat( OS_HANDLE          *pTimerHandle,
                            UINT32		        initValue,
                            UINT32		        reloadValue,
                            OS_TIMER_ROUTINE   *fnRoutine,
                            void               *RoutineArg,
                            UINT16              opt);
extern OS_ERR osTimerDelete (OS_HANDLE hTimer);
extern OS_ERR osTimerStart  (OS_HANDLE hTimer, UINT32 timeMS);
extern OS_ERR osTimerStop   (OS_HANDLE hTimer);

extern void  *osHeapMalloc  (size_t size);
extern void  *osHeapRealloc (void *mem, size_t size);
extern void   osHeapFree    (void *mem);

#endif
/* EOF */
