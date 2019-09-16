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

/*============================ MACROS ========================================*/
#define TIMER_TIMEOUT_FLAG_MSK          (0x01u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
extern void timer_timerout_callback(timer_t *timer);

/*============================ LOCAL VARIABLES ===============================*/
static volatile uint32_t    scanHand;
static volatile uint32_t    scanHandOld;
static list_node_t          timerListToday;
static list_node_t          timerListNextDay;
static bool                 isInitOK;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool timer_init(void)
{
    isInitOK = true;
    scanHandOld = scanHand;
    list_init(&timerListToday);
    list_init(&timerListNextDay);
    return true;
}

static void timer_list_insert(timer_t *timer)
{
    list_node_t *pList;

    //! timer->Count forward scanHand?
    if (timer->Count > scanHand) {  //! yes.
        pList = &timerListToday;
    } else {                        //! no.
        pList = &timerListNextDay;
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
    if (timer->Period != 0u) {
        timer->Count = timer->Period + scanHand;
        timer_list_insert(timer);
    }
    timer->Flag |= TIMER_TIMEOUT_FLAG_MSK;
    if (timer->pRoutine != NULL) {
        timer->pRoutine(timer);
    }

    timer_timerout_callback(timer);
}

//! This function should be called periodly.
void timer_tick(void)
{
    if (!isInitOK) {
        return;
    }

    //! increase scanHand
    ++scanHand;
}

void timer_watchman(void)
{
    list_node_t *pNode;
    timer_t *pTimer;

    if (!isInitOK) {
        return;
    }

    TIMER_CRITICAL_SECTION_BEGIN();
    if (scanHandOld > scanHand) {   //! Has the hand made a revolution?...
                                    //! ...Yes
        //! All the timers in timerListToday has timeout. so empty it.
        while (!LIST_IS_EMPTY(timerListToday)) {
            pNode = timerListToday.Next;
            pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
            list_remove(pNode);
            timer_timeout_processs(pTimer);
        }

        //! then move the timerListNextDay list to timerListToday list. Note: this is a Circular Doubly Linked List.
        if (!LIST_IS_EMPTY(timerListNextDay)) { //! there is no need to swap the lists if they are both empty.
            list_node_t *pHead = timerListNextDay.Next;
            list_node_t *pTail = timerListNextDay.Prev;
            timerListNextDay.Next = &timerListNextDay;
            timerListNextDay.Prev = &timerListNextDay;
            timerListToday.Next = pHead;
            timerListToday.Prev = pTail;
            pHead->Prev = &timerListToday;
            pTail->Next = &timerListToday;
        }
    }

    while (!LIST_IS_EMPTY(timerListToday)) {    //! to check if there is any timer has timeout.
        pNode = timerListToday.Next;
        pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
        if (pTimer->Count > scanHand) { //!< no.
            break;                      //!< The list has been sorted, so we just break here.
        } else {                        //!< yes
            list_remove(pNode);
            timer_timeout_processs(pTimer);
        }
    }
    
    scanHandOld = scanHand;
    TIMER_CRITICAL_SECTION_END();
}

bool timer_config(
    timer_t        *timer,
    uint32_t        initValue,
    uint32_t        reloadValue,
	timer_routine_t *pRoutine)
{
    initValue     = initValue;
    timer->Period = reloadValue;
    timer->Flag   = 0;
    timer->pRoutine     = pRoutine;
    list_init(&timer->ListNode);
    if (initValue != 0u) {
        //! start it.
        TIMER_CRITICAL_SECTION_BEGIN();
        timer->Count = initValue + scanHand;
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
        timer->Count = value + scanHand;
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

bool timer_is_timeout(timer_t *timer)
{
    if (timer->Flag) {
        timer->Flag &= ~TIMER_TIMEOUT_FLAG_MSK;
        return true;
    }

    return false;
}

bool timer_is_running(timer_t *timer)
{
    if (LIST_IS_EMPTY(timer->ListNode)) {
        return false;
    }
    return true;
}


/* EOF */
