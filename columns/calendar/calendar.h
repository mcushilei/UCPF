/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __CALENDAR_H__
#define __CALENDAR_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define SECONDS_OF_MINUTE       (60u)
#define SECONDS_OF_HOUR         (60u * 60u)
#define SECONDS_OF_DAY          (24u * 60u * 60u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    uint32_t    Year;
    uint8_t     Month;
    uint8_t     Day;
} date_t;

typedef struct {
    uint8_t     Hour;
    uint8_t     Minute;
    uint8_t     Second;
} time24_t;

typedef struct {
    union {
        date_t      Date;
        date_t;
    };
    union {
        time24_t    Time;
        time24_t;
    };
} date_time_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     is_leap_year(uint32_t year);
extern uint32_t days_in_year(const date_t *pDate);
extern uint32_t count_leap_years_between(uint32_t year1, uint32_t year2);
extern int32_t  count_days_between(const date_t *pStart, const date_t *pEnd);
extern bool     date_plus_days(date_t *pDate, int32_t deltaDays);

extern uint32_t time_to_seconds(const time24_t *pTime);
extern uint32_t seconds_to_time(time24_t *pTime, uint32_t seconds);
extern int32_t  count_seconds_between(const time24_t *pStart, const time24_t *pEnd);

#endif  //! #ifndef __CALENDAR_H__
/* EOF */
