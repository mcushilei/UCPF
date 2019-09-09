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

#ifndef __REG_UART_H__
#define __REG_UART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define UART0_REG                      (*(uart_reg_t *)UART0_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! UART REGISTER PAGE TYPE 
//! \name usart_t
//! @{
typedef struct {
    union {
        reg8_t COMTX;
        reg8_t COMRX;
        reg8_t COMDIV0;
    }; 
    REG8_RSVD(3)
    
    union {
        reg8_t COMIEN0;
        reg8_t COMDIV1;
    };    
    REG8_RSVD(3)
    
    reg8_t COMIID0;
    REG8_RSVD(3)
        
    reg8_t COMCON0;
    REG8_RSVD(3)
        
    reg8_t COMCON1;
    REG8_RSVD(3)
        
    reg8_t COMSTA0;
    REG8_RSVD(3)
        
    reg8_t COMSTA1;
    REG8_RSVD(3)
    REG32_RSVD(4)
        
    reg16_t COMDIV2;
    REG16_RSVD(1)                   //!< Reserved

} uart_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
