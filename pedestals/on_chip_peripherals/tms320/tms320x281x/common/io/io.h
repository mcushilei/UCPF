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


#ifndef __DRIVER_TMS320_COMMON_GIO_H__
#define __DRIVER_TMS320_COMMON_GIO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __IO_PINA_NAME(__N, __OFFSET)       PA##__N = (__OFFSET) + (__N),
#define __IO_PINB_NAME(__N, __OFFSET)       PB##__N = (__OFFSET) + (__N),
#define __IO_PINC_NAME(__N, __OFFSET)       PC##__N = (__OFFSET) + (__N),
#define __IO_PIND_NAME(__N, __OFFSET)       PD##__N = (__OFFSET) + (__N),
#define __IO_PINE_NAME(__N, __OFFSET)       PE##__N = (__OFFSET) + (__N),
#define __IO_PINF_NAME(__N, __OFFSET)       PF##__N = (__OFFSET) + (__N),
#define __IO_PING_NAME(__N, __OFFSET)       PG##__N = (__OFFSET) + (__N),

#define __IO_PINA_MSK(__N, __OFFSET)        PA##__N##_MSK = (1ul<<(__N)),
#define __IO_PINB_MSK(__N, __OFFSET)        PB##__N##_MSK = (1ul<<(__N)),
#define __IO_PINC_MSK(__N, __OFFSET)        PC##__N##_MSK = (1ul<<(__N)),
#define __IO_PIND_MSK(__N, __OFFSET)        PD##__N##_MSK = (1ul<<(__N)),
#define __IO_PINE_MSK(__N, __OFFSET)        PE##__N##_MSK = (1ul<<(__N)),
#define __IO_PINF_MSK(__N, __OFFSET)        PF##__N##_MSK = (1ul<<(__N)),
#define __IO_PING_MSK(__N, __OFFSET)        PG##__N##_MSK = (1ul<<(__N)),

#define IOCTRL_FUNCTION_SELECT(__PORT, __PIN, __FUN)    do {    \
            uint16_t __MSK = IOCTRL_MUX_MSK << (__PIN);         \
            EALLOW();                                           \
            IOCTRL_##__PORT##_REG.MUX =                         \
                (IOCTRL_##__PORT##_REG.MUX & ~(__MSK)) | ((__FUN) << (__PIN)); \
            EDIS();                                             \
        } while (0)

#define GPIO_DIRECTION_CFG(__PORT, __PIN, __DIR)        do {    \
            uint16_t __MSK = IOCTRL_DIR_MSK << (__PIN);         \
            EALLOW();                                           \
            IOCTRL_##__PORT##_REG.DIR =                         \
                (IOCTRL_##__PORT##_REG.DIR & ~(__MSK)) | ((__DIR) << (__PIN)); \
            EDIS();                                             \
        } while (0)

/*============================ TYPES =========================================*/
//! \name PORT name
//! @{
typedef enum {
#if defined(IO_PORTA)
    PORTA,
#endif
#if defined(IO_PORTB)
    PORTB,
#endif
#if defined(IO_PORTC)
    PORTC,
#endif
#if defined(IO_PORTD)
    PORTD,
#endif
#if defined(IO_PORTE)
    PORTE,
#endif
#if defined(IO_PORTF)
    PORTF,
#endif
#if defined(IO_PORTG)
    PORTG,
#endif
} em_io_port_name_t;
//! @}

//! \name PIN name
//! @{
typedef enum {
#if defined(IO_PORTA)
    MREPEAT(16, __IO_PINA_NAME, 0)
#endif
#if defined(IO_PORTB)
    MREPEAT(16, __IO_PINB_NAME, 0)
#endif
#if defined(IO_PORTC)
    MREPEAT(16, __IO_PINC_NAME, 0)
#endif
#if defined(IO_PORTD)
    MREPEAT(16, __IO_PIND_NAME, 0)
#endif
#if defined(IO_PORTE)
    MREPEAT(16, __IO_PINE_NAME, 0)
#endif
#if defined(IO_PORTF)
    MREPEAT(16, __IO_PINF_NAME, 0)
#endif
#if defined(IO_PORTG)
    MREPEAT(16, __IO_PING_NAME, 0)
#endif
} em_io_pin_name_t;
//! @}

//! \name PIN mask
//! @{
typedef enum {
#if defined(IO_PORTA)
    MREPEAT(16, __IO_PINA_MSK, 0)
#endif
#if defined(IO_PORTB)
    MREPEAT(16, __IO_PINB_MSK, 0)
#endif
#if defined(IO_PORTC)
    MREPEAT(16, __IO_PINC_MSK, 0)
#endif
#if defined(IO_PORTD)
    MREPEAT(16, __IO_PIND_MSK, 0)
#endif
#if defined(IO_PORTE)
    MREPEAT(16, __IO_PINE_MSK, 0)
#endif
#if defined(IO_PORTF)
    MREPEAT(16, __IO_PINF_MSK, 0)
#endif
#if defined(IO_PORTG)
    MREPEAT(16, __IO_PING_MSK, 0)
#endif
} em_io_pin_msk_t;
//! @}

//! \brief IO functions
//! @{
typedef enum {
    IO_WORKS_AS_GPIO        = 0,
    IO_WORKS_AS_FUNC0       = 0,
    IO_WORKS_AS_FUNC1       = 1,


} em_io_func_sel_t;
//! @}

//! \brief GPIO direction
//! @{
typedef enum {
    GPIO_DIRECTION_IN       = 0,
    GPIO_DIRECTION_OUT      = 1,
} em_io_dir_sel_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
