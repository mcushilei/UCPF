/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_PIN_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_PIN_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\pm.h"
#include ".\reg_iocon.h"
#include ".\reg_swm.h"
#include ".\reg_pinint.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __PIN_PORT_NAME(__N, __OFFSET)        PORT##__N = (__N),
#define __PIN_PIN_NAME(__N, __OFFSET)         PIN##__N = (__N),
#define __PIN_PIN_MASK(__N, __OFFSET)         PIN##__N##_MSK = (1u << (__N)),

#define IO_CFG(...)     \
    do {                                                    \
        static const io_cfg_t Config[] = {__VA_ARGS__};     \
        IO.Config(Config, UBOUND(Config));                  \
    } while(0)


/*============================ TYPES =========================================*/
enum {
    MREPEAT(IO_PORT_COUNT, __PIN_PORT_NAME, 0)
};

enum {
    MREPEAT(32, __PIN_PIN_NAME, 0)
};

enum {
    MREPEAT(32, __PIN_PIN_MASK, 0)
};

#if defined(__LPC81X__)
enum {
    PIN_FN_U0_TXD       = 0,
    PIN_FN_U0_RXD,
    PIN_FN_U0_RTS,
    PIN_FN_U0_CTS,
    PIN_FN_U0_SCLK,
    PIN_FN_U1_TXD,
    PIN_FN_U1_RXD,
    PIN_FN_U1_RTS,
    PIN_FN_U1_CTS,
    PIN_FN_U1_SCLK,
    PIN_FN_U2_TXD,
    PIN_FN_U2_RXD,
    PIN_FN_U2_RTS,
    PIN_FN_U2_CTS,
    PIN_FN_U2_SCLK,
    PIN_FN_SPI0_SCK,
    PIN_FN_SPI0_MOSI,
    PIN_FN_SPI0_MISO,
    PIN_FN_SPI0_SSEL,
    PIN_FN_SPI1_SCK,
    PIN_FN_SPI1_MOSI,
    PIN_FN_SPI1_MISO,
    PIN_FN_SPI1_SSEL,
    PIN_FN_CTIN_0,
    PIN_FN_CTIN_1,
    PIN_FN_CTIN_2,
    PIN_FN_CTIN_3,
    PIN_FN_CTOUT_0,
    PIN_FN_CTOUT_1,
    PIN_FN_CTOUT_2,
    PIN_FN_CTOUT_3,
    PIN_FN_I2C_SDA,
    PIN_FN_I2C_SCL,
    PIN_FN_ACMP_0,
    PIN_FN_CKLOUT,
    PIN_FN_GPIO_INT_BMAT,
    
    PIN_FN_ACMP_I1,
    PIN_FN_ACMP_I2,
    PIN_FN_SWCLK,
    PIN_FN_SWDIO,
    PIN_FN_XTALIN,
    PIN_FN_XTALOUT,
    PIN_FN_RESET,
    PIN_FN_CKLIN,
    PIN_FN_VDDCMP,
};
#elif defined(__LPC82X__)
enum {
    PIN_FN_U0_TXD       = 0,
    PIN_FN_U0_RXD,
    PIN_FN_U0_RTS,
    PIN_FN_U0_CTS,
    
    PIN_FN_U0_SCLK,
    PIN_FN_U1_TXD,
    PIN_FN_U1_RXD,
    PIN_FN_U1_RTS,
    
    PIN_FN_U1_CTS,
    PIN_FN_U1_SCLK,
    PIN_FN_U2_TXD,
    PIN_FN_U2_RXD,
    
    PIN_FN_U2_RTS,
    PIN_FN_U2_CTS,
    PIN_FN_U2_SCLK,
    PIN_FN_SPI0_SCK,
    
    PIN_FN_SPI0_MOSI,
    PIN_FN_SPI0_MISO,
    PIN_FN_SPI0_SSEL0,
    PIN_FN_SPI0_SSEL1,
    
    PIN_FN_SPI0_SSEL2,
    PIN_FN_SPI0_SSEL3,
    PIN_FN_SPI1_SCK,
    PIN_FN_SPI1_MOSI,
    
