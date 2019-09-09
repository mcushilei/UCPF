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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_LPC17XX_WDT_C__

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
END_DEF_INTERFACE(i_wdt_t)
//! @}

/*============================ PROTOTYPES ====================================*/
extern bool watchdog_init(wdt_cfg_t *tCfg);
extern bool watchdog_enable(void);
extern bool watchdog_disable(void);
extern void watchdog_feed(void);
extern uint32_t watchdog_get_count_value(void);
extern bool watchdog_feed_timeout_flag_get(void);
extern void watchdog_feed_timeout_flag_clear(void);

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief define the WDT
const i_wdt_t WDT = {
    &watchdog_init,                                 //!< initialize the wdt
    &watchdog_enable,                               //!< enable the watchdog
    &watchdog_disable,                               //!< enable the watchdog
    &watchdog_feed,                                 //!< feed the watchdog
};

/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
/*!\brief init watchdog
 *! \param wdt_cfg_t*
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_init(wdt_cfg_t *tCfg)
{
    if ( NULL == tCfg ) {
        return false;
    }

    SAFE_ATOM_CODE (
        WDT_REG.WDMOD       = tCfg->wMode & (~WDT_MOD_WDEN_MSK);
        WDT_REG.WDFEED      = 0x000000AA;
        WDT_REG.WDFEED      = 0x00000055;
        WDT_REG.WDCLKSEL    = tCfg->wClockSelect;
        WDT_REG.WDTC        = tCfg->wTimeCount;
    )
    
    return true;
}

/*! \brief ensable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_enable(void)
{
    SAFE_ATOM_CODE (
        WDT_REG.WDMOD |= WDT_MOD_WDEN_MSK;
        WDT_REG.WDFEED = 0x000000AA;
        WDT_REG.WDFEED = 0x00000055;
    )
        
    return true;
}

/*! \brief once open, can not stop;
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_disable(void)
{
    if (WDT_REG.WDMOD & WDT_MOD_WDEN_MSK) {
        return false;
    }
    
    return true;
}

/*!\brief reset (feed) watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
void watchdog_feed(void)
{
    SAFE_ATOM_CODE (
        WDT_REG.WDFEED = 0x000000AA;
        WDT_REG.WDFEED = 0x00000055;
    )
}

/*!\brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t watchdog_get_count_value(void)
{
	return WDT_REG.WDTV;
}

/*! \brief get timeover flag status
 *! \param void
 *! \retval true timeover flag set
 *! \retval false timeover flag not set
 */
bool watchdog_feed_timeout_flag_get(void)
{
	if ((WDT_REG.WDMOD & WDT_MOD_WDTOF_MSK) >> WDT_MOD_WDTOF) {
        return true;
    }

    return false;
}

/*! \brief clear timeover flag status
 *! \param void
 *! \retval none
 */
void watchdog_feed_timeout_flag_clear(void)
{
	WDT_REG.WDMOD &= ~WDT_MOD_WDTOF_MSK;
}

