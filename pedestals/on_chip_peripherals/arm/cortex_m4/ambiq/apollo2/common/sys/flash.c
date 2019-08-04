/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_cachectrl.h"
#include ".\pm.h"
#include ".\flash.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
//
// Look-up table
//
const i_flash_t FLASH_OPT = {
    //
    // The basics.
    //
    // flash_mass_erase()
    ((int  (*)(uint32_t, uint32_t))                                 0x0800004d),
    // flash_page_erase()
    ((int  (*)(uint32_t, uint32_t, uint32_t))                       0x08000051),
    // flash_program_main()
    ((int  (*)(uint32_t, uint32_t *, uint32_t *, uint32_t))         0x08000055),
    // flash_program_info()
    ((int  (*)(uint32_t, uint32_t, uint32_t *, uint32_t, uint32_t)) 0x08000059),

    //
    // Non-blocking variants, but be careful these are not interrupt safe so
    // mask interrupts while these very long operations proceed.
    //
    // flash_mass_erase_nb()
    ((int   (*)(uint32_t, uint32_t))                                0x0800006d),
    // flash_page_erase_nb()
    ((int   (*)(uint32_t, uint32_t, uint32_t))                      0x08000071),
    // flash_nb_operation_complete()
    ((bool  (*)(void))                                              0x0800007d),

    //
    // Essentially these are recovery options.
    //
    // flash_erase_info()
    ((int   (*)(uint32_t, uint32_t))                                0x08000081),
    // flash_erase_main_plus_info()
    ((int   (*)(uint32_t, uint32_t))                                0x08000089),
    // flash_erase_main_plus_info_both_instances()
    ((int   (*)(uint32_t))                                          0x08000091),
    // flash_recovery()
    ((void  (*)(uint32_t))                                          0x08000099),

    //
    // Useful utilities.
    //
    // flash_util_read_word()
    ((uint32_t (*)(uint32_t*))                                      0x08000075),
    // flash_util_write_word()
    ((void     (*)(uint32_t*, uint32_t))                            0x08000079),
    // delay_cycles()
    ((void     (*)(uint32_t))                                       0x0800009d),

    //
    // The following functions pointers must never be called from user
    // programs. They are here primarily to document these entry points
    // which are usable from a debugger or debugger script.
    //
    // flash_program_main_sram()
    ((void  (*)(void))                                              0x0800005d),
    // flash_program_info_sram()
    ((void  (*)(void))                                              0x08000061),
    // flash_erase_main_pages_sram()
    ((void  (*)(void))                                              0x08000065),
    // flash_mass_erase_sram()
    ((void  (*)(void))                                              0x08000069),
    // flash_erase_info_sram()
    ((void  (*)(void))                                              0x08000085),
    // flash_erase_main_plus_info_sram()
    ((void  (*)(void))                                              0x0800008d)
};

/*============================ IMPLEMENTATION ================================*/
//*****************************************************************************
//
// Default settings for the cache.
//
//*****************************************************************************
const driver_cachectrl_config_t driverCachectrlDefaults = {
    .EnableCache = 1,
    .LRU = 0,
    .EnableNCregions = 0,
    .Config = REG_BFVS( CACHECTRL, CACHECFG, CONFIG, 5 ),
    .SerialCacheMode = 0,
    .FlashCachingEnables = 3,
    .EnableCacheClockGating = 1,
    .EnableLightSleep = 0,
    .Dly = 1,
    .SMDly = 1,
    .EnableDataClockGating = 1,
    .EnableCacheMonitoring = 0,
};

