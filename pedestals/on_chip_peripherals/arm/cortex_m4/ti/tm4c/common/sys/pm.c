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
#include ".\reg_scon.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define PM_SET_PLL0CON(__VALUE) do {\
            SC_REG.PLL0CON  = __VALUE;\
            SC_REG.PLL0FEED = 0xAA;\
            SC_REG.PLL0FEED = 0x55;\
        } while (0)

#define PM_SET_PLL0CFG(__VALUE) do {\
            SC_REG.PLL0CFG  = __VALUE;\
            SC_REG.PLL0FEED = 0xAA;\
            SC_REG.PLL0FEED = 0x55;\
        } while (0)

#define PM_PLL_MSEL(__VALUE)    (((__VALUE) << SC_PLL0CFG_MSEL0) & SC_PLL0CFG_MSEL_MSK)
#define PM_PLL_NSEL(__VALUE)    (((__VALUE) << SC_PLL0CFG_NSEL0) & SC_PLL0CFG_NSEL_MSK)
        
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
extern bool     peripheral_clock_config(em_pclksel_t tPer , em_pclk_div_t tDiv);
extern uint32_t peripheral_clock_get(em_pclksel_t tPer);
extern bool     clock_out_config(em_out_clk_src_t tSrc, uint32_t wDiv);
extern bool     enter_lowpower_mode(em_lowpower_mode_t tSleep);

