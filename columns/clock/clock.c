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




/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./clock_plug.h"
#include "./clock.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    list_node_t     Alarm;          //! this alarm would be triggered every day.
    list_node_t    *NextAlarmToTrigger;
    list_node_t     DateAlarm;      //! this alarm would be only triggered one time.
    date_t          OriginDate;
    uint32_t        TickTock;       //! the elapsed second from OriginDate.
    uint32_t        TimeOfDay;      //! Only used by Alarm.
    bool            IsRunning;
} real_clock_t;

/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
static real_clock_t realClock = {0};

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void clock_alarm_list_insert(clock_alarm_t *alarm)
{
    list_node_t *pNode;
    clock_alarm_t *pAlarm;
    
    for (pNode = realClock.Alarm.Next; pNode != &realClock.Alarm; pNode = pNode->Next) {
        pAlarm = CONTAINER_OF(pNode, clock_alarm_t, ListNode);
        if (pAlarm->Time >= alarm->Time) {
            break;
        }
    }
    list_insert(&alarm->ListNode, pNode->Prev);

    if (pNode == realClock.NextAlarmToTrigger) {
        //! adjust NextAlarmToTrigger.
        if (realClock.TimeOfDay < alarm->Time) {
            realClock.NextAlarmToTrigger = &alarm->ListNode;
        }
    }
}

static void clock_alarm_list_remove(clock_alarm_t *alarm)
{
    if (&alarm->ListNode == realClock.NextAlarmToTrigger) {
        //! adjust NextAlarmToTrigger.
        realClock.NextAlarmToTrigger = realClock.NextAlarmToTrigger->Next;
    }

    list_remove(&alarm->ListNode);
}

static void clock_date_alarm_list_insert(clock_alarm_t *alarm)
{
    list_node_t *pNode;
    clock_alarm_t *pAlarm;
    
    for (pNode = realClock.DateAlarm.Next; pNode != &realClock.DateAlarm; pNode = pNode->Next) {
        pAlarm = CONTAINER_OF(pNode, clock_alarm_t, ListNode);
        if (pAlarm->Time >= alarm->Time) {
            break;
        }
    }
    list_insert(&alarm->ListNode, pNode->Prev);
}

static void clock_date_alarm_list_remove(clock_alarm_t *alarm)
{
    list_remove(&alarm->ListNode);
}

static void clock_alarm_process(clock_alarm_t *alarm, bool isTimeout)
{
    if (alarm->Routine != NULL) {
        alarm->Routine(alarm, isTimeout);
    }
}

static void check_date_alarm(bool isTimeout)
{
    clock_alarm_t *pAlarm;

    while (!LIST_IS_EMPTY(realClock.DateAlarm)) {
        pAlarm = CONTAINER_OF(realClock.DateAlarm.Next, clock_alarm_t, ListNode);
        if (realClock.TickTock < pAlarm->Time) {
            break;
        }

        clock_date_alarm_list_remove(pAlarm);
        clock_alarm_process(pAlarm, isTimeout);
    }
}

//! This function should be called periodly by RTC's ISR.
void clock_tick_tock(void)
{
    clock_alarm_t *pAlarm;

    if (!realClock.IsRunning) {
        return;
    }

    CLOCK_CRITICAL_SECTION_BEGIN();

    realClock.TickTock++;
    //! process date-alarm here.
    check_date_alarm(true);

    //! process alarm.
    realClock.TimeOfDay++;
    if (realClock.TimeOfDay >= SECONDS_OF_DAY) {
        realClock.TimeOfDay = 0u;
        realClock.NextAlarmToTrigger = realClock.Alarm.Next;    //! reset NextAlarmToTrigger.
    }
    //! move NextAlarmToTrigger forward till we reach a un-timeout alarm or the head of the list.
    while (realClock.NextAlarmToTrigger != &realClock.Alarm) {
        pAlarm = CONTAINER_OF(realClock.NextAlarmToTrigger, clock_alarm_t, ListNode);
        if (pAlarm->Time <= realClock.TimeOfDay) {
            realClock.NextAlarmToTrigger = realClock.NextAlarmToTrigger->Next;
            clock_alarm_process(pAlarm, true);
        } else {
            break;
        }
    }

    CLOCK_CRITICAL_SECTION_END();
}

