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

#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_CACHECTRL_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_CACHECTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_CACHECTRL          (*(reg_cachectrl_t *)      REG_CACHECTRL_BASEADDR)

//*****************************************************************************
//
// CACHECTRL_CACHECFG - Flash Cache Control Register
//
//*****************************************************************************
// Enable Cache Monitoring Stats.  Only enable this for debug/performance
// analysis since it will consume additional power.  See IMON/DMON registers for
// data.
#define REG_CACHECTRL_CACHECFG_ENABLE_MONITOR_S   24
#define REG_CACHECTRL_CACHECFG_ENABLE_MONITOR_M   0x01000000

// Enable clock gating of entire cache data array subsystem.  This should be
// enabled for normal operation.
#define REG_CACHECTRL_CACHECFG_DATA_CLKGATE_S     20
#define REG_CACHECTRL_CACHECFG_DATA_CLKGATE_M     0x00100000

// Unused.  Should be left at default value.
#define REG_CACHECTRL_CACHECFG_SMDLY_S            16
#define REG_CACHECTRL_CACHECFG_SMDLY_M            0x000F0000

// Unused.  Should be left at default value.
#define REG_CACHECTRL_CACHECFG_DLY_S              12
#define REG_CACHECTRL_CACHECFG_DLY_M              0x0000F000

// Enable LS (light sleep) of cache RAMs.  This should not be enabled for normal
// operation.  When this bit is set, the cache's RAMS will be put into light
// sleep mode while inactive.  NOTE:  if the cache is actively used, this may
// have an adverse affect on power since entering/exiting LS mode may consume
// more power than would be saved.
#define REG_CACHECTRL_CACHECFG_CACHE_LS_S         11
#define REG_CACHECTRL_CACHECFG_CACHE_LS_M         0x00000800

// Enable clock gating of individual cache RAMs.  This bit should be enabled for
// normal operation for lowest power consumption.
#define REG_CACHECTRL_CACHECFG_CACHE_CLKGATE_S    10
#define REG_CACHECTRL_CACHECFG_CACHE_CLKGATE_M    0x00000400

// Enable Flash Data Caching.  When set to 1, all instruction accesses to flash
// will be cached.
#define REG_CACHECTRL_CACHECFG_DCACHE_ENABLE_S    9
#define REG_CACHECTRL_CACHECFG_DCACHE_ENABLE_M    0x00000200

// Enable Flash Instruction Caching.  When set to 1, all instruction accesses to
// flash will be cached.
#define REG_CACHECTRL_CACHECFG_ICACHE_ENABLE_S    8
#define REG_CACHECTRL_CACHECFG_ICACHE_ENABLE_M    0x00000100

// Bitfield should always be programmed to 0.
#define REG_CACHECTRL_CACHECFG_SERIAL_S           7
#define REG_CACHECTRL_CACHECFG_SERIAL_M           0x00000080

// Sets the cache configuration.  Only a single configuration of 0x5 is valid.
#define REG_CACHECTRL_CACHECFG_CONFIG_S           4
#define REG_CACHECTRL_CACHECFG_CONFIG_M           0x00000070

// Enable Non-cacheable region 1.  See the NCR1 registers to set the region
// boundaries and size.
#define REG_CACHECTRL_CACHECFG_ENABLE_NC1_S       3
#define REG_CACHECTRL_CACHECFG_ENABLE_NC1_M       0x00000008

// Enable Non-cacheable region 0.  See the NCR0 registers to set the region
// boundaries and size.
#define REG_CACHECTRL_CACHECFG_ENABLE_NC0_S       2
#define REG_CACHECTRL_CACHECFG_ENABLE_NC0_M       0x00000004

// Sets the cache replacement policy.  0=LRR (least recently replaced), 1=LRU
// (least recently used).  LRR minimizes writes to the TAG SRAM and is
// recommended.
#define REG_CACHECTRL_CACHECFG_LRU_S              1
#define REG_CACHECTRL_CACHECFG_LRU_M              0x00000002

// Enables the main flash cache controller logic and enables power to the cache
// RAMs.  Instruction and Data caching need to be enabled independently using
// the ICACHE_ENABLE and DCACHE_ENABLE bits.
#define REG_CACHECTRL_CACHECFG_ENABLE_S           0
#define REG_CACHECTRL_CACHECFG_ENABLE_M           0x00000001

//*****************************************************************************
//
// CACHECTRL_FLASHCFG - Flash Control Register
//
//*****************************************************************************
// Sets read waitstates for flash accesses (in clock cycles).  This should be
// left at the default value for normal flash operation.
#define REG_CACHECTRL_FLASHCFG_RD_WAIT_S          0
#define REG_CACHECTRL_FLASHCFG_RD_WAIT_M          0x00000007

