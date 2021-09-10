/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_GPIO_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_GPIO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      gpio_interface_t PORT##__N;

/*============================ TYPES =========================================*/
//! \name gpio control interface
//! @{
typedef struct {
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
    uint32_t    (*Read)(void);
    void        (*Write)(uint32_t wValue, uint32_t wPinMask);
    void        (*Set)(uint32_t wPinMask);
    void        (*Clear)(uint32_t wPinMask);
    void        (*Toggle)(uint32_t wPinMask);
} gpio_interface_t;
//! @}

//! \name csc user interface
//! @{
typedef struct {
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    union {
        gpio_interface_t  PORT[IO_PORT_COUNT];               //!< dedicated gpio control interface
        struct {
            MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE, 0)
        };
    };
} i_gpio_t;
//! @}

/*============================ PUBLIC VARIABLES ==============================*/
extern const i_gpio_t GPIO;

/*============================ PUBLIC PROTOTYPES =============================*/

#endif  // #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_GPIO_H__
/* EOF */
