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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_REG_GPIO_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_REG_GPIO_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define GPIO0_REG           (*(gpio_reg_t *)   (GPIO_BASE_ADDRESS))

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef volatile struct {
    reg8_t B[29];            // 0x0 - 0x1c Byte pin registers P0.0 - P0.28
    reg8_t  __reserved0[4067];
    reg32_t W[29];           // 0x1000 - 0x1074 Word pin registers
    reg32_t __reserved1[995];
    reg32_t DIR;             // 0x2000
    REG32_RSVD(31)
    reg32_t MASK;            // 0x2080
    REG32_RSVD(31)
    reg32_t PIN;             // 0x2100 
    REG32_RSVD(31)
    reg32_t MPIN;            // 0x2180
    REG32_RSVD(31)
    reg32_t SET;             // 0x2200
    REG32_RSVD(31)
    reg32_t CLR;             // 0x2280
    REG32_RSVD(31)
    reg32_t NOT;             // 0x2300
    REG32_RSVD(31)
    reg32_t DIRSET;          // 0x2380
    REG32_RSVD(31)
    reg32_t DIRCLR;          // 0x2400
    REG32_RSVD(31)
    reg32_t DIRNOT;          // 0x2480
} gpio_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  // #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_REG_GPIO_H__
/* EOF */