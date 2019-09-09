/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __IAP_H__
#define __IAP_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IAP_FLASH_PAGE_SIZE         (256u)

#define IAP_RET_CMD_SUCCESS         (0u)
#define IAP_RET_INVALID_COMMAND     (1u)
#define IAP_RET_SRC_ADDR_ERROR      (2u)
#define IAP_RET_DST_ADDR_ERROR      (3u)
#define IAP_RET_SRC_ADDR_NOT_MAPPED (4u)
#define IAP_RET_DST_ADDR_NOT_MAPPED (5u)
#define IAP_RET_COUNT_ERROR         (6u)
#define IAP_RET_INVALID_SECTOR      (7u)
#define IAP_RET_SECTOR_NOT_BLANK    (8u)
#define IAP_RET_SECTOR_NOT_PREPARED (9u)
#define IAP_RET_COMPARE_NOT_SAME    (10u)
#define IAP_RET_BUSY                (11u)
#define IAP_RET_INVALD_PARAM        (12u)
#define IAP_RET_ADDR_ERROR          (13u)
#define IAP_RET_ADDR_NOT_MAPPED     (14u)
#define IAP_RET_ADDR_CMD_LOCKED     (15u)
#define IAP_RET_INVALID_CODE        (16u)
#define IAP_RET_INVALID_BAUD_RATE               (17u)
#define IAP_RET_INVALID_STOP_BIT                (18u)
#define IAP_RET_CODE_READ_PROTECTION_ENABLED    (19u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern uint32_t iap_flash_write_page(uint32_t page, uint32_t *buffer);
extern uint32_t iap_flash_erase_pages(uint32_t startPage, uint32_t endPage);
extern uint32_t iap_eeprom_write(uint32_t addr, const void *buffer, uint32_t len);
extern uint32_t iap_eeprom_read(uint32_t addr, void *buffer, uint32_t len);

#endif
/* EOF */
