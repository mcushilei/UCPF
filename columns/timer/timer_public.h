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

//! Do not move this pre-processor statement to other places
#ifndef __SERVICE_TIMER_PUBLIC_H__
#define __SERVICE_TIMER_PUBLIC_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\list\list.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void timer_routine_t(void);

typedef struct {
	uint8_t         Flag;
	uint32_t        Count;
	uint32_t        Period;
	timer_routine_t *pRoutine;
	list_node_t     ListNode;
} timer_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool timer_init(void);
extern void timer_tick(void);
extern bool timer_config(
	timer_t        *timer,
	uint32_t		initValue,
	uint32_t		reloadValue,
	timer_routine_t *pRoutine);
extern void timer_start(timer_t *timer, uint32_t value);
extern void timer_stop(timer_t *timer);
extern bool timer_is_timeout(timer_t *timer);
extern bool timer_is_running(timer_t *timer);

#endif  //!< #ifndef __TIMER_PUBLIC_H__
/* EOF */
