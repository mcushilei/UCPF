/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __BLOCK_MEMORY_C__
#ifndef __BLOCK_MEMORY_H__
#define __BLOCK_MEMORY_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef bool block_memory_read_write_opt_t(uint32_t block, uint8_t *pData);

typedef struct {
    block_memory_read_write_opt_t  *Read;
    block_memory_read_write_opt_t  *Write;
    uint32_t                        BlockSize;
    uint32_t                        BlockNum;
    uint8_t                        *Buffer;
} block_mem_device_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool block_memory_device_init(
                            block_mem_device_t            *pDevice,
                            block_memory_read_write_opt_t *pRead,
                            block_memory_read_write_opt_t *pWrite,
                            uint32_t                       blockSize,
                            uint32_t                       blockNum,
                            uint8_t                       *pBuffer);
extern bool block_memory_write(
                            const block_mem_device_t   *pDevice,
                            uint32_t                    addr,
                            const uint8_t              *pData,
                            uint32_t                    dataLength);
extern bool block_memory_read(
                            const block_mem_device_t   *pDevice,
                            uint32_t                    addr,
                            uint8_t                    *pData,
                            uint32_t                    dataLength);

#endif  //! #ifndef __BLOCK_H__
#endif  //! #ifndef __BLOCK_C__
/* EOF */
