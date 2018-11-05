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

#ifndef __REG_SYSCON_H__
#define __REG_SYSCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SYSCON_REG              (*(reg_syscon_t *)SYSCON_BASE_ADDRESS)
#define FLASH_REG               (*(flash_reg_t *)FLASH_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SCON_SYSPLLCLKSEL_BIAS          (0u)
#define SCON_SYSPLLCLKSEL_MASK          BIT(0)

#define SCON_SYSPLLCTRL_MSEL_BIAS       (0u)
#define SCON_SYSPLLCTRL_MSEL_MASK       BITMASK(4, 0)
#define SCON_SYSPLLCTRL_PSEL_BIAS       (5u)
#define SCON_SYSPLLCTRL_PSEL_MASK       BITMASK(6, 5)

#define SCON_SYSPLLSTAT_LOCK_BIAS       (0u)
#define SCON_SYSPLLSTAT_LOCK_MASK       BIT(0)

#define SCON_MAINCLKSEL_SEL_BIAS        (0u)
#define SCON_MAINCLKSEL_SEL_MASK        BITMASK(2, 0)

#define SCON_SYSOSC_BYPASS_BIAS         (0u)
#define SCON_SYSOSC_BYPASS_MASK         BIT(0)
#define SCON_SYSOSC_FREQRANGE_BIAS      (1u)
#define SCON_SYSOSC_FREQRANGE_MASK      BIT(1)

#define SCON_RTCOSCCTRL_RTCOSCEN_BIAS   0
#define SCON_RTCOSCCTRL_RTCOSCEN_MASK   BIT(0)

#define SCON_FLASHCFG_TIM_BIAS          0
#define SCON_FLASHCFG_TIM_MASK          BITMASK(1, 0)

#define SCON_WDTOSCCRTL_DIV_BIAS        0
#define SCON_WDTOSCCRTL_DIV_MASK        BITMASK(4, 0)
#define SCON_WDTOSCCRTL_FREQSEL_BIAS    5
#define SCON_WDTOSCCRTL_FREQSEL_MASK    BITMASK(8, 5)

#define SCON_USART0CLKDIV_BIAS          0
#define SCON_USART0CLKDIV_MASK          BITMASK(7, 0)

#define SCON_PRESETCTRL_SSP0_BIAS       0
#define SCON_PRESETCTRL_SSP0_MASK       BIT(0)
#define SCON_PRESETCTRL_I2C0_BIAS       1
#define SCON_PRESETCTRL_I2C0_MASK       BIT(1)
#define SCON_PRESETCTRL_SSP1_BIAS       2
#define SCON_PRESETCTRL_SSP1_MASK       BIT(2)
#define SCON_PRESETCTRL_I2C1_BIAS       3
#define SCON_PRESETCTRL_I2C1_MASK       BIT(3)
#define SCON_PRESETCTRL_FRG_BIAS        4
#define SCON_PRESETCTRL_FRG_MASK        BIT(4)
#define SCON_PRESETCTRL_USART1_BIAS     5
#define SCON_PRESETCTRL_USART1_MASK     BIT(5)
#define SCON_PRESETCTRL_USART2_BIAS     6
#define SCON_PRESETCTRL_USART2_MASK     BIT(6)
#define SCON_PRESETCTRL_USART3_BIAS     7
#define SCON_PRESETCTRL_USART3_MASK     BIT(7)
#define SCON_PRESETCTRL_USART4_BIAS     8
#define SCON_PRESETCTRL_USART4_MASK     BIT(8)
#define SCON_PRESETCTRL_SCT0_BIAS       9
#define SCON_PRESETCTRL_SCT0_MASK       BIT(9)
#define SCON_PRESETCTRL_SCT1_BIAS       10
#define SCON_PRESETCTRL_SCT1_MASK       BIT(10)

#define SCON_SSP0CLKDIV_DIV_BIAS        0
#define SCON_SSP0CLKDIV_DIV_MASK        BIT(0)
#define SCON_SSP1CLKDIV_DIV_BIAS        0
#define SCON_SSP1CLKDIV_DIV_MASK        BIT(0)

