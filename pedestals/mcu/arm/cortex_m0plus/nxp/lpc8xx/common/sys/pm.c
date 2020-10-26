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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_syscon.h"
#include ".\reg_pmu.h"
#include ".\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
static void sysosc_enable(void);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


/* Peripheral power management ----------------------------------------*/
bool power_enable(uint32_t parts)
{
    SYSCON_REG.PDRUNCFG &= ~parts;
    return true;
}

bool power_disable(uint32_t parts)
{
    SYSCON_REG.PDRUNCFG |= parts;
    return true;
}

uint32_t power_status_get(uint32_t parts)
{
    return SYSCON_REG.PDRUNCFG & parts;
}

bool power_status_resume(uint32_t parts , uint32_t status)
{
    SYSCON_REG.PDRUNCFG = (SYSCON_REG.PDRUNCFG & ~parts) 
                        | (status & parts);
    return true;
}


void sysctrl_periph_reset(uint32_t mask)
{
    SYSCON_REG.PRESETCTRL &= ~mask;
    SYSCON_REG.PRESETCTRL |=  mask;
}

/* clock management ------------------------------------------------------*/
//! \brief enable part's clock
bool clock_enable(uint32_t Part)
{
    if (Part > 31u) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL |= BIT(Part);

    return true;
}

//! \brief disable part's clock
bool clock_disable(uint32_t Part)
{
    if (Part > 31u) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL &= ~BIT(Part);

    return true;
}

//! \brief get part's clock status
uint32_t clock_get_status(uint32_t Part)
{
    if (Part > 31u) {
        return 0;
    }

    return SYSCON_REG.SYSAHBCLKCTRL & BIT(Part);
}

//! \brief Resume part's clock status
bool clock_resume_status(uint32_t Part, uint32_t status)
{
    if (Part > 31u) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL = (SYSCON_REG.SYSAHBCLKCTRL & ~BIT(Part))
                             | (status & BIT(Part));

    return true;
}


