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
#include ".\reg_syscon.h"
#include ".\reg_pmu.h"
#include "..\io\io.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define IS_POWER_ENABLED(__PART)    ((~SYSCON_REG.PDRUNCFG) & parts)

/*============================ TYPES =========================================*/
//! \name the lowpower mode
//! @{
typedef enum {
    SLEEP           = 1,
    DEEP_SLEEP      = 2,
    POWER_DOWN      = 3,
    DEEP_POWER_DOWN = 4,
} em_lowpower_mode_t;
//! @}

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool power_enable(uint32_t parts)
{
    SAFE_ATOM_CODE(
        SYSCON_REG.PDRUNCFG &= ~parts;
    )
    return true;
}

bool power_disable(uint32_t parts)
{
    SAFE_ATOM_CODE(
        SYSCON_REG.PDRUNCFG |= parts;
    )
    return true;
}

uint32_t power_status_get(uint32_t parts)
{
    return SYSCON_REG.PDRUNCFG & parts;
}

bool power_status_resume(uint32_t parts , uint32_t status)
{
    SAFE_ATOM_CODE(
        SYSCON_REG.PDRUNCFG = (SYSCON_REG.PDRUNCFG & ~parts) 
                            | (status & parts);
    )
    return true;
}

static void sysosc_enable(void)
{
    uint32_t RegValue = 0;
    
    IO_CFG(
        {PORT2, PIN0, IO_WORKS_AS_FUNC1, IO_INACTIVE},
        {PORT2, PIN1, IO_WORKS_AS_FUNC1, IO_INACTIVE},
    );
    
    if (SYSOSC_BYPASS_EN) {
        RegValue |= SCON_SYSOSC_BYPASS_MASK;
    }
    if (SYS_OSC_FREQ > 15MHz) {
        RegValue |= SCON_SYSOSC_FREQRANGE_MASK;
    }
    SYSCON_REG.SYSOSCCTRL = RegValue;
    
    power_enable(POWER_SYSOSC);
    for (volatile uint32_t i = 25000; i; i--);      //! wait OSC to stable for at least 500us.
}

bool pll_enable(void)
{
    power_enable(POWER_SYSPLL);
    for (uint32_t i = 60000; i; i--) {
        if ((SYSCON_REG.SYSPLLSTAT & SCON_SYSPLLSTAT_LOCK_MASK) != 0) {
            return true;
        }
    }
    power_disable(POWER_SYSPLL);
    return false;
}

bool pll_disable(void)
{
    power_disable(POWER_SYSPLL);
    return true;
}

static bool pll_clock_source_select(uint32_t Source)
{
    //! enable clock source.
    switch (Source) {
        case SYSPLL_CKLSRC_IRC:
            power_enable(POWER_IRC);
            break;
        case SYSPLL_CLKSRC_SYSOSC:
            sysosc_enable();
            break;
        case SYSPLL_CLKSRC_RTC:
            SYSCON_REG.RTCOSCCTRL = SCON_RTCOSCCTRL_RTCOSCEN_MASK;
            break;
        default:
            return false;
    }
    
    //! select clock source.
    SYSCON_REG.SYSPLLCLKSEL = Source;
    SYSCON_REG.SYSPLLCLKUEN = 0;
    SYSCON_REG.SYSPLLCLKUEN = 1;
    
    return true;
}

uint32_t pll_get_in_clock(void)
{
    uint32_t wResult = 0;
    
    switch ((SYSCON_REG.SYSPLLCLKSEL & SCON_SYSPLLCLKSEL_MASK) >> SCON_SYSPLLCLKSEL_BIAS) {
        case SYSPLL_CKLSRC_IRC: 
            wResult = IRC_OSC_FREQ;
            break;
        case SYSPLL_CLKSRC_SYSOSC: 
            wResult = SYS_OSC_FREQ;
            break;
        case SYSPLL_CLKSRC_RTC:
            wResult = RTC_OSC_FREQ;
            break;
        default:
            wResult = 0;
            break;
    }

    return wResult;
}

uint32_t pll_get_out_clock(void)
{
    uint32_t wClk = 0;

    wClk = pll_get_in_clock();
    wClk *= ((SYSCON_REG.SYSPLLCTRL & SCON_SYSPLLCTRL_MSEL_MASK) >> SCON_SYSPLLCTRL_MSEL_BIAS)
            + 1;

    return wClk;
}

bool pll_init(uint32_t Source, uint32_t Freq)
{
    uint32_t m, p;
    uint32_t SourceFreq;
    
    power_disable(POWER_SYSPLL);
    
    switch (Source) {
        case SYSPLL_CKLSRC_IRC:
        case SYSPLL_CLKSRC_SYSOSC:
            break;
        default:
            return false;
    }
    if (!pll_clock_source_select(Source)) {
        return false;
    }
    SourceFreq = pll_get_in_clock();

    m = Freq / (SourceFreq);
    if ((m == 0) || (m > 32)) {
        return false;
    }
    Freq = m * SourceFreq;
    m--;

    for (p = 0; p < 4; p++) {
        uint32_t fcco = Freq * BIT(p + 1);
        if ((fcco > 156 MHz) && (fcco < 320 MHz)) {
            break;
        }
    }
    if (p >= 4) {
        return false;
    }
    SYSCON_REG.SYSPLLCTRL = ((m << SCON_SYSPLLCTRL_MSEL_BIAS) & SCON_SYSPLLCTRL_MSEL_MASK)
                          | ((p << SCON_SYSPLLCTRL_PSEL_BIAS) & SCON_SYSPLLCTRL_PSEL_MASK);
    
    power_enable(POWER_SYSPLL);
    for (uint32_t i = 60000; i; i--) {
        if ((SYSCON_REG.SYSPLLSTAT & SCON_SYSPLLSTAT_LOCK_MASK) != 0) {
            return true;
        }
    }
    power_disable(POWER_SYSPLL);
    return false;
}

