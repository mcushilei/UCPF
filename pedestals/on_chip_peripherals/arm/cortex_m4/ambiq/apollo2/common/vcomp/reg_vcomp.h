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
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_VCOMP_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_VCOMP_H__



/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_VCOMP           (*(reg_vcomp_t *)REG_VCOMP_BASEADDR)

#define REG_VCOMPn(n)       (*(reg_vcomp_t *)(REG_VCOMP_BASEADDR + 0x00000000 * n))


//*****************************************************************************
//
// Key values.
//
//*****************************************************************************
#define REG_VCOMP_PWDKEY_KEYVAL                   0x00000037

//*****************************************************************************
//
// VCOMP_INTEN - Voltage Comparator Interrupt registers: Enable
//
//*****************************************************************************
// This bit is the vcompout high interrupt.
#define REG_VCOMP_INTEN_OUTHI_S                   1
#define REG_VCOMP_INTEN_OUTHI_M                   0x00000002
#define REG_VCOMP_INTEN_OUTHI(n)                  (((uint32_t)(n) << 1) & 0x00000002)

// This bit is the vcompout low interrupt.
#define REG_VCOMP_INTEN_OUTLOW_S                  0
#define REG_VCOMP_INTEN_OUTLOW_M                  0x00000001
#define REG_VCOMP_INTEN_OUTLOW(n)                 (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// VCOMP_INTSTAT - Voltage Comparator Interrupt registers: Status
//
//*****************************************************************************
// This bit is the vcompout high interrupt.
#define REG_VCOMP_INTSTAT_OUTHI_S                 1
#define REG_VCOMP_INTSTAT_OUTHI_M                 0x00000002
#define REG_VCOMP_INTSTAT_OUTHI(n)                (((uint32_t)(n) << 1) & 0x00000002)

// This bit is the vcompout low interrupt.
#define REG_VCOMP_INTSTAT_OUTLOW_S                0
#define REG_VCOMP_INTSTAT_OUTLOW_M                0x00000001
#define REG_VCOMP_INTSTAT_OUTLOW(n)               (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// VCOMP_INTCLR - Voltage Comparator Interrupt registers: Clear
//
//*****************************************************************************
// This bit is the vcompout high interrupt.
#define REG_VCOMP_INTCLR_OUTHI_S                  1
#define REG_VCOMP_INTCLR_OUTHI_M                  0x00000002
#define REG_VCOMP_INTCLR_OUTHI(n)                 (((uint32_t)(n) << 1) & 0x00000002)

// This bit is the vcompout low interrupt.
#define REG_VCOMP_INTCLR_OUTLOW_S                 0
#define REG_VCOMP_INTCLR_OUTLOW_M                 0x00000001
#define REG_VCOMP_INTCLR_OUTLOW(n)                (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// VCOMP_INTSET - Voltage Comparator Interrupt registers: Set
//
//*****************************************************************************
// This bit is the vcompout high interrupt.
#define REG_VCOMP_INTSET_OUTHI_S                  1
#define REG_VCOMP_INTSET_OUTHI_M                  0x00000002
#define REG_VCOMP_INTSET_OUTHI(n)                 (((uint32_t)(n) << 1) & 0x00000002)

