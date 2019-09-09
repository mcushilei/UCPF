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

#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_PWRCTRL_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_PWRCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_PWRCTRL          (*(reg_pwrctrl_t *)      REG_PWRCTRL_BASEADDR)

//*****************************************************************************
//
// PWRCTRL_SUPPLYSRC - Memory and Core Voltage Supply Source Select Register
//
//*****************************************************************************
// Switches the CORE DOMAIN from BUCK mode (if enabled) to LDO when CPU is in
// DEEP SLEEP. If all the devices are off then this does not matter and LDO (low
// power mode) is used
#define REG_PWRCTRL_SUPPLYSRC_SWITCH_LDO_IN_SLEEP_S 2
#define REG_PWRCTRL_SUPPLYSRC_SWITCH_LDO_IN_SLEEP_M 0x00000004

// Enables and Selects the Core Buck as the supply for the low-voltage power
// domain.
#define REG_PWRCTRL_SUPPLYSRC_COREBUCKEN_S        1
#define REG_PWRCTRL_SUPPLYSRC_COREBUCKEN_M        0x00000002

// Enables and select the Memory Buck as the supply for the Flash and SRAM power
// domain.
#define REG_PWRCTRL_SUPPLYSRC_MEMBUCKEN_S         0
#define REG_PWRCTRL_SUPPLYSRC_MEMBUCKEN_M         0x00000001

//*****************************************************************************
//
// PWRCTRL_POWERSTATUS - Power Status Register for MCU supplies and peripherals
//
//*****************************************************************************
// Indicates whether the Core low-voltage domain is supplied from the LDO or the
// Buck.
#define REG_PWRCTRL_POWERSTATUS_COREBUCKON_S      1
#define REG_PWRCTRL_POWERSTATUS_COREBUCKON_M      0x00000002

// Indicate whether the Memory power domain is supplied from the LDO or the
// Buck.
#define REG_PWRCTRL_POWERSTATUS_MEMBUCKON_S       0
#define REG_PWRCTRL_POWERSTATUS_MEMBUCKON_M       0x00000001

//*****************************************************************************
//
// PWRCTRL_DEVICEEN - DEVICE ENABLES for SHELBY
//
//*****************************************************************************
// Enable PDM Digital Block
#define REG_PWRCTRL_DEVICEEN_PWRPDM_S             10
#define REG_PWRCTRL_DEVICEEN_PWRPDM_M             0x00000400

// Enable ADC Digital Block
#define REG_PWRCTRL_DEVICEEN_PWRADC_S             9
#define REG_PWRCTRL_DEVICEEN_PWRADC_M             0x00000200

// Enable UART 1
#define REG_PWRCTRL_DEVICEEN_UART1_S              8
#define REG_PWRCTRL_DEVICEEN_UART1_M              0x00000100

// Enable UART 0
#define REG_PWRCTRL_DEVICEEN_UART0_S              7
#define REG_PWRCTRL_DEVICEEN_UART0_M              0x00000080

// Enable IO MASTER 5
#define REG_PWRCTRL_DEVICEEN_IO_MASTER5_S         6
#define REG_PWRCTRL_DEVICEEN_IO_MASTER5_M         0x00000040

// Enable IO MASTER 4
#define REG_PWRCTRL_DEVICEEN_IO_MASTER4_S         5
#define REG_PWRCTRL_DEVICEEN_IO_MASTER4_M         0x00000020

// Enable IO MASTER 3
#define REG_PWRCTRL_DEVICEEN_IO_MASTER3_S         4
#define REG_PWRCTRL_DEVICEEN_IO_MASTER3_M         0x00000010

// Enable IO MASTER 2
#define REG_PWRCTRL_DEVICEEN_IO_MASTER2_S         3
#define REG_PWRCTRL_DEVICEEN_IO_MASTER2_M         0x00000008

// Enable IO MASTER 1
#define REG_PWRCTRL_DEVICEEN_IO_MASTER1_S         2
#define REG_PWRCTRL_DEVICEEN_IO_MASTER1_M         0x00000004

// Enable IO MASTER 0
#define REG_PWRCTRL_DEVICEEN_IO_MASTER0_S         1
#define REG_PWRCTRL_DEVICEEN_IO_MASTER0_M         0x00000002

// Enable IO SLAVE
#define REG_PWRCTRL_DEVICEEN_IO_SLAVE_S           0
#define REG_PWRCTRL_DEVICEEN_IO_SLAVE_M           0x00000001

