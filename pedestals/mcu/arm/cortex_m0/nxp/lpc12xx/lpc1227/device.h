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


#ifndef __DRIVER_ARM_M0_NXP_LPC1227_DEVICE_H__
#define __DRIVER_ARM_M0_NXP_LPC1227_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
  HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC12xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_IRQn                  = 0,        /*!< All I/O pins can be used as wakeup source.       */
  WAKEUP1_IRQn                  = 1,        /*!< There are 12 pins in total for LPC12xx           */
  WAKEUP2_IRQn                  = 2,
  WAKEUP3_IRQn                  = 3,
  WAKEUP4_IRQn                  = 4,   
  WAKEUP5_IRQn                  = 5,        
  WAKEUP6_IRQn                  = 6,        
  WAKEUP7_IRQn                  = 7,        
  WAKEUP8_IRQn                  = 8,        
  WAKEUP9_IRQn                  = 9,        
  WAKEUP10_IRQn                 = 10,       
  WAKEUP11_IRQn                 = 11,              
  I2C_IRQn                      = 12,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 13,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 14,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 15,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 16,       /*!< 32-bit Timer1 Interrupt                          */
  SSP_IRQn                      = 17,       /*!< SSP Interrupt                                    */
  UART0_IRQn                    = 18,       /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 19,       /*!< UART1 Interrupt                                  */
  CMP_IRQn                      = 20,       /*!< Comparator Interrupt                             */
  ADC_IRQn                      = 21,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 22,       /*!< Watchdog timer Interrupt                         */  
  BOD_IRQn                      = 23,       /*!< Brown Out Detect(BOD) Interrupt                  */
  EINT0_IRQn                    = 25,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 26,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 27,       /*!< External Interrupt 2 Interrupt                   */
  DMA_IRQn                      = 29,       /*!< DMA Interrupt                                    */
  RTC_IRQn                      = 30,       /*!< RTC Interrupt                                    */
} IRQn_Type;


/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

#include "..\..\..\common\common.h"

/*============================ MACROS ========================================*/
//! \name pm
//! @{
#define MAX_PLL_OUTCLK_FREQ             (100MHz)
#define MAX_SYSTEM_CLK_FREQ             (45MHz)
#define IRC_OSC_FREQ                    (12MHz)
//! @}

#define I2C0_BASE_ADDRESS               (0x40000000ul)
#define WDT_BASE_ADDRESS                (0x40004000ul)
#define UART0_BASE_ADDRESS              (0x40008000ul)
#define UART1_BASE_ADDRESS              (0x4000C000ul)
#define TIMER0_BASE_ADDRESS             (0x40010000ul)
#define TIMER1_BASE_ADDRESS             (0x40014000ul)
#define TIMER2_BASE_ADDRESS             (0x40018000ul)
#define TIMER3_BASE_ADDRESS             (0x4001C000ul)
#define ADC_BASE_ADDRESS                (0x40020000ul)
#define PMU_BASE_ADDRESS                (0x40038000ul)
#define SPI0_BASE_ADDRESS               (0x40040000ul)
#define IOCTRL_BASE_ADDRESS             (0x40044000ul)
#define SYSCON_BASE_ADDRESS             (0x40048000ul)
#define RTC_BASE_ADDRESS                (0x40050000ul)
#define GPIO0_BASE_ADDRESS              (0x50000000ul)
#define GPIO1_BASE_ADDRESS              (0x50010000ul)
#define GPIO2_BASE_ADDRESS              (0x50020000ul)
#define FLASH_BASE_ADDRESS              (0x50060000ul)

#define GSP_FLASH                       (*(flash_reg_t *)FLASH_BASE_ADDRESS)
#define GSP_TIMER0                      (*(timer_reg_t *)TIMER0_BASE_ADDRESS)
#define GSP_TIMER1                      (*(timer_reg_t *)TIMER1_BASE_ADDRESS)
#define GSP_TIMER2                      (*(timer_reg_t *)TIMER2_BASE_ADDRESS)
#define GSP_TIMER3                      (*(timer_reg_t *)TIMER3_BASE_ADDRESS)

//! \name adc
//! @{
#define ADC_COUNT                       1
#define ADC0                            ADC[0]
//! @}

//! \name flash controller
//! @{
//! flash register interface base address
//! \brief flash page size
#define FLASH_PAGE_SIZE                 (512ul)
#define FLASH0                          Flash
//! @}

//! \name uart
//! @{
#define UART_COUNT                     2
//! @}

//! \name i2c
//! @{
#define I2C_COUNT                       1
#define I2C0                            I2C[0]
//! @}


//! \name spi0
//! @{
#define SPI_COUNT                       1
#define SPI0                            SPI[0]
//! @}

//! \name IO Controller macros
//! @{
#define IO_PORT_COUNT                   3
#define IO_PORT_PIN_COUNT               32
#define IO_PIN_COUNT                    60
#define IO_PORTA
#define IO_PORTB
#define IO_PORTC
//! @}

//! \name timer
//! @{
#define TIMER_COUNT                     4
#define TIMER0                          TIMER[0]
#define TIMER1                          TIMER[1]
#define TIMER2                          TIMER[2]
#define TIMER3                          TIMER[3]
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name PIN name
//! @{
typedef enum {
    PA19    = 2,
    PA20,
    PA21,
    PA22,
    PA23,
    PA24,
    PA25,
    PA26,
    PA27,
    PC12,
    PC13,
    PC14,
    PC15,
    PA28,
    PA29,
    PA0,
    PA1,
    PA2,
    PA3     = 21,
    PA4,
    PA5,
    PA6,
    PA7,
    PA8,
    PA9,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PA10,
    PA11,
    PA12,
    PA13,
    PA14,
    PA15,
    PA16,
    PA17,
    PA18,
    PA30,
    PA31,
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PB5,
    PB6,
    PC8     = 56,
    PC9,
    PC10,
    PC11,
} em_io_pin_t;
//! @}



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
