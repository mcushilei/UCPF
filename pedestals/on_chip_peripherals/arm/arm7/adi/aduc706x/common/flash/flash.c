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

#define __DRIVER_FLASH_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_flash.h"
#include ".\flash_public.h"

/*============================ MACROS ========================================*/

#define FLASH_CMD_NULL              (0x00u)
#define FLASH_CMD_SINGLE_READ       (0x01u)
#define FLASH_CMD_SINGLE_WRITE      (0x02u)
#define FLASH_CMD_ERASE_WRITE       (0x03u)
#define FLASH_CMD_SINGLE_VERIFY     (0x04u)
#define FLASH_CMD_SINGLE_ERASE      (0x05u)
#define FLASH_CMD_MASS_ERASE        (0x06u)
#define FLASH_CMD_SIGNATURE         (0x0Bu)
#define FLASH_CMD_PROTECT           (0x0Cu)
#define FLASH_CMD_PING              (0x0Fu)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
uint32_t flash_erase_page(uint32_t wPage)
{
    uint16_t wState;
    
    SAFE_ATOM_CODE(
        FLASH_REG.FEEMOD    = (0u << 9)     //!< reserved.
                            | (1u << 8)     //!< reserved, always set to 1.
                            | (0u << 5)     //!< reserved.
                            | (0u << 4)     //!< interrupt not enable.
                            | (1u << 3)     //!< earse/write command enable
                            | (0u << 0);    //!< reserved.
        FLASH_REG.FEEADR = wPage * FLASH_BLOCK_SIZE;
        FLASH_REG.FEECON = FLASH_CMD_SINGLE_ERASE;
        do {
            wState = FLASH_REG.FEESTA;
        } while (wState & (1u << 2));
    )
    if (wState & (1u << 1)) {
        return FLASH_ERR_CMD_FAIL;
    }
    return FLASH_ERR_NONE;
}

uint32_t flash_write_page(uint32_t wPage, uint8_t *pData)
{
    uint32_t wAddr;
    uint16_t wState;

    wAddr = wPage * FLASH_BLOCK_SIZE;
    for (uint32_t i = 0; i < FLASH_BLOCK_SIZE; i += 2u) {
        SAFE_ATOM_CODE(
            FLASH_REG.FEEMOD    = (0u << 9)     //!< reserved.
                                | (1u << 8)     //!< reserved, always set to 1.
                                | (0u << 5)     //!< reserved.
                                | (0u << 4)     //!< interrupt not enable.
                                | (1u << 3)     //!< earse/write command enable
                                | (0u << 0);    //!< reserved.
            FLASH_REG.FEEADR = wAddr;
            FLASH_REG.FEEDAT = (pData[i + 1] << 8) + pData[i];
            FLASH_REG.FEECON = FLASH_CMD_SINGLE_WRITE;
            do {
                wState = FLASH_REG.FEESTA;
            } while (wState & (1u << 2));
        )
        if (wState & (1u << 1)) {
            return FLASH_ERR_CMD_FAIL;
        }
        wAddr += 2;
    }
    return FLASH_ERR_NONE;
}

uint32_t flash_write_data(uint32_t wPage, uint8_t *pData, uint8_t len)
{
    uint32_t wAddr;
    uint16_t wState;

    wAddr = wPage * 512u;
    for (uint32_t i = 0; i < FLASH_BLOCK_SIZE; i += 2u) {
        SAFE_ATOM_CODE(
            FLASH_REG.FEEMOD    = (0u << 9)     //!< reserved.
                                | (1u << 8)     //!< reserved, always set to 1.
                                | (0u << 5)     //!< reserved.
                                | (0u << 4)     //!< interrupt not enable.
                                | (1u << 3)     //!< earse/write command enable
                                | (0u << 0);    //!< reserved.
            FLASH_REG.FEEADR = wAddr;
            FLASH_REG.FEEDAT = (pData[i + 1] << 8) + pData[i];
            FLASH_REG.FEECON = FLASH_CMD_SINGLE_WRITE;
            do {
                wState = FLASH_REG.FEESTA;
            } while (wState & (1u << 2));
        )
        if (wState & (1u << 1)) {
            return FLASH_ERR_CMD_FAIL;
        }
        wAddr += 2;
    }
    return FLASH_ERR_NONE;
}
/* EOF */
