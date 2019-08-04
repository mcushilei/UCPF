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
#ifndef __OS_CFG_H__
#define __OS_CFG_H__

#include ".\app_cfg.h"

//! default configration.
#ifndef __USER_XOS_CFG__

//! MISCELLANEOUS
#define OS_HOOKS_EN               1u   //!< Hooks are found in the processor port files

#define OS_DEBUG_EN               1u   //!< Enable (1) debug variables
#define OS_ARG_CHK_EN             1u   //!< Enable (1) or Disable (0) argument checking
#define OS_SCHED_LOCK_EN          1u   //!< Include code for osLockSched() and osUnlockSched()

#define OS_MAX_PRIO_LEVELS       64u   //!< Max. number of priority levels ...
                                       //!< ... MUST be >= 1 and <= 256!

#define OS_MAX_QUEUES            16u   //!< Max. number of queues    in your application
#define OS_MAX_SEMAPHORES        16u   //!< Max. number of semaphore in your application
#define OS_MAX_MUTEXES           16u   //!< Max. number of mutex     in your application
#define OS_MAX_FLAGS             16u   //!< Max. number of flag      in your application
#define OS_MAX_TASKS             14u   //!< Max. number of tasks     in your application

#define OS_TICKS_PER_SEC        100u   //!< Set the number of ticks in one second
#define OS_TIME_TICK_HOOK_EN      1u   //!< Include code for OSTimeTickHook()

#define OS_STAT_EN                1u   //!< Enable (1) or Disable (0) the statistics task
#define OS_STAT_TASK_STK_CHK_EN   1u   //!< Check tasks' stack in statistic task


//! TASK STACK SIZE
#define OS_TASK_STAT_STK_SIZE    56u   //!< Statistics task stack size (# of CPU_STK wide entries)
#define OS_TASK_IDLE_STK_SIZE    32u   //!< Idle       task stack size (# of CPU_STK wide entries)


//! TASK MANAGEMENT
#define OS_TASK_CHANGE_PRIO_EN    1u   //!< Include code for osTaskChangePrio()
#define OS_TASK_PROFILE_EN        1u   //!< Include variables in OS_TCB for profiling
#define OS_TASK_DEL_EN            1u   //!< Include code for os_task_del()
#define OS_TASK_SW_HOOK_EN        1u   //!< Include code for OSTaskSwHook()


//! EVENT FLAGS
#define OS_FLAG_EN                1u   //!< Enable (1) or Disable (0) code generation for FLAG
#define OS_FLAG_DEL_EN            1u   //!< Include code for osFlagDelete()
#define OS_FLAG_QUERY_EN          1u   //!< Include code for osFlagQuery()


//! MUTUAL EXCLUSION SEMPAPHORES
#define OS_MUTEX_EN               1u   //!< Enable (1) or Disable (0) code generation for MUTEX
#define OS_MUTEX_DEL_EN           1u   //!< Include code for osMutexDelete()
#define OS_MUTEX_QUERY_EN         1u   //!< Include code for osMutexQuery()


//! SEMPAPHORES
#define OS_SEM_EN                 1u   //!< Enable (1) or Disable (0) code generation for SEMPAPHORE
#define OS_SEM_DEL_EN             1u   //!< Include code for osSemDelete()
#define OS_SEM_QUERY_EN           1u   //!< Include code for osSemQuery()
#define OS_SEM_SET_EN             1u   //!< Include code for OSSemSet()
#define OS_SEM_PEND_ABORT_EN      1u   //!< Include code for osSemPendAbort()


//! QUEUE
#define OS_QUEUE_EN               1u   //!< Enable (1) or Disable (0) code generation for QUEUE
#define OS_QUEUE_DEL_EN           1u   //!< Include code for osQueueDelete()
#define OS_QUEUE_QUERY_EN         1u   //!< Include code for osQueueQuery()

//#define SAFETY_CRITICAL_RELEASE

#endif      //!< #ifnde __USER_OS_CFG__

#endif
