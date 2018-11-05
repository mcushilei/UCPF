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
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_PM_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_pwrctrl.h"
#include ".\reg_mcuctrl.h"
#include ".\reg_clkgen.h"
#include ".\pm_public.h"
#include ".\flash.h"

/*============================ MACROS ========================================*/
//*****************************************************************************
//
// Peripheral power enable and disable delays
// The delay counts are based on an internal clock that runs at half of
// HFRC. Therefore, we need to double the delay cycles.
//
//*****************************************************************************
#define DRIVER_PWRCTRL_DEVICEEN_DELAYCYCLES     (22 * 2)
#define DRIVER_PWRCTRL_DEVICEDIS_DELAYCYCLES    (22 * 2)

//
// Use the following only when enabling after sleep (not during initialization).
//
#define DRIVER_PWRCTRL_BUCKEN_DELAYCYCLES       (0 * 2)
#define DRIVER_PWRCTRL_BUCKDIS_DELAYCYCLES      (15 * 2)

//*****************************************************************************
//
// Memory PWRONSTATUS enable values for driver_pwrctrl_memory_enable()
//
//*****************************************************************************
#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_8K                  \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_16K                 \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_24K                 \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_32K                 \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_64K                 \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_96K                 \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_128K                \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_160K                \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_192K                \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP5_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_224K                \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP6_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP5_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_256K                \
           (REG_PWRCTRL_PWRONSTATUS_PD_GRP7_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP6_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP5_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP4_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP3_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP2_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP1_SRAM_M   |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM3_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM2_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM1_M  |   \
            REG_PWRCTRL_PWRONSTATUS_PD_GRP0_SRAM0_M)

#define DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL                 \
        DRIVER_PWRCTRL_PWRONSTATUS_SRAM_256K



/*============================ MACROFIED FUNCTIONS ===========================*/
#define POLL_PWRSTATUS(ui32Peripheral)                                  \
    if (1) {                                                                   \
        uint32_t ui32PwrOnStat;                                         \
                                                                        \
        if ( ui32Peripheral & BITMASK(3, 1) ) {                         \
            ui32PwrOnStat = BIT(2);                                     \
        } else if ( ui32Peripheral & BITMASK(6, 4) ) {                  \
            ui32PwrOnStat = BIT(3);                     \
        } else if ( ui32Peripheral & BITMASK(8, 7) ) {  \
            ui32PwrOnStat = BIT(1);                 \
        } else if ( ui32Peripheral & BIT(9) ) {    \
            ui32PwrOnStat = BIT(7);                 \
        } else if ( ui32Peripheral & BIT(10) ) {    \
            ui32PwrOnStat = BIT(4);                 \
        } else {                                            \
            ui32PwrOnStat = 0xFFFFFFFF;                                 \
        }                                                               \
                                                                        \
        /* */                                                           \
        /* Wait for the power control setting to take effect. */        \
        /* */                                                           \
        while ( !(REG_PWRCTRL.PWRONSTATUS & ui32PwrOnStat) );      \
    }

/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


/* Peripheral power management ----------------------------------------*/
bool driver_pwrctrl_periph_enable(uint32_t parts)
{
    SAFE_ATOM_CODE(
        REG_PWRCTRL.DEVICEEN |= parts;
    )
    //
    // Wait for the power to stablize.  Using a simple delay loop is more
    // power efficient than a polling loop.
    //
    FLASH_OPT.DelayCycles(DRIVER_PWRCTRL_DEVICEEN_DELAYCYCLES / 3);
    
    //
    // Quick check to guarantee we're good (should never be more than 1 read).
    //
    POLL_PWRSTATUS(parts);
    return true;
}

bool driver_pwrctrl_periph_disable(uint32_t parts)
{
    SAFE_ATOM_CODE(
        REG_PWRCTRL.DEVICEEN &= ~parts;
    )
    //
    // Wait for the power to stablize.  Using a simple delay loop is more
    // power efficient than a polling loop.
    //
    FLASH_OPT.DelayCycles(DRIVER_PWRCTRL_DEVICEEN_DELAYCYCLES / 3);
    return true;
}

