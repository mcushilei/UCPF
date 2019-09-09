/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_PIN_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_PIN_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\gpio\reg_gpio.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define PIN_CFG(...)                                        \
    do {                                                    \
        static const pin_cfg_t __config[] = {__VA_ARGS__};  \
        PIN.Config(__config, UBOUND(__config));             \
    } while(0)


//
// Helper macros used for unraveling the GPIO configuration value (configval).
//
// Note that the configval, which is passed into functions such as
//  am_hal_gpio_pin_config() as well as various helper macros, is a concatenated
//  value that contains values used in multiple configuration registers.
//
// The GPIO configuration value fields are arranged as follows:
// [ 7: 0] PADREG configuration.
// [11: 8] CFG
// [15:12] Unused.
// [23:16] ALTPADREG configuration.
//
// Define macros describing these configval fields.
//
#define __PIN_CFGVAL_PADREG_S             0
#define __PIN_CFGVAL_PADREG_M             (0xFFu << __PIN_CFGVAL_PADREG_S)
#define __PIN_CFGVAL_CFG_S                8
#define __PIN_CFGVAL_CFG_M                (0x0Fu << __PIN_CFGVAL_CFG_S)
#define __PIN_CFGVAL_ALTPAD_S             16
#define __PIN_CFGVAL_ALTPAD_M             (0xFFu << __PIN_CFGVAL_ALTPAD_S)


//*****************************************************************************
//
// Pad function select
// This macro represents the 3 bit function select field in the PADREG byte.
//
//*****************************************************************************
#define DRIVER_PIN_FUNC(x)        ( ( ((x) & 0x7u) << 3 ) << __PIN_CFGVAL_PADREG_S )

//*****************************************************************************
//
// Input options.
//
//*****************************************************************************
#define DRIVER_PIN_INP_DISABLE    (0x0u << __PIN_CFGVAL_PADREG_S) // Disable input transistors.
#define DRIVER_PIN_INP_ENABLE     (0x2u << __PIN_CFGVAL_PADREG_S) // Enable input transistors.
#define DRIVER_PIN_INP_RDZERO     (0x1u << __PIN_CFGVAL_CFG_S)    // Disable input read registers.

//*****************************************************************************
//
// Output options.
//
//*****************************************************************************
#define DRIVER_PIN_OUT_DISABLE    ((0x0u << 1) << __PIN_CFGVAL_CFG_S)
#define DRIVER_PIN_OUT_PUSHPULL   ((0x1u << 1) << __PIN_CFGVAL_CFG_S)
#define DRIVER_PIN_OUT_OPENDRAIN  ((0x2u << 1) << __PIN_CFGVAL_CFG_S)
#define DRIVER_PIN_OUT_3STATE     ((0x3u << 1) << __PIN_CFGVAL_CFG_S)

//*****************************************************************************
//
// Special options for IOM0 and IOM4 clocks.
// For 24MHz operation, a special enable must be selected. The 24MHZ select is
// selected via bit0 of OUTCFG (which is, in a way,an alias of OUT_PUSHPULL).
//
//*****************************************************************************
#define DRIVER_PIN_24MHZ_ENABLE    ((0x1u << 1) << __PIN_CFGVAL_CFG_S)

//*****************************************************************************
//
// Pad configuration options.
// (Configuration value bits 7:0.)
//
//*****************************************************************************
#define DRIVER_PIN_HIGH_DRIVE      (0x04u << __PIN_CFGVAL_PADREG_S)
#define DRIVER_PIN_LOW_DRIVE       (0x00u << __PIN_CFGVAL_PADREG_S)
#define DRIVER_PIN_PULLUP          (0x01u << __PIN_CFGVAL_PADREG_S)
#define DRIVER_PIN_PULL1_5K        ( (0x01u << __PIN_CFGVAL_PADREG_S) |   \
                                      DRIVER_PIN_PULLUP )
#define DRIVER_PIN_PULL6K          ( (0x40u << __PIN_CFGVAL_PADREG_S) |   \
                                      DRIVER_PIN_PULLUP )
