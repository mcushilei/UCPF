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

#ifndef __FRAMEWORK_TRACE_LOG_H__
#define __FRAMEWORK_TRACE_LOG_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define PRINT_LOG

#ifdef __DEBUG__
#define DBG_LOG(fmt, ...)  printf("\r\n[D][%s:%d]"fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_LOG(fmt, ...)
#endif

#ifdef PRINT_LOG
#ifdef __DEBUG__
#define RTT_LOG(fmt, ...)  printf("\r\n[I][%s:%d]"fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define RTT_LOG(fmt, ...)  printf("\r\n[I]"fmt, ##__VA_ARGS__)
#endif
#else
#define RTT_LOG(fmt, ...)
#endif



/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/

#endif  //!< #ifndef __FRAMEWORK_TRACE_LOG_H__
/* EOF */
