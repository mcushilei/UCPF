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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_FLASH_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_FLASH_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//*****************************************************************************
//
// Structure of function pointers to helper functions for invoking various
// flash operations. The functions we are pointing to here are in the Apollo 2
// integrated BOOTROM.
//
//*****************************************************************************
typedef struct am_hal_flash_helper_struct {
    //
    // The basics.
    //
    int      (*MassErase)(uint32_t, uint32_t);
    int      (*PageErase)(uint32_t, uint32_t, uint32_t);
    int      (*ProgramMain)(uint32_t,  uint32_t *,
                                          uint32_t*, uint32_t);
    int      (*ProgramInfo)(uint32_t,   uint32_t,
                                         uint32_t*,  uint32_t, uint32_t);

    //
    // Non-blocking variants, but be careful these are not interrupt safe so
    // mask interrupts while these very long operations proceed.
    //
    int      (*MassEraseNB)(uint32_t, uint32_t);
    int      (*PageEraseNB)(uint32_t, uint32_t, uint32_t);
    bool     (*NBOperationComplete)(void);

    //
    // Essentially these are recovery options.
    //
    int      (*EraseInfo)(uint32_t, uint32_t);
    int      (*EraseMainPlusInfo)(uint32_t, uint32_t);
    int      (*EraseMainPlusInfoBothInstances)(uint32_t);
    void     (*Recovery)(uint32_t);

    //
    // Useful utilities.
    //
    uint32_t (*UtilReadWord)(uint32_t*);
    void     (*UtilWriteWord)(uint32_t*, uint32_t);
    void     (*DelayCycles)(uint32_t);

    //
    // The following functions pointers will generally never be called from
    // user programs. They are here primarily to document these entry points
    // which are usable from a debugger or debugger script.
    //
    void     (*ProgramMainSRAM)(void);
    void     (*ProgramInfoSRAM)(void);
    void     (*EraseMainPagesSRAM)(void);
    void     (*MassEraseSRAM)(void);
    void     (*EraseInfoSRAM)(void);
    void     (*EraseMainPlusInfoSRAM)(void);
} i_flash_t;

//*****************************************************************************
//
// Cache configuration structure
//
//*****************************************************************************
typedef struct {
    //
    //! Set to 1 to enable the cache.
    //
    uint8_t EnableCache;

    //
    //! Set to 1 to enable the LRU cache replacement policy.
    //! Set to 0 to enable the LRR (least recently used) replacement policy.
    //! LEE minimizes writes to the TAG SRAM.
    //
    uint8_t LRU;

    //
    //! Set to 3 to enable non-cachable region 1 and non-cachable region 0.
    //! Set to 2 to enable non-cachable region 1.
    //! Set to 1 to enable non-cachable region 0.
    //! Set to 0 to make all regions cacheable.
    //
    uint8_t EnableNCregions;

    //
    //! Set to:
    //! AM_HAL_CACHECTRL_CACHECFG_CONFIG_DIRECT_256 for direct-mapped,
    //!     128-bit linesize, 256 entries (2 SRAMs active)
    //! AM_HAL_CACHECTRL_CACHECFG_CONFIG_2WAY_256 for two-way set associative,
    //!     128-bit linesize, 256 entries (4 SRAMs active)
    //! AM_HAL_CACHECTRL_CACHECFG_CONFIG_2WAY_512 for two-way set associative,
    //!     128-bit linesize, 512 entries (8 SRAMs active)
    //
    uint8_t Config;

    //
    //! Set to 1 to enable serial cache mode.
    //
    uint8_t SerialCacheMode;

    //
    //! Set to 3 to enable flash data caching and flash instruction caching.
    //! Set to 2 to enable flash data caching.
    //! Set to 1 to enable flash instruction caching.
    //! Set to 0 to disable flash data caching and flash instruction caching.
    //
    uint8_t FlashCachingEnables;

    //
    //! Set to 1 to enable clock gating of cache RAMs.
    //
    uint8_t EnableCacheClockGating;

    //
    //! Set to 1 to enable light sleep of cache RAMs.
    //
    uint8_t EnableLightSleep;

    //
    //! Set Data RAM delay value (0x0 - 0xF).
    //
    uint8_t Dly;

    //
    //! Set SM Data RAM delay value (0x0 - 0xF).
    //
    uint8_t SMDly;

    //
    //! Set to 1 to enable clock gating of the entire data array.
    //
    uint8_t EnableDataClockGating;

    //
    //! Set to 1 to enable cache monitor statistics.
    //
    uint8_t EnableCacheMonitoring;
} driver_cachectrl_config_t;



/*============================ PUBLIC VARIABLES ==============================*/
extern const driver_cachectrl_config_t driverCachectrlDefaults;
extern const i_flash_t FLASH_OPT;

/*============================ PUBLIC PROTOTYPES =============================*/
extern void driver_cachectrl_enable(const driver_cachectrl_config_t *psConfig);
extern void driver_cachectrl_disable(void);
extern void driver_cachectrl_config_default(void);
extern void driver_cachectrl_config(driver_cachectrl_config_t *psConfig);
extern uint32_t driver_cachectrl_cache_enables(uint32_t u32EnableMask, uint32_t u32DisableMask);
extern void driver_cachectrl_cache_config(uint32_t ui32CacheConfig);
extern void driver_cachectrl_invalidate_flash_cache(void);
extern void driver_cachectrl_reset_statistics(void);
extern uint32_t driver_cachectrl_sleep_mode_status(void);
extern uint32_t driver_cachectrl_sleep_mode_enable(uint32_t ui32EnableMask, uint32_t ui32DisableMask);


#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_FLASH_H__
/* EOF */
