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
    extern bool uart##__N##_int_enable(uint32_t wMask);                     \
    extern bool uart##__N##_int_disable(uint32_t wMask);                    \
    extern bool uart##__N##_irda_init(uart_IrDA_cfg_t *ptUsartCFG);        \
    extern bool uart##__N##_485_init(uart_485_cfg_t *pt485CFG);            \
    extern bool uart##__N##_485_send_address(uint8_t chAddress);            \
        

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
} em_uart_mode_t;
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} uart_cfg_t;
//! @}

//! \name uart_irda_mode_t
//! @{
typedef enum {
    UART_IRDA_DISABLE           = 0x00,         //!< disable irda
    UART_IRDA_ENABLE            = 0x01,         //!< enable irda
    
    UART_IRDA_NOT_INVERTED      = 0x00,         //!< not inverted the input
    UART_IRDA_INVERTED          = 0x02,         //!< inverted the input
    
    UART_IRDA_FIXPULSE_DISABLE  = 0x00,         //!< disable fixpulse
    UART_IRDA_FIXPULSE_ENABLE   = 0x04,         //!< enable fixpulse
    
    UART_IRDA_PULSEDIV_2        = 0x00,         //!< 2* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_4        = 0x08,         //!< 4* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_8        = 0x10,         //!< 8* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_16       = 0x18,         //!< 16* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_32       = 0x20,         //!< 32* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_64       = 0x28,         //!< 64* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_128      = 0x30,         //!< 128* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_256      = 0x38          //!< 256* T_pclk(fixpulse enable)
} em_uart_irda_mode_t;
//! @}

//! \name uart_IrDA_cfg_t
//! @{
typedef struct {
    uint16_t            hwIrDAMode;            //!< IrDA config word
} uart_IrDA_cfg_t;
//! @} 

//! \name uart 485 working mode
//! @{
typedef enum {
    UART_485_DISABLE               = 0x00,
    UART_485_MULTI_STATION         = 0x01,
    UART_485_DISABLE_RX            = 0x02,
    UART_485_AUTO_ADDRESS_MATCH    = 0x04,
    UART_485_USE_DTR_AS_DIR_CTRL   = 0x08,    
    UART_485_ENABLE_AUTO_DIR_CTRL  = 0x10,
    UART_485_DIR_PIN_IDLE_LOW      = 0x20
}em_uart_485_mode_t;
//! @}

//! \name uart 485 configuration
//! @{
typedef struct {
    uint8_t chMode;
    uint8_t chAddress;
    uint8_t chDelay;
} uart_485_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name class: uart_irda_t
//! @{
DEF_INTERFACE(uart_irda_t)
    bool        (*Init)(uart_IrDA_cfg_t *ptCFG);
END_DEF_INTERFACE(uart_irda_t)
//! @}

//! \name class: uart_485_t
//! @{
DEF_INTERFACE(uart_485_t)
    bool        (*Init)(uart_485_cfg_t *ptCFG);
    bool        (*SendAddress)(uint8_t chAddress);
END_DEF_INTERFACE(uart_485_t)
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
    
#if UART_485_SUPPORT == ENABLED
    uart_485_t  RS485;
#endif

#if UART_IRDA_SUPPORT == ENABLED
    uart_irda_t IrDA;     
#endif
END_DEF_INTERFACE(i_uart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_uart_t UART[UART_COUNT];

/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, UART_EXTERN_FUN, 0)

#endif
