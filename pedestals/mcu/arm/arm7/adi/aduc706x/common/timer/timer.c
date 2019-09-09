/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! \note do not move this pre-processor statement to other places
#define __DRIVER_ARM7_ADCU706X_COMMON_TIMER_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_timer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/   
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
extern bool watchdog_init(void);
extern bool watchdog_enable(void);
extern bool watchdog_disable(void);
extern void watchdog_feed(void);

extern bool real_timer_init(void);
extern bool real_timer_get_time(uint32_t *pwTime);
extern bool real_timer_set_time(uint32_t wTime);
extern bool real_timer_enable(void);
extern bool real_timer_disable(void);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool watchdog_init(void)
{
    TIMER2_REG.LD  = 128 * DEVICE_WDT_TIME;        //!< N * 1sec
    TIMER2_REG.CON =    (0u << 8) |     //!< Count down.
                        (0u << 7) |     //!< Disnable.
                        (1u << 6) |     //!< Mode: periodic.
                        (0u << 5) |     //!< Watchdog: enable.
                        (0x2u << 2) |   //!< Clock: 32768 / 256.
                        (0u << 1) |     //!< IRQ: disable.
                        (0u << 0);      //!< Powerdown: disable.
    
    return true;
}

bool watchdog_enable(void)
{
    if (0 == TIMER2_REG.LD) {   //!< it's not been initialed
        return false;
    }
    
    TIMER2_REG.CON |= (1u << 7);
    return true;
}

bool watchdog_disable(void)
{
    TIMER2_REG.CON &= ~(1u << 7);
    return true;
}

void watchdog_feed(void)
{
    TIMER2_REG.CLRI = 0;
}


bool real_timer_init(void)
{
    TIMER0_REG.LD  = 0;             //!< 0
    TIMER0_REG.CON =    
                    (0u << 24) |    //!< Postscale: 0.
                    (0u << 23) |    //!< Postscale: Disable.
                    (0u << 19) |
                    (0u << 18) |    //!< Interrupt source: Timer0.
                    (0u << 17) |    //!< Event enable: Disable.
                    (0u << 12) |    //!< Event select: .
                    (0u << 9) |     //!< Clock: 32768.
                    (1u << 8) |     //!< Dir: up.
                    (1u << 7) |     //!< Enable: enable.
                    (0u << 6) |     //!< Mode: Free running.
                    (0u << 4) |     //!< Format: binary.
                    (0xFu << 0);    //!< Prescaler: 32768.
    
    return true;
}

bool real_timer_get_time(uint32_t *pwTime)
{
    if (NULL != pwTime) {
        *pwTime = TIMER0_REG.VAL;
    }
    
    return true;
}

bool real_timer_set_time(uint32_t wTime)
{
    TIMER0_REG.LD = wTime;
    return true;
}

bool real_timer_enable(void)
{
    return true;
}

bool real_timer_disable(void)
{
    return true;
}


