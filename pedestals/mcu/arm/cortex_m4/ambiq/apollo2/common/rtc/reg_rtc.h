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
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_RTC_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_RTC_H__



/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_RTC           (*(reg_rtc_t *)REG_RTC_BASEADDR)

#define REG_RTCn(n)       (*(reg_rtc_t *)(REG_RTC_BASEADDR + 0x00000000 * n))


//*****************************************************************************
//
// RTC_INTEN - RTC Interrupt Register: Enable
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_RTC_INTEN_ALM_S                       3
#define REG_RTC_INTEN_ALM_M                       0x00000008
#define REG_RTC_INTEN_ALM(n)                      (((uint32_t)(n) << 3) & 0x00000008)

// XT Oscillator Fail interrupt
#define REG_RTC_INTEN_OF_S                        2
#define REG_RTC_INTEN_OF_M                        0x00000004
#define REG_RTC_INTEN_OF(n)                       (((uint32_t)(n) << 2) & 0x00000004)

// Autocalibration Complete interrupt
#define REG_RTC_INTEN_ACC_S                       1
#define REG_RTC_INTEN_ACC_M                       0x00000002
#define REG_RTC_INTEN_ACC(n)                      (((uint32_t)(n) << 1) & 0x00000002)

// Autocalibration Fail interrupt
#define REG_RTC_INTEN_ACF_S                       0
#define REG_RTC_INTEN_ACF_M                       0x00000001
#define REG_RTC_INTEN_ACF(n)                      (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// RTC_INTSTAT - RTC Interrupt Register: Status
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_RTC_INTSTAT_ALM_S                     3
#define REG_RTC_INTSTAT_ALM_M                     0x00000008
#define REG_RTC_INTSTAT_ALM(n)                    (((uint32_t)(n) << 3) & 0x00000008)

// XT Oscillator Fail interrupt
#define REG_RTC_INTSTAT_OF_S                      2
#define REG_RTC_INTSTAT_OF_M                      0x00000004
#define REG_RTC_INTSTAT_OF(n)                     (((uint32_t)(n) << 2) & 0x00000004)

// Autocalibration Complete interrupt
#define REG_RTC_INTSTAT_ACC_S                     1
#define REG_RTC_INTSTAT_ACC_M                     0x00000002
#define REG_RTC_INTSTAT_ACC(n)                    (((uint32_t)(n) << 1) & 0x00000002)

// Autocalibration Fail interrupt
#define REG_RTC_INTSTAT_ACF_S                     0
#define REG_RTC_INTSTAT_ACF_M                     0x00000001
#define REG_RTC_INTSTAT_ACF(n)                    (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// RTC_INTCLR - RTC Interrupt Register: Clear
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_RTC_INTCLR_ALM_S                      3
#define REG_RTC_INTCLR_ALM_M                      0x00000008
#define REG_RTC_INTCLR_ALM(n)                     (((uint32_t)(n) << 3) & 0x00000008)

// XT Oscillator Fail interrupt
#define REG_RTC_INTCLR_OF_S                       2
#define REG_RTC_INTCLR_OF_M                       0x00000004
#define REG_RTC_INTCLR_OF(n)                      (((uint32_t)(n) << 2) & 0x00000004)

// Autocalibration Complete interrupt
#define REG_RTC_INTCLR_ACC_S                      1
#define REG_RTC_INTCLR_ACC_M                      0x00000002
#define REG_RTC_INTCLR_ACC(n)                     (((uint32_t)(n) << 1) & 0x00000002)

// Autocalibration Fail interrupt
#define REG_RTC_INTCLR_ACF_S                      0
#define REG_RTC_INTCLR_ACF_M                      0x00000001
#define REG_RTC_INTCLR_ACF(n)                     (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// RTC_INTSET - RTC Interrupt Register: Set
//
//*****************************************************************************
// RTC Alarm interrupt
#define REG_RTC_INTSET_ALM_S                      3
#define REG_RTC_INTSET_ALM_M                      0x00000008
#define REG_RTC_INTSET_ALM(n)                     (((uint32_t)(n) << 3) & 0x00000008)

// XT Oscillator Fail interrupt
#define REG_RTC_INTSET_OF_S                       2
#define REG_RTC_INTSET_OF_M                       0x00000004
#define REG_RTC_INTSET_OF(n)                      (((uint32_t)(n) << 2) & 0x00000004)

// Autocalibration Complete interrupt
#define REG_RTC_INTSET_ACC_S                      1
#define REG_RTC_INTSET_ACC_M                      0x00000002
#define REG_RTC_INTSET_ACC(n)                     (((uint32_t)(n) << 1) & 0x00000002)