uint32_t driver_pwrctrl_periph_status_get(uint32_t parts)
{
    return REG_PWRCTRL.DEVICEEN & parts;
}


//*****************************************************************************
//
//! @brief Enable and disable power for memory devices (SRAM, flash, cache).
//!
//! @param ui32MemEn - The memory and amount to be enabled.
//!         Must be one of the following:
//!         DRIVER_PWRCTRL_MEMEN_CACHE
//!         DRIVER_PWRCTRL_MEMEN_CACHE_DIS
//!         DRIVER_PWRCTRL_MEMEN_FLASH512K
//!         DRIVER_PWRCTRL_MEMEN_FLASH1M
//!         DRIVER_PWRCTRL_MEMEN_SRAM8K
//!         DRIVER_PWRCTRL_MEMEN_SRAM16K
//!         DRIVER_PWRCTRL_MEMEN_SRAM24K
//!         DRIVER_PWRCTRL_MEMEN_SRAM32K
//!         DRIVER_PWRCTRL_MEMEN_SRAM64K
//!         DRIVER_PWRCTRL_MEMEN_SRAM96K
//!         DRIVER_PWRCTRL_MEMEN_SRAM128K
//!         DRIVER_PWRCTRL_MEMEN_SRAM160K
//!         DRIVER_PWRCTRL_MEMEN_SRAM192K
//!         DRIVER_PWRCTRL_MEMEN_SRAM224K
//!         DRIVER_PWRCTRL_MEMEN_SRAM256K
//!         DRIVER_PWRCTRL_MEMEN_ALL (the default, power-up state)
//!
//! This function enables/disables power to provide only the given amount of
//! the type of memory specified.
//!
//! Only the type of memory specified is affected. Therefore separate calls
//! are required to affect power settings for FLASH, SRAM, or CACHE.
//!
//! Settings for zero SRAM or FLASH are not provided as device behavior under
//! either of those conditions is undefined.
//!
//! @note Unpowered memory devices may lose their configuration information.
//! This function does not save or restore peripheral configuration registers.
//!
//! @return None.
//
//*****************************************************************************
bool driver_pwrctrl_memory_enable(uint32_t ui32MemEn)
{
    uint32_t ui32MemEnMask, ui32MemDisMask;
    uint32_t ui32PwrStatEnMask, ui32PwrStatDisMask;
    int32_t  i32TOcnt;

    if        ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_FLASH512K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_FLASH0_M;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_FLASH1_M;
        ui32PwrStatEnMask  = REG_PWRCTRL_PWRONSTATUS_PD_FLAM0_M;
        ui32PwrStatDisMask = REG_PWRCTRL_PWRONSTATUS_PD_FLAM1_M;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_FLASH1M ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_FLASH0_M |
                         REG_PWRCTRL_MEMEN_FLASH1_M;
        ui32MemDisMask = 0;
        ui32PwrStatEnMask  = REG_PWRCTRL_PWRONSTATUS_PD_FLAM0_M  |
                             REG_PWRCTRL_PWRONSTATUS_PD_FLAM1_M;
        ui32PwrStatDisMask = 0;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM8K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM8K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM8K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_8K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_8K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM16K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM16K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM16K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_16K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_16K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM24K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM0       |
                         REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM1       |
                         REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM2;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~(REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM0     |
                           REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM1     |
                           REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM2);
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_24K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_24K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM32K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM32K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM32K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_32K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_32K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM64K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM64K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM64K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_64K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_64K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM96K ) {
        ui32MemEnMask  = DRIVER_PWRCTRL_MEMEN_SRAM96K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~DRIVER_PWRCTRL_MEMEN_SRAM96K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_96K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_96K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM128K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_128K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_128K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM160K ) {
        ui32MemEnMask  = DRIVER_PWRCTRL_MEMEN_SRAM160K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~DRIVER_PWRCTRL_MEMEN_SRAM160K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_160K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_160K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM192K ) {
        ui32MemEnMask  = DRIVER_PWRCTRL_MEMEN_SRAM192K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~DRIVER_PWRCTRL_MEMEN_SRAM192K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_192K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_192K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM224K ) {
        ui32MemEnMask  = DRIVER_PWRCTRL_MEMEN_SRAM224K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~DRIVER_PWRCTRL_MEMEN_SRAM224K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_224K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_224K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_SRAM256K ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_SRAMEN_SRAM256K;
        ui32MemDisMask = REG_PWRCTRL_MEMEN_SRAMEN_ALL    &
                         ~REG_PWRCTRL_MEMEN_SRAMEN_SRAM256K;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_256K;
        ui32PwrStatDisMask = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL    &
                             ~DRIVER_PWRCTRL_PWRONSTATUS_SRAM_256K;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_CACHE ) {
        ui32MemEnMask  = REG_PWRCTRL_MEMEN_CACHEB0_M    |
                         REG_PWRCTRL_MEMEN_CACHEB2_M;
        ui32MemDisMask = 0;
        ui32PwrStatEnMask  = REG_PWRCTRL_PWRONSTATUS_PD_CACHEB2_M    |
                             REG_PWRCTRL_PWRONSTATUS_PD_CACHEB0_M;
        ui32PwrStatDisMask = 0;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_CACHE_DIS ) {
        ui32MemEnMask = 0;
        ui32MemDisMask  = REG_PWRCTRL_MEMEN_CACHEB0_M   |
                          REG_PWRCTRL_MEMEN_CACHEB2_M;
        ui32PwrStatEnMask  = 0;
        ui32PwrStatDisMask = REG_PWRCTRL_PWRONSTATUS_PD_CACHEB2_M    |
                             REG_PWRCTRL_PWRONSTATUS_PD_CACHEB0_M;
    } else if ( ui32MemEn == DRIVER_PWRCTRL_MEMEN_ALL ) {
        ui32MemEnMask  = DRIVER_PWRCTRL_MEMEN_ALL;
        ui32MemDisMask = 0;
        ui32PwrStatEnMask  = DRIVER_PWRCTRL_PWRONSTATUS_SRAM_ALL;
        ui32PwrStatDisMask = 0;
    } else {
        return false;
    }

    //
    // Disable unneeded memory. If nothing to be disabled, skip to save time.
    //
    // Note that a deliberate disable step using a disable mask is taken here
    // for 2 reasons: 1) To only affect the specified type of memory, and 2)
    // To avoid inadvertently disabling any memory currently being depended on.
    //
    if ( ui32MemDisMask != 0 ) {
        REG_PWRCTRL.MEMEN &= ~ui32MemDisMask;
    }

    //
    // Enable the required memory.
    //
    if ( ui32MemEnMask != 0 ) {
        REG_PWRCTRL.MEMEN |= ui32MemEnMask;
    }

    //
    // Wait for the power to be turned on.
    // Apollo2 note - these loops typically end up taking 1 iteration.
    //
    i32TOcnt = 200;
    if ( ui32PwrStatDisMask ) {
        while ( --i32TOcnt          &&
                (REG_PWRCTRL.PWRONSTATUS & ui32PwrStatDisMask) );
    }

    if ( i32TOcnt <= 0 ) {
        return false;
    }

    i32TOcnt = 200;
    if ( ui32PwrStatEnMask ) {
        while  ( --i32TOcnt         &&
                 ( (REG_PWRCTRL.PWRONSTATUS & ui32PwrStatEnMask) 
                   != ui32PwrStatEnMask ) );
    }
    if ( i32TOcnt <= 0 ) {
        return false;
    }

    return true;
}

