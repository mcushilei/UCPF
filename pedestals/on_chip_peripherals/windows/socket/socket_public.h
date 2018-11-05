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
#ifndef __WINDOWS_SOCKET_PUBLIC_H__
#define __WINDOWS_SOCKET_PUBLIC_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool socket_init(void);
extern bool socket_deinit(void);
extern uint32_t tcp_write(SOCKET so, uint8_t *pBuffer, uint32_t bufferSize);
extern uint32_t tcp_read(SOCKET so, uint8_t *pBuffer, uint32_t bufferSize);
extern uint32_t udp_write(SOCKET so, struct sockaddr *pAddr, uint32_t addrSize, uint8_t *pBuffer, uint32_t bufferSize);
extern uint32_t udp_read(SOCKET so, struct sockaddr *pAddr, uint32_t *pAddrSize, uint8_t *pBuffer, uint32_t bufferSize);

#endif  //!< #ifndef __WINDOWS_SOCKET_PUBLIC_H__
/* EOF */