    PIN_FN_SPI1_MISO,
    PIN_FN_SPI1_SSEL0,
    PIN_FN_SPI1_SSEL1,
    PIN_FN_CTIN_0,
    
    PIN_FN_CTIN_1,
    PIN_FN_CTIN_2,
    PIN_FN_CTIN_3,
    PIN_FN_CTOUT_0,
    
    PIN_FN_CTOUT_1,
    PIN_FN_CTOUT_2,
    PIN_FN_CTOUT_3,
    PIN_FN_CTOUT_4,
    
    PIN_FN_CTOUT_5,
    PIN_FN_I2C1_SDA,
    PIN_FN_I2C1_SCL,
    PIN_FN_I2C2_SDA,
    
    PIN_FN_I2C2_SCL,
    PIN_FN_I2C3_SDA,
    PIN_FN_I2C3_SCL,
    PIN_FN_ADC_PINTRIG0,
    
    PIN_FN_ADC_PINTRIG1,
    PIN_FN_ACMP_0,
    PIN_FN_CKLOUT,
    PIN_FN_GPIO_INT_BMAT,
    
    PIN_FN_ACMP_I1,
    PIN_FN_ACMP_I2,
    PIN_FN_ACMP_I3,
    PIN_FN_ACMP_I4,
    PIN_FN_SWCLK,
    PIN_FN_SWDIO,
    PIN_FN_XTALIN,
    PIN_FN_XTALOUT,
    PIN_FN_RESET,
    PIN_FN_CKLIN,
    PIN_FN_VDDCMP,
    PIN_FN_I2C0_SDA,
    PIN_FN_I2C0_SCL,
    PIN_FN_ADC_0,
    PIN_FN_ADC_1,
    PIN_FN_ADC_2,
    PIN_FN_ADC_3,
    PIN_FN_ADC_4,
    PIN_FN_ADC_5,
    PIN_FN_ADC_6,
    PIN_FN_ADC_7,
    PIN_FN_ADC_8,
    PIN_FN_ADC_9,
    PIN_FN_ADC_10,
    PIN_FN_ADC_11,
};
#endif

/**
 * @brief LPC8XX Switch Matrix Fixed pins
 */
#if defined(__LPC82X__)
enum {
	SWM_FIXED_ACMP_I1 = 0,	/*!< ACMP I1 */
	SWM_FIXED_ACMP_I2 = 1,	/*!< ACMP I2 */
	SWM_FIXED_ACMP_I3 = 2,	/*!< ACMP I3 */
	SWM_FIXED_ACMP_I4 = 3,	/*!< ACMP I4 */
	SWM_FIXED_SWCLK   = 4,	/*!< SWCLK */
	SWM_FIXED_SWDIO   = 5,	/*!< SWDIO */
	SWM_FIXED_XTALIN  = 6,	/*!< XTALIN */
	SWM_FIXED_XTALOUT = 7,	/*!< XTALOUT */
	SWM_FIXED_RST     = 8,	/*!< Reset */
	SWM_FIXED_CLKIN   = 9,	/*!< Clock Input */
	SWM_FIXED_VDDCMP  = 10,	/*!< VDD */
	SWM_FIXED_I2C0_SDA  = 11,	/*!< I2C0 SDA */
	SWM_FIXED_I2C0_SCL  = 12,	/*!< I2C0 SCL */
	SWM_FIXED_ADC0    = 13,	/*!< ADC0 */
	SWM_FIXED_ADC1    = 14,	/*!< ADC1 */
	SWM_FIXED_ADC2    = 15,	/*!< ADC2 */
	SWM_FIXED_ADC3    = 16,	/*!< ADC3 */
	SWM_FIXED_ADC4    = 17,	/*!< ADC4 */
	SWM_FIXED_ADC5    = 18,	/*!< ADC5 */
	SWM_FIXED_ADC6    = 19,	/*!< ADC6 */
	SWM_FIXED_ADC7    = 20,	/*!< ADC7 */
	SWM_FIXED_ADC8    = 21,	/*!< ADC8 */
	SWM_FIXED_ADC9    = 22,	/*!< ADC9 */
	SWM_FIXED_ADC10   = 23,	/*!< ADC10 */
	SWM_FIXED_ADC11   = 24,	/*!< ADC11 */
};
#else
enum {
	SWM_FIXED_ACMP_I1 = 0,	/*!< ACMP I1 */
	SWM_FIXED_ACMP_I2 = 1,	/*!< ACMP I2 */
	SWM_FIXED_SWCLK   = 2,	/*!< SWCLK */
	SWM_FIXED_SWDIO   = 3,	/*!< SWDIO */
	SWM_FIXED_XTALIN  = 4,	/*!< XTALIN */
	SWM_FIXED_XTALOUT = 5,	/*!< XTALOUT */
	SWM_FIXED_RST     = 6,	/*!< Reset */
	SWM_FIXED_CLKIN   = 7,	/*!< Clock Input */
	SWM_FIXED_VDDCMP  = 8	/*!< VDD */
};
#endif

