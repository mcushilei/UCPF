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


#ifndef __REG_PM_H__
#define __REG_PM_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PLL_REG                     (*(pll_reg_t *)PLL_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type 
//! @{
typedef volatile struct {
    REG32_RSVD(1)
        
    reg16_t     POWKEY1;
    REG16_RSVD(1)
        
    reg8_t      POWCON0;
    REG8_RSVD(3)
        
    reg16_t     POWKEY2;
    REG16_RSVD(1)
        
    reg16_t     PLLKEY1;
    REG16_RSVD(1)
        
    reg8_t      PLLCON;
    REG8_RSVD(3)
        
    reg16_t     PLLKEY2;
    REG16_RSVD(1)
        
    REG32_RSVD(6)
            
    reg16_t     POWKEY3;
    REG16_RSVD(1)
        
    reg16_t      POWCON1;
    REG16_RSVD(1)
        
    reg16_t     POWKEY4;
    REG16_RSVD(1)
        
    REG32_RSVD(9)
      
} pll_reg_t;
//! @}



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif