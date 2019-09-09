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


#ifndef __DRIVER_ADC_H__
#define __DRIVER_ADC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_adc.h"

/*============================ MACROS ========================================*/
#define ADC_MAX_CHANNELs    (8u)

#define ADC_CHANNEL_0       (1u << 0)
#define ADC_CHANNEL_1       (1u << 1)
#define ADC_CHANNEL_2       (1u << 2)
#define ADC_CHANNEL_3       (1u << 3)
#define ADC_CHANNEL_4       (1u << 4)
#define ADC_CHANNEL_5       (1u << 5)
#define ADC_CHANNEL_6       (1u << 6)
#define ADC_CHANNEL_7       (1u << 7)

#define ADC_TRIGGER_BURST   (0u << 24)
#define ADC_TRIGGER_ONCE    (1u << 24)
#define ADC_TRIGGER_EINT0   (2u << 24)
#define ADC_TRIGGER_CAP0_1  (3u << 24)
#define ADC_TRIGGER_MAT0_1  (4u << 24)
#define ADC_TRIGGER_MAT0_3  (5u << 24)
#define ADC_TRIGGER_MAT1_0  (6u << 24)
#define ADC_TRIGGER_MAT1_1  (7u << 24)

#define ADC_TRUGGER_ON_FALLING  (1u << 27)
#define ADC_TRUGGER_ON_RISING   (0u << 27)

#define ADC_INT_EN_CHN0     (1u << 0)
#define ADC_INT_EN_CHN1     (1u << 1)
#define ADC_INT_EN_CHN2     (1u << 2)
#define ADC_INT_EN_CHN3     (1u << 3)
#define ADC_INT_EN_CHN4     (1u << 4)
#define ADC_INT_EN_CHN5     (1u << 5)
#define ADC_INT_EN_CHN6     (1u << 6)
#define ADC_INT_EN_CHN7     (1u << 7)
#define ADC_INT_EN_GLOBAL   (1u << 8)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define DRIVER_ADC_DONE_FLAG_IS_SET(__CHN)       (ADC_REG.STAT & (__CHN))

#define DRIVER_ADC_INI_FLAG_IS_SET(__INT)       (ADC_REG.STAT & (__INT))

/*============================ TYPES =========================================*/

typedef struct {
    uint32_t ClockFrq;
    uint32_t Channels;
    uint32_t TriggerSource;
    uint32_t TriggerEdge;
    uint32_t EnableInterrupt;
} driver_adc_cfg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool adc_init(driver_adc_cfg_t *cfg);
extern bool adc_deinit(void);
extern uint16_t adc_get_channel_resault(uint32_t wChannel);

#endif  //! #ifndef __DRIVER_ADC_H__
/* EOF */
