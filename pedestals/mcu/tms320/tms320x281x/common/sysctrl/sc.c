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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_sc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
    LOW_SPEED_PCLK  = 0,
    HIGH_SPEED_PCLK,
} em_pclksel_t;

typedef enum {
    PCLK_DIV_1      = 0,
    PCLK_DIV_2,
    PCLK_DIV_4,
    PCLK_DIV_6,
    PCLK_DIV_8,
    PCLK_DIV_10,
    PCLK_DIV_12,
    PCLK_DIV_14,
} em_pclk_div_t;

//! \brief power interface
//! @{
DEF_INTERFACE(i_ppwr_t)
    bool                        (*Enable)(em_pclkcr_t tIndex);
    bool                        (*Disable)(em_pclkcr_t tIndex);
    uint16_t                    (*GetStatus)(em_pclkcr_t tIndex);
    bool                        (*ResumeStatus)(em_pclkcr_t tIndex, uint16_t tStatus);
END_DEF_INTERFACE(i_ppwr_t)
//! @}

//! \name pll interface
//! @{
DEF_INTERFACE(i_pll_t)
    bool                        (*Config)(uint16_t chNsel);
    uint32_t                    (*GetClockIn)(void);
    uint32_t                    (*GetClockOut)(void);
END_DEF_INTERFACE(i_pll_t)
//! @}

//! \name main clock interface
//! @{
DEF_INTERFACE(i_main_clk_t)
    uint32_t                    (*Get)(void);
END_DEF_INTERFACE(i_main_clk_t)
//! @}

//! \name main clock interface
//! @{
DEF_INTERFACE(i_core_clk_t)
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
    i_ppwr_t                    Peripheral;
END_DEF_INTERFACE(i_power_t)
//! @}

//! \brief clock interface
//! @{
DEF_INTERFACE(i_clk_t)
    i_pll_t                     PLL;
    i_main_clk_t                Main;
    i_core_clk_t                Core;
    i_pclk_t                    Peripheral;
END_DEF_INTERFACE(i_clk_t)
//! @}

//! \brief pm interface
//! @{
DEF_INTERFACE(i_pm_t)
    i_power_t                   Power;
    i_clk_t                     Clock;
END_DEF_INTERFACE(i_pm_t)
//! @}

/*============================ PROTOTYPES ====================================*/
extern bool pll_cfg(uint16_t hwMultiplier);
extern uint32_t pll_get_in_clock_fre(void);
extern uint32_t pll_get_out_clock_fre(void);
extern uint32_t main_clock_get(void);
extern bool peripheral_clock_enable(em_pclkcr_t tIndex);
extern bool peripheral_clock_disable(em_pclkcr_t tIndex);
extern uint16_t peripheral_clock_get_status(em_pclkcr_t tIndex);
extern bool peripheral_clock_resume_status(em_pclkcr_t tIndex, uint16_t tStatus);
extern bool peripheral_clock_cfg(em_pclksel_t tPer, em_pclk_div_t tDiv);
extern uint32_t peripheral_clock_get(em_pclksel_t tPer);

/*============================ LOCAL VARIABLES ===============================*/
const i_pm_t PM = {
        .Clock = {
            .PLL = {
                .Config      = pll_cfg,
                .GetClockIn  = &pll_get_in_clock_fre,
                .GetClockOut = &pll_get_out_clock_fre,
            },
            .Main = {
                .Get    = &main_clock_get,
            },
            .Core = {
                .Get    = &main_clock_get,
            },
            .Peripheral = {
                .Config = &peripheral_clock_cfg,
                .Get    = &peripheral_clock_get,
            },
        },
        .Power = {
            .Peripheral = {
                .Enable       = &peripheral_clock_enable,
                .Disable      = &peripheral_clock_disable,
                .GetStatus    = &peripheral_clock_get_status,
                .ResumeStatus = &peripheral_clock_resume_status,
            },
        },
    };

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool pll_cfg(uint16_t hwMultiplier)
{
    volatile uint16_t hwCnt;

    if (hwMultiplier > 10) {
        return false;
    }

    if (hwMultiplier == SYS_REG.PLLCR.DIV) {
        return true;
    }

    EALLOW();
    SYS_REG.PLLCR.DIV = hwMultiplier;
    EDIS();

    for (hwCnt = ((131072/2)/12); hwCnt; --hwCnt) {
        //! feed dog?
    }

    return true;
}

uint32_t pll_get_in_clock_fre(void)
{
    return SYS_OSC_FREQ;
}

uint32_t pll_get_out_clock_fre(void)
{
    switch (SYS_REG.PLLCR.DIV) {
        case 0:
        case 1:
            return SYS_OSC_FREQ / 2;
        default:
            return SYS_REG.PLLCR.DIV * SYS_OSC_FREQ / 2;
    }
}

uint32_t main_clock_get(void)
{
#if PLL_ENABLED == ENABLED
    return pll_get_out_clock_fre();
#else
    return SYS_OSC_FREQ;
#endif
}

//! \brief enable specified peripheral clock supply
bool peripheral_clock_enable(em_pclkcr_t tIndex)
{
    if (tIndex > 16) {
        return false;
    }

    EALLOW();
    SYS_REG.PCLKCR.Value |= BIT(tIndex);
    EDIS();

    return true;
}

//! \brief disable specified peripheral clock supply
bool peripheral_clock_disable(em_pclkcr_t tIndex)
{
    if (tIndex > 16) {
        return false;
    }

    EALLOW();
    SYS_REG.PCLKCR.Value &= ~(BIT(tIndex));
    EDIS();

    return true;
}

//! \brief get specified peripheral clock supply status
uint16_t peripheral_clock_get_status(em_pclkcr_t tIndex)
{
    if (tIndex > 16) {
        return 0;
    }

    return SYS_REG.PCLKCR.Value & BIT(tIndex);
}

//! \brief Resume specified peripheral clock supply
bool peripheral_clock_resume_status(em_pclkcr_t tIndex, uint16_t tStatus)
{
    if (tIndex > 16) {
        return false;
    }

    EALLOW();
    SYS_REG.PCLKCR.Value = (SYS_REG.PCLKCR.Value & ~BIT(tIndex)) | tStatus;
    EDIS();

    return true;
}

bool peripheral_clock_cfg(em_pclksel_t tPer, em_pclk_div_t tDiv)
{
    switch (tPer) {
        case LOW_SPEED_PCLK:
            EALLOW();
            SYS_REG.LOSPCP.LSPCLK = tDiv;
            EDIS();
            break;
        case HIGH_SPEED_PCLK:
            EALLOW();
            SYS_REG.HISPCP.HSPCLK = tDiv;
            EDIS();
            break;
        default:
            return false;
    }

    return true;
}

uint32_t peripheral_clock_get(em_pclksel_t tPer)
{
    uint32_t wRetVal = 0;

    switch (tPer) {
        case LOW_SPEED_PCLK:
            wRetVal = SYS_REG.LOSPCP.LSPCLK;
            break;
        case HIGH_SPEED_PCLK:
            wRetVal = SYS_REG.HISPCP.HSPCLK;
            break;
        default:
            return 0;
    }

    if (wRetVal) {
        wRetVal = main_clock_get() / (wRetVal * 2);
    } else {
        wRetVal = main_clock_get();
    }

    return wRetVal;
}

/* EOF */
