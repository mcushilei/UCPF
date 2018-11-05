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
#define __COMPONENT_LED_SEGMENT_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\text_display_device_type.h"

/*============================ MACROS ========================================*/
#define LED_SEGMENT_TYPE_LENGTH_MSK     (0x07)
#define LED_SEGMENT_TYPE_DOT_MSK        (0x80)

#define LED_SEGMENT_TYPE_LENGTH_7       (0x00)
#define LED_SEGMENT_TYPE_LENGTH_14      (0x01)
#define LED_SEGMENT_TYPE_LENGTH_16      (0x02)
#define LED_SEGMENT_TYPE_WITH_DOT       (0x80)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(led_segment_properties_t)
    uint8_t     chType;         //!common P or N
    uint8_t     chRowSize;
    uint8_t     chColumnSize;
END_DEF_STRUCTURE(led_segment_properties_t)

#if ((LED_SEGMENT_TYPE & LED_SEGMENT_TYPE_LENGTH_MSK) == LED_SEGMENT_TYPE_LENGTH_7)
typedef uint8_t     led_segment_dram_t;
#else
typedef uint16_t    led_segment_dram_t;
#endif
    
DEF_STRUCTURE(led_segment_display_t)
    display_pointer_t   tScanPointer;
    led_segment_dram_t  Framebuffer[TEXT_DISPLAY_RAM_ROW_SIZE][TEXT_DISPLAY_RAM_COLUMN_SIZE]; //!< has been decoded.
    uint8_t             Dot[TEXT_DISPLAY_RAM_ROW_SIZE][(TEXT_DISPLAY_RAM_COLUMN_SIZE >> 3) | 1u];
    union {
        uint8_t chValue;
        struct {
            uint8_t     EN:1;
        };
    } tState;
END_DEF_STRUCTURE(led_segment_display_t);
    
DEF_STRUCTURE(led_segment_display_data_t)
    uint8_t Char[TEXT_DISPLAY_RAM_ROW_SIZE][TEXT_DISPLAY_RAM_COLUMN_SIZE];
    uint8_t Dot[TEXT_DISPLAY_RAM_ROW_SIZE][(TEXT_DISPLAY_RAM_COLUMN_SIZE >> 3) | 1u];
END_DEF_STRUCTURE(led_segment_display_data_t);


/*============================ PROTOTYPES ====================================*/
extern void led_segment_pin_init(void);
extern void led_segment_select(display_pointer_t const *ptPosition);
extern void led_segment_select_none(void);
extern void led_segment_write_data(uint_fast16_t hwData);
extern void led_segment_write_dot(uint_fast8_t chDot);

/*============================ LOCAL VARIABLES ===============================*/
static const led_segment_properties_t c_tLEDSegmentProperties = {
    LED_SEGMENT_TYPE,
    TEXT_DISPLAY_RAM_ROW_SIZE,
    TEXT_DISPLAY_RAM_COLUMN_SIZE,
};
static led_segment_display_t        s_tLEDSegmentDisplay;
static led_segment_display_data_t   s_tLEDSegmentDataRam;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool led_segment_init(void)
{
    s_tLEDSegmentDisplay.tState.chValue = 0;
    s_tLEDSegmentDisplay.tScanPointer.chColumn = 0;
    s_tLEDSegmentDisplay.tScanPointer.chRow = 0;
    led_segment_pin_init();
    
    return true;
}

void led_segment_display_on(void)
{
    s_tLEDSegmentDisplay.tState.EN = 1;
}

void led_segment_display_off(void)
{
    s_tLEDSegmentDisplay.tState.EN = 0;
    led_segment_select_none();
}

//! refresh display ram
void led_segment_framebuffer_refesh(void)
{
    uint_fast8_t chRow = 0, chColumn = 0;
    
    for (; chRow < c_tLEDSegmentProperties.chRowSize; chRow++) {
        for (chColumn = 0; chColumn < c_tLEDSegmentProperties.chColumnSize; chColumn++) {
            s_tLEDSegmentDisplay.Framebuffer[chRow][chColumn] = s_tLEDSegmentDataRam.Char[chRow][chColumn];
        }
        
        for (chColumn = 0; chColumn < ((c_tLEDSegmentProperties.chColumnSize >> 3) | 1u); chColumn++) {
            s_tLEDSegmentDisplay.Dot[chRow][chColumn] = s_tLEDSegmentDataRam.Dot[chRow][chColumn];
        }
    }
}

