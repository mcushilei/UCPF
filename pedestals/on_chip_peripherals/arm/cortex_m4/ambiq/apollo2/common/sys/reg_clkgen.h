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

#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_CLKGEN_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_CLKGEN_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_CLKGEN          (*(reg_clkgen_t *)REG_CLKGEN_BASEADDR)

#define REG_CLKGENn(n)      (*(reg_clkgen_t *)(REG_CLKGEN_BASEADDR + 0x00000000 * n))

//*****************************************************************************
//
// Key values.
//
//*****************************************************************************
#define REG_CLKGEN_CLKKEY_KEYVAL                  0x00000047

//*****************************************************************************
//
// CLKGEN_INTEN - CLKGEN Interrupt Register: Enable
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_CLKGEN_INTEN_ALM_S                    3
#define REG_CLKGEN_INTEN_ALM_M                    0x00000008

// XT Oscillator Fail interrupt
#define REG_CLKGEN_INTEN_OF_S                     2
#define REG_CLKGEN_INTEN_OF_M                     0x00000004

// Autocalibration Complete interrupt
#define REG_CLKGEN_INTEN_ACC_S                    1
#define REG_CLKGEN_INTEN_ACC_M                    0x00000002

// Autocalibration Fail interrupt
#define REG_CLKGEN_INTEN_ACF_S                    0
#define REG_CLKGEN_INTEN_ACF_M                    0x00000001

//*****************************************************************************
//
// CLKGEN_INTSTAT - CLKGEN Interrupt Register: Status
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_CLKGEN_INTSTAT_ALM_S                  3
#define REG_CLKGEN_INTSTAT_ALM_M                  0x00000008

// XT Oscillator Fail interrupt
#define REG_CLKGEN_INTSTAT_OF_S                   2
#define REG_CLKGEN_INTSTAT_OF_M                   0x00000004

// Autocalibration Complete interrupt
#define REG_CLKGEN_INTSTAT_ACC_S                  1
#define REG_CLKGEN_INTSTAT_ACC_M                  0x00000002

// Autocalibration Fail interrupt
#define REG_CLKGEN_INTSTAT_ACF_S                  0
#define REG_CLKGEN_INTSTAT_ACF_M                  0x00000001

//*****************************************************************************
//
// CLKGEN_INTCLR - CLKGEN Interrupt Register: Clear
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_CLKGEN_INTCLR_ALM_S                   3
#define REG_CLKGEN_INTCLR_ALM_M                   0x00000008

// XT Oscillator Fail interrupt
#define REG_CLKGEN_INTCLR_OF_S                    2
#define REG_CLKGEN_INTCLR_OF_M                    0x00000004

// Autocalibration Complete interrupt
#define REG_CLKGEN_INTCLR_ACC_S                   1
#define REG_CLKGEN_INTCLR_ACC_M                   0x00000002

// Autocalibration Fail interrupt
#define REG_CLKGEN_INTCLR_ACF_S                   0
#define REG_CLKGEN_INTCLR_ACF_M                   0x00000001

//*****************************************************************************
//
// CLKGEN_INTSET - CLKGEN Interrupt Register: Set
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_CLKGEN_INTSET_ALM_S                   3
#define REG_CLKGEN_INTSET_ALM_M                   0x00000008

// XT Oscillator Fail interrupt
#define REG_CLKGEN_INTSET_OF_S                    2
#define REG_CLKGEN_INTSET_OF_M                    0x00000004

// Autocalibration Complete interrupt
#define REG_CLKGEN_INTSET_ACC_S                   1
#define REG_CLKGEN_INTSET_ACC_M                   0x00000002

// Autocalibration Fail interrupt
#define REG_CLKGEN_INTSET_ACF_S                   0
#define REG_CLKGEN_INTSET_ACF_M                   0x00000001

