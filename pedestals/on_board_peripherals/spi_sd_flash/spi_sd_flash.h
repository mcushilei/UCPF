/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __SPI_SD_H__
#define __SPI_SD_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool spi_sd_disable(void);
extern bool spi_sd_enable(void);
extern bool spi_sd_card_detect(void);
extern bool spi_sd_get_card_info(void);
extern bool spi_sd_read_blocks(uint32_t block, uint8_t *buf, uint32_t cnt);
extern bool spi_sd_write_blocks(uint32_t block, const uint8_t *buf, uint32_t cnt);
extern bool spi_sd_erase_blocks(uint32_t block, uint32_t cnt);

#endif /* __SPI_SD_H__ */
/* EOF */
