/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#ifndef __WINDOWS_SOCKET_H__
#define __WINDOWS_SOCKET_H__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iphlpapi.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    SOCKET so;
    HANDLE WriteEvent;
    HANDLE ReadEvent;
} socket_t;

enum {
    SOCKET_ERR_NONE         = 0,
    SOCKET_ERR_TIMEOUT,
    SOCKET_ERR_FAIL,
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool socket_api_init(void);
extern bool socket_api_deinit(void);

extern socket_t *socket_api_create(void);
extern int       socket_api_delete(socket_t *pSocket);
extern int  socket_api_connect(socket_t *pSocket, const char* host, uint32_t port);
extern int  socket_api_bind(socket_t *pSocket, const char *host, uint32_t port);
extern int  socket_api_send(socket_t *pSocket, const char *buf, uint32_t *len);
extern int  socket_api_sendto(socket_t *pSocket, const char *buf, uint32_t *len, const char *ipaddr, int port);
extern int  socket_api_recv(socket_t *pSocket, char *buf, uint32_t *len, uint32_t timeout);
extern int  socket_api_recvfrom(socket_t *pSocket, char *buf, uint32_t *len, char *senderIP, int *senderPort, uint32_t timeout);
extern int  socket_api_shutdown(socket_t *pSocket);

#endif  //!< #ifndef __WINDOWS_SOCKET_H__
/* EOF */
