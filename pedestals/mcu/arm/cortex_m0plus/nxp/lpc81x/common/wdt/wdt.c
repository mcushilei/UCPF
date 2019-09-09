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
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_WDT_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\wdt_public.h"
#include "..\sys\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                      \
    {                                                           \
        uint32_t wStatus = clock_get_status(CLOCK_WWDT);        \
        clock_enable(CLOCK_WWDT);                               \
        __VA_ARGS__                                             \
        clock_resume_status(CLOCK_WWDT, wStatus);               \
    }

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief disable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_disable(void)
{
    return false;
}

/*!\brief ensable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_enable(void)
{
    clock_enable(CLOCK_WWDT);
    return true;
}

bool watchdog_start(void)
{
    SAFE_CLK_CODE(
        SAFE_ATOM_CODE (
            WWDT_REG.MOD |= 1u << 0;
            WWDT_REG.FEED = 0x000000AA;
            WWDT_REG.FEED = 0x00000055;
        )
    )
        
    return true;
}

bool watchdog_config(const wdt_cfg_t *tCfg)
{
    wdt_cfg_t cfg = *tCfg;
    
    cfg.TimeValue      &= BITMASK(23, 0);
    cfg.WindowCmpValue &= BITMASK(23, 0);
    if (cfg.WindowCmpValue < 0x100u) {
        cfg.WindowCmpValue = 0x100u;
    }
    cfg.WarnIntCmpValue &= BITMASK(9, 0);
    
#ifdef __DEBUG__
    cfg.Mode & ~(1u << 1);      //! to case interrupt but not reset.
#endif
    
    SAFE_CLK_CODE(
        WWDT_REG.MOD     = cfg.Mode;
        WWDT_REG.TC      = cfg.TimeValue;
        WWDT_REG.WARNINT = cfg.WarnIntCmpValue;
        WWDT_REG.WINDOW  = cfg.WindowCmpValue;
    )
    
    return true;
}

/*! \brief feed watchdog
 *! \param void
 *! \retval none
 */
void watchdog_feed(void)
{
    SAFE_CLK_CODE(
        SAFE_ATOM_CODE (
            WWDT_REG.FEED = 0x000000AA;
            WWDT_REG.FEED = 0x00000055;
        )
    )
}

/*! \brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t watchdog_get_count_value(void)
{
    uint32_t wTmp = 0;
    
    SAFE_CLK_CODE(
        wTmp = WWDT_REG.TV;
    )
        
    return wTmp;
}

