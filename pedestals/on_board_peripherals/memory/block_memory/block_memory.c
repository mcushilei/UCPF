/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! \note do not move this pre-processor statement to other places
#define __BLOCK_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define BLOCK_SIZE      (512u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    HANDLE      File;
    uint32_t    BlockSize;
    uint32_t    BlockNum;
} mem_info_t;

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static mem_info_t   mem;

/*============================ IMPLEMENTATION ================================*/

bool block_memory_init(void)
{
    DWORD errorCode;
    CHAR  currentPath[MAX_PATH] = {0};

    if (GetCurrentDirectory(MAX_PATH, currentPath) == 0) {
        return GetLastError();
    }

    //! creat file.
    CHAR newFileName[MAX_PATH];
    strcpy_s(newFileName, MAX_PATH, "D:");
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), "\\");
    strcat_s(newFileName, MAX_PATH - strlen(newFileName), "flash_file.dat");
    mem.File = CreateFile(
        newFileName,                    // file name.
        GENERIC_WRITE | GENERIC_READ,   // access mode: read and write.
        FILE_SHARE_READ,                // share mode.
        NULL,                           // security attribute.
        OPEN_ALWAYS,                    // creation disposition.
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   // flag and attribute.
        NULL);                          // handle to a template file.
    if (mem.File == INVALID_HANDLE_VALUE) {
        errorCode = GetLastError();
        printf("\r\nERROR, CreateFile fail: %u", errorCode);
        return false;
    }

    mem.BlockSize = BLOCK_SIZE;
    mem.BlockNum = 1024u * 1024u * 1024u / BLOCK_SIZE * 4;

    return true;
}

bool block_memory_write_block(uint32_t block, uint8_t *pData)
{
    DWORD errorCode;
    DWORD byteWritten;
    BOOL  boolValue;
    OVERLAPPED  oWriteFile;
    DWORD waitRetValue;

    //! write something in it.
    ZeroMemory(&oWriteFile, sizeof(oWriteFile));
    oWriteFile.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oWriteFile.hEvent == NULL) {
        errorCode = GetLastError();
        printf("\r\nERROR, CreateEvent fail: %u", errorCode);
        //! close file.
        return false;
    }

    oWriteFile.Offset       = block * BLOCK_SIZE;
    oWriteFile.OffsetHigh   = 0;
    boolValue = WriteFile(mem.File,
                          pData,
                          BLOCK_SIZE,
                          NULL,
                          &oWriteFile);
    if (boolValue == FALSE) {
        errorCode = GetLastError();
        if (errorCode != ERROR_IO_PENDING) {
            //! Fatal error.
            return false;
        }
    } else {
        //! write successfully.
    }

    waitRetValue = WaitForSingleObject(oWriteFile.hEvent, 5000);
    switch (waitRetValue) {
        case WAIT_ABANDONED:
            return false;

        case WAIT_TIMEOUT:
            printf("\r\n write timeout.");
            CancelIoEx(mem.File, &oWriteFile);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oWriteFile.hEvent, 0)) {
                //! write success
            } else {
                return false;
            }
            //break;

        case WAIT_OBJECT_0:
            GetOverlappedResult(mem.File, &oWriteFile, &byteWritten, TRUE);
            break;

        case WAIT_FAILED:
        default:
            errorCode = GetLastError();
            return false;
    }

    return true;
}

