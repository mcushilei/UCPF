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



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\socket.h"
#pragma comment(lib, "Ws2_32.lib")

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

uint32_t tcp_write(SOCKET so, uint8_t *pBuffer, uint32_t bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oSend = {0};
    DWORD           bytesTrans = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oSend.hEvent = WSACreateEvent();
    if (oSend.hEvent == WSA_INVALID_EVENT) {
        return 0;
    }

    socketBuffer.buf = pBuffer;
    socketBuffer.len = bufferSize;
    if (0 != WSASend(
        so,
        &socketBuffer,
        1,
        NULL,
        0,
        &oSend,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            printf("\r\nWSASend failed. Error: %d", socketErrorCode);
            goto __ERROR_EXIT;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oSend.hEvent, FALSE, WSA_INFINITE, FALSE);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        DWORD dwFlags;
        WSAResetEvent(oSend.hEvent);
        if (!WSAGetOverlappedResult(so, &oSend, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }

__ERROR_EXIT:
    WSACloseEvent(oSend.hEvent);

    return bytesTrans;
}

uint32_t tcp_read(SOCKET so, uint8_t *pBuffer, uint32_t bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oRecv = {0};
    DWORD           bytesTrans = 0;
    DWORD           dwFlags = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oRecv.hEvent = WSACreateEvent();
    if (oRecv.hEvent == WSA_INVALID_EVENT) {
        return 0;
    }

    socketBuffer.buf = pBuffer;
    socketBuffer.len = bufferSize;
    if (0 != WSARecv(
        so,
        &socketBuffer,
        1,
        NULL,
        &dwFlags,
        &oRecv,
        NULL)) {
        socketErrorCode = WSAGetLastError();
        if (socketErrorCode != WSA_IO_PENDING) {
            printf("\r\nWSARecv failed. Error: %d", socketErrorCode);
            goto __ERROR_EXIT;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oRecv.hEvent, FALSE, WSA_INFINITE, FALSE);
    WSAResetEvent(oRecv.hEvent);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        if (!WSAGetOverlappedResult(so, &oRecv, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }

__ERROR_EXIT:
    WSACloseEvent(oRecv.hEvent);

    return bytesTrans;
}


uint32_t udp_write(SOCKET so, struct sockaddr *pAddr, uint32_t addrSize, uint8_t *pBuffer, uint32_t bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oSend = {0};
    DWORD           bytesTrans = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oSend.hEvent = WSACreateEvent();
    if (oSend.hEvent == WSA_INVALID_EVENT) {
        return 0;
    }

    socketBuffer.buf = pBuffer;
    socketBuffer.len = bufferSize;
    if (0 != WSASendTo(
        so,
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
            goto __ERROR_EXIT;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oSend.hEvent, FALSE, WSA_INFINITE, FALSE);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        DWORD dwFlags;
        WSAResetEvent(oSend.hEvent);
        if (!WSAGetOverlappedResult(so, &oSend, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }

__ERROR_EXIT:
    WSACloseEvent(oSend.hEvent);

    return bytesTrans;
}

uint32_t udp_read(SOCKET so, struct sockaddr *pAddr, uint32_t *pAddrSize, uint8_t *pBuffer, uint32_t bufferSize)
{
    WSABUF          socketBuffer;
    WSAOVERLAPPED   oRecv = {0};
    DWORD           bytesTrans = 0;
    DWORD           dwFlags = 0;
    int             socketErrorCode;
    DWORD           wRes;

    oRecv.hEvent = WSACreateEvent();
    if (oRecv.hEvent == WSA_INVALID_EVENT) {
        return 0;
    }

    socketBuffer.buf = pBuffer;
    socketBuffer.len = bufferSize;
    if (0 != WSARecvFrom(
        so,
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
            goto __ERROR_EXIT;
        }
    }

    wRes = WSAWaitForMultipleEvents(1, &oRecv.hEvent, FALSE, WSA_INFINITE, FALSE);
    WSAResetEvent(oRecv.hEvent);
    if (wRes != WSA_WAIT_EVENT_0) {
    } else {
        if (!WSAGetOverlappedResult(so, &oRecv, &bytesTrans, FALSE, &dwFlags)) {
            socketErrorCode = WSAGetLastError();
            printf("\r\nWSAGetOverlappedResult failed. Error: %d", socketErrorCode);
        }
    }

__ERROR_EXIT:
    WSACloseEvent(oRecv.hEvent);

    return bytesTrans;
}

bool socket_init(void)
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

bool socket_deinit(void)
{
    //! Cleanup Winsocket.
    WSACleanup();

    return true;
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
