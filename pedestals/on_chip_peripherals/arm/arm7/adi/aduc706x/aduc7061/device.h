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


#ifndef __DRIVER_ARM_ARM7_ADUC7061_DEVICE_H__
#define __DRIVER_ARM_ARM7_ADUC7061_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\..\..\common\common.h"         /* Cortex-M3 processor and core peripherals           */

/*============================ MACROS ========================================*/
#define FLASH_PAGE_SIZE                 (512ul)
#define FLASH_BASE_ADDR                 (0x80000u)
#define FLASH_SIZE                      (30u * 1024u)

#define UART_COUNT                      1
#define I2C_COUNT                       1
#define SPI_COUNT                       1
#define GPIO_COUNT                      3
#define TIMER_COUNT                     4

#define UART0                           UART[0]

//! \name IO Controller macros
//! @{
#define IO_PORT_PIN_COUNT               8
#define PIO_PORTA
#define PIO_PORTB
#define PIO_PORTC
//! @}

//! \note interanal (RC)oscillator frequency
//! @{
#define INT_OSC_FREQ            (32768ul)
//! @}
#define PLL_OUT_FREQ            (10240000ul)

#define PLL_BASE_ADDRESS        (0xFFFF0400u)
#define IRQ_BASE_ADDRESS        (0xFFFF0000u)
#define SYS_BASE_ADDRESS        (0xFFFF0200u)
#define TIMER0_BASE_ADDRESS     (0xFFFF0320u)
#define TIMER1_BASE_ADDRESS     (0xFFFF0340u)
#define TIMER2_BASE_ADDRESS     (0xFFFF0360u)
#define TIMER3_BASE_ADDRESS     (0xFFFF0380u)
#define IOCTRL_BASE_ADDRESS     (0xFFFF0D00u)
#define GP0CON1_BASE_ADDRESS    (0xFFFF0464u)
#define GPIO0_BASE_ADDRESS      (0xFFFF0D20u)
#define GPIO1_BASE_ADDRESS      (0xFFFF0D30u)
#define GPIO2_BASE_ADDRESS      (0xFFFF0D40u)
#define ADC_BASE_ADDRESS        (0xFFFF0500u)
#define DAC0_BASE_ADDRESS       (0xFFFF0600u)
#define UART0_BASE_ADDRESS      (0xFFFF0700u)
#define FLASH_BASE_ADDRESS      (0xFFFF0E00u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
