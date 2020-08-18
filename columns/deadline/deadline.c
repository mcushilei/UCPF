/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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




/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./deadline.h"
#include "./deadline_plug.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/



void deadline_init(deadline_t *dlTimer)
{
	dlTimer->EndTime = 0;
    dlTimer->StartTime = 0;
}

bool deadline_is_expired(deadline_t *dlTimer)
{
	uint32_t now, deltaExpire, deltaDeadline;

	dead_line_get_tick(&now);
    deltaExpire = now - dlTimer->StartTime;
    deltaDeadline = dlTimer->EndTime - dlTimer->StartTime;
    if (deltaExpire >= deltaDeadline) {
        return true;
    } else {
        return false;
    }
}

void deadline_set_ms(deadline_t *dlTimer, uint32_t timeout)
{
	dead_line_get_tick(&dlTimer->StartTime);
    dlTimer->EndTime = dlTimer->StartTime + dead_line_ms_to_tick(timeout);
}

void deadline_set(deadline_t *dlTimer, uint32_t timeout)
{
    deadline_set_ms(dlTimer, timeout * 1000);
}

int deadline_left_ms(deadline_t *dlTimer)
{
	uint32_t now, deltaExpire, deltaDeadline;

	dead_line_get_tick(&now);
    deltaExpire = now - dlTimer->StartTime;
    deltaDeadline = dlTimer->EndTime - dlTimer->StartTime;
    if (deltaExpire >= deltaDeadline) {
        return 0;
    } else {
        return dead_line_tick_to_ms(deltaDeadline - deltaExpire);
    }
}


/* EOF */
