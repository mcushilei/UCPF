/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! \brief normal precision ac that count in millisecond. abstract: a clock 
//!        with only one hand which has a one-day cycle.

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\clock.h"
#include "..\scheduler\os.h"

/*============================ MACROS ========================================*/
#define CLOCK_TICKS_A_DAY   (24u * 60u * 60u)

#ifndef CLOCK_CRITICAL_SECTION_BEGIN
#define CLOCK_CRITICAL_SECTION_BEGIN()
#endif

#ifndef CLOCK_CRITICAL_SECTION_END
#define CLOCK_CRITICAL_SECTION_END()
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    uint32_t        Hand;
    list_node_t    *NextAlarm;
    list_node_t     AlarmList;
} real_clock_t;

/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
static real_clock_t realClock;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
static void __clock_list_insert(list_node_t *pList, alarmclock_t *ac)
{
    list_node_t  *pNode;

    for (pNode = pList; pNode != &realClock.AlarmList; pNode = pNode->Next) {
        alarmclock_t *pAC = CONTAINER_OF(pNode, alarmclock_t, ListNode);
        if (ac->Count < pAC->Count) {
            break;
        }
    }
    pNode = pNode->Prev;
    list_insert(&ac->ListNode, pNode);
}

static void clock_list_insert(alarmclock_t *ac)
{
    list_node_t *pList;

    if (realClock.NextAlarm == &realClock.AlarmList) {
        pList = realClock.AlarmList.Next;
        __clock_list_insert(pList, ac);
        //! consdering we may add node to the tail, so we need to check if we should start checking alarm.
        alarmclock_t *pAC = CONTAINER_OF(realClock.AlarmList.Prev, alarmclock_t, ListNode);
        if (realClock.Hand < pAC->Count) {
            realClock.NextAlarm = realClock.AlarmList.Prev;
        }
    } else {
        alarmclock_t *pAC = CONTAINER_OF(realClock.NextAlarm, alarmclock_t, ListNode);
        if (ac->Count >= pAC->Count) {
            pList = realClock.NextAlarm;
        } else {
            pList = realClock.AlarmList.Next;
        }
        __clock_list_insert(pList, ac);
    }
}

static void clock_list_remove(alarmclock_t *ac)
{
    alarmclock_t *pAC = CONTAINER_OF(realClock.NextAlarm, alarmclock_t, ListNode);
    if (ac == pAC) {
        realClock.NextAlarm = realClock.NextAlarm->Next;
    }
    list_remove(&ac->ListNode);
}

static void clock_alarm_process(alarmclock_t *ac)
{
    ac->Flag |= BIT(0);
    if (ac->pRoutine != NULL) {
        ac->pRoutine();
    }
}

//! This function should be called periodly.
void clock_tick_tock(void)
{
    if (realClock.NextAlarm == NULL) {  //! to avoid error when this is called from ISR...
        return;                         //! ...before clock has been initialised.
    }

    CLOCK_CRITICAL_SECTION_BEGIN();
    do {
        //! move hand forward.
        ++realClock.Hand;
        //! and then to see if it has run over.
        if (realClock.Hand >= CLOCK_TICKS_A_DAY) {
            realClock.Hand = 0u;
            realClock.NextAlarm = realClock.AlarmList.Next;
        }

        //! to see if there is any alarm triggered in list.
        while (realClock.NextAlarm != &realClock.AlarmList) {
            alarmclock_t *pAC = CONTAINER_OF(realClock.NextAlarm, alarmclock_t, ListNode);
            if (pAC->Count > realClock.Hand) {  //!< no.
                break;            //!< The list has been sorted, so we just break.
            } else {                            //!< yes
                realClock.NextAlarm = realClock.NextAlarm->Next;
                clock_alarm_process(pAC);
            }
        }
    } while (0);
    CLOCK_CRITICAL_SECTION_END();
}

bool clock_init(void)
{
    realClock.Hand = 0u;
    list_init(&realClock.AlarmList);
    realClock.NextAlarm = &realClock.AlarmList;
    return true;
}

bool clock_set_time(uint32_t time)
{
    bool forward = false;
    
    CLOCK_CRITICAL_SECTION_BEGIN();
    do {
        if (time > realClock.Hand) {
            forward = true;
        }
        realClock.Hand = time;
    
        if (forward) {
            for (; realClock.NextAlarm != &realClock.AlarmList; realClock.NextAlarm = realClock.NextAlarm->Next) {
                alarmclock_t *pAC = CONTAINER_OF(realClock.NextAlarm, alarmclock_t, ListNode);
                if (pAC->Count > time) {
                    break;
                }
            }
        } else {
            if (realClock.NextAlarm == &realClock.AlarmList) {
                realClock.NextAlarm = realClock.NextAlarm->Prev;
            }
            for (; realClock.NextAlarm != &realClock.AlarmList; realClock.NextAlarm = realClock.NextAlarm->Prev) {
                alarmclock_t *pAC = CONTAINER_OF(realClock.NextAlarm, alarmclock_t, ListNode);
                if (pAC->Count < time) {
                    realClock.NextAlarm = realClock.NextAlarm->Next;
                    break;
                }
            }
        }
    } while (0);
    CLOCK_CRITICAL_SECTION_END();
    return true;
}

bool alarmclock_config(
    alarmclock_t           *ac,
    uint32_t                value,
    alarmclock_routine_t   *pRoutine)
{
    value %= CLOCK_TICKS_A_DAY;
    ac->Flag     = 0;
    ac->pRoutine = pRoutine;
    list_init(&ac->ListNode);
    //! start it.
    CLOCK_CRITICAL_SECTION_BEGIN();
    ac->Count = value;
    clock_list_insert(ac);
    CLOCK_CRITICAL_SECTION_END();

    return true;
}

void alarmclock_start(alarmclock_t *ac, uint32_t value)
{
    value %= CLOCK_TICKS_A_DAY;
    CLOCK_CRITICAL_SECTION_BEGIN();
    //! remove it from running list.
    clock_list_remove(ac);
    ac->Count = value;
    //! start it again.
    clock_list_insert(ac);
    CLOCK_CRITICAL_SECTION_END();
}

void alarmclock_stop(alarmclock_t *ac)
{
    CLOCK_CRITICAL_SECTION_BEGIN();
    //! remove it from running list.
    clock_list_remove(ac);
    CLOCK_CRITICAL_SECTION_END();
}

bool alarmclock_is_timeout(alarmclock_t *ac)
{
    if (ac->Flag) {
        ac->Flag &= ~BIT(0);
        return true;
    }

    return false;
}

/* EOF */
