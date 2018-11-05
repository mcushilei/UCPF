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
#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_WDT_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_WDT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define WWDT_REG    (*(wdt_reg_t *) WDT_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name register page type
//! @{
typedef volatile struct {
	reg32_t         MOD;			/*!< Watchdog mode register. This register contains the basic mode and status of the Watchdog Timer. */
	reg32_t         TC;			/*!< Watchdog timer constant register. This register determines the time-out value. */
	reg32_t         FEED;		/*!< Watchdog feed sequence register. Writing 0xAA followed by 0x55 to this register reloads the Watchdog timer with the value contained in WDTC. */
	reg32_t const   TV;			/*!< Watchdog timer value register. This register reads out the current value of the Watchdog timer. */
	REG32_RSVD(1)           	
	reg32_t         WARNINT;		/*!< Watchdog warning interrupt register. This register contains the Watchdog warning interrupt compare value. */
	reg32_t         WINDOW;		/*!< Watchdog timer window register. This register contains the Watchdog window value. */
} wdt_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif  //! #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_WDT_H__
