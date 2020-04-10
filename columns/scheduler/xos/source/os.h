/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#ifndef __XOS_SOURCE_OS_H__
#define __XOS_SOURCE_OS_H__


/*!
 *! OS VERSION NUMBER
 */
#define  OS_VERSION                 901u       //!< Version (Vxx.yyy mult. by 1000)

/*!
 *! INCLUDE HEADER FILES
 */
#include "./os_cfg.h"
#include "../ports/ports.h"
#if defined(__DEBUG__)
#include "./os_private.h"
#endif


#define OS_INFINITE                 (-1)
#define OS_TASK_LOWEST_PRIO         (OS_MAX_PRIO_LEVELS - 1u)


/*!
 *! \Brief  OS???Delete() OPTIONS
 */
#define OS_DEL_NOT_IN_USE           (0u)
#define OS_DEL_ALWAYS               (1u)


/*!
 *! \Brief  TASK OPTIONS (see osTaskCreate())
 */
#define OS_TASK_OPT_NONE            (0x0000u)    //!< NO option selected
#define OS_TASK_OPT_STK_CHK         (0x0001u)    //!< Enable stack checking for the task
#define OS_TASK_OPT_STK_CLR         (0x0002u)    //!< Clear the stack when the task is create
#define OS_TASK_OPT_STK_STATIC      (0x0000u)    //!< the stack is allocced in static memory.
#define OS_TASK_OPT_STK_HEAP        (0x0004u)    //!< the stack is allocced in heap memory.
#define OS_TASK_OPT_SAVE_FP         (0x0010u)    //!< Save the contents of any floating-point registers
#define OS_TASK_OPT_USED_BY_OS      (0xF800u)    //!< these bits are used by the os itself. any bit set by the user will have no effect.


/*!
 *! \Brief  ERROR CODES
 */
enum {
    OS_ERR_NONE                     = 0x00u,

    OS_ERR_OBJ_TYPE                 = 0x01u,
    OS_ERR_PDATA_NULL               = 0x02u,
    OS_ERR_NULL_POINTER             = 0x02u,
    OS_ERR_INVALID_HANDLE           = 0x03u,
    OS_ERR_INVALID_OPT              = 0x04u,
    OS_ERR_USE_IN_ISR               = 0x05u,
    OS_ERR_OBJ_DEPLETED             = 0x06u,
    OS_ERR_OUT_OF_MEMORY            = 0x06u,
    OS_ERR_INVALID_SLEEP_TIME,

    OS_ERR_TIMEOUT                  = 0x30u,
    OS_ERR_PEND_LOCKED              = 0x31u,
    OS_ERR_PEND_ABORT               = 0x32u,
    OS_ERR_DELETE_IN_USE            = 0x33u,

    OS_ERR_INVALID_PRIO             = 0x60u,
    OS_ERR_TASK_OPT                 = 0x61u,
    OS_ERR_TASK_EXIST               = 0x62u,
    OS_ERR_TASK_NOT_EXIST           = 0x63u,

    OS_ERR_SEM_OVF                  = 0x90u,
    OS_ERR_NOT_MUTEX_OWNER          = 0x91u,
    OS_ERR_MUTEX_IS_OWNED           = 0x92u,
    OS_ERR_OVERLAP_MUTEX            = 0x93u,
    OS_ERR_MUTEX_OVERFLOW           = 0x94u,
};
    

typedef CPU_STK         OS_CPU_STK;
typedef CPU_SR          OS_CPU_SR;
/*!
 *! \Note   1) a task will be deleted when it returns if deleting task is enabled. This is the only way
 *!            to terminate a task. There is no API like TaskDelete() in this OS.
 *!         2) the return value will be given to OSTaskReturnHook(). OS will not handle this value.
 */
typedef void           *OS_TASK(void *);
typedef void           *OS_HANDLE;
typedef UINT16          OS_ERR;



/*!
 *! \Brief  FLAG INFO TYPE
 */
