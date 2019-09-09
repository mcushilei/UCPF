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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
#define IAP_FLASH_PAGE_SIZE         (256u)

#define IAP_LOCATION                (0x1FFF1FF1u)

#define IAP_CMD_PREPARE_SECTOR      (50u)
#define IAP_CMD_WRITE               (51u)
#define IAP_CMD_ERASE_SECTOR        (52u)
#define IAP_CMD_BLANK_CHECK         (53u)
#define IAP_CMD_READ_PID            (54u)
#define IAP_CMD_READ_BOOT_VER       (55u)
#define IAP_CMD_COMPARE             (56u)
#define IAP_CMD_REINVOKE_ISP        (57u)
#define IAP_CMD_READ_UID            (58u)
#define IAP_CMD_ERASE_PAGE          (59u)
#define IAP_CMD_ERASE_INFO_PAGE     (60u)
#define IAP_CMD_EEPROM_WRITE        (61u)
#define IAP_CMD_EEPROM_READ         (62u)

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
#define IAP_EXECUTE_CMD(__CMD, __RET)   \
    do {\
        SAFE_ATOM_CODE(\
            (*(fn_iap_t *)IAP_LOCATION)(__CMD, __RET);\
        )\
    } while (0)

/*============================ TYPES =========================================*/
typedef void fn_iap_t(uint32_t [], uint32_t []);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
static uint32_t iap_prepare_sectors(uint32_t firstSector, uint32_t lastSector)
{
	uint32_t command[5];
	uint32_t result[5];

	if (lastSector < firstSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    command[0] = IAP_CMD_PREPARE_SECTOR;
    command[1] = firstSector;
    command[2] = lastSector;
    
    IAP_EXECUTE_CMD(command, result);

    return result[0];
}

static uint32_t iap_copy_ram_to_flash(uint32_t firstPage, uint32_t *buffer, uint32_t num)
{
	uint32_t command[5];
	uint32_t result[5];

	if ((0 == num) || (3 < num)) {
		return IAP_RET_INVALD_PARAM;
	}

	command[0] = IAP_CMD_WRITE;
	command[1] = firstPage * IAP_FLASH_PAGE_SIZE;
	command[2] = (uint32_t)buffer;
	command[3] = num * IAP_FLASH_PAGE_SIZE;
	command[4] = PM_CORE_CLK_GET() / 1000u;
	
	IAP_EXECUTE_CMD(command, result);

	return result[0];
}

static uint32_t iap_erase_sectors(uint32_t firstSector, uint32_t lastSector)
{
	uint32_t command[5];
	uint32_t result[5];

	if (lastSector < firstSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    command[0] = IAP_CMD_ERASE_SECTOR;
    command[1] = firstSector;
    command[2] = lastSector;
    command[3] = PM_CORE_CLK_GET() / 1000u;
    
    IAP_EXECUTE_CMD(command, result);

	return result[0];
}

static uint32_t iap_check_blank_ectors(uint32_t firstSector, uint32_t lastSector, uint32_t *pu32Result)
{
	uint32_t command[5];
	uint32_t result[5];

	if (lastSector < firstSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    command[0] = IAP_CMD_BLANK_CHECK;
    command[1] = firstSector;
    command[2] = lastSector;

    IAP_EXECUTE_CMD(command, result);

    if (result[0] == IAP_RET_SECTOR_NOT_BLANK) {
        *pu32Result = result[1];
    }

    return result[0];
}

static uint32_t iap_read_part_id(uint32_t *id)
{
	uint32_t command[5];
	uint32_t result[5];

	command[0] = IAP_CMD_READ_PID;

	IAP_EXECUTE_CMD(command, result);

	*id = result[1];

	return result[0];
}

static uint32_t iap_read_boot_code_version(uint32_t *major, uint32_t *minor)
{
	uint32_t command[5];
	uint32_t result[5];

	command[0] = IAP_CMD_READ_BOOT_VER;

	IAP_EXECUTE_CMD(command, result);

	*major = (result[1] & 0x0000FF00UL) >> 8;
	*minor =  result[1] & 0x000000FFUL;

	return result[0];
}

static uint32_t iap_compare_memory(uint32_t dst, uint32_t src, uint32_t len, uint32_t *offset)
{
	uint32_t command[5];
	uint32_t result[5];

	command[0] = IAP_CMD_COMPARE;
	command[1] = dst;
	command[2] = src;
	command[3] = len;

	IAP_EXECUTE_CMD(command, result);

	if (result[0] == IAP_RET_COMPARE_NOT_SAME) {
        *offset = result[1];
	}
    
	return result[0];
}

static void iap_reinvoke_isp(void)
{
	uint32_t command[5];
	uint32_t result[5];

	command[0] = IAP_CMD_REINVOKE_ISP;

	IAP_EXECUTE_CMD(command, result);
}

static uint32_t iap_read_uid(uint32_t *pu32byte0, uint32_t *pu32byte1,
                     uint32_t *pu32byte2, uint32_t *pu32byte3)
{
	uint32_t command[5];
	uint32_t result[5];

	command[0] = IAP_CMD_READ_UID;

	IAP_EXECUTE_CMD(command, result);

	*pu32byte0 = result[1];
	*pu32byte1 = result[2];
	*pu32byte2 = result[3];
	*pu32byte3 = result[4];
    
    return result[0];
}

static uint32_t iap_erase_pages(uint32_t firstPage, uint32_t lastPage)
{
	uint32_t command[5];
	uint32_t result[5];

	if (lastPage < firstPage) {
		return IAP_RET_INVALD_PARAM;
	}
    
    command[0] = IAP_CMD_ERASE_PAGE;
    command[1] = firstPage;
    command[2] = lastPage;
    command[3] = PM_CORE_CLK_GET() / 1000u;
    
    IAP_EXECUTE_CMD(command, result);

	return result[0];
}

uint32_t iap_eeprom_write(uint32_t addr, const void *buffer, uint32_t len)
{
	uint32_t command[5];
	uint32_t result[5];

    command[0] = IAP_CMD_EEPROM_WRITE;
    command[1] = addr;
    command[2] = (uint32_t)buffer;
    command[3] = len;
    command[4] = PM_CORE_CLK_GET() / 1000u;
    
    IAP_EXECUTE_CMD(command, result);

	return result[0];
}

uint32_t iap_eeprom_read(uint32_t addr, void *buffer, uint32_t len)
{
	uint32_t command[5];
	uint32_t result[5];

    command[0] = IAP_CMD_EEPROM_READ;
    command[1] = addr;
    command[2] = (uint32_t)buffer;
    command[3] = len;
    command[4] = PM_CORE_CLK_GET() / 1000u;
    
    IAP_EXECUTE_CMD(command, result);

	return result[0];
}

uint32_t iap_flash_erase_pages(uint32_t firstPage, uint32_t lastPage)
{
    uint32_t error = 0;
    uint32_t firstSector, lastSector;
    
    firstSector = firstPage >> 4; //!< firstPage / 16. One sector contain 16 pages.
    lastSector   = lastPage   >> 4;
    
    error = iap_prepare_sectors(firstSector, lastSector);
	if (IAP_RET_CMD_SUCCESS != error) {
	    return error;
    }
    
    error = iap_erase_pages(firstPage, lastPage);
    
    return error;
}

uint32_t iap_flash_write_page(uint32_t page, uint32_t *buffer)
{
	uint32_t error = 0;
    uint32_t sector;
    
    sector = page >> 4;             //!< page / 16
    
    error = iap_prepare_sectors(sector, sector);
	if (IAP_RET_CMD_SUCCESS != error) {
        return error;
    }

    error = iap_erase_pages(page, 1);
    if (IAP_RET_CMD_SUCCESS != error) {
        return error;
    }

    error = iap_prepare_sectors(sector, sector);
	if (IAP_RET_CMD_SUCCESS != error) {
        return error;
    }

    error = iap_copy_ram_to_flash(page, buffer, IAP_FLASH_PAGE_SIZE);
    
    return error;
}

/* EOF */
