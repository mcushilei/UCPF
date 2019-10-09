/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __COLUMNS_CLOCK_H__
#define __COLUMNS_CLOCK_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "../list/list.h"
#include "../calendar/calendar.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct clock_alarm_t    clock_alarm_t;
typedef void clock_alarm_callback_t(clock_alarm_t *alarm, bool isTimeout);

struct clock_alarm_t {
	list_node_t             ListNode;
    uint32_t                Time;       //! second of day for day-alarm; absolute value in second for date-alarm.
};

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void clock_tick_tock(void);
extern bool clock_init(const date_time_t *originDate, const date_time_t *currentTime, clock_alarm_callback_t *callback);
extern bool         clock_set_time(const date_time_t *newTime);
extern date_time_t  clock_get_time(void);
extern uint32_t     clock_get_ticktock(void);
extern bool clock_init_alarm(clock_alarm_t *alarm);
extern bool clock_add_alarm(clock_alarm_t *alarm, time24_t *time);
extern void clock_remove_alarm(clock_alarm_t *alarm);
extern bool clock_add_timer(clock_alarm_t *alarm, const date_time_t *time);
extern void clock_remove_timer(clock_alarm_t *alarm);

#endif  //! #ifndef __COLUMNS_CLOCK_H__
/* EOF */