//*****************************************************************************
//
//! @brief Initialize the core and memory buck converters.
//!
//! This function is intended to be used for first time core and memory buck
//! converters initialization.
//!
//! @return None
//
//*****************************************************************************
void driver_pwrctrl_bucks_init(void)
{
    driver_pwrctrl_bucks_enable();

    while ( ( REG_PWRCTRL.POWERSTATUS                       &
               (REG_PWRCTRL_POWERSTATUS_COREBUCKON_M        |
                REG_PWRCTRL_POWERSTATUS_MEMBUCKON_M) )      !=
            (REG_PWRCTRL_POWERSTATUS_COREBUCKON_M           |
             REG_PWRCTRL_POWERSTATUS_MEMBUCKON_M) );

    //
    // Additional delay to make sure BUCKs are initialized.
    //
    FLASH_OPT.DelayCycles(200 / 3);
}

//*****************************************************************************
//
//! @brief Enable the core and memory buck converters.
//!
//! This function enables the core and memory buck converters.
//!
//! @return None
//
//*****************************************************************************
void driver_pwrctrl_bucks_enable(void)
{
    //
    // Check to see if the bucks are already on. If so, we can just return.
    //
    if ( REG_BFR( PWRCTRL, POWERSTATUS, COREBUCKON )   &&
         REG_BFR( PWRCTRL, POWERSTATUS, MEMBUCKON ) ) {
        return;
    }

    //
    // Enable BUCK power up
    //
    REG_PWRCTRL.SUPPLYSRC |= REG_BFVS( PWRCTRL, SUPPLYSRC, COREBUCKEN, 1 ) |
                             REG_BFVS( PWRCTRL, SUPPLYSRC, MEMBUCKEN, 1 );

    //
    // Make sure bucks are ready.
    //
    while ( ( REG_PWRCTRL.POWERSTATUS   &
              (REG_BFVS( PWRCTRL, POWERSTATUS, COREBUCKON, 1 )    |
               REG_BFVS( PWRCTRL, POWERSTATUS, MEMBUCKON,  1 )) ) !=
            (REG_BFVS( PWRCTRL, POWERSTATUS, COREBUCKON, 1 )    |
             REG_BFVS( PWRCTRL, POWERSTATUS, MEMBUCKON,  1 )) );
}

