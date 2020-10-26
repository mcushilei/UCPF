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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\sys\pm.h"
#include ".\gpio.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE_INIT(__N, __VALUE)                         \
    {                                                               \
        &gpio##__N##_set_direction,                                 \
        &gpio##__N##_get_direction,                                 \
        &gpio##__N##_set_input,                                     \
        &gpio##__N##_set_output,                                    \
        &gpio##__N##_read,                                          \
        &gpio##__N##_write,                                         \
        &gpio##__N##_set,                                           \
        &gpio##__N##_clear,                                         \
        &gpio##__N##_toggle,                                        \
    },

#define __GPIO_FUNC_PROTOTYPE(__N,__VALUE)                      \
    static void     gpio##__N##_set_direction(uint32_t dir, uint32_t pins);  \
    static uint32_t gpio##__N##_get_direction(uint32_t pins);                \
    static void     gpio##__N##_set_input(uint32_t pins);                    \
    static void     gpio##__N##_set_output(uint32_t pins);                   \
    static uint32_t gpio##__N##_read(void);                                 \
    static void     gpio##__N##_write(uint32_t value, uint32_t pins);       \
    static void     gpio##__N##_set(uint32_t pins);                          \
    static void     gpio##__N##_clear(uint32_t pins);                        \
    static void     gpio##__N##_toggle(uint32_t pins);                       

#define __GPIO_FUNC_DEF(__N, __VALUE)                                          \
    static void gpio##__N##_set_direction(uint32_t dir, uint32_t pins)   \
    {                                                                               \
            uint32_t wTemp = GPIO##__N##_REG.DIR & ~pins;                         \
            wTemp |= (dir & pins);                                       \
            GPIO##__N##_REG.DIR = wTemp;                                              \
    }                                                                               \
    static uint32_t gpio##__N##_get_direction(uint32_t pins)                    \
    {                                                                               \
        return GPIO##__N##_REG.DIR;                                                   \
    }                                                                               \
    static void gpio##__N##_set_input(uint32_t pins)                            \
    {                                                                               \
            GPIO##__N##_REG.DIR &= ~pins;                                             \
    }                                                                               \
    static void gpio##__N##_set_output(uint32_t pins)                           \
    {                                                                               \
            GPIO##__N##_REG.DIR |= pins;                                              \
    }                                                                               \
    static uint32_t gpio##__N##_read(void)                                          \
    {                                                                               \
        return GPIO##__N##_REG.PIN;                                                    \
    }                                                                               \
    static void gpio##__N##_write(uint32_t value, uint32_t pins)               \
    {                                                                               \
            uint32_t wTemp = GPIO##__N##_REG.PIN & (~pins);                         \
            wTemp |= value & pins;                                           \
            GPIO##__N##_REG.PIN = wTemp;                                              \
    }                                                                               \
    static void gpio##__N##_set(uint32_t pins)                                  \
    {                                                                               \
        GPIO##__N##_REG.SET = pins;                                            \
    }                                                                               \
    static void gpio##__N##_clear(uint32_t pins)                                \
    {                                                                               \
        GPIO##__N##_REG.CLR = pins;                                            \
    }                                                                               \
    static void gpio##__N##_toggle(uint32_t pins)                               \
    {                                                                               \
        GPIO##__N##_REG.NOT = pins;                                            \
    }
    
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
MREPEAT(IO_PORT_COUNT, __GPIO_FUNC_PROTOTYPE, NULL)
static bool gpio_enable(void);
static bool gpio_disable(void);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
//! \brief io interface
const i_gpio_t GPIO = {
    &gpio_enable,
    &gpio_disable,
    {
        {MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE_INIT, NULL)}
    }
};

/*============================ IMPLEMENTATION ================================*/
MREPEAT(IO_PORT_COUNT, __GPIO_FUNC_DEF, NULL)

static bool gpio_enable(void)
{
    return clock_enable(CLOCK_GPIO);
}

static bool gpio_disable(void)
{
    return clock_disable(CLOCK_GPIO);
}


/* EOF */