/* PLL management ------------------------------------------------------------*/
bool pll_enable(void)
{
    power_enable(POWER_SYSPLL);
    for (uint32_t i = 60000u; i; i--) {
        if ((SYSCON_REG.SYSPLLSTAT & BIT(0)) != 0u) {
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

static bool pll_clock_source_select(uint32_t source)
{
    //! enable clock source.
    switch (source) {
        case SYSPLL_CKLSRC_IRC:
            power_enable(POWER_IRC);
            break;
        case SYSPLL_CLKSRC_SYSOSC:
            sysosc_enable();
            break;
        case SYSPLL_CLKSRC_CLKIN:
            break;
        default:
            return false;
    }
    
    //! select clock source.
    SYSCON_REG.SYSPLLCLKSEL = (source << 0);
    SYSCON_REG.SYSPLLCLKUEN = 0;
    SYSCON_REG.SYSPLLCLKUEN = 1;
    
    return true;
}

uint32_t pll_get_in_clock(void)
{
    uint32_t wResult = 0;
    
    switch ((SYSCON_REG.SYSPLLCLKSEL & BITMASK(1, 0)) >> 0) {
        case 0x00: 
            wResult = IRC_OSC_FREQ;
            break;
        case 0x01:
            wResult = SYS_OSC_FREQ;
            break;
        case 0x03:
            wResult = CLKIN_FREQ;
            break;
        default:
            break;
    }

    return wResult;
}

uint32_t pll_get_out_clock(void)
{
    uint32_t wClk = 0;

    wClk  = pll_get_in_clock();
    wClk *= ((SYSCON_REG.SYSPLLCTRL & BITMASK(4, 0)) >> 0) + 1;

    return wClk;
}

bool pll_init(uint32_t source, uint32_t freq)
{
    uint32_t m, p;
    uint32_t sourceFreq;
    
    power_disable(POWER_SYSPLL);
    
    if (freq > 100MHz) {
        return false;
    }

    if (!pll_clock_source_select(source)) {
        return false;
    }
    sourceFreq = pll_get_in_clock();
    if ((sourceFreq < 10MHz) || (sourceFreq > 25MHz)) {
        return false;
    }

    m = freq / (sourceFreq);
    if ((m == 0) || (m > 32)) {
        return false;
    }
    freq = m * sourceFreq;
    m--;

    for (p = 0; p < 4; p++) {
        uint32_t fcco = freq * BIT(p + 1);
        if ((fcco > 156MHz) && (fcco < 320MHz)) {
            break;
        }
    }
    if (p >= 4) {
        return false;
    }
    SYSCON_REG.SYSPLLCTRL = ((m << 0) & BITMASK(4, 0))
                          | ((p << 5) & BITMASK(6, 5));
    
    return true;
}



static void sysosc_enable(void)
{
    uint32_t regValue = 0;
    
    if (SYSOSC_BYPASS_EN) {
        regValue |= (1u << 0);
    }
    if (SYS_OSC_FREQ > 15MHz) {
        regValue |= (1u << 1);
    }
    SYSCON_REG.SYSOSCCTRL = regValue;
    
    power_enable(POWER_SYSOSC);
    for (volatile uint32_t i = 25000; i; i--);      //! wait OSC to stable for at least 500us.
}


bool wdtosc_enable(void)
{
    SYSCON_REG.WDTOSCCTRL = (0 << 0) | (1 << 5);
    power_enable(POWER_WDTOSC);
    
    return true;
}

uint32_t wdtosc_clock_get(void)
{
    static const uint32_t clockTable[16] = {
           0u,    600KHz, 1050KHz, 1400KHz,
        1750KHz, 2100KHz, 2400KHz, 2700KHz,
        3000KHz, 3250KHz, 3500KHz, 3750KHz,
        4000KHz, 4200KHz, 4400KHz, 4600KHz,
    };
    
    if (!power_status_get(POWER_WDTOSC)) {
        uint32_t regValue = SYSCON_REG.WDTOSCCTRL;
        uint32_t clk;
        clk = clockTable[(regValue & BITMASK(8, 5)) >> 5];
        clk /= 2u * ( 1u + (regValue & BITMASK(4, 0)) );
        return clk;
    }
    
    return 0u;
}


#define FLASHCON_SAFE_CLK_CODE(...)                                 \
    {                                                               \
        clock_enable(CLOCK_FLASHREG);                               \
        __VA_ARGS__;                                                \
    }

bool core_clock_config(uint32_t source, uint32_t div)
{
    if (0 == div) {
        return false;
    }

    switch (source) {
        case MAIN_CLKSRC_IRC:
            power_enable(POWER_IRC);
            break;
            
        case MAIN_CLKSRC_PLLIN:
            if((pll_get_in_clock() / div) > 30MHz) {
                return false;
            }
            break;
            
        case MAIN_CLKSRC_WDTOSC:
            power_enable(POWER_WDTOSC);
            break;
            
        case MAIN_CLKSRC_PLLOUT:
            if((pll_get_out_clock() / div) > 30MHz) {
                return false;
            }
            break;
            
        default:
            return false;
    }
    
    //! temporarily set flash the slowest speed. 
    FLASHCON_SAFE_CLK_CODE(
        uint32_t flashcfg = FLASHCTRL_REG.FLASHCFG & ~(BITMASK(1, 0));
        flashcfg |= 0x01u << 0;
        FLASHCTRL_REG.FLASHCFG = flashcfg;
    )

    //! temporarily switch main clock to IRC which is slower.
    SYSCON_REG.MAINCLKSEL = MAIN_CLKSRC_IRC;
    SYSCON_REG.MAINCLKUEN = 0;
    SYSCON_REG.MAINCLKUEN = 1;

    //! config flash access time to avoid error to switch a high frq clk.
    uint32_t freq;
    switch (source) {
        case MAIN_CLKSRC_PLLOUT:
            freq = pll_get_out_clock() / div;
            break;
        case MAIN_CLKSRC_PLLIN:
            freq = pll_get_in_clock() / div;
            break;
        case MAIN_CLKSRC_IRC:
            freq = IRC_OSC_FREQ / div;
            break;
        case MAIN_CLKSRC_WDTOSC:
            freq = 1MHz;
            break;
    }
    FLASHCON_SAFE_CLK_CODE(
        uint32_t flashcfg = FLASHCTRL_REG.FLASHCFG;
        FLASHCTRL_REG.FLASHCFG = flashcfg;
    )
    
    //! Switch main clock to specify source.
    SYSCON_REG.SYSAHBCLKDIV = div;      //! set the system clock divider
    SYSCON_REG.MAINCLKSEL   = source;   //! set the main clock source
    SYSCON_REG.MAINCLKUEN   = 0;
    SYSCON_REG.MAINCLKUEN   = 1;

    return true;
}

uint32_t core_clock_get(void)
{
    return main_clock_get() / SYSCON_REG.SYSAHBCLKDIV;
}

uint32_t main_clock_get(void)
{
    uint32_t wResult = 0;
    
    //! \brief check the main clock select register
    switch (SYSCON_REG.MAINCLKSEL & BITMASK(1, 0) >> 0){
        case 0x00:
            wResult = IRC_OSC_FREQ;
            break;
            
        case 0x01:
            wResult = pll_get_in_clock();
            break;
            
        case 0x02:
            break;
            
        case 0x03:
            wResult = pll_get_out_clock();
            break;
    }

    return wResult;
}

// where: frac = main_clock / div * 256 / wanted_clock - 256
bool uart_clock_config(uint32_t div, uint32_t frac)
{
    SYSCON_REG.UARTCLKDIV  = div;
    SYSCON_REG.UARTFRGMULT = frac;
    SYSCON_REG.UARTFRGDIV  = 0xFFu;
    
    return true;
}

uint32_t uart_clock_get(void)
{
    uint32_t clock;
    uint32_t div;
    
    div = SYSCON_REG.UARTCLKDIV;
    if (div == 0u) {
        return 0u;
    }
    clock = main_clock_get();
    clock /= div;
    clock *= 256u;
    clock /= 256u + SYSCON_REG.UARTFRGMULT;
    
    return clock;
}


bool enter_lowpower_mode(uint32_t mode)
{
//    switch (mode) {
//        case SLEEP:
//            PMU_REG.PCON = 0u << PMU_PCON_PM_BIAS;
//            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
//            __WFI();
//            break;
//        case DEEP_SLEEP:
//            PMU_REG.PCON = 1u << PMU_PCON_PM_BIAS;
//            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//            __WFI();
//            break;
//        case POWER_DOWN:
//            PMU_REG.PCON = 2u << PMU_PCON_PM_BIAS;
//            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//            __WFI();
//            break;
//    }

    return true;
}

