/*******************************************************************************
 *  Copyright(C)2018-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./clock.h"
#include "./clock_plug.h"
#include "../scheduler/os.h"


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void clock_alarm_callback_t(clock_alarm_t *alarm, bool isTimeout);
typedef void clock_engine_atom_lock_set_t(void);
typedef void clock_engine_atom_lock_reset_t(void);

typedef struct {
    list_node_t             Alarm;              //! this alarm would be triggered every day.
    list_node_t            *NextAlarmToTrigger; //! scanhand point to the next alarm to check.
    list_node_t             DateAlarm;          //! this alarm would be only triggered one time.
    clock_alarm_callback_t *AlarmCallback;
    clock_engine_atom_lock_set_t    *SafeAtomStart;
    clock_engine_atom_lock_reset_t  *SafeAtomEnd;
    date_t          OriginDate;
    uint32_t        TickTock;       //! the elapsed second from OriginDate.
    uint32_t        TimeOfDay;      //! Only used by Alarm.
    bool            IsRunning;
} real_clock_t;

/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
static real_clock_t realClock = {0};
static const clock_alarm_t NULL_ALARM = { 0 };

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
    if (realClock.AlarmCallback != NULL) {
        realClock.AlarmCallback(alarm, isTimeout);
    }
}

static void check_date_alarm(bool isTimeout)
{
    clock_alarm_t *pAlarm;

    while (!LIST_IS_EMPTY(&realClock.DateAlarm)) {
        pAlarm = CONTAINER_OF(realClock.DateAlarm.Next, clock_alarm_t, ListNode);
        if (realClock.TickTock < pAlarm->Time) {
            break;
        }

        clock_date_alarm_list_remove(pAlarm);
        clock_alarm_process(pAlarm, isTimeout);
    }
}

static void check_daily_alarm(bool isTimeout)
{
    clock_alarm_t *pAlarm;

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
}

void clock_tick_tock(void)
{
    if (!realClock.IsRunning) {
        return;
    }

    realClock.SafeAtomStart();

    realClock.TickTock++;
    //! process date-alarm here.
    check_date_alarm(true);

    realClock.TimeOfDay++;
    if (realClock.TimeOfDay >= SECONDS_OF_DAY) {
        realClock.TimeOfDay = 0u;
        realClock.NextAlarmToTrigger = realClock.Alarm.Next;    //! reset NextAlarmToTrigger.
    }
    //! process daily alarm.
    check_daily_alarm(true);

    realClock.SafeAtomEnd();
}

bool clock_init(
                const date_time_t *dataStartFrom,
                const date_time_t *timeNow,
                void (*callback)(clock_alarm_t *alarm, bool isTimeout),
                void (*lockSet)(void),
                void (*lockReset)(void) )
{
    uint32_t sec = 0u;
    int32_t  days = 0;

    //! validate parameters.
    sec = time_to_seconds(&timeNow->Time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    if (!validate_date(&dataStartFrom->Date)) {
        return false;
    }

    if (!validate_date(&timeNow->Date)) {
        return false;
    }

    days = count_days_between(&dataStartFrom->Date, &timeNow->Date);
    if (days < 0) {
        return false;
    }

    realClock.SafeAtomStart = lockSet;
    realClock.SafeAtomEnd   = lockReset;
    list_init(&realClock.Alarm);
    list_init(&realClock.DateAlarm);
    realClock.NextAlarmToTrigger = &realClock.Alarm;
    realClock.AlarmCallback      = callback;
    realClock.OriginDate    = dataStartFrom->Date;
    realClock.TickTock      = days * SECONDS_OF_DAY + sec;
    realClock.TimeOfDay     = sec;
    realClock.IsRunning     = true;

    return true;
}

static bool clock_set_ticktock(uint32_t time)
{
    bool forward;
    uint32_t sec;
    clock_alarm_t *pAlarm;

    realClock.SafeAtomStart();
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
    realClock.SafeAtomEnd();

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

    realClock.SafeAtomStart();
    dateTime.Date = realClock.OriginDate;
    days = realClock.TickTock / SECONDS_OF_DAY;
    sec  = realClock.TickTock - days * SECONDS_OF_DAY;
    realClock.SafeAtomEnd();
    date_plus_days(&dateTime.Date, days);
    seconds_to_time(&dateTime.Time, sec);
    return dateTime;
}

uint32_t clock_get_ticktock(void)
{
    uint32_t value;

    realClock.SafeAtomStart();
    value = realClock.TickTock;
    realClock.SafeAtomEnd();
    return value;
}

bool clock_init_alarm(clock_alarm_t *alarm)
{
    //! init it.
    *alarm = NULL_ALARM;
    list_init(&alarm->ListNode);

    return true;
}

bool clock_add_alarm(clock_alarm_t *alarm, const time24_t *time)
{
    uint32_t sec = 0u;

    //! validate parameters.
    sec = time_to_seconds(time);
    if (sec >= SECONDS_OF_DAY) {
        return false;
    }

    //! init it.
    list_init(&alarm->ListNode);
    alarm->Time     = sec;

    //! insert it to running list.
    realClock.SafeAtomStart();
    clock_alarm_list_insert(alarm);
    realClock.SafeAtomEnd();

    return true;
}

void clock_remove_alarm(clock_alarm_t *alarm)
{
    //! remove it from running list.
    realClock.SafeAtomStart();
    clock_alarm_list_remove(alarm);
    realClock.SafeAtomEnd();
}

bool clock_add_timer(clock_alarm_t *alarm, const date_time_t *time)
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
    *alarm = NULL_ALARM;
    list_init(&alarm->ListNode);
    alarm->Time = sec;

    realClock.SafeAtomStart();
    //! the alarm time should be in the future.
    if (sec <= realClock.TickTock) {
        realClock.SafeAtomEnd();
        return false;
    }
    //! insert it to the running list.
    clock_date_alarm_list_insert(alarm);
    realClock.SafeAtomEnd();

    return true;
}

void clock_remove_timer(clock_alarm_t *alarm)
{
    //! remove it from running list.
    realClock.SafeAtomStart();
    clock_date_alarm_list_remove(alarm);
    realClock.SafeAtomEnd();
}

/* EOF */