// Autocalibration Fail interrupt
#define REG_RTC_INTSET_ACF_S                      0
#define REG_RTC_INTSET_ACF_M                      0x00000001
#define REG_RTC_INTSET_ACF(n)                     (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// RTC_CTRLOW - RTC Counters Lower
//
//*****************************************************************************
// Hours Counter
#define REG_RTC_CTRLOW_CTRHR_S                    24
#define REG_RTC_CTRLOW_CTRHR_M                    0x3F000000
#define REG_RTC_CTRLOW_CTRHR(n)                   (((uint32_t)(n) << 24) & 0x3F000000)

// Minutes Counter
#define REG_RTC_CTRLOW_CTRMIN_S                   16
#define REG_RTC_CTRLOW_CTRMIN_M                   0x007F0000
#define REG_RTC_CTRLOW_CTRMIN(n)                  (((uint32_t)(n) << 16) & 0x007F0000)

// Seconds Counter
#define REG_RTC_CTRLOW_CTRSEC_S                   8
#define REG_RTC_CTRLOW_CTRSEC_M                   0x00007F00
#define REG_RTC_CTRLOW_CTRSEC(n)                  (((uint32_t)(n) << 8) & 0x00007F00)

// 100ths of a second Counter
#define REG_RTC_CTRLOW_CTR100_S                   0
#define REG_RTC_CTRLOW_CTR100_M                   0x000000FF
#define REG_RTC_CTRLOW_CTR100(n)                  (((uint32_t)(n) << 0) & 0x000000FF)

//*****************************************************************************
//
// RTC_CTRUP - RTC Counters Upper
//
//*****************************************************************************
// Counter read error status
#define REG_RTC_CTRUP_CTERR_S                     31
#define REG_RTC_CTRUP_CTERR_M                     0x80000000
#define REG_RTC_CTRUP_CTERR(n)                    (((uint32_t)(n) << 31) & 0x80000000)
#define REG_RTC_CTRUP_CTERR_NOERR                 0x00000000
#define REG_RTC_CTRUP_CTERR_RDERR                 0x80000000

// Century enable
#define REG_RTC_CTRUP_CEB_S                       28
#define REG_RTC_CTRUP_CEB_M                       0x10000000
#define REG_RTC_CTRUP_CEB(n)                      (((uint32_t)(n) << 28) & 0x10000000)
#define REG_RTC_CTRUP_CEB_DIS                     0x00000000
#define REG_RTC_CTRUP_CEB_EN                      0x10000000

// Century
#define REG_RTC_CTRUP_CB_S                        27
#define REG_RTC_CTRUP_CB_M                        0x08000000
#define REG_RTC_CTRUP_CB(n)                       (((uint32_t)(n) << 27) & 0x08000000)
#define REG_RTC_CTRUP_CB_2000                     0x00000000
#define REG_RTC_CTRUP_CB_1900_2100                0x08000000

// Weekdays Counter
#define REG_RTC_CTRUP_CTRWKDY_S                   24
#define REG_RTC_CTRUP_CTRWKDY_M                   0x07000000
#define REG_RTC_CTRUP_CTRWKDY(n)                  (((uint32_t)(n) << 24) & 0x07000000)

// Years Counter
#define REG_RTC_CTRUP_CTRYR_S                     16
#define REG_RTC_CTRUP_CTRYR_M                     0x00FF0000
#define REG_RTC_CTRUP_CTRYR(n)                    (((uint32_t)(n) << 16) & 0x00FF0000)

// Months Counter
#define REG_RTC_CTRUP_CTRMO_S                     8
#define REG_RTC_CTRUP_CTRMO_M                     0x00001F00
#define REG_RTC_CTRUP_CTRMO(n)                    (((uint32_t)(n) << 8) & 0x00001F00)

// Date Counter
#define REG_RTC_CTRUP_CTRDATE_S                   0
#define REG_RTC_CTRUP_CTRDATE_M                   0x0000003F
#define REG_RTC_CTRUP_CTRDATE(n)                  (((uint32_t)(n) << 0) & 0x0000003F)

//*****************************************************************************
//
// RTC_ALMLOW - RTC Alarms Lower
//
//*****************************************************************************
// Hours Alarm
#define REG_RTC_ALMLOW_ALMHR_S                    24
#define REG_RTC_ALMLOW_ALMHR_M                    0x3F000000
#define REG_RTC_ALMLOW_ALMHR(n)                   (((uint32_t)(n) << 24) & 0x3F000000)