#define DRIVER_PIN_PULL12K         ( (0x80u << __PIN_CFGVAL_PADREG_S) |   \
                                      DRIVER_PIN_PULLUP )
#define DRIVER_PIN_PULL24K         ( (0xC0u << __PIN_CFGVAL_PADREG_S) |   \
                                      DRIVER_PIN_PULLUP )

// POWER SWITCH is available on selected pins
#define DRIVER_PIN_POWER           (0x80u << __PIN_CFGVAL_PADREG_S)

//*****************************************************************************
//
//! ALTPADREG configuration options.
//! (Configuration value bits 23:16.)
//!
//! All Apollo2 GPIO pins can be configured for 2mA or 4mA.
//!     DRIVER_PIN_DRIVE_2MA  =  2mA configuration.
//!     DRIVER_PIN_DRIVE_4MA  =  4mA configuration.
//!
//! Certain Apollo2 GPIO pins can be configured to drive up to 12mA.
//!     DRIVER_PIN_DRIVE_8MA  =  8mA configuration.
//!     DRIVER_PIN_DRIVE_12MA = 12mA configuration.
//!
//! Notes:
//! - Always consult the Apollo2 data sheet for the latest details.
//! - The higher drive GPIOxx pads generally include:
//!   0-2,5,7-8,10,12-13,22-23,26-29,38-39,42,44-48.
//! - GPIOxx pads that do not support the higher drive:
//!   3-4,6,9,11,14-21,24-25,30-37,40-41,43,49.
//! - User is responsible for ensuring that the selected pin actually supports
//!   the higher drive (8mA or 12mA) capabilities. See the Apollo2 data sheet.
//! - Attempting to set the higher drive (8mA or 12mA) configuration on a
//!   non-supporting pad will actually set the pad for 4mA drive strength,
//!   regardless of the lower bit setting.
//
//*****************************************************************************
#define DRIVER_PIN_DRIVE_2MA       (0)
#define DRIVER_PIN_DRIVE_4MA       DRIVER_PIN_HIGH_DRIVE
#define DRIVER_PIN_DRIVE_8MA       (0x01 << __PIN_CFGVAL_ALTPAD_S)
#define DRIVER_PIN_DRIVE_12MA      ( (0x01 << __PIN_CFGVAL_ALTPAD_S)  |  \
                                      DRIVER_PIN_HIGH_DRIVE )

#define DRIVER_PIN_SLEWRATE        (0x10 << __PIN_CFGVAL_ALTPAD_S)

//*****************************************************************************
//
// Interrupt polarity
// These values can be used directly in the configval.
//
//*****************************************************************************
#define DRIVER_PIN_CFG_INTD_FALLING   ((1u << 2) << __PIN_CFGVAL_CFG_S)
#define DRIVER_PIN_CFG_INTD_RISING    ((0u << 2) << __PIN_CFGVAL_CFG_S)

//*****************************************************************************
//
// Pin definition macros.
//
//*****************************************************************************
#define DRIVER_PIN_0_SLSCL        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_0_SLSCK        (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_0_CLKOUT       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_0_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_0_MxSCKLB      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_0_M2SCK        (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_0_MxSCLLB      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_0_M2SCL        (DRIVER_PIN_FUNC(7) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_1_SLSDA        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_1_SLMISO       (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_1_UART0TX      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_1_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_1_MxMISOLB     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_1_M2MISO       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_1_MxSDALB      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_1_M2SDA        (DRIVER_PIN_FUNC(7) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_2_SLWIR3       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_2_SLMOSI       (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_2_UART0RX      (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_2_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_2_MxMOSILB     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_2_M2MOSI       (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_2_MxWIR3LB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_2_M2WIR3       (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_3_UART0RTS     (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_3_SLnCE        (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_3_M1nCE4       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_3_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_3_MxnCELB      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_3_M2nCE0       (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_3_TRIG1        (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_3_I2S_WCLK     (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_4_UART0CTS     (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_4_SLINT        (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_4_M0nCE5       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_4_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_4_SLINTGP      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_4_M2nCE5       (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_4_CLKOUT       (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_4_32KHZ_XT     (DRIVER_PIN_FUNC(7))
// PSINK usage: GPIOWT=0 to activate the power switch, GPIOWT=1 to disable
#define DRIVER_PIN_4_PSINK        (DRIVER_PIN_FUNC(3) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_POWER)