static void led_segment_framebuffer_syn(void)
{
}

//! display ram to device, once a char
bool led_segment_scan(void)
{
    //! off current display
    led_segment_select_none();
    if (s_tLEDSegmentDisplay.tState.EN) {
        //! write display data to data bus
        led_segment_write_data(s_tLEDSegmentDisplay.Framebuffer[s_tLEDSegmentDisplay.tScanPointer.chRow][s_tLEDSegmentDisplay.tScanPointer.chColumn]);
        led_segment_write_dot(s_tLEDSegmentDisplay.Dot[s_tLEDSegmentDisplay.tScanPointer.chRow][s_tLEDSegmentDisplay.tScanPointer.chColumn >> 3]
                              & BIT(BITMASK(2, 0) & s_tLEDSegmentDisplay.tScanPointer.chColumn));
        //! set select line to display
        led_segment_select(&s_tLEDSegmentDisplay.tScanPointer);
        //! move curser
        s_tLEDSegmentDisplay.tScanPointer.chColumn++;
        if (s_tLEDSegmentDisplay.tScanPointer.chColumn >= c_tLEDSegmentProperties.chColumnSize) {
            s_tLEDSegmentDisplay.tScanPointer.chColumn = 0;
            s_tLEDSegmentDisplay.tScanPointer.chRow++;
            if (s_tLEDSegmentDisplay.tScanPointer.chRow >= c_tLEDSegmentProperties.chRowSize) {
                s_tLEDSegmentDisplay.tScanPointer.chRow = 0;
                //! refresh framebuffer
                led_segment_framebuffer_syn();
            }
        }
    }
    
    return true;
}

bool led_segment_write_byte(uint8_t chByte, uint8_t chRow, uint8_t chColumn)
{
    if ((chRow >= c_tLEDSegmentProperties.chRowSize)
    ||  (chColumn >= c_tLEDSegmentProperties.chColumnSize)) {
        return false;
    }
    
    s_tLEDSegmentDataRam.Char[chRow][chColumn] = chByte;
    
    return true;
}

bool led_segment_read_byte(uint8_t *pchByte, uint8_t chRow, uint8_t chColumn)
{
    if ((chRow >= c_tLEDSegmentProperties.chRowSize)
    ||  (chColumn >= c_tLEDSegmentProperties.chColumnSize)) {
        return false;
    }
    
    if (NULL != pchByte) {
        *pchByte = s_tLEDSegmentDataRam.Char[chRow][chColumn];
    }
    
    return true;
}

bool led_segment_set_dot(uint8_t chRow, uint8_t chColumn)
{
    if ((chRow >= c_tLEDSegmentProperties.chRowSize)
    ||  (chColumn >= c_tLEDSegmentProperties.chColumnSize)) {
        return false;
    }
    
    s_tLEDSegmentDataRam.Dot[chRow][chColumn >> 3] |= BIT(BITMASK(2, 0) & chColumn);
    
    return true;
}

bool led_segment_clear_dot(uint8_t chRow, uint8_t chColumn)
{
    if ((chRow >= c_tLEDSegmentProperties.chRowSize)
    ||  (chColumn >= c_tLEDSegmentProperties.chColumnSize)) {
        return false;
    }
    
    s_tLEDSegmentDataRam.Dot[chRow][chColumn >> 3] &= ~BIT(BITMASK(2, 0) & chColumn);
    
    return true;
}

void led_segment_clear_ram(void)
{
    uint_fast8_t chRow = 0, chColumn = 0;
    
    for (; chRow < c_tLEDSegmentProperties.chRowSize; chRow++) {
        for (chColumn = 0; chColumn < c_tLEDSegmentProperties.chColumnSize; chColumn++) {
            s_tLEDSegmentDisplay.Framebuffer[chRow][chColumn] = 0xFF;
            s_tLEDSegmentDataRam.Char[chRow][chColumn] = 0;
        }
        
        for (chColumn = 0; chColumn < ((c_tLEDSegmentProperties.chColumnSize >> 3) | 1u); chColumn++) {
            s_tLEDSegmentDisplay.Dot[chRow][chColumn] = 0;
            s_tLEDSegmentDataRam.Dot[chRow][chColumn] = 0;
        }
    }
}

/* EOF */
