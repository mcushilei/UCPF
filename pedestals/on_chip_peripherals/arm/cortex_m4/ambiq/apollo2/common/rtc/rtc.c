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
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_RTC_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\rtc_public.h"
#include ".\reg_rtc.h"
#include "..\sys\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

//*****************************************************************************
//
// Converts a Binary Coded Decimal (BCD) byte to its Decimal form.
//
//*****************************************************************************
static uint8_t bcd_to_dec(uint8_t ui8BCDByte)
{
  return (((ui8BCDByte & 0xF0) >> 4) * 10) + (ui8BCDByte & 0x0F);
}

//*****************************************************************************
//
// Converts a Decimal byte to its Binary Coded Decimal (BCD) form.
//
//*****************************************************************************
static uint8_t dec_to_bcd(uint8_t ui8DecimalByte)
{
  return (((ui8DecimalByte / 10) << 4) | (ui8DecimalByte % 10));
}

//*****************************************************************************
//
//! @brief Selects the clock source for the RTC.
//!
//! @param osc the clock source for the RTC.
//!
//! This function selects the clock source for the RTC.
//!
//! Valid values for osc are:
//!
//!     DRIVER_RTC_OSC_LFRC
//!     DRIVER_RTC_OSC_XT
//!
//! @return None
//!
//! @note After selection of the RTC oscillator, a 2 second delay occurs before
//! the new setting is reflected in status. Therefore the CLKGEN.STATUS.OMODE
//! bit will not reflect the new status until after the 2s wait period.
//
//*****************************************************************************
void driver_rtc_osc_select(uint32_t osc)
{
    //
    // Set XT if flag is set.
    // Otherwise configure for LFRC.
    //
    if (osc) {
        REG_CLKGEN.OCTRL |= REG_CLKGEN_OCTRL_OSEL_M;
    } else {
        REG_CLKGEN.OCTRL &= ~REG_CLKGEN_OCTRL_OSEL_M;
    }
}

//*****************************************************************************
//
//! @brief Enable/Start the RTC oscillator.
//!
//! Starts the RTC oscillator.
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_start(void)
{
    //
    // Start the RTC Oscillator.
    //
    REG_RTC.RTCCTL &= ~REG_RTC_RTCCTL_RSTOP(1);
}

//*****************************************************************************
//
//! @brief Disable/Stop the RTC oscillator.
//!
//! Stops the RTC oscillator.
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_stop(void)
{
    //
    // Stop the RTC Oscillator.
    //
    REG_RTC.RTCCTL |= REG_RTC_RTCCTL_RSTOP(1);
}

//*****************************************************************************
//
//! @brief Set the Real Time Clock counter registers.
//!
//! @param *pTime - A pointer to the time structure.
//!
//! Sets the RTC counter registers to the supplied values.
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_time_set(driver_rtc_time_t *pTime)
{
    //
    // Enable writing to the counters.
    //
    REG_RTC.RTCCTL |= REG_RTC_RTCCTL_WRTC(1);

    //
    // Write the RTCLOW register.
    //
    REG_RTC.CTRLOW =
        REG_RTC_CTRLOW_CTRHR(dec_to_bcd(pTime->Hour))                |
        REG_RTC_CTRLOW_CTRMIN(dec_to_bcd(pTime->Minute))             |
        REG_RTC_CTRLOW_CTRSEC(dec_to_bcd(pTime->Second))             |
        REG_RTC_CTRLOW_CTR100(dec_to_bcd(pTime->Hundredths));

    //
    // Write the RTCUP register.
    //
    REG_RTC.CTRUP =
        REG_RTC_CTRUP_CEB((pTime->CenturyEnable))                   |
        REG_RTC_CTRUP_CB((pTime->Century))                          |
        REG_RTC_CTRUP_CTRWKDY((pTime->Weekday))                     |
        REG_RTC_CTRUP_CTRYR(dec_to_bcd((pTime->Year)))              |
        REG_RTC_CTRUP_CTRMO(dec_to_bcd((pTime->Month)))             |
        REG_RTC_CTRUP_CTRDATE(dec_to_bcd((pTime->Day)));

    //
    // Disable writing to the counters.
    //
    REG_RTC.RTCCTL |= REG_RTC_RTCCTL_WRTC(0);
}

