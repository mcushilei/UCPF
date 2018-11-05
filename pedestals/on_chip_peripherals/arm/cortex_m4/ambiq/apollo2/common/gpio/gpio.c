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
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_GPIO_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"
#include ".\gpio_public.h"

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
        SAFE_ATOM_CODE(                                                            \
            uint32_t wTemp = REG_GPIO.EN##__N & ~pins;                         \
            wTemp |= (dir & pins);                                       \
            REG_GPIO.EN##__N = wTemp;                                              \
        )                                                                           \
    }                                                                               \
    static uint32_t gpio##__N##_get_direction(uint32_t pins)                    \
    {                                                                               \
        return REG_GPIO.EN##__N;                                                   \
    }                                                                               \
    static void gpio##__N##_set_input(uint32_t pins)                            \
    {                                                                               \
        REG_GPIO.ENC##__N = pins;                                             \
    }                                                                               \
    static void gpio##__N##_set_output(uint32_t pins)                           \
    {                                                                               \
        REG_GPIO.ENS##__N = pins;                                              \
    }                                                                               \
    static uint32_t gpio##__N##_read(void)                                          \
    {                                                                               \
        return REG_GPIO.RD##__N;                                                    \
    }                                                                               \
    static void gpio##__N##_write(uint32_t value, uint32_t pins)               \
    {                                                                               \
        SAFE_ATOM_CODE(                                                            \
            uint32_t wTemp = REG_GPIO.WT##__N & (~pins);                         \
            wTemp |= value & pins;                                           \
            REG_GPIO.WT##__N = wTemp;                                              \
        )                                                                           \
    }                                                                               \
    static void gpio##__N##_set(uint32_t pins)                                  \
    {                                                                               \
        REG_GPIO.WTS##__N = pins;                                            \
    }                                                                               \
    static void gpio##__N##_clear(uint32_t pins)                                \
    {                                                                               \
        REG_GPIO.WTC##__N = pins;                                            \
    }                                                                               \
    static void gpio##__N##_toggle(uint32_t pins)                               \
    {                                                                               \
        SAFE_ATOM_CODE(                                                            \
            uint32_t wTemp = REG_GPIO.RD##__N;                         \
            wTemp ^= pins;                                           \
            REG_GPIO.WT##__N = wTemp;                                              \
        )                                                                           \
    }
    
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
MREPEAT_N2A(GPIO_COUNT, __GPIO_FUNC_PROTOTYPE, NULL)
static bool gpio_enable(void);
static bool gpio_disable(void);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
//! \brief io interface
const i_gpio_t GPIO = {
    &gpio_enable,
    &gpio_disable,
    {
        {MREPEAT_N2A(GPIO_COUNT, __GPIO_INTERFACE_INIT, NULL)}
    }
};

/*============================ IMPLEMENTATION ================================*/
MREPEAT_N2A(GPIO_COUNT, __GPIO_FUNC_DEF, NULL)

static bool gpio_enable(void)
{
    return true;
}

static bool gpio_disable(void)
{
    return false;
}


/* EOF */