#if OS_FLAG_EN > 0u
typedef struct {
    BOOL                OSFlagStatus;
    BOOL                OSFlagManualReset;
} OS_FLAG_INFO;
#endif

/*!
 *! \Brief  MUTEX INFO TYPE
 */
#if OS_MUTEX_EN > 0u
typedef struct {
    UINT8               OSOwnerPrio;
    UINT8               OSCeilingPrio;
} OS_MUTEX_INFO;
#endif

/*!
 *! \Brief  SEMAPHORE INFO TYPE
 */
#if OS_SEM_EN > 0u
typedef struct {
    UINT16              OSCnt;                      //!< Semaphore count
} OS_SEM_INFO;
#endif

#if OS_QUEUE_EN > 0u
/*!
 *! \Brief  QUEUE INFO TYPE
 */
typedef struct {
    UINT16              OSQueueInfoLength;          //!< Queue length.
    UINT16              OSQueueInfoHead;
    UINT16              OSQueueInfoTail;
    UINT16              OSQueueInfoSize;
} OS_QUEUE_INFO;
#endif


/*!
 *! \Brief  EVENT FLAGS MANAGEMENT
 */
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)

extern OS_ERR   osFlagCreate       (OS_HANDLE      *pFlagHandle,
                                    BOOL            initValue,
                                    BOOL            manualReset);

#if OS_FLAG_DEL_EN > 0u
extern OS_ERR   osFlagDelete       (OS_HANDLE       hFlag,
                                    UINT16          opt);
#endif

extern OS_ERR   osFlagPend         (OS_HANDLE       hFlag,
                                    UINT32          timeout);

extern OS_ERR   osFlagSet          (OS_HANDLE       hFlag);

extern OS_ERR   osFlagReset        (OS_HANDLE       hFlag);

#if OS_FLAG_QUERY_EN > 0u
extern OS_ERR   osFlagQuery        (OS_HANDLE       hFlag,
                                    OS_FLAG_INFO   *pInfo);
#endif  //!< #if OS_FLAG_QUERY_EN > 0u
#endif  //!< #if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)

/*!
 *! \Brief  MUTUAL EXCLUSION SEMAPHORE MANAGEMENT
 */
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)

extern OS_ERR   osMutexCreate      (OS_HANDLE      *pMutexHandle,
                                    UINT8           ceilingPrio);

#if OS_MUTEX_DEL_EN > 0u
extern OS_ERR   osMutexDelete      (OS_HANDLE       hMutex,
                                    UINT16          opt);
#endif

extern OS_ERR   osMutexPend        (OS_HANDLE       hMutex,
                                    UINT32          timeout);

extern OS_ERR   osMutexPost        (OS_HANDLE       hMutex);

#if OS_MUTEX_QUERY_EN > 0u
extern OS_ERR   osMutexQuery       (OS_HANDLE       hMutex,
                                    OS_MUTEX_INFO  *pInfo);
#endif  //!< #if OS_MUTEX_QUERY_EN > 0u
#endif  //!< #if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)

/*!
 *! \Brief  SEMAPHORE MANAGEMENT
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)

extern OS_ERR   osSemCreate        (OS_HANDLE      *pSemHandle,
                                    UINT16          cnt);

#if OS_SEM_DEL_EN > 0u
extern OS_ERR   osSemDelete        (OS_HANDLE       hSemaphore,
                                    UINT16          opt);
#endif

extern OS_ERR   osSemPend          (OS_HANDLE       hSemaphore,
                                    UINT32          timeout);

extern OS_ERR   osSemPost          (OS_HANDLE       hSemaphore,
                                    UINT16          cnt);

#if OS_SEM_PEND_ABORT_EN > 0u
extern OS_ERR   osSemPendAbort     (OS_HANDLE       hSemaphore);
#endif

#if OS_SEM_SET_EN > 0u
extern OS_ERR   osSemSet           (OS_HANDLE       hSemaphore,
                                    UINT16          cnt);
#endif

#if OS_SEM_QUERY_EN > 0u
extern OS_ERR   osSemQuery         (OS_HANDLE       hSemaphore,
                                    OS_SEM_INFO    *pInfo);
#endif  //!< #if OS_SEM_QUERY_EN > 0u
#endif  //!< #if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)


/*!
 *! \Brief  QUEUE MANAGEMENT
 */