/**
 * @brief	Enables a Fixed Function Pin in the Switch Matrix
 * @param	pin		: Pin to be enabled
 * @return	Nothing
 */
static INLINE void Chip_SWM_EnableFixedPin(uint32_t pin)
{
    clock_enable(CLOCK_SWM); 
	SWM_REG.PINENABLE0 &= ~(1u << pin);
    clock_disable(CLOCK_SWM); 
}

/**
 * @brief	Disables a Fixed Function Pin in the Switch Matrix
 * @param	pin		: Pin to be disabled
 * @return	Nothing
 */
static INLINE void Chip_SWM_DisableFixedPin(uint32_t pin)
{
    clock_enable(CLOCK_SWM); 
	SWM_REG.PINENABLE0 |= 1u << pin;
    clock_disable(CLOCK_SWM); 
}

//! argument values for pin mode. values in different group can be OR-ed together.
enum {
    PIN_MODE_INACTIVE       = (0u << 3),
    PIN_MODE_PULL_DOWN      = (1u << 3),
    PIN_MODE_PULL_UP        = (2u << 3),
    PIN_MODE_REPEAT         = (3u << 3),
    
    PIN_HYSTERESIS          = (1u << 5),
    PIN_INPUT_INVERT        = (1u << 6),
    PIN_OPEN_DRAIN          = (1u << 10),

    PIN_FILTER_BYPASS       = (0u << 11),           //!< filter is bypassed
    PIN_FILTER_1CLK         = (1u << 11),           //!< levels should keep 1 clock
    PIN_FILTER_2CLK         = (2u << 11),           //!< levels should keep 2 clock
    PIN_FILTER_3CLK         = (3u << 11),           //!< levels should keep 3 clock
    
    PIN_FILTER_CLK_DIV0     = (0u << 13),           //!< select clock divider 0 for filter
    PIN_FILTER_CLK_DIV1     = (1u << 13),           //!< select clock divider 1 for filter
    PIN_FILTER_CLK_DIV2     = (2u << 13),           //!< select clock divider 2 for filter
    PIN_FILTER_CLK_DIV3     = (3u << 13),           //!< select clock divider 3 for filter
    PIN_FILTER_CLK_DIV4     = (4u << 13),           //!< select clock divider 4 for filter
    PIN_FILTER_CLK_DIV5     = (5u << 13),           //!< select clock divider 5 for filter
    PIN_FILTER_CLK_DIV6     = (6u << 13),           //!< select clock divider 6 for filter
};

//! \name io configuration structure
//! @{
typedef struct {
    uint8_t         Port;
    uint8_t         PIN;           //!< pin number
    uint8_t         Function;      //!< io Funcitons
    uint32_t        Mode;          //!< io mode
} io_cfg_t;
//! @}

//! \name csc user interface
//! @{
DEF_INTERFACE(io_t)
    bool            (*Config)(io_cfg_t const *pConfig, uint32_t size);//!< io configuration
END_DEF_INTERFACE(io_t)
//! @}



/*============================ PUBLIC VARIABLES ==============================*/
extern const io_t IO;

/*============================ PUBLIC PROTOTYPES =============================*/

#endif  // #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_PIN_H__