//*****************************************************************************
//
//! @brief Enable the cache using the supplied settings
//!
//! @param psConfig - pointer to a config structure containing cache settings.
//!
//! This function takes in a structure of cache settings, and uses them to
//! enable the cache. This function will take care of the necessary register
//! writes both in this module and in the power control module, so a separate
//! powerctrl call is not necessary.
//!
//! For most applications, the default cache settings will be the most
//! efficient choice. To use the default cache settings with this function, use
//! the address of the global driverCachectrlDefaults structure as the
//! psConfig argument.
//!
//! @return None.
//
//*****************************************************************************
void driver_cachectrl_enable(const driver_cachectrl_config_t *psConfig)
{
    uint32_t ui32ConfigValue;
    uint32_t ui32Timeout;

    //
    // Pull the configuration data from the structure, and prepare to write the
    // cache configuration register.
    //
    // NOTE: ICACHE and DCACHE settings were left out from this step. This is a
    // workaround for a timing issue with early versions of Apollo2 that caused
    // the cache to incorrectly mark itself valid during the startup sequence.
    // The workaround calls for us to start the cache, manually invalidate it,
    // and then enable ICACHE and DCACHE operation.
    //
    ui32ConfigValue = (  BIT(0)                                                                 |
                       ( ((uint32_t)psConfig->LRU << 1) & BIT(1) )                              |
                       ( ((uint32_t)( (psConfig->EnableNCregions & 0x1) >> 0 ) << 2) & BIT(2) ) |
                       ( ((uint32_t)( (psConfig->EnableNCregions & 0x2) >> 1 ) << 3) & BIT(3) ) |
                       psConfig->Config                                                         |
                       ( ((uint32_t)psConfig->SerialCacheMode << 7) & BIT(7) )                  |
                       ( ((uint32_t)psConfig->EnableCacheClockGating << 10) & BIT(10) )         |
                       ( ((uint32_t)psConfig->EnableLightSleep << 11) & BIT(11) )               |
                       ( ((uint32_t)psConfig->Dly << 12) & BITMASK(15, 12) )                    |
                       ( ((uint32_t)psConfig->SMDly << 16) & BITMASK(19, 16) )                  |
                       ( ((uint32_t)psConfig->EnableDataClockGating << 20) & BIT(20) )          |
                       ( ((uint32_t)psConfig->EnableCacheMonitoring << 24) & BIT(24) ));

    //
    // Make sure the cache is enabled in the power control block.
    //
    driver_pwrctrl_memory_enable(DRIVER_PWRCTRL_MEMEN_CACHE);

    //
    // Set the initial cache settings.
    //
    REG_CACHECTRL.CACHECFG = ui32ConfigValue;

    //
    // Wait for the cache ready signal.
    //
    for (ui32Timeout = 0; ui32Timeout < 50; ui32Timeout++) {
        if (REG_BFM(CACHECTRL, CACHECTRL, CACHE_READY)) {
            break;
        }
    }

    //
    // Manually invalidate the cache (workaround for the issue described above.)
    //
    REG_BFW( CACHECTRL, CACHECTRL, INVALIDATE, 1 );

    //
    // Wait for the cache ready signal again.
    //
    for (ui32Timeout = 0; ui32Timeout < 50; ui32Timeout++) {
        if (REG_BFM(CACHECTRL, CACHECTRL, CACHE_READY)) {
            break;
        }
    }

    //
    // Now that the cache is running, and correctly marked invalid, we can OR in
    // the ICACHE and DCACHE settings.
    //
    ui32ConfigValue |= (REG_BFVS( CACHECTRL, CACHECFG, ICACHE_ENABLE, (psConfig->FlashCachingEnables & 0x1) >> 0 )   |
                        REG_BFVS( CACHECTRL, CACHECFG, DCACHE_ENABLE, (psConfig->FlashCachingEnables & 0x2) >> 1 ) );

    //
    // Write the final configuration settings to the CACHECTRL register.
    //
    REG_CACHECTRL.CACHECFG = ui32ConfigValue;
}

