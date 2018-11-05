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

//! \note do not move this pre-processor statement to other places
#include "..\app_cfg.h"

#ifndef __SERVICE_TIMER_APP_CFG_H__
#define __SERVICE_TIMER_APP_CFG_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define TIMER_TICK_CYCLE    (10u)       //! in ms

#if     defined(__OS_WINDOWS__) || defined(__OS_RTOS__)
#define __TIMER_SAFE_ATOM_CODE(...)  OS_CRITICAL_SECTION(__VA_ARGS__)
#else
#define __TIMER_SAFE_ATOM_CODE(...)     SAFE_ATOM_CODE(__VA_ARGS__)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif  //! #ifndef __SERVICE_TIMER_APP_CFG_H__
/* EOF */