//*****************************************************************************
//
// CACHECTRL_CACHECTRL - Cache Control
//
//*****************************************************************************
// Enable Flash Sleep Mode.  After writing this bit, the flash instance 1 will
// enter a low-power mode until the CPU writes the SLM_DISABLE bit or a flash
// access occurs.  Wake from SLM requires ~5us, so this should only be set if
// the flash will not be accessed for reasonably long time.
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_ENABLE_S 10
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_ENABLE_M 0x00000400

// Disable Flash Sleep Mode.  Allows CPU to manually disable SLM mode.
// Performing a flash read will also wake the array.
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_DISABLE_S 9
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_DISABLE_M 0x00000200

// Flash Sleep Mode Status.  When 1, flash instance 1 is asleep.
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_STATUS_S 8
#define REG_CACHECTRL_CACHECTRL_FLASH1_SLM_STATUS_M 0x00000100

// Enable Flash Sleep Mode.  After writing this bit, the flash instance 0 will
// enter a low-power mode until the CPU writes the SLM_DISABLE bit or a flash
// access occurs.  Wake from SLM requires ~5us, so this should only be set if
// the flash will not be accessed for reasonably long time.
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_ENABLE_S 6
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_ENABLE_M 0x00000040

// Disable Flash Sleep Mode.  Allows CPU to manually disable SLM mode.
// Performing a flash read will also wake the array.
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_DISABLE_S 5
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_DISABLE_M 0x00000020

// Flash Sleep Mode Status.  When 1, flash instance 0 is asleep.
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_STATUS_S 4
#define REG_CACHECTRL_CACHECTRL_FLASH0_SLM_STATUS_M 0x00000010

// Cache Ready Status.  A value of 1 indicates the cache is enabled and not
// processing an invalidate operation.
#define REG_CACHECTRL_CACHECTRL_CACHE_READY_S     2
#define REG_CACHECTRL_CACHECTRL_CACHE_READY_M     0x00000004

// Writing a 1 to this bitfield will reset the cache monitor statistics
// (DMON0-3, IMON0-3).  Statistic gathering can be paused/stopped by disabling
// the MONITOR_ENABLE bit in CACHECFG, which will maintain the count values
// until the stats are reset by writing this bitfield.
#define REG_CACHECTRL_CACHECTRL_RESET_STAT_S      1
#define REG_CACHECTRL_CACHECTRL_RESET_STAT_M      0x00000002

// Writing a 1 to this bitfield invalidates the flash cache contents.
#define REG_CACHECTRL_CACHECTRL_INVALIDATE_S      0
#define REG_CACHECTRL_CACHECTRL_INVALIDATE_M      0x00000001

//*****************************************************************************
//
// CACHECTRL_NCR0START - Flash Cache Noncachable Region 0 Start Address.
//
//*****************************************************************************
// Start address for non-cacheable region 0.  The physical address of the start
// of this region should be programmed to this register and must be aligned to a
// 16-byte boundary (thus the lower 4 address bits are unused).
#define REG_CACHECTRL_NCR0START_ADDR_S            4
#define REG_CACHECTRL_NCR0START_ADDR_M            0x000FFFF0

//*****************************************************************************
//
// CACHECTRL_NCR0END - Flash Cache Noncachable Region 0 End
//
//*****************************************************************************
// End address for non-cacheable region 0.  The physical address of the end of
// this region should be programmed to this register and must be aligned to a
// 16-byte boundary (thus the lower 4 address bits are unused).
#define REG_CACHECTRL_NCR0END_ADDR_S              4
#define REG_CACHECTRL_NCR0END_ADDR_M              0x000FFFF0

//*****************************************************************************
//
// CACHECTRL_NCR1START - Flash Cache Noncachable Region 1 Start
//
//*****************************************************************************
// Start address for non-cacheable region 1.  The physical address of the start
// of this region should be programmed to this register and must be aligned to a
// 16-byte boundary (thus the lower 4 address bits are unused).
#define REG_CACHECTRL_NCR1START_ADDR_S            4
#define REG_CACHECTRL_NCR1START_ADDR_M            0x000FFFF0

//*****************************************************************************
//
// CACHECTRL_NCR1END - Flash Cache Noncachable Region 1 End
//
//*****************************************************************************
// End address for non-cacheable region 1.  The physical address of the end of
// this region should be programmed to this register and must be aligned to a
// 16-byte boundary (thus the lower 4 address bits are unused).
#define REG_CACHECTRL_NCR1END_ADDR_S              4
#define REG_CACHECTRL_NCR1END_ADDR_M              0x000FFFF0

