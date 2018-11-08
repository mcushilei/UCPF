/*******************************************************************************
 *  Copyright(C)2015-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
HANDLE comRcvEvent;

void com_event_handle(DWORD eventFlag)
{
    if (eventFlag & EV_RXCHAR) {
        SetEvent(comRcvEvent);
    }
}

com_t testCOM = {0};
uint32_t comWriteDataLength = 0, comReadDataLength = 0;
bool comStopRead = FALSE;
bool comStopWrite = FALSE;

DWORD WINAPI com_read_task(void *pArg)
{
    DWORD errorCode;
    CHAR  currentPath[MAX_PATH] = {0};

    if (GetCurrentDirectory(MAX_PATH, currentPath) == 0) {
        return GetLastError();
    }

    //! creat file.
    CHAR newFileName[MAX_PATH];
    HANDLE newFlie;
    strcpy_s(newFileName, MAX_PATH, currentPath);
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), "\\");
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), "my_file_1.txt");
    newFlie = CreateFile(
        newFileName,                    // file name.
        GENERIC_WRITE | GENERIC_READ,   // access mode: read and write.
        FILE_SHARE_READ,                // share mode.
        NULL,                           // security attribute.
        OPEN_ALWAYS,                    // creation disposition.
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   // flag and attribute.
        NULL);                          // handle to a template file.
    if (newFlie == INVALID_HANDLE_VALUE) {
        errorCode = GetLastError();
        printf("\r\nERROR, CreateFile fail: %u", errorCode);
        return errorCode;
    }

    //! write something in it.
    OVERLAPPED oWriteFile = {0};
    oWriteFile.hEvent       = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oWriteFile.hEvent == NULL) {
        errorCode = GetLastError();
        printf("\r\nERROR, CreateEvent fail: %u", errorCode);
        goto __Exit;
    }

    while (!comStopRead) {
        uint8_t     comBuffer[128];
        uint32_t    dataLength = 0;
        dataLength = com_read(&testCOM, comBuffer, sizeof(comBuffer));
        if (0 == dataLength) {
            WaitForSingleObject(comRcvEvent, 5000);
        } else {
            DWORD byteWritten;
            BOOL boolValue;
            oWriteFile.Offset       = comReadDataLength;
            oWriteFile.OffsetHigh   = 0;
            boolValue = WriteFile(newFlie,
                                  comBuffer,
                                  dataLength,
                                  NULL,
                                  &oWriteFile);
            if (boolValue == FALSE) {
                errorCode = GetLastError();
                if (errorCode == ERROR_IO_PENDING) {
                    DWORD waitRetValue;
                    waitRetValue = WaitForSingleObject(oWriteFile.hEvent, 5000);
                    switch (waitRetValue) {
                        case WAIT_ABANDONED:
                            break;
                        case WAIT_TIMEOUT:
                            printf("\r\n write timeout.");
                            CancelIoEx(newFlie, &oWriteFile);
                            if (WAIT_OBJECT_0 == WaitForSingleObject(oWriteFile.hEvent, 0)) {
                                //! write success
                            } else {
                            }
                            break;
                        case WAIT_OBJECT_0:
                            GetOverlappedResult(newFlie, &oWriteFile, &byteWritten, TRUE);
                            printf("\r\n Bytes to write: %u", dataLength);
                            printf("\r\n Bytes written: %u", byteWritten);
                            break;
                        case WAIT_FAILED:
                        default:
                            errorCode = GetLastError();
                            break;
                    }
                } else {
                    //! Fatal error.
                    break;
                }
            } else {
                //! write successfully.
            }
            comReadDataLength += dataLength;
        }
    }

__Exit:
    //! close file.
    CloseHandle(oWriteFile.hEvent);

    return 0;
}

DWORD WINAPI com_write_task(void *pArg)
{
    uint8_t comBuffer[128] = "sdjfpoiadjsfojuaowieur20987457";
    uint32_t dataLength = 0;

    while (!comStopWrite) {
        dataLength = com_write(&testCOM, comBuffer, strlen(comBuffer));
        comWriteDataLength += dataLength;
        Sleep(1000);
    }
    return 0;
}


int app_main(void)
{
    com_cfg_t comConfig;
    HANDLE hComReadTask, hComWriteTask;

    enum_all_comm();

    comConfig.hwMode = UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH;
    comConfig.wBaudrate = 115200;
    if (!com_open(&testCOM, "COM1", &comConfig, &com_event_handle)) {
        printf("Can not open COM1.\r\n");
    } else {
        comRcvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        hComReadTask = CreateThread(
            NULL,
            0,
            &com_read_task,
            NULL,
            0,
            NULL);
        hComWriteTask = CreateThread(
            NULL,
            0,
            &com_write_task,
            NULL,
            0,
            NULL);
    }


    MessageBox(NULL, "Stop port?", "Warning", MB_ICONEXCLAMATION | MB_OK);
    comStopWrite = TRUE;
    comStopRead  = TRUE;
    com_close(&testCOM);
    WaitForSingleObject(hComWriteTask, INFINITE);
    WaitForSingleObject(hComReadTask, INFINITE);

    CHAR megString[128];
    sprintf_s(megString, sizeof(megString), "Write:%u\r\nRead:%u", comWriteDataLength, comReadDataLength);
    MessageBox(NULL, megString, "success!", MB_ICONEXCLAMATION | MB_OK);

    CloseHandle(hComReadTask);
    CloseHandle(hComWriteTask);

    printf("\r\n");
    return 0;
}


 
/* EOF */