bool block_memory_read_block(uint32_t block, uint8_t *pData)
{
    DWORD errorCode;
    DWORD byteRead;
    BOOL  boolValue;
    OVERLAPPED  oReadFile;
    DWORD waitRetValue;

    ZeroMemory(&oReadFile, sizeof(oReadFile));
    oReadFile.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (oReadFile.hEvent == NULL) {
        errorCode = GetLastError();
        printf("\r\nERROR, CreateEvent fail: %u", errorCode);
        //! close file.
        return false;
    }

    oReadFile.Offset       = block * BLOCK_SIZE;
    oReadFile.OffsetHigh   = 0;
    boolValue = ReadFile(mem.File,
                          pData,
                          BLOCK_SIZE,
                          NULL,
                          &oReadFile);
    if (boolValue == FALSE) {
        errorCode = GetLastError();
        if (errorCode != ERROR_IO_PENDING) {
            //! Fatal error.
            return false;
        }
    } else {
        //! write successfully.
    }

    waitRetValue = WaitForSingleObject(oReadFile.hEvent, 5000);
    switch (waitRetValue) {
        case WAIT_ABANDONED:
            return false;

        case WAIT_TIMEOUT:
            printf("\r\n read timeout.");
            CancelIoEx(mem.File, &oReadFile);
            if (WAIT_OBJECT_0 == WaitForSingleObject(oReadFile.hEvent, 0)) {
                //! read success
            } else {
                return false;
            }
            //break;

        case WAIT_OBJECT_0:
            GetOverlappedResult(mem.File, &oReadFile, &byteRead, TRUE);
            break;

        case WAIT_FAILED:
            errorCode = GetLastError();
        default:
            return false;
    }

    return true;
}

bool block_memory_write(uint32_t addr, uint8_t *pData, uint32_t dataLength)
{
    uint32_t blockAddr, byteAddr, byteCnt;
    uint8_t buffer[BLOCK_SIZE];
    uint8_t state = 0;

    blockAddr = addr / BLOCK_SIZE;
    byteAddr = addr % BLOCK_SIZE;

    if (byteAddr == 0u) {
        state = 1;
    } else {
        state = 0;
    }

    switch (state) {
        case 0:
            byteCnt = BLOCK_SIZE - byteAddr;
            byteCnt = (byteCnt < dataLength) ? byteCnt : dataLength;
            memset(buffer, 0, BLOCK_SIZE);
            if (false == block_memory_read_block(blockAddr, buffer)) {
                return false;
            }
            memcpy(buffer + byteAddr, pData, byteCnt);
            if (false == block_memory_write_block(blockAddr, buffer)) {
                return false;
            }
            dataLength -= byteCnt;
            pData += byteCnt;
            blockAddr++;
            if (dataLength == 0) {
                return true;
            } else {
                state = 1;
            }

        case 1:
            while (dataLength >= BLOCK_SIZE) {
                if (false == block_memory_write_block(blockAddr, pData)) {
                    return false;
                }
                dataLength -= BLOCK_SIZE;
                pData += BLOCK_SIZE;
                blockAddr++;
            }
            if (dataLength == 0) {
                return true;
            } else {
                state = 2;
            }

        case 2:
            memset(buffer, 0, BLOCK_SIZE);
            if (false == block_memory_read_block(blockAddr, buffer)) {
                return false;
            }
            memcpy(buffer, pData, dataLength);
            if (false == block_memory_write_block(blockAddr, buffer)) {
                return false;
            }
            break;
    }

    return true;
}

bool block_memory_read(uint32_t addr, uint8_t *pData, uint32_t dataLength)
{
    uint32_t blockAddr, byteAddr, byteCnt;
    uint8_t buffer[BLOCK_SIZE];
    uint8_t state = 0;

    blockAddr = addr / BLOCK_SIZE;
    byteAddr = addr % BLOCK_SIZE;

    switch (state) {
        case 0:
            byteCnt = BLOCK_SIZE - byteAddr;
            byteCnt = (byteCnt < dataLength) ? byteCnt : dataLength;
            if (false == block_memory_read_block(blockAddr, buffer)) {
                return false;
            }
            memcpy(pData, buffer + byteAddr, byteCnt);
            dataLength -= byteCnt;
            pData += byteCnt;
            blockAddr++;
            if (dataLength == 0) {
                return true;
            } else {
                state = 1;
            }

        case 1:
            while (dataLength >= BLOCK_SIZE) {
                if (false == block_memory_read_block(blockAddr, pData)) {
                    return false;
                }
                dataLength -= BLOCK_SIZE;
                pData += BLOCK_SIZE;
                blockAddr++;
            }
            if (dataLength == 0) {
                return true;
            } else {
                state = 2;
            }

        case 2:
            if (false == block_memory_read_block(blockAddr, buffer)) {
                return false;
            }
            memcpy(pData, buffer, dataLength);
            break;
    }

    return true;
}

/* EOF */