//*****************************************************************************
//
// CLKGEN_CALXT - XT Oscillator Control
//
//*****************************************************************************
// XT Oscillator calibration value
#define REG_CLKGEN_CALXT_CALXT_S                  0
#define REG_CLKGEN_CALXT_CALXT_M                  0x000007FF

//*****************************************************************************
//
// CLKGEN_CALRC - RC Oscillator Control
//
//*****************************************************************************
// LFRC Oscillator calibration value
#define REG_CLKGEN_CALRC_CALRC_S                  0
#define REG_CLKGEN_CALRC_CALRC_M                  0x0003FFFF

//*****************************************************************************
//
// CLKGEN_ACALCTR - Autocalibration Counter
//
//*****************************************************************************
// Autocalibration Counter result.
#define REG_CLKGEN_ACALCTR_ACALCTR_S              0
#define REG_CLKGEN_ACALCTR_ACALCTR_M              0x00FFFFFF

//*****************************************************************************
//
// CLKGEN_OCTRL - Oscillator Control
//
//*****************************************************************************
// Autocalibration control
#define REG_CLKGEN_OCTRL_ACAL_S                   8
#define REG_CLKGEN_OCTRL_ACAL_M                   0x00000700

// Selects the RTC oscillator (1 => LFRC, 0 => XT)
#define REG_CLKGEN_OCTRL_OSEL_S                   7
#define REG_CLKGEN_OCTRL_OSEL_M                   0x00000080

// Oscillator switch on failure function
#define REG_CLKGEN_OCTRL_FOS_S                    6
#define REG_CLKGEN_OCTRL_FOS_M                    0x00000040

// Stop the LFRC Oscillator to the RTC
#define REG_CLKGEN_OCTRL_STOPRC_S                 1
#define REG_CLKGEN_OCTRL_STOPRC_M                 0x00000002

// Stop the XT Oscillator to the RTC
#define REG_CLKGEN_OCTRL_STOPXT_S                 0
#define REG_CLKGEN_OCTRL_STOPXT_M                 0x00000001

//*****************************************************************************
//
// CLKGEN_CLKOUT - CLKOUT Frequency Select
//
//*****************************************************************************
// Enable the CLKOUT signal
#define REG_CLKGEN_CLKOUT_CKEN_S                  7
#define REG_CLKGEN_CLKOUT_CKEN_M                  0x00000080

// CLKOUT signal select.  Note that HIGH_DRIVE should be selected if any high
// frequencies (such as from HFRC) are selected for CLKOUT.
#define REG_CLKGEN_CLKOUT_CKSEL_S                 0
#define REG_CLKGEN_CLKOUT_CKSEL_M                 0x0000003F
#define REG_CLKGEN_CLKOUT_CKSEL(n)                (((uint32_t)(n) << 0) & 0x0000003F)
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC              0x00000000
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV2           0x00000001
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV4           0x00000002
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV8           0x00000003
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV16          0x00000004
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV32          0x00000005
#define REG_CLKGEN_CLKOUT_CKSEL_RTC_1Hz           0x00000010
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV2M          0x00000016
#define REG_CLKGEN_CLKOUT_CKSEL_XT                0x00000017
#define REG_CLKGEN_CLKOUT_CKSEL_CG_100Hz          0x00000018
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC              0x00000019
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV4         0x0000001A
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV8         0x0000001B
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV16        0x0000001C
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64        0x0000001D
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV128       0x0000001E
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV256       0x0000001F
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV512       0x00000020
#define REG_CLKGEN_CLKOUT_CKSEL_FLASH_CLK         0x00000022
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2         0x00000023
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32        0x00000024
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV512       0x00000025
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32K       0x00000026
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV256         0x00000027
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV8K          0x00000028
#define REG_CLKGEN_CLKOUT_CKSEL_XT_DIV64K         0x00000029
#define REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV16       0x0000002A
#define REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV128      0x0000002B
#define REG_CLKGEN_CLKOUT_CKSEL_ULFRC_1Hz         0x0000002C
#define REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV4K       0x0000002D
#define REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV1M       0x0000002E
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64K       0x0000002F
#define REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV16M       0x00000030
#define REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2M        0x00000031
#define REG_CLKGEN_CLKOUT_CKSEL_HFRCNE            0x00000032
#define REG_CLKGEN_CLKOUT_CKSEL_HFRCNE_DIV8       0x00000033
#define REG_CLKGEN_CLKOUT_CKSEL_XTNE              0x00000035
#define REG_CLKGEN_CLKOUT_CKSEL_XTNE_DIV16        0x00000036
#define REG_CLKGEN_CLKOUT_CKSEL_LFRCNE_DIV32      0x00000037
#define REG_CLKGEN_CLKOUT_CKSEL_LFRCNE            0x00000039


