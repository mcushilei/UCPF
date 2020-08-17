/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "../deadline/deadline.h"

/*============================ MACROS ========================================*/
#define HTTP_MAX_HANDLERS       (10u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct http_network_t {
	void     *MySocket;
    // return the number of byte has been received.
	int     (*Read) (struct http_network_t *network, char *buffer, int length, int timeoutMS);
    // return the number of byte has been sent out.
	int     (*Write) (struct http_network_t *network, char *buffer, int length, int timeoutMS);
    // return 0 if success.
    int     (*Connect) (struct http_network_t *network);
    void    (*Disconnect) (struct http_network_t *network);
} http_network_t;

typedef struct {
    union {
        const char *StatusLine;
        const char *RequestLine;
    };
    const char *Header[HTTP_MAX_HANDLERS];
    const char *Body;
    uint32_t    BodyLength;
    uint8_t     HeaderCount;
} http_message_t;

typedef struct {
    char       *Buffer;
    uint32_t    Size;
    uint32_t    Offset;
} http_buffer_t;

typedef struct {
    http_network_t *Network;
    
    http_buffer_t   Buffer;
    
    deadline_t      Deadline;
    uint32_t        TimeoutMS;
} http_client_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool http_client_init(http_client_t *client, http_network_t *network, char *buffer, uint32_t bufferSize, uint32_t timeoutMS);
extern bool http_client_transive(http_client_t *client, http_message_t *resquest, http_message_t *response);


#endif  //!< #ifndef __HTTP_CLIENT_H__
/* EOF */
