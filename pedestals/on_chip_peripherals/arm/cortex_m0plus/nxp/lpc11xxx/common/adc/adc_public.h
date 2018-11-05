/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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




#ifndef __DRIVER_ADC_PUBLIC_H__
#define __DRIVER_ADC_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define ADC_MAX_CHANNELS    (12u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool adc_init(void);
extern bool adc_enable(void);
extern bool adc_disable(void);
extern bool adc_trige_single_convert(uint32_t wChannel);
extern bool adc_stop_convert(void);
extern bool adc_is_work_ongoing(uint32_t ch);
extern uint32_t adc_get_resault(uint32_t ch);

#endif  //! #ifndef __DRIVER_ADC_PUBLIC_H__
/* EOF */