//*****************************************************************************
//
// CLKGEN_CCTRL - HFRC Clock Control
//
//*****************************************************************************
// Core Clock divisor
#define REG_CLKGEN_CCTRL_CORESEL_S                0
#define REG_CLKGEN_CCTRL_CORESEL_M                0x00000001
#define REG_CLKGEN_CCTRL_CORESEL_HFRC             0x00000000
#define REG_CLKGEN_CCTRL_CORESEL_HFRC_DIV2        0x00000001


//*****************************************************************************
//
// CLKGEN_STATUS - Clock Generator Status
//
//*****************************************************************************
// XT Oscillator is enabled but not oscillating
#define REG_CLKGEN_STATUS_OSCF_S                  1
#define REG_CLKGEN_STATUS_OSCF_M                  0x00000002

// Current RTC oscillator (1 => LFRC, 0 => XT)
#define REG_CLKGEN_STATUS_OMODE_S                 0
#define REG_CLKGEN_STATUS_OMODE_M                 0x00000001

//*****************************************************************************
//
// CLKGEN_HFADJ - HFRC Adjustment
//
//*****************************************************************************
#define REG_CLKGEN_HFADJ_HFXTADJ_DEFAULT         0x5B8


// Gain control for HFRC adjustment
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_S             21
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_M             0x00E00000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN(n)            (((uint32_t)(n) << 21) & 0x00E00000)
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1     0x00000000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_2 0x00200000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_4 0x00400000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_8 0x00600000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_16 0x00800000
#define REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_32 0x00A00000

// XT warmup period for HFRC adjustment
#define REG_CLKGEN_HFADJ_HFWARMUP_S               20
#define REG_CLKGEN_HFADJ_HFWARMUP_M               0x00100000
#define REG_CLKGEN_HFADJ_HFWARMUP(n)              (((uint32_t)(n) << 20) & 0x00100000)
#define REG_CLKGEN_HFADJ_HFWARMUP_1SEC            0x00000000
#define REG_CLKGEN_HFADJ_HFWARMUP_2SEC            0x00100000

// Target HFRC adjustment value.
#define REG_CLKGEN_HFADJ_HFXTADJ_S                8
#define REG_CLKGEN_HFADJ_HFXTADJ_M                0x000FFF00
#define REG_CLKGEN_HFADJ_HFXTADJ(n)               (((uint32_t)(n) << 8) & 0x000FFF00)

// Repeat period for HFRC adjustment
#define REG_CLKGEN_HFADJ_HFADJCK_S                1
#define REG_CLKGEN_HFADJ_HFADJCK_M                0x0000000E
#define REG_CLKGEN_HFADJ_HFADJCK(n)               (((uint32_t)(n) << 1) & 0x0000000E)
#define REG_CLKGEN_HFADJ_HFADJCK_4SEC             0x00000000
#define REG_CLKGEN_HFADJ_HFADJCK_16SEC            0x00000002
#define REG_CLKGEN_HFADJ_HFADJCK_32SEC            0x00000004
#define REG_CLKGEN_HFADJ_HFADJCK_64SEC            0x00000006
#define REG_CLKGEN_HFADJ_HFADJCK_128SEC           0x00000008
#define REG_CLKGEN_HFADJ_HFADJCK_256SEC           0x0000000A
#define REG_CLKGEN_HFADJ_HFADJCK_512SEC           0x0000000C
#define REG_CLKGEN_HFADJ_HFADJCK_1024SEC          0x0000000E

