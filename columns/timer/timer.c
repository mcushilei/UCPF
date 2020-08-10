/*******************************************************************************
*  Copyright(C)2016-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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

/* \brief timer implemented in Timing-Wheel method.
 */

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./timer_plug.h"
#include "../list/list.h"
#include "./timer.h"

/*============================ MACROS ========================================*/
#define TIMER_WHEEL_BIT_MASK        ((1u << TIMER_WHEEL_BIT_WIDTH) - 1u)
#define TIMER_WHEEL_COUNTER_MASK(n) ((uint32_t)TIMER_WHEEL_BIT_MASK << ((n) * TIMER_WHEEL_BIT_WIDTH))
#define TIMER_WHEEL_COUNTER_VALUE(c, n) (( (c) >> ((n) * TIMER_WHEEL_BIT_WIDTH) ) & TIMER_WHEEL_BIT_MASK)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static bool insert_timer(timer_engine_t *timerEngine, timer_t *timer)
{
    int wheel;
    uint32_t wheelCounter;
    uint32_t a;
    
    //! to find which wheel the timer will be inserted.
    a = timerEngine->Counter ^ timer->Value;
    for (wheel = TIMER_WHEEL_NUM - 1; wheel >= 0; wheel--) {
        if ((a & TIMER_WHEEL_COUNTER_MASK(wheel)) != 0u) {
            break;
        }
    }
    //! in case the timer is timeout. (where timerEngine->Counter == timer->Value)
    if (wheel < 0) {
        return false;
    }

    wheelCounter = TIMER_WHEEL_COUNTER_VALUE(timer->Value, wheel);
    list_insert(&timer->ListNode, timerEngine->TimerWheel[wheel][wheelCounter].Prev);
    return true;
}

static void remove_timer(timer_t *timer)
{
    list_remove(&timer->ListNode);
}

static void timer_timeout_processs(timer_engine_t *timerEngine, timer_t *timer)
{
    if (timerEngine->TimeoutCallback != NULL) {
        timerEngine->TimeoutCallback(timer);
    }

    //! if this is a periodic timer, re-add it to the list.
    if (timer->Period != 0u) {
        timer->Value = timerEngine->Counter + timer->Period;
        insert_timer(timerEngine, timer);
    }
}

static void timer_wheel_timeout_processs(timer_engine_t *timerEngine, uint32_t wheel, uint32_t wheelCounter)
{
    timer_t *timer;
	list_node_t *node;
    
    while (!LIST_IS_EMPTY(timerEngine->TimerWheel[wheel][wheelCounter])) {
        node = timerEngine->TimerWheel[wheel][wheelCounter].Next;
        timer = CONTAINER_OF(node, timer_t, ListNode);
        list_remove(node);
        
        if (!insert_timer(timerEngine, timer)) {
            timer_timeout_processs(timerEngine, timer);
        }
    }
}

/*
 \note  This function should be called periodically by a clock source normally in
        a interrupt of a hardware counter/timer.
 */
void timer_engine_tick(timer_engine_t *timerEngine)
{
    int wheel;
    uint32_t wheelCounter;

    if (!timerEngine->IsRunning) {
        return;
    }

    timerEngine->SafeAtomStart();

    //! first increase timerEngine->Counter
    ++timerEngine->Counter;

    //! then check if there is any timer that has been timeout.   
    for (wheel = TIMER_WHEEL_NUM - 1; wheel >= 0; wheel--) {
        wheelCounter = TIMER_WHEEL_COUNTER_VALUE(timerEngine->Counter, wheel);
        if (!LIST_IS_EMPTY(timerEngine->TimerWheel[wheel][wheelCounter])) {
            timer_wheel_timeout_processs(timerEngine, wheel, wheelCounter);
        }
    }
    
    timerEngine->SafeAtomEnd();
}

/*
 \param [I] timer       the timer to be configed.
 \param [I] initValue   if it is not 0 the counter will start with this value.
 \param [I] reloadValue if it is not 0, the counter will be reloaded when it reaches 0.
 */
bool timer_config(
    timer_engine_t *timerEngine,
    timer_t        *timer,
    uint32_t        initValue,
    uint32_t        reloadValue)
{
    timer->Period = reloadValue;
    list_init(&timer->ListNode);
    timer_start(timerEngine, timer, initValue);

    return true;
}

/*
 \param [I] timer   the timer to be started.
 \param [I] value   to specify the time when the timer count to.
 \note  it is ment to restart a running timer. A timer is usually started when
        it is initialised by calling timer_config() with a non-zero initValue.
 */
void timer_start(timer_engine_t *timerEngine, timer_t *timer, uint32_t value)
{
    if (value != 0u) {
        timerEngine->SafeAtomStart();
        remove_timer(timer);
        timer->Value = value + timerEngine->Counter;
        insert_timer(timerEngine, timer);
        timerEngine->SafeAtomEnd();
    } else {
        //! nothing else to do.
    }
}

void timer_stop(timer_engine_t *timerEngine, timer_t *timer)
{
    timerEngine->SafeAtomStart();
    remove_timer(timer);
    timerEngine->SafeAtomEnd();
}

/*
 \param [I] timer_engine_timout_fn  this callback will be called by timer_tick()
    when any timer reaches it's deadline. Do NOT do any heavy job in it.
 */
bool timer_engine_init( timer_engine_t                  *timerEngine,
                        timer_engine_timout_fn          *callback,
                        timer_engine_safe_atom_start_fn *safeAtomStart,
                        timer_engine_safe_atom_end_fn   *safeAtomEnd)
{
    if (NULL == safeAtomStart) {
        return false;
    }
    if (NULL == safeAtomEnd) {
        return false;
    }
    
    timerEngine->TimeoutCallback    = callback;
    timerEngine->SafeAtomStart      = safeAtomStart;
    timerEngine->SafeAtomEnd        = safeAtomEnd;
	for (uint32_t j = 0; j < TIMER_WHEEL_NUM; j++) {
		for (uint32_t i = 0; i < TIMER_WHEEL_BUCKET_NUM; i++) {
			list_init(&timerEngine->TimerWheel[j][i]);
		}
	}
    timerEngine->Counter    = 0;
    timerEngine->IsRunning  = true;
    return true;
}


/* EOF */
