/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_WDT_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_WDT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name watchdog initialization arguments defination
//! @{
enum {
    WDT_CASE_INT        = 0x00,         //! A watchdog time-out will not cause reset
    WDT_CASE_RESET      = (1u << 1),    //! A watchdog time-out will cause reset
};
//! @}


//! \name wdt config struct
//! @{
typedef struct {
    uint32_t    TimeValue;          //!< Watchdog time-out value.
    uint32_t    WarnIntCmpValue;    //!< Watchdog warning interrupt compare value.
    uint32_t    WindowCmpValue;     //!< Watchdog window value.
    uint32_t    Mode;               //!< Watchdog work mode.
} wdt_cfg_t;
//! @}

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool     watchdog_disable(void);
extern bool     watchdog_enable(void);
extern bool     watchdog_config(const wdt_cfg_t *tCfg);
extern bool     watchdog_start(void);
extern void     watchdog_feed(void);
extern uint32_t watchdog_get_count_value(void);

#endif  //!< #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_WDT_H__