static bool isRev_ADC(void)
{
    return (MCUCTRL_REG.CHIPREV & BITMASK(7, 4)) == (0x02u << 4) ?
            true : false;
}

//*****************************************************************************
//
//! @brief Disable the core and memory buck converters.
//!
//! This function disables the core and memory buck converters.
//!
//! @return None
//
//*****************************************************************************
void driver_pwrctrl_bucks_disable(void)
{
    //
    // Check to see if the bucks are already off. If so, we can just return.
    //
    if ( REG_BFR( PWRCTRL, POWERSTATUS, COREBUCKON ) == 0u   &&
         REG_BFR( PWRCTRL, POWERSTATUS, MEMBUCKON ) == 0u ) {
        return;
    }

    //
    // Handle the special case if only the ADC is powered.
    //
    if ( isRev_ADC() &&
        (REG_PWRCTRL.DEVICEEN ==
         REG_BFVS( PWRCTRL, DEVICEEN, PWRADC, 1 )) ) {
        //
        // Set SUPPLYSRC to handle this case
        //
        REG_PWRCTRL.SUPPLYSRC &=
            REG_BFVS( PWRCTRL, SUPPLYSRC, SWITCH_LDO_IN_SLEEP, 1 )    |
            REG_BFVS( PWRCTRL, SUPPLYSRC, MEMBUCKEN,           1 );
    } else {
        //
        // Power them down
        //
        REG_BFW( PWRCTRL, SUPPLYSRC, COREBUCKEN, 0 );
        REG_BFW( PWRCTRL, SUPPLYSRC, MEMBUCKEN,  0 );
    }

    //
    // Wait until BUCKs are disabled.
    //
    FLASH_OPT.DelayCycles(DRIVER_PWRCTRL_BUCKDIS_DELAYCYCLES / 3);
}

//*****************************************************************************
//
//! @brief Misc low power initializations.
//!
//! This function performs low power initializations that aren't specifically
//! handled elsewhere.
//!
//! @return None
//
//*****************************************************************************
void driver_pwrctrl_low_power_init(void)
{
    //!
    //! For lowest power, we enable clock gating for all SRAM configuration.
    //!
    REG_PWRCTRL.SRAMCTRL |=
        REG_BFVS( PWRCTRL, SRAMCTRL, SRAM_MASTER_CLKGATE, 1 )   |
        REG_BFVS( PWRCTRL, SRAMCTRL, SRAM_CLKGATE,        1 )   |
        REG_BFVS( PWRCTRL, SRAMCTRL, SRAM_LIGHT_SLEEP,    0 );

    //!
    //! For lowest deep sleep power, make sure we stay in BUCK mode.
    //!
    REG_BFC( PWRCTRL, SUPPLYSRC, SWITCH_LDO_IN_SLEEP );
}






