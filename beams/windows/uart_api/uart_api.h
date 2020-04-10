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


#ifndef __WINDOWS_COM_H__
#define __WINDOWS_COM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name usart working mode 
//! @{
typedef enum {
    UART_NO_PARITY          = 0x0000,
    UART_EVEN_PARITY        = 0x0001,
    UART_ODD_PARITY         = 0x0002,
    UART_FORCE_1_PARITY     = 0x0003,
    UART_FORCE_0_PARITY     = 0x0004,

    UART_1_STOPBIT          = 0x0010,
    UART_2_STOPBIT          = 0x0020,

    UART_5_BIT_LENGTH       = 0x0500,
    UART_6_BIT_LENGTH       = 0x0600,
    UART_7_BIT_LENGTH       = 0x0700,
    UART_8_BIT_LENGTH       = 0x0800,
} em_com_mode_t;
//! @}

//! \name usart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} com_cfg_t;
//! @}


typedef struct {
    HANDLE          hCom;
    HANDLE          hHardwareEvent;
    HANDLE          hHardwareThread;
    bool            bIsOpened;
    DWORD           flagClosePort;
    void(*pComEventHandle)(DWORD eventFlag);
} com_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern uint32_t enum_all_comm(void);
extern bool com_open(com_t *ptThis, const uint8_t *pchCom, const com_cfg_t *ptCfg, void *pComEventHandle);
extern bool com_close(com_t *ptThis);
extern uint32_t com_write(com_t *ptThis, uint8_t *pchData, uint32_t wLen, uint32_t timeoutMS);
extern uint32_t com_read(com_t *ptThis, uint8_t *pchData, uint32_t wLen, uint32_t timeoutMS);
extern bool com_flush_buffer(com_t *ptThis);

#endif
/* EOF */