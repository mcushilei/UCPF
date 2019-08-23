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


//! \note do not move this pre-processor statement to other places
#ifndef __XOS_SOURCE_CFG_H__
#define __XOS_SOURCE_CFG_H__

#include "..\os_cfg.h"

/*
 *  LOOK FOR MISSING AND ERROR #define CONSTANTS
 *
 *  This section is used to generate ERROR messages at compile time if certain #define constants are
 *  MISSING or ERROR in OS_CFG.H.  This allows you to quickly determine the source of the error.
 *
 *  You SHOULD NOT change this section UNLESS you would like to add more comments as to the source of the
 *  compile time error.
 */

//! FLAGS
#ifndef OS_FLAG_EN
#   error "OS_CFG.H, Missing OS_FLAG_EN: Enable (1) or Disable (0) code generation for Event Flags"
#else
#   ifndef OS_MAX_FLAGS
#       error "OS_CFG.H, Missing OS_MAX_FLAGS: Max. number of Event Flag Groups in your application"
#   else
#       if  OS_MAX_FLAGS > 65500u
#           error "OS_CFG.H, OS_MAX_FLAGS must be <= 65500"
#       endif
#   endif
#   ifndef  OS_FLAG_DEL_EN
#       error "OS_CFG.H, Missing OS_FLAG_DEL_EN: Include code for osFlagDelete()"
#   endif
#   ifndef  OS_FLAG_QUERY_EN
#       error "OS_CFG.H, Missing OS_FLAG_QUERY_EN: Include code for osFlagQuery()"
#   endif
#endif

//! MUTEXES
#ifndef OS_MUTEX_EN
#   error "OS_CFG.H, Missing OS_MUTEX_EN: Enable (1) or Disable (0) code generation for MUTEX"
#else
#   ifndef  OS_MUTEX_DEL_EN
#       error "OS_CFG.H, Missing OS_MUTEX_DEL_EN: Include code for osMutexDelete()"
#   endif
#   ifndef  OS_MUTEX_QUERY_EN
#       error "OS_CFG.H, Missing OS_MUTEX_QUERY_EN: Include code for osMutexQuery()"
#   endif
#endif

//! SEMAPHORES
#ifndef OS_SEM_EN
#   error "OS_CFG.H, Missing OS_SEM_EN: Enable (1) or Disable (0) code generation for SEMAPHORES"
#else
#   ifndef  OS_SEM_DEL_EN
#       error "OS_CFG.H, Missing OS_SEM_DEL_EN: Include code for osSemDelete()"
#   endif
#   ifndef  OS_SEM_PEND_ABORT_EN
#       error "OS_CFG.H, Missing OS_SEM_PEND_ABORT_EN: Include code for osSemPendAbort()"
#   endif
#   ifndef  OS_SEM_QUERY_EN
#       error "OS_CFG.H, Missing OS_SEM_QUERY_EN: Include code for osSemQuery()"
#   endif
#   ifndef  OS_SEM_SET_EN
#       error "OS_CFG.H, Missing OS_SEM_SET_EN: Include code for osSemSet()"
#   endif
#endif

//! TASK MANAGEMENT
#ifndef OS_TASK_CHANGE_PRIO_EN
#   error "OS_CFG.H, Missing OS_TASK_CHANGE_PRIO_EN: Include code for osTaskChangePrio()"
#endif

#ifndef OS_TASK_DEL_EN
#   error "OS_CFG.H, Missing OS_TASK_DEL_EN: Include code for os_task_del()"
#endif

//! TICKS
#ifndef OS_TICKS_PER_SEC
#   error "OS_CFG.H, Missing OS_TICKS_PER_SEC: Sets the number of ticks in one second"
#else
#   if  OS_TICKS_PER_SEC == 0
#       error "OS_CFG.H, OS_TICKS_PER_SEC must be > 0"
#   endif
#endif

//! TASK STACK SIZE
#ifndef OS_TASK_STAT_STK_SIZE
#   error "OS_CFG.H, Missing OS_TASK_STAT_STK_SIZE: Statistics task stack size"
#endif

#ifndef OS_TASK_IDLE_STK_SIZE
#   error "OS_CFG.H, Missing OS_TASK_IDLE_STK_SIZE: Idle task stack size"
#endif

//! MISCELLANEOUS
#ifndef OS_ARG_CHK_EN
#   error "OS_CFG.H, Missing OS_ARG_CHK_EN: Enable (1) or Disable (0) argument checking"
#endif

#ifndef OS_HOOKS_EN
#   error "OS_CFG.H, Missing OS_HOOKS_EN: Hooks are Enable (1) or Disable (0)"
#endif