//*****************************************************************************
//
// PWRCTRL_SRAMPWDINSLEEP - Powerdown an SRAM Banks in Deep Sleep mode
//
//*****************************************************************************
// Enable CACHE BANKS to power down in deep sleep
#define REG_PWRCTRL_SRAMPWDINSLEEP_CACHE_PWD_SLP_S 31
#define REG_PWRCTRL_SRAMPWDINSLEEP_CACHE_PWD_SLP_M 0x80000000

// Selects which SRAM banks are powered down in deep sleep mode, causing the
// contents of the bank to be lost.
#define REG_PWRCTRL_SRAMPWDINSLEEP_SRAMSLEEPPOWERDOWN_S 0
#define REG_PWRCTRL_SRAMPWDINSLEEP_SRAMSLEEPPOWERDOWN_M 0x000007FF

//*****************************************************************************
//
// PWRCTRL_MEMEN - Disables individual banks of the MEMORY array
//
//*****************************************************************************
// Enable CACHE BANK 2
#define REG_PWRCTRL_MEMEN_CACHEB2_S               31
#define REG_PWRCTRL_MEMEN_CACHEB2_M               0x80000000

// Enable CACHE BANK 0
#define REG_PWRCTRL_MEMEN_CACHEB0_S               29
#define REG_PWRCTRL_MEMEN_CACHEB0_M               0x20000000

// Enable FLASH1
#define REG_PWRCTRL_MEMEN_FLASH1_S                12
#define REG_PWRCTRL_MEMEN_FLASH1_M                0x00001000

// Enable FLASH 0
#define REG_PWRCTRL_MEMEN_FLASH0_S                11
#define REG_PWRCTRL_MEMEN_FLASH0_M                0x00000800

// Enables power for selected SRAM banks (else an access to its address space to
// generate a Hard Fault).
#define REG_PWRCTRL_MEMEN_SRAMEN_S                0
#define REG_PWRCTRL_MEMEN_SRAMEN_M                0x000007FF

//*****************************************************************************
//
// PWRCTRL_PWRONSTATUS - POWER ON Status
//
//*****************************************************************************
// This bit is 1 if power is supplied to CACHE BANK 2
#define REG_PWRCTRL_PWRONSTATUS_PD_CACHEB2_S      21
#define REG_PWRCTRL_PWRONSTATUS_PD_CACHEB2_M      0x00200000

// This bit is 1 if power is supplied to CACHE BANK 0
#define REG_PWRCTRL_PWRONSTATUS_PD_CACHEB0_S      19
#define REG_PWRCTRL_PWRONSTATUS_PD_CACHEB0_M      0x00080000

// This bit is 1 if power is supplied to SRAM domain PD_GRP7
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP7_SRAM_S    18
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP7_SRAM_M    0x00040000

// This bit is 1 if power is supplied to SRAM domain PD_GRP6
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP6_SRAM_S    17
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP6_SRAM_M    0x00020000

// This bit is 1 if power is supplied to SRAM domain PD_GRP5
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP5_SRAM_S    16
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP5_SRAM_M    0x00010000

// This bit is 1 if power is supplied to SRAM domain PD_GRP4
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_S    15
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_M    0x00008000

// This bit is 1 if power is supplied to SRAM domain PD_GRP3
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_S    14
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M    0x00004000

// This bit is 1 if power is supplied to SRAM domain PD_GRP2
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_S    13
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M    0x00002000

// This bit is 1 if power is supplied to SRAM domain PD_GRP1
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_S    12
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M    0x00001000

// This bit is 1 if power is supplied to SRAM domain PD_SRAM0_3
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_S   11
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M   0x00000800

// This bit is 1 if power is supplied to SRAM domain PD_SRAM0_2
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_S   10
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M   0x00000400

// This bit is 1 if power is supplied to SRAM domain SRAM0_1
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_S   9
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M   0x00000200

// This bit is 1 if power is supplied to SRAM domain SRAM0_0
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_S   8
#define REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M   0x00000100

// This bit is 1 if power is supplied to domain PD_ADC
#define REG_PWRCTRL_PWRONSTATUS_PDADC_S           7
#define REG_PWRCTRL_PWRONSTATUS_PDADC_M           0x00000080

// This bit is 1 if power is supplied to domain PD_FLAM1
#define REG_PWRCTRL_PWRONSTATUS_PD_FLAM1_S        6
#define REG_PWRCTRL_PWRONSTATUS_PD_FLAM1_M        0x00000040

// This bit is 1 if power is supplied to domain PD_FLAM0
#define REG_PWRCTRL_PWRONSTATUS_PD_FLAM0_S        5
#define REG_PWRCTRL_PWRONSTATUS_PD_FLAM0_M        0x00000020

// This bit is 1 if power is supplied to domain PD_PDM
#define REG_PWRCTRL_PWRONSTATUS_PD_PDM_S          4
#define REG_PWRCTRL_PWRONSTATUS_PD_PDM_M          0x00000010

