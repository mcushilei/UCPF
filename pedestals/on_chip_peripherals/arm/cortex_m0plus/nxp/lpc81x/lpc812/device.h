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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC812_DEVICE_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC812_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

typedef enum IRQn {
    Reset_IRQn                    = -15,    /*!< 1 Reset Vector, invoked on Power up and warm reset*/  
    NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
    HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
    SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
    DebugMonitor_IRQn             = -4,     /*!< 12  Debug Monitor                                 */
    PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
    SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

    SPI0_IRQn                     = 0,        /*!< SPI0                                             */
    SPI1_IRQn                     = 1,        /*!< SPI1                                             */

    UART0_IRQn                    = 3,        /*!< USART0                                           */
    UART1_IRQn                    = 4,        /*!< USART1                                           */
    UART2_IRQn                    = 5,        /*!< USART2                                           */

    I2C1_IRQn                     = 7,        /*!< I2C1                                             */
    I2C0_IRQn                     = 8,        /*!< I2C0                                             */
    SCT_IRQn                      = 9,        /*!< SCT                                              */
    MRT_IRQn                      = 10,       /*!< MRT                                              */ 
    CMP_IRQn                      = 11,       /*!< CMP                                              */
    WDT_IRQn                      = 12,       /*!< WDT                                              */
    BOD_IRQn                      = 13,       /*!< BOD                                              */
    FLASH_IRQn                    = 14,       /*!< FLASH                                            */
    WKT_IRQn                      = 15,       /*!< WKT Interrupt                                    */
    ADC_SEQA_IRQn                 = 16,       /*!< ADC Seq. A                                       */    
    ADC_SEQB_IRQn                 = 17,       /*!< ADC Seq. B                                       */
    ADC_THCMP_IRQn                = 18,       /*!< ADC Thresh Comp                                  */    
    ADC_OVR_IRQn                  = 19,       /*!< ADC overrun                                      */
    DMA_IRQn                      = 20,       /*!< DMA                                              */    
    I2C2_IRQn                     = 21,       /*!< I2C2                                             */
    I2C3_IRQn                     = 22,       /*!< I2C3                                             */    

    PININT0_IRQn                  = 24,       /*!< External Interrupt 0                             */
    PININT1_IRQn                  = 25,       /*!< External Interrupt 1                             */  
    PININT2_IRQn                  = 26,       /*!< External Interrupt 2                             */
    PININT3_IRQn                  = 27,       /*!< External Interrupt 3                             */
    PININT4_IRQn                  = 28,       /*!< External Interrupt 4                             */
    PININT5_IRQn                  = 29,       /*!< External Interrupt 5                             */
    PININT6_IRQn                  = 30,       /*!< External Interrupt 6                             */
    PININT7_IRQn                  = 31,       /*!< External Interrupt 7                             */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0PLUS_REV               0x0001
#define __MPU_PRESENT               0       /*!< MPU present or not                               */
#define __VTOR_PRESENT              1       /*!< Set to 1 if vector table is used     */
#define __NVIC_PRIO_BITS            2       /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig      0       /*!< Set to 1 if different SysTick Config is used     */

#include "..\..\..\common\common.h"

/*============================ MACROS ========================================*/
#define IO_PORT_COUNT               1
#define UART_COUNT                  2

#define WDT_BASE_ADDRESS                (0x40000000ul)
#define MRT_BASE_ADDRESS                (0x40004000ul)
#define SWM_BASE_ADDRESS                (0x4000C000ul)
#define PMU_BASE_ADDRESS                (0x40020000ul)
#define FLASHCRTL_BASE_ADDRESS          (0x40040000ul)
#define IOCON_BASE_ADDRESS              (0x40044000ul)
#define SYSCON_BASE_ADDRESS             (0x40048000ul)
#define USART0_BASE_ADDRESS             (0x40064000ul)
#define USART1_BASE_ADDRESS             (0x40068000ul)
#define USART2_BASE_ADDRESS             (0x4006C000ul)
#define GPIO_BASE_ADDRESS               (0xA0000000ul)
#define PININT_BASE_ADDRESS             (0xA0004000ul)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
