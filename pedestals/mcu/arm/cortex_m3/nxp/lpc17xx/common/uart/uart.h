/*******************************************************************************
 *  Copyright(C)2016-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_uart.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
#define UART0       UART[0]
#define UART1       UART[1]
#define UART2       UART[2]
#define UART3       UART[3]

#define UART_CFG(__UART, ...)                                               \
        do {                                                                \
            uart_cfg_t tCFG = {__VA_ARGS__};                                \
            __UART.Config(&tCFG);                                           \
        } while(0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart working mode 
//! @{
enum {
    DRIVER_UART_NO_AUTO_BAUD      = 0x00,
    DRIVER_UART_AUTO_BAUD_MODE0   = 0x01,
    DRIVER_UART_AUTO_BAUD_MODE1   = 0x03,
    DRIVER_UART_AUTO_RESTART      = 0x04,
    DRIVER_UART_NO_AUTO_RESTART   = 0x00,

    DRIVER_UART_NO_PARITY         = 0x0000,
    DRIVER_UART_EVEN_PARITY       = 0x0018,
    DRIVER_UART_ODD_PARITY        = 0x0008,
    DRIVER_UART_FORCE_1_PARITY    = 0x0028,
    DRIVER_UART_FORCE_0_PARITY    = 0x0038,

    DRIVER_UART_1_STOPBITS          = 0x0000,
    DRIVER_UART_2_STOPBITS          = 0x0040,

    DRIVER_UART_ENABLE_FIFO         = 0x0000,
    DRIVER_UART_DISABLE_FIFO        = 0x0080,

    DRIVER_UART_5_BITS_LENGTH       = 0x0000,
    DRIVER_UART_6_BITS_LENGTH       = 0x0100,
    DRIVER_UART_7_BITS_LENGTH       = 0x0200,
    DRIVER_UART_8_BITS_LENGTH       = 0x0300,
};
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint32_t        Baudrate;
    uint16_t        DataBits;
    uint16_t        StopBits;
    uint16_t        Parity;
    uint16_t        FlowCtrl;
} uart_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name  uart interface
//! @{
DEF_INTERFACE(i_uart_t)
    bool            (*Config)(const uart_cfg_t *ptCFG);
    bool            (*Idle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;
END_DEF_INTERFACE(i_uart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_uart_t UART[UART_COUNT];

/*============================ PROTOTYPES ====================================*/

#endif
