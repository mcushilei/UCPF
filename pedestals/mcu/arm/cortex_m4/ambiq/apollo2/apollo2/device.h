/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
 *                                                                            *
 *  This program is free SOFTWARE; you can redistribute it and/or modify it   *
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

#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_APOLLO2_DEVICE_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_APOLLO2_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

typedef enum {
    NonMaskableInt_IRQn     = -14,      /*!< 2  Cortex-M4 Non Maskable Interrupt                */
    HardFault_IRQn          = -13,      /*!< 4  Cortex-M4 Hard Fault Interrupt                  */
    MemoryManagement_IRQn   = -12,      /*!< 4  Cortex-M4 Memory Management Fault Interrupt     */
    BusFault_IRQn           = -11,      /*!< 5  Cortex-M4 Bus Fault Interrupt                   */
    UsageFault_IRQn         = -10,      /*!< 6  Cortex-M4 Usage Fault Interrupt                 */
    SVCall_IRQn             = -5,       /*!< 11 Cortex-M4 SV Call Interrupt                     */
    DebugMonitor_IRQn       = -4,       /*!< 12 Cortex-M4 Debug Monitor Interrupt               */
    PendSV_IRQn             = -2,       /*!< 14 Cortex-M4 Pend SV Interrupt                     */
    SysTick_IRQn            = -1,       /*!< 15 Cortex-M4 System Tick Interrupt                 */

    BROWNOUT_IRQn           = 0,
    WDT_IRQn                = 1,
    CLKGEN_IRQn             = 2,
    VCOMP_IRQn              = 3,
    IOSLAVE_IOS_IRQn        = 4,
    IOSLAVE_ACC_IRQn        = 5,
    IOMASTER0_IRQn          = 6,
    IOMASTER1_IRQn          = 7,
    IOMASTER2_IRQn          = 8,
    IOMASTER3_IRQn          = 9,
    IOMASTER4_IRQn          = 10,
    IOMASTER5_IRQn          = 11,
    GPIO_IRQn               = 12,
    CTIMER_IRQn             = 13,
    UART_IRQn               = 14,
    UART1_IRQn              = 15,
    ADC_IRQn                = 16,
    PDM_IRQn                = 17,
    STIMER_IRQn             = 18,
    STIMER_CMPR0_IRQn       = 19,
    STIMER_CMPR1_IRQn       = 20,
    STIMER_CMPR2_IRQn       = 21,
    STIMER_CMPR3_IRQn       = 22,
    STIMER_CMPR4_IRQn       = 23,
    STIMER_CMPR5_IRQn       = 24,
    STIMER_CMPR6_IRQn       = 25,
    STIMER_CMPR7_IRQn       = 26,
    FLASH_IRQn              = 27,
    SOFTWARE0_IRQn          = 28,
    SOFTWARE1_IRQn          = 29,
    SOFTWARE2_IRQn          = 30,
    SOFTWARE3_IRQn          = 31,
} IRQn_Type;

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __FPU_PRESENT             1         /*!< FPU present or not                               */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "..\..\..\common\common.h"         /* Cortex-M4 processor and core peripherals           */

/*============================ MACROS ========================================*/
#define APOLLO2_PACKAGE_BGA

#define UART_COUNT                      2
#define I2C_COUNT                       1
#define SPI_COUNT                       1
#define SSP_COUNT                       1
#define GPIO_COUNT                      2
#define TIMER_COUNT                     4
#define PAD_COUNT                       50

#define FLASH_PAGE_SIZE                 (512ul)

//! \note IRC and RTC frequency
//! @{
#define IRC_CLK_FREQ            (48MHz)
#define RTC_CLK_FREQ            (32768ul)
//! @}

// Peripheral register space
#define REG_ADC_BASEADDR                         (0x50010000UL)
#define REG_CACHECTRL_BASEADDR                   (0x40018000UL)
#define REG_CLKGEN_BASEADDR                      (0x40004000UL)
#define REG_CTIMER_BASEADDR                      (0x40008000UL)
#define REG_STIMER_BASEADDR                      (0x40008000UL)
#define REG_GPIO_BASEADDR                        (0x40010000UL)
#define REG_IOMSTR_BASEADDR                      (0x50004000UL)
#define REG_IOSLAVE_BASEADDR                     (0x50000000UL)
#define REG_MCUCTRL_BASEADDR                     (0x40020000UL)
#define REG_PDM_BASEADDR                         (0x50011000UL)
#define REG_PWRCTRL_BASEADDR                     (0x40021000UL)
#define REG_RSTGEN_BASEADDR                      (0x40000000UL)
#define REG_RTC_BASEADDR                         (0x40004000UL)
#define REG_UART0_BASEADDR                       (0x4001C000UL)
#define REG_UART1_BASEADDR                       (0x4001D000UL)
#define REG_VCOMP_BASEADDR                       (0x4000C000UL)
#define REG_WDT_BASEADDR                         (0x40024000UL)

// SRAM address space
#define SRAM_BASEADDR                            (0x10000000UL)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
