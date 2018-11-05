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


#ifndef __DRIVER_LPC17XX_WDT_C__
#ifndef __DRIVER_LPC17XX_WDT_H__
#define __DRIVER_LPC17XX_WDT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name wdt config struct
//! @{
DEF_STRUCTURE(wdt_cfg_t)
    uint32_t    wMode;                  //!< Watchdog config ward
    uint32_t    wClockSelect;           //!< Watchdog clock source select.
    uint32_t    wTimeCount;             //!< Watchdog time-out value.
END_DEF_STRUCTURE(wdt_cfg_t)
//! @}

//! \name wdt struct
//! @{
DEF_INTERFACE(i_wdt_t)
    bool        (*Init)(wdt_cfg_t *tCfg);           //!< initialize the wdt
    bool        (*Enable)(void);                    //!< enable the watchdog
    bool        (*Disable)(void);                    //!< enable the watchdog
    void        (*Feed)(void);                      //!< feed the watchdog
    uint32_t    (*GetCountValue)(void);             //!< get the time count value
END_DEF_INTERFACE(i_wdt_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief declare the PMU
extern const i_wdt_t WDT;

/*============================ PROTOTYPES ====================================*/
extern bool watchdog_init(wdt_cfg_t *tCfg);
extern bool watchdog_enable(void);
extern bool watchdog_disable(void);
extern void watchdog_feed(void);
extern uint32_t watchdog_get_count_value(void);
extern bool watchdog_feed_timeout_flag_get(void);
extern void watchdog_feed_timeout_flag_clear(void);

#endif
#endif
