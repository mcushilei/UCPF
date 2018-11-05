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

//! \note do not move this pre-processor statement to other places
#define __POOL_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\pool_public.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
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
bool pool_init(void *obj, pool_uint_t blockSize)
{    
    pool_t *pool = (pool_t *)obj;

    //! block size should be mulitple size of pointer.
    if (NULL == obj || 0u == blockSize || 0u != (blockSize % sizeof(void *))) {
        return false;
    }

    pool->FreeList  = NULL;
    pool->BlockSize = blockSize;
    pool->Size      = 0u;
    pool->Level     = 0u;
    pool->LevelMin  = 0u;

    return true;
}

bool pool_deinit(void *obj)
{
    return true;
}

/*! \brief  add memory to pool.
 */
bool pool_add_memory(void *obj, void *pMem, pool_uint_t num)
{
    pool_t *pool = (pool_t *)obj;

    if ((NULL == obj) || (NULL == pMem)) {
        return false;
    }

    __POOL_ATOM_ACCESS(
        for (; num; num--) {
            ((pool_block_t *)pMem)->Next = (pool_block_t *)pool->FreeList;
            pool->FreeList = pMem;
            pMem = (char *)pMem + pool->BlockSize;
        }
        pool->Level     += num;
        pool->LevelMin  += num;
    )

    return true;
}

bool pool_free(void* obj, void* pMem)
{    
    pool_t *pool = (pool_t *)obj;

    if ((NULL == obj) || (NULL == pMem)) {
        return false;
    }

    __POOL_ATOM_ACCESS(
        do {
            ((pool_block_t *)pMem)->Next = (pool_block_t *)pool->FreeList;
            pool->FreeList = pMem;
            pool->Level++;
        } while (0);
    )

    return true;
}

void *pool_new(void *obj)
{
    pool_t *pool = (pool_t *)obj;
    void *pMem = NULL;

    if (NULL == obj) {
        return NULL;
    }

     __POOL_ATOM_ACCESS(
        do {
            if (NULL == pool->FreeList) {
                break; 
            }
            pMem = pool->FreeList;
            pool->FreeList = ((pool_block_t *)pMem)->Next; 
            ((pool_block_t*)pMem)->Next = NULL;
            pool->Level--;
            if (pool->Level < pool->LevelMin) {
                pool->LevelMin = pool->Level;
            }
        } while (0);
    )

    return pMem;     
}    

pool_uint_t pool_get_level(void *obj)
{
    pool_t *pool = (pool_t *)obj;

    if (NULL == obj) {
        return 0;
    }

    return pool->Level;
}

pool_uint_t pool_get_min_level(void *obj)
{
    pool_t *pool = (pool_t *)obj;

    if (NULL == obj) {
        return 0;
    }

    return pool->LevelMin;
}


/* EOF */
