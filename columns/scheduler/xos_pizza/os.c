/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include "../xos/source/os_private.h"
#include "../xos/source/os_port.h"

#include "../../heap_memory/heap_first_fit/heap_first_fit.h"
#include <string.h>
#include "../../timer/timer.h"
#include "./os.h"

/*============================ MACROS ========================================*/
#define OS_MAX_TIMERS       (16u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct os_timer_t OS_TIMER;

struct os_timer_t {
    OS_OBJ_HEAD         OSMutexObjHeader;

    timer_t             OSTimerData;
    UINT16              OSTimerOpt;
    OS_TIMER_ROUTINE   *OSTimerRoutine;
    void               *OSTimerRoutineArg;
};


static  OS_MEM_POOL     osTimerObjPool;
static  OS_TIMER        osTimerObjTbl[OS_MAX_TIMERS];

/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void *osHeapMalloc(size_t size)
{
    void *mem;

    OSEnterCriticalSection();
    mem = heap_malloc(size);
    OSExitCriticalSection();
    
    return mem;
}

void *osHeapRealloc(void *mem, size_t size)
{
    void *newMem = NULL;

    OSEnterCriticalSection();
    newMem = heap_realloc(mem, size);
    OSExitCriticalSection();
    
    return newMem;
}

void osHeapFree(void *mem)
{
    OSEnterCriticalSection();
    heap_free(mem);
    OSExitCriticalSection();
}



ROOT void OSSysTickHook(void)
{
    timer_tick();
    timer_watchman();
}

ROOT void OSInitHookEnd(void)
{
    memset((char *)osTimerObjTbl, 0, sizeof(osTimerObjTbl));
    pool_init(&osTimerObjPool, UBOUND(osTimerObjTbl), osTimerObjTbl, sizeof(OS_TIMER));
    
    timer_init();
}





static void os_timer_routine_wrapper(timer_t *timer)
{
    OS_TIMER *osTimer;
    
    osTimer = CONTAINER_OF(timer, OS_TIMER, OSTimerData);
    if (NULL != osTimer->OSTimerRoutine) {
        osTimer->OSTimerRoutine(osTimer->OSTimerRoutineArg);
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

    OSEnterCriticalSection();
    timer = pool_new(&osTimerObjPool);
    if (timer == NULL) {
        OSExitCriticalSection();
        return OS_ERR_OUT_OF_MEMORY;
    }
    OSExitCriticalSection();

    timer->OSMutexObjHeader.OSObjType = OS_OBJ_TYPE_SET(7);
    timer->OSTimerOpt = 0;
    if (initValue != 0u && reloadValue == 0u) {
        timer->OSTimerOpt |= opt;
    }
    timer->OSTimerRoutine       = fnRoutine;
    timer->OSTimerRoutineArg    = RoutineArg;
    timer_config(&timer->OSTimerData, initValue, reloadValue, &os_timer_routine_wrapper);
    *pTimerHandle = timer;

    return OS_ERR_NONE;
}

OS_ERR osTimerDelete(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;

    OSEnterCriticalSection();
    timer_stop(&timer->OSTimerData);
    pool_free(&osTimerObjPool, timer);
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

OS_ERR osTimerStart(OS_HANDLE hTimer, UINT32 timeMS)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    OSEnterCriticalSection();
    timer_start(&timer->OSTimerData, timeMS);
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}

OS_ERR osTimerStop(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    OSEnterCriticalSection();
    timer_stop(&timer->OSTimerData);
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}

void timer_timerout_callback(OS_TIMER *hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    if (timer->OSTimerOpt & OS_TIMER_OPT_AUTO_DELETE) {
        pool_free(&osTimerObjPool, timer);
    }
}

/* EOF */
