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


#ifndef __REG_WDT_H__
#define __REG_WDT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define WDT_REG                     (*(wdt_reg_t *) WDT_BASE)

//! \brief Watchdog Mode register 
#define WDT_MOD_WDEN                0
#define WDT_MOD_WDEN_MSK            (1u << WDT_MOD_WDEN)

#define WDT_MOD_WDRESET             1
#define WDT_MOD_WDRESET_MSK         (1u << WDT_MOD_WDRESET)

#define WDT_MOD_WDTOF               2
#define WDT_MOD_WDTOF_MSK           (1u << WDT_MOD_WDTOF)

#define WDT_MOD_WDINT               3
#define WDT_MOD_WDINT_MSK           (1u << WDT_MOD_WDINT)

#define WDT_CLKSEL_CLKSRC           0
#define WDT_CLKSEL_CLKSRC_IRC       (0u << WDT_CLKSRC)
#define WDT_CLKSEL_CLKSRC_PCLK      (1u << WDT_CLKSRC)
#define WDT_CLKSEL_CLKSRC_RTC       (2u << WDT_CLKSRC)
#define WDT_CLKSEL_CLKSRC_MSK       (WDT_CLKSRC_IRC |\
                                     WDT_CLKSRC_PCLK |\
                                     WDT_CLKSRC_RTC)

#define WDT_CLKSEL_LOCK             31
#define WDT_CLKSEL_LOCK_MSK         (1u << WDT_CLKSEL_LOCK)

//! \brief Watchdog Timer Constant register
#define WDT_TC_WDTC                 0
#define WDT_TC_WDTC_MSK             (0xFFFFFFFFu << WDT_TC_WDTC)

//! \brief Watchdog Feed register
#define WDT_FEED_WDTC               0
#define WDT_FEED_WDTC_MSK           (0xFFu << WDT_FEED_WDTC)

//! \brief Watchdog Timer Value register
#define WDT_TV_WDTC                 0
#define WDT_TV_WDTC_MSK             (0xFFFFFFFFu << WDT_TV_WDTC)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- Watchdog Timer (WDT) -----------------------------------------*/
/** @brief  Watchdog Timer (WDT) register structure definition */
typedef struct {
    reg32_t WDMOD;
    reg32_t WDTC;
    reg32_t WDFEED;
    reg32_t WDTV;
    reg32_t WDCLKSEL;
} wdt_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