//*****************************************************************************
//
//! @brief Select the clock divisor for the main system clock.
//!
//! @param ui32ClockSetting - The divisor value for the system clock.
//!
//! This function can be used to select the frequency of the main system clock.
//! The \e ui32ClockSetting parameter should be set to one of the following
//! values:
//!
//!     DRIVER_CLKGEN_SYSCLK_MAX
//!     DRIVER_CLKGEN_SYSCLK_48MHZ
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_sysclk_select(uint32_t ui32ClockSetting)
{
    //
    // Unlock the clock control register.
    //
    REG_CLKGEN.CLKKEY = 0x00000047;

    //
    // Set the HFRC divisor to the required operating value.
    //
    REG_CLKGEN.CCTRL = ui32ClockSetting;

    //
    // Lock the clock configuration registers.
    //
    REG_CLKGEN.CLKKEY = 0;
}

//*****************************************************************************
//
//! @brief Get the current system clock frequency.
//!
//! This function can be used to determine the frequency of the main system
//! clock. The return value is the system clock frequency measured in hertz.
//!
//! @return System clock frequency in Hz
//
//*****************************************************************************
uint32_t driver_clkgen_sysclk_get(void)
{
    uint32_t ui32ClockSetting;

    //
    // Read the value of the clock divider.
    //
    ui32ClockSetting = REG_BFR( CLKGEN, CCTRL, CORESEL );

    switch ( ui32ClockSetting ) {
        case 0:
            return 48000000;
        case 1:
            return 24000000;
        default:
            return 0xFFFFFFFF;
    }
}

//*****************************************************************************
//
//! @brief Starts the desired oscillator(s) (OSC).
//!
//! @param ui32OscFlags oscillator(s) to start.
//!
//! This function starts the desired oscillator(s) (OSC).
//!
//! Valid values for ui32OscFlags are:
//!
//!     DRIVER_CLKGEN_OSC_LFRC
//!     DRIVER_CLKGEN_OSC_XT
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_osc_start(uint32_t ui32OscFlags)
{
    if ( ui32OscFlags & (DRIVER_CLKGEN_OSC_LFRC | DRIVER_CLKGEN_OSC_XT) )
    {
        //
        // Start the oscillator(s).
        // Note that these bits are cleared in order to enable the oscillator.
        //
        REG_CLKGEN.OCTRL &= ~ui32OscFlags;
    }
}

//*****************************************************************************
//
//! @brief Stops the desired oscillator(s) (OSC).
//!
//! @param ui32OscFlags oscillator(s) to stop.
//!
//! This function stops the desired oscillator(s) (OSC).
//!
//! Valid values for ui32OscFlags are:
//!
//!     DRIVER_CLKGEN_OSC_LFRC
//!     DRIVER_CLKGEN_OSC_XT
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_osc_stop(uint32_t ui32OscFlags)
{
    if ( ui32OscFlags & (DRIVER_CLKGEN_OSC_LFRC | DRIVER_CLKGEN_OSC_XT) )
    {
        //
        // Stop the oscillator(s).
        // Note that these bits are set in order to stop the oscillator.
        //
        REG_CLKGEN.OCTRL |= ui32OscFlags;
    }
}