//*****************************************************************************
//
//! @brief Get the Real Time Clock current time.
//!
//! @param *pTime - A pointer to the time structure to store the current time.
//!
//! Gets the RTC's current time
//!
//! @return 0 for success and 1 for error.
//
//*****************************************************************************
uint32_t driver_rtc_time_get(driver_rtc_time_t *pTime)
{
    uint32_t ui32RTCLow, ui32RTCUp, ui32Value;

    //
    // Read the upper and lower RTC registers.
    //
    ui32RTCLow = REG_RTC.CTRLOW;
    ui32RTCUp = REG_RTC.CTRUP;

    //
    // Break out the lower word.
    //
    ui32Value = REG_BFVG( RTC, CTRLOW, CTRHR, ui32RTCLow );
    pTime->Hour = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRLOW, CTRMIN, ui32RTCLow );
    pTime->Minute = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRLOW, CTRSEC, ui32RTCLow );
    pTime->Second = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRLOW, CTR100, ui32RTCLow );
    pTime->Hundredths = bcd_to_dec(ui32Value);

    //
    // Break out the upper word.
    //
    pTime->ReadError = REG_BFVG( RTC, CTRUP, CTERR, ui32RTCUp );

    pTime->CenturyEnable = REG_BFVG( RTC, CTRUP, CEB, ui32RTCUp );

    pTime->Century = REG_BFVG( RTC, CTRUP, CB, ui32RTCUp );

    ui32Value = REG_BFVG( RTC, CTRUP, CTRWKDY, ui32RTCUp );
    pTime->Weekday = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRUP, CTRYR, ui32RTCUp );
    pTime->Year = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRUP, CTRMO, ui32RTCUp );
    pTime->Month = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, CTRUP, CTRDATE, ui32RTCUp );
    pTime->Day = bcd_to_dec(ui32Value);

    //
    // Was there a read error?
    //
    if (pTime->ReadError) {
        return 1;
    } else {
        return 0;
    }
}

//*****************************************************************************
//
//! @brief Sets the alarm repeat interval.
//!
//! @param repeatInterval the desired repeat interval.
//!
//! Sets the alarm repeat interval.
//!
//! Valid values for repeatInterval:
//!
//!     DRIVER_RTC_ALM_RPT_DIS
//!     DRIVER_RTC_ALM_RPT_YR
//!     DRIVER_RTC_ALM_RPT_MTH
//!     DRIVER_RTC_ALM_RPT_WK
//!     DRIVER_RTC_ALM_RPT_DAY
//!     DRIVER_RTC_ALM_RPT_HR
//!     DRIVER_RTC_ALM_RPT_MIN
//!     DRIVER_RTC_ALM_RPT_SEC
//!     DRIVER_RTC_ALM_RPT_10TH
//!     DRIVER_RTC_ALM_RPT_100TH
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_alarm_interval_set(uint32_t repeatInterval)
{
    uint32_t ui32RptInt, ui32Alm100, ui32Value;

    switch (repeatInterval) {
        //
        // If repeat every 10th set RPT and ALM100 field accordinly
        //
        case DRIVER_RTC_ALM_RPT_10TH:
            ui32RptInt = DRIVER_RTC_ALM_RPT_SEC;
            ui32Alm100 = DRIVER_RTC_ALM100_10TH;
            break;
        //
        // If repeat every 100th set RPT and ALM100 field accordinly
        //
        case DRIVER_RTC_ALM_RPT_100TH:
            ui32RptInt = DRIVER_RTC_ALM_RPT_SEC;
            ui32Alm100 = DRIVER_RTC_ALM100_100TH;
            break;
        //
        // Otherwise set RPT as value passed.  ALM100 values need to be 0xnn
        // in this setting where n = 0-9.
        //
        default:
            //
            // Get the current value of the ALM100 field.
            //
            ui32Value = REG_BFR(RTC, ALMLOW, ALM100);

            //
            // If ALM100 was previous EVERY_10TH or EVERY_100TH reset to zero
            // otherwise keep previous setting.
            //
            ui32Alm100 = ui32Value >= 0xF0 ? 0 : ui32Value;

            //
            // Set RPT value to value passed.
            //
            ui32RptInt = repeatInterval;
            break;
    }

    //
    // Write the interval to the register.
    //
    REG_BFW(RTC, RTCCTL, RPT, ui32RptInt);

    //
    // Write the Alarm 100 bits in the ALM100 register.
    //
    REG_BFW(RTC, ALMLOW, ALM100, ui32Alm100);
}

