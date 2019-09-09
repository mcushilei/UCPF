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


#ifndef __DRIVER_TMS320_COMMON_SCI_H__
#define __DRIVER_TMS320_COMMON_SCI_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_sci.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define USART_FUNCTION_EXTERN(__N, __A)                                 \
    extern bool usart##__N##_init(usart_cfg_t *ptUsartCFG);             \
    extern bool usart##__N##_check_idle(void);                          \
    extern bool usart##__N##_enable(void);                              \
    extern bool usart##__N##_disable(void);                             \
    extern bool usart##__N##_write_byte(uint8_t chByte);                \
    extern bool usart##__N##_read_byte(uint8_t *pchByte);               \
    extern bool usart##__N##_baudrate_set(uint32_t wBaudrate);          \
    extern uint32_t usart##__N##_baudrate_get(void);                    \

#define USART_CFG(__USART, ...)                                             \
        do {                                                                \
            usart_cfg_t tCFG = {__VA_ARGS__};                               \
            __USART.Init(&tCFG);                                            \
        } while(0)

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

    USART_5_BIT_LENGTH      = 0x0400,
    USART_6_BIT_LENGTH      = 0x0500,
    USART_7_BIT_LENGTH      = 0x0600,
    USART_8_BIT_LENGTH      = 0x0700,

    USART_SYNC_MODE         = 0x0800,
} em_usart_mode_t;
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

//! \name i_usart_t
//! @{
DEF_INTERFACE(i_usart_t)
    bool            (*Init)(usart_cfg_t *ptCFG);
    bool            (*CheckIdle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;

    sci_reg_t *const RegPage;
END_DEF_INTERFACE(i_usart_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_usart_t USART[USART_COUNT];

/*============================ PROTOTYPES ====================================*/
MREPEAT(USART_COUNT, USART_FUNCTION_EXTERN, 0)

#endif
/* EOF */