// HFRC adjustment control
#define REG_CLKGEN_HFADJ_HFADJEN_S                0
#define REG_CLKGEN_HFADJ_HFADJEN_M                0x00000001
#define REG_CLKGEN_HFADJ_HFADJEN(n)               (((uint32_t)(n) << 0) & 0x00000001)
#define REG_CLKGEN_HFADJ_HFADJEN_DIS              0x00000000
#define REG_CLKGEN_HFADJ_HFADJEN_EN               0x00000001

//*****************************************************************************
//
// CLKGEN_CLOCKEN - Clock Enable Status
//
//*****************************************************************************
// Clock enable status
#define REG_CLKGEN_CLOCKEN_CLOCKEN_S              0
#define REG_CLKGEN_CLOCKEN_CLOCKEN_M              0xFFFFFFFF

//*****************************************************************************
//
// CLKGEN_CLOCKEN2 - Clock Enable Status
//
//*****************************************************************************
// Clock enable status 2
#define REG_CLKGEN_CLOCKEN2_CLOCKEN2_S            0
#define REG_CLKGEN_CLOCKEN2_CLOCKEN2_M            0xFFFFFFFF

//*****************************************************************************
//
// CLKGEN_CLOCKEN3 - Clock Enable Status
//
//*****************************************************************************
// Clock enable status 3
#define REG_CLKGEN_CLOCKEN3_CLOCKEN3_S            0
#define REG_CLKGEN_CLOCKEN3_CLOCKEN3_M            0xFFFFFFFF

//*****************************************************************************
//
// CLKGEN_UARTEN - UART Enable
//
//*****************************************************************************
// UART1 system clock control
#define REG_CLKGEN_UARTEN_UART1EN_S               8
#define REG_CLKGEN_UARTEN_UART1EN_M               0x00000300
#define REG_CLKGEN_UARTEN_UART1EN(n)              (((uint32_t)(n) << 8) & 0x00000300)
#define REG_CLKGEN_UARTEN_UART1EN_DIS             0x00000000
#define REG_CLKGEN_UARTEN_UART1EN_EN              0x00000100
#define REG_CLKGEN_UARTEN_UART1EN_REDUCE_FREQ     0x00000200
#define REG_CLKGEN_UARTEN_UART1EN_EN_POWER_SAV    0x00000300

// UART0 system clock control
#define REG_CLKGEN_UARTEN_UART0EN_S               0
#define REG_CLKGEN_UARTEN_UART0EN_M               0x00000003
#define REG_CLKGEN_UARTEN_UART0EN(n)              (((uint32_t)(n) << 0) & 0x00000003)
#define REG_CLKGEN_UARTEN_UART0EN_DIS             0x00000000
#define REG_CLKGEN_UARTEN_UART0EN_EN              0x00000001
#define REG_CLKGEN_UARTEN_UART0EN_REDUCE_FREQ     0x00000002
#define REG_CLKGEN_UARTEN_UART0EN_EN_POWER_SAV    0x00000003


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    reg32_t CALXT;
    reg32_t CALRC;
    reg32_t ACALCTR;
    reg32_t OCTRL;
    reg32_t CLKOUT;
    reg32_t CLKKEY;
        
    reg32_t CCTRL;
    reg32_t STATUS;
    reg32_t HFADJ;
    reg32_t HFVAL;
    reg32_t CLOCKEN;
    reg32_t CLOCKEN2;
    reg32_t CLOCKEN3;
    reg32_t UARTEN;
    REG32_RSVD(50)
    
    reg32_t INTEN;
    reg32_t INTSTAT;
    reg32_t INTCLR;
    reg32_t INTSET;
} reg_clkgen_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
