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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC11E68_DEVICE_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC11E68_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

typedef enum IRQn {
	NonMaskableInt_IRQn           = -14,	/*!< 2 Non Maskable Interrupt                         */
	HardFault_IRQn                = -13,	/*!< 3 Cortex-M0 Hard Fault Interrupt                 */
	SVCall_IRQn                   = -5,		/*!< 11 Cortex-M0 SV Call Interrupt                   */
	PendSV_IRQn                   = -2,		/*!< 14 Cortex-M0 Pend SV Interrupt                   */
	SysTick_IRQn                  = -1,		/*!< 15 Cortex-M0 System Tick Interrupt               */

	PIN_INT0_IRQn                 = 0,		/*!< Pin Interrupt 0                                  */
	PIN_INT1_IRQn                 = 1,		/*!< Pin Interrupt 1                                  */
	PIN_INT2_IRQn                 = 2,		/*!< Pin Interrupt 2                                  */
	PIN_INT3_IRQn                 = 3,		/*!< Pin Interrupt 3                                  */
	PIN_INT4_IRQn                 = 4,		/*!< Pin Interrupt 4                                  */
	PIN_INT5_IRQn                 = 5,		/*!< Pin Interrupt 5                                  */
	PIN_INT6_IRQn                 = 6,		/*!< Pin Interrupt 6                                  */
	PIN_INT7_IRQn                 = 7,		/*!< Pin Interrupt 7                                  */
	GINT0_IRQn                    = 8,		/*!< GPIO interrupt status of port 0                  */
	GINT1_IRQn                    = 9,		/*!< GPIO interrupt status of port 1                  */
	I2C1_IRQn                     = 10,		/*!< I2C1 Interrupt                                   */
	USART1_4_IRQn                 = 11,		/*!< Combined USART1 and USART4 interrupts            */
	USART2_3_IRQn                 = 12,		/*!< Combined USART2 and USART3 interrupts            */
	SCT0_1_IRQn                   = 13,		/*!< Combined SCT0 and SCT1 interrupts                */
	SSP1_IRQn                     = 14,		/*!< SSP1 Interrupt                                   */
	I2C0_IRQn                     = 15,		/*!< I2C0 Interrupt                                   */
	TIMER_16_0_IRQn               = 16,		/*!< 16-bit Timer0 Interrupt                          */
	TIMER_16_1_IRQn               = 17,		/*!< 16-bit Timer1 Interrupt                          */
	TIMER_32_0_IRQn               = 18,		/*!< 32-bit Timer0 Interrupt                          */
	TIMER_32_1_IRQn               = 19,		/*!< 32-bit Timer1 Interrupt                          */
	SSP0_IRQn                     = 20,		/*!< SSP0 Interrupt                                   */
	USART0_IRQn                   = 21,		/*!< USART0 interrupt                                 */
	USB0_IRQn                     = 22,		/*!< USB IRQ interrupt                                */
	USB0_FIQ_IRQn                 = 23,		/*!< USB FIQ interrupt                                */
	ADC_A_IRQn                    = 24,		/*!< ADC_A Converter Interrupt                        */
	RTC_IRQn                      = 25,		/*!< RTC Interrupt                                    */
	BOD_WDT_IRQn                  = 26,		/*!< Shared Brown Out Detect(BOD) and WDT Interrupts  */
	FMC_IRQn                      = 27,		/*!< FLASH Interrupt                                  */
	DMA_IRQn                      = 28,		/*!< DMA Interrupt                                    */
	ADC_B_IRQn                    = 29,		/*!< ADC_B Interrupt                                  */
	USB_WAKEUP_IRQn               = 30,		/*!< USB wake-up interrupt Interrupt                  */
	RESERVED31_IRQn               = 31,
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0_REV                   0x0000  /*!< Cortex-M0 Core Revision                          */
#define __MPU_PRESENT               0       /*!< MPU present or not                               */
#define __VTOR_PRESENT              1       /*!< Set to 1 if vector table is used     */
#define __NVIC_PRIO_BITS            2       /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig      0       /*!< Set to 1 if different SysTick Config is used     */

#include "../../../../CMSIS/Include/core_cm0plus.h"

/*============================ MACROS ========================================*/
#define MAX_MAIN CLOCK_FREQ             (100MHz)
#define MAX_SYS_CLOCK_FREQ              (50MHz)
#define IRC_OSC_FREQ                    (12MHz)
#define RTC_OSC_FREQ                    (32768u)

#define IO_PORT_COUNT                   3
#define IO_PORT_PIN_COUNT               32
#define UART_COUNT                      4
#define SSP_COUNT                       2

#define WDT_BASE_ADDRESS                (0x40004000ul)
#define USART0_BASE_ADDRESS             (0x40008000ul)
#define USART1_BASE_ADDRESS             (0x4006C000ul)
#define USART2_BASE_ADDRESS             (0x40070000ul)
#define USART3_BASE_ADDRESS             (0x40074000ul)
#define USART4_BASE_ADDRESS             (0x4004C000ul)
#define SSP0_BASE_ADDRESS               (0x40040000ul)
#define SSP1_BASE_ADDRESS               (0x40058000ul)
#define IOCON_BASE_ADDRESS              (0x40044000ul)
#define SYSCON_BASE_ADDRESS             (0x40048000ul)
#define RTC_BASE_ADDRESS                (0x40024000ul)
#define GPIO_BASE_ADDRESS               (0xA0000000ul)
#define FLASH_BASE_ADDRESS              (0x4003C000ul)
#define PMU_BASE_ADDRESS                (0x40038000ul)
#define PININT_BASE_ADDRESS             (0xA0004000ul)
#define ADC_BASE_ADDRESS                (0x4001C000u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