// Minutes Alarm
#define REG_RTC_ALMLOW_ALMMIN_S                   16
#define REG_RTC_ALMLOW_ALMMIN_M                   0x007F0000
#define REG_RTC_ALMLOW_ALMMIN(n)                  (((uint32_t)(n) << 16) & 0x007F0000)

// Seconds Alarm
#define REG_RTC_ALMLOW_ALMSEC_S                   8
#define REG_RTC_ALMLOW_ALMSEC_M                   0x00007F00
#define REG_RTC_ALMLOW_ALMSEC(n)                  (((uint32_t)(n) << 8) & 0x00007F00)

// 100ths of a second Alarm
#define REG_RTC_ALMLOW_ALM100_S                   0
#define REG_RTC_ALMLOW_ALM100_M                   0x000000FF
#define REG_RTC_ALMLOW_ALM100(n)                  (((uint32_t)(n) << 0) & 0x000000FF)

//*****************************************************************************
//
// RTC_ALMUP - RTC Alarms Upper
//
//*****************************************************************************
// Weekdays Alarm
#define REG_RTC_ALMUP_ALMWKDY_S                   16
#define REG_RTC_ALMUP_ALMWKDY_M                   0x00070000
#define REG_RTC_ALMUP_ALMWKDY(n)                  (((uint32_t)(n) << 16) & 0x00070000)

// Months Alarm
#define REG_RTC_ALMUP_ALMMO_S                     8
#define REG_RTC_ALMUP_ALMMO_M                     0x00001F00
#define REG_RTC_ALMUP_ALMMO(n)                    (((uint32_t)(n) << 8) & 0x00001F00)

// Date Alarm
#define REG_RTC_ALMUP_ALMDATE_S                   0
#define REG_RTC_ALMUP_ALMDATE_M                   0x0000003F
#define REG_RTC_ALMUP_ALMDATE(n)                  (((uint32_t)(n) << 0) & 0x0000003F)

//*****************************************************************************
//
// RTC_RTCCTL - RTC Control Register
//
//*****************************************************************************
// Hours Counter mode
#define REG_RTC_RTCCTL_HR1224_S                   5
#define REG_RTC_RTCCTL_HR1224_M                   0x00000020
#define REG_RTC_RTCCTL_HR1224(n)                  (((uint32_t)(n) << 5) & 0x00000020)
#define REG_RTC_RTCCTL_HR1224_24HR                0x00000000
#define REG_RTC_RTCCTL_HR1224_12HR                0x00000020

// RTC input clock control
#define REG_RTC_RTCCTL_RSTOP_S                    4
#define REG_RTC_RTCCTL_RSTOP_M                    0x00000010
#define REG_RTC_RTCCTL_RSTOP(n)                   (((uint32_t)(n) << 4) & 0x00000010)
#define REG_RTC_RTCCTL_RSTOP_RUN                  0x00000000
#define REG_RTC_RTCCTL_RSTOP_STOP                 0x00000010

// Alarm repeat interval
#define REG_RTC_RTCCTL_RPT_S                      1
#define REG_RTC_RTCCTL_RPT_M                      0x0000000E
#define REG_RTC_RTCCTL_RPT(n)                     (((uint32_t)(n) << 1) & 0x0000000E)
#define REG_RTC_RTCCTL_RPT_DIS                    0x00000000
#define REG_RTC_RTCCTL_RPT_YEAR                   0x00000002
#define REG_RTC_RTCCTL_RPT_MONTH                  0x00000004
#define REG_RTC_RTCCTL_RPT_WEEK                   0x00000006
#define REG_RTC_RTCCTL_RPT_DAY                    0x00000008
#define REG_RTC_RTCCTL_RPT_HR                     0x0000000A
#define REG_RTC_RTCCTL_RPT_MIN                    0x0000000C
#define REG_RTC_RTCCTL_RPT_SEC                    0x0000000E

// Counter write control
#define REG_RTC_RTCCTL_WRTC_S                     0
#define REG_RTC_RTCCTL_WRTC_M                     0x00000001
#define REG_RTC_RTCCTL_WRTC(n)                    (((uint32_t)(n) << 0) & 0x00000001)
#define REG_RTC_RTCCTL_WRTC_DIS                   0x00000000
#define REG_RTC_RTCCTL_WRTC_EN                    0x00000001

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef volatile struct {
    REG32_RSVD(16)
    reg32_t CTRLOW;
    reg32_t CTRUP;
    reg32_t ALMLOW;
    reg32_t ALMUP;
    reg32_t RTCCTL;
    REG32_RSVD(43)
    reg32_t INTEN;
    reg32_t INTSTAT;
    reg32_t INTCLR;
    reg32_t INTSET;
} reg_rtc_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_RTC_H__
/* EOF */
