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


#ifndef __USART_H__
#define __USART_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_usart.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
#define USART_EXTERN_FUN(_N, _A)                                            \
    extern bool usart##_N_init(usart_cfg_t *ptUsartCFG);                    \
    extern bool usart##_N_idle(void);                                       \
    extern bool usart##_N_enable(void);                                     \
    extern bool usart##_N_disable(void);                                    \
    extern bool usart##_N_write_byte(uint8_t chByte);                       \
    extern bool usart##_N_read_byte(uint8_t *pchByte);                      \
    extern bool usart##_N##_set(uint32_t wBaudrate);                        \
    extern uint32_t usart##_N##_get(void);                                  

#define USART_CFG(__USART, ...)                                             \
        do {                                                                \
            usart_cfg_t tCFG = {__VA_ARGS__};                               \
            __USART.Init(&tCFG);                                            \
        } while(0)

#define USART_INT_ENABLE(__USART, __MSK)                                    \
        do {                                                                \
            __USART.Interrupt.Enable(__MSK);                                \
        } while(0)

#define USART_INT_DISABLE(__USART, __MSK)                                   \
        do {                                                                \
            __USART.Interrupt.Disable(__MSK);                               \
        } while(0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name usart working mode 
//! @{
typedef enum {
    USART_NO_AUTO_BAUD      = 0x00,
    USART_AUTO_BAUD_MODE0   = 0x01,
    USART_AUTO_BAUD_MODE1   = 0x03,
    USART_AUTO_RESTART      = 0x04,
    USART_NO_AUTO_RESTART   = 0x00,

    USART_NO_PARITY         = 0x00,
    USART_EVEN_PARITY       = 0x18,
    USART_ODD_PARITY        = 0x08,
    USART_FORCE_1_PARITY    = 0x28,
    USART_FORCE_0_PARITY    = 0x38,

    USART_1_STOPBIT         = 0x00,
    USART_2_STOPBIT         = 0x40,

    USART_ENABLE_FIFO       = 0x00,
    USART_DISABLE_FIFO      = 0x80,

    USART_5_BIT_LENGTH      = 0x0000,
    USART_6_BIT_LENGTH      = 0x0100,
    USART_7_BIT_LENGTH      = 0x0200,
    USART_8_BIT_LENGTH      = 0x0300,
}em_usart_mode_t;
//! @}

//! \name usart configuration
//! @{
typedef struct {
    uint16_t        hwMode;
    uint32_t        wBaudrate;
} usart_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name  usart interface
//! @{
DEF_INTERFACE(i_usart_t)
    bool            (*Init)(usart_cfg_t *ptCFG);
    bool            (*Idle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;
END_DEF_INTERFACE(i_usart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_usart_t USART[USART_COUNT];  //!< this is used by upper layer

/*============================ PROTOTYPES ====================================*/
MREPEAT(USART_COUNT, USART_EXTERN_FUN, 0)   //!< this is used by bottom layer

#endif
