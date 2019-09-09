/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __REG_PININT_H__
#define __REG_PININT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PININT_REG         (*(pinint_reg_t *)PININT_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! System Control Register File
typedef volatile struct {
	reg32_t ISEL;		/*!< Pin Interrupt Mode register */
	reg32_t IENR;		/*!< Pin Interrupt Enable (Rising) register */
	reg32_t SIENR;	/*!< Set Pin Interrupt Enable (Rising) register */
	reg32_t CIENR;	/*!< Clear Pin Interrupt Enable (Rising) register */
	reg32_t IENF;		/*!< Pin Interrupt Enable Falling Edge / Active Level register */
	reg32_t SIENF;	/*!< Set Pin Interrupt Enable Falling Edge / Active Level register */
	reg32_t CIENF;	/*!< Clear Pin Interrupt Enable Falling Edge / Active Level address */
	reg32_t RISE;		/*!< Pin Interrupt Rising Edge register */
	reg32_t FALL;		/*!< Pin Interrupt Falling Edge register */
	reg32_t IST;		/*!< Pin Interrupt Status register */
	reg32_t PMCTRL;	/*!< GPIO pattern match interrupt control register          */
	reg32_t PMSRC;	/*!< GPIO pattern match interrupt bit-slice source register */
	reg32_t PMCFG;	/*!< GPIO pattern match interrupt bit slice configuration register */
} pinint_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __REG_PININT_H__
/* EOF */
