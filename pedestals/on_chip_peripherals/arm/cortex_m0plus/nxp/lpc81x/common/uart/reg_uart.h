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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_UART_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_UART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define USART0_REG          (*(uart_reg_t *)    USART0_BASE_ADDRESS)
#define USART1_REG          (*(uart_reg_t *)    USART1_BASE_ADDRESS)
#define USART2_REG          (*(uart_reg_t *)    USART2_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart_reg_t
//! @{
typedef volatile struct {
    reg32_t       CFG;
    reg32_t       CTL;
    reg32_t       STAT;
    reg32_t       INTENSET;
    reg32_t       INTENCLR;
    reg32_t const RXDAT;
    reg32_t const RXDATSTAT;
    reg32_t       TXDAT;
    reg32_t       BRG;
    reg32_t const INTSTAT;
    reg32_t       OSR;
    reg32_t       ADDR;
} uart_reg_t;
//! @}


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
