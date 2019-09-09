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

#ifndef __REG_IOCTRL_H__
#define __REG_IOCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IOCTRL_REG              (*(ioctrl_reg_t *)IOCTRL_BASE_ADDRESS)
#define GP0CON1_REG             (*(gp0con1_reg_t *)GP0CON1_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name io controller register page
//! @{
typedef volatile struct {
    reg32_t GP0CON0;
    reg32_t GP1CON;
    reg32_t GP2CON;
} ioctrl_reg_t;
//! @}

//! \name io controller register page
//! @{
typedef volatile struct {
    reg16_t     GP0KEY1;
    REG16_RSVD(1)
        
    reg8_t      GP0CON1;
    REG8_RSVD(3)
        
    reg16_t     GP0KEY2;
    REG16_RSVD(1)
} gp0con1_reg_t;
//! @}


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
