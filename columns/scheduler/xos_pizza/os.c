/*******************************************************************************
 *  Copyright(C)2017-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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
#define OS_MAX_TIMERS       (32u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct os_timer_t OS_TIMER;

struct os_timer_t {
    OS_OBJ_HEAD         OSTimerObjHeader;

    timer_t             OSTimerData;
    UINT16              OSTimerOpt;
    OS_TIMER_ROUTINE   *OSTimerRoutine;
    void               *OSTimerRoutineArg;
};


/*============================ PRIVATE PROTOTYPES ============================*/
static void os_timer_engine_timeout_callback(timer_t *timer);

static  OS_MEM_POOL     osTimerObjPool;
static  OS_TIMER        osTimerObjTbl[OS_MAX_TIMERS];

static timer_engine_t myOSTimerEngine;

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





static void os_timer_engine_timeout_callback(timer_t *timer)
{
    OS_TIMER *osTimer;
    
    osTimer = CONTAINER_OF(timer, OS_TIMER, OSTimerData);
    
    if (NULL != osTimer->OSTimerRoutine) {
        osTimer->OSTimerRoutine(osTimer->OSTimerRoutineArg);
    }
    
    if (osTimer->OSTimerOpt & OS_TIMER_OPT_AUTO_DELETE) {
        OSEnterCriticalSection();
        pool_free(&osTimerObjPool, osTimer);
        OSExitCriticalSection();
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

    timer->OSTimerObjHeader.OSObjType = OS_OBJ_TYPE_SET(7);
    timer->OSTimerOpt = 0;
    if (initValue != 0u && reloadValue == 0u) {
        // nothing to do. 
        // OS_TIMER_OPT_AUTO_DELETE can be used only when this condition is matched.
    } else {
        // disable OS_TIMER_OPT_AUTO_DELETE option.
        opt &= ~(OS_TIMER_OPT_AUTO_DELETE);
    }
    timer->OSTimerOpt           = opt;
    timer->OSTimerRoutine       = fnRoutine;
    timer->OSTimerRoutineArg    = RoutineArg;
    timer_config(&myOSTimerEngine, &timer->OSTimerData, initValue, reloadValue);  // Note: the timer will start if initValue is not 0.
    *pTimerHandle = timer;
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

OS_ERR osTimerDelete(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;

    OSEnterCriticalSection();
    timer_stop(&myOSTimerEngine, &timer->OSTimerData);
    pool_free(&osTimerObjPool, timer);
    OSExitCriticalSection();

    return OS_ERR_NONE;
}

// if the timer is not running, the timer will start after calling this;
// if the timer is running, the timer will restart with the time value timeMS.
OS_ERR osTimerStart(OS_HANDLE hTimer, UINT32 timeMS)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    OSEnterCriticalSection();
    timer_start(&myOSTimerEngine, &timer->OSTimerData, timeMS);
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}

// stop a timer no matter whether it is running.
OS_ERR osTimerStop(OS_HANDLE hTimer)
{
    OS_TIMER *timer = (OS_TIMER *)hTimer;
    
    OSEnterCriticalSection();
    timer_stop(&myOSTimerEngine, &timer->OSTimerData);
    OSExitCriticalSection();
    
    return OS_ERR_NONE;
}






static void timer_engine_safe_atom_start(void)
{
	OSEnterCriticalSection();
}

static void timer_engine_safe_atom_end(void)
{
    OSExitCriticalSection();
}

ROOT void OSSysTickHook(void)
{
    timer_engine_tick(&myOSTimerEngine);
}

ROOT void OSInitHookEnd(void)
{
    memset((char *)osTimerObjTbl, 0, sizeof(osTimerObjTbl));
    pool_init(&osTimerObjPool, UBOUND(osTimerObjTbl), osTimerObjTbl, sizeof(OS_TIMER));
    
    timer_engine_init(&myOSTimerEngine, &os_timer_engine_timeout_callback, &timer_engine_safe_atom_start, &timer_engine_safe_atom_end);
}


/* EOF */