//! \brief enable part's ahb clock
bool ahb_clock_enable(uint32_t Part)
{
    if (Part > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL |= BIT(Part);

    return true;
}

//! \brief disable part's ahb clock
bool ahb_clock_disable(uint32_t Part)
{
    if (Part > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL &= ~BIT(Part);

    return true;
}

//! \brief get part's ahb clock status
uint32_t ahb_clock_get_status(uint32_t Part)
{
    if (Part > 31) {
        return 0;
    }

    return SYSCON_REG.SYSAHBCLKCTRL & BIT(Part);
}

//! \brief Resume part's ahb clock status
bool ahb_clock_resume_status(uint32_t Part, uint32_t status)
{
    if (Part > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL = (SYSCON_REG.SYSAHBCLKCTRL & ~BIT(Part))
                             | (status & BIT(Part));

    return true;
}


#define SAFE_CLK_CODE(...)  \
    {\
        uint32_t status   = ahb_clock_get_status(AHBCLK_FLASHREG);       \
        ahb_clock_enable(AHBCLK_FLASHREG);                                \
        __VA_ARGS__;                                                    \
        ahb_clock_resume_status(AHBCLK_FLASHREG, status);                \
    }

bool core_clock_config(uint32_t Source, uint32_t Div)
{
    if (0 == Div) {
        return false;
    }

    switch (Source) {
        case MAIN_CLKSRC_IRC:
            power_enable(POWER_IRC);
            break;
            
        case MAIN_CLKSRC_PLLIN:
            if((pll_get_in_clock() / Div) > MAX_SYS_CLOCK_FREQ) {
                return false;
            }
            break;
            
        case MAIN_CLKSRC_WDTOSC:
            power_enable(POWER_WDTOSC);
            break;
            
        case MAIN_CLKSRC_PLLOUT:
            if((pll_get_out_clock() / Div) > MAX_SYS_CLOCK_FREQ) {
                return false;
            }
            break;
            
        default:
            return false;
    }
    
    //! Switch main clock to internal RC oscilator.
    SYSCON_REG.MAINCLKSEL = MAIN_CLKSRC_IRC;
    SYSCON_REG.MAINCLKUEN = 0;
    SYSCON_REG.MAINCLKUEN = 1;

    //! config flash access time to avoid error to switch a high frq clk.
    //! Note: IRC is 12MHz which below 24MHz, that is definite.
    SAFE_CLK_CODE(
        uint32_t Freq;
        switch (Source) {
            case MAIN_CLKSRC_PLLOUT:
                Freq = pll_get_out_clock() / Div;
                break;
            case MAIN_CLKSRC_PLLIN:
                Freq = pll_get_in_clock() / Div;
                break;
            case MAIN_CLKSRC_IRC:
                Freq = IRC_OSC_FREQ / Div;
                break;
            case MAIN_CLKSRC_WDTOSC:
                Freq = 1MHz;
                break;
        }
        FLASH_REG.FLASHCFG &= ~SCON_FLASHCFG_TIM_MASK;  //! 1 clk wait state
        if (Freq < 20MHz) {   //! 2 clk wait state
        } else if (Freq < 40MHz) {
            FLASH_REG.FLASHCFG = (FLASH_REG.FLASHCFG & (~SCON_FLASHCFG_TIM_MASK)) | 1;
        } else {
            FLASH_REG.FLASHCFG = (FLASH_REG.FLASHCFG & (~SCON_FLASHCFG_TIM_MASK)) | 2;
        }
    )
    
    //! Switch main clock to specify source.
    SYSCON_REG.SYSAHBCLKDIV = Div;   //! set the system clock divider
    SYSCON_REG.MAINCLKSEL   = Source;   //! set the main clock source
    SYSCON_REG.MAINCLKUEN   = 0;
    SYSCON_REG.MAINCLKUEN   = 1;

    return true;
}

uint32_t main_clock_get(void)
{
    uint32_t wResult = 0;
    
    //! \brief check the main clock select register
    switch (SYSCON_REG.MAINCLKSEL & SCON_MAINCLKSEL_SEL_MASK){
        case MAIN_CLKSRC_IRC:
            wResult = IRC_OSC_FREQ;
            break;
        case MAIN_CLKSRC_PLLIN:
            wResult = pll_get_in_clock();
            break;
        case MAIN_CLKSRC_WDTOSC:
            break;
        case MAIN_CLKSRC_PLLOUT: 
            wResult = pll_get_out_clock();
            break;
    }

    return wResult;
}

uint32_t core_clock_get(void)
{
    return main_clock_get() / SYSCON_REG.SYSAHBCLKDIV;
}

bool scon_usart_clock_enable(void)
{
    SYSCON_REG.FRGCLKDIV   = 1u;
    return true;
}

bool scon_usart_clock_disable(void)
{
    SYSCON_REG.FRGCLKDIV   = 0u;
    return true;
}

uint32_t scon_usart_clock_get(void)
{
    return main_clock_get();
}

bool enter_lowpower_mode(uint32_t mode)
{
    switch (mode) {
        case SLEEP:
            PMU_REG.PCON = 0u << PMU_PCON_PM_BIAS;
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            __WFI();
            break;
        case DEEP_SLEEP:
            PMU_REG.PCON = 1u << PMU_PCON_PM_BIAS;
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFI();
            break;
        case POWER_DOWN:
            PMU_REG.PCON = 2u << PMU_PCON_PM_BIAS;
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __WFI();
            break;
    }

    return true;
}

