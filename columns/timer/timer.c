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

/*============================ MACROS ========================================*/
#define TIMER_TIMEOUT_FLAG_MSK          (0x01u)

#ifndef TIMER_CRITICAL_SECTION_BEGIN
#define TIMER_CRITICAL_SECTION_BEGIN()
#endif

#ifndef TIMER_CRITICAL_SECTION_END
#define TIMER_CRITICAL_SECTION_END()
#endif

#ifndef TIMER_TICK_CYCLE
#define TIMER_TICK_CYCLE    (10u)       //! in ms
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
extern void timer_timerout_callback(timer_t *timer);

/*============================ LOCAL VARIABLES ===============================*/
static volatile uint32_t    scanHand;
static volatile uint32_t    scanHandOld;
static list_node_t          timerListToday;
static list_node_t          timerListNextDay;
static uint8_t              isStart;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool timer_init(void)
{
    isStart = 0xCC;
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

    if (LIST_IS_EMPTY(pList)) {
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
        timer->pRoutine(timer->RoutineArg);
    }

    timer_timerout_callback(timer);
}

//! This function should be called periodly.
void timer_tick(void)
{
    //! increase scanHand
    ++scanHand;
    
    if (isStart != 0xCCu) {
        return;
    }

    TIMER_CRITICAL_SECTION_BEGIN();
    //! to see if it has run over.
    if (scanHandOld > scanHand) { //! yes.            
        //! all timer in timerListToday has timeout. so, empty it.
        if (!LIST_IS_EMPTY(&timerListToday)) {
            for (list_node_t *pNode = timerListToday.Next; pNode != &timerListToday; ) {
                timer_t *pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
                pNode = pNode->Next;
                list_remove(&pTimer->ListNode);
                timer_timeout_processs(pTimer);
            }
        }

        //! move timerListNextDay to timerListToday.
        if (!LIST_IS_EMPTY(&timerListNextDay)) {
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

    if (!LIST_IS_EMPTY(&timerListToday)) {
        //! to see if there is any timer overflow in timerListToday.
        for (list_node_t *pNode = timerListToday.Next; pNode != &timerListToday; ) {
            timer_t *pTimer = CONTAINER_OF(pNode, timer_t, ListNode);
            //! to see if it has overflow.
            if (pTimer->Count > scanHand) { //!< no.
                break;            //!< The list has been sorted, so we just break.
            } else {                        //!< yes
                pNode = pNode->Next;
                list_remove(&pTimer->ListNode);
                timer_timeout_processs(pTimer);
            }
        }
    }
    TIMER_CRITICAL_SECTION_END();
    
    scanHandOld = scanHand;
}

bool timer_config(
    timer_t        *timer,
    uint32_t        initValue,
    uint32_t        reloadValue,
	timer_routine_t *pRoutine,
    void            *RoutineArg)
{
    initValue     = (initValue   + TIMER_TICK_CYCLE - 1u) / TIMER_TICK_CYCLE;
    timer->Period = (reloadValue + TIMER_TICK_CYCLE - 1u) / TIMER_TICK_CYCLE;
    timer->Flag   = 0;
    timer->pRoutine     = pRoutine;
    timer->RoutineArg   = RoutineArg;
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
    value = (value + TIMER_TICK_CYCLE - 1u) / TIMER_TICK_CYCLE;
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
    if (LIST_IS_EMPTY(&timer->ListNode)) {
        return false;
    }
    return true;
}


/* EOF */
