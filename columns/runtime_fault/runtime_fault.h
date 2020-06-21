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

#ifndef __BEAM_RUNTIME_FAULT_H__
#define __BEAM_RUNTIME_FAULT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#ifdef __DEBUG__
#define RTT_LOG(fmt, ...)  printf("\r\n[I][%s:%d]"fmt, __ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#define RTT_LOG(fmt, ...)  printf("\r\n[I]"fmt, ##__VA_ARGS__)
#endif


#if defined(__DEBUG__)
#define RUNTIME_FAULT(__error_code, __info)     \
        runtime_fault(__error_code, __ThisFileName, __LINE__, __info)
#else
#define RUNTIME_FAULT(__error_code, __info)     \
        runtime_fault(__error_code, __info)
#endif


#if defined(__DEBUG__)
#define LOWLEVEL_RUNTIME_FAULT(__error_code)    \
        runtime_fault(__error_code, __ThisFileName, __LINE__, NULL)
#else
#define LOWLEVEL_RUNTIME_FAULT(__error_code)    \
        runtime_fault(__error_code, NULL)
#endif

/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
#if defined(__DEBUG__)
extern void runtime_fault           (int ec, const char *file, int line, const char *info);
#else
extern void runtime_fault           (int ec, const char *info);
#endif

#endif  //!< #ifndef __BEAM_RUNTIME_FAULT_H__
/* EOF */
