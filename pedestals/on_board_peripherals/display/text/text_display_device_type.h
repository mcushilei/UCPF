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


#ifndef __COMPONENT_TEXT_DISPLAY_DEVICE_H__
#define __COMPONENT_TEXT_DISPLAY_DEVICE_H__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(display_pointer_t)
    uint8_t chRow;
    uint8_t chColumn;
END_DEF_STRUCTURE(display_pointer_t);

typedef uint16_t display_ram_t[TEXT_DISPLAY_RAM_ROW_SIZE][TEXT_DISPLAY_RAM_COLUMN_SIZE];

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __COMPONENT_TEXT_DISPLAY_DEVICE_H__
/* EOF */
