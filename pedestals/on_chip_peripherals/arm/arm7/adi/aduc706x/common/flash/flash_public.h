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



#ifndef __DRIVER_PUBLIC_FLASH_H__
#define __DRIVER_PUBLIC_FLASH_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define FLASH_BLOCK_SIZE            (512u)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define ADDRESS_TO_PAGE(__ADDR)     (((uint32_t)(__ADDR) - 0x80000u) / 512u)

/*============================ TYPES =========================================*/
enum {
    FLASH_ERR_NONE              = 0,
    FLASH_ERR_INVALID_ADDR,
    FLASH_ERR_CMD_FAIL,
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern uint32_t flash_erase_page(uint32_t wPage);
extern uint32_t flash_write_page(uint32_t wPage, uint8_t *pData);
extern uint32_t flash_write_data(uint32_t wPage, uint8_t *pData, uint8_t len);

#endif  //! #ifndef __DRIVER_PUBLIC_FLASH_H__
/* EOF */
