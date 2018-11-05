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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_STIMER_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_STIMER_H__



/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_STIMER           (*(reg_stimer_t *)REG_STIMER_BASEADDR)

#define REG_STIMERn(n)       (*(reg_stimer_t *)(REG_STIMER_BASEADDR + 0x00000000 * n))


//*****************************************************************************
//
// STIMER_STCFG - Configuration Register
//
//*****************************************************************************
// Set this bit to one to freeze the clock input to the COUNTER register. Once
// frozen, the value can be safely written from the MCU.  Unfreeze to resume.
#define REG_STIMER_STCFG_FREEZE_S                 31
#define REG_STIMER_STCFG_FREEZE_M                 0x80000000
#define REG_STIMER_STCFG_FREEZE(n)                (((uint32_t)(n) << 31) & 0x80000000)
#define REG_STIMER_STCFG_FREEZE_THAW              0x00000000
#define REG_STIMER_STCFG_FREEZE_FREEZE            0x80000000

// Set this bit to one to clear the System Timer register.  If this bit is set
// to '1', the system timer register will stay cleared.  It needs to be set to
// '0' for the system timer to start running.
#define REG_STIMER_STCFG_CLEAR_S                  30
#define REG_STIMER_STCFG_CLEAR_M                  0x40000000
#define REG_STIMER_STCFG_CLEAR(n)                 (((uint32_t)(n) << 30) & 0x40000000)
#define REG_STIMER_STCFG_CLEAR_RUN                0x00000000
#define REG_STIMER_STCFG_CLEAR_CLEAR              0x40000000

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_H_EN_S           15
#define REG_STIMER_STCFG_COMPARE_H_EN_M           0x00008000
#define REG_STIMER_STCFG_COMPARE_H_EN(n)          (((uint32_t)(n) << 15) & 0x00008000)
#define REG_STIMER_STCFG_COMPARE_H_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_H_EN_ENABLE      0x00008000

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_G_EN_S           14
#define REG_STIMER_STCFG_COMPARE_G_EN_M           0x00004000
#define REG_STIMER_STCFG_COMPARE_G_EN(n)          (((uint32_t)(n) << 14) & 0x00004000)
#define REG_STIMER_STCFG_COMPARE_G_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_G_EN_ENABLE      0x00004000

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_F_EN_S           13
#define REG_STIMER_STCFG_COMPARE_F_EN_M           0x00002000
#define REG_STIMER_STCFG_COMPARE_F_EN(n)          (((uint32_t)(n) << 13) & 0x00002000)
#define REG_STIMER_STCFG_COMPARE_F_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_F_EN_ENABLE      0x00002000

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_E_EN_S           12
#define REG_STIMER_STCFG_COMPARE_E_EN_M           0x00001000
#define REG_STIMER_STCFG_COMPARE_E_EN(n)          (((uint32_t)(n) << 12) & 0x00001000)
#define REG_STIMER_STCFG_COMPARE_E_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_E_EN_ENABLE      0x00001000

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_D_EN_S           11
#define REG_STIMER_STCFG_COMPARE_D_EN_M           0x00000800
#define REG_STIMER_STCFG_COMPARE_D_EN(n)          (((uint32_t)(n) << 11) & 0x00000800)
#define REG_STIMER_STCFG_COMPARE_D_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_D_EN_ENABLE      0x00000800

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_C_EN_S           10
#define REG_STIMER_STCFG_COMPARE_C_EN_M           0x00000400
#define REG_STIMER_STCFG_COMPARE_C_EN(n)          (((uint32_t)(n) << 10) & 0x00000400)
#define REG_STIMER_STCFG_COMPARE_C_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_C_EN_ENABLE      0x00000400

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_B_EN_S           9
#define REG_STIMER_STCFG_COMPARE_B_EN_M           0x00000200
#define REG_STIMER_STCFG_COMPARE_B_EN(n)          (((uint32_t)(n) << 9) & 0x00000200)
#define REG_STIMER_STCFG_COMPARE_B_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_B_EN_ENABLE      0x00000200

