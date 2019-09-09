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


#ifndef __REG_SCON_H__
#define __REG_SCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SC_REG                  (*(sc_reg_t        *) SC_BASE       )

//! \note mask for FLASHTIM
//! @{
#define SC_FLASHTIM_RESVERED_VALUE  (0x3Aul)
#define SC_FLASHTIM_0               12
#define SC_FLASHTIM_MASK            (0x0ful << SC_FLASHTIM_0)
//! @}

#define SC_CLKSRCSEL_MSK        (0x03ul)

//! \note mask for PLL0CON
//! @{
#define SC_PLL0CON_PLLE0        0
#define SC_PLL0CON_PLLE0_MSK    (1ul << SC_PLL0CON_PLLE0)

#define SC_PLL0CON_PLLC0        1
#define SC_PLL0CON_PLLC0_MSK    (1ul << SC_PLL0CON_PLLC0)
//! @}
     
//! \note mask for PLL0STAT
//! @{
#define SC_PLL0STAT_MSEL0       0
#define SC_PLL0STAT_MSEL_MSK    ((1ul << 15) - 1)

#define SC_PLL0STAT_NSEL0       16
#define SC_PLL0STAT_NSEL_MSK    (((1ul << 24) - 1) ^ ((1ul << 15) - 1))
     
#define SC_PLL0STAT_PLLE0       24
#define SC_PLL0STAT_PLLE0_MSK   (1ul << SC_PLL0STAT_PLLE0)

#define SC_PLL0STAT_PLLC0       25
#define SC_PLL0STAT_PLLC0_MSK   (1ul << SC_PLL0STAT_PLLE0)

#define SC_PLL0STAT_PLOCK0      26
#define SC_PLL0STAT_PLOCK0_MSK  (1ul << SC_PLL0STAT_PLLE0)
//! @}
     
//! \note mask for PLL0CFG
//! @{
#define SC_PLL0CFG_MSEL0        0
#define SC_PLL0CFG_MSEL_MSK     ((1ul << 15) - 1)

#define SC_PLL0CFG_NSEL0        16
#define SC_PLL0CFG_NSEL_MSK     (((1ul << 24) - 1) ^ ((1ul << 15) - 1))
//! @}

//! \note mask for SCS
//! @{
#define SC_SCS_OSCRANGE         4
#define SC_SCS_OSCRANGE_MSK     (1ul << SC_SCS_OSCRANGE)

#define SC_SCS_OSCEN            5
#define SC_SCS_OSCEN_MSK        (1ul << SC_SCS_OSCEN)

#define SC_SCS_OSCSTAT          6
#define SC_SCS_OSCSTAT_MSK      (1ul << SC_SCS_OSCSTAT)
//! @}

#define SC_CCLKCFG_CCLKSEL_MSK  (0xFFul)

//! \note mask for CLKOUTCFG
//! @{
#define SC_CLKOUTCFG_SEL0       0
#define SC_CLKOUTCFG_SEL_MSK    (0x0Ful)

#define SC_CLKOUTCFG_DIV0       4
#define SC_CLKOUTCFG_DIV_MSK    (0x0Ful << SC_CLKOUTCFG_DIV0)

#define SC_CLKOUTCFG_EN         8
#define SC_CLKOUTCFG_EN_MSK     (1ul << SC_CLKOUTCFG_EN)

#define SC_CLKOUTCFG_ACT        9
#define SC_CLKOUTCFG_ACT_MSK    (1ul << SC_CLKOUTCFG_ACT)
//! @}

typedef enum {
    PCONP_TIM0      = 1,
    PCONP_TIM1,
    PCONP_UART0,
    PCONP_UART1,
    PCONP_PWM1      = 6,
    PCONP_I2C0,
    PCONP_SPI,
    PCONP_RTC,
    PCONP_SSP1,
    PCONP_ADC       = 12,
    PCONP_CAN1,
    PCONP_CAN2,
    PCONP_GPIO,
    PCONP_RIT,
    PCONP_MC,
    PCONP_QEI,
    PCONP_I2C1,
    PCONP_SSP0      = 21,
    PCONP_TIM2,
    PCONP_TIM3,
    PCONP_UART2,
    PCONP_UART3,
    PCONP_I2C2,
    PCONP_I2S,
    PCONP_GPDMA     = 29,
    PCONP_ENET,
    PCONP_USB,
} em_pconp_t;

