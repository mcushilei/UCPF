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


#ifndef __REG_WDT_H__
#define __REG_WDT_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define WDT_REG         (*(wdt_reg_t *)WDT_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! System Control Register File
typedef volatile struct {
    // 18: System control and status register
    reg16_t     SCSR;

    // 19: WD counter register
    reg16_t     WDCNTR;

    // 20: reserved
    REG16_RSVD(1)

    // 21: WD reset key register
    reg16_t     WDKEY;

    // 22-24: reserved
    REG16_RSVD(3)

    // 25: WD timer control register
    reg16_t     WDCR;

    // 26-31:  reserved
    REG16_RSVD(6)
} wdt_reg_t;

#define WDCR_WDFLAG_BIT0    7u
#define WDCR_WDFLAG_MSK     (1u << WDCR_WDFLAG_BIT0)
#define WDCR_WDDIS_BIT0     6u
#define WDCR_WDDIS_MSK      (1u << WDCR_WDDIS_BIT0)
#define WDCR_WDCHK_BIT0     5u
#define WDCR_WDCHK_BIT1     4u
#define WDCR_WDCHK_BIT2     3u
#define WDCR_WDCHK_MSK      ((1u << WDCR_WDCHK_BIT0) \
                            |(1u << WDCR_WDCHK_BIT1) \
                            |(1u << WDCR_WDCHK_BIT2))
#define WDCR_WDCHK_VALUE    (0x5u << WDCR_WDCHK_BIT0)

typedef enum {
    WDPS_PRESCALE1 = 0,
    WDPS_PRESCALE2 = 2,
    WDPS_PRESCALE4,
    WDPS_PRESCALE8,
    WDPS_PRESCALE16,
    WDPS_PRESCALE32,
    WDPS_PRESCALE64,
} em_wdps_t;

#define WDCR_WDPS_BIT0      0u
#define WDCR_WDPS_BIT1      1u
#define WDCR_WDPS_BIT2      2u
#define WDCR_WDPS_MSK       ((1u << WDCR_WDPS_BIT0) \
                            |(1u << WDCR_WDPS_BIT1) \
                            |(1u << WDCR_WDPS_BIT2))

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
