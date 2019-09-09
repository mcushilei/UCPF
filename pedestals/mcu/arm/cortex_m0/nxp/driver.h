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


#ifndef __DRIVER_ARM_M0_NXP_DRIVER_H__
#define __DRIVER_ARM_M0_NXP_DRIVER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if     defined(__LPC12XX__)    
#   include ".\lpc12xx\driver.h"
#else
#   error "No supported arm device, please check your configuration."
#endif

/*============================ MACROS ========================================*/

#ifndef PSC_UID0
#   define PSC_UID0         (((uint32_t *)(ARM_SCB.VTOR + 28))[0])
#endif
#ifndef PSC_UID1
#   define PSC_UID1         (((uint32_t *)(ARM_SCB.VTOR + 28))[1])
#endif
#ifndef PSC_UID2
#   define PSC_UID2         (((uint32_t *)(ARM_SCB.VTOR + 28))[2])
#endif
#ifndef PSC_UID3
#   define PSC_UID3         (((uint32_t *)(ARM_SCB.VTOR + 28))[3])
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
