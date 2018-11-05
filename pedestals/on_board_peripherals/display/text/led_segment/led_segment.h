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


#ifndef __COMPONENT_LED_SEGMENT_C__
#ifndef __COMPONENT_LED_SEGMENT_H__
#define __COMPONENT_LED_SEGMENT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

    //! Common P LED Segment
typedef enum {
    LED_7SEGMENT_CODE_0     = 0xC0, 
    LED_7SEGMENT_CODE_1     = 0xCF, 
    LED_7SEGMENT_CODE_2     = 0xA4, 
    LED_7SEGMENT_CODE_3     = 0xB0,   //!< 0~3
    LED_7SEGMENT_CODE_4     = 0x99, 
    LED_7SEGMENT_CODE_5     = 0x92, 
    LED_7SEGMENT_CODE_6     = 0x82, 
    LED_7SEGMENT_CODE_7     = 0xF8,   //!< 4~7
    LED_7SEGMENT_CODE_8     = 0x80, 
    LED_7SEGMENT_CODE_9     = 0x90, 
    LED_7SEGMENT_CODE_UA    = 0x88, 
    LED_7SEGMENT_CODE_B     = 0x83,   //!< 8, 9, A, b
    LED_7SEGMENT_CODE_UC    = 0xC6, 
    LED_7SEGMENT_CODE_D     = 0xA1, 
    LED_7SEGMENT_CODE_UE    = 0x86, 
    LED_7SEGMENT_CODE_UF    = 0x8E,   //!< C, d, E, F
    LED_7SEGMENT_CODE_DIM   = 0x7F,
} em_led_segment_code_t;


DEF_STRUCTURE(led_segment_display_data_t)
    uint8_t Char[TEXT_DISPLAY_RAM_ROW_SIZE][TEXT_DISPLAY_RAM_COLUMN_SIZE];
    uint8_t Dot[TEXT_DISPLAY_RAM_ROW_SIZE][(TEXT_DISPLAY_RAM_COLUMN_SIZE >> 3) | 1u];
END_DEF_STRUCTURE(led_segment_display_data_t);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool led_segment_init(void);
extern void led_segment_display_on(void);
extern void led_segment_display_off(void);
extern void led_segment_framebuffer_refesh(void);
extern bool led_segment_scan(void);
extern bool led_segment_write_byte(uint8_t chByte, uint8_t chRow, uint8_t chColumn);
extern bool led_segment_read_byte(uint8_t *pchByte, uint8_t chRow, uint8_t chColumn);
extern bool led_segment_set_dot(uint8_t chRow, uint8_t chColumn);
extern bool led_segment_clear_dot(uint8_t chRow, uint8_t chColumn);
extern void led_segment_clear_ram(void);

#endif  //! #ifndef __COMPONENT_LED_SEGMENT_H__
#endif  //! #ifndef __COMPONENT_LED_SEGMENT_C__
/* EOF */
