/***************************************************************************
 *   Copyright(C)2009-2013 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __IO_C__
#ifndef __IO_H__
#define __IO_H__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"
#include ".\reg_iocon.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define __GPIO_INTERFACE(__N, __VALUE)      i_gpio_t GPIO##__N;
#define __IO_PORT_NAM(__N, __OFFSET)        PORT##__N = (__N),
#define __IO_PIN_NAM(__N, __OFFSET)         PIN##__N = (__N),
#define __IO_PIN_MSK(__N, __OFFSET)         PIN##__N##_MSK = (1ul << (__N)),


#define IO_CFG(...)                     do {                            \
            static const io_cfg_t Config[] = {__VA_ARGS__};             \
            IO.Config(Config, ARRAY_LENGTH(Config));                          \
        } while(0)

/*============================ TYPES =========================================*/
//! \name pin mask
//! @{
typedef enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_MSK, 0)
} em_io_pin_msk_t;
//! @}

typedef enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_NAM, 0)
} em_io_pin_nam_t;

typedef enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PORT_NAM, 0)
} em_io_port_nam_t;

//! \name io configuration structure
//! @{
typedef struct {
    uint32_t        Port;
    uint32_t        PIN;           //!< pin number
    uint32_t        Function;      //!< io Funcitons
    uint32_t        Mode;          //!< io mode
} io_cfg_t;
//! @}

//! \name gpio control interface
//! @{
DEF_INTERFACE(i_gpio_t)
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
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
    bool            (*Config)(io_cfg_t const *ptCFG, uint32_t wSize);//!< io configuration
    union {
        i_gpio_t  GPIO[IO_PORT_COUNT];               //!< dedicated gpio control interface
        struct {
            MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE, 0)
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
