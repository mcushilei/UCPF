/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __TIMER_H__
#define __TIMER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_timer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define TIMER_FUNCTION_PROTOTYPE(__N, __A)                          \
    extern bool timer##__N##_init(void);                            \
    extern bool timer##__N##_check_idle(void);                      \
    extern bool timer##__N##_enable(void);                          \
    extern bool timer##__N##_disable(void);                         \
    extern bool timer##__N##_start(void);                           \
    extern bool timer##__N##_stop(void);                            \
    extern bool timer##__N##_interrupt_enalbe(void);                \
    extern bool timer##__N##_interrupt_disable(void);               \
    extern bool timer##__N##_overflow_flag_get(void);               \
    extern bool timer##__N##_overflow_flag_clear(void);             \
    extern bool     timer##__N##_period_set(uint32_t wPeriod);          \
    extern uint32_t timer##__N##_period_get(void);                      \
    extern bool     timer##__N##_prescale_set(uint16_t hwPrescale);     \
    extern uint16_t timer##__N##_prescale_get(void);                    \

/*============================ TYPES =========================================*/
DEF_INTERFACE(i_interrupt_t)
    bool    (*Enable)(void);
    bool    (*Disable)(void);
END_DEF_INTERFACE(i_interrupt_t)

DEF_INTERFACE(i_overflow_flag_t)
    bool    (*Get)(void);
    bool    (*Clear)(void);
END_DEF_INTERFACE(i_overflow_flag_t)

DEF_INTERFACE(i_period_t)
    bool        (*Set)(uint32_t wPeriod);
    uint32_t    (*Get)(void);
END_DEF_INTERFACE(i_period_t)

DEF_INTERFACE(i_prescale_t)
    bool        (*Set)(uint16_t hwPrescale);
    uint16_t    (*Get)(void);
END_DEF_INTERFACE(i_prescale_t)

DEF_INTERFACE(i_timer_t)
    bool        (*Init)(void);
    bool        (*CheckIdle)(void);
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    bool        (*Start)(void);
    bool        (*Stop)(void);

    i_interrupt_t       Interrupt;
    i_overflow_flag_t   OverflowFlag;
    i_period_t          Period;
    i_prescale_t        Prescale;

    timer_reg_t *const RegPage;
END_DEF_INTERFACE(i_timer_t)

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_timer_t TIMER[TIMER_COUNT];

/*============================ PROTOTYPES ====================================*/
MREPEAT(TIMER_COUNT, TIMER_FUNCTION_PROTOTYPE, 0)


#endif
/* EOF */
