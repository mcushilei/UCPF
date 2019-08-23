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


#ifndef __XOS_OS_H__
#define __XOS_OS_H__

/*============================ INCLUDES ======================================*/
#include ".\source\os.h"

/*============================ MACROS ========================================*/
#define OS_CRITICAL_SECTION_DEFINE()    
#define OS_CRITICAL_SECTION_BEGIN()         osEnterCriticalSection()
#define OS_CRITICAL_SECTION_END()           osExitCriticalSection()


#define OS_QUEUE_TYPE                       OS_HANDLE
#define OS_QUEUE_CREATE(__Q, __BUF, __L)    osQueueCreate(&(__Q), __BUF, __L)
#define OS_QUEUE_DELETE(__Q)                osQueueDelete(__Q, OS_DEL_ALWAYS)
#define OS_QUEUE_WRITE(__Q, __BUF, __T)     osQueueWrite(__Q, __BUF, __T)
#define OS_QUEUE_READ(__Q, __BUF, __T)      osQueueRead(__Q, __BUF, __T)


#define OS_SEMAPHORE_TYPE                   OS_HANDLE
#define OS_SEMAPHORE_CREATE(__SEM, __CNT)   osSemCreate(&(__SEM), __CNT)
#define OS_SEMAPHORE_DELETE(__SEM)          osSemDelete(__SEM, OS_DEL_ALWAYS)
#define OS_SEMAPHORE_RELEASE(__SEM)         osSemPost(__SEM, 1u)
#define OS_SEMAPHORE_WAIT(__SEM, __T)       osSemPend(__SEM, __T)


#define OS_MUTEX_TYPE                       OS_HANDLE
#define OS_MUTEX_CREATE(__MUTEX)            osMutexCreate(&(__MUTEX), OS_TASK_LOWEST_PRIO)
#define OS_MUTEX_DELETE(__MUTEX)            osMutexDelete(__MUTEX, OS_DEL_ALWAYS)
#define OS_MUTEX_RELEASE(__MUTEX)           osMutexPost(__MUTEX)
#define OS_MUTEX_WAIT(__MUTEX, __T)         osMutexPend(__MUTEX, __T)


#define OS_FLAG_TYPE                        OS_HANDLE
#define OS_FLAG_CREATE(__FLAG, __BMANUAL, __BINITVAL) osFlagCreate(&(__FLAG), __BINITVAL, __BMANUAL)
#define OS_FLAG_DELETE(__FLAG)              osFlagDelete(__FLAG, OS_DEL_ALWAYS)
#define OS_FLAG_SET(__FLAG)                 osFlagSet(__FLAG)
#define OS_FLAG_RESET(__FLAG)               osFlagReset(__FLAG)
#define OS_FLAG_WAIT(__FLAG, __T)           osFlagPend(__FLAG, __T)


#define OS_TASK_CREATE(__HANDLE, __NAME, __ENTRY, __ARG,  __STACK_SIZE, __RIO, __OPT)   \
    osTaskCreate( &(__HANDLE), __ENTRY, __ARG,  NULL, __STACK_SIZE, __OPT, __RIO );
#define OS_TASK_ENTRY(__TASK)               void *__TASK(void *pArg)
#define OS_TASK_ARG                         (pArg)
#define OS_TASK_SLEEP(__T)                  osTaskSleep(__T)



                                                
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
