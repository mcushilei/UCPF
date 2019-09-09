/*******************************************************************************
 *  Copyright(C)2016-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __DRIVER_ARM_M4_DRIVER_H__
#define __DRIVER_ARM_M4_DRIVER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if     defined(__TI__)
#   include ".\TI\driver.h"
#elif   defined(__FREESCALE__)
#   include ".\freescale\driver.h"
#elif   defined(__AMBIQ__)
#   include ".\ambiq\driver.h"
#elif   defined(__ST__)
#   include ".\st\driver.h"
#else
#   error "Unsupported arm device, please check your configuration."
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
