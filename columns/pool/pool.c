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



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\pool.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#ifndef POOL_CRITICAL_SECTION_BEGIN
#define POOL_CRITICAL_SECTION_BEGIN()
#endif

#ifndef POOL_CRITICAL_SECTION_END
#define POOL_CRITICAL_SECTION_END()
#endif


/*============================ TYPES =========================================*/
//! \brief   memory block structure: single-list node.
//! @{
DEF_STRUCTURE(pool_block_t)
    pool_block_t   *Next;
END_DEF_STRUCTURE(pool_block_t)
//! @}

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief  init an empty pool.
 */
bool pool_init(pool_t *pool, pool_uint_t level, void *mem, size_t blockSize)
{
    pool_block_t **ppBlock;
    
    //! block size should be at least the size of pointers.
    if (NULL == pool || (0u == level)
        || (NULL == mem)
        || (blockSize < sizeof(void *))
        || ( (size_t)mem & (((size_t)1 << sizeof(void *)) - 1) != 0u ) ) {
        return false;
    }

    pool->FreeList  = NULL;
    pool->Level     = level;
    pool->LevelMin  = level;

    ppBlock = (pool_block_t **)&pool->FreeList;
    for (; level; level--) {
        *ppBlock = (pool_block_t *)mem;
        ppBlock = &((pool_block_t *)mem)->Next;
        mem = (char *)mem + blockSize;
    }
    *ppBlock = NULL;
    
    return true;
}

bool pool_free(pool_t *pool, void *mem)
{    
    if ((NULL == pool) || (NULL == mem)
        || ( (size_t)mem & (((size_t)1 << sizeof(void *)) - 1) != 0u ) ) {
        return false;
    }

    POOL_CRITICAL_SECTION_BEGIN();
    do {
        ((pool_block_t *)mem)->Next = (pool_block_t *)pool->FreeList;
        pool->FreeList = (pool_block_t *)mem;
        pool->Level++;
    } while (0);
    POOL_CRITICAL_SECTION_END();

    return true;
}

void *pool_new(pool_t *pool)
{
    void *mem = NULL;

    if (NULL == pool) {
        return NULL;
    }

    POOL_CRITICAL_SECTION_BEGIN();
    do {
        if (NULL == pool->FreeList) {
            break; 
        }
        mem = pool->FreeList;
        pool->FreeList = ((pool_block_t *)mem)->Next; 
        pool->Level--;
        if (pool->Level < pool->LevelMin) {
            pool->LevelMin = pool->Level;
        }
        ((pool_block_t*)mem)->Next = NULL;
    } while (0);
    POOL_CRITICAL_SECTION_END();

    return mem;     
}    

pool_uint_t pool_get_level(pool_t *pool)
{
    if (NULL == pool) {
        return 0;
    }

    return pool->Level;
}

pool_uint_t pool_get_min_level(pool_t *pool)
{
    if (NULL == pool) {
        return 0;
    }

    return pool->LevelMin;
}


/* EOF */