// Selects whether compare is enabled for the corresponding SCMPR register. If
// compare is enabled, the interrupt status is set once the comparision is met.
#define REG_STIMER_STCFG_COMPARE_A_EN_S           8
#define REG_STIMER_STCFG_COMPARE_A_EN_M           0x00000100
#define REG_STIMER_STCFG_COMPARE_A_EN(n)          (((uint32_t)(n) << 8) & 0x00000100)
#define REG_STIMER_STCFG_COMPARE_A_EN_DISABLE     0x00000000
#define REG_STIMER_STCFG_COMPARE_A_EN_ENABLE      0x00000100

// Selects an appropriate clock source and divider to use for the System Timer
// clock.
#define REG_STIMER_STCFG_CLKSEL_S                 0
#define REG_STIMER_STCFG_CLKSEL_M                 0x0000000F
#define REG_STIMER_STCFG_CLKSEL(n)                (((uint32_t)(n) << 0) & 0x0000000F)
#define REG_STIMER_STCFG_CLKSEL_NOCLK             0x00000000
#define REG_STIMER_STCFG_CLKSEL_HFRC_DIV16        0x00000001
#define REG_STIMER_STCFG_CLKSEL_HFRC_DIV256       0x00000002
#define REG_STIMER_STCFG_CLKSEL_XTAL_DIV1         0x00000003
#define REG_STIMER_STCFG_CLKSEL_XTAL_DIV2         0x00000004
#define REG_STIMER_STCFG_CLKSEL_XTAL_DIV32        0x00000005
#define REG_STIMER_STCFG_CLKSEL_LFRC_DIV1         0x00000006
#define REG_STIMER_STCFG_CLKSEL_STIMER0A          0x00000007
#define REG_STIMER_STCFG_CLKSEL_STIMER0B          0x00000008

//*****************************************************************************
//
// STIMER_STTMR - System Timer Count Register
//
//*****************************************************************************
// Value of the 32-bit counter as it ticks over.
#define REG_STIMER_STTMR_VALUE_S                  0
#define REG_STIMER_STTMR_VALUE_M                  0xFFFFFFFF
#define REG_STIMER_STTMR_VALUE(n)                 (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_CAPTURE_CONTROL - Capture Control Register
//
//*****************************************************************************
// Selects whether capture is enabled for the specified capture register.
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_D_S    3
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_D_M    0x00000008
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_D(n)   (((uint32_t)(n) << 3) & 0x00000008)
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_D_DISABLE 0x00000000
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_D_ENABLE 0x00000008

// Selects whether capture is enabled for the specified capture register.
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_C_S    2
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_C_M    0x00000004
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_C(n)   (((uint32_t)(n) << 2) & 0x00000004)
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_C_DISABLE 0x00000000
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_C_ENABLE 0x00000004

// Selects whether capture is enabled for the specified capture register.
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_S    1
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_M    0x00000002
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_B(n)   (((uint32_t)(n) << 1) & 0x00000002)
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_DISABLE 0x00000000
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_ENABLE 0x00000002

// Selects whether capture is enabled for the specified capture register.
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_S    0
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_M    0x00000001
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_A(n)   (((uint32_t)(n) << 0) & 0x00000001)
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_DISABLE 0x00000000
#define REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_ENABLE 0x00000001

