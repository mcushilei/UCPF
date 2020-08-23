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

#ifndef __DEADLINE_H__
#define __DEADLINE_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
 
#if !defined(FRAMEWORK_DEADLINE_IMPLEMNET_FILE)
#error "deadline_type.h path must be defined by FRAMEWORK_DEADLINE_IMPLEMNET_FILE."
#else
#include FRAMEWORK_DEADLINE_IMPLEMNET_FILE
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void deadline_init(deadline_t *dlTimer);
extern bool deadline_is_expired(deadline_t *dlTimer);
extern void deadline_set_ms(deadline_t *dlTimer, uint32_t timeout);
extern void deadline_set(deadline_t *dlTimer, uint32_t timeout);
extern int deadline_left_ms(deadline_t *dlTimer);

#endif  //!< #ifndef __DEADLINE_H__
/* EOF */
