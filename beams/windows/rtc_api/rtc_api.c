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
typedef struct {
    clock_alarm_t           ClockAlarm;
    uint32_t                DayOfMonth; //! only used for day-alarm; bit0: 1st, bit1: 2nd ...
    uint8_t                 DayOfWeek;  //! only used for day-alarm; bit0: Mon, Bit1: Tur ...
} rtc_alarm_t;

/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

date_time_t rtc_api_get_time(void)
{
    //date_time_t value;
    //struct tm *ptm;
    //time_t rawtime;

    //time(&rawtime);
    //ptm = gmtime(&rawtime);

    //value.Date.Year     = 1900 + ptm->tm_year;
    //value.Date.Month    = 1 + ptm->tm_mon;
    //value.Date.Day      = ptm->tm_mday;
    //value.Time.Hour     = ptm->tm_hour;
    //value.Time.Minute   = ptm->tm_min;
    //value.Time.Second   = ptm->tm_sec;

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

void clock_alarm_routine_wrapper(rtc_alarm_t *alarm)
{
    uint32_t dayOfWeek = 0;
    uint32_t dayOfMonth = 0;

    //! to check trigger condition.
    if (((1u << dayOfWeek) & alarm->DayOfWeek) || ((1u << dayOfMonth) & alarm->DayOfMonth)) {
        //! handle the real routine of the alarm.
    }

}

char *rtc_api_get_time_string(char stringBuf[32])
{
    uint32_t stringOffset = 0u;

    date_time_t tiemstamp = clock_get_time();
    stringOffset += snprintf(stringBuf, 32u, "%u-%02u-%02u %02u:%02u:%02u", tiemstamp.Date.Year, tiemstamp.Date.Month, tiemstamp.Date.Day, tiemstamp.Time.Hour, tiemstamp.Time.Minute, tiemstamp.Time.Second);
    stringBuf[stringOffset] = '\0';

    return stringBuf;
}

/* EOF */
