/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include ".\reg_ioctrl.h"
#include ".\reg_gpio.h"
#include "./io.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#ifndef SAFE_ATOM_CODE
#define SAFE_ATOM_CODE(...) __VA_ARGS__
#endif

#define __GPIO_INTERFACE_DEF(__N, __VALUE)                              \
    {                                                                   \
        &gpio##__N##_set_direction,                                     \
        &gpio##__N##_get_direction,                                     \
        &gpio##__N##_set_input,                                         \
        &gpio##__N##_set_output,                                        \
        &gpio##__N##_reverse_direction,                                 \
        &gpio##__N##_read,                                              \
        &gpio##__N##_write,                                             \
        &gpio##__N##_set,                                               \
        &gpio##__N##_clear,                                             \
        &gpio##__N##_toggle,                                            \
    },

#define __GPIO_FUNC_PROTOTYPES(__N,__VALUE)                                     \
static void     gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask);  \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask);                   \
static void     gpio##__N##_set_input(uint32_t wPinMask);                       \
static void     gpio##__N##_set_output(uint32_t wPinMask);                      \
static void     gpio##__N##_reverse_direction(uint32_t wPinMask);               \
static uint32_t gpio##__N##_read(void);                                         \
static void     gpio##__N##_write(uint32_t wValue, uint32_t wPinMask);          \
static void     gpio##__N##_set(uint32_t wPinMask);                             \
static void     gpio##__N##_clear(uint32_t wPinMask);                           \
static void     gpio##__N##_toggle(uint32_t wPinMask);                       

#define __GPIO_FUNC_BODY(__N, __VALUE)                                          \
static void gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask)   \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        uint32_t wTemp = GPIO##__N##_REG.FIODIR & ~wPinMask;                      \
        wTemp |= (wDirection & wPinMask);                                       \
        GPIO##__N##_REG.FIODIR = wTemp;                                           \
    )                                                                           \
}                                                                               \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask)                    \
{                                                                               \
    return GPIO##__N##_REG.FIODIR;                                                \
}                                                                               \
static void gpio##__N##_set_input(uint32_t wPinMask)                            \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR &= ~wPinMask;                                      \
    )                                                                           \
}                                                                               \
static void gpio##__N##_set_output(uint32_t wPinMask)                           \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR |= wPinMask;                                       \
    )                                                                           \
}                                                                               \
static void gpio##__N##_reverse_direction(uint32_t wPinMask)                    \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR ^= wPinMask;                                       \
    )                                                                           \
}                                                                               \
                                                                                \
static uint32_t gpio##__N##_read(void)                                          \
{                                                                               \
    return GPIO##__N##_REG.FIOPIN;                                                \
}                                                                               \
static void gpio##__N##_write(uint32_t wValue, uint32_t wPinMask)               \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        uint32_t wTemp = GPIO##__N##_REG.FIOPIN & ~wPinMask;                      \
        wTemp |= (wValue & wPinMask);                                           \
        GPIO##__N##_REG.FIOPIN = wTemp;                                           \
    )                                                                           \
}                                                                               \
                                                                                \
static void gpio##__N##_set(uint32_t wPinMask)                                  \
{                                                                               \
    GPIO##__N##_REG.FIOSET = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_clear(uint32_t wPinMask)                                \
{                                                                               \
    GPIO##__N##_REG.FIOCLR = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_toggle(uint32_t wPinMask)                               \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIOPIN ^= wPinMask;                                       \
    )                                                                           \
}                                                                                                                                             

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static bool io_configuration( io_cfg_t *pConfig, uint32_t size );
MREPEAT(GPIO_COUNT, __GPIO_FUNC_PROTOTYPES, NULL)

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
const i_io_t GPIO = {
        {
            {MREPEAT(GPIO_COUNT, __GPIO_INTERFACE_DEF, NULL)}
        }
    };


const i_pin_t PIN = {
        &io_configuration,                  //!< general io configuration
    };

/*============================ IMPLEMENTATION ================================*/
MREPEAT(GPIO_COUNT, __GPIO_FUNC_BODY, NULL)

/*! \name io general configuration
 *! \param io configuration structure array
 *! \param size array size
 *! \retval true configuration succeed
 *! \retval false configuration fail
 */
static bool io_configuration(io_cfg_t *pConfig, uint32_t size)
{
    if (NULL == pConfig || 0u == size) {
        return false;
    }

    //! io configure
    for (; size != 0u; --size){
        uint32_t wFunction  = pConfig->Function;
        uint32_t wIOMODE    = pConfig->Mode;
        uint32_t wRegIndex, wBitIndex, wMask;

        wRegIndex = (pConfig->Port * 2u) + ((pConfig->Pin >> 4) & 0x01u);
        wBitIndex = (pConfig->Pin & 0x0Fu) * 2u;
        wMask = 0x03u << wBitIndex;
        PINCON_REG.PINSEL[wRegIndex]  = PINCON_REG.PINSEL[wRegIndex] & ~wMask | ((wFunction & 0x03u) << wBitIndex);
        PINCON_REG.PINMODE[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & ~wMask | ((wIOMODE & 0x03u) << wBitIndex);

        wRegIndex = pConfig->Port;
        wBitIndex = pConfig->Pin;
        wMask = 0x01u << wBitIndex;
        PINCON_REG.PINMODE_OD[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & ~wMask | (((wIOMODE >> 2) & 0x01u) << wBitIndex);
                
        pConfig++;
    }

    return true;
}


  
/* EOF */