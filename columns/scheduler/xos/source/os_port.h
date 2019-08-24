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

#ifndef __XOS_SOURCE_PORT_H__
#define __XOS_SOURCE_PORT_H__

/*
 *  \brief
 *
 *  the variables, functions declared here would be imported from the 'ports'.
 */


/*============================ INCLUDES ======================================*/
#include "..\ports\ports.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern CPU_STK *OSTaskStkInit(CPU_STK *ptos, void *wrap, void *task, void *parg);
extern void     OSStartTheFirstThread(void);
extern void     OSCtxSw(void);
extern void     OSIntCtxSw(void);
extern void     OSEnterCriticalSection(void);
extern void     OSExitCriticalSection(void);
extern CPU_REG  OSDisableInterrupt(void);
extern void     OSResumeInterrupt(CPU_REG level);

#if OS_HEAP_MEM_EN > 0
extern void     OSHeapInit(void);
extern void    *OSHeapAlloc(size_t size);
extern void     OSHeapFree(void *mem);
#endif

#if OS_HOOKS_EN > 0
extern void OSInitHookBegin(void);
extern void OSInitHookEnd(void);
extern void OSTaskCreateHook(OS_TCB *ptcb);
extern void OSTaskReturnHook(OS_TCB *ptcb, void *arg);
extern void OSTaskIdleHook(void);
extern void OSTaskStatHook(void);
extern void OSTaskSwHook(void);
extern void OSTCBInitHook(OS_TCB *ptcb);
extern void OSTimeTickHook(void);
#endif

#if OS_DEBUG_EN > 0u
extern void OSDebugInit(void);
#endif

#endif
