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

//! \note do not move this pre-processor statement to other places
#define __BLOCK_MEMORY_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include <string.h>

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

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
bool block_memory_device_init(block_mem_device_t            *pDevice,
                              block_memory_read_write_opt_t *pRead,
                              block_memory_read_write_opt_t *pWrite,
                              uint32_t                       blockSize,
                              uint32_t                       blockNum,
                              uint8_t                       *pBuffer)
{
    pDevice->Read        = pRead;
    pDevice->Write       = pWrite;
    pDevice->BlockSize   = blockSize;
    pDevice->BlockNum    = blockNum;
    pDevice->Buffer      = pBuffer;
    
    return true;
}

bool block_memory_write(const block_mem_device_t *pDevice, uint32_t addr, const uint8_t *pData, uint32_t dataLength)
{
    uint32_t blockAddr, byteAddr, byteCnt;
    uint8_t state = 0;
    bool res = true;
    
    blockAddr = addr / pDevice->BlockSize;
    byteAddr  = addr % pDevice->BlockSize;

    if (byteAddr == 0u) {
        state = 1;
    } else {
        state = 0;
    }

    switch (state) {
        case 0:
            byteCnt = pDevice->BlockSize - byteAddr;
            byteCnt = (byteCnt < dataLength) ? byteCnt : dataLength;
            memset(pDevice->Buffer, 0, pDevice->BlockSize);
            if (false == pDevice->Read(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            memcpy(pDevice->Buffer + byteAddr, pData, byteCnt);
            if (false == pDevice->Write(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            dataLength  -= byteCnt;
            pData       += byteCnt;
            if (dataLength == 0) {
                res = true;
                goto __EXIT;
            } else {
                blockAddr++;
                state = 1;
            }
            //break;

        case 1:
            while (dataLength >= pDevice->BlockSize) {
                if (false == pDevice->Write(blockAddr, (uint8_t *)pData)) {
                    res = false;
                    goto __EXIT;
                }
                dataLength  -= pDevice->BlockSize;
                pData       += pDevice->BlockSize;
                blockAddr++;
            }
            if (dataLength == 0) {
                res = true;
                goto __EXIT;
            } else {
                state = 2;
            }
            //break;

        case 2:
            memset(pDevice->Buffer, 0, pDevice->BlockSize);
            if (false == pDevice->Read(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            memcpy(pDevice->Buffer, pData, dataLength);
            if (false == pDevice->Write(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            break;
    }
__EXIT:

    return res;
}

bool block_memory_read(const block_mem_device_t *pDevice, uint32_t addr, uint8_t *pData, uint32_t dataLength)
{
    uint32_t blockAddr, byteAddr, byteCnt;
    bool res = true;

    uint8_t state = 0;

    blockAddr = addr / pDevice->BlockSize;
    byteAddr  = addr % pDevice->BlockSize;

    switch (state) {
        case 0:
            byteCnt = pDevice->BlockSize - byteAddr;
            byteCnt = (byteCnt < dataLength) ? byteCnt : dataLength;
            if (false == pDevice->Read(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            memcpy(pData, pDevice->Buffer + byteAddr, byteCnt);
            dataLength  -= byteCnt;
            pData       += byteCnt;
            if (dataLength == 0) {
                res = true;
                goto __EXIT;
            } else {
                blockAddr++;
                state = 1;
            }
            //break;

        case 1:
            while (dataLength >= pDevice->BlockSize) {
                if (false == pDevice->Read(blockAddr, pData)) {
                    res = false;
                    goto __EXIT;
                }
                dataLength  -= pDevice->BlockSize;
                pData       += pDevice->BlockSize;
                blockAddr++;
            }
            if (dataLength == 0) {
                res = true;
                goto __EXIT;
            } else {
                state = 2;
            }
            //break;

        case 2:
            if (false == pDevice->Read(blockAddr, pDevice->Buffer)) {
                res = false;
                goto __EXIT;
            }
            memcpy(pData, pDevice->Buffer, dataLength);
            break;
    }
__EXIT:

    return res;
}

/* EOF */
