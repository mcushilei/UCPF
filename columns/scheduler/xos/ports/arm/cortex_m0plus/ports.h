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

#ifndef  __OS_CORTEX_M0_PORTS_H__
#define  __OS_CORTEX_M0_PORTS_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\os_cpu_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#if     OS_CRITICAL_METHOD == 3
    #define OSEnterCriticalSection(__INT)   __INT = OS_CPU_ATOM_CODE_BEGIN()
    #define OSExitCriticalSection(__INT)    OS_CPU_ATOM_CODE_END(__INT)
#elif   OS_CRITICAL_METHOD == 2
    #define OSEnterCriticalSection(__INT)   os_cpu_enter_critical_section()
    #define OSExitCriticalSection(__INT)    os_cpu_exit_critical_section()
#else
    #error "This CPU does not support this method!"
#endif


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
#if     OS_CRITICAL_METHOD == 3
extern uint32_t OS_CPU_ATOM_CODE_BEGIN(void);
extern void     OS_CPU_ATOM_CODE_END(uint32_t);
#elif   OS_CRITICAL_METHOD == 2
extern void os_cpu_exit_critical_section(void);
extern void os_cpu_enter_critical_section(void);
#endif


#endif
