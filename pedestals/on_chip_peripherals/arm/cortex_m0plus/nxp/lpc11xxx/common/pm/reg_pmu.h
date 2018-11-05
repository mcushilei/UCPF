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


#ifndef __REG_PMU_H__
#define __REG_PMU_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PMU_REG                     (*(pmu_reg_t *)PMU_BASE_ADDRESS)

#define PMU_PCON_PM_BIAS            0
#define PMU_PCON_PM_MASK            BITMASK(2, 0)

#define PMU_PCON_NODPD_BIAS         3
#define PMU_PCON_NODPD_MASK         BIT(3)

#define PMU_PCON_SLEEPFLAG_BIAS     8
#define PMU_PCON_SLEEPFLAG_MASK     BIT(8)

#define PMU_PCON_DPDFLAG_BIAS       11
#define PMU_PCON_DPDFLAG_MASK       BIT(11)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type 
//! @{
typedef volatile struct {
	reg32_t PCON;		/*!< Offset: 0x000 Power control Register (R/W) */
	reg32_t GPREG[4];	/*!< Offset: 0x004 General purpose Registers 0..3 (R/W) */
	reg32_t DPDCTRL;	/*!< Offset: 0x014 Deep power-down control register (R/W) */
} pmu_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
