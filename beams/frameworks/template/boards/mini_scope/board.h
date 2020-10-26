/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __BOARD_MINI_SCOPE_BOARD_H__
#define __BOARD_MINI_SCOPE_BOARD_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define LED_PIN_MSK             PIN28_MSK

#define BREATH_LED_ON()     GPIO0_REG.CLR = LED_PIN_MSK
#define BREATH_LED_OFF()    GPIO0_REG.SET = LED_PIN_MSK
    
#define OLED_SPI_MOSI_PIN   PIN15
#define OLED_SPI_CLK_PIN    PIN24
#define OLED_RESET_PIN      PIN20
#define OLED_CD_PIN         PIN19

#define OLED_SPI_MOSI_PIN_MSK   PIN15_MSK
#define OLED_SPI_CLK_PIN_MSK    PIN24_MSK
#define OLED_RESET_PIN_MSK      PIN20_MSK
#define OLED_CD_PIN_MSK         PIN19_MSK

#define OLED_SPI_MOSI_CLR()   GPIO0_REG.CLR = OLED_SPI_MOSI_PIN_MSK
#define OLED_SPI_MOSI_SET()   GPIO0_REG.SET = OLED_SPI_MOSI_PIN_MSK

#define OLED_SPI_CLK_CLR()   GPIO0_REG.CLR = OLED_SPI_CLK_PIN_MSK
#define OLED_SPI_CLK_SET()   GPIO0_REG.SET = OLED_SPI_CLK_PIN_MSK

#define OLED_RES_CLR()   GPIO0_REG.CLR = OLED_RESET_PIN_MSK
#define OLED_RES_SET()   GPIO0_REG.SET = OLED_RESET_PIN_MSK

#define OLED_CD_CLR()   GPIO0_REG.CLR = OLED_CD_PIN_MSK
#define OLED_CD_SET()   GPIO0_REG.SET = OLED_CD_PIN_MSK


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool board_init(void);

#endif  //!< #ifndef __BOARD_MINI_SCOPE_BOARD_H__
/* EOF */
