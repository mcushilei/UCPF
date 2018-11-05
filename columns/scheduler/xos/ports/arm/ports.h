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


#ifndef __KERNEL_PORTS_ARM_PORTS_H__
#define __KERNEL_PORTS_ARM_PORTS_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"


#if     defined(__CORTEX_M0P__)
    #include ".\cortex_m0plus\ports.h"
#elif   defined(__CORTEX_M3__)
    #include ".\cortex_m3\ports.h"
#elif   defined(__CORTEX_M4__)
    #include ".\cortex_m4\ports.h"
#else
    #error "No supported CPU."
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
