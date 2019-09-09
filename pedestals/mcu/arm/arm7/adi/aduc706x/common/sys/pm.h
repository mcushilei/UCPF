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


#ifndef __DRIVER_ARM7_ADCU706X_COMMON_PM_C__
#ifndef __DRIVER_ARM7_ADCU706X_COMMON_PM_H__
#define __DRIVER_ARM7_ADCU706X_COMMON_PM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
    PLL_CLKSRC_IRC  = 0,
    PLL_CLKSRC_OSC,
} em_pll_clk_src_t;

typedef enum {
    MAIN_CLKSRC_PLL      = 0x0,
    MAIN_CLKSRC_EXT,
    MAIN_CLKSRC_CURRENT,
} em_main_clk_src_t;

typedef enum {
    PERIPHERAL_CLOCK_I2C    = 0,
    PERIPHERAL_CLOCK_ADC,
    PERIPHERAL_CLOCK_DAC,
    PERIPHERAL_CLOCK_UART0,
    PERIPHERAL_CLOCK_SPI,
    PERIPHERAL_CLOCK_TIMER,
    PERIPHERAL_CLOCK_INVALID_VALUE,
} em_peripheral_clk_t;

typedef enum {
    PERIPHERAL_POWER_INVALID_VALUE = 0,
    PERIPHERAL_POWER_I2C_SPI    = 1,
    PERIPHERAL_POWER_UART0,
    PERIPHERAL_POWER_PWM,
    PERIPHERAL_POWER_MAX_VALUE,
} em_peripheral_pw_t;

//! \name the lowpower mode
//! @{
typedef enum {
    SLEEP           = 0,
    DEEP_SLEEP,
    POWER_DOWN,
    DEEP_POWER_DOWN,
} em_lowpower_mode_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern bool     pll_clock_select(uint32_t wSrc);
extern uint32_t pll_get_in_clock(void);
extern uint32_t pll_get_out_clock(void);
extern uint32_t main_clock_get(void);
extern bool     core_clock_config(uint32_t wSrc, uint32_t wDiv);
extern uint32_t core_clock_get(void);
extern bool     enter_lowpower_mode(uint32_t wMode);
extern bool     peripheral_clock_config(uint8_t chIndex, uint8_t chDiv);
extern uint32_t peripheral_clock_get_div(uint8_t chIndex);
extern uint32_t peripheral_clock_get(uint8_t chIndex);
extern bool     peripheral_power_enable(uint32_t wIndex);
extern bool     peripheral_power_disable(uint32_t wIndex);
extern uint32_t peripheral_power_status_get(uint32_t wIndex);
extern bool     peripheral_power_status_resume(uint32_t wIndex, uint32_t tStatus);

#endif
#endif