#define SC_PCONP_TIM0_MSK           (1ul << PCONP_TIM0)
#define SC_PCONP_TIM1_MSK           (1ul << PCONP_TIM1)
#define SC_PCONP_UART0_MSK          (1ul << PCONP_UART0)
#define SC_PCONP_UART1_MSK          (1ul << PCONP_UART1)
#define SC_PCONP_PWM1_MSK           (1ul << PCONP_PWM1)
#define SC_PCONP_I2C0_MSK           (1ul << PCONP_I2C0)
#define SC_PCONP_SPI_MSK            (1ul << PCONP_SPI)
#define SC_PCONP_RTC_MSK            (1ul << PCONP_RTC)
#define SC_PCONP_SSP1_MSK           (1ul << PCONP_SSP1)
#define SC_PCONP_ADC_MSK            (1ul << PCONP_ADC)
#define SC_PCONP_CAN1_MSK           (1ul << PCONP_CAN1)
#define SC_PCONP_CAN2_MSK           (1ul << PCONP_CAN2)
#define SC_PCONP_GPIO_MSK           (1ul << PCONP_GPIO)
#define SC_PCONP_RIT_MSK            (1ul << PCONP_RIT)
#define SC_PCONP_MC_MSK             (1ul << PCONP_MC)
#define SC_PCONP_QEI_MSK            (1ul << PCONP_QEI)
#define SC_PCONP_I2C1_MSK           (1ul << PCONP_I2C1)
#define SC_PCONP_SSP0_MSK           (1ul << PCONP_SSP0)
#define SC_PCONP_TIM2_MSK           (1ul << PCONP_TIM2)
#define SC_PCONP_TIM3_MSK           (1ul << PCONP_TIM3)
#define SC_PCONP_UART2_MSK          (1ul << PCONP_UART2)
#define SC_PCONP_UART3_MSK          (1ul << PCONP_UART3)
#define SC_PCONP_I2C2_MSK           (1ul << PCONP_I2C2)
#define SC_PCONP_I2S_MSK            (1ul << PCONP_I2S)
#define SC_PCONP_GPDMA_MSK          (1ul << PCONP_GPDMA)
#define SC_PCONP_ENET_MSK           (1ul << PCONP_ENET)
#define SC_PCONP_USB_MSK            (1ul << PCONP_USB)

typedef enum {
    PCLK_WDT        = 0,
    PCLK_TIM0       = 2,
    PCLK_TIM1       = 4,
    PCLK_UART0      = 6,
    PCLK_UART1      = 8,
    PCLK_PWM1       = 12,
    PCLK_I2C0       = 14,
    PCLK_SPI        = 16,
    PCLK_SSP1       = 20,
    PCLK_DAC        = 22,
    PCLK_ADC        = 24,
    PCLK_CAN1       = 26,
    PCLK_CAN2       = 28,
    PCLK_ACF        = 30,

    PCLK_QEI        = 0x80 + 0,
    PCLK_GPIOINT    = 0x80 + 2,
    PCLK_PCB        = 0x80 + 4,
    PCLK_I2C1       = 0x80 + 6,
    PCLK_SSP0       = 0x80 + 10,
    PCLK_TIM2       = 0x80 + 12,
    PCLK_TIM3       = 0x80 + 14,
    PCLK_UART2      = 0x80 + 16,
    PCLK_UART3      = 0x80 + 18,
    PCLK_I2C2       = 0x80 + 20,
    PCLK_I2S        = 0x80 + 22,
    PCLK_RIT        = 0x80 + 26,
    PCLK_SYSCON     = 0x80 + 28,
    PCLK_MC         = 0x80 + 30,
} em_pclksel_t;

typedef enum {
    PCLK_DIV_4      = 0,
    PCLK_DIV_1,
    PCLK_DIV_2,
    PCLK_DIV_8,
} em_pclk_div_t;

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- System Control (SC) ------------------------------------------*/
/** @brief System Control (SC) register structure definition */
typedef struct {
    reg32_t FLASHCFG;               /* Flash Accelerator Module           */
    REG32_RSVD(31)
    reg32_t PLL0CON;                /* Clocking and Power Control         */
    reg32_t PLL0CFG;
    reg32_t PLL0STAT;
    reg32_t PLL0FEED;
    REG32_RSVD(4)
    reg32_t PLL1CON;
    reg32_t PLL1CFG;
    reg32_t PLL1STAT;
    reg32_t PLL1FEED;
    REG32_RSVD(4)
    reg32_t PCON;
    reg32_t PCONP;
    REG32_RSVD(15)
    reg32_t CCLKCFG;
    reg32_t USBCLKCFG;
    reg32_t CLKSRCSEL;
    reg32_t	CANSLEEPCLR;
    reg32_t	CANWAKEFLAGS;
    REG32_RSVD(10)
    reg32_t EXTINT;                 /* External Interrupts                */
    REG32_RSVD(1)
    reg32_t EXTMODE;
    reg32_t EXTPOLAR;
    REG32_RSVD(12)
    reg32_t RSID;                   /* Reset                              */
    REG32_RSVD(7)
    reg32_t SCS;                    /* Syscon Miscellaneous Registers     */
    reg32_t IRCTRIM;                /* Clock Dividers                     */
    reg32_t PCLKSEL0;
    reg32_t PCLKSEL1;
    REG32_RSVD(4)
    reg32_t USBIntSt;               /* USB Device/OTG Interrupt Register  */
    reg32_t DMAREQSEL;
    reg32_t CLKOUTCFG;              /* Clock Output Configuration         */
 } sc_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/



#endif