#define SCON_STARTERP1_RTCINT_BIAS      12
#define SCON_STARTERP1_RTCINT_MASK      BIT(SCON_STARTERP1_RTCINT_BIAS)
#define SCON_STARTERP1_WWDT_BODINT_BIAS 13
#define SCON_STARTERP1_WWDT_BODINT_MASK BIT(SCON_STARTERP1_WWDT_BODINT_BIAS)
#define SCON_STARTERP1_USB_WAKEUP_BIAS  19
#define SCON_STARTERP1_USB_WAKEUP_MASK  BIT(SCON_STARTERP1_USB_WAKEUP_BIAS)
#define SCON_STARTERP1_GROUP0INT_BIAS   20
#define SCON_STARTERP1_GROUP0INT_MASK   BIT(SCON_STARTERP1_GROUP0INT_BIAS)
#define SCON_STARTERP1_GROUP1INT_BIAS   21
#define SCON_STARTERP1_GROUP1INT_MASK   BIT(SCON_STARTERP1_GROUP1INT_BIAS)
#define SCON_STARTERP1_USART1_4_BIAS    23
#define SCON_STARTERP1_USART1_4_MASK    BIT(SCON_STARTERP1_USART1_4_BIAS)
#define SCON_STARTERP1_USART2_3_BIAS    24
#define SCON_STARTERP1_USART2_3_MASK    BIT(SCON_STARTERP1_USART2_3_BIAS)

/*============================ TYPES =========================================*/
//! \name clock source
//! @{
typedef enum {
    SYSPLL_CKLSRC_IRC       = 0,
    SYSPLL_CLKSRC_SYSOSC    = 1,
    SYSPLL_CLKSRC_RTC       = 3,
} em_pll_clksrc_t;

typedef enum {
    MAIN_CLKSRC_IRC         = 0,
    MAIN_CLKSRC_PLLIN       = 1,
    MAIN_CLKSRC_WDTOSC      = 2,
    MAIN_CLKSRC_PLLOUT      = 3,
} em_main_clk_src_t;

typedef enum {
    CLKOUT_CLKSRC_IRC       = 0,
    CLKOUT_CLKSRC_SYSOSC    = 1,
    CLKOUT_CLKSRC_WDOSC     = 2,
    CLKOUT_CLKSRC_MCLK      = 3,
} em_out_clk_src_t;
//! @}

typedef enum {
    POWER_IRCOUT    = BIT(0),
    POWER_IRC       = BIT(1),
    POWER_FLASH     = BIT(2),
    POWER_BOD       = BIT(3),
    POWER_ADC       = BIT(4),
    POWER_SYSOSC    = BIT(5),
    POWER_WDTOSC    = BIT(6),
    POWER_SYSPLL    = BIT(7),
    POWER_USBPLL    = BIT(8),
    POWER_USBPAD    = BIT(10),
    POWER_TEMPSENSE = BIT(13),
} em_power_t;

//! \name Peripheral AHB Clock Macros
//! @{
typedef enum {
    AHBCLK_SYS              = 0,
    AHBCLK_ROM              = 1,
    AHBCLK_RAM0             = 2,
    AHBCLK_FLASHREG         = 3,
    AHBCLK_FLASH            = 4,
    AHBCLK_I2C0             = 5,
    AHBCLK_GPIO             = 6,
    AHBCLK_CT16B0           = 7,
    AHBCLK_CT16B1           = 8,
    AHBCLK_CT32B0           = 9,
    AHBCLK_CT32B1           = 10,
    AHBCLK_SSP0             = 11,
    AHBCLK_USART0           = 12,
    AHBCLK_ADC              = 13,
    AHBCLK_USB              = 14,
    AHBCLK_WWDT             = 15,
    AHBCLK_IOCON            = 16,
    AHBCLK_SSP1             = 18,
    AHBCLK_PINT             = 19,
    AHBCLK_USART1           = 20,
    AHBCLK_USART2           = 21,
    AHBCLK_USART3           = 22,       // USART3 USART4 has the identical bit.
    AHBCLK_USART4           = 22,       // USART3 USART4 has the identical bit.
    AHBCLK_GROUP0INT        = 23,
    AHBCLK_GROUP1INT        = 24,
    AHBCLK_I2C1             = 25,
    AHBCLK_RAM1             = 26,
    AHBCLK_USBRAM           = 27,
    AHBCLK_CRC              = 28,
    AHBCLK_DMA              = 29,
    AHBCLK_RTC              = 30,
    AHBCLK_SCT0_1           = 31,
} em_ahbclk_t;
//! @}

