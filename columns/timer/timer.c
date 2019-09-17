/*******************************************************************************
*  Copyright(C)2016-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

//! \brief normal precision timer that count in millisecond. abstract: a clock 
//!        with only one hand which has a one-day cycle.

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./timer.h"
#include "./timer_plug.h"
#include "../scheduler/os.h"


/*============================ MACROS ========================================*/
#define TIMER_TIMEOUT_FLAG_MSK          (0x01u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    list_node_t         TimerListToday;
    list_node_t         TimerListNextDay;
    timer_callback_t   *TimeoutCallback;
    uint32_t            ScanHand;
    uint32_t            ScanHandOld;
    bool                IsRunning;
} timer_watch_t;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
static timer_watch_t    timerWatch;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void timer_list_insert(timer_t *timer)
{
    list_node_t *pList;

    //! timer->Count forward timerWatch.ScanHand?
    if (timer->Count > timerWatch.ScanHand) {  //! yes.
        pList = &timerWatch.TimerListToday;
    } else {                        //! no.
        pList = &timerWatch.TimerListNextDay;
    }

    if (LIST_IS_EMPTY(*pList)) {
        list_insert(&timer->ListNode, pList);
    } else {
        timer_t *pTimer;
        list_node_t *pNode;

        for (pNode = pList->Next; pNode != pList; pNode = pNode->Next) {
            pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
            if (timer->Count < pTimer->Count) {
                break;
            }
        }
        list_insert(&timer->ListNode, pNode->Prev);
    }
}

static void timer_list_remove(timer_t *timer)
{
    list_remove(&timer->ListNode);
}

static void timer_timeout_processs(timer_t *timer)
{
    //! if this is a periodic timer, re-add it to the list.
    if (timer->Period != 0u) {
        timer->Count = timer->Period + timerWatch.ScanHand;
        timer_list_insert(timer);
    }

    if (timerWatch.TimeoutCallback) {
        timerWatch.TimeoutCallback(timer);
    }
}

//! This function should be called periodly.
void timer_tick(void)
{
    list_node_t *pNode;
    timer_t *pTimer;

    if (!timerWatch.IsRunning) {
        return;
    }

    //! increase timerWatch.ScanHand
    ++timerWatch.ScanHand;

    TIMER_CRITICAL_SECTION_BEGIN();
    if (timerWatch.ScanHandOld > timerWatch.ScanHand) {   //! Has the hand made a revolution?...
                                    //! ...Yes
        //! All the timers in timerWatch.TimerListToday has timeout. so empty it.
        while (!LIST_IS_EMPTY(timerWatch.TimerListToday)) {
            pNode = timerWatch.TimerListToday.Next;
            pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
            list_remove(pNode);
            timer_timeout_processs(pTimer);
        }

        //! then move the timerWatch.TimerListNextDay list to timerWatch.TimerListToday list. Note: this is a Circular Doubly Linked List.
        if (!LIST_IS_EMPTY(timerWatch.TimerListNextDay)) { //! there is no need to swap the lists if they are both empty.
            list_node_t *pHead = timerWatch.TimerListNextDay.Next;
            list_node_t *pTail = timerWatch.TimerListNextDay.Prev;
            timerWatch.TimerListNextDay.Next = &timerWatch.TimerListNextDay;
            timerWatch.TimerListNextDay.Prev = &timerWatch.TimerListNextDay;
            timerWatch.TimerListToday.Next = pHead;
            timerWatch.TimerListToday.Prev = pTail;
            pHead->Prev = &timerWatch.TimerListToday;
            pTail->Next = &timerWatch.TimerListToday;
        }
    }

    while (!LIST_IS_EMPTY(timerWatch.TimerListToday)) {    //! to check if there is any timer has timeout.
        pNode = timerWatch.TimerListToday.Next;
        pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
        if (pTimer->Count > timerWatch.ScanHand) { //!< no.
            break;                      //!< The list has been sorted, so we just break here.
        } else {                        //!< yes
            list_remove(pNode);
            timer_timeout_processs(pTimer);
        }
    }
    
    timerWatch.ScanHandOld = timerWatch.ScanHand;
    TIMER_CRITICAL_SECTION_END();
}

bool timer_config(
    timer_t        *timer,
    uint32_t        initValue,
    uint32_t        reloadValue)
{
    initValue     = initValue;
    timer->Period = reloadValue;
    list_init(&timer->ListNode);
    if (initValue != 0u) {
        //! start it.
        TIMER_CRITICAL_SECTION_BEGIN();
        timer->Count = initValue + timerWatch.ScanHand;
        timer_list_insert(timer);
        TIMER_CRITICAL_SECTION_END();
    }

    return true;
}

void timer_start(timer_t *timer, uint32_t value)
{
    if (value != 0u) {
        TIMER_CRITICAL_SECTION_BEGIN();
        //! remove it from running list.
        timer_list_remove(timer);
        //! start it again.
        timer->Count = value + timerWatch.ScanHand;
        timer_list_insert(timer);
        TIMER_CRITICAL_SECTION_END();
    } else {
        //! nothing else to do.
    }
}

void timer_stop(timer_t *timer)
{
    TIMER_CRITICAL_SECTION_BEGIN();
    //! remove it from running list.
    timer_list_remove(timer);
    TIMER_CRITICAL_SECTION_END();
}

bool timer_is_running(timer_t *timer)
{
    if (LIST_IS_EMPTY(timer->ListNode)) {
        return false;
    }
    return true;
}

bool timer_init(timer_callback_t *callback)
{
    timerWatch.TimeoutCallback = callback;
    timerWatch.ScanHandOld     = timerWatch.ScanHand;
    list_init(&timerWatch.TimerListToday);
    list_init(&timerWatch.TimerListNextDay);
    timerWatch.IsRunning = true;
    return true;
}


/* EOF */