bool clock_init(const date_time_t *originDate, const date_time_t *currentTime)
{
    uint32_t sec = 0u;
    int32_t  days = 0;

    //! validate parameters.
    sec = time_to_seconds(&currentTime->Time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    if (!validate_date(&originDate->Date)) {
        return false;
    }

    if (!validate_date(&currentTime->Date)) {
        return false;
    }

    days = count_days_between(&originDate->Date, &currentTime->Date);
    if (days < 0) {
        return false;
    }

    list_init(&realClock.Alarm);
    list_init(&realClock.DateAlarm);
    realClock.NextAlarmToTrigger = &realClock.Alarm;
    realClock.OriginDate    = originDate->Date;
    realClock.TickTock      = days * SECONDS_OF_DAY + sec;
    realClock.TimeOfDay     = sec;
    realClock.IsRunning     = true;

    return true;
}

bool clock_set_ticktock(uint32_t time)
{
    bool forward;
    uint32_t sec;
    clock_alarm_t *pAlarm;

    CLOCK_CRITICAL_SECTION_BEGIN();
    do {
        if (time > realClock.TickTock) {
            forward = true;
        } else {
            forward = false;
        }
        realClock.TickTock  = time;

        if (forward) {
            //! checkout invalid date-alarm.
            check_date_alarm(false);
        } else {
            //! nothing to do.
        }

        sec = time % SECONDS_OF_DAY;
        if (sec > realClock.TimeOfDay) {
            forward = true;
        } else {
            forward = false;
        }
        realClock.TimeOfDay = sec;

        //! adjust NextAlarmToTrigger.
        for (realClock.NextAlarmToTrigger = realClock.Alarm.Next;
             realClock.NextAlarmToTrigger != &realClock.Alarm;
             realClock.NextAlarmToTrigger = realClock.NextAlarmToTrigger->Next) {
            pAlarm = CONTAINER_OF(realClock.NextAlarmToTrigger, clock_alarm_t, ListNode);
            if (pAlarm->Time <= realClock.TimeOfDay) {
            } else {
                break;
            }
        }

    } while (0);
    CLOCK_CRITICAL_SECTION_END();

    return true;
}

bool clock_set_time(const date_time_t *newTime)
{
    uint32_t sec = 0u;
    int32_t  days = 0;

    //! validate parameters.
    sec = time_to_seconds(&newTime->Time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    if (!validate_date(&newTime->Date)) {
        return false;
    }

    days = count_days_between(&realClock.OriginDate, &newTime->Date);
    if (days < 0) {
        return false;
    }
    sec += days * SECONDS_OF_DAY;

    return clock_set_ticktock(sec);
}

date_time_t clock_get_time(void)
{
    date_time_t dateTime;
    uint32_t sec = 0u;
    int32_t  days = 0;

    CLOCK_CRITICAL_SECTION_BEGIN();
    dateTime.Date = realClock.OriginDate;
    days = realClock.TickTock / SECONDS_OF_DAY;
    sec = realClock.TickTock - days * SECONDS_OF_DAY;
    CLOCK_CRITICAL_SECTION_END();
    date_plus_days(&dateTime.Date, days);
    seconds_to_time(&dateTime.Time, sec);
    return dateTime;
}

uint32_t clock_get_ticktock(void)
{
    uint32_t value;

    CLOCK_CRITICAL_SECTION_BEGIN();
    value = realClock.TickTock;
    CLOCK_CRITICAL_SECTION_END();
    return value;
}

bool clock_add_alarm(clock_alarm_t *alarm, time24_t *time, clock_alarm_routine_t *routine)
{
    uint32_t sec = 0u;

    //! validate parameters.
    sec = time_to_seconds(time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    //! init it.
    list_init(&alarm->ListNode);
    alarm->Routine  = routine;
    alarm->Time     = sec;

    //! insert it to running list.
    CLOCK_CRITICAL_SECTION_BEGIN();
    clock_alarm_list_insert(alarm);
    CLOCK_CRITICAL_SECTION_END();

    return true;
}

void clock_remove_alarm(clock_alarm_t *alarm)
{
    //! remove it from running list.
    CLOCK_CRITICAL_SECTION_BEGIN();
    clock_alarm_list_remove(alarm);
    CLOCK_CRITICAL_SECTION_END();
}

bool clock_add_timer(clock_alarm_t *alarm, const date_time_t *time, clock_alarm_routine_t *routine)
{
    uint32_t sec = 0u;
    int32_t  days = 0;

    //! validate parameters.
    sec = time_to_seconds(&time->Time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    if (!validate_date(&time->Date)) {
        return false;
    }

    days = count_days_between(&realClock.OriginDate, &time->Date);
    if (days < 0) {
        return false;
    }
    sec += days * SECONDS_OF_DAY;

    //! init it.
    list_init(&alarm->ListNode);
    alarm->Routine  = routine;
    alarm->Time     = sec;

    //! insert it to running list.
    CLOCK_CRITICAL_SECTION_BEGIN();
    if (sec <= realClock.TickTock) {
        CLOCK_CRITICAL_SECTION_END();
        return false;
    }
    clock_date_alarm_list_insert(alarm);
    CLOCK_CRITICAL_SECTION_END();

    return true;
}

void clock_remove_timer(clock_alarm_t *alarm)
{
    //! remove it from running list.
    CLOCK_CRITICAL_SECTION_BEGIN();
    clock_date_alarm_list_remove(alarm);
    CLOCK_CRITICAL_SECTION_END();
}

/* EOF */