#define DRIVER_PIN_5_M0SCL        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_5_M0SCK        (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_5_UART0RTS     (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_5_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_5_M0SCKLB      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_5_M0SCLLB      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_5_M1nCE2       (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_6_M0SDA        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_6_M0MISO       (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_6_UART0CTS     (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_6_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_6_SLMISOLB     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_6_M1nCE0       (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_6_SLSDALB      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_6_I2S_DAT      (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_7_M0WIR3       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_7_M0MOSI       (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_7_CLKOUT       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_7_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_7_TRIG0        (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_7_UART0TX      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_7_SLWIR3LB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_7_M1nCE1       (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_8_M1SCL        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_8_M1SCK        (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_8_M0nCE4       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_8_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_8_M2nCE4       (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_8_M1SCKLB      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_8_UART1TX      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_8_M1SCLLB      (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_9_M1SDA        (DRIVER_PIN_FUNC(0) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_9_M1MISO       (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_9_M0nCE5       (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_9_GPIO         (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_9_M4nCE5       (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_9_SLMISOLB     (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_9_UART1RX      (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_9_SLSDALB      (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_10_M1WIR3      (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_10_M1MOSI      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_10_M0nCE6      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_10_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_10_M2nCE6      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_10_UART1RTS    (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_10_M4nCE4      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_10_SLWIR3LB    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_11_ADCSE2      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_11_M0nCE0      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_11_CLKOUT      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_11_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_11_M2nCE7      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_11_UART1CTS    (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_11_UART0RX     (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_11_PDM_DATA    (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_12_ADCD0NSE9   (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_12_M1nCE0      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_12_TCTA0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_12_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_12_CLKOUT      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_12_PDM_CLK     (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_12_UART0CTS    (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_12_UART1TX     (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_13_ADCD0PSE8   (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_13_M1nCE1      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_13_TCTB0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_13_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_13_M2nCE3      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_13_UART0RTS    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_13_UART1RX     (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_14_ADCD1P      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_14_M1nCE2      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_14_UART1TX     (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_14_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_14_M2nCE1      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_14_SWDCK       (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_14_32KHZ_XT    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_15_ADCD1N      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_15_M1nCE3      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_15_UART1RX     (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_15_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_15_M2nCE2      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_15_SWDIO       (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_15_SWO         (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_16_ADCSE0      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_16_M0nCE4      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_16_TRIG0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_16_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_16_M2nCE3      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_16_CMPIN0      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_16_UART0TX     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_16_UART1RTS    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_17_CMPRF1      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_17_M0nCE1      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_17_TRIG1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_17_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_17_M4nCE3      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_17_UART0RX     (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_17_UART1CTS    (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_18_CMPIN1      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_18_M0nCE2      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_18_TCTA1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_18_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_18_M4nCE1      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_18_UART1TX     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_18_32KHZ_XT    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_19_CMPRF0      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_19_M0nCE3      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_19_TCTB1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_19_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_19_TCTA1       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_19_UART1RX     (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_19_I2S_BCLK    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_20_SWDCK       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_20_M1nCE5      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_20_TCTA2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_20_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_20_UART0TX     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_20_UART1TX     (DRIVER_PIN_FUNC(5))

#define DRIVER_PIN_21_SWDIO       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_21_M1nCE6      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_21_TCTB2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_21_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_21_UART0RX     (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_21_UART1RX     (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)

#define DRIVER_PIN_22_UART0TX     (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_22_M1nCE7      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_22_TCTA3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_22_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_22_PDM_CLK     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_22_TCTB1       (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_22_SWO         (DRIVER_PIN_FUNC(7))
// PSOURCE usage in pushpull: GPIOWT=1 to activate the power switch, GPIOWT=0 to disable
#define DRIVER_PIN_22_PSOURCE     (DRIVER_PIN_FUNC(3) | DRIVER_PIN_OUT_PUSHPULL | DRIVER_PIN_POWER)

