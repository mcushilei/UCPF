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

#ifndef __DRIVER_ARM_M0_ISTARCHIP_ES_XN12XX_IO_H__
#define __DRIVER_ARM_M0_ISTARCHIP_ES_XN12XX_IO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"
#include ".\reg_ioctrl.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      const i_gpio_t GPIO##__N;

#define __IO_PORT_PIN_MSK(__N, __OFFSET)    PIN##__N##_MSK = (1ul << (__N)),

//! \brief enable pull-up resistor
#define IOCTRL_ENABLE_PULL_UP(__PIN)        do {                                \
            uint32_t wRegIndex, wBitIndex, wMask;                               \
            wRegIndex = __PIN >> 4;                                             \
            wBitIndex = __PIN & (16 - 1);                                       \
            wMask = 3ul << wBitIndex;                                           \
            PINCON_REG.PINMODE[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & wMask | (IO_MOD_PULL_UP & 0x03ul);\
        } while(0)

//! \brief disable pull-up resistor
#define IOCTRL_DISABLE_PULL_UP(__PIN)       do {                                \
            uint32_t wRegIndex, wBitIndex, wMask;                               \
            wRegIndex = __PIN >> 4;                                             \
            wBitIndex = __PIN & (16 - 1);                                       \
            wMask = 3ul << wBitIndex;                                           \
            PINCON_REG.PINMODE[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & wMask | (IO_MOD_NONE & 0x03ul);\
        } while(0)

//! \brief function selection
#define IOCTRL_FUNCTION_SELECT(__PIN, __FUNC)   do {                            \
            uint32_t wRegIndex, wBitIndex, wMask;                               \
            wRegIndex = __PIN >> 4;                                             \
            wBitIndex = __PIN & (16 - 1);                                       \
            wMask = 3ul << wBitIndex;                                           \
            PINCON_REG.PINSEL[wRegIndex] = PINCON_REG.PINSEL[wRegIndex] & wMask | __FUNC;   \
        } while(0)

#define IO_CFG(...)                                                         \
            do {                                                            \
                io_cfg_t tCFG[] = {__VA_ARGS__};                            \
                IO.Config(tCFG, UBOUND(tCFG));                              \
            } while(0)

/*============================ TYPES =========================================*/
//! \name PIN mask
//! @{
typedef enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PORT_PIN_MSK, 0)
} em_io_pin_msk_t;
//! @}

//! \name Port name 
//! @{
typedef enum {
#if defined(PIO_PORTA)
    PA,
#endif
#if defined(PIO_PORTB)
    PB,
#endif
#if defined(PIO_PORTC)
    PC,
#endif
#if defined(PIO_PORTD)
    PD,
#endif
#if defined(PIO_PORTE)
    PE,
#endif
} em_io_port_t;
//! @}

//! \name io configuration structure
//! @{
typedef struct {
    em_io_pin_t         tPIN;                   //!< pin number
    em_io_func_sel_t    tFunction;              //!< io Funcitons
    io_mode_sel_t       tMode;                  //!< io mode
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
    bool            (*Config)(io_cfg_t *ptCFG, uint32_t chSize);
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
/* EOF */
