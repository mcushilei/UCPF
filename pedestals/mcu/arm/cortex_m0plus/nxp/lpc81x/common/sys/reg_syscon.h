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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_SYSCON_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_SYSCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SYSCON_REG          (*(reg_syscon_t *)      SYSCON_BASE_ADDRESS)
#define FLASHCTRL_REG       (*(flashctrl_reg_t *)   FLASHCRTL_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    reg32_t SYSMEMREMAP;            //!< Offset: 0x000 System memory remap (R/W) */
    reg32_t PRESETCTRL;             //!< Offset: 0x004 Peripheral reset control (R/W) */
    reg32_t SYSPLLCTRL;             //!< Offset: 0x008 System PLL control (R/W) */
    reg32_t SYSPLLSTAT;             //!< Offset: 0x00C System PLL status (R/W ) */
    REG32_RSVD(4)

    reg32_t SYSOSCCTRL;             //!< Offset: 0x020 System oscillator control (R/W) */
    reg32_t WDTOSCCTRL;             //!< Offset: 0x024 Watchdog oscillator control (R/W) */
    reg32_t IRCCTRL;                // 0x28            
    REG32_RSVD(1)
    reg32_t SYSRSTSTAT;             //!< Offset: 0x030 System reset status Register (R/W ) */
    REG32_RSVD(3)
    reg32_t SYSPLLCLKSEL;           //!< Offset: 0x040 System PLL clock source select (R/W) */	
    reg32_t SYSPLLCLKUEN;           //!< Offset: 0x044 System PLL clock source update enable (R/W) */
    REG32_RSVD(10)

    reg32_t MAINCLKSEL;             //!< Offset: 0x070 Main clock source select (R/W) */
    reg32_t MAINCLKUEN;             //!< Offset: 0x074 Main clock source update enable (R/W) */
    reg32_t SYSAHBCLKDIV;           //!< Offset: 0x078 System AHB clock divider (R/W) */
    REG32_RSVD(1)

    reg32_t SYSAHBCLKCTRL;          //!< Offset: 0x080 System AHB clock control (R/W) */
    REG32_RSVD(4)        
    reg32_t UARTCLKDIV;             //!< Offset: 0x094 UART clock divider (R/W) */         
    REG32_RSVD(18)

    reg32_t CLKOUTSEL;              //!< Offset: 0x0E0 CLKOUT clock source select (R/W) */
    reg32_t CLKOUTUEN;              //!< Offset: 0x0E4 CLKOUT clock source update enable (R/W) */
    reg32_t CLKOUTDIV;              //!< Offset: 0x0E8 CLKOUT clock divider (R/W) */       
    REG32_RSVD(1)
    reg32_t UARTFRGDIV;             //!< Offset: 0x0F0 UART fractional divider SUB(R/W) */ 
    reg32_t UARTFRGMULT;             //!< Offset: 0x0F4 UART fractional divider ADD(R/W) */   
    REG32_RSVD(1)
    reg32_t EXTTRACECMD;            //!< (@ 0x400480FC) External trace buffer command register  */  
    reg32_t PIOPORCAP0;             //!< Offset: 0x100 POR captured PIO status 0 (R/ ) */  
    REG32_RSVD(12)
    reg32_t IOCONCLKDIV6;           // 0x134
    reg32_t IOCONCLKDIV5;           // 0x138
    reg32_t IOCONCLKDIV4;           // 0x13c
    reg32_t IOCONCLKDIV3;           // 0x140
    reg32_t IOCONCLKDIV2;           // 0x144
    reg32_t IOCONCLKDIV1;           // 0x148
    reg32_t IOCONCLKDIV0;           // 0x14c
    reg32_t BODCTRL;                //!< Offset: 0x150 BOD control (R/W) */
    reg32_t SYSTCKCAL;              //!< Offset: 0x154 System tick counter calibration (R/W) */
    REG32_RSVD(6)
    reg32_t IRQLATENCY;             //!< (@ 0x40048170) IRQ delay */
    reg32_t NMISRC;                 //!< (@ 0x40048174) NMI Source Control     */
    reg32_t PINTSEL[8];             //!< (@ 0x40048178) GPIO Pin Interrupt Select register 0 */
    REG32_RSVD(27)
    reg32_t STARTERP0;              //!< Offset: 0x204 Start logic signal enable Register 0 (R/W) */      
    REG32_RSVD(3)
    reg32_t STARTERP1;              //!< Offset: 0x214 Start logic signal enable Register 0 (R/W) */      
    REG32_RSVD(6)
    reg32_t PDSLEEPCFG;             //!< Offset: 0x230 Power-down states in Deep-sleep mode (R/W) */
    reg32_t PDAWAKECFG;             //!< Offset: 0x234 Power-down states after wake-up (R/W) */        
    reg32_t PDRUNCFG;               //!< Offset: 0x238 Power-down configuration Register (R/W) */
    REG32_RSVD(111)
    reg32_t DEVICE_ID;              // 0x3f8
} reg_syscon_t;

typedef volatile struct {
    REG32_RSVD(4)
    reg32_t  FLASHCFG;               //!< (@ 0x40040010) Flash configuration register */
    REG32_RSVD(3)
    reg32_t  FMSSTART;               //!< (@ 0x40040020) Signature start address register */
    reg32_t  FMSSTOP;                //!< (@ 0x40040024) Signature stop-address register  */
    REG32_RSVD(1)
    reg32_t  FMSW0;                  //!< (@ 0x4004002C) Signature Word */
} flashctrl_reg_t; 

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
