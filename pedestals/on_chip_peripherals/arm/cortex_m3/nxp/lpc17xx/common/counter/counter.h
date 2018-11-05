/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __DRIVER_TIMER_C__
#ifndef __DRIVER_TIMER_H__
#define __DRIVER_TIMER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_counter.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __TIMER_EXTERN_FUN(_N, _A)                                              \
    extern bool timer##_N##_init(void);                                         \
    extern bool timer##_N##_enable(void);                                       \
    extern bool timer##_N##_disable(void);                                      \
    extern bool timer##_N##_start(void);                                        \
    extern bool timer##_N##_stop(void);                                         \

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
MREPEAT(TIMER_COUNT, __TIMER_EXTERN_FUN, 0)   //!< this is used by bottom layer

#endif
#endif
/* EOF */