// This bit is the vcompout low interrupt.
#define REG_VCOMP_INTSET_OUTLOW_S                 0
#define REG_VCOMP_INTSET_OUTLOW_M                 0x00000001
#define REG_VCOMP_INTSET_OUTLOW(n)                (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// VCOMP_CFG - Configuration Register
//
//*****************************************************************************
// When the reference input NSEL is set to NSEL_DAC, this bitfield selects the
// voltage level for the negative input to the comparator.
#define REG_VCOMP_CFG_LVLSEL_S                    16
#define REG_VCOMP_CFG_LVLSEL_M                    0x000F0000
#define REG_VCOMP_CFG_LVLSEL(n)                   (((uint32_t)(n) << 16) & 0x000F0000)
#define REG_VCOMP_CFG_LVLSEL_0P58V                0x00000000
#define REG_VCOMP_CFG_LVLSEL_0P77V                0x00010000
#define REG_VCOMP_CFG_LVLSEL_0P97V                0x00020000
#define REG_VCOMP_CFG_LVLSEL_1P16V                0x00030000
#define REG_VCOMP_CFG_LVLSEL_1P35V                0x00040000
#define REG_VCOMP_CFG_LVLSEL_1P55V                0x00050000
#define REG_VCOMP_CFG_LVLSEL_1P74V                0x00060000
#define REG_VCOMP_CFG_LVLSEL_1P93V                0x00070000
#define REG_VCOMP_CFG_LVLSEL_2P13V                0x00080000
#define REG_VCOMP_CFG_LVLSEL_2P32V                0x00090000
#define REG_VCOMP_CFG_LVLSEL_2P51V                0x000A0000
#define REG_VCOMP_CFG_LVLSEL_2P71V                0x000B0000
#define REG_VCOMP_CFG_LVLSEL_2P90V                0x000C0000
#define REG_VCOMP_CFG_LVLSEL_3P09V                0x000D0000
#define REG_VCOMP_CFG_LVLSEL_3P29V                0x000E0000
#define REG_VCOMP_CFG_LVLSEL_3P48V                0x000F0000

// This bitfield selects the negative input to the comparator.
#define REG_VCOMP_CFG_NSEL_S                      8
#define REG_VCOMP_CFG_NSEL_M                      0x00000300
#define REG_VCOMP_CFG_NSEL(n)                     (((uint32_t)(n) << 8) & 0x00000300)
#define REG_VCOMP_CFG_NSEL_VREFEXT1               0x00000000
#define REG_VCOMP_CFG_NSEL_VREFEXT2               0x00000100
#define REG_VCOMP_CFG_NSEL_VREFEXT3               0x00000200
#define REG_VCOMP_CFG_NSEL_DAC                    0x00000300

// This bitfield selects the positive input to the comparator.
#define REG_VCOMP_CFG_PSEL_S                      0
#define REG_VCOMP_CFG_PSEL_M                      0x00000003
#define REG_VCOMP_CFG_PSEL(n)                     (((uint32_t)(n) << 0) & 0x00000003)
#define REG_VCOMP_CFG_PSEL_VDDADJ                 0x00000000
#define REG_VCOMP_CFG_PSEL_VTEMP                  0x00000001
#define REG_VCOMP_CFG_PSEL_VEXT1                  0x00000002
#define REG_VCOMP_CFG_PSEL_VEXT2                  0x00000003

//*****************************************************************************
//
// VCOMP_STAT - Status Register
//
//*****************************************************************************
// This bit indicates the power down state of the voltage comparator.
#define REG_VCOMP_STAT_PWDSTAT_S                  1
#define REG_VCOMP_STAT_PWDSTAT_M                  0x00000002
#define REG_VCOMP_STAT_PWDSTAT(n)                 (((uint32_t)(n) << 1) & 0x00000002)
#define REG_VCOMP_STAT_PWDSTAT_POWERED_DOWN       0x00000002

// This bit is 1 if the positive input of the comparator is greater than the
// negative input.
#define REG_VCOMP_STAT_CMPOUT_S                   0
#define REG_VCOMP_STAT_CMPOUT_M                   0x00000001
#define REG_VCOMP_STAT_CMPOUT(n)                  (((uint32_t)(n) << 0) & 0x00000001)
#define REG_VCOMP_STAT_CMPOUT_VOUT_LOW            0x00000000
#define REG_VCOMP_STAT_CMPOUT_VOUT_HIGH           0x00000001


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! System Control Register File
typedef volatile struct {
    reg32_t CFG;
    reg32_t STAT;
    reg32_t PWDKEY;
    REG32_RSVD(125)
    reg32_t INTEN;
    reg32_t INTSTAT;
    reg32_t INTCLR;
    reg32_t INTSET;
} reg_vcomp_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_VCOMP_H__
/* EOF */
