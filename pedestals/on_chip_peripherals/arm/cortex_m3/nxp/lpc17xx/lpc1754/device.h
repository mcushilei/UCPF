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


#ifndef __DRIVER_ARM_M3_NXP_LPC1754_DEVICE_H__
#define __DRIVER_ARM_M3_NXP_LPC1754_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/** @brief IRQ interrupt source definition */
typedef enum {
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
    MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
    BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
    UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
    SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
    DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
    PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
    SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
    WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
    TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
    TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
    TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
    TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
    UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
    UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
    UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
    UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
    PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
    I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
    I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
    I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
    SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
    SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
    SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
    PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
    RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
    EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
    EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
    EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
    EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
    ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
    BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
    USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
    CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
    DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
    I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
    ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
    RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
    MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
    QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
    PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
    USBActivity_IRQn				= 33,		/*!< USB Activity Interrupt 						  */
    CANActivity_IRQn				= 34		/*!< CAN Activity Interrupt 						  */
} IRQn_Type;

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "..\..\..\common\common.h"         /* Cortex-M3 processor and core peripherals           */

/*============================ MACROS ========================================*/

#define UART_COUNT                      4
#define I2C_COUNT                       2
#define SPI_COUNT                       1
#define SSP_COUNT                       2
#define GPIO_COUNT                      5
#define TIMER_COUNT                     4

//! \name IO Controller macros
//! @{
#define IO_PORT_COUNT                   5
#define IO_PORT_PIN_COUNT               32
//! @}

#define FLASH_PAGE_SIZE                 (512u)

//! \note IRC and RTC frequency
//! @{
#define IRC_CLK_FREQ            (4000000u)
#define RTC_CLK_FREQ            (32768u)
//! @}

#define GPIO0_BASE_ADDRESS          (0x2009C000)
#define GPIO1_BASE_ADDRESS          (0x2009C020)
#define GPIO2_BASE_ADDRESS          (0x2009C040)
#define GPIO3_BASE_ADDRESS          (0x2009C060)
#define GPIO4_BASE_ADDRESS          (0x2009C080)
#define UART0_BASE_ADDRESS              (0x4000C000u)
#define UART1_BASE_ADDRESS              (0x40010000u)
#define UART2_BASE_ADDRESS              (0x40098000u)
#define UART3_BASE_ADDRESS              (0x4009C000u)
#define SSP0_BASE_ADDRESS               (0x40088000u)
#define SSP1_BASE_ADDRESS               (0x40030000u)
#define SPI_BASE_ADDRESS                (0x40020000u)
#define I2C0_BASE_ADDRESS               (0x4001C000u)
#define I2C1_BASE_ADDRESS               (0x4005C000u)
#define I2C2_BASE_ADDRESS               (0x400A0000u)
#define RTC_BASE_ADDRESS                (0x40024000u)
#define ADC_BASE_ADDRESS                (0x40034000u)
#define TIM0_BASE_ADDRESS               (0x40004000u)
#define TIM1_BASE_ADDRESS               (0x40008000u)
#define TIM2_BASE_ADDRESS               (0x40010000u)
#define TIM3_BASE_ADDRESS               (0x40014000u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