#if (OS_QUEUE_EN > 0u) && (OS_MAX_QUEUES > 0u)
extern OS_ERR   osQueueCreate      (OS_HANDLE     *hQueue,
                                    void          *buffer,
                                    UINT16         queueSize,
                                    size_t         elementSize);

#if OS_QUEUE_DEL_EN > 0
extern OS_ERR   osQueueStopEnqueue (OS_HANDLE      hQueue);

extern OS_ERR   osQueueDelete      (OS_HANDLE      hQueue,
                                    UINT16         opt);
#endif

extern OS_ERR   osQueueWrite       (OS_HANDLE      hQueue,
                                    const void    *buffer,
                                    UINT32         timeout);

extern OS_ERR   osQueueRead        (OS_HANDLE      hQueue,
                                    void          *buffer,
                                    UINT32         timeout);

#if OS_QUEUE_QUERY_EN > 0u
extern OS_ERR   osQueueQuery       (OS_HANDLE      hQueue,
                                    OS_QUEUE_INFO *pInfo);
#endif  //!< #if OS_QUEUE_QUERY_EN > 0u
#endif  //!< #if (OS_QUEUE_EN > 0u) && (OS_MAX_QUEUES > 0u)

/*!
 *! \Brief  TASK MANAGEMENT
 */
extern OS_ERR   osTaskCreate       (OS_HANDLE      *pHandle,
                                    const char     *name,
                                    OS_TASK        *Entry,
                                    void           *argument,
                                    CPU_STK        *stack,
                                    UINT32          stackSize,
                                    UINT16          options,
                                    UINT8           priority);

#if OS_TASK_CHANGE_PRIO_EN > 0u
extern OS_ERR   osTaskChangePrio   (OS_HANDLE       handle,
                                    UINT8           newprio);
#endif

extern OS_ERR   osTaskSleep        (UINT32          ticks);

/*!
 *! \Brief  TIME MANAGEMENT
 */
extern void     osSysTick               (void);

/*!
 *! \Brief  MISCELLANEOUS
 */
extern void     osInit                  (void);
#if OS_STAT_EN > 0u
extern void     osStatInit              (void);
#endif
extern void     osStart                 (void);

extern void     osIntEnter              (void);
extern void     osIntExit               (void);

#if OS_SCHED_LOCK_EN > 0u
extern void     osLockSched             (void);
extern void     osUnlockSched           (void);
#endif

/*
 *  \brief      GET VERSION
 * 
 *  \param      none
 * 
 *  \return     The version number of OS multiplied by 10000.
 */
extern UINT16   osVersion               (void);

/*
 *  \brief      get the count of system clock.
 * 
 *  \param      none
 * 
 *  \return     The count of current system clock.
 *
 *  \note       count the system clock tick from zero since the system boots.
 */
extern UINT32   osGetSysTickCount       (void);

#define OS_MS_PER_TICK                  (1000u / OS_TICKS_PER_SEC)
#define OS_MS2TICK(__ms)                ((__ms) / (OS_MS_PER_TICK))

extern void     OSEnterCriticalSection  (void);
extern void     OSExitCriticalSection   (void);
extern CPU_REG  OSDisableInterrupt      (void);
extern void     OSResumeInterrupt       (CPU_REG level);

/*!
 *! \Brief  Enable and disable interrupt
 */
#define     osEnterCriticalSection()    OSEnterCriticalSection()
#define     osExitCriticalSection()     OSExitCriticalSection()

#define     osDisableInterrupt()        OSDisableInterrupt()
#define     osResumeInterrupt()         OSResumeInterrupt()



#endif  ////!< #ifndef __XOS_SOURCE_OS_H__
