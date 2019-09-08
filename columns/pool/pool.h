/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __POOL_H__
#define __POOL_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(pool_t)
    void           *FreeList;
    pool_uint_t     Capacity;
    pool_uint_t     Level;          //!< current pool level.
    pool_uint_t     LevelMin;       //!< minimum pool level.
END_DEF_STRUCTURE(pool_t)

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
//! \note   the memory block you want to be managed by pool should be multiple size of void *.
extern bool         pool_init           (pool_t *pool, pool_uint_t level, void *mem, size_t blockSize);
extern void        *pool_new            (pool_t *pool);
extern bool         pool_free           (pool_t *pool, void *mem);
extern pool_uint_t  pool_get_level      (pool_t *pool);
extern pool_uint_t  pool_get_min_level  (pool_t *pool);

#endif      //!< #ifndef __POOL_H__
/* EOF */
