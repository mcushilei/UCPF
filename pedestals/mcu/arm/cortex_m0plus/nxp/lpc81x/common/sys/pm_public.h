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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_PM_PUBLIC_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_PM_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_syscon.h"
#include ".\reg_pmu.h"

/*============================ MACROS ========================================*/
#define DIV_(_N,_D)     DIV_##_N = (_N),

#define IRC_OSC_FREQ    (12MHz)
#define SYS_OSC_FREQ    (12MHz)
#define CLKIN_FREQ      (12MHz)

#define SYSOSC_BYPASS_EN    DISABLED

/*============================ MACROFIED FUNCTIONS ===========================*/
#define PM_MAIN_CLK_GET()                   main_clk_get()
#define PM_CORE_CLK_GET()                   core_clock_get()
#define PM_CLK_GET_STATUS(__INDEX)          clock_get_status((__INDEX))
#define PM_CLK_ENABLE(__INDEX)              clock_enable((__INDEX))
#define PM_CLK_DISABLE(__INDEX)             clock_disable((__INDEX))
#define PM_CLK_RESUME(__INDEX, __STATUS)    clock_resume_status((__INDEX), (__STATUS))

/*============================ TYPES =========================================*/
//! \name the lowpower mode
//! @{
enum {
    SLEEP           = 1,        //! 0 is reversed for invalid value.
    DEEP_SLEEP      = 2,
    POWER_DOWN      = 3,
    DEEP_POWER_DOWN = 4,
};
//! @}

enum {
    SYSPLL_CKLSRC_IRC       = 0,
    SYSPLL_CLKSRC_SYSOSC    = 1,
    SYSPLL_CLKSRC_CLKIN     = 3,
};

enum {
    MAIN_CLKSRC_IRC         = 0,
    MAIN_CLKSRC_PLLIN       = 1,
    MAIN_CLKSRC_WDTOSC      = 2,
    MAIN_CLKSRC_PLLOUT      = 3,
};

enum {
    CLKOUT_CLKSRC_IRC       = 0,
    CLKOUT_CLKSRC_SYSOSC    = 1,
    CLKOUT_CLKSRC_WDOSC     = 2,
    CLKOUT_CLKSRC_MCLK      = 3,
};

enum {
    POWER_IRCOUT    = BIT(0),
    POWER_IRC       = BIT(1),
    POWER_FLASH     = BIT(2),
    POWER_BOD       = BIT(3),
    POWER_SYSOSC    = BIT(5),
    POWER_WDTOSC    = BIT(6),
    POWER_SYSPLL    = BIT(7),
    POWER_ACMP      = BIT(15),
};

//! \name Peripheral AHB Clock Macros
//! @{
enum {
    CLOCK_SYS              = 0,
    CLOCK_ROM              = 1,
    CLOCK_RAM              = 2,
    CLOCK_FLASHREG         = 3,
    CLOCK_FLASH            = 4,
    CLOCK_I2C              = 5,
    CLOCK_GPIO             = 6,
    CLOCK_SWM              = 7,
    CLOCK_SCT              = 8,
    CLOCK_WKT              = 9,
    CLOCK_MRT              = 10,
    CLOCK_SPI0             = 11,
    CLOCK_SPI1             = 12,
    CLOCK_CRC              = 13,
    CLOCK_UART0            = 14,
    CLOCK_UART1            = 15,
    CLOCK_UART2            = 16,
    CLOCK_WWDT             = 17,
    CLOCK_IOCON            = 18,
    CLOCK_ACMP             = 19,
};


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool     power_enable(uint32_t parts);
extern bool     power_disable(uint32_t parts);
extern uint32_t power_status_get(uint32_t parts);
extern bool     power_status_resume(uint32_t parts , uint32_t status);

//! Before call: PLL should NOT be in use AND should be powered down.
//! After  call: If there is any error(return false), PLL has been powered down;
//!              In other cases, PLL is working well.
extern bool     pll_init(uint32_t Source, uint32_t Freq);
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern uint32_t pll_get_in_clock(void);
extern uint32_t pll_get_out_clock(void);

extern bool     clock_enable(uint32_t parts);
extern bool     clock_disable(uint32_t parts);
extern uint32_t clock_get_status(uint32_t parts);
extern bool     clock_resume_status(uint32_t parts, uint32_t status);

extern bool     core_clock_config(uint32_t Source, uint32_t Div);
extern uint32_t core_clock_get(void);

extern uint32_t main_clock_get(void);

extern bool     uart_clock_config(uint32_t div, uint32_t frac);
extern uint32_t uart_clock_get(void);

extern bool     enter_lowpower_mode(uint32_t mode);

extern bool     wdtosc_enable(void);
extern uint32_t wdtosc_clock_get(void);

#endif  //!< #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_PM_PUBLIC_H__
/* EOF */
