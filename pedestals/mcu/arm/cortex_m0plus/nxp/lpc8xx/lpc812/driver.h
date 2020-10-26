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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC812_DRIVER_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC812_DRIVER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\device.h"
#include "..\common\sys\pm.h"
#include "..\common\sys\pin.h"
#include "..\common\gpio\gpio.h"
#include "..\common\uart\uart.h"
#include "..\common\wdt\wdt.h"
#include "..\common\iap\iap.h"
#include "..\common\spi\spi.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

/*! \note initialize driver
 *  \param none
 *  \retval true hal initialization succeeded.
 *  \retval false hal initialization failed
 */
extern bool driver_init( void );

#endif
/* EOF */
