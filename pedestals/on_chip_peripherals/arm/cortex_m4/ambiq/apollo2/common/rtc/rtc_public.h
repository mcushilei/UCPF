/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_RTC_PUBLIC_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_RTC_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_rtc.h"

/*============================ MACROS ========================================*/
//*****************************************************************************
//
//! @name OSC Start and Stop
//! @brief OSC Start and Stop defines.
//!
//! OSC Start and Stop defines to be used with \e am_hal_clkgen_osc_x().
//! @{
//
//*****************************************************************************
#define DRIVER_RTC_OSC_LFRC          0x1
#define DRIVER_RTC_OSC_XT            0x0
//! @}

//*****************************************************************************
//
//! @name RTC Interrupts
//! @brief Macro definitions for RTC interrupt status bits.
//!
//! These macros correspond to the bits in the RTC interrupt status register.
//! They may be used with any of the \e am_hal_rtc_int_x() functions.
//!
//! @{
//
//*****************************************************************************
#define DRIVER_RTC_INT_ALM                  REG_RTC_INTEN_ALM_M
#define DRIVER_RTC_INT_OF                   REG_RTC_INTEN_OF_M
#define DRIVER_RTC_INT_ACC                  REG_RTC_INTEN_ACC_M
#define DRIVER_RTC_INT_ACF                  REG_RTC_INTEN_ACF_M
//! @}

//*****************************************************************************
//
//! @name RTC Alarm Repeat Interval.
//! @brief Macro definitions for the RTC alarm repeat interval.
//!
//! These macros correspond to the RPT bits in the RTCCTL register.
//! They may be used with the \e am_hal_rtc_alarm_interval_set() function.
//!
//! Note: DRIVER_RTC_ALM_RPT_10TH and DRIVER_RTC_ALM_RPT_100TH do not
//! correspond to the RPT bits but are used in conjunction with setting the
//! ALM100 bits in the ALMLOW register.
//!
//! @{
//
//*****************************************************************************
#define DRIVER_RTC_ALM_RPT_DIS              0x0
#define DRIVER_RTC_ALM_RPT_YR               0x1
#define DRIVER_RTC_ALM_RPT_MTH              0x2
#define DRIVER_RTC_ALM_RPT_WK               0x3
#define DRIVER_RTC_ALM_RPT_DAY              0x4
#define DRIVER_RTC_ALM_RPT_HR               0x5
#define DRIVER_RTC_ALM_RPT_MIN              0x6
#define DRIVER_RTC_ALM_RPT_SEC              0x7
#define DRIVER_RTC_ALM_RPT_10TH             0x8
#define DRIVER_RTC_ALM_RPT_100TH            0x9
//! @}

//*****************************************************************************
//
//! @name RTC Alarm 100 Interval.
//! @brief Macro definitions for the RTC alarm ms intervals.
//!
//! These macros are used inside the #am_hal_rtc_alarm_interval_set function
//! when 10ms and 100ms repeated alarm intervals are desired.
//!
//! @{
//
//*****************************************************************************
#define DRIVER_RTC_ALM100_DEFAULT           0x00
#define DRIVER_RTC_ALM100_10TH              0xF0
#define DRIVER_RTC_ALM100_100TH             0xFF
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//*****************************************************************************
//
//! @brief The basic time structure used by the HAL for RTC interaction.
//!
//! All values are positive whole numbers. The HAL routines convert back and
//! forth to BCD.
//
//*****************************************************************************
typedef struct {
    uint32_t ReadError;
    uint32_t CenturyEnable;
    uint32_t Weekday;
    uint32_t Century;
    uint32_t Year;
    uint32_t Month;
    uint32_t Day;
    uint32_t Hour;
    uint32_t Minute;
    uint32_t Second;
    uint32_t Hundredths;
} driver_rtc_time_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void driver_rtc_osc_select(uint32_t OSC);
extern void driver_rtc_start(void);
extern void driver_rtc_stop(void);
extern void     driver_rtc_time_set(driver_rtc_time_t *pTime);
extern uint32_t driver_rtc_time_get(driver_rtc_time_t *pTime);
extern void driver_rtc_alarm_interval_set(uint32_t repeatInterval);
extern void driver_rtc_alarm_set(driver_rtc_time_t *pTime, uint32_t repeatInterval);
extern void driver_rtc_alarm_get(driver_rtc_time_t *pTime);

extern void     driver_rtc_int_enable(uint32_t interr);
extern void     driver_rtc_int_disable(uint32_t interr);
extern uint32_t driver_rtc_int_enable_get(void);
extern void     driver_rtc_int_set(uint32_t interr);
extern void     driver_rtc_int_clear(uint32_t interr);
extern uint32_t driver_rtc_int_status_get(bool onlyEnabled);

#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_RTC_PUBLIC_H__
/* EOF */
