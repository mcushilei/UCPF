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
#define __COMPONENT_LED_SEGMENT_ADAPTOR_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\text_display_device_type.h"
#include ".\led_segment.h"

/*============================ MACROS ========================================*/
#define LED_SEGMENT_CS0         26
#define LED_SEGMENT_CS1         27
#define LED_SEGMENT_CS2         28
#define LED_SEGMENT_CS3         29
#define LED_SEGMENT_CS_MSK      ( (1u << LED_SEGMENT_CS0)\
                                | (1u << LED_SEGMENT_CS1)\
                                | (1u << LED_SEGMENT_CS2)\
                                | (1u << LED_SEGMENT_CS3))

#define LED_SEGMENT_DATA0       19
#define LED_SEGMENT_DATA1       20
#define LED_SEGMENT_DATA2       21
#define LED_SEGMENT_DATA3       22
#define LED_SEGMENT_DATA4       23
#define LED_SEGMENT_DATA5       24
#define LED_SEGMENT_DATA6       25
#define LED_SEGMENT_DATA_MSK    ( (1u << LED_SEGMENT_DATA0)\
                                | (1u << LED_SEGMENT_DATA1)\
                                | (1u << LED_SEGMENT_DATA2)\
                                | (1u << LED_SEGMENT_DATA3)\
                                | (1u << LED_SEGMENT_DATA4)\
                                | (1u << LED_SEGMENT_DATA5)\
                                | (1u << LED_SEGMENT_DATA6))

#define LED_SEGMENT_DOT0        16
#define LED_SEGMENT_DOT1        17
#define LED_SEGMENT_DOT_MSK     ( (1u << LED_SEGMENT_DOT0)\
                                | (1u << LED_SEGMENT_DOT1))

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
void led_segment_pin_init(void)
{
//    IO_CFG(
//        {PA19,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA20,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA21,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA22,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA23,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA24,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA25,  IO_WORKS_AS_FUNC6, IO_PULL_UP},
//    );
//    
//    IO_CFG(
//        {PA26,  IO_WORKS_AS_FUNC6, IO_PULL_UP},
//        {PA27,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA28,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA29,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//    );
//    
//    IO_CFG(
//        {PA16,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//        {PA17,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
//    );
//    
//    GPIO0_REG.DIR |= LED_SEGMENT_CS_MSK | LED_SEGMENT_DATA_MSK | LED_SEGMENT_DOT_MSK;
}

void led_segment_select(display_pointer_t const *ptPosition)
{
//    if (NULL == ptPosition) {
//        IO.GPIO0.ptRegPage->OUTSET = LED_SEGMENT_CS_MSK;
//        return;
//    }
//    
//    switch (ptPosition->chColumn) {
//        case 0:
//            IO.GPIO0.ptRegPage->OUTSET = BIT(LED_SEGMENT_CS0);
//            break;
//        case 1:
//            IO.GPIO0.ptRegPage->OUTSET = BIT(LED_SEGMENT_CS1);
//            break;
//        case 2:
//            IO.GPIO0.ptRegPage->OUTSET = BIT(LED_SEGMENT_CS2);
//            break;
//        case 3:
//            IO.GPIO0.ptRegPage->OUTSET = BIT(LED_SEGMENT_CS3);
//            break;
//        default:
//            IO.GPIO0.ptRegPage->OUTSET = LED_SEGMENT_CS_MSK;
//            break;
//    }
}

void led_segment_select_none(void)
{
//    IO.GPIO0.ptRegPage->OUTCLR = LED_SEGMENT_CS_MSK;
}

void led_segment_write_data(uint_fast16_t hwData)
{
//    uint32_t wTemp = 0;
//    
//    wTemp = IO.GPIO0.ptRegPage->MSK;
//    IO.GPIO0.ptRegPage->MSK = ~LED_SEGMENT_DATA_MSK;
//    IO.GPIO0.ptRegPage->OUT = (IO.GPIO0.ptRegPage->OUT & (~LED_SEGMENT_DATA_MSK)) | ((uint32_t)(hwData) << LED_SEGMENT_DATA0);
//    IO.GPIO0.ptRegPage->MSK = wTemp;
}

void led_segment_write_dot(uint_fast8_t chDot)
{
//    if (chDot) {
//        IO.GPIO0.ptRegPage->OUTCLR = LED_SEGMENT_DOT_MSK;
//    } else {
//        IO.GPIO0.ptRegPage->OUTSET = LED_SEGMENT_DOT_MSK;
//    }
}

/* EOF */