//*****************************************************************************
//
//! @brief Disable the cache.
//!
//! Call this function to completely shut down cache features.
//!
//! @return None.
//
//*****************************************************************************
void driver_cachectrl_disable(void)
{
    uint32_t ui32CacheCfg;

    //
    // Save the cache settings.
    //
    ui32CacheCfg = REG_CACHECTRL.CACHECFG;

    //
    // Remove the ICACHE and DCACHE settings.
    //
    ui32CacheCfg &= ( REG_BFVS( CACHECTRL, CACHECFG, DCACHE_ENABLE, 0 ) |
                      REG_BFVS( CACHECTRL, CACHECFG, ICACHE_ENABLE, 0 ) );

    //
    // Write the resulting value back to the register.
    //
    REG_CACHECTRL.CACHECFG = ui32CacheCfg;

    //
    // Read the CACHECTRL register a few times
    //
    REG_CACHECTRL.CACHECTRL;
    REG_CACHECTRL.CACHECTRL;
    REG_CACHECTRL.CACHECTRL;

    //
    // Disable the cache completely.
    //
    REG_BFW( CACHECTRL, CACHECFG, ENABLE, 0 );

    //
    // Power the cache down in the powerctrl block.
    //
    driver_pwrctrl_memory_enable(DRIVER_PWRCTRL_MEMEN_CACHE_DIS);
}

//*****************************************************************************
//
//! @brief Set a default cache configuration.
//!
//! This function is used to set a default cache configuration.
//
//*****************************************************************************
void driver_cachectrl_config_default(void)
{
    //
    // Set PWRCTRL
    //
    driver_pwrctrl_memory_enable(DRIVER_PWRCTRL_MEMEN_CACHE);

    //
    // Write a default configuration to the CACHECFG register.
    //
    REG_CACHECTRL.CACHECFG =                                                 \
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE, 1 )                   |   \
        REG_BFVS( CACHECTRL, CACHECFG, LRU, 0 )                      |   \
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_NC0, 0 )               |   \
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_NC1, 0 )               |   \
        REG_BFVS( CACHECTRL, CACHECFG, CONFIG, 5)                    |   \
        REG_BFVS( CACHECTRL, CACHECFG, SERIAL, 0 )                   |   \
        REG_BFVS( CACHECTRL, CACHECFG, ICACHE_ENABLE, 1 )            |   \
        REG_BFVS( CACHECTRL, CACHECFG, DCACHE_ENABLE, 1 )            |   \
        REG_BFVS( CACHECTRL, CACHECFG, CACHE_CLKGATE, 1 )            |   \
        REG_BFVS( CACHECTRL, CACHECFG, CACHE_LS, 0 )                 |   \
        REG_BFVS( CACHECTRL, CACHECFG, DLY, 1 )                      |   \
        REG_BFVS( CACHECTRL, CACHECFG, SMDLY, 1 )                    |   \
        REG_BFVS( CACHECTRL, CACHECFG, DATA_CLKGATE, 1 )             |   \
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_MONITOR, 0 );

    //
    // Write a default configuration to the FLASHCFG register.
    //
    REG_BFW( CACHECTRL, FLASHCFG, RD_WAIT, 1 );

    //
    // Write a default configuration to the CACHECTRL register.
    //
    REG_CACHECTRL.CACHECTRL =                                                  \
        REG_BFVS( CACHECTRL, CACHECTRL, FLASH1_SLM_ENABLE,  1 )        |   \
        REG_BFVS( CACHECTRL, CACHECTRL, FLASH1_SLM_DISABLE, 0 )        |   \
        REG_BFVS( CACHECTRL, CACHECTRL, FLASH0_SLM_ENABLE,  1 )        |   \
        REG_BFVS( CACHECTRL, CACHECTRL, FLASH0_SLM_DISABLE, 0 )        |   \
        REG_BFVS( CACHECTRL, CACHECTRL, RESET_STAT, 0 )                |   \
        REG_BFVS( CACHECTRL, CACHECTRL, INVALIDATE, 0 );

    //
    // Write a default configuration to the NCR0START and NCR0END registers.
    //
    REG_BFW( CACHECTRL, NCR0START, ADDR, 0 );
    REG_BFW( CACHECTRL, NCR0END,   ADDR, 0 );

    //
    // Write a default configuration to the NCR1START and NCR1END registers.
    //
    REG_BFW( CACHECTRL, NCR1START, ADDR, 0 );
    REG_BFW( CACHECTRL, NCR1END,   ADDR, 0 );

    //
    // Write a default configuration to the DMONn and IMONn registers.
    //
    REG_BFW( CACHECTRL, DMON0, DACCESS_COUNT,   0 );
    REG_BFW( CACHECTRL, DMON1, DLOOKUP_COUNT,   0 );
    REG_BFW( CACHECTRL, DMON2, DHIT_COUNT,      0 );
    REG_BFW( CACHECTRL, DMON3, DLINE_COUNT,     0 );
    REG_BFW( CACHECTRL, IMON0, IACCESS_COUNT,   0 );
    REG_BFW( CACHECTRL, IMON1, ILOOKUP_COUNT,   0 );
    REG_BFW( CACHECTRL, IMON2, IHIT_COUNT,      0 );
    REG_BFW( CACHECTRL, IMON3, ILINE_COUNT,     0 );
}

