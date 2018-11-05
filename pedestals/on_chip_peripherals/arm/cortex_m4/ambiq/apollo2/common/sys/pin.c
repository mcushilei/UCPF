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

#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_PIN_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\gpio\reg_gpio.h"
#include ".\pin_public.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//
// Extraction macros
//
#define CFGVAL_PADREG_X(x)          (((uint32_t)(x) & __PIN_CFGVAL_PADREG_M)  >>  \
                                     __PIN_CFGVAL_PADREG_S)
#define CFGVAL_CFG_X(x)             (((uint32_t)(x) & __PIN_CFGVAL_CFG_M) >>  \
                                     __PIN_CFGVAL_CFG_S)
#define CFGVAL_ALTPAD_X(x)          (((uint32_t)(x) & __PIN_CFGVAL_ALTPAD_M)  >>  \
                                     __PIN_CFGVAL_ALTPAD_S)

//*****************************************************************************
//
// PADREG helper macros.
//
//*****************************************************************************
#define __DRIVER_GPIO_PADREG(n)                                                 \
    (REG_GPIO.PADREG[(uint32_t)(n) / 4u])

#define __DRIVER_GPIO_PADREG_S(n)                                               \
    ( ((uint32_t)(n) % 4) * 8u )

#define __DRIVER_GPIO_PADREG_M(n)                                               \
    ( (uint32_t)0xFF << __DRIVER_GPIO_PADREG_S(n) )

#define __DEVICE_GPIO_PADREG_FIELD(n, configval)                                \
    ( (CFGVAL_PADREG_X(configval) << __DRIVER_GPIO_PADREG_S(n)) & __DRIVER_GPIO_PADREG_M(n) )

#define DRIVER_GPIO_PADREG_W(n, configval)                                    \
    __DRIVER_GPIO_PADREG(n) =                                       \
    ( __DRIVER_GPIO_PADREG(n) & ~__DRIVER_GPIO_PADREG_M(n) )    |   \
    __DEVICE_GPIO_PADREG_FIELD(n, configval)

#define DRIVER_GPIO_PADREG_R(n)                                     \
    ( ( __DRIVER_GPIO_PADREG(n) & __DRIVER_GPIO_PADREG_M(n) )   >>  \
    __DRIVER_GPIO_PADREG_S(n) )

//*****************************************************************************
//
// CFG helper macros.
//
//*****************************************************************************
#define __DRIVER_GPIO_CFG(n)                                                    \
    (REG_GPIO.CFG[(uint32_t)(n) / 8u])

#define __DRIVER_GPIO_CFG_S(n)                                                  \
    ( ((uint32_t)(n) % 8) * 4u )

#define __DRIVER_GPIO_CFG_M(n)                                                  \
    ( (uint32_t) 0x7 << __DRIVER_GPIO_CFG_S(n) )

#define __DRIVER_GPIO_CFG_FIELD(n, configval)                                   \
    ( (CFGVAL_CFG_X(configval) << __DRIVER_GPIO_CFG_S(n)) & __DRIVER_GPIO_CFG_M(n) )

#define DRIVER_GPIO_CFG_W(n, configval)                         \
    __DRIVER_GPIO_CFG(n) =                                      \
    ( __DRIVER_GPIO_CFG(n) & ~__DRIVER_GPIO_CFG_M(n) )      |   \
    __DRIVER_GPIO_CFG_FIELD(n, configval)

#define DRIVER_GPIO_CFG_R(n)                                    \
    ( ( __DRIVER_GPIO_CFG(n) & __DRIVER_GPIO_CFG_M(n) )     >>  \
    __DRIVER_GPIO_CFG_S(n) )


//*****************************************************************************
//
// ALTPADCFG helper macros.
// The ALTPADCFG bits are located in [23:16] of the configval.
//
//*****************************************************************************
#define __DRIVER_GPIO_ALTPADREG(n)                                              \
    (REG_GPIO.ALTPADCFG[(uint32_t)(n) / 4u])

#define __DRIVER_GPIO_ALTPADREG_S(n)                                            \
    ( ((uint32_t)(n) % 4) * 8u )

#define __DRIVER_GPIO_ALTPADREG_M(n)                                            \
    ( (uint32_t) 0xFF << __DRIVER_GPIO_ALTPADREG_S(n) )

#define __DRIVER_GPIO_ALTPADREG_FIELD(n, configval)                             \
    ( (CFGVAL_ALTPAD_X(configval) << __DRIVER_GPIO_ALTPADREG_S(n)) & __DRIVER_GPIO_ALTPADREG_M(n) )

#define DRIVER_GPIO_ALTPADREG_W(n, configval)                                 \
    __DRIVER_GPIO_ALTPADREG(n) =                                        \
    ( __DRIVER_GPIO_ALTPADREG(n) & ~__DRIVER_GPIO_ALTPADREG_M(n) )  |   \
    __DRIVER_GPIO_ALTPADREG_FIELD(n, configval)

#define DRIVER_GPIO_ALTPADREG_R(n)                                      \
    ( ( __DRIVER_GPIO_ALTPADREG(n) & __DRIVER_GPIO_ALTPADREG_M(n) ) >>  \
     __DRIVER_GPIO_ALTPADREG_S(n) )
        

/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
static bool pin_configurate(pin_cfg_t const *pConfig, uint32_t size);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
//! \brief io interface
const i_pin_t PIN = {
    &pin_configurate,
};

/*============================ IMPLEMENTATION ================================*/

/*! \param  io configuration structure array
 *! \param  chSize array size
 *! \retval true configuration succeed
 *! \retval false configuration fail
 */
static bool pin_configurate(pin_cfg_t const *pConfig, uint32_t size)
{
    if (NULL == pConfig) {
        return false;
    }

    for (; size; --size) {
        uint32_t ui32PinNumber      = pConfig->Pin;
        uint32_t ui32Config         = pConfig->Config;
        
        if ( ui32PinNumber < PAD_COUNT ) {
            SAFE_ATOM_CODE(
                REG_GPIO.PADKEY = REG_GPIO_PADKEY_KEYVAL;
                DRIVER_GPIO_PADREG_W(ui32PinNumber, ui32Config);
                DRIVER_GPIO_CFG_W(ui32PinNumber, ui32Config);
                DRIVER_GPIO_ALTPADREG_W(ui32PinNumber, ui32Config);
                REG_GPIO.PADKEY = 0;
            )
        }
        
        pConfig++;
    }

    return true;
}


  
/* EOF */