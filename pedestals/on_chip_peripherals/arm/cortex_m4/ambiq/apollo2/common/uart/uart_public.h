/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_UART_PUBLIC_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_UART_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_uart.h"

/*============================ MACROS ========================================*/
#define DRIVER_UART_5_BITS_LENGTH       0u
#define DRIVER_UART_6_BITS_LENGTH       REG_UART_LCRH_WLEN(1)
#define DRIVER_UART_7_BITS_LENGTH       REG_UART_LCRH_WLEN(2)
#define DRIVER_UART_8_BITS_LENGTH       REG_UART_LCRH_WLEN(3)

#define DRIVER_UART_NO_PARITY           0u
#define DRIVER_UART_EVEN_PARITY         REG_UART_LCRH_PEN_M | REG_UART_LCRH_EPS_M
#define DRIVER_UART_ODD_PARITY          REG_UART_LCRH_PEN_M

#define DRIVER_UART_1_STOPBITS          0u
#define DRIVER_UART_2_STOPBITS          REG_UART_LCRH_STP2_M

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name uart configuration
//! @{
typedef struct {
    uint32_t        Baudrate;
    uint8_t         DataBits;
    uint8_t         StopBits;
    uint8_t         Parity;
    uint8_t         FlowCtrl;
} uart_cfg_t;
//! @}

//! \name class: i_uart_t
//! @{
DEF_INTERFACE(i_uart_t)
    bool        (*Config)(const uart_cfg_t *pCFG);
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    bool        (*Open)(void);
    bool        (*Close)(void);
    bool        (*WriteByte)(uint8_t byte);
    bool        (*ReadByte)(uint8_t *pByte);
END_DEF_INTERFACE(i_uart_t)
//! @}

/*============================ PUBLIC VARIABLES ==============================*/
extern const i_uart_t UART[UART_COUNT];

/*============================ PUBLIC PROTOTYPES =============================*/

#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_UART_PUBLIC_H__
/* EOF */