//*****************************************************************************
//
// CACHECTRL_CACHEMODE - Flash Cache Mode Register.  Used to trim
// performance/power.
//
//*****************************************************************************
// Disallow Simultaneous Data RAM reads (from 2 line hits on each bus).  Value
// should be left at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE6_S       5
#define REG_CACHECTRL_CACHEMODE_THROTTLE6_M       0x00000020

// Disallow Data RAM reads (from line hits) during lookup read ops.  Value
// should be left at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE5_S       4
#define REG_CACHECTRL_CACHEMODE_THROTTLE5_M       0x00000010

// Disallow Data RAM reads (from line hits) on tag RAM fill cycles. Value should
// be left at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE4_S       3
#define REG_CACHECTRL_CACHEMODE_THROTTLE4_M       0x00000008

// Disallow cache data RAM writes on data RAM read cycles. Value should be left
// at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE3_S       2
#define REG_CACHECTRL_CACHEMODE_THROTTLE3_M       0x00000004

// Disallow cache data RAM writes on tag RAM read cycles. Value should be left
// at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE2_S       1
#define REG_CACHECTRL_CACHEMODE_THROTTLE2_M       0x00000002

// Disallow cache data RAM writes on tag RAM fill cycles. Value should be left
// at zero for optimal performance.
#define REG_CACHECTRL_CACHEMODE_THROTTLE1_S       0
#define REG_CACHECTRL_CACHEMODE_THROTTLE1_M       0x00000001

//*****************************************************************************
//
// CACHECTRL_DMON0 - Data Cache Total Accesses
//
//*****************************************************************************
// Total accesses to data cache
#define REG_CACHECTRL_DMON0_DACCESS_COUNT_S       0
#define REG_CACHECTRL_DMON0_DACCESS_COUNT_M       0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_DMON1 - Data Cache Tag Lookups
//
//*****************************************************************************
// Total tag lookups from data cache
#define REG_CACHECTRL_DMON1_DLOOKUP_COUNT_S       0
#define REG_CACHECTRL_DMON1_DLOOKUP_COUNT_M       0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_DMON2 - Data Cache Hits
//
//*****************************************************************************
// Cache hits from lookup operations
#define REG_CACHECTRL_DMON2_DHIT_COUNT_S          0
#define REG_CACHECTRL_DMON2_DHIT_COUNT_M          0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_DMON3 - Data Cache Line Hits
//
//*****************************************************************************
// Cache hits from line cache
#define REG_CACHECTRL_DMON3_DLINE_COUNT_S         0
#define REG_CACHECTRL_DMON3_DLINE_COUNT_M         0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_IMON0 - Instruction Cache Total Accesses
//
//*****************************************************************************
// Total accesses to Instruction cache
#define REG_CACHECTRL_IMON0_IACCESS_COUNT_S       0
#define REG_CACHECTRL_IMON0_IACCESS_COUNT_M       0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_IMON1 - Instruction Cache Tag Lookups
//
//*****************************************************************************
// Total tag lookups from Instruction cache
#define REG_CACHECTRL_IMON1_ILOOKUP_COUNT_S       0
#define REG_CACHECTRL_IMON1_ILOOKUP_COUNT_M       0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_IMON2 - Instruction Cache Hits
//
//*****************************************************************************
// Cache hits from lookup operations
#define REG_CACHECTRL_IMON2_IHIT_COUNT_S          0
#define REG_CACHECTRL_IMON2_IHIT_COUNT_M          0xFFFFFFFF

//*****************************************************************************
//
// CACHECTRL_IMON3 - Instruction Cache Line Hits
//
//*****************************************************************************
// Cache hits from line cache
#define REG_CACHECTRL_IMON3_ILINE_COUNT_S         0
#define REG_CACHECTRL_IMON3_ILINE_COUNT_M         0xFFFFFFFF


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    reg32_t CACHECFG;
    reg32_t FLASHCFG;
    reg32_t CACHECTRL;
    REG32_RSVD(1)
    reg32_t NCR0START;
    reg32_t NCR0END;
    reg32_t NCR1START;
    reg32_t NCR1END;
    REG32_RSVD(4)
        
    reg32_t CACHEMODE;
    REG32_RSVD(3)
        
    reg32_t DMON0;
    reg32_t DMON1;
    reg32_t DMON2;
    reg32_t DMON3;
    reg32_t IMON0;
    reg32_t IMON1;
    reg32_t IMON2;
    reg32_t IMON3;
} reg_cachectrl_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
