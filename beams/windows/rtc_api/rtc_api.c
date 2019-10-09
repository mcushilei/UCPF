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
#include "./rtc_api.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
static void clock_alarm_callback(clock_alarm_t *alarm, bool isTimeout);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
static rtc_alarm_t  rtcAlarm[32];
static pool_t       rtcAlarmPool;
static const date_time_t startTime = { .Year = 2000, .Month = 1, .Day = 1, .Hour = 0, .Minute = 0, .Second = 0 };

/*============================ IMPLEMENTATION ================================*/

void utc_to_beijing(date_time_t *pDateTime)
{
    int32_t  d;
    uint32_t s;

    s = time_to_seconds(&pDateTime->Time);
    s += 8u * SECONDS_OF_HOUR;

    d = seconds_to_time(&pDateTime->Time, s);
    date_plus_days(&pDateTime->Date, d);
}

bool rtc_api_init(void)
{
    //! init real-time-clock: load time from hardware RTC.
    date_time_t nowTime;
    struct tm *ptm;
    time_t rawtime;

    time(&rawtime);
    ptm = gmtime(&rawtime);

    nowTime.Year     = 1900 + ptm->tm_year;
    nowTime.Month    = 1 + ptm->tm_mon;
    nowTime.Day      = ptm->tm_mday;
    nowTime.Hour     = ptm->tm_hour;
    nowTime.Minute   = ptm->tm_min;
    nowTime.Second   = ptm->tm_sec;
    utc_to_beijing(&nowTime);

    if (!clock_init(&startTime, &nowTime, &clock_alarm_callback)) {
        return false;
    }

    if (!pool_init(&rtcAlarmPool, UBOUND(rtcAlarm), rtcAlarm, sizeof(rtc_alarm_t))) {
        return false;
    }

    return true;
}

date_time_t rtc_api_get_time(void)
{
    return clock_get_time();
}

void rtc_api_set_time(date_time_t value)
{
    clock_set_time(&value);
}

uint32_t rtc_api_get_ticktock(void)
{
    return clock_get_ticktock();
}

char *rtc_api_get_time_string(char stringBuf[32])
{
    uint32_t stringOffset = 0u;

    date_time_t tiemstamp = clock_get_time();
    stringOffset += snprintf(stringBuf, 32u, "%u-%02u-%02u %02u:%02u:%02u", tiemstamp.Date.Year, tiemstamp.Date.Month, tiemstamp.Date.Day, tiemstamp.Time.Hour, tiemstamp.Time.Minute, tiemstamp.Time.Second);
    stringBuf[stringOffset] = '\0';

    return stringBuf;
}

static void clock_alarm_callback(clock_alarm_t *alarm, bool isTimeout)
{
    uint32_t dayOfWeek = 0;
    uint32_t dayOfMonth = 0;
    rtc_alarm_t *rtcAlarm = CONTAINER_OF(alarm, rtc_alarm_t, ClockAlarm);
    date_time_t endTime = clock_get_time();
    int32_t days = count_days_between(&startTime.Date, &endTime.Date);
    dayOfWeek = (days + 6 - 1) % 7u;
    dayOfMonth = endTime.Day - 1;
    //! to check trigger condition.
    if ( ((1u << dayOfWeek) & rtcAlarm->DayOfWeek) || ((1u << dayOfMonth) & rtcAlarm->DayOfMonth) ) {
        //! handle the real routine of the alarm.
        if (NULL != rtcAlarm->Routine) {
            rtcAlarm->Routine(rtcAlarm, rtcAlarm->RoutineArg);
        }
    }
}

rtc_alarm_t *rtc_alarm_creat(uint8_t weakMask, uint32_t monthMask, rtc_alarm_routine_t *routine, void *routineArg)
{
    rtc_alarm_t *rtcAlarm = pool_new(&rtcAlarmPool);
    if (NULL == rtcAlarm) {
        return false;
    }

    clock_init_alarm(&rtcAlarm->ClockAlarm);
    rtcAlarm->DayOfWeek = weakMask;
    rtcAlarm->DayOfMonth = monthMask;
    rtcAlarm->Routine = routine;
    rtcAlarm->RoutineArg = routineArg;

    return rtcAlarm;
}

bool rtc_alarm_start(rtc_alarm_t *rtcAlarm, time24_t *time)
{
    return clock_add_alarm(&rtcAlarm->ClockAlarm, time);
}

bool rtc_alarm_stop(rtc_alarm_t *rtcAlarm)
{
    clock_remove_alarm(&rtcAlarm->ClockAlarm);
    return true;
}

bool rtc_alarm_delete(rtc_alarm_t *rtcAlarm)
{
    OS_CRITICAL_SECTION_BEGIN();
    clock_remove_alarm(&rtcAlarm->ClockAlarm);
    pool_free(&rtcAlarmPool, rtcAlarm);
    OS_CRITICAL_SECTION_END();
    return true;
}



/* EOF */
