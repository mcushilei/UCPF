/*******************************************************************************
*  Copyright(C)2016-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __COLUMNS_TIMER_H__
#define __COLUMNS_TIMER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\list\list.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct timer_t  timer_t;
typedef void timer_callback_t(timer_t *timer);

struct timer_t {
	list_node_t         ListNode;
	uint32_t            Count;
	uint32_t            Period;
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool timer_init(timer_callback_t *callback);
extern void timer_tick(void);
extern bool timer_config(timer_t *timer, uint32_t initValue, uint32_t reloadValue);
extern void timer_start(timer_t *timer, uint32_t value);
extern void timer_stop(timer_t *timer);
extern bool timer_is_running(timer_t *timer);

#endif  //! #ifndef __COLUMNS_TIMER_H__
/* EOF */