//*****************************************************************************
//
//! @brief Enable UART system clock.
//!
//! This function enables or disables the UART system clock.
//!
//! @param ui32Module is 0 or 1 for Apollo2.
//! @param ui32UartEn is one of the following.
//!     DRIVER_CLKGEN_UARTEN_DIS
//!     DRIVER_CLKGEN_UARTEN_EN
//!     DRIVER_CLKGEN_UARTEN_REDUCE_FREQ
//!     DRIVER_CLKGEN_UARTEN_EN_POWER_SAV
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_uarten_set(uint32_t ui32Module, uint32_t ui32UartEn)
{
    uint32_t ui32Mask;

    if ( (ui32Module >= UART_COUNT)        ||
         (ui32UartEn > REG_CLKGEN_UARTEN_UART0EN_EN_POWER_SAV) )
    {
        return;
    }

    ui32UartEn <<= ui32Module * ( ui32Module * (REG_CLKGEN_UARTEN_UART1EN_S - REG_CLKGEN_UARTEN_UART0EN_S) );
    ui32Mask = ~(REG_CLKGEN_UARTEN_UART0EN_M << ( ui32Module * (REG_CLKGEN_UARTEN_UART1EN_S - REG_CLKGEN_UARTEN_UART0EN_S) ));

    SAFE_ATOM_CODE(

        //
        // Set the UART clock
        //
        REG_CLKGEN.UARTEN &= ui32Mask;
        REG_CLKGEN.UARTEN |= ui32UartEn;

    )
}

//*****************************************************************************
//
//! @brief Enables the clock out signal.
//!
//! @param ui32Signal desired location for the clock out signal.
//!
//! This function enables the clock out signal. See driver_clkgen.h for
//! available signals.
//!
//! e.g. DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC
//!      DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV4
//!      DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC
//!      ...
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_clkout_enable(uint32_t ui32Signal)
{
    //
    // Enable the clock out on desired signal.
    //
    REG_CLKGEN.CLKOUT = REG_CLKGEN_CLKOUT_CKEN_M | ui32Signal;
}

//*****************************************************************************
//
//! @brief Disables the clock out signal.
//!
//! This function disables the clock out signal.
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_clkout_disable(void)
{
    //
    // Disable the clock out.
    //
    REG_CLKGEN.CLKOUT = 0;
}


//*****************************************************************************
//
//! @brief Enables HFRC auto-adjustment at the specified interval.
//!
//! @param ui32Warmup - How long to give the HFRC to stabilize during each
//! calibration attempt.
//! @param ui32Frequency - How often the auto-adjustment should happen.
//!
//! This function enables HFRC auto-adjustment from an external crystal
//! oscillator even when the crystal is not normally being used.
//!
//! ui32Warmup should be one of the following values:
//!
//!     REG_CLKGEN_HFADJ_HFWARMUP_1SEC
//!     REG_CLKGEN_HFADJ_HFWARMUP_2SEC
//!
//! ui32Frequency should be one of the following values:
//!
//!     REG_CLKGEN_HFADJ_HFADJCK_4SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_16SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_32SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_64SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_128SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_256SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_512SEC
//!     REG_CLKGEN_HFADJ_HFADJCK_1024SEC
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_hfrc_adjust_enable(uint32_t ui32Warmup, uint32_t ui32Frequency)
{
    //
    // Set the HFRC Auto-adjust register for the user's chosen settings. Assume
    // that the HFRC should be calibrated to 48 MHz and that the crystal is
    // running at 32.768 kHz.
    //
    REG_CLKGEN.HFADJ =
         REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_2                      |
         ui32Warmup                                                         |
         REG_CLKGEN_HFADJ_HFXTADJ(REG_CLKGEN_HFADJ_HFXTADJ_DEFAULT)   |
         ui32Frequency                                                      |
         REG_CLKGEN_HFADJ_HFADJEN_EN;
}

//*****************************************************************************
//
//! @brief Disables HFRC auto-adjustment.
//!
//! This function disables HFRC auto-adjustment.
//!
//! @return None.
//
//*****************************************************************************
void driver_clkgen_hfrc_adjust_disable(void)
{
    //
    // Disable the clock out.
    //
    REG_CLKGEN.HFADJ =
        REG_CLKGEN_HFADJ_HFADJ_GAIN_Gain_of_1_in_2                       |
        REG_CLKGEN_HFADJ_HFWARMUP_1SEC                                   |
        REG_CLKGEN_HFADJ_HFXTADJ(REG_CLKGEN_HFADJ_HFXTADJ_DEFAULT)    |
        REG_CLKGEN_HFADJ_HFADJCK_4SEC                                    |
        REG_CLKGEN_HFADJ_HFADJEN_DIS;
}
