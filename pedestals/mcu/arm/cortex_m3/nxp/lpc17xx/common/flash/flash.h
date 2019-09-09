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


#ifndef __DRIVER_LPC17XX_FLASH_C__
#ifndef __DRIVER_LPC17XX_FLASH_H__
#define __DRIVER_LPC17XX_FLASH_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern uint32_t flash_write         (uint32_t wDstAddr, uint32_t wSrcAddr, uint32_t wLen);
extern uint32_t flash_erase_sector  (uint32_t wStartSector, uint32_t wEndSector);
extern uint32_t IAP_ReadUID(uint32_t *pu32byte0, uint32_t *pu32byte1,
                            uint32_t *pu32byte2, uint32_t *pu32byte3);
extern uint32_t IAP_ReadPartID(uint32_t *pu32PartID);

#endif
#endif
/* EOF */