//*****************************************************************************
//
// STIMER_SCMPR0 - Compare Register A
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_A_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR0_VALUE_S                 0
#define REG_STIMER_SCMPR0_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR0_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR1 - Compare Register B
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_B_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR1_VALUE_S                 0
#define REG_STIMER_SCMPR1_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR1_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR2 - Compare Register C
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_C_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR2_VALUE_S                 0
#define REG_STIMER_SCMPR2_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR2_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR3 - Compare Register D
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_D_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR3_VALUE_S                 0
#define REG_STIMER_SCMPR3_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR3_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR4 - Compare Register E
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_E_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR4_VALUE_S                 0
#define REG_STIMER_SCMPR4_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR4_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR5 - Compare Register F
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_F_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR5_VALUE_S                 0
#define REG_STIMER_SCMPR5_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR5_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR6 - Compare Register G
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_G_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR6_VALUE_S                 0
#define REG_STIMER_SCMPR6_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR6_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCMPR7 - Compare Register H
//
//*****************************************************************************
// Compare this value to the value in the COUNTER register according to the
// match criterion, as selected in the COMPARE_H_EN bit in the REG_STIMER_STCGF
// register.
#define REG_STIMER_SCMPR7_VALUE_S                 0
#define REG_STIMER_SCMPR7_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCMPR7_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCAPT0 - Capture Register A
//
//*****************************************************************************
// Whenever the event is detected, the value in the COUNTER is copied into this
// register and the corresponding interrupt status bit is set.
#define REG_STIMER_SCAPT0_VALUE_S                 0
#define REG_STIMER_SCAPT0_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCAPT0_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCAPT1 - Capture Register B
//
//*****************************************************************************
// Whenever the event is detected, the value in the COUNTER is copied into this
// register and the corresponding interrupt status bit is set.
#define REG_STIMER_SCAPT1_VALUE_S                 0
#define REG_STIMER_SCAPT1_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCAPT1_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCAPT2 - Capture Register C
//
//*****************************************************************************
// Whenever the event is detected, the value in the COUNTER is copied into this
// register and the corresponding interrupt status bit is set.
#define REG_STIMER_SCAPT2_VALUE_S                 0
#define REG_STIMER_SCAPT2_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCAPT2_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SCAPT3 - Capture Register D
//
//*****************************************************************************
// Whenever the event is detected, the value in the COUNTER is copied into this
// register and the corresponding interrupt status bit is set.
#define REG_STIMER_SCAPT3_VALUE_S                 0
#define REG_STIMER_SCAPT3_VALUE_M                 0xFFFFFFFF
#define REG_STIMER_SCAPT3_VALUE(n)                (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SNVR0 - System Timer NVRAM_A Register
//
//*****************************************************************************
// Value of the 32-bit counter as it ticks over.
#define REG_STIMER_SNVR0_VALUE_S                  0
#define REG_STIMER_SNVR0_VALUE_M                  0xFFFFFFFF
#define REG_STIMER_SNVR0_VALUE(n)                 (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SNVR1 - System Timer NVRAM_B Register
//
//*****************************************************************************
// Value of the 32-bit counter as it ticks over.
#define REG_STIMER_SNVR1_VALUE_S                  0
#define REG_STIMER_SNVR1_VALUE_M                  0xFFFFFFFF
#define REG_STIMER_SNVR1_VALUE(n)                 (((uint32_t)(n) << 0) & 0xFFFFFFFF)

//*****************************************************************************
//
// STIMER_SNVR2 - System Timer NVRAM_C Register
//
//*****************************************************************************
// Value of the 32-bit counter as it ticks over.
#define REG_STIMER_SNVR2_VALUE_S                  0
#define REG_STIMER_SNVR2_VALUE_M                  0xFFFFFFFF
#define REG_STIMER_SNVR2_VALUE(n)                 (((uint32_t)(n) << 0) & 0xFFFFFFFF)



/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    REG32_RSVD(63)
    reg32_t STCFG;
    reg32_t STTMR;
    reg32_t CAPTURE_CONTROL;
    REG32_RSVD(1)
    union {
        struct {
            reg32_t SCMPR0;
            reg32_t SCMPR1;
            reg32_t SCMPR2;
            reg32_t SCMPR3;
            reg32_t SCMPR4;
            reg32_t SCMPR5;
            reg32_t SCMPR6;
            reg32_t SCMPR7;
        };
        reg32_t SCMPR[8];
    };
    REG32_RSVD(44)
    union {
        struct {
            reg32_t SCAPT0;
            reg32_t SCAPT1;
            reg32_t SCAPT2;
            reg32_t SCAPT3;
        };
        reg32_t SCAPT[4];
    };
    union {
        struct {
            reg32_t SNVR0;
            reg32_t SNVR1;
            reg32_t SNVR2;
        };
        reg32_t SNVR[3];
    };
    REG32_RSVD(65)
    reg32_t STMINTEN;
    reg32_t STMINTSTAT;
    reg32_t STMINTCLR;
    reg32_t STMINTSET;
} reg_stimer_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_STIMER_H__
/* EOF */
