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


#ifndef __PM_H__
#define __PM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_scon.h"

/*============================ MACROS ========================================*/
#define PM_POWER_ENABLE(__P)            peripheral_clock_enable(__P)
#define PM_POWER_DISABLE(__P)           peripheral_clock_disable(__P)
#define PM_POWER_GET_STATUS(__P)        peripheral_clock_get_status(__P)
#define PM_POWER_RESUME(__P, __S)       peripheral_clock_resume_status(__P, __S)
#define PM_PCLK_CFG(__P, __DIV)         peripheral_clock_config(__P, __DIV)
#define PM_CORE_CLK_GET()               core_clock_get()

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \brief 
//! @{
typedef enum {
    PLL_CLKSRC_IRC              = 0x0,      //!< sys source clk is IRC
    PLL_CLKSRC_OSC              = 0x1,      //!< sys source clk is Oscilator
    PLL_CLKSRC_RTC              = 0x2,      //!< sys source clk is RTC
} em_pll_clk_src_t;

typedef enum {
    MAIN_CLKSRC_PLLIN           = 0x0,      //!< Main source clk is SYSCLK
    MAIN_CLKSRC_PLLOUT,                     //!< Main source clk is PLLOUT
} em_main_clk_src_t;

typedef enum {
    OUT_CLKSRC_CPU              = 0x0,      //!< sys source clk is IRC
    OUT_CLKSRC_OSC              = 0x1,      //!< sys source clk is Oscilator
    OUT_CLKSRC_IRC              = 0x2,      //!< sys source clk is IRC
    OUT_CLKSRC_USB              = 0x3,      //!< sys source clk is RTC
    OUT_CLKSRC_RTC              = 0x4,      //!< sys source clk is RTC
} em_out_clk_src_t;
//! @}

//! \name the lowpower mode
//! @{
typedef enum {
    WAIT            = 0,
    SLEEP           = 1,
    DEEP_SLEEP      = 2,
    POWER_DOWN      = 3,
} em_lowpower_mode_t;
//! @}

//! \name pll interface
//! @{
DEF_INTERFACE(i_pll_t)
    bool                        (*Enable)(void);
    bool                        (*Disable)(void);
    bool                        (*SelectSource)(em_pll_clk_src_t tSrc);
    bool                        (*Config)(uint32_t chMsel, uint32_t chNsel);
    uint32_t                    (*GetClockOut)(void); 
    uint32_t                    (*GetClockIn)(void); 
END_DEF_INTERFACE(i_pll_t)
//! @}

//! \name main clock interface
//! @{
DEF_INTERFACE(i_core_clk_t)
    bool                        (*Config)(em_main_clk_src_t tSrc, uint32_t wDiv);
    uint32_t                    (*Get)(void);
END_DEF_INTERFACE(i_core_clk_t)
//! @}

//! \name peripheral clock interface
//! @{
DEF_INTERFACE(i_pclk_t)
    bool                        (*Config)(em_pclksel_t tPer, em_pclk_div_t tDiv);
    uint32_t                    (*Get)(em_pclksel_t tPer);
END_DEF_INTERFACE(i_pclk_t)
//! @}

//! \brief power interface
//! @{
DEF_INTERFACE(i_power_t)
    bool                        (*Enable)(em_pconp_t tIndex);
    bool                        (*Disable)(em_pconp_t tIndex);
    uint32_t                    (*GetStatus)(em_pconp_t tIndex);
    bool                        (*Resume)(em_pconp_t tIndex, uint32_t tStatus);
END_DEF_INTERFACE(i_power_t)
//! @}

//! \brief clock interface
//! @{
DEF_INTERFACE(i_clk_t)
    i_pll_t                     PLL;
    i_core_clk_t                Core;
    i_pclk_t                    Peripheral;
    bool                        (*OutConfig)(em_out_clk_src_t tSrc, uint32_t wDiv);
END_DEF_INTERFACE(i_clk_t)
//! @}

//! \brief pm interface
//! @{
DEF_INTERFACE(i_pm_t)   
    i_power_t                   Power;
    i_clk_t                     Clock;
END_DEF_INTERFACE(i_pm_t)
//! @}
/*============================ GLOBAL VARIABLES ==============================*/
extern ROOT const i_pm_t PM;

/*============================ PROTOTYPES ====================================*/
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern bool     pll_clock_select(em_pll_clk_src_t tSrc);
extern bool     pll_config(uint32_t chMsel, uint32_t chNsel);
extern uint32_t pll_get_in_clock(void);
extern uint32_t pll_get_out_clock(void);
extern uint32_t main_clock_get(void);
extern bool     core_clock_cfg(em_main_clk_src_t tSrc, uint32_t wDiv);
extern uint32_t core_clock_get(void);
extern bool     peripheral_clock_enable(em_pconp_t tIndex);
extern bool     peripheral_clock_disable(em_pconp_t tIndex);
extern uint32_t peripheral_clock_get_status(em_pconp_t tIndex);
extern bool     peripheral_clock_resume_status(em_pconp_t tIndex, uint32_t wStatus);
extern bool     peripheral_clock_config(em_pclksel_t tPer, em_pclk_div_t tDiv);
extern uint32_t peripheral_clock_get(em_pclksel_t tPer);
extern bool     clock_out_config(em_out_clk_src_t tSrc, uint32_t wDiv);
extern bool     enter_lowpower_mode(em_lowpower_mode_t tSleep);

#endif
