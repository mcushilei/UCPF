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

#ifndef __DRIVER_LPC17XX_IO_C__
#ifndef __DRIVER_LPC17XX_IO_H__
#define __DRIVER_LPC17XX_IO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"
#include ".\reg_ioctrl.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      const i_gpio_t GPIO##__N;

#define __IO_PORT_NAM(__N, __OFFSET)        PORT##__N = (__N),
#define __IO_PIN_NAM(__N, __OFFSET)         PIN##__N = (__N),
#define __IO_PIN_MSK(__N, __OFFSET)         PIN##__N##_MSK = (1ul << (__N)),

#define IO_CFG(...)                                                 \
    do {                                                            \
        io_cfg_t tCFG[] = {__VA_ARGS__};                            \
        IO.Config(tCFG, ARRAY_LENGTH(tCFG));                        \
    } while(0)

/*============================ TYPES =========================================*/
enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_MSK, 0)
};

enum {
    MREPEAT(IO_PORT_COUNT, __IO_PORT_NAM, 0)
};

enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_NAM, 0)
};

//! \name io configuration structure
//! @{
typedef struct {
    uint8_t         Port;                  //!< port number
    uint8_t         Pin;                   //!< pin number
    uint8_t         Function;              //!< io Funcitons
    uint8_t         Mode;                  //!< io mode
} io_cfg_t;
//! @}

//! \name gpio control interface
//! @{
DEF_INTERFACE(i_gpio_t)
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
    void        (*ReverseDirection)(uint32_t wPinMask);
    uint32_t    (*Read)(void);
    void        (*Write)(uint32_t wValue, uint32_t wPinMask);
    void        (*Set)(uint32_t wPinMask);
    void        (*Clear)(uint32_t wPinMask);
    void        (*Toggle)(uint32_t wPinMask);
END_DEF_INTERFACE(i_gpio_t)
//! @}

//! \name csc user interface
//! @{
DEF_INTERFACE(i_io_t)
    //! general io configuration
    bool            (*Config)(io_cfg_t *pConfig, uint32_t size);
    union {
        const i_gpio_t  GPIO[GPIO_COUNT];               //!< dedicated gpio control interface
        struct {
            MREPEAT(GPIO_COUNT, __GPIO_INTERFACE, NULL)
        };
    };
END_DEF_INTERFACE(i_io_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
extern const i_io_t IO;

/*============================ PROTOTYPES ====================================*/

#endif
#endif
/* EOF */
