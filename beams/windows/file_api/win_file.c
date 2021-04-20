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
#include "./win_file.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
THIS_FILE_NAME("win_file");

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool file_api_open(file_t *pHandle, char *fileName)
{
    CHAR  currentPath[MAX_PATH] = {0};

    if (GetCurrentDirectory(MAX_PATH, currentPath) == 0) {
        DBG_LOG("GetCurrentDirectory fail: %u", GetLastError());
        return false;
    }

    //! open existing file.
    CHAR newFileName[MAX_PATH];
    strcpy_s(newFileName, MAX_PATH, currentPath);
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), "\\");
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), fileName);
    *pHandle = CreateFile(
        newFileName,                    // file name.
        GENERIC_WRITE | GENERIC_READ,   // access mode: read and write.
        FILE_SHARE_READ,                // share mode.
        NULL,                           // security attribute.
        OPEN_ALWAYS,                    // creation disposition.
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   // flag and attribute.
        NULL);                          // handle to a template file.
    if (*pHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

bool file_api_get_size(file_t fileHandle, uint64_t *fileSize)
{
    LARGE_INTEGER sizeValue;
    if (!GetFileSizeEx(fileHandle, &sizeValue)) {
        return false;
    }
    *fileSize = sizeValue.QuadPart;
    return true;
}

bool file_api_close(file_t fileHandle)
{
    //! close file.
    CloseHandle(fileHandle);

    return true;
}

bool file_api_write(file_t fileHandle, uint64_t offset, const char *buf, uint32_t bufSize)
{
    DWORD errorCode;
    bool retVal = false;

    OVERLAPPED oWriteFile = {0};
    oWriteFile.Offset       = offset;
    oWriteFile.OffsetHigh   = offset >> 32;
    oWriteFile.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oWriteFile.hEvent == NULL) {
        errorCode = GetLastError();
        DBG_LOG("CreateEvent fail: %u", errorCode);
        return false;
    }

    DWORD byteWritten = 0u;
    BOOL  boolValue;
    boolValue = WriteFile(  fileHandle,
                            buf,
                            bufSize,
                            NULL,
                            &oWriteFile);
    if (boolValue == FALSE) {
        errorCode = GetLastError();
        if (errorCode != ERROR_IO_PENDING) {
            //! Fatal error.
            DBG_LOG("WriteFile fail: %u", errorCode);
            goto __Exit;
        }
    } else {
        //! wait to finish writig.
    }

    DWORD waitRetValue;
    waitRetValue = WaitForSingleObject(oWriteFile.hEvent, 5000);
    switch (waitRetValue) {
        case WAIT_ABANDONED:
            goto __Exit;

        case WAIT_TIMEOUT:
            CancelIoEx(fileHandle, &oWriteFile);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oWriteFile.hEvent, 0)) {
                //! write ok
            } else {
                break;
            }

        case WAIT_OBJECT_0:
            GetOverlappedResult(fileHandle, &oWriteFile, &byteWritten, TRUE);
            if (byteWritten != bufSize) {
                goto __Exit;
            }
            break;

        case WAIT_FAILED:
            errorCode = GetLastError();
            goto __Exit;

        default:
            break;
    }
    retVal = true;

__Exit:
    CloseHandle(oWriteFile.hEvent);
    return retVal;
}

bool file_api_read(file_t fileHandle, uint64_t offset, char *buf, uint32_t *dataLength)
{
    DWORD errorCode;
    bool retVal = false;
    DWORD bufSize = *dataLength;

    *dataLength = 0;

    OVERLAPPED oWriteFile = {0};
    oWriteFile.Offset       = offset;
    oWriteFile.OffsetHigh   = offset >> 32;
    oWriteFile.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oWriteFile.hEvent == NULL) {
        errorCode = GetLastError();
        DBG_LOG("CreateEvent fail: %u", errorCode);
        return false;
    }

    DWORD lengthRead = 0;
    BOOL  boolValue;
    boolValue = ReadFile(  fileHandle,
                            buf,
                            bufSize,
                            NULL,
                            &oWriteFile);
    if (boolValue == FALSE) {
        errorCode = GetLastError();
        if (errorCode != ERROR_IO_PENDING) {
            //! Fatal error.
            DBG_LOG("WriteFile fail: %u", errorCode);
            goto __Exit;
        }
    } else {
        //! wait to finish writig.
    }

    DWORD waitRetValue;
    waitRetValue = WaitForSingleObject(oWriteFile.hEvent, 5000);
    switch (waitRetValue) {
        case WAIT_ABANDONED:
            goto __Exit;

        case WAIT_TIMEOUT:
            CancelIoEx(fileHandle, &oWriteFile);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oWriteFile.hEvent, 0)) {
                //! read ok
                GetOverlappedResult(fileHandle, &oWriteFile, &lengthRead, TRUE);
                *dataLength = lengthRead;
            } else {
                break;
            }

        case WAIT_OBJECT_0:
            GetOverlappedResult(fileHandle, &oWriteFile, &lengthRead, TRUE);
            *dataLength = lengthRead;
            break;

        case WAIT_FAILED:
            errorCode = GetLastError();
            goto __Exit;

        default:
            break;
    }
    retVal = true;

__Exit:
    CloseHandle(oWriteFile.hEvent);
    return retVal;
}

bool file_api_erase_all(file_t fileHandle)
{
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(fileHandle, &fileSize)) {
        DBG_LOG("GetFileSizeEx error: %lu", GetLastError());
        return false;
    } else {
    }

    SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN);
    if (!SetEndOfFile(fileHandle)) {
        DBG_LOG("SetEndOfFile error: %lu", GetLastError());
        return false;
    }

    return true;
}


/* EOF */
