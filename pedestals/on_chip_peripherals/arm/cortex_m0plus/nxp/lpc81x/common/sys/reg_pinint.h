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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_PININT_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_PININT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PININT_REG         (*(pinint_reg_t *)   PININT_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
	reg32_t ISEL;
	reg32_t IENR;
	reg32_t SIENR;
	reg32_t CIENR;
	reg32_t IENF;
	reg32_t SIENF;
	reg32_t CIENF;
	reg32_t RISE;
	reg32_t FALL;
	reg32_t IST;
	reg32_t PMCTRL;
	reg32_t PMSRC;
	reg32_t PMCFG;
} pinint_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __REG_PININT_H__
/* EOF */
