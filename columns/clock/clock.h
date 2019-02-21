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


#ifndef __SERVICE_CLOCK_H__
#define __SERVICE_CLOCK_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\list\list.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void alarmclock_routine_t(void);

typedef struct {
	uint8_t                 Flag;
	uint32_t                Count;
	alarmclock_routine_t   *pRoutine;
	list_node_t             ListNode;
} alarmclock_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool clock_init          (void);
extern bool clock_set_time      (uint32_t time);
extern void clock_tick_tock     (void);
extern bool alarmclock_config   (alarmclock_t *ac, uint32_t value, alarmclock_routine_t *pRoutine);
extern void alarmclock_start    (alarmclock_t *ac, uint32_t value);
extern void alarmclock_stop     (alarmclock_t *ac);
extern bool alarmclock_is_timeout(alarmclock_t *ac);

#endif  //! #ifndef __SERVICE_CLOCK_H__
/* EOF */