//*****************************************************************************
//
//! @brief Sets the RTC's Alarm.
//!
//! @param *pTime - A pointer to the time structure.
//! @param repeatInterval - the desired alarm repeat interval.
//!
//! Set the Real Time Clock Alarm Parameters.
//!
//! Valid values for repeatInterval:
//!
//!     DRIVER_RTC_ALM_RPT_DIS
//!     DRIVER_RTC_ALM_RPT_YR
//!     DRIVER_RTC_ALM_RPT_MTH
//!     DRIVER_RTC_ALM_RPT_WK
//!     DRIVER_RTC_ALM_RPT_DAY
//!     DRIVER_RTC_ALM_RPT_HR
//!     DRIVER_RTC_ALM_RPT_MIN
//!     DRIVER_RTC_ALM_RPT_SEC
//!     DRIVER_RTC_ALM_RPT_10TH
//!     DRIVER_RTC_ALM_RPT_EVERY_100TH
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_alarm_set(driver_rtc_time_t *pTime, uint32_t repeatInterval)
{
    uint8_t ui8Value = 0;

    //
    // Write the interval to the register.
    //
    REG_RTC.RTCCTL |=
        REG_BFVS( RTC, RTCCTL, RPT, repeatInterval > 0x7 ? 0x7 : repeatInterval );

    //
    // Check if the interval is 10th or every 100th and track it in ui8Value.
    //
    if (repeatInterval == DRIVER_RTC_ALM_RPT_10TH) {
        ui8Value = 0xF0;
    } else if (repeatInterval == DRIVER_RTC_ALM_RPT_100TH) {
        ui8Value = 0xFF;
    }

    //
    // Write the ALMUP register.
    //
    REG_RTC.ALMUP =
        REG_BFVS( RTC, ALMUP, ALMWKDY, pTime->Weekday )                     |
        REG_BFVS( RTC, ALMUP, ALMMO,   dec_to_bcd(pTime->Month) )           |
        REG_BFVS( RTC, ALMUP, ALMDATE, dec_to_bcd(pTime->Day) );

    //
    // Write the ALMLOW register.
    //
    REG_RTC.ALMLOW =
        REG_BFVS( RTC, ALMLOW, ALMHR,  dec_to_bcd(pTime->Hour) )               |
        REG_BFVS( RTC, ALMLOW, ALMMIN, dec_to_bcd(pTime->Minute) )             |
        REG_BFVS( RTC, ALMLOW, ALMSEC, dec_to_bcd(pTime->Second) )             |
        REG_BFVS( RTC, ALMLOW, ALM100, dec_to_bcd(pTime->Hundredths) | ui8Value );
}

//*****************************************************************************
//
//! @brief Get the Real Time Clock Alarm Parameters
//!
//! @param *pTime - A pointer to the time structure to store the current alarm.
//!
//! Gets the RTC's Alarm time
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_alarm_get(driver_rtc_time_t *pTime)
{
    uint32_t ui32ALMLow, ui32ALMUp, ui32Value;

    //
    // Read the upper and lower RTC registers.
    //
    ui32ALMLow = REG_RTC.ALMLOW;
    ui32ALMUp = REG_RTC.ALMUP;

    //
    // Break out the lower word.
    //
    ui32Value = REG_BFVG( RTC, ALMLOW, ALMHR, ui32ALMLow );
    pTime->Hour = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, ALMLOW, ALMMIN, ui32ALMLow );
    pTime->Minute = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, ALMLOW, ALMSEC, ui32ALMLow );
    pTime->Second = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, ALMLOW, ALM100, ui32ALMLow );
    pTime->Hundredths = bcd_to_dec(ui32Value);

    //
    // Break out the upper word.
    //
    pTime->ReadError = 0;
    pTime->CenturyEnable = 0;
    pTime->Century = 0;

    ui32Value = REG_BFVG( RTC, ALMUP, ALMWKDY, ui32ALMUp );
    pTime->Weekday = bcd_to_dec(ui32Value);

    pTime->Year = 0;

    ui32Value = REG_BFVG( RTC, ALMUP, ALMMO, ui32ALMUp );
    pTime->Month = bcd_to_dec(ui32Value);

    ui32Value = REG_BFVG( RTC, ALMUP, ALMDATE, ui32ALMUp );
    pTime->Day = bcd_to_dec(ui32Value);
}