//! \name sys control register page.
//! @{
typedef struct {
	reg32_t         SYSMEMREMAP;		/*!< System Memory remap register */
	reg32_t         PRESETCTRL;		/*!< Peripheral reset Control register */
	reg32_t         SYSPLLCTRL;		/*!< System PLL control register */
	const reg32_t   SYSPLLSTAT;		/*!< System PLL status register */
	reg32_t         USBPLLCTRL;		/*!< USB PLL control register */
	const reg32_t   USBPLLSTAT;		/*!< USB PLL status register */
	REG32_RSVD(1)
	reg32_t         RTCOSCCTRL;		/*!< RTC Oscillator control register */
	reg32_t         SYSOSCCTRL;		/*!< System Oscillator control register */
	reg32_t         WDTOSCCTRL;		/*!< Watchdog Oscillator control register */
	REG32_RSVD(2)
	reg32_t         SYSRSTSTAT;		/*!< System Reset Status register */
	REG32_RSVD(3)
	reg32_t         SYSPLLCLKSEL;	/*!< System PLL clock source select register */
	reg32_t         SYSPLLCLKUEN;	/*!< System PLL clock source update enable register*/
	reg32_t         USBPLLCLKSEL;	/*!< USB PLL clock source select register */
	reg32_t         USBPLLCLKUEN;	/*!< USB PLL clock source update enable register */
	REG32_RSVD(8)
	reg32_t         MAINCLKSEL;		/*!< Main clock source select register */
	reg32_t         MAINCLKUEN;		/*!< Main clock source update enable register */
	reg32_t         SYSAHBCLKDIV;	/*!< System Clock divider register */
	REG32_RSVD(1)
	reg32_t         SYSAHBCLKCTRL;	/*!< System clock control register */
	REG32_RSVD(4)
	reg32_t         SSP0CLKDIV;		/*!< SSP0 clock divider register */
	reg32_t         USART0CLKDIV;	/*!< UART clock divider register */
	reg32_t         SSP1CLKDIV;		/*!< SSP1 clock divider register */
	reg32_t         FRGCLKDIV;		/*!< FRG clock divider (USARTS 1 - 4) register */
	REG32_RSVD(7)
	reg32_t         USBCLKSEL;		/*!< USB clock source select register */
	reg32_t         USBCLKUEN;		/*!< USB clock source update enable register */
	reg32_t         USBCLKDIV;		/*!< USB clock source divider register */
	REG32_RSVD(5)
	reg32_t         CLKOUTSEL;		/*!< Clock out source select register */
	reg32_t         CLKOUTUEN;		/*!< Clock out source update enable register */
	reg32_t         CLKOUTDIV;		/*!< Clock out divider register */
	REG32_RSVD(1)
	reg32_t         UARTFRGDIV;		/*!< USART fractional generator divider (USARTS 1 - 4) register */
	reg32_t         UARTFRGMULT;		/*!< USART fractional generator multiplier (USARTS 1 - 4) register */
	REG32_RSVD(1)
	reg32_t         EXTTRACECMD;		/*!< External trace buffer command register */
	const reg32_t   PIOPORCAP[3];	/*!< POR captured PIO status registers */
	REG32_RSVD(10)
	reg32_t         IOCONCLKDIV[7];	/*!< IOCON block for programmable glitch filter divider registers */
	reg32_t         BODCTRL;			/*!< Brown Out Detect register */
	reg32_t         SYSTCKCAL;		/*!< System tick counter calibration register */
	REG32_RSVD(6)
	reg32_t         IRQLATENCY;		/*!< IRQ delay register */
	reg32_t         NMISRC;			/*!< NMI source control register */
	reg32_t         PINTSEL[8];		/*!< GPIO pin interrupt select register 0-7 */
	reg32_t         USBCLKCTRL;		/*!< USB clock control register */
	const reg32_t   USBCLKST;		/*!< USB clock status register */
	REG32_RSVD(25)
	reg32_t         STARTERP0;		/*!< Start logic 0 interrupt wake-up enable register */
	REG32_RSVD(3)
	reg32_t         STARTERP1;		/*!< Start logic 1 interrupt wake-up enable register */
	REG32_RSVD(6)
	reg32_t         PDSLEEPCFG;		/*!< Power down states in deep sleep mode register */
	reg32_t         PDWAKECFG;		/*!< Power down states in wake up from deep sleep register */
	reg32_t         PDRUNCFG;		/*!< Power configuration register*/
	REG32_RSVD(110)
	const reg32_t   DEVICEID;		/*!< Device ID register */
} reg_syscon_t;
//! @}

typedef volatile struct {
    REG32_RSVD(4) //!< Reserved
    reg32_t         FLASHCFG;              
} flash_reg_t; 


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
