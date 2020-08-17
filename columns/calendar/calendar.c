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



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\calendar.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static const uint8_t daysOfMonth[13]     = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint8_t daysOfMonthLeap[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*============================ IMPLEMENTATION ================================*/
bool is_leap_year(uint32_t year)
{
    if ((year % 400u == 0u) || ((year % 4u == 0u) && (year % 100u != 0u))) {
        return true;
    }

    return false;
}

static bool __correct_date(date_t *pDate)
{
    bool ret = true;

    if (pDate->Year == 0) {
        pDate->Year = 1;
        ret = false;
    }

    if (pDate->Month == 0) {
        pDate->Month = 1;
        ret = false;
    } else if (pDate->Month > 12) {
        pDate->Month = 12;
        ret = false;
    }

    if (pDate->Day == 0) {
        pDate->Day = 1;
        ret = false;
    } else {
        uint8_t month = pDate->Month;

        if (is_leap_year(pDate->Year)) {
            if (pDate->Day > daysOfMonthLeap[month]) {
                pDate->Day = daysOfMonthLeap[month];
                ret = false;
            }
        } else {
            if (pDate->Day > daysOfMonth[month]) {
                pDate->Day = daysOfMonth[month];
                ret = false;
            }
        }
    }

    return ret;
}

bool validate_date(const date_t *pDate)
{
    date_t date = *pDate;
    return __correct_date(&date);
}

//! correct a date if it's invalid.
void correct_date(date_t *pDate)
{
    __correct_date(pDate);
}


//! count days elapsed in a year.(include the current day)
uint32_t days_in_year(const date_t *pDate)
{
    uint32_t        days;
    uint_fast8_t    i, month;
    date_t          date;

    date = *pDate;
    correct_date(&date);

    days  = date.Day;
    month = date.Month;
    month--;
    if (is_leap_year(date.Year)) {
        for (i = month; i != 0u; i--) {
            days += daysOfMonthLeap[i];
        }
    } else {
        for (i = month; i != 0u; i--) {
            days += daysOfMonth[i];
        }
    }

    return days;
}

//! count leap years from year 1 to specified year, exclude specified year.
static uint32_t count_leap_years(uint32_t year)
{
    if (year == 0u) {
        return 0;
    }
    year--;
    return year / 4u - year / 100u + year / 400u;
}

//! count leap years from year1 to year2.(exclude year1 and year2)
uint32_t count_leap_years_between(uint32_t year1, uint32_t year2)
{
    if (year1 == 0u) {
        year1 = 1u;
    }
    if (year2 == 0u) {
        year2 = 1u;
    }

    if (year1 == year2) {
        return 0;
    } else if (year1 > year2) {
        uint32_t t;
        t = year1;
        year1 = year2;
        year2 = t;
    }

    year1++;
    return count_leap_years(year2) - count_leap_years(year1);
}

//! calculate pEnd - pStart. eg: 2019/1/3 - 2019/1/4 = -1, 2019/1/4 - 2019/1/1 = 2.
int32_t count_days_between(const date_t *pStart, const date_t *pEnd)
{
    int32_t         days;
    uint32_t        years;
    date_t          start;
    date_t          end;
    date_t          date;
    bool            negative = false;

    start = *pStart;
    end   = *pEnd;
    correct_date(&start);
    correct_date(&end);

    do {
        if (end.Year > start.Year) {
            break;
        }
        
        if (end.Year == start.Year) {
            if (end.Month > start.Month) {
                break;
            }
            
            if (end.Month == start.Month) {
                if (end.Day >= start.Day) {
                    return end.Day - start.Day;
                } else {
                    days = start.Day - end.Day;
                    return -days;
                }
            }
        }
        
        date  = start;
        start = end;
        end   = date;
        negative = true;
    } while (0);

    if (start.Year != end.Year) {
        if (is_leap_year(start.Year)) {
            days = 366u - days_in_year(pStart);
        } else {
            days = 365u - days_in_year(pStart);
        }
        years = count_leap_years_between(start.Year, end.Year);
        days += 366u * years;
        days += 365u * (end.Year - start.Year - 1u - years);
        days += days_in_year(pEnd);
    } else {
        days = days_in_year(pEnd) - days_in_year(pStart);
    }

    if (negative) {
        return -days;
    } else {
        return days;
    }
}

//! calculate pDate + deltaDays.
bool date_plus_days(date_t *pDate, int32_t deltaDays)
{
    uint32_t year;
    uint8_t  month;
    uint8_t  day;
    date_t date;
    const uint8_t *pDaysInMonth = NULL;
    uint32_t days, dd;

    correct_date(pDate);
    year  = pDate->Year;
    month = pDate->Month;
    day   = pDate->Day;

    if (deltaDays < 0) {        //! see if minus
        dd = -deltaDays;
__SUB_STATE_0:
        date.Year  = year;
        date.Month = month;
        date.Day   = day;
        days = days_in_year(&date);
        if (dd < days) {
        } else {
            dd -= days;
            year--;
            if (year == 0u) {
                return false;
            }
            month = 12;
            day   = 31;

            while (dd > 365u) {
                if (is_leap_year(year)) {
                    if (dd > 366u) {
                        dd -= 366u;
                        year--;
                        if (year == 0u) {
                            return false;
                        }
                    } else {
                        break;
                    }
                } else {
                    dd -= 365u;
                    year--;
                    if (year == 0u) {
                        return false;
                    }
                }
            }
            goto __SUB_STATE_0;
        }

        if (is_leap_year(year)) {
            pDaysInMonth = daysOfMonthLeap;
        } else {
            pDaysInMonth = daysOfMonth;
        }
        if (dd < day) {
            day -= dd;
        } else {
            dd -= day;
            month--;
            day = pDaysInMonth[month];
            while (dd >= day) {
                dd -= day;
                month--;
                day = pDaysInMonth[month];
            }
            day -= dd;
        }

    } else {                    //! or plus
        dd = deltaDays;
__ADD_STATE_0:
        date.Year  = year;
        date.Month = month;
        date.Day   = day;
        if (is_leap_year(year)) {
            days = 366u - days_in_year(&date);
        } else {
            days = 365u - days_in_year(&date);
        }
        if (dd <= days) {
        } else {
            dd -= days + 1u;
            year++;
            month = 1;
            day = 1;

            while (dd > 365u) {
                if (is_leap_year(year)) {
                    if (dd > 366u) {
                        dd -= 366u;
                        year++;
                    } else {
                        break;
                    }
                } else {
                    dd -= 365u;
                    year++;
                }
            }
            goto __ADD_STATE_0;
        }

        if (is_leap_year(year)) {
            pDaysInMonth = daysOfMonthLeap;
        } else {
            pDaysInMonth = daysOfMonth;
        }
        if (dd <= (uint32_t)(pDaysInMonth[month] - day)) {
            day += dd;
        } else {
            dd -= pDaysInMonth[month] - day + 1u;
            day = 1u;
            month++;
            while (dd > 28u) {
                if (dd > pDaysInMonth[month]) {
                    dd -= pDaysInMonth[month];
                    month++;
                } else {
                    break;
                }
            }
            day += dd;
            while (day > pDaysInMonth[month]) {
                day -= pDaysInMonth[month];
                month++;
            }
        }
    }

    pDate->Year  = year;
    pDate->Month = month;
    pDate->Day   = day;

    return true;
}

void correct_time(time24_t *pTime)
{
    pTime->Hour   %= 24u;
    pTime->Minute %= 60u;
    pTime->Second %= 60u;
}

uint32_t time_to_seconds(const time24_t *pTime)
{
    uint32_t s = 0;
    
    s += SECONDS_OF_HOUR * pTime->Hour;
    s += SECONDS_OF_MINUTE * pTime->Minute;
    s += pTime->Second;
    
    return s;
}

//! return days in case 'seconds' is more than the seconds in a day.
uint32_t seconds_to_time(time24_t *pTime, uint32_t seconds)
{
    uint32_t days;

    days = seconds / SECONDS_OF_DAY;
    seconds -= days * SECONDS_OF_DAY;
    
    pTime->Hour = seconds / SECONDS_OF_HOUR;
    seconds -= pTime->Hour * SECONDS_OF_HOUR;
    
    pTime->Minute = seconds / SECONDS_OF_MINUTE;
    seconds -= pTime->Minute * SECONDS_OF_MINUTE;
    
    pTime->Second = seconds;

    return days;
}

//! calculate pEnd - pStart.
int32_t count_seconds_between(const time24_t *pStart, const time24_t *pEnd)
{
    int32_t s1, s2;

    s1 = time_to_seconds(pStart);
    s2 = time_to_seconds(pEnd);
    
    return s2 - s1;
}

// TODO: test this funtion
bool datetime_plus_seconds(date_time_t *datetime, int32_t deltaSeconds)
{
    int32_t deltaDays = deltaSeconds / SECONDS_OF_DAY;
    int32_t secondRemain;
    
    deltaSeconds -= deltaDays * SECONDS_OF_DAY;
    secondRemain = time_to_seconds(&datetime->Time);
    
    if (deltaSeconds < 0) {
        if ((-deltaSeconds) >= secondRemain) {
            deltaDays--;
            deltaSeconds = deltaSeconds + secondRemain + SECONDS_OF_DAY;
            datetime->Hour = 0;
            datetime->Minute = 0;
            datetime->Second = 0;
        } else {
        }
    } else {
        if (deltaSeconds >= (SECONDS_OF_DAY - secondRemain)) {
            deltaDays++;
            deltaSeconds = deltaSeconds + secondRemain - SECONDS_OF_DAY;
            datetime->Hour = 0;
            datetime->Minute = 0;
            datetime->Second = 0;
        } else {
        }
    }
    
    if (!date_plus_days(&datetime->Date, deltaDays)) {
        // we cannot support AC.
        return false;
    }
    seconds_to_time(&datetime->Time, deltaSeconds);
    return true;
}

/* EOF */