// This bit is 1 if power is supplied to power domain C, which supplies IOM3-5.
#define REG_PWRCTRL_PWRONSTATUS_PDC_S             3
#define REG_PWRCTRL_PWRONSTATUS_PDC_M             0x00000008

// This bit is 1 if power is supplied to power domain B, which supplies IOM0-2.
#define REG_PWRCTRL_PWRONSTATUS_PDB_S             2
#define REG_PWRCTRL_PWRONSTATUS_PDB_M             0x00000004

// This bit is 1 if power is supplied to power domain A, which supplies IOS and
// UART0,1.
#define REG_PWRCTRL_PWRONSTATUS_PDA_S             1
#define REG_PWRCTRL_PWRONSTATUS_PDA_M             0x00000002

//*****************************************************************************
//
// PWRCTRL_SRAMCTRL - SRAM Control register
//
//*****************************************************************************
// Enables top-level clock gating in the SRAM block.  This bit should be enabled
// for lowest power operation.
#define REG_PWRCTRL_SRAMCTRL_SRAM_MASTER_CLKGATE_S 2
#define REG_PWRCTRL_SRAMCTRL_SRAM_MASTER_CLKGATE_M 0x00000004

// Enables individual per-RAM clock gating in the SRAM block.  This bit should
// be enabled for lowest power operation.
#define REG_PWRCTRL_SRAMCTRL_SRAM_CLKGATE_S       1
#define REG_PWRCTRL_SRAMCTRL_SRAM_CLKGATE_M       0x00000002

// Enable LS (light sleep) of cache RAMs.  When this bit is set, the RAMS will
// be put into light sleep mode while inactive.  NOTE:  if the SRAM is actively
// used, this may have an adverse affect on power since entering/exiting LS mode
// may consume more power than would be saved.
#define REG_PWRCTRL_SRAMCTRL_SRAM_LIGHT_SLEEP_S   0
#define REG_PWRCTRL_SRAMCTRL_SRAM_LIGHT_SLEEP_M   0x00000001

//*****************************************************************************
//
// PWRCTRL_ADCSTATUS - Power Status Register for ADC Block
//
//*****************************************************************************
// This bit indicates that the ADC REFBUF is powered down
#define REG_PWRCTRL_ADCSTATUS_ADC_REFBUF_PWD_S    5
#define REG_PWRCTRL_ADCSTATUS_ADC_REFBUF_PWD_M    0x00000020

// This bit indicates that the ADC REFKEEP is powered down
#define REG_PWRCTRL_ADCSTATUS_ADC_REFKEEP_PWD_S   4
#define REG_PWRCTRL_ADCSTATUS_ADC_REFKEEP_PWD_M   0x00000010

// This bit indicates that the ADC VBAT resistor divider is powered down
#define REG_PWRCTRL_ADCSTATUS_ADC_VBAT_PWD_S      3
#define REG_PWRCTRL_ADCSTATUS_ADC_VBAT_PWD_M      0x00000008

// This bit indicates that the ADC temperature sensor input buffer is powered
// down
#define REG_PWRCTRL_ADCSTATUS_ADC_VPTAT_PWD_S     2
#define REG_PWRCTRL_ADCSTATUS_ADC_VPTAT_PWD_M     0x00000004

// This bit indicates that the ADC Band Gap is powered down
#define REG_PWRCTRL_ADCSTATUS_ADC_BGT_PWD_S       1
#define REG_PWRCTRL_ADCSTATUS_ADC_BGT_PWD_M       0x00000002

// This bit indicates that the ADC is powered down
#define REG_PWRCTRL_ADCSTATUS_ADC_PWD_S           0
#define REG_PWRCTRL_ADCSTATUS_ADC_PWD_M           0x00000001

//*****************************************************************************
//
// PWRCTRL_MISCOPT - Power Optimization Control Bits
//
//*****************************************************************************
// Setting this bit will enable the MEM LDO to be in LPMODE during deep sleep
// even when the ctimers or stimers are running
#define REG_PWRCTRL_MISCOPT_DIS_LDOLPMODE_TIMERS_S 2
#define REG_PWRCTRL_MISCOPT_DIS_LDOLPMODE_TIMERS_M 0x00000004


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    reg32_t SUPPLYSRC;
    reg32_t POWERSTATUS;
    reg32_t DEVICEEN;
    reg32_t SRAMPWDINSLEEP;
    reg32_t MEMEN;
    reg32_t PWRONSTATUS;
    reg32_t SRAMCTRL;
    reg32_t ADCSTATUS;
    reg32_t MISCOPT;
} reg_pwrctrl_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
