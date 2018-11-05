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


#ifndef __DRIVER_UART_C__
#ifndef __DRIVER_UART_H__
#define __DRIVER_UART_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_uart.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
#define __UART_FUNCTION_PROTOTYPES(_N, _A)                                 \
    extern bool usart##_N_config(usart_cfg_t *ptUsartCFG);                    \
    extern bool usart##_N_idle(void);                                       \
    extern bool usart##_N_enable(void);                                     \
    extern bool usart##_N_disable(void);                                    \
    extern bool usart##_N_write_byte(uint8_t chByte);                       \
    extern bool usart##_N_read_byte(uint8_t *pchByte);                      \
    extern bool     usart##_N##_baudrate_set(uint32_t wBaudrate);           \
    extern uint32_t usart##_N##_baudrate_get(void);                         \

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name usart working mode 
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
} em_usart_mode_t;
//! @}

//! \name usart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} usart_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}


//! \name class: i_uart_t
//! @{
DEF_INTERFACE(i_uart_t)
    bool        (*Config)(usart_cfg_t *ptCFG);
    bool        (*Idle)(void);
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    bool        (*WriteByte)(uint8_t chByte);
    bool        (*ReadByte)(uint8_t *pchByte);
    
    i_baudrate_t Baudrate;

END_DEF_INTERFACE(i_uart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_uart_t UART[UART_COUNT];

/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, __UART_FUNCTION_PROTOTYPES, 0)

#endif
#endif
