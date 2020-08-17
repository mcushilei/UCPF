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




/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "../deadline/deadline.h"
#include "./HTTPClient.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
THIS_FILE_NAME("HTTPClient");

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static bool buffer_ensure(const http_buffer_t *buf, uint32_t needed)
{
    return needed < (buf->Size - buf->Offset);
}

static uint32_t buffer_left(const http_buffer_t *buf)
{
    return buf->Size - buf->Offset;
}

static bool buffer_write_byte(http_buffer_t *buf, char byte)
{
    if (1 >= (buf->Size - buf->Offset)) {
        return false;
    }
    buf->Buffer[buf->Offset] = byte;
    buf->Offset++;
    return true;
}

static bool http_serialize_message(http_buffer_t *buf, http_message_t *message)
{
    uint32_t length = 0;
    
    buf->Offset = 0;
    memset(buf->Buffer, 0, buf->Size);
    
    //! write request.
    if (!buffer_ensure(buf, strlen(message->RequestLine))) {
        return false;
    }
    length = snprintf(&buf->Buffer[buf->Offset], buffer_left(buf), "%s\r\n", message->RequestLine);
    buf->Offset += length;
    
    //! write headers
    for (uint32_t i = 0; i < message->HeaderCount; i++) {
        if (!buffer_ensure(buf, strlen(message->Header[i]))) {
            return false;
        }
        length = snprintf(&buf->Buffer[buf->Offset], buffer_left(buf), "%s\r\n", message->Header[i]);
        buf->Offset += length;
    }
    
    //! write end of heeader
    if (!buffer_ensure(buf, 2)) {
        return false;
    }
    memcpy(&buf->Buffer[buf->Offset], "\r\n", 2);
    buf->Offset += 2;
    
    //! write body
    if (message->BodyLength == 0 || message->Body == NULL) {
        return true;
    }
    
    if (!buffer_ensure(buf, message->BodyLength)) {
        return false;
    }
    memcpy(&buf->Buffer[buf->Offset], message->Body, message->BodyLength);
    buf->Offset += message->BodyLength;
    
    return true;
}

static char *wait_for_header(http_client_t *client)
{
    char buf;
    int len = 0;
    char *start = &client->Buffer.Buffer[client->Buffer.Offset];
    
    do {
        len = client->Network->Read(client->Network, &buf, 1, deadline_left_ms(&client->Deadline));
        if (len <= 0) {
            return NULL;
        }
        if (buf == '\r') {
            buf = '\0';
            if (!buffer_write_byte(&client->Buffer, buf)) {
                return NULL;
            }
            break;
        } else {
            if (!buffer_write_byte(&client->Buffer, buf)) {
                return NULL;
            }
        }
    } while (1);
    
    len = client->Network->Read(client->Network, &buf, 1, deadline_left_ms(&client->Deadline));
    if (len <= 0) {
        return NULL;
    }
    if (buf == '\n') {
        buf = '\0';
        if (!buffer_write_byte(&client->Buffer, buf)) {
            return NULL;
        }
    } else {
        return NULL;
    }
    
    return start;
}

bool http_client_transive(http_client_t *client, http_message_t *resquest, http_message_t *response)
{
    enum {
        STATUS_WAIT_CR_1 = 0,
        STATUS_WAIT_LF_1,
        STATUS_WAIT_CR_2,
        STATUS_WAIT_LF_2,
        STATUS_WAIT_BODY_END,
    };
    char *str = NULL;
    uint32_t i = 0;
    uint32_t contentLength = 0;
    int32_t len = 0;
    bool rc = false;
    
    if (!http_serialize_message(&client->Buffer, resquest)) {
        return false;
    }
    DBG_LOG("http send:\r\n%s", client->Buffer.Buffer);
    
    deadline_init(&client->Deadline);
    deadline_set_ms(&client->Deadline, client->TimeoutMS);
    
    //! connect network
    if (0 != client->Network->Connect(client->Network)) {
        return false;
    }
    
    //! send message
    if (client->Buffer.Offset
    != client->Network->Write(client->Network, client->Buffer.Buffer, client->Buffer.Offset, deadline_left_ms(&client->Deadline))) {
        goto err_exit;
    }
    
    
    client->Buffer.Offset = 0;
    memset(client->Buffer.Buffer, 0, client->Buffer.Size);
    memset(response, 0, sizeof(http_message_t));
    
    //! receive headers
    str = wait_for_header(client);
    if (str == NULL) {
        goto err_exit;
    }
    if (0 != strncmp("HTTP", str, 4)) {
        DBG_LOG("invalid resp");
        goto err_exit;
    }
    response->StatusLine = str;
    
    while (response->HeaderCount < UBOUND(response->Header)) {
        str = wait_for_header(client);
        if (str == NULL) {
            goto err_exit;
        }
        
        if (0 == strlen(str)) {
            break;
        }
        
        response->Header[response->HeaderCount] = str;
        response->HeaderCount++;
    }
    if (response->HeaderCount >= UBOUND(response->Header)) {
        DBG_LOG("http header too long");
        goto err_exit;
    }
    
    //! get content-length
    for (i = 0; i < response->HeaderCount; i++) {
        str = strstr(response->Header[i], "Content-Length:");
        if (str != NULL) {
            break;
        }
    }
    if (i >= response->HeaderCount) {
        goto err_exit;
    }
    if (1 != sscanf(response->Header[i], "Content-Length:%u", &contentLength)) {
        goto err_exit;
    }

    //! receive body
    if (!buffer_ensure(&client->Buffer, contentLength)) {
        goto err_exit;
    }

    response->Body = &client->Buffer.Buffer[client->Buffer.Offset];
    len = client->Network->Read(client->Network, &client->Buffer.Buffer[client->Buffer.Offset], contentLength, deadline_left_ms(&client->Deadline));
    if (len != contentLength) {
        goto err_exit;
    }
    client->Buffer.Offset += len;
    response->BodyLength = len;
    
    rc = true;
err_exit:
    //! disconnect network
    client->Network->Disconnect(client->Network);
    
    return rc;
}

bool http_client_init(http_client_t *client, http_network_t *network, char *buffer, uint32_t bufferSize, uint32_t timeoutMS)
{
    client->Network = network;
    client->TimeoutMS = timeoutMS;
    client->Buffer.Buffer = buffer;
    client->Buffer.Size = bufferSize;
    client->Buffer.Offset = 0;
    return true;
}


/* EOF */
