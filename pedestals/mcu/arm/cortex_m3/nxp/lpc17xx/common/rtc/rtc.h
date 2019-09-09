/*******************************************************************************
 *  Copyright(C)2016-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_RTC_H__
#define __DRIVER_RTC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_rtc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    uint32_t Year;
    uint8_t  Month;
    uint8_t  Day;
    uint8_t  Hour;
    uint8_t  Minute;
    uint8_t  Second;
} rtc_time_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     rtc_init(void);
extern bool     rtc_deinit(void);

extern uint32_t rtc_get_counter_value(void);
extern void     rtc_set_counter_value(uint32_t wValue);
extern uint32_t rtc_get_counter_match_value(void);
extern void     rtc_set_counter_match_value(uint32_t wValue);

extern void     rtc_get_time_value(rtc_time_t* pTime);
extern void     rtc_set_time_value(rtc_time_t* pTime);
extern void     rtc_set_time_match_value(rtc_time_t* pTime);
extern void     rtc_get_time_match_value(rtc_time_t* pTime);

#endif
/* EOF */