/*============================ LOCAL VARIABLES ===============================*/
ROOT const i_pm_t PM = {
    .Power = {
        &peripheral_clock_enable,
        &peripheral_clock_disable,
        &peripheral_clock_get_status,
        &peripheral_clock_resume_status,
    },
    .Clock = {
        .PLL = {
            &pll_enable,
            &pll_disable,
            &pll_clock_select,
            &pll_config,
            &pll_get_out_clock,
            &pll_get_in_clock,
        },
        .Core = {
            &core_clock_cfg,
            &core_clock_get,
        },
        .Peripheral = {
            &peripheral_clock_config,
            &peripheral_clock_get,
        },
        &clock_out_config,
    },
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief  Config main clock.
 *! \param  tSrc[in] specify clock source.
 *!         wDiv[in] specify divider value, should only be 1, 2, 4, 6, ... 256
 *! \retval true for success
 *!         false for invalid argument.
 */
bool core_clock_cfg(em_main_clk_src_t tSrc, uint32_t wDiv)
{
    uint32_t wMainClk;

    if (wDiv > 256) {
        return false;
    }

    wDiv &= ~0x01ul;
    if (wDiv == 0) {
        wDiv = 1;
    }

    SC_REG.FLASHCFG = (5 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK | SC_FLASHTIM_RESVERED_VALUE;
    if (tSrc == MAIN_CLKSRC_PLLOUT) {
        SC_REG.CCLKCFG = (wDiv - 1) & SC_CCLKCFG_CCLKSEL_MSK;
        PM_SET_PLL0CON(SC_REG.PLL0CON | SC_PLL0CON_PLLC0_MSK);
        while ((SC_REG.PLL0STAT & SC_PLL0STAT_PLLC0_MSK) == 0);     //!< wati pll0 to connect.
    } else {
        PM_SET_PLL0CON(SC_REG.PLL0CON & (~SC_PLL0CON_PLLC0_MSK));
        SC_REG.CCLKCFG = (wDiv - 1) & SC_CCLKCFG_CCLKSEL_MSK;
    }

    wMainClk = core_clock_get();
    if        (wMainClk < 20MHz) {
        SC_REG.FLASHCFG = ((0 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    } else if (wMainClk < 40MHz) {
        SC_REG.FLASHCFG = ((1 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    } else if (wMainClk < 60MHz) {
        SC_REG.FLASHCFG = ((2 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    } else if (wMainClk < 80MHz) {
        SC_REG.FLASHCFG = ((3 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    } else if (wMainClk < 100MHz) {
        SC_REG.FLASHCFG = ((4 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    } else {
        SC_REG.FLASHCFG = ((5 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;
    }
    SC_REG.FLASHCFG = ((5 << SC_FLASHTIM_0) & SC_FLASHTIM_MASK) | SC_FLASHTIM_RESVERED_VALUE;

    return true;
}

/*!\brief 
*! \param void
*! \return the system clock
*/
uint32_t core_clock_get(void)
{
    return main_clock_get() / ((SC_REG.CCLKCFG & SC_CCLKCFG_CCLKSEL_MSK) + 1);
}

/*! \brief  Fet main clock frequency.
 *! \param  None
 *! \return main clock frequency
 */
uint32_t main_clock_get(void)
{
    uint32_t wResult;
    
    if (SC_REG.PLL0STAT & SC_PLL0STAT_PLLC0_MSK) {
        wResult = pll_get_out_clock();
    } else {
        wResult = pll_get_in_clock();
    }

    return wResult;
}

bool pll_enable(void)
{
    PM_SET_PLL0CON(SC_REG.PLL0CON | SC_PLL0CON_PLLE0_MSK);      //!< enable pll0
    while ((SC_REG.PLL0STAT & SC_PLL0STAT_PLLE0_MSK) == 0);     //!< wait PLL to enable
    while ((SC_REG.PLL0STAT & SC_PLL0STAT_PLOCK0_MSK) == 0);    //!< wati pll0 to lock(pll stable)
    return true;
}

bool pll_disable(void)
{
    PM_SET_PLL0CON(0);                       //!< disable pll0
    return true;
}

/*! \brief  System clock selection.
 *! \param  tSrc[in], can be one of follow: PLL_CLKSRC_IRC, PLL_CLKSRC_OSC, PLL_CLKSRC_RTC
 *! \retval None
 */
bool pll_clock_select(em_pll_clk_src_t tSrc)
{
    if (tSrc == PLL_CLKSRC_OSC) {
        if (SYS_OSC_FREQ > 15MHz) {
            SC_REG.SCS |= SC_SCS_OSCRANGE_MSK;
        }
        SC_REG.SCS |= SC_SCS_OSCEN_MSK;
        while (!(SC_REG.SCS & SC_SCS_OSCSTAT_MSK));
    } else {
        SC_REG.SCS &= ~SC_SCS_OSCEN_MSK;
    }
    SC_REG.CLKSRCSEL = tSrc;
    
    return true;
}

/*! \brief  Config pll Multiplier value and Pre-Divider value.
 *! \param  wMsel[in] Multiplier value 
 *!         wNsel[in] Pre-Divider value
 *! \retval true for success
 *!         false for invalid argument.
 */
bool pll_config(uint32_t wMsel, uint32_t wNsel)
{
    if ((wMsel < 6) || (wMsel > 512) || (wNsel == 0) || (wNsel > 32)) {
        return false;
    }
    PM_SET_PLL0CON(0);
    PM_SET_PLL0CFG(PM_PLL_MSEL(wMsel - 1) | PM_PLL_NSEL(wNsel - 1));

    return true;
}

/*! \brief  Get pll input clock frequency.
 *! \param  None
 *! \retval the pll input clock frequency
 */
uint32_t pll_get_in_clock(void)
{
    uint32_t wResult = 0;
    
    switch (SC_REG.CLKSRCSEL & SC_CLKSRCSEL_MSK) {
        case PLL_CLKSRC_IRC: 
            wResult = IRC_CLK_FREQ;
            break;
        case PLL_CLKSRC_OSC: 
            wResult = SYS_OSC_FREQ;
            break;
        case PLL_CLKSRC_RTC: 
            wResult = RTC_CLK_FREQ;
            break;
        default:
            break;
    }

    return wResult;
}

/*! \brief  Get pll output clock frequency.
 *! \param  None
 *! \retval the pll output clock frequency
 */
uint32_t pll_get_out_clock(void)
{
    uint32_t wClk = 0;

    wClk = pll_get_in_clock();
    wClk = wClk / (((SC_REG.PLL0STAT & SC_PLL0STAT_NSEL_MSK) >> SC_PLL0STAT_NSEL0) + 1)
            * (((SC_REG.PLL0STAT & SC_PLL0STAT_MSEL_MSK) >> SC_PLL0STAT_MSEL0) + 1)
            * 2;

    return wClk;
}

/*!\brief config the clock out
 *! \param config struct
 *! \return the main clock
 */
bool clock_out_config(em_out_clk_src_t tSrc, uint32_t wDiv)
{
    SC_REG.CLKOUTCFG = (tSrc << SC_CLKOUTCFG_SEL0) & SC_CLKOUTCFG_SEL_MSK 
                     | (wDiv << SC_CLKOUTCFG_DIV0) & SC_CLKOUTCFG_DIV_MSK
                     | SC_CLKOUTCFG_EN_MSK;
    return true;
}

//! \brief enable specified peripheral clock supply
bool peripheral_clock_enable(em_pconp_t tIndex)
{
    if (tIndex > 31) {
        return false;
    }

    SC_REG.PCONP |= BIT(tIndex);

    return true;
}

//! \brief disable specified peripheral clock supply
bool peripheral_clock_disable(em_pconp_t tIndex)
{
    if (tIndex > 31) {
        return false;
    }

    SC_REG.PCONP &= ~(BIT(tIndex));

    return true;
}

//! \brief get specified peripheral clock supply status
uint32_t peripheral_clock_get_status(em_pconp_t tIndex)
{
    if (tIndex > 31) {
        return 0;
    }

    return SC_REG.PCONP & BIT(tIndex);
}

//! \brief Resume specified peripheral clock supply status
bool peripheral_clock_resume_status(em_pconp_t tIndex, uint32_t wStatus)
{
    if (tIndex > 31) {
        return false;
    }

    SC_REG.PCONP = (SC_REG.PCONP & ~BIT(tIndex)) | (wStatus & BIT(tIndex));
    return true;
}

/*! \brief  peripheral clock config
 *! \param  config struct
 *! \return the main clock
 */
bool peripheral_clock_config(em_pclksel_t tPer, em_pclk_div_t tDiv)
{
    if (tPer > 0x80){
        SC_REG.PCLKSEL1 |= (0x03 & tDiv) << (tPer - 0x80);
    } else {
        SC_REG.PCLKSEL0 |= (0x03 & tDiv) << tPer;
    }

    return true;
}

/*!\brief get peripheral clock status
*! \param config struct
*! \return the main clock
*/
static em_pclk_div_t get_peripheral_clock_div(em_pclksel_t tPer)
{
    if (tPer > 0x80){
        return 0x03ul & (SC_REG.PCLKSEL1 >> ((uint32_t)tPer - 0x80));
    } else {
        return 0x03ul & (SC_REG.PCLKSEL0 >> (uint32_t)tPer);
    }
}

/*!\brief get peripheral clock c
*! \param config struct
*! \return the main clock
*/
uint32_t peripheral_clock_get(em_pclksel_t tPer)
{
    uint32_t wCClk;
    uint32_t wDiv;

    switch (get_peripheral_clock_div(tPer)) {
        case PCLK_DIV_4:
            wDiv = 2;
            break;
        case PCLK_DIV_1:
            wDiv = 0;
            break;
        case PCLK_DIV_2:
            wDiv = 1;
            break;
        case PCLK_DIV_8:
            wDiv = 3;
            break;
    }

    wCClk = core_clock_get();

    return  wCClk >> wDiv;
}





/*!\brief enter the lowpower mode
 *! \param the lowpower mode
 */
bool enter_lowpower_mode(em_lowpower_mode_t tSleep)
{
    return true;
}