//*****************************************************************************
//
//! @brief Enable the flash cache controller via a configuration structure.
//!
//! @param psConfig - Pointer to a data structure containing all of the data
//      necessary to configure the CACHECFG register.
//!
//! This function is used to configure all fields of the CACHECFG.
//
//*****************************************************************************
void driver_cachectrl_config(driver_cachectrl_config_t *psConfig)
{
    uint32_t u32ConfigValue;

    //
    // Arrange all of the members of the data structure into a single u32 that
    //  can be written to the register.
    //
    u32ConfigValue =
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE, psConfig->EnableCache )       |
        REG_BFVS( CACHECTRL, CACHECFG, LRU,    psConfig->LRU )                  |
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_NC0,
            (psConfig->EnableNCregions & 0x1) >> 0 )                    |
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_NC1,
            (psConfig->EnableNCregions & 0x2) >> 1 )                    |
        psConfig->Config                                                |
        REG_BFVS( CACHECTRL, CACHECFG, SERIAL,psConfig->SerialCacheMode)     |
        REG_BFVS( CACHECTRL, CACHECFG, ICACHE_ENABLE,
            (psConfig->FlashCachingEnables & 0x1) >> 0 )                |
        REG_BFVS( CACHECTRL, CACHECFG, DCACHE_ENABLE,
            (psConfig->FlashCachingEnables & 0x2) >> 1 )                |
        REG_BFVS( CACHECTRL, CACHECFG, CACHE_CLKGATE,
            psConfig->EnableCacheClockGating )                          |
        REG_BFVS( CACHECTRL, CACHECFG, CACHE_LS,
            psConfig->EnableLightSleep )                                |
        REG_BFVS( CACHECTRL, CACHECFG, DLY, psConfig->Dly )                  |
        REG_BFVS( CACHECTRL, CACHECFG, SMDLY, psConfig->SMDly )              |
        REG_BFVS( CACHECTRL, CACHECFG, DATA_CLKGATE,
            psConfig->EnableDataClockGating )                           |
        REG_BFVS( CACHECTRL, CACHECFG, ENABLE_MONITOR,
            psConfig->EnableCacheMonitoring );

    //
    // Write the configuration value to the CACHECFG register.
    //
    REG_CACHECTRL.CACHECFG = u32ConfigValue;
}

