/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __CJSON_PORT_H__
#define __CJSON_PORT_H__

/*============================ INCLUDES ======================================*/
#include "..\app_cfg.h"


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define CJSON_PORT_MALLOC cjson_port_malloc
#define CJSON_PORT_FREE cjson_port_free
#define CJSON_PORT_REALLOC cjson_port_realloc

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void *cjson_port_malloc(size_t size);
extern void cjson_port_free(void *ptr);
extern void *cjson_port_realloc(void *pointer, size_t size);

#endif  //!< #ifndef __CJSON_PORT_H__