#define DRIVER_PIN_23_UART0RX     (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_23_M0nCE0      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_23_TCTB3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_23_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_23_PDM_DATA    (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_23_CMPOUT      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_23_TCTB1       (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_24_M2nCE1      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_24_M0nCE1      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_24_CLKOUT      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_24_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_24_M5nCE0      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_24_TCTA1       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_24_I2S_BCLK    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_24_SWO         (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_25_M0nCE2      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_25_TCTA0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_25_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_25_M2SDA       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_25_M2MISO      (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_25_SLMISOLB    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_25_SLSDALB     (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#define DRIVER_PIN_26_M0nCE3      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_26_TCTB0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_26_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_26_M2nCE0      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_26_TCTA1       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_26_M5nCE1      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_26_M3nCE0      (DRIVER_PIN_FUNC(7))

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_27_M1nCE4      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_27_TCTA1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_27_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_27_M2SCL       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_27_M2SCK       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_27_M2SCKLB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_27_M2SCLLB     (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#define DRIVER_PIN_28_I2S_WCLK    (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_28_M1nCE5      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_28_TCTB1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_28_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_28_M2WIR3      (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_28_M2MOSI      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_28_M5nCE3      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_28_SLWIR3LB    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_29_ADCSE1      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_29_M1nCE6      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_29_TCTA2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_29_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_29_UART0CTS    (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_29_UART1CTS    (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_29_M4nCE0      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_29_PDM_DATA    (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_30_M1nCE7      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_30_TCTB2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_30_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_30_UART0TX     (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_30_UART1RTS    (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_30_I2S_DAT     (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_31_ADCSE3      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_31_M0nCE4      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_31_TCTA3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_31_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_31_UART0RX     (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_31_TCTB1       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_32_ADCSE4      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_32_M0nCE5      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_32_TCTB3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_32_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_32_TCTB1       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_33_ADCSE5      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_33_M0nCE6      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_33_32KHZ_XT    (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_33_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_33_M3nCE7      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_33_TCTB1       (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_33_SWO         (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_34_ADCSE6      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_34_M0nCE7      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_34_M2nCE3      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_34_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_34_CMPRF2      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_34_M3nCE1      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_34_M4nCE0      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_34_M5nCE2      (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_35_ADCSE7      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_35_M1nCE0      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_35_UART1TX     (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_35_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_35_M4nCE6      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_35_TCTA1       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_35_UART0RTS    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_35_M3nCE2      (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_36_TRIG1       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_36_M1nCE1      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_36_UART1RX     (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_36_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_36_32KHZ_XT    (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_36_M2nCE0      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_36_UART0CTS    (DRIVER_PIN_FUNC(6) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_36_M3nCE3      (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_37_TRIG2       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_37_M1nCE2      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_37_UART0RTS    (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_37_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_37_M3nCE4      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_37_M4nCE1      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_37_PDM_CLK     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_37_TCTA1       (DRIVER_PIN_FUNC(7) | DRIVER_PIN_INP_ENABLE)
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_38_TRIG3       (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_38_M1nCE3      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_38_UART0CTS    (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_38_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_38_M3WIR3      (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_38_M3MOSI      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_38_M4nCE7      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_38_SLWIR3LB    (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#define DRIVER_PIN_39_UART0TX     (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_39_UART1TX     (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_39_CLKOUT      (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_39_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_39_M4SCL       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_39_M4SCK       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_39_M4SCKLB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_39_M4SCLLB     (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_40_UART0RX     (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_40_UART1RX     (DRIVER_PIN_FUNC(1) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_40_TRIG0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_40_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_40_M4SDA       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_40_M4MISO      (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_40_SLMISOLB    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_40_SLSDALB     (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_41_M2nCE1      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_41_CLKOUT      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_41_SWO         (DRIVER_PIN_FUNC(2))
#define DRIVER_PIN_41_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_41_M3nCE5      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_41_M5nCE7      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_41_M4nCE2      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_41_UART0RTS    (DRIVER_PIN_FUNC(7))
// PSOURCE usage in pushpull: GPIOWT=1 to activate the power switch, GPIOWT=0 to disable
#define DRIVER_PIN_41_PSOURCE     (DRIVER_PIN_FUNC(3) | DRIVER_PIN_OUT_PUSHPULL | DRIVER_PIN_POWER)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_42_M2nCE2      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_42_M0nCE0      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_42_TCTA0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_42_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_42_M3SCL       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_42_M3SCK       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_42_M3SCKLB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_42_M3SCLLB     (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_43_M2nCE4      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_43_M0nCE1      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_43_TCTB0       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_43_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_43_M3SDA       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_43_M3MISO      (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_43_SLMISOLB    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_43_SLSDALB     (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#define DRIVER_PIN_44_UART1RTS    (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_44_M0nCE2      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_44_TCTA1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_44_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_44_M4WIR3      (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_44_M4MOSI      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_44_M5nCE6      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_44_SLWIR3LB    (DRIVER_PIN_FUNC(7))

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_45_UART1CTS    (DRIVER_PIN_FUNC(0) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_45_M0nCE3      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_45_TCTB1       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_45_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_45_M4nCE3      (DRIVER_PIN_FUNC(4))
#define DRIVER_PIN_45_M3nCE6      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_45_M5nCE5      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_45_SWO         (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#if defined (APOLLO2_PACKAGE_BGA)
#define DRIVER_PIN_46_32KHZ_XT    (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_46_M0nCE4      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_46_TCTA2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_46_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_46_TCTA1       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_46_M5nCE4      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_46_M4nCE4      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_46_SWO         (DRIVER_PIN_FUNC(7))
#endif // defined (APOLLO2_PACKAGE_BGA)

#define DRIVER_PIN_47_M2nCE5      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_47_M0nCE5      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_47_TCTB2       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_47_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_47_M5WIR3      (DRIVER_PIN_FUNC(4) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_47_M5MOSI      (DRIVER_PIN_FUNC(5))
#define DRIVER_PIN_47_M4nCE5      (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_47_SLWIR3LB    (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_48_M2nCE6      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_48_M0nCE6      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_48_TCTA3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_48_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_48_M5SCL       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_48_M5SCK       (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_48_M5SCKLB     (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_48_M5SCLLB     (DRIVER_PIN_FUNC(7))

#define DRIVER_PIN_49_M2nCE7      (DRIVER_PIN_FUNC(0))
#define DRIVER_PIN_49_M0nCE7      (DRIVER_PIN_FUNC(1))
#define DRIVER_PIN_49_TCTB3       (DRIVER_PIN_FUNC(2) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_49_GPIO        (DRIVER_PIN_FUNC(3))
#define DRIVER_PIN_49_M5SDA       (DRIVER_PIN_FUNC(4) | DRIVER_PIN_OUT_OPENDRAIN | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_49_M5MISO      (DRIVER_PIN_FUNC(5) | DRIVER_PIN_INP_ENABLE)
#define DRIVER_PIN_49_SLMISOLB    (DRIVER_PIN_FUNC(6))
#define DRIVER_PIN_49_SLSDALB     (DRIVER_PIN_FUNC(7))

/*============================ TYPES =========================================*/
//! \name io configuration structure
//! @{
typedef struct {
    uint8_t         Port;       //!< port number
    uint8_t         Pin;        //!< pin number
    uint32_t        Config;     //!< pin mode
} pin_cfg_t;
//! @}

//! \name csc user interface
//! @{
DEF_INTERFACE(i_pin_t)
    bool            (*Config)(pin_cfg_t const *pConfig, uint32_t size); //!< io configuration
END_DEF_INTERFACE(i_pin_t)
//! @}


/*============================ PUBLIC VARIABLES ==============================*/
extern const i_pin_t PIN;

/*============================ PUBLIC PROTOTYPES =============================*/


#endif  // #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_PIN_H__
