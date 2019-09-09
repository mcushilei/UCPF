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


#ifndef __UART_H__
#define __UART_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_uart.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
#define UART0       UART[0]
#define UART1       UART[1]
#define UART2       UART[2]
#define UART3       UART[3]

#define UART_EXTERN_FUN(__N, __A)                                            \
    extern bool uart##__N##_init(uart_cfg_t *ptUsartCFG);                  \
    extern bool uart##__N##_idle(void);                                     \
    extern bool     uart##__N##_baudrate_set(uint32_t wBaudrate);          \
    extern uint32_t uart##__N##_baudrate_get(void);                        \
    extern bool uart##__N##_enable(void);                                   \
    extern bool uart##__N##_disable(void);                                  \
    extern bool uart##__N##_write_byte(uint8_t chByte);                     \
    extern bool uart##__N##_read_byte(uint8_t *pchByte);                    \
        

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart working mode 
//! @{
typedef enum {
    //! bit 0-3
    UART_5_BIT_LENGTH      = 0u << 0,
    UART_6_BIT_LENGTH      = 1u << 0,
    UART_7_BIT_LENGTH      = 2u << 0,
    UART_8_BIT_LENGTH      = 3u << 0,

    //! bit 4-7
    UART_NO_PARITY         = 0u << 4,
    UART_EVEN_PARITY       = 1u << 4,
    UART_ODD_PARITY        = 2u << 4,
    UART_FORCE_1_PARITY    = 3u << 4,
    UART_FORCE_0_PARITY    = 4u << 4,

    //! bit 8-9
    UART_1_STOPBIT         = 0u << 8,
    UART_2_STOPBIT         = 1u << 8,

    //! bit 10
    UART_ENABLE_FIFO       = 1u << 10,

    //! bit 11
    UART_AUTO_BAUD_ENABLE  = 1u << 11,
} em_uart_mode_t;
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} uart_cfg_t;
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
    bool        (*Init)(uart_cfg_t *ptCFG);
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
MREPEAT(UART_COUNT, UART_EXTERN_FUN, 0)

#endif
