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

#ifndef __DRIVER_ARM7_ADCU706X_COMMON_TIMER_C__
#ifndef __DRIVER_ARM7_ADCU706X_COMMON_TIMER_H__
#define __DRIVER_ARM7_ADCU706X_COMMON_TIMER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_timer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
extern bool watchdog_init(void);
extern bool watchdog_enable(void);
extern bool watchdog_disable(void);
extern void watchdog_feed(void);

extern bool real_timer_init(void);
extern bool real_timer_get_time(uint32_t *pwTime);
extern bool real_timer_set_time(uint32_t wTime);
extern bool real_timer_enable(void);
extern bool real_timer_disable(void);


#endif      //!< __DRIVER_ARM7_ADCU706X_COMMON_TIMER_H__
#endif      //!< __DRIVER_ARM7_ADCU706X_COMMON_TIMER_C__
/* EOF */
