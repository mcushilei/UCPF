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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_rtc.h"
#include "..\scon\pm.h"
#include "./rtc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __SAFE_CLK_CODE(...)                     {                      \
        uint32_t tAHBStatus = peripheral_clock_get_status(PCONP_RTC);   \
        peripheral_clock_enable(PCONP_RTC);                             \
        __VA_ARGS__;                                                    \
        peripheral_clock_resume_status(PCONP_RTC,tAHBStatus);           \
    }

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


WEAK void __driver_rtc_initpd(void)
{
    peripheral_clock_enable(PCONP_RTC);
}

WEAK void __driver_rtc_deinitpd(void)
{
    peripheral_clock_disable(PCONP_RTC);
}

/*!\brief init real-time clock
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool rtc_init(void)
{
    __driver_rtc_initpd();
    
    RTC_REG.CCR     = (1u << 0)         //!< clock is enabled.
                    | (0u << 1)         //!< not reset.
                    | (0u << 4);        //!< calibration is not enabled.
    RTC_REG.CIIR    = (0u << 0)         //!< increment of second will not trige an interrupt.
                    | (0u << 1)         //!< increment of minute will not trige an interrupt.
                    | (0u << 2)         //!< increment of hour   will not trige an interrupt.
                    | (0u << 3)         //!< increment of day of month will trige an interrupt.
                    | (0u << 4)         //!< increment of day of week  will not trige an interrupt.
                    | (0u << 5)         //!< increment of day of yeat  will not trige an interrupt.
                    | (0u << 6)         //!< increment of month  will not trige an interrupt.
                    | (0u << 7);        //!< increment of year   will not trige an interrupt.
    RTC_REG.AMR     = (0u << 0)         //!< second will not trige an interrupt.
                    | (0u << 1)         //!< minute will not trige an interrupt.
                    | (0u << 2)         //!< hour   will not trige an interrupt.
                    | (0u << 3)         //!< day of month will trige an interrupt.
                    | (0u << 4)         //!< day of week  will not trige an interrupt.
                    | (0u << 5)         //!< day of yeat  will not trige an interrupt.
                    | (0u << 6)         //!< month  will not trige an interrupt.
                    | (0u << 7);        //!< year   will not trige an interrupt.
    return true;
}

bool rtc_deinit(void)
{
    return false;
}


/*!\brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t rtc_get_counter_value(void)
{
    return 0;
}

/*!\brief set count value
 *! \param void
 *! \retval return geh time count value
 */
void rtc_set_counter_value(uint32_t wValue)
{
}

/*!\brief get match value
 *! \param void
 *! \retval return match value
 */
uint32_t rtc_get_counter_match_value(void)
{
    return 0;
}

/*!\brief Set match value
 *! \param time match value
 *! \retval none
 */
void rtc_set_counter_match_value(uint32_t wValue)
{
}



void rtc_get_time_value(rtc_time_t* pTime)
{
    __SAFE_CLK_CODE (
        pTime->Year  = RTC_REG.YEAR;
        pTime->Month = RTC_REG.MONTH;
        pTime->Day   = RTC_REG.DOM;
        pTime->Hour  = RTC_REG.HOUR;
        pTime->Minute = RTC_REG.MIN;
        pTime->Second = RTC_REG.SEC;
    )
}

void rtc_set_time_value(rtc_time_t* pTime)
{
    __SAFE_CLK_CODE (
        RTC_REG.SEC = pTime->Second;
        RTC_REG.MIN = pTime->Minute;
        RTC_REG.HOUR  = pTime->Hour;
        RTC_REG.DOM   = pTime->Day;
        RTC_REG.MONTH = pTime->Month;
        RTC_REG.YEAR  = pTime->Year;
    )
}

void rtc_set_time_match_value(rtc_time_t* pTime)
{
    __SAFE_CLK_CODE (
        RTC_REG.ALSEC = pTime->Second;
        RTC_REG.ALMIN = pTime->Minute;
        RTC_REG.ALHOUR  = pTime->Hour;
        RTC_REG.ALDOM   = pTime->Day;
        RTC_REG.ALMON   = pTime->Month;
        RTC_REG.ALYEAR  = pTime->Year;
    )
}

void rtc_get_time_match_value(rtc_time_t* pTime)
{
    __SAFE_CLK_CODE (
        pTime->Year  = RTC_REG.ALYEAR;
        pTime->Month = RTC_REG.ALMON;
        pTime->Day   = RTC_REG.ALDOM;
        pTime->Hour  = RTC_REG.ALHOUR;
        pTime->Minute = RTC_REG.ALMIN;
        pTime->Second = RTC_REG.ALSEC;
    )
}

