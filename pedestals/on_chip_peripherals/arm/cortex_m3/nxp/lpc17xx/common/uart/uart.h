/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __DRIVER_UART_C__
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

#define UART_EXTERN_FUN(_N, _A)                                            \
    extern bool uart##_N##_init(uart_cfg_t *ptUsartCFG);                    \
    extern bool uart##_N##_idle(void);                                       \
    extern bool uart##_N##_enable(void);                                     \
    extern bool uart##_N##_disable(void);                                    \
    extern bool uart##_N##_write_byte(uint8_t chByte);                       \
    extern bool uart##_N##_read_byte(uint8_t *pchByte);                      \
    extern bool uart##_N##_set(uint32_t wBaudrate);                        \
    extern uint32_t uart##_N##_get(void);                                  

#define UART_CFG(__UART, ...)                                             \
        do {                                                                \
            uart_cfg_t tCFG = {__VA_ARGS__};                               \
            __UART.Config(&tCFG);                                            \
        } while(0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart working mode 
//! @{
typedef enum {
    UART_NO_AUTO_BAUD      = 0x00,
    UART_AUTO_BAUD_MODE0   = 0x01,
    UART_AUTO_BAUD_MODE1   = 0x03,
    UART_AUTO_RESTART      = 0x04,
    UART_NO_AUTO_RESTART   = 0x00,

    UART_NO_PARITY         = 0x00,
    UART_EVEN_PARITY       = 0x18,
    UART_ODD_PARITY        = 0x08,
    UART_FORCE_1_PARITY    = 0x28,
    UART_FORCE_0_PARITY    = 0x38,

    UART_1_STOPBIT         = 0x00,
    UART_2_STOPBIT         = 0x40,

    UART_ENABLE_FIFO       = 0x00,
    UART_DISABLE_FIFO      = 0x80,

    UART_5_BIT_LENGTH      = 0x0000,
    UART_6_BIT_LENGTH      = 0x0100,
    UART_7_BIT_LENGTH      = 0x0200,
    UART_8_BIT_LENGTH      = 0x0300,
}em_uart_mode_t;
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint16_t        hwMode;
    uint32_t        wBaudrate;
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
    bool            (*Config)(uart_cfg_t *ptCFG);
    bool            (*Idle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;
END_DEF_INTERFACE(i_uart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_uart_t UART[UART_COUNT];  //!< this is used by upper layer

/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, UART_EXTERN_FUN, 0)   //!< this is used by bottom layer

#endif
#endif
