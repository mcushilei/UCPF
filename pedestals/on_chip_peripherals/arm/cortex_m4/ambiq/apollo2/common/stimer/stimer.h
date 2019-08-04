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
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_STIMER_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_STIMER_H__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_stimer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

extern uint32_t driver_stimer_config        (uint32_t ui32STimerConfig);
extern uint32_t driver_stimer_counter_get   (void);
extern void     driver_stimer_counter_clear (void);
extern void     driver_stimer_compare_delta_set(uint32_t ui32CmprInstance, uint32_t ui32Delta);
extern uint32_t driver_stimer_compare_get   (uint32_t ui32CmprInstance);
extern void     driver_stimer_capture_start (uint32_t ui32CaptureNum,
                            uint32_t ui32GPIONumber,
                            bool bPolarity);
extern void     driver_stimer_capture_stop  (uint32_t ui32CaptureNum);
extern uint32_t driver_stimer_capture_get   (uint32_t ui32CaptureNum);
extern void     driver_stimer_int_enable    (uint32_t ui32Interrupt);
extern uint32_t driver_stimer_int_enable_get(void);
extern void     driver_stimer_int_disable   (uint32_t ui32Interrupt);
extern void     driver_stimer_int_set       (uint32_t ui32Interrupt);
extern void     driver_stimer_int_clear     (uint32_t ui32Interrupt);
extern uint32_t driver_stimer_int_status_get(bool bEnabledOnly);


#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_STIMER_H__
/* EOF */
