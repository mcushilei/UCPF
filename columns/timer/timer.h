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


#ifndef __COLUMNS_TIMER_H__
#define __COLUMNS_TIMER_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./timer_plug.h"
#include "../list/list.h"

/*============================ MACROS ========================================*/
#define TIMER_WHEEL_NUM             (32u / TIMER_WHEEL_BIT_WIDTH)
#define TIMER_WHEEL_BUCKET_NUM      (1u << TIMER_WHEEL_BIT_WIDTH)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct timer_t  timer_t;
typedef void timer_engine_timout_fn(timer_t *timer);
typedef void timer_engine_safe_atom_start_fn(void);
typedef void timer_engine_safe_atom_end_fn(void);

typedef struct {
    list_node_t                     TimerWheel[TIMER_WHEEL_NUM][TIMER_WHEEL_BUCKET_NUM];
    timer_engine_timout_fn          *TimeoutCallback;
    timer_engine_safe_atom_start_fn *SafeAtomStart;
    timer_engine_safe_atom_end_fn   *SafeAtomEnd;
    uint32_t                        Counter;
    bool                            IsRunning;
} timer_engine_t;

struct timer_t {
	list_node_t         ListNode;
	uint32_t            Value;
	uint32_t            Period;     //!< 0: Single-Shot Timer; !0: for Reapting Timer.
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool timer_engine_init(timer_engine_t                *timerEngine,
                            timer_engine_timout_fn          *callback,
                            timer_engine_safe_atom_start_fn *safeAtomStart,
                            timer_engine_safe_atom_end_fn   *safeAtomEnd);
extern void timer_engine_tick(timer_engine_t *timerEngine);
extern bool timer_config(timer_engine_t *timerEngine, timer_t *timer, uint32_t initValue, uint32_t reloadValue);
extern void timer_start(timer_engine_t *timerEngine, timer_t *timer, uint32_t value);
extern void timer_stop(timer_engine_t *timerEngine, timer_t *timer);
extern uint32_t timer_next_expiry_left(timer_engine_t *timerEngine);

#endif  //! #ifndef __COLUMNS_TIMER_H__
/* EOF */
