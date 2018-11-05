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


#ifndef __DRIVER_TMS320_TMS320X2812_DEVICE_H__
#define __DRIVER_TMS320_TMS320X2812_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define DEV_EMU_BASE_ADDRESS        (0x000880)     /* device emulation registers */
#define PIE_VECT_BASE_ADDRESS       (0x000D00)     /* PIE Vector Table */
#define FLASH_BASE_ADDRESS          (0x000A80)     /* FLASH registers */
#define CSM_BASE_ADDRESS            (0x000AE0)     /* code security module registers */
#define XINTF_BASE_ADDRESS          (0x000B20)     /* external interface registers */
#define CPU_TIMER0_BASE_ADDRESS     (0x000C00)     /* CPU Timer0 registers */
#define CPU_TIMER1_BASE_ADDRESS     (0x000C08)     /* CPU Timer1 registers */
#define CPU_TIMER2_BASE_ADDRESS     (0x000C10)     /* CPU Timer2 registers */
#define PIE_BASE_ADDRESS            (0x000CE0)     /* PIE control registers */
#define ECANA_BASE_ADDRESS          (0x006000)     /* eCAN control and status registers */
#define ECANA_LAM_BASE_ADDRESS      (0x006040)     /* eCAN local acceptance masks */
#define ECANA_MOTS_BASE_ADDRESS     (0x006080)     /* eCAN message object time stamps */
#define ECANA_MOTO_BASE_ADDRESS     (0x0060C0)     /* eCAN object time-out registers */
#define ECANA_MBOX_BASE_ADDRESS     (0x006100)     /* eCAN mailboxes */
#define SYSTEM_BASE_ADDRESS         (0x007010)     /* System control registers */
#define SPIA_BASE_ADDRESS           (0x007040)     /* SPI registers */
#define SCIA_BASE_ADDRESS           (0x007050)     /* SCI-A registers */
#define XINTRUPT_BASE_ADDRESS       (0x007070)     /* external interrupt registers */
#define GPIOMUX_BASE_ADDRESS        (0x0070C0)     /* GPIO mux registers */
#define GPIODAT_BASE_ADDRESS        (0x0070E0)     /* GPIO data registers */
#define ADC_BASE_ADDRESS            (0x007100)     /* ADC registers */
#define EVA_BASE_ADDRESS            (0x007400)     /* Event Manager A registers */
#define EVB_BASE_ADDRESS            (0x007500)     /* Event Manager B registers */
#define SCIB_BASE_ADDRESS           (0x007750)     /* SCI-B registers */
#define MCBSPA_BASE_ADDRESS         (0x007800)     /* McBSP registers */
#define CSM_PSW_BASE_ADDRESS        (0x3F7FF8)     /* Part of FLASHA.  CSM password locations. */

/*-------------------------------- IO ----------------------------------------*/
#define WDT_BASE_ADDRESS            0x00007022

//! \name gpio macros
//! @{
#define GPIOA_BASE_ADDRESS          0x000070E0
#define GPIOB_BASE_ADDRESS          0x000070E4
#define GPIOD_BASE_ADDRESS          0x000070EC
#define GPIOE_BASE_ADDRESS          0x000070F0
#define GPIOF_BASE_ADDRESS          0x000070F4
#define GPIOG_BASE_ADDRESS          0x000070F8
//! @}

//! \name IO Controller macros
//! @{
#define IOCTRLA_BASE_ADDRESS        0x000070C0
#define IOCTRLB_BASE_ADDRESS        0x000070C4
#define IOCTRLD_BASE_ADDRESS        0x000070CC
#define IOCTRLE_BASE_ADDRESS        0x000070D0
#define IOCTRLF_BASE_ADDRESS        0x000070D4
#define IOCTRLG_BASE_ADDRESS        0x000070D8
//! @}

#define IO_PORT_NUM                 (6u)
#define IO_PIN_NUM                  (16u)
#define IO_PORTA
#define IO_PORTB
#define IO_PORTD
#define IO_PORTE
#define IO_PORTF
#define IO_PORTG

/*--------------------------------- SCI --------------------------------------*/
#define USART_COUNT                 2
#define USART0                      USART[0]
#define USART1                      USART[1]

#define PIE_VECTOR_TABLE_SIZE       (128u)

