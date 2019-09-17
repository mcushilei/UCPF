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



/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./socket_api.h"
#pragma comment(lib, "Ws2_32.lib")

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

static int tcp_write(socket_t *pSocket, const uint8_t *pBuffer, uint32_t *bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oSend = {0};
    DWORD           bytesTrans = 0;
    int             socketErrorCode;
    DWORD           wRes;
    int             ret = SOCKET_ERR_NONE;

    socketBuffer.buf = pBuffer;
    socketBuffer.len = *bufferSize;
    *bufferSize = 0u;

    oSend.hEvent = pSocket->WriteEvent;

    if (0 != WSASend(
        pSocket->so,
        &socketBuffer,
        1,
        NULL,
        0,
        &oSend,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            printf("\r\nWSASend failed. Error: %d", socketErrorCode);
            return SOCKET_ERR_FAIL;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oSend.hEvent, FALSE, WSA_INFINITE, FALSE);
    if (wRes == WSA_WAIT_EVENT_0) {
        DWORD dwFlags;
        if (!WSAGetOverlappedResult(pSocket->so, &oSend, &bytesTrans, FALSE, &dwFlags)) {
            ret = SOCKET_ERR_FAIL;
        } else {
            *bufferSize = bytesTrans;
        }
    } else if (WSA_WAIT_TIMEOUT == wRes) {
        ret = SOCKET_ERR_TIMEOUT;
    } else {
        ret = SOCKET_ERR_FAIL;
    }

    return ret;
}

static int tcp_read(socket_t *pSocket, uint8_t *pBuffer, uint32_t *bufferSize, uint32_t timeout)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oRecv = {0};
    DWORD           bytesTrans = 0;
    DWORD           dwFlags = 0;
    int             socketErrorCode;
    DWORD           wRes;
    int             ret = SOCKET_ERR_NONE;

    socketBuffer.buf = pBuffer;
    socketBuffer.len = *bufferSize;
    *bufferSize = 0u;

    oRecv.hEvent = pSocket->ReadEvent;

    if (0 != WSARecv(
        pSocket->so,
        &socketBuffer,
        1,
        NULL,
        &dwFlags,
        &oRecv,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            return SOCKET_ERR_FAIL;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oRecv.hEvent, FALSE, timeout, FALSE);//WSA_INFINITE timeout
    if (wRes == WSA_WAIT_EVENT_0) {
        if (!WSAGetOverlappedResult(pSocket->so, &oRecv, &bytesTrans, FALSE, &dwFlags)) {
            ret = SOCKET_ERR_FAIL;
        } else {
            *bufferSize = bytesTrans;
        }
    } else if (WSA_WAIT_TIMEOUT == wRes) {
        ret = SOCKET_ERR_TIMEOUT;
    } else {
        ret = SOCKET_ERR_FAIL;
    }

    return ret;
}

static int udp_write(socket_t *pSocket, struct sockaddr *pAddr, uint32_t addrSize, uint8_t *pBuffer, uint32_t *bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oSend = {0};
    DWORD           bytesTrans = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oSend.hEvent = pSocket->WriteEvent;

    socketBuffer.buf = pBuffer;
    socketBuffer.len = *bufferSize;
    *bufferSize = 0;
    if (0 != WSASendTo(
        pSocket->so,
        &socketBuffer,
        1,
        NULL,
        0,
        pAddr,
        addrSize,
        &oSend,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            printf("\r\nWSASendTo failed. Error: %d", socketErrorCode);
            return SOCKET_ERR_FAIL;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oSend.hEvent, FALSE, WSA_INFINITE, FALSE);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        DWORD dwFlags;
        WSAResetEvent(oSend.hEvent);
        if (!WSAGetOverlappedResult(pSocket->so, &oSend, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }
    *bufferSize = bytesTrans;

    return SOCKET_ERR_NONE;
}

static int udp_read(socket_t *pSocket, struct sockaddr *pAddr, uint32_t *pAddrSize, uint8_t *pBuffer, uint32_t *bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oRecv = {0};
    DWORD           bytesTrans = 0;
    DWORD           dwFlags = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oRecv.hEvent = pSocket->ReadEvent;

    socketBuffer.buf = pBuffer;
    socketBuffer.len = *bufferSize;
    *bufferSize = 0;
    if (0 != WSARecvFrom(
        pSocket->so,
        &socketBuffer,
        1,
        NULL,
        &dwFlags,
        pAddr,
        pAddrSize,
        &oRecv,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            printf("\r\nWSARecvFrom failed. Error: %d", socketErrorCode);
            return SOCKET_ERR_FAIL;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oRecv.hEvent, FALSE, WSA_INFINITE, FALSE);
    WSAResetEvent(oRecv.hEvent);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        if (!WSAGetOverlappedResult(pSocket->so, &oRecv, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }
    *bufferSize = bytesTrans;

    return SOCKET_ERR_NONE;
}

bool socket_api_init(void)
{
    int     retResault;
    WSADATA wsaData;        //! use to save Windows Socket information.

    // Initialize Winsock
    ZeroMemory(&wsaData, sizeof(wsaData));
    retResault = WSAStartup((2u << 8) | (2u << 0),      //!< version 2.0
                            &wsaData);
    if (retResault != 0) {
        return false;
    }

    return true;
}

bool socket_api_deinit(void)
{
    //! Cleanup Winsocket.
    WSACleanup();

    return true;
}

socket_t *socket_api_create(void)
{
    socket_t *mySocket = malloc(sizeof(socket_t));
    if (NULL == mySocket) {
        return NULL;
    }
    mySocket->so         = INVALID_SOCKET;
    mySocket->ReadEvent  = WSACreateEvent();
    mySocket->WriteEvent = WSACreateEvent();

    if (mySocket->ReadEvent == WSA_INVALID_EVENT || mySocket->WriteEvent == WSA_INVALID_EVENT) {
        if (mySocket->ReadEvent != WSA_INVALID_EVENT) {
            WSACloseEvent(mySocket->ReadEvent);
        }
        if (mySocket->WriteEvent != WSA_INVALID_EVENT) {
            WSACloseEvent(mySocket->WriteEvent);
        }
        return NULL;
    }

    return mySocket;
}

/*
 *  \brief  connect to specified destination port.
 *  \return <0       cannot bind a socket.
 *  \return >=0      The socket ID to be used.
 */
int socket_api_connect(socket_t *pSocket, const char *host, const char *port)
{
    int     socketErrorCode;
    int     returnValue;
    struct  addrinfo *pAddrInfoList = NULL,
                    *pAddrInfo = NULL,
                    addrInfo = {0};
    SOCKET  so = INVALID_SOCKET;

    // Resolve the server address and port
    addrInfo.ai_family   = AF_INET;
    addrInfo.ai_socktype = SOCK_STREAM;
    addrInfo.ai_protocol = IPPROTO_TCP;
    returnValue = getaddrinfo(host, port, &addrInfo, &pAddrInfoList);
    if (returnValue != 0) {
        socketErrorCode = WSAGetLastError();
        DBG_LOG("getaddrinfo failed. Error: %u", socketErrorCode);
        return SOCKET_ERR_FAIL;
    }

    // Connect to server.
    for (pAddrInfo = pAddrInfoList; pAddrInfo != NULL; pAddrInfo = pAddrInfo->ai_next) {
        //! Create a SOCKET for connecting to server
        //! Overlapped sockets can utilize WSASend, WSASendTo, WSARecv, WSARecvFrom,
        //! and WSAIoctl for overlapped I/O operations.
        so = WSASocket(
            pAddrInfo->ai_family,
            pAddrInfo->ai_socktype,
            pAddrInfo->ai_protocol,
            NULL,
            0,
            WSA_FLAG_OVERLAPPED);
        if (so == INVALID_SOCKET) {
            socketErrorCode = WSAGetLastError();
            DBG_LOG("WSASocket failed. Error: %u\n", socketErrorCode);
            break;
        }


        //! connect to server by using a random port.
        returnValue = WSAConnect(
            so,
            pAddrInfo->ai_addr,
            pAddrInfo->ai_addrlen,
            NULL,
            NULL,
            NULL,
            NULL);
        if (returnValue != 0) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAConnect failed. Error: %u\n", socketErrorCode);

            closesocket(so);
            so = INVALID_SOCKET;
        } else {
            break;
        }
    }
    freeaddrinfo(pAddrInfoList);

    if (so == INVALID_SOCKET) {
        printf("\r\nUnable to connect to server!");
        return SOCKET_ERR_FAIL;
    }
    pSocket->so = so;

    DBG_LOG("TCP connected.");

    return SOCKET_ERR_NONE;
}

int socket_api_shutdown(socket_t *pSocket)
{
    shutdown(pSocket->so, SD_BOTH);
    return SOCKET_ERR_NONE;
}

/*
 *  \brief  send data on a socket.
 *  \return AT_OK
 *  \return AT_FAILED
 */
int socket_api_send(socket_t *pSocket, const uint8_t *buf, uint32_t *len)
{
    return tcp_write(pSocket, buf, len);
}

int socket_api_recv(socket_t *pSocket, uint8_t *buf, uint32_t *len, uint32_t timeout)
{
    return tcp_read(pSocket , buf, len, timeout);
}

int socket_api_delete(socket_t *pSocket)
{
    closesocket(pSocket->so);
    pSocket->so = INVALID_SOCKET;

    if (pSocket->ReadEvent != WSA_INVALID_EVENT) {
        WSACloseEvent(pSocket->ReadEvent);
    }
    if (pSocket->WriteEvent != WSA_INVALID_EVENT) {
        WSACloseEvent(pSocket->WriteEvent);
    }

    free(pSocket);

    return SOCKET_ERR_NONE;
}

//! usage example
#if 0
DWORD WINAPI udp_task(void *pArg)
{
    int     socketErrorCode;
    int     returnValue;
    struct  addrinfo *pAddrInfoList = NULL,
        *pAddrInfo = NULL,
        addrInfo = {0};
    SOCKET  mySocket = INVALID_SOCKET;
    DWORD   bytesTrans;

    addrInfo.ai_family   = AF_INET;
    addrInfo.ai_socktype = SOCK_DGRAM;
    addrInfo.ai_protocol = IPPROTO_UDP;
    addrInfo.ai_flags    = AI_PASSIVE;

    //! Create a SOCKET for connecting to server
    //! Overlapped sockets can utilize WSASend, WSASendTo, WSARecv, WSARecvFrom,
    //! and WSAIoctl for overlapped I/O operations.
    mySocket = WSASocket(
        addrInfo.ai_family,
        addrInfo.ai_socktype,
        addrInfo.ai_protocol,
        NULL,
        0,
        WSA_FLAG_OVERLAPPED);
    if (mySocket == INVALID_SOCKET) {
        socketErrorCode = WSAGetLastError();
        printf("\r\nWSASocket failed. Error: %u\n", socketErrorCode);
        return 1;
    }

    if (1) {
        // Resolve the local address and port
        socketErrorCode = getaddrinfo(NULL, "20700", &addrInfo, &pAddrInfoList);
        if (socketErrorCode != 0) {
            printf("\r\ngetaddrinfo failed. Error: %d", socketErrorCode);
            goto __error_exit;
        }
        socketErrorCode = bind(mySocket, pAddrInfoList->ai_addr, pAddrInfoList->ai_addrlen);
        freeaddrinfo(pAddrInfoList);
        if (socketErrorCode == SOCKET_ERROR) {
            printf("\r\nbind failed. Error: %d", WSAGetLastError());
            goto __error_exit;
        }
    } else {
        // nothing to do.
    }

    // Resolve the destination address and port
    socketErrorCode = getaddrinfo(TEST_SERVER_IP, TEST_SERVER_PORT, &addrInfo, &pAddrInfoList);
    if (socketErrorCode != 0) {
        printf("\r\ngetaddrinfo failed. Error: %d", socketErrorCode);
        goto __error_exit;
    }

    // Send something.
    uint8_t sendbuf[50] = "\r\nHello server!";
    udp_write(mySocket, pAddrInfoList->ai_addr, pAddrInfoList->ai_addrlen, sendbuf, strlen(sendbuf));

    udp_read(mySocket, NULL, NULL, sendbuf, sizeof(sendbuf));
    printf("recv: %s", sendbuf);

    freeaddrinfo(pAddrInfoList);


__error_exit:
    closesocket(mySocket);

    return 0;
}

DWORD WINAPI tcp_client_task(void *pArg)
{
    int     socketErrorCode;
    int     returnValue;
    struct addrinfo *pAddrInfoList = NULL,
        *pAddrInfo = NULL,
        addrInfo = {0};
    SOCKET  mySocket = INVALID_SOCKET;
    DWORD   bytesTrans;

    // Resolve the server address and port
    addrInfo.ai_family   = AF_INET;
    addrInfo.ai_socktype = SOCK_STREAM;
    addrInfo.ai_protocol = IPPROTO_TCP;
    returnValue = getaddrinfo(TEST_SERVER_IP, TEST_SERVER_PORT, &addrInfo, &pAddrInfoList);
    if (returnValue != 0) {
        socketErrorCode = WSAGetLastError();
        printf("\r\ngetaddrinfo failed. Error: %u", socketErrorCode);
        return 1;
    }

    // Connect to server.
    for (pAddrInfo = pAddrInfoList; pAddrInfo != NULL; pAddrInfo = pAddrInfo->ai_next) {
        //! Create a SOCKET for connecting to server
        //! Overlapped sockets can utilize WSASend, WSASendTo, WSARecv, WSARecvFrom,
        //! and WSAIoctl for overlapped I/O operations.
        mySocket = WSASocket(
            pAddrInfo->ai_family,
            pAddrInfo->ai_socktype,
            pAddrInfo->ai_protocol,
            NULL,
            0,
            WSA_FLAG_OVERLAPPED);
        if (mySocket == INVALID_SOCKET) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSASocket failed. Error: %u\n", socketErrorCode);
            freeaddrinfo(pAddrInfoList);
            return 1;
        }


        //! connect to server by using a random port.
        returnValue = WSAConnect(
            mySocket,
            pAddrInfo->ai_addr,
            pAddrInfo->ai_addrlen,
            NULL,
            NULL,
            NULL,
            NULL);
        if (returnValue != 0) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAConnect failed. Error: %u\n", socketErrorCode);

            closesocket(mySocket);
            mySocket = INVALID_SOCKET;
        } else {
            break;
        }
    }
    freeaddrinfo(pAddrInfoList);
    if (mySocket == INVALID_SOCKET) {
        printf("\r\nUnable to connect to server!");
        return 1;
    }

    uint8_t sendbuf[] = "\r\nHello server!";

    // Send something.
    tcp_write(mySocket, sendbuf, strlen(sendbuf));

    //! shutdown the connection for sending since no more data will be sent
    //! the client can still use the mySocket for receiving data
    shutdown(mySocket, SD_SEND);

    //! Receive data until the server closes the connection
    uint8_t recvbuf[128];
    do {
        bytesTrans = tcp_read(mySocket, recvbuf, 128);
        printf("\r\n Client rcv. %u", bytesTrans);
    } while (bytesTrans);

    closesocket(mySocket);

    return 0;
}

DWORD WINAPI tcp_server_worker_task(void *pArg)
{
    SOCKET mySocket = (SOCKET)pArg;
    DWORD  bytesTrans;

    uint8_t sendbuf[] = "\r\nHello client!";

    //! send something.
    tcp_write(mySocket, sendbuf, strlen(sendbuf));

    //! wait for client replying.
    uint8_t recvbuf[128];
    do {
        bytesTrans = tcp_read(mySocket, recvbuf, 128);
        printf("\r\n Server rcv. %u", bytesTrans);
    } while (bytesTrans);
    printf("\r\nConnection disconnect by client.");

    // shutdown the connection since we're done
    shutdown(mySocket, SD_SEND);
    printf("\r\nConnection closed.");

    closesocket(mySocket);

    return 0;
}

DWORD WINAPI tcp_server_task(void *pArg)
{
    int socketErrorCode;
    struct addrinfo *pAddrInfoList = NULL, addrInfo = {0};
    SOCKET socketListen = INVALID_SOCKET;
    DWORD   errorCode;

    addrInfo.ai_family   = AF_INET;
    addrInfo.ai_socktype = SOCK_STREAM;
    addrInfo.ai_protocol = IPPROTO_TCP;
    addrInfo.ai_flags    = AI_PASSIVE;

    // Resolve the server address and port
    socketErrorCode = getaddrinfo(NULL, TEST_SERVER_PORT, &addrInfo, &pAddrInfoList);
    if (socketErrorCode != 0) {
        printf("\r\ngetaddrinfo failed. Error: %d", socketErrorCode);
        return 0;
    }

    // Create a SOCKET.
    socketListen = WSASocket(
        pAddrInfoList->ai_family,
        pAddrInfoList->ai_socktype,
        pAddrInfoList->ai_protocol,
        NULL,
        0,
        WSA_FLAG_OVERLAPPED);
    if (socketListen == INVALID_SOCKET) {
        printf("\r\nsocket failed. Error: %ld", WSAGetLastError());
        freeaddrinfo(pAddrInfoList);
        return 0;
    }

    // Setup the TCP listening socket
    socketErrorCode = bind(socketListen, pAddrInfoList->ai_addr, pAddrInfoList->ai_addrlen);
    freeaddrinfo(pAddrInfoList);
    if (socketErrorCode == SOCKET_ERROR) {
        printf("\r\nbind failed. Error: %d", WSAGetLastError());
        goto __CLOSE_LISTEN_SOCKET;
    }


    // listening on this socket.
    socketErrorCode = listen(socketListen, 100);
    if (socketErrorCode == SOCKET_ERROR) {
        printf("\r\nlisten failed. Error: %d", WSAGetLastError());
        goto __CLOSE_LISTEN_SOCKET;
    }

    printf("\r\nTCP Server start.");
    do {
        // Accept a client and get a new socket.
        SOCKET socketAccept = INVALID_SOCKET;
        socketAccept = accept(socketListen, NULL, NULL);
        if (socketAccept == INVALID_SOCKET) {
            socketErrorCode = WSAGetLastError();
            printf("\r\naccept failed. Error: %d", socketErrorCode);
        } else {
            printf("\r\nClient connect.");
            HANDLE handleThread = NULL;
            handleThread = CreateThread(
                NULL,
                0,
                &tcp_server_worker_task,
                (void *)socketAccept,
                0,
                NULL);
            if (handleThread == NULL) {
                errorCode = GetLastError();
                printf("\r\nCreateThread failed. Error: %u", errorCode);
                shutdown(socketAccept, SD_SEND);
                closesocket(socketAccept);
            } else {
                CloseHandle(handleThread);
            }
        }
    } while (1);

__CLOSE_LISTEN_SOCKET:
    shutdown(socketListen, SD_BOTH);
    closesocket(socketListen);

    return 0;
}


int main(void)
{
    HANDLE  clientTaskHandle;
    HANDLE  serverTaskHandle;
    DWORD   errorCode;

    // Initialize Winsock
    if (!socket_init()) {
        printf("\r\nWSAStartup failed!");
        return 0;
    }


    clientTaskHandle = CreateThread(
        NULL,
        0,
        &tcp_client_task,
        NULL,
        0,
        NULL);
    if (clientTaskHandle == NULL) {
        errorCode = GetLastError();
        printf("\r\nCreate thread failed! Error: %u", errorCode);
        return errorCode;
    }
    serverTaskHandle = CreateThread(
        NULL,
        0,
        &tcp_server_task,
        NULL,
        0,
        NULL);
    if (serverTaskHandle == NULL) {
        errorCode = GetLastError();
        printf("\r\nCreate thread failed! Error: %u", errorCode);
        return errorCode;
    }


    //! wait thread to run complete.
    WaitForSingleObject(clientTaskHandle, INFINITE);
    CloseHandle(clientTaskHandle);
    WaitForSingleObject(serverTaskHandle, INFINITE);
    CloseHandle(serverTaskHandle);

    printf("\r\n");

    //! Cleanup Winsocket.
    socket_deinit();

    return 0;
}

#endif

/* EOF */
