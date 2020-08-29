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

#ifndef __WINDOWS_RTC_API_H__
#define __WINDOWS_RTC_API_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "clock/clock.h"
#include "calendar/calendar.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct rtc_alarm_t  rtc_alarm_t;
typedef void rtc_alarm_routine_t(rtc_alarm_t *rtcAlarm, void *arg);

struct rtc_alarm_t {
    clock_alarm_t           ClockAlarm;
    rtc_alarm_routine_t    *Routine;
    void                   *RoutineArg;
    uint32_t                DayOfMonth; //! bit0: 1st, bit1: 2nd, ... bit30: 31st.
    uint8_t                 DayOfWeek;  //! bit0: Mon, Bit1: Tur, ... bit6: Sun.
};


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool         rtc_api_init(void);
extern date_time_t  rtc_api_get_time(void);
extern void         rtc_api_set_time(date_time_t value);
extern uint32_t     rtc_api_get_ticktock(void);
extern char        *rtc_api_get_time_string(char stringBuf[32]);
extern rtc_alarm_t *rtc_alarm_creat(uint8_t weakMask, uint32_t monthMask, rtc_alarm_routine_t *routine, void *routineArg);
extern bool         rtc_alarm_start(rtc_alarm_t *rtcAlarm, time24_t *time);
extern bool         rtc_alarm_stop(rtc_alarm_t *rtcAlarm);
extern bool         rtc_alarm_delete(rtc_alarm_t *rtcAlarm);

#endif  //!< #ifndef __WINDOWS_RTC_API_H__
/* EOF */