//*****************************************************************************
//
//! @brief Enable selected RTC interrupts.
//!
//! @param interr - desired interrupts
//!
//! Enables the RTC interrupts.
//!
//! interr should be an OR of the following:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_int_enable(uint32_t interr)
{
    //
    // Enable the interrupts.
    //
    REG_RTC.INTEN |= interr;
}

//*****************************************************************************
//
//! @brief Disable selected RTC interrupts.
//!
//! @param interr - desired interrupts
//!
//! Disables the RTC interrupts.
//!
//! interr should be an OR of the following:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_int_disable(uint32_t interr)
{
    //
    // Disable the interrupts.
    //
    REG_RTC.INTEN &= ~interr;
}

//*****************************************************************************
//
//! @brief Return the enabled RTC interrupts.
//!
//! Returns the enabled RTC interrupts.
//!
//! @return enabled RTC interrupts. Return is a logical or of:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//
//*****************************************************************************
uint32_t driver_rtc_int_enable_get(void)
{
    //
    // Read the RTC interrupt enable register, and return its contents.
    //
    return REG_RTC.INTEN;
}

//*****************************************************************************
//
//! @brief Sets the selected RTC interrupts.
//!
//! @param interr - desired interrupts
//!
//! Sets the RTC interrupts causing them to immediately trigger.
//!
//! interr should be an OR of the following:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_int_set(uint32_t interr)
{
    //
    // Set the interrupts.
    //
    REG_RTC.INTSET = interr;
}

//*****************************************************************************
//
//! @brief Clear selected RTC interrupts.
//!
//! @param interr - desired interrupts
//!
//! Clears the RTC interrupts.
//!
//! interr should be an OR of the following:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//!
//! @return None.
//
//*****************************************************************************
void driver_rtc_int_clear(uint32_t interr)
{
    //
    // Clear the interrupts.
    //
    REG_RTC.INTCLR = interr;
}

//*****************************************************************************
//
//! @brief Returns the RTC interrupt status.
//!
//! @param onlyEnabled - return the status of only the enabled interrupts.
//!
//! Returns the RTC interrupt status.
//!
//! @return Bitwise representation of the current interrupt status.
//!
//! The return value will be the logical OR of one or more of the following
//! values:
//!
//!     DRIVER_RTC_INT_ALM
//!     DRIVER_RTC_INT_OF
//!     DRIVER_RTC_INT_ACC
//!     DRIVER_RTC_INT_ACF
//
//*****************************************************************************
uint32_t driver_rtc_int_status_get(bool onlyEnabled)
{
    //
    // Get the interrupt status.
    //
    if (onlyEnabled) {
        uint32_t u32RetVal;
        u32RetVal  = REG_RTC.INTSTAT;
        u32RetVal &= REG_RTC.INTEN;
        return u32RetVal                &
                (DRIVER_RTC_INT_ALM     |
                 DRIVER_RTC_INT_OF      |
                 DRIVER_RTC_INT_ACC     |
                 DRIVER_RTC_INT_ACF);
    } else {
        return REG_RTC.INTSTAT & (DRIVER_RTC_INT_ALM |
                                  DRIVER_RTC_INT_OF  |
                                  DRIVER_RTC_INT_ACC |
                                  DRIVER_RTC_INT_ACF);
    }
}


/* EOF */