/*--------------------------------- TIMER ------------------------------------*/
#define TIMER_COUNT                 3
#define TIMER0                      TIMER[0]
#define TIMER1                      TIMER[1]
#define TIMER2                      TIMER[2]

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
// Define Vector Table:
typedef enum {
    //! 32 cpu interrupts
    RESET_IRQn          = 0,
    INT1_IRQn,      //! INT1_IRQn~INT12_IRQn used by PIE group1~group12
    INT2_IRQn,
    INT3_IRQn,
    INT4_IRQn,
    INT5_IRQn,
    INT6_IRQn,
    INT7_IRQn,
    INT8_IRQn,
    INT9_IRQn,
    INT10_IRQn,
    INT11_IRQn,
    INT12_IRQn,
    XINT13,         // INT13_IRQn
    TINT2,          // INT14_IRQn
    DATALOG,        // DATALOG_IRQn
    RTOSINT,        // RTOSINT_IRQn
    EMUINT,         // rsvd_IRQn
    XNMI,           // XNMI_IRQn
    ILLEGAL,        // ILLEGAL_IRQn
    USER1_IRQn,     // User Defined trap 1
    USER2_IRQn,     // User Defined trap 2
    USER3_IRQn,     // User Defined trap 3
    USER4_IRQn,     // User Defined trap 4
    USER5_IRQn,     // User Defined trap 5
    USER6_IRQn,     // User Defined trap 6
    USER7_IRQn,     // User Defined trap 7
    USER8_IRQn,     // User Defined trap 8
    USER9_IRQn,     // User Defined trap 9
    USER10_IRQn,    // User Defined trap 10
    USER11_IRQn,    // User Defined trap 11
    USER12_IRQn,    // User Defined trap 12

    // Group 1 PIE Peripheral Vectors:
    PDPINTA_IRQn,   // EV-A
    PDPINTB_IRQn,   // EV-B
    rsvd1_3_IRQn,
    XINT1_IRQn,
    XINT2_IRQn,
    ADCINT_IRQn,    // ADC
    TINT0_IRQn,     // Timer 0
    WAKEINT_IRQn,   // WD

    // Group 2 PIE Peripheral Vectors:
    CMP1INT_IRQn,   // EV-A
    CMP2INT_IRQn,   // EV-A
    CMP3INT_IRQn,   // EV-A
    T1PINT_IRQn,    // EV-A
    T1CINT_IRQn,    // EV-A
    T1UFINT_IRQn,   // EV-A
    T1OFINT_IRQn,   // EV-A
    rsvd2_8_IRQn,

    // Group 3 PIE Peripheral Vectors:
    T2PINT_IRQn,    // EV-A
    T2CINT_IRQn,    // EV-A
    T2UFINT_IRQn,   // EV-A
    T2OFINT_IRQn,   // EV-A
    CAPINT1_IRQn,   // EV-A
    CAPINT2_IRQn,   // EV-A
    CAPINT3_IRQn,   // EV-A
    rsvd3_8_IRQn,

    // Group 4 PIE Peripheral Vectors:
    CMP4INT_IRQn,   // EV-B
    CMP5INT_IRQn,   // EV-B
    CMP6INT_IRQn,   // EV-B
    T3PINT_IRQn,    // EV-B
    T3CINT_IRQn,    // EV-B
    T3UFINT_IRQn,   // EV-B
    T3OFINT_IRQn,   // EV-B
    rsvd4_8_IRQn,

    // Group 5 PIE Peripheral Vectors:
    T4PINT_IRQn,    // EV-B
    T4CINT_IRQn,    // EV-B
    T4UFINT_IRQn,   // EV-B
    T4OFINT_IRQn,   // EV-B
    CAPINT4_IRQn,   // EV-B
    CAPINT5_IRQn,   // EV-B
    CAPINT6_IRQn,   // EV-B
    rsvd5_8_IRQn,

    // Group 6 PIE Peripheral Vectors:
    SPIRXINTA_IRQn, // SPI-A
    SPITXINTA_IRQn, // SPI-A
    rsvd6_3_IRQn,
    rsvd6_4_IRQn,
    MRINTA_IRQn,    // McBSP-A
    MXINTA_IRQn,    // McBSP-A
    rsvd6_7_IRQn,
    rsvd6_8_IRQn,

    // Group 7 PIE Peripheral Vectors:
    rsvd7_1_IRQn,
    rsvd7_2_IRQn,
    rsvd7_3_IRQn,
    rsvd7_4_IRQn,
    rsvd7_5_IRQn,
    rsvd7_6_IRQn,
    rsvd7_7_IRQn,
    rsvd7_8_IRQn,

    // Group 8 PIE Peripheral Vectors:
    rsvd8_1_IRQn,
    rsvd8_2_IRQn,
    rsvd8_3_IRQn,
    rsvd8_4_IRQn,
    rsvd8_5_IRQn,
    rsvd8_6_IRQn,
    rsvd8_7_IRQn,
    rsvd8_8_IRQn,

    // Group 9 PIE Peripheral Vectors:
    RXAINT_IRQn,    // SCI-A
    TXAINT_IRQn,    // SCI-A
    RXBINT_IRQn,    // SCI-B
    TXBINT_IRQn,    // SCI-B
    ECAN0INTA_IRQn, // eCAN
    ECAN1INTA_IRQn, // eCAN
    rsvd9_7_IRQn,
    rsvd9_8_IRQn,

    // Group 10 PIE Peripheral Vectors:
    rsvd10_1_IRQn,
    rsvd10_2_IRQn,
    rsvd10_3_IRQn,
    rsvd10_4_IRQn,
    rsvd10_5_IRQn,
    rsvd10_6_IRQn,
    rsvd10_7_IRQn,
    rsvd10_8_IRQn,

    // Group 11 PIE Peripheral Vectors:
    rsvd11_1_IRQn,
    rsvd11_2_IRQn,
    rsvd11_3_IRQn,
    rsvd11_4_IRQn,
    rsvd11_5_IRQn,
    rsvd11_6_IRQn,
    rsvd11_7_IRQn,
    rsvd11_8_IRQn,

    // Group 12 PIE Peripheral Vectors:
    rsvd12_1_IRQn,
    rsvd12_2_IRQn,
    rsvd12_3_IRQn,
    rsvd12_4_IRQn,
    rsvd12_5_IRQn,
    rsvd12_6_IRQn,
    rsvd12_7_IRQn,
    rsvd12_8_IRQn,
} em_irqn_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
