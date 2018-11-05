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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_LPC17XX_FLASH_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
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
/*****************************************************************************
** Function name:	IAP_PrepareSectors	
**
** Description:		Prepares sector(s) for erasing or write operations. This
** 					command must be executed before executing the "Copy RAM to
** 					Flash" or "Erase Sector(s)" commands.
**
** Parameters:		wStartSector - Number of first sector to prepare.
** 					wEndSector - Number of last sector to prepare.
**
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_PrepareSectors(uint32_t wStartSector, uint32_t wEndSector)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	if (wEndSector < wStartSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    wCommand[0] = IAP_CMD_PREPARE_SECTOR;
    wCommand[1] = wStartSector;
    wCommand[2] = wEndSector;
    
    IAP_EXECUTE_CMD(wCommand, wResult);

    return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_CopyRAMToFlash  IAP RAM->Flash
**
** Description:		Program the flash memory with data stored in RAM.
**
** Parameters:	   	wDstAddr - Destination Flash address, should be a word boundary.
**			 		wSrcAddr - Source RAM address, should be a word boundary.
**			 		wLen     - Number of bytes to write, must be a multiple 
                               of 4 bytes and max 4096.
*
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_CopyRAMToFlash(uint32_t wDstAddr, uint32_t wSrcAddr, uint32_t wLen)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_WRITE;
	wCommand[1] = wDstAddr;
	wCommand[2] = wSrcAddr;
	wCommand[3] = wLen;
	wCommand[4] = PM_CORE_CLK_GET() / 1000UL;
	
	IAP_EXECUTE_CMD(wCommand, wResult);

	return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_EraseSectors	 
**
** Description:		Erase a sector or multiple sectors of on-chip Flash memory.
**
** Parameters:		wStartSector - Number of first sector to erase.
** 					wEndSector - Number of last sector to erase.
*
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_EraseSectors(uint32_t wStartSector, uint32_t wEndSector)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	if (wEndSector < wStartSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    wCommand[0] = IAP_CMD_ERASE_SECTOR;
    wCommand[1] = wStartSector;
    wCommand[2] = wEndSector;
    wCommand[3] = PM_CORE_CLK_GET() / 1000UL;
    
    IAP_EXECUTE_CMD(wCommand, wResult);

	return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_BlankCheckSectors
**
** Description:		Blank check a sector or multiple sectors of on-chip flash
** 					memory.
**
** Parameters:		wStartSector - Number of first sector to check.
** 					wEndSector - Number of last sector to check.
** 					pu32Result[0] - Offset of the first non blank word location
**                  if the Status Code is IAP_STA_SECTOR_NOT_BLANK.
** 					pu32Result[1] - Contents of non blank word location.
**
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_BlankCheckSectors(uint32_t wStartSector, uint32_t wEndSector, uint32_t *pu32Result)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	if (wEndSector < wStartSector) {
		return IAP_RET_INVALD_PARAM;
	}
    
    wCommand[0] = IAP_CMD_BLANK_CHECK;
    wCommand[1] = wStartSector;
    wCommand[2] = wEndSector;

    IAP_EXECUTE_CMD(wCommand, wResult);

    if (wResult[0] == IAP_RET_SECTOR_NOT_BLANK) {
        *pu32Result = wResult[1];
    }

    return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_ReadPartID	 
**
** Description:		Read the part identification number.
**
** Parameters:		pu32PartID - Pointer to storage for part ID number.
*
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_ReadPartID(uint32_t *pu32PartID)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_READ_PID;

	IAP_EXECUTE_CMD(wCommand, wResult);

	*pu32PartID = wResult[1];

	return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_ReadBootVersion
**
** Description:		Read the boot code version number.
**
** Parameters:		pu32Major - Major version number in ASCII format.
** 					pu32Minor - Minor version number in ASCII format.
**
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_ReadBootVersion(uint32_t *pu32Major, uint32_t *pu32Minor)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_READ_BOOT_VER;

	IAP_EXECUTE_CMD(wCommand, wResult);

	*pu32Major = (wResult[1] & 0x0000FF00UL) >> 8;
	*pu32Minor =  wResult[1] & 0x000000FFUL;

	return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_ReadBootVersion
**
** Description:		Read the boot code version number.
**
** Parameters:		pu32Major - Major version number in ASCII format.
** 					pu32Minor - Minor version number in ASCII format.
**
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_ReadUID(uint32_t *pu32byte0, uint32_t *pu32byte1,
                     uint32_t *pu32byte2, uint32_t *pu32byte3)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_READ_UID;

	IAP_EXECUTE_CMD(wCommand, wResult);

	*pu32byte0 = wResult[1];
	*pu32byte1 = wResult[2];
	*pu32byte2 = wResult[3];
	*pu32byte3 = wResult[4];
    
    return wResult[0];
}

/*****************************************************************************
** Function name:	IAP_Compare	 IAP У׼
**
** Description:		Compares the memory contents at two locations.
**
** Parameters:		wLen - Number of bytes to compare, must be a multiple of 4.
**					pu32Offset - Offset of the first mismatch if the Status Code is COMPARE_ERROR
**
** Returned value:	Status code returned by IAP ROM function.
**
******************************************************************************/
uint32_t IAP_Compare(uint32_t wDstAddr, uint32_t wSrcAddr, uint32_t wLen, uint32_t *pu32Offset)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_COMPARE;
	wCommand[1] = wDstAddr;
	wCommand[2] = wSrcAddr;
	wCommand[3] = wLen;

	IAP_EXECUTE_CMD(wCommand, wResult);

	if (wResult[0] == IAP_RET_COMPARE_NOT_SAME) {
        *pu32Offset = wResult[1];
	}
    
	return wResult[0];
}

/*****************************************************************************
** Function name:	vIAP_ReinvokeISP
**
** Description:		Invoke the bootloader in ISP mode.
**
** Parameters:		None.
*
** Returned value:	None.
**
******************************************************************************/
void IAP_ReinvokeISP(void)
{
	uint32_t wCommand[5];
	uint32_t wResult[5];

	wCommand[0] = IAP_CMD_REINVOKE_ISP;

	IAP_EXECUTE_CMD(wCommand, wResult);
}

uint32_t flash_write(uint32_t wDstAddr, uint32_t wSrcAddr, uint32_t wLen)
{
	uint32_t wErrorCode = 0;
    uint32_t wStartSector, wEndSector;
    
    if (wDstAddr < 0x10000) {
        wStartSector = wDstAddr >> 12; //!< / 8
    } else {
        wStartSector = 16 + (wDstAddr >> 17);
    }
    
    if ((wDstAddr + wLen) < 0x10000) {
        wEndSector = (wDstAddr + wLen) >> 12;
    } else {
        wEndSector = 16 + ((wDstAddr + wLen) >> 17);
    }
    
    wErrorCode = IAP_PrepareSectors(wStartSector, wEndSector);
	if (IAP_RET_CMD_SUCCESS != wErrorCode) {
        return (wErrorCode);
    }

    wErrorCode = IAP_CopyRAMToFlash(wDstAddr, wSrcAddr, wLen);
    return (wErrorCode);
}

uint32_t flash_erase_sector(uint32_t wStartSector, uint32_t wEndSector)
{
    uint32_t wErrorCode = 0;
    
    wErrorCode = IAP_PrepareSectors(wStartSector, wEndSector);
	if (IAP_RET_CMD_SUCCESS != wErrorCode) {
        return (wErrorCode);
    }
    
    wErrorCode = IAP_EraseSectors(wStartSector, wEndSector);
    return (wErrorCode);
}

/* EOF */
