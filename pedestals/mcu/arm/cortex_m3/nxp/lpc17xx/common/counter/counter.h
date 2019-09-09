/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_CONTER_H__
#define __DRIVER_CONTER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_counter.h"

/*============================ MACROS ========================================*/
#define DRIVER_CNT_CLOCKSOURCE_TIMER            (0u << 0)
#define DRIVER_CNT_CLOCKSOURCE_CAR_ON_RISING    (1u << 0)
#define DRIVER_CNT_CLOCKSOURCE_CAP_ON_FALING    (2u << 0)
#define DRIVER_CNT_CLOCKSOURCE_CAP_ON_BOTH      (3u << 0)

#define DRIVER_CNT_CLOCKSOURCE_CAP0             (0u << 2)
#define DRIVER_CNT_CLOCKSOURCE_CAP1             (1u << 2)

#define DRIVER_CNT_INTERRUPT_ON_MATCH   (1u << 0)
#define DRIVER_CNT_RESET_ON_MATCH       (1u << 1)
#define DRIVER_CNT_STOP_ON_MATCH        (1u << 2)

#define DRIVER_CNT_RISING_TRIGGER_CAP   (1u << 0)
#define DRIVER_CNT_FALLING_TRIGGER_CAP  (1u << 1)
#define DRIVER_CNT_INT_WHEN_TRIGGER_CAP (1u << 2)

#define DRIVER_CNT_ON_MATCH_DO_NOTHING  (0u)
#define DRIVER_CNT_ON_MATCH_CLEAR       (1u)
#define DRIVER_CNT_ON_MATCH_SET         (2u)
#define DRIVER_CNT_ON_MATCH_TOGGLE      (3u)


#define DRIVER_CNT_INI_FLAG_IS_SET(__REG, __INT)  ((__REG).IR & (__INT))
#define DRIVER_CNT_CLR_INT(__REG, __INT)  ((__REG).IR = (__INT))
#define DRIVER_CNT_INT_MR0      (1u << 0)
#define DRIVER_CNT_INT_MR1      (1u << 1)
#define DRIVER_CNT_INT_MR2      (1u << 2)
#define DRIVER_CNT_INT_MR3      (1u << 3)
#define DRIVER_CNT_INT_CR0      (1u << 4)
#define DRIVER_CNT_INT_CR1      (1u << 5)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    uint32_t ClockSource;
    uint32_t ClockPrescal;
    uint32_t Cap0Trigger;
    uint32_t Cap1Trigger;
    uint32_t Match0Value;
    uint32_t Match1Value;
    uint32_t Match2Value;
    uint32_t Match3Value;
    uint32_t Match0Action;
    uint32_t Match1Action;
    uint32_t Match2Action;
    uint32_t Match3Action;
    uint32_t Match0PinAction;
    uint32_t Match1PinAction;
    uint32_t Match2PinAction;
    uint32_t Match3PinAction;
} driver_counter_cfg_t;
        
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool counter_init(tmr_reg_t *instance, driver_counter_cfg_t *cfg);
extern bool counter_deinit(tmr_reg_t *instance);
extern bool counter_start(tmr_reg_t *instance);
extern bool counter_stop(tmr_reg_t *instance);

extern bool counter_clear_int(tmr_reg_t *instance, uint32_t interrupt);


#endif  //!< #endif __DRIVER_CONTER_H__
/* EOF */