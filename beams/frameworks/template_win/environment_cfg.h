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

#ifndef __ENVIRONMENT_CFG_H__
#define __ENVIRONMENT_CFG_H__

/*============================ INCLUDES ======================================*/
#include "./ports/json_checker/JSON_checker_port.h"
#include "./ports/cJSON/cJSON_port.h"

/*============================ MACROS ========================================*/
#define BLOCK_MEM_BLOCK_SIZE    (512u)


/*============================ MACROFIED FUNCTIONS ===========================*/

#define TIMER_CRITICAL_SECTION_BEGIN    OS_CRITICAL_SECTION_BEGIN
#define TIMER_CRITICAL_SECTION_END      OS_CRITICAL_SECTION_END

#define CLOCK_CRITICAL_SECTION_BEGIN    OS_CRITICAL_SECTION_BEGIN
#define CLOCK_CRITICAL_SECTION_END      OS_CRITICAL_SECTION_END

#define POOL_CRITICAL_SECTION_BEGIN     OS_CRITICAL_SECTION_BEGIN
#define POOL_CRITICAL_SECTION_END       OS_CRITICAL_SECTION_END



/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif  //! #ifndef __ENVIRONMENT_CFG_H__
/* EOF */
