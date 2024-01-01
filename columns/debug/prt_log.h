/*******************************************************************************
 *  Copyright(C)2023      by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __SERVICE_PRT_LOG_H__
#define __SERVICE_PRT_LOG_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include <stdio.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#if !defined(STRING_EOL)
#   define STRING_EOL        "\n"
#endif

//! \brief run-time message log
//! {
#if defined(__DEBUG__)
#   define PRT_LOG(fmt, ...)    printf("[I][%s:%d]" fmt STRING_EOL, __ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#   define PRT_LOG(fmt, ...)    printf("[I]" fmt STRING_EOL, ##__VA_ARGS__)
#endif
//! }

/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif      //!< #ifndef __SERVICE_PRT_LOG_H__
/* EOF */
