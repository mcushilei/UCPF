/*******************************************************************************
 *  Copyright(C)2015-2017 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __FIFO_C__
#ifndef __FIFO_H__
#define __FIFO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fifo_public.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define FIFO8_INIT(__FIFO, __PBUF, __BUF_SIZE)                         \
    fifo8_init(&__FIFO, __PBUF, __BUF_SIZE)
#define FIFO8_IN(__FIFO, __OBJ)         fifo8_in(&__FIFO, (const uint8_t *)&(__OBJ))
#define FIFO8_OUT(__FIFO, __OBJ)        fifo8_out(&__FIFO, (uint8_t *)&(__OBJ))


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
#endif
/* EOF */
