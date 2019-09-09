/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __REG_RTC_H__
#define __REG_RTC_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define RTC_REG                (*(rtc_reg_t *)RTC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name register page type
//! @{
typedef volatile struct {
    uint8_t  ILR;
    REG8_RSVD(7)
    uint8_t  CCR;
    REG8_RSVD(3)
    uint8_t  CIIR;
    REG8_RSVD(3)
    uint8_t  AMR;
    REG8_RSVD(3)
        
    uint32_t CTIME0;
    uint32_t CTIME1;
    uint32_t CTIME2;
    
    uint8_t  SEC;
    REG8_RSVD(3)
    uint8_t  MIN;
    REG8_RSVD(3)
    uint8_t  HOUR;
    REG8_RSVD(3)
    uint8_t  DOM;
    REG8_RSVD(3)
    uint8_t  DOW;
    REG8_RSVD(3)
    uint16_t DOY;
    REG16_RSVD(1)
    uint8_t  MONTH;
    REG8_RSVD(3)
    uint16_t YEAR;
    REG16_RSVD(1)
        
    uint32_t CALIBRATION;
    
    uint32_t GPREG0;
    uint32_t GPREG1;
    uint32_t GPREG2;
    uint32_t GPREG3;
    uint32_t GPREG4;
    
    uint8_t  RTC_AUXEN;
    REG8_RSVD(3)
    uint8_t  RTC_AUX;
    REG8_RSVD(3)
        
    uint8_t  ALSEC;
    REG8_RSVD(3)
    uint8_t  ALMIN;
    REG8_RSVD(3)
    uint8_t  ALHOUR;
    REG8_RSVD(3)
    uint8_t  ALDOM;
    REG8_RSVD(3)
    uint8_t  ALDOW;
    REG8_RSVD(3)
    uint16_t ALDOY;
    REG16_RSVD(1)
    uint8_t  ALMON;
    REG8_RSVD(3)
    uint16_t ALYEAR;
    REG16_RSVD(1)
        
} rtc_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif