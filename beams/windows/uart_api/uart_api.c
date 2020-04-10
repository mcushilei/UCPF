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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\uart_api.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)
#define UART_PARITY_MSK         (0x000fu)
#define UART_STOPBIT_MSK        (0x00f0u)
#define UART_BIT_LENGTH_MSK     (0x0f00u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
bool com_close(com_t *ptThis);
static DWORD WINAPI com_hardware_event_task(void *pArg);

/*============================ IMPLEMENTATION ================================*/
static void com_in_error(com_t *ptThis)
{
    com_close(ptThis);
}

uint32_t enum_all_comm(void)
{
    HKEY hkey;
    uint32_t result;

    result = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        "Hardware\\DeviceMap\\SerialComm",
        0,
        KEY_READ,
        &hkey);

    if (ERROR_SUCCESS == result) {
        uint8_t portName[128], commName[128];
        uint32_t wLong, wSize;
        uint32_t i = 0;

        do {
            wSize = sizeof(portName) / sizeof(uint8_t);
            wLong = wSize;
            result = RegEnumValue(
                hkey,
                i,
                portName,
                &wLong,
                NULL,
                NULL,
                commName,
                &wSize);
            if (ERROR_SUCCESS == result) {
                printf("\r\nport name: %s", portName);
                printf("\r\ncom: %s", commName);
                i++;
            }
        } while (ERROR_SUCCESS == result);
        if (ERROR_NO_MORE_ITEMS == result) {
            printf("\r\nall %u com port.\r\n", i);
        } else {
            printf("\r\nError %u.\r\n", result);
        }
        RegCloseKey(hkey);
    }

    return 0;
}

static bool com_cfg(com_t *ptThis, const com_cfg_t *ptCfg)
{
    DCB             tDCB = { 0 };
    COMMTIMEOUTS    tTimeOuts = { 0 };

    if (!GetCommState(this.hCom, &tDCB)) {
        return false;
    }

    switch (ptCfg->hwMode & UART_PARITY_MSK) {
        case UART_EVEN_PARITY:
            tDCB.fParity    = 1;
            tDCB.Parity     = EVENPARITY;
            break;
        case UART_ODD_PARITY:
            tDCB.fParity    = 1;
            tDCB.Parity     = ODDPARITY;
            break;
        case UART_NO_PARITY:
        default:
            tDCB.fParity    = 0;
            tDCB.Parity     = NOPARITY;
            break;
    }

    switch (ptCfg->hwMode & UART_STOPBIT_MSK) {
        case UART_1_STOPBIT:
        default:
            tDCB.StopBits = ONESTOPBIT;
            break;
        case UART_2_STOPBIT:
            tDCB.StopBits = TWOSTOPBITS;
            break;
    }

    switch (ptCfg->hwMode & UART_BIT_LENGTH_MSK) {
        case UART_5_BIT_LENGTH:
            tDCB.ByteSize = 5;
            break;
        case UART_6_BIT_LENGTH:
            tDCB.ByteSize = 6;
            break;
        case UART_7_BIT_LENGTH:
            tDCB.ByteSize = 7;
            break;
        default:
        case UART_8_BIT_LENGTH:
            tDCB.ByteSize = 8;
            break;
    }

    tDCB.BaudRate = ptCfg->wBaudrate;

    if (!SetCommState(this.hCom, &tDCB)) {
        return false;
    }

    uint32_t wBitsPerByte = 1 + tDCB.ByteSize + (uint32_t)tDCB.fParity + tDCB.StopBits;
    uint32_t wBytesPerSecond = (tDCB.BaudRate + wBitsPerByte - 1) / wBitsPerByte;
    uint32_t wByteTime = (1000 + wBytesPerSecond - 1) / wBytesPerSecond;
    tTimeOuts.ReadIntervalTimeout           = wByteTime;
    tTimeOuts.ReadTotalTimeoutMultiplier    = wByteTime;
    tTimeOuts.ReadTotalTimeoutConstant      = 0;
    tTimeOuts.WriteTotalTimeoutMultiplier   = wByteTime;
    tTimeOuts.WriteTotalTimeoutConstant     = 0;
    SetCommTimeouts(this.hCom, &tTimeOuts);

    SetupComm(this.hCom, COM_DEVICE_BUFFER_SIZE, COM_DEVICE_BUFFER_SIZE);
    PurgeComm(this.hCom, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

    SetCommMask(this.hCom, EV_RXCHAR);

    return true;
}

bool com_open(com_t *ptThis, const uint8_t *pchCom, const com_cfg_t *ptCfg, void *pComEventHandle)
{
    if ((NULL == ptThis) || (NULL == pchCom) || (NULL == ptCfg)) {
        return false;
    }

    ZeroMemory(ptThis, sizeof(ptThis));

    uint8_t comName[32] = "\\\\.\\";
    if (0u != strncat_s(comName, _countof(comName), pchCom, _TRUNCATE)) {
        com_in_error(ptThis);
        return false;
    }

    this.hCom = CreateFile(
                    pchCom,
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    NULL);
    if (INVALID_HANDLE_VALUE == this.hCom) {
        com_in_error(ptThis);
        return false;
    }

    this.hHardwareEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == this.hHardwareEvent) {
        com_in_error(ptThis);
        return false;
    }

    //! configration.
    if (!com_cfg(ptThis, ptCfg)) {
        com_in_error(ptThis);
        return false;
    }

    this.pComEventHandle = pComEventHandle;
    this.hHardwareThread = CreateThread(
        NULL,
        0,
        &com_hardware_event_task,
        ptThis,
        0,
        NULL);
    if (NULL == this.hHardwareThread) {
        com_in_error(ptThis);
        return false;
    }

    this.bIsOpened = true;

    return true;
}

bool com_close(com_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }

    if (INVALID_HANDLE_VALUE == this.hCom) {
        return false;
    }

    CancelIoEx(this.hCom, NULL);
    PurgeComm(this.hCom, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
    ptThis->flagClosePort = 1;
    WaitForSingleObject(this.hHardwareThread, 5000);
    CloseHandle(this.hCom);
    CloseHandle(this.hHardwareEvent);
    CloseHandle(this.hHardwareThread);

    ZeroMemory(ptThis, sizeof(ptThis));

    return true;
}


/*! \note   write data to com directly.
 *  \param  ptThis      com to write
 *  \param  pchData     pointer to the buffer containing the data to be written
 *  \param  wLen        number of bytes to be written
 *  \retval number of bytes has been written.
 */
uint32_t com_write(com_t *ptThis, uint8_t *pchData, uint32_t wLen, uint32_t timeoutMS)
{
    DWORD       numBytesWrite = 0;
    OVERLAPPED  oWriter = {0};
    DWORD       errorCode;
    DWORD       wRes;

    if ((0 == wLen) || (NULL == pchData) || (NULL == ptThis))  {
        return 0;
    }

    if (INVALID_HANDLE_VALUE == this.hCom) {
        return 0;
    }

    oWriter.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oWriter.hEvent == NULL) {
        return 0;
    }

    if (FALSE == WriteFile( 
        this.hCom,
        pchData,
        wLen,
        NULL,
        &oWriter)) {
        errorCode = GetLastError();
        if (ERROR_IO_PENDING != errorCode) {
            //! Fatal Error.
            goto __ERROR_EXIT;
        }
    }

    wRes = WaitForSingleObject(oWriter.hEvent, INFINITE);
    switch (wRes) {
        case WAIT_ABANDONED:
            goto __ERROR_EXIT;

        case WAIT_OBJECT_0:
            if (!GetOverlappedResult(this.hCom, &oWriter, &numBytesWrite, FALSE)) {
            }
            break;

        case WAIT_TIMEOUT:
            CancelIoEx(this.hCom, &oWriter);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oWriter.hEvent, 0)) {
                //! write ok
            } else {
                break;
            }
        case WAIT_FAILED:
            errorCode = GetLastError();
            goto __ERROR_EXIT;

        default:
            break;
    }

__ERROR_EXIT:
    CloseHandle(oWriter.hEvent);

    return numBytesWrite;
}

/*! \brief  read data from com directly.This function will not return untill there is
 *          some data received or there is some error happen.
 *  \param  ptThis      com to write
 *  \param  pchData     the buffer where the received data to be saved.
 *  \param  wLen        length of buffer
 *  \retval number of bytes in buffer.
 */
uint32_t com_read(com_t *ptThis, uint8_t *pchData, uint32_t wLen, uint32_t timeoutMS)
{
    DWORD       numBytesRead = 0;
    OVERLAPPED  oReader = {0};
    DWORD       errorCode;
    DWORD       wRes;

    if ((0 == wLen) || (NULL == pchData) || (NULL == ptThis)) {
        return 0;
    }

    if (INVALID_HANDLE_VALUE == this.hCom) {
        return 0;
    }

    oReader.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (NULL == oReader.hEvent) {
        return 0;
    }

    if (FALSE == ReadFile(
        this.hCom,
        pchData,
        wLen,
        NULL,
        &oReader)) {
        errorCode = GetLastError();
        if (ERROR_IO_PENDING != errorCode) {
            //! Fatal Error.
            goto __ERROR_EXIT;
        }
    }

    wRes = WaitForSingleObject(oReader.hEvent, timeoutMS);
    switch (wRes) {
        case WAIT_ABANDONED:
            goto __ERROR_EXIT;

        case WAIT_OBJECT_0:
            if (!GetOverlappedResult(this.hCom, &oReader, &numBytesRead, FALSE)) {
            }
            break;

        case WAIT_TIMEOUT:
            CancelIoEx(this.hCom, &oReader);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oReader.hEvent, 0)) {
                //! write ok
            } else {
                break;
            }
        case WAIT_FAILED:
            errorCode = GetLastError();
            goto __ERROR_EXIT;

        default:
            break;
    }

__ERROR_EXIT:
    CloseHandle(oReader.hEvent);

    return numBytesRead;
}

bool com_flush_buffer(com_t *ptThis)
{
    if (0 == PurgeComm(this.hCom, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR)) {
        return false;
    } else {
        return true;
    }
}

static DWORD WINAPI com_hardware_event_task(void *pArg)
{
    OVERLAPPED  oStatus = {0};
    DWORD       wRes, commEvent;
    com_t      *ptThis = (com_t *)pArg;
    DWORD  errCode;

    oStatus.hEvent = this.hHardwareEvent;

    while (this.flagClosePort == 0) {
        if (!WaitCommEvent(this.hCom, &commEvent, &oStatus)) {
            errCode = GetLastError();
            if (ERROR_IO_PENDING != errCode) {
                return 0;
            } else {
                wRes = WaitForSingleObject(oStatus.hEvent, INFINITE);
                if (WAIT_OBJECT_0 != wRes) {
                    return 0;
                }
            }
        }

        if (NULL != this.pComEventHandle) {
            this.pComEventHandle(commEvent);
        }
    }

    return 0;
}

