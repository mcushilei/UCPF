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


#ifndef __DRIVER_ADC_C__
#ifndef __DRIVER_ADC_H__
#define __DRIVER_ADC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_adc.h"

/*============================ MACROS ========================================*/
#define ADC_CHANNEL_COUNT               (8u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
    ADC_CHANNEL_0   = 0,
    ADC_CHANNEL_1   = 1,
    ADC_CHANNEL_2   = 2,
    ADC_CHANNEL_3   = 3,
    ADC_CHANNEL_4   = 4,
    ADC_CHANNEL_5   = 5,
    ADC_CHANNEL_6   = 6,
    ADC_CHANNEL_7   = 7,
} em_adc_channel_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool adc_init(void);
extern bool adc_enable(void);
extern bool adc_disable(void);
extern bool adc_trige_single_convert(uint32_t wChannel);
extern bool adc_stop_convert(void);
extern bool adc_check_channel_statue(uint32_t wChannel);
extern uint32_t adc_get_channel_resault(uint32_t wChannel);

#endif  //! #ifndef __DRIVER_ADC_H__
#endif  //! #ifndef __DRIVER_ADC_C__
/* EOF */