#ifndef OS_DEBUG_EN
#   error "OS_CFG.H, Missing OS_DEBUG_EN: Allows you to include variables for debugging or not"
#endif

#ifndef OS_MAX_PRIO_LEVELS
#   error "OS_CFG.H, Missing OS_MAX_PRIO_LEVELS: Max. levels of priority in your application"
#else
#   if  OS_MAX_PRIO_LEVELS <  1u
#       error "OS_CFG.H, OS_MAX_PRIO_LEVELS must be >= 1"
#   endif
#   if  OS_MAX_PRIO_LEVELS >  256u
#       error "OS_CFG.H, OS_MAX_PRIO_LEVELS must be <= 256"
#   endif
#endif

#ifndef OS_MAX_SEMAPHORES
#   error "OS_CFG.H, Missing OS_MAX_SEMAPHORES: Max. number of event control blocks in your application"
#else
#   if  OS_MAX_SEMAPHORES > 65535u
#       error "OS_CFG.H, OS_MAX_SEMAPHORES must be <= 65535"
#   endif
#endif

#ifndef OS_MAX_MUTEXES
#   error "OS_CFG.H, Missing OS_MAX_MUTEXES: Max. number of event control blocks in your application"
#else
#   if  OS_MAX_MUTEXES > 65535u
#       error "OS_CFG.H, OS_MAX_MUTEXES must be <= 65535"
#   endif
#endif

#ifndef OS_MAX_FLAGS
#   error "OS_CFG.H, Missing OS_MAX_FLAGS: Max. number of event control blocks in your application"
#else
#   if  OS_MAX_FLAGS > 65535u
#       error "OS_CFG.H, OS_MAX_FLAGS must be <= 65535"
#   endif
#endif

#ifndef OS_SCHED_LOCK_EN
#   error "OS_CFG.H, Missing OS_SCHED_LOCK_EN: Include code for osLockSched() and osUnlockSched()"
#endif

#ifndef OS_TASK_SW_HOOK_EN
#   error "OS_CFG.H, Missing OS_TASK_SW_HOOK_EN: Allows you to include the code for OSTaskSwHook() or not"
#endif

#ifndef OS_TIME_TICK_HOOK_EN
#   error "OS_CFG.H, Missing OS_TIME_TICK_HOOK_EN: Allows you to include the code for OSTimeTickHook() or not"
#endif

#ifndef OS_STAT_EN
#   error "OS_CFG.H, Missing OS_STAT_EN: Enable (1) or Disable(0) the statistics task"
#endif

#ifndef OS_STAT_TASK_STK_CHK_EN
#   error "OS_CFG.H, Missing OS_STAT_TASK_STK_CHK_EN: Check task stacks from statistics task"
#endif

//! SAFETY CRITICAL USE
#ifdef SAFETY_CRITICAL_RELEASE
#   if    OS_ARG_CHK_EN == 0u
#       error "OS_CFG.H, OS_ARG_CHK_EN must be enabled for safety-critical release code"
#   endif
#   if    OS_DEBUG_EN > 0u
#       error "OS_CFG.H, OS_DEBUG_EN must be disabled for safety-critical release code"
#   endif
#   if    OS_STAT_EN > 0u
#       error "OS_CFG.H, OS_STAT_EN must be disabled for safety-critical release code"
#   endif
#   if    OS_TASK_DEL_EN > 0u
#       error "OS_CFG.H, OS_TASK_DEL_EN must be disabled for safety-critical release code"
#   endif
#   if    OS_FLAG_EN > 0u
#       if OS_FLAG_DEL_EN > 0
#           error "OS_CFG.H, OS_FLAG_DEL_EN must be disabled for safety-critical release code"
#       endif
#   endif
#   if    OS_MUTEX_EN > 0u
#       if OS_MUTEX_DEL_EN > 0u
#           error "OS_CFG.H, OS_MUTEX_DEL_EN must be disabled for safety-critical release code"
#       endif
#   endif
#   if    OS_SEM_EN > 0u
#       if OS_SEM_DEL_EN > 0u
#           error "OS_CFG.H, OS_SEM_DEL_EN must be disabled for safety-critical release code"
#       endif
#   endif
#   if    OS_CRITICAL_METHOD != 3u
#       error "OS_CPU.H, OS_CRITICAL_METHOD must be type 3 for safety-critical release code"
#   endif
#endif  //!< SAFETY_CRITICAL_RELEASE


#endif  //!< __XOS_SOURCE_CFG_H__
