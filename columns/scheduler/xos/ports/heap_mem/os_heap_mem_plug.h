/*******************************************************************************
 *  Copyright(C)2019-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __OS_HEAP_MEM_PLUG_H__
#define __OS_HEAP_MEM_PLUG_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
#ifndef HEAP_MEM_CFG_LEN
#error "HEAP_MEM_CFG_LEN must be defined"
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
extern const heap_memory_cfg_t heapCfg[HEAP_MEM_CFG_LEN];

/*============================ PUBLIC PROTOTYPES =============================*/

#endif  //!< #ifndef __OS_HEAP_MEM_PLUG_H__
/* EOF */