//*****************************************************************************
//
//! @brief Configure the various flash cache controller enables.
//!
//! @param u32EnableMask  - Mask of features to be enabled.
//! @param u32DisableMask - Mask of features to be disabled.
//!
//! This function is used to enable or disable the various flash cache
//! controller configuration enables which consist of the following:
//!     AM_HAL_CACHECTRL_CACHECFG_ENABLE                Flash cache controller
//!     AM_HAL_CACHECTRL_CACHECFG_LRU_ENABLE            LRU (disabled = LRR)
//!     AM_HAL_CACHECTRL_CACHECFG_NC0_ENABLE            Non-cacheable region 0
//!     AM_HAL_CACHECTRL_CACHECFG_NC1_ENABLE            Non-cacheable region 1
//!     AM_HAL_CACHECTRL_CACHECFG_SERIAL_ENABLE         Serial cache mode
//!     AM_HAL_CACHECTRL_CACHECFG_ICACHE_ENABLE         Instruction caching
//!     AM_HAL_CACHECTRL_CACHECFG_DCACHE_ENABLE         Data caching.
//!     AM_HAL_CACHECTRL_CACHECFG_CACHE_CLKGATE_ENABLE  Cache clock gating
//!     AM_HAL_CACHECTRL_CACHECFG_LS_ENABLE             Light sleep cache RAMs
//!     AM_HAL_CACHECTRL_CACHECFG_DATA_CLKGATE_ENABLE   Data clock gating
//!     AM_HAL_CACHECTRL_CACHECFG_MONITOR_ENABLE        Cache Monitoring Stats
//!
//! Note that if both an enable and disable are provided in their respective
//! masks, the enable will take precendence.
//!
//! @return The previous status of the flash cache controller enables.
//
//*****************************************************************************
#define CACHECTRL_VALID_ENABLES                     (    \
        REG_CACHECTRL_CACHECFG_ENABLE_M              |   \
        REG_CACHECTRL_CACHECFG_LRU_M                 |   \
        REG_CACHECTRL_CACHECFG_ENABLE_NC0_M          |   \
        REG_CACHECTRL_CACHECFG_ENABLE_NC1_M          |   \
        REG_CACHECTRL_CACHECFG_SERIAL_M              |   \
        REG_CACHECTRL_CACHECFG_ICACHE_ENABLE_M       |   \
        REG_CACHECTRL_CACHECFG_DCACHE_ENABLE_M       |   \
        REG_CACHECTRL_CACHECFG_CACHE_CLKGATE_M       |   \
        REG_CACHECTRL_CACHECFG_CACHE_LS_M            |   \
        REG_CACHECTRL_CACHECFG_DATA_CLKGATE_M        |   \
        REG_CACHECTRL_CACHECFG_ENABLE_MONITOR_M )

uint32_t driver_cachectrl_cache_enables(uint32_t u32EnableMask, uint32_t u32DisableMask)
{
    uint32_t ui32RetVal = REG_BFR( CACHECTRL, CACHECFG, ENABLE ) &
                          CACHECTRL_VALID_ENABLES;

    //
    // Make sure the enable masks include only valid bits.
    //
    u32EnableMask  &= CACHECTRL_VALID_ENABLES;
    u32DisableMask &= CACHECTRL_VALID_ENABLES;

    //
    // First, do the disables.
    //
    REG_CACHECTRL.CACHECFG &= ~u32DisableMask;

    //
    // Now set the enables.
    //
    REG_CACHECTRL.CACHECFG |= u32EnableMask;

    return ui32RetVal;
}

//*****************************************************************************
//
//! @brief Select the cache configuration type.
//!
//! This functions only sets the CACHECFG CONFIG field.
//!
//! @param ui32CacheConfig - The cache configuration value.
//!
//! This function can be used to select the type of cache.frequency of the main
//! system clock.  The ui32CacheConfig parameter should be set to one of the
//! following values:
//!
//!     AM_HAL_CACHECTRL_CACHECFG_CONFIG_DIRECT_256 : Direct mapped,
//!         128-bit linesize, 256 entries (2 SRAMs active).
//!     AM_HAL_CACHECTRL_CACHECFG_CONFIG_2WAY_256   : Two-way set associative,
//!         128-bit linesize, 256 entries (4 SRAMs active).
//!     AM_HAL_CACHECTRL_CACHECFG_CONFIG_2WAY_512   : Two-way set associative,
//!         128-bit linesize, 512 entries (8 SRAMs active).
//!
//! @return None.
//
//*****************************************************************************
void driver_cachectrl_cache_config(uint32_t ui32CacheConfig)
{
    //
    // Clear the bitfield
    //
    REG_BFW( CACHECTRL, CACHECFG, CONFIG, 0 );

    //
    // Write the new value to the bitfield.
    //
    REG_BFW( CACHECTRL, CACHECFG, CONFIG, ui32CacheConfig );
}

