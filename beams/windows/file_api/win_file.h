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

#ifndef __WIN_FILE_H__
#define __WIN_FILE_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef HANDLE  file_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool file_api_open(file_t *pHandle, char *fileName);
extern bool file_api_close(file_t fileHandle);
extern bool file_api_get_size(file_t fileHandle, uint64_t *fileSize);
extern bool file_api_write(file_t fileHandle, uint64_t offset, const char *buf, uint32_t bufSize);
extern bool file_api_read(file_t fileHandle, uint64_t offset, char *buf, uint32_t bufSize);
extern bool file_api_erase_all(file_t fileHandle);

#endif  //!< #ifndef __WIN_FILE_H__
/* EOF */
