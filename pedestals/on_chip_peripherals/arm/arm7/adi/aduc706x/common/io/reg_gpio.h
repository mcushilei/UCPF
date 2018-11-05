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

#ifndef __REG_GPIO_H__
#define __REG_GPIO_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define GPIO_PORT0_REG                  (*(gpio_reg_t *)GPIO0_BASE_ADDRESS)
#define GPIO_PORT1_REG                  (*(gpio_reg_t *)GPIO1_BASE_ADDRESS)
#define GPIO_PORT2_REG                  (*(gpio_reg_t *)GPIO2_BASE_ADDRESS)
#define GPIO0_REG                       (*(gpio_reg_t *)GPIO0_BASE_ADDRESS)
#define GPIO1_REG                       (*(gpio_reg_t *)GPIO1_BASE_ADDRESS)
#define GPIO2_REG                       (*(gpio_reg_t *)GPIO2_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name cross switch controller register page
//! @{
typedef volatile struct {
    reg32_t     DAT;
    reg32_t     SET;        //!< Data Output Value Set
    reg32_t     CLR;        //!< Data Output Value Clear
    reg32_t     PAR;             
} gpio_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