//*****************************************************************************
//
//! @brief Invalidate the flash cache.
//!
//! This function is used to invalidate the flash cache.
//!
//! @return None.
//
//*****************************************************************************
void driver_cachectrl_invalidate_flash_cache(void)
{
    //
    // Write the bit to invalidate the flash cache.
    // Note - this bit is not sticky, no need to write it back to 0.
    //
    REG_BFS( CACHECTRL, CACHECTRL, INVALIDATE );
}

//*****************************************************************************
//
//! @brief Reset cache statistics.
//!
//! This function is used to reset cache statistics.
//!
//! @return None.
//
//*****************************************************************************
void driver_cachectrl_reset_statistics(void)
{
    //
    // Write the bit to reset flash statistics.
    // Note - this bit is not sticky, no need to write it back to 0.
    //
    REG_BFS( CACHECTRL, CACHECTRL, RESET_STAT );
}

//*****************************************************************************
//
//! @brief Get flash cache sleep mode status.
//!
//! This function returns flash cache sleep mode statuses.
//!
//! @return
//!     bit0 indicates that flash0 flash sleep mode is enabled.
//!     bit1 indicates that flash1 flash sleep mode is enabled.
//
//*****************************************************************************
uint32_t driver_cachectrl_sleep_mode_status(void)
{
    uint32_t ui32Status, ui32Ret;

    //
    // Get the current sleep mode status bits.
    //
    ui32Status = REG_CACHECTRL.CACHECTRL;
    ui32Ret  = (ui32Status & REG_CACHECTRL_CACHECTRL_FLASH0_SLM_STATUS_M) >>          \
                (REG_CACHECTRL_CACHECTRL_FLASH0_SLM_STATUS_S - 0);
    ui32Ret |= (ui32Status & REG_CACHECTRL_CACHECTRL_FLASH1_SLM_STATUS_M) >>          \
                (REG_CACHECTRL_CACHECTRL_FLASH1_SLM_STATUS_S - 1);

    return ui32Ret;
}

//*****************************************************************************
//
//! @brief Enable or disable flash cache sleep mode.
//!
//! This function enables or disables flash cache sleep mode.
//! @param ui32EnableMask  - bit0 for flash0, bit1 for flash1.
//! @param ui32DisableMask - bit0 for flash0, bit1 for flash1.
//!
//! Note that if both an enable and disable are provided in their respective
//! masks, the enable will take precedence.
//!
//! @return Previous status.
//!     bit0 indicates that flash0 flash sleep mode was previously enabled.
//!     bit1 indicates that flash1 flash sleep mode was previously enabled.
//
//*****************************************************************************
uint32_t driver_cachectrl_sleep_mode_enable(uint32_t ui32EnableMask, uint32_t ui32DisableMask)
{
    uint32_t ui32Ret = driver_cachectrl_sleep_mode_status();

    if ( ui32DisableMask & 0x1 ) {
        REG_BFS( CACHECTRL, CACHECTRL, FLASH0_SLM_DISABLE );
    }

    if ( ui32DisableMask & 0x2 ) {
        REG_BFS( CACHECTRL, CACHECTRL, FLASH1_SLM_DISABLE );
    }

    if ( ui32EnableMask & 0x1 ) {
        REG_BFS( CACHECTRL, CACHECTRL, FLASH0_SLM_ENABLE );
    }

    if ( ui32EnableMask & 0x2 ) {
        REG_BFS( CACHECTRL, CACHECTRL, FLASH1_SLM_ENABLE );
    }

    return ui32Ret;
}
/* EOF */
