/*******************************************************************************
 *  Copyright(C)2017-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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
#ifndef   __OS_H__
#define   __OS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *! OS VERSION NUMBER
 */
#define  OS_VERSION                 901u       //!< Version (Vxx.yyy mult. by 1000)

/*!
 *! INCLUDE HEADER FILES
 */
#include "..\os_cfg.h"
#include "..\ports\ports.h"

/*!
 *! MISCELLANEOUS
 */
#ifdef  __OS_CORE_C__
#   define OS_EXT
#else
#   define OS_EXT  extern
#endif

#define OS_INFINITE                 (~0u)

#if OS_STAT_EN > 0u
#   define OS_N_SYS_TASKS           (2u)        //!< Number of system tasks
#else
#   define OS_N_SYS_TASKS           (1u)
#endif

#define OS_TASK_LOWEST_PRIO         (OS_MAX_PRIO_LEVELS - 1u)
#define OS_TASK_IDLE_PRIO           (OS_MAX_PRIO_LEVELS - 1u)           //!< IDLE task priority
#define OS_TASK_STAT_PRIO           (OS_MAX_PRIO_LEVELS - 1u)           //!< Statistic task priority

#if OS_MAX_PRIO_LEVELS <= 64u
#   define OS_BITMAP_TBL_SIZE       ((OS_MAX_PRIO_LEVELS + 7u) / 8u)    //!< Size of bitmap table
#else
#   define OS_BITMAP_TBL_SIZE       ((OS_MAX_PRIO_LEVELS + 15u) / 16u)  //!< Size of bitmap table
#endif

#define OS_EVENT_EN                 (OS_MUTEX_EN | OS_FLAG_EN |OS_SEM_EN)
    
#define OS_CONTAINER_OF(__ptr, __type, __member) (  \
        (__type*)( (char*)(__ptr) - offsetof(__type, __member) ))

/*!
 *! TASK PEND STATUS (Status codes for OSTCBStatPend)
 */
#define OS_STAT_PEND_OK             (0u)      //!< Not pending, or pending complete
#define OS_STAT_PEND_TO             (1u)      //!< Pending timed out
#define OS_STAT_PEND_ABORT          (2u)      //!< Pending aborted

/*!
 *! OBJECT TYPES
 */
enum {
    OS_OBJ_TYPE_UNUSED  = 0,
    OS_OBJ_TYPE_SEM,
    OS_OBJ_TYPE_MUTEX,
    OS_OBJ_TYPE_FLAG,
    OS_OBJ_TYPE_TCB,
};
#define OS_OBJ_TYPE_MSK             (0x07u << 0)
#define OS_OBJ_TYPE_SET(__OT)       ((UINT8)( ((UINT8)(__OT) << 0) & OS_OBJ_TYPE_MSK ))
#define OS_OBJ_TYPE_GET(__OT)       ((UINT8)( ((UINT8)(__OT) & OS_OBJ_TYPE_MSK) >> 0 ))

#define OS_OBJ_TYPE_WAITABLE_MSK    (1u << 7)

enum {
    OS_OBJ_PRIO_TYPE_LIST       = 0,
    OS_OBJ_PRIO_TYPE_PRIO_LIST,
};
#define OS_OBJ_PRIO_TYPE_MSK        (0x03u << 3)
#define OS_OBJ_PRIO_TYPE_SET(__OT)  ((UINT8)( ((UINT8)(__OT) << 3) & OS_OBJ_TYPE_MSK ))
#define OS_OBJ_PRIO_TYPE_GET(__OT)  ((UINT8)( ((UINT8)(__OT) & OS_OBJ_TYPE_MSK) >> 3 ))

/*!
 *! \Brief  OS???Delete() OPTIONS
 */
#define OS_DEL_NOT_IN_USE           (0u)
#define OS_DEL_ALWAYS               (1u)

/*!
 *! \Brief  OS???Pend() OPTIONS
 *!
 *! These #defines are used to establish the options for OS???PendAbort().
 */
#define OS_PEND_OPT_NONE            (0u)       //!< NO option selected
#define OS_PEND_OPT_BROADCAST       (1u)       //!< Broadcast action to ALL tasks waiting

/*!
 *! \Brief  TASK OPTIONS (see osTaskCreate())
 */
#define OS_TASK_OPT_NONE            (0x00u)    //!< NO option selected
#define OS_TASK_OPT_STK_CHK         (0x01u)    //!< Enable stack checking for the task
#define OS_TASK_OPT_STK_CLR         (0x02u)    //!< Clear the stack when the task is create
#define OS_TASK_OPT_SAVE_FP         (0x04u)    //!< Save the contents of any floating-point registers

enum {
    OS_TASK_STA_STOP = 0u,          //!< task has not been init.(not be created)
    OS_TASK_STA_PEND_WAIT,          //!< task is waiting for a event or sleeping for a DEFINITE time.(in wait list)
    OS_TASK_STA_PEND,               //!< task is waiting for a event or suspended for a INFINITE time.(in pend list)
    OS_TASK_STA_RDY,                //!< task is ready to run.(in the list of ready table)
    OS_TASK_STA_RUN,                //!< task is running.
};

/*!
 *! \Brief  ERROR CODES
 */
enum {
    OS_ERR_NONE                     = 0x00u,

    OS_ERR_OBJ_TYPE                 = 0x01u,
    OS_ERR_PDATA_NULL               = 0x02u,
    OS_ERR_INVALID_HANDLE           = 0x03u,
    OS_ERR_INVALID_OPT              = 0x04u,
    OS_ERR_USE_IN_ISR               = 0x05u,
    OS_ERR_OBJ_DEPLETED             = 0x06u,

    OS_ERR_TIMEOUT                  = 0x30u,
    OS_ERR_PEND_LOCKED              = 0x31u,
    OS_ERR_PEND_ABORT               = 0x32u,
    OS_ERR_TASK_WAITING             = 0x33u,

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
    
/*!
 *! \Brief  ELEMENTARY TYPE
 */
typedef UINT16                  OS_ERR;

#if OS_MAX_PRIO_LEVELS <= 64u
typedef UINT8                   OS_BITMAP_UINT;
#else
typedef UINT16                  OS_BITMAP_UINT;
#endif

typedef void                    OS_TASK(void *);

typedef void                   *OS_HANDLE;

typedef struct os_list_node     OS_LIST_NODE;
typedef struct os_mem_pool      OS_MEM_POOL;
typedef struct os_prio_bitmap   OS_PRIO_BITMAP;
typedef struct os_waitable_obj  OS_WAITABLE_OBJ;
typedef struct os_tcb           OS_TCB;
typedef struct os_flag          OS_FLAG;
typedef struct os_mutex         OS_MUTEX;
typedef struct os_sem           OS_SEM;
typedef struct os_wait_node     OS_WAIT_NODE;

typedef struct {
    UINT16              OSObjType;
} OS_OBJ_HEAD;

//! list type
struct os_list_node {
    OS_LIST_NODE       *Prev;
    OS_LIST_NODE       *Next;
};

//! memory pool.
struct os_mem_pool {
    OS_LIST_NODE        OSMemList;
};

//!< Ready bitmap
struct os_prio_bitmap {
    OS_BITMAP_UINT      Y;
    OS_BITMAP_UINT      X[OS_BITMAP_TBL_SIZE];
};

//!< wait-node object.
struct os_wait_node {
    OS_TCB             *OSWaitNodeTCB;              //!< Pointer to TCB.
    OS_WAITABLE_OBJ    *OSWaitNodeECB;              //!< Pointer to ECB.
    OS_LIST_NODE        OSWaitNodeList;             //!< list of wait node.
    UINT8               OSWaitNodeRes;              //!< Wait resault.
};

/*!
 *! \brief  Waitable object type.
 *! \Note   For casting use, do NOT define any varibles.
 */
struct os_waitable_obj {
    OS_OBJ_HEAD         OSWaitObjHeader;
    UINT16              OSWaitObjCnt;               //!< counter. usage spedified by different objects.
    OS_LIST_NODE        OSWaitObjWaitNodeList;      //!< List of wait-node of task waiting on it.
};

/*!
 *! \Brief  SEMAPHORE CONTROL BLOCK
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)
struct os_sem {
    OS_OBJ_HEAD         OSSemObjHead;
    UINT16              OSSemCnt;                   //!< Semaphore count.
    OS_LIST_NODE        OSSemWaitList;              //!< List of wait-node of task waiting on it.
};
#endif

/*!
 *! \Brief  MUTEX SEMAPHORE CONTROL BLOCK
 */
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
struct os_mutex {
    OS_OBJ_HEAD         OSMutexObjHeader;
    UINT16              OSMutexCnt;                 //!< recursive counter. to allow a task to get the same mutex multiple times.
    OS_LIST_NODE        OSMutexWaitList;            //!< List of wait-node of task waiting on it.

#if OS_MUTEX_OVERLAP_EN > 0u
    OS_LIST_NODE        OSMutexOvlpList;            //!< list of mutex. to allow a task to own multiple mutex.
#endif
    
    OS_TCB             *OSMutexOwnerTCB;            //!< Pointer to mutex owner's TCB
    UINT8               OSMutexOwnerPrio;           //!< Backup of mutex owner's prio, for owner's prio may be changed to a higher one.
    UINT8               OSMutexCeilingPrio;         //!< Mutex's ceiling prio.
};
#endif

/*!
 *! \Brief  EVENT FLAGS CONTROL BLOCK
 */
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
struct os_flag {
    OS_OBJ_HEAD         OSFlagObjHeader;
    UINT16              OSFlagFlags;                //!< Flag options
    OS_LIST_NODE        OSFlagWaitList;             //!< List of wait-node of task waiting on it.
};
#endif

/*!
 *! \Brief  TASK CONTROL BLOCK
 */
struct os_tcb {
    OS_OBJ_HEAD         OSTCBObjHeader;
    
    UINT8               OSTCBOpt;                   //!< Task options as passed by osTaskCreate()
    
    UINT8               OSTCBPrio;                  //!< Task priority (0 == highest)

    UINT32              OSTCBDly;                   //!< time to wait.
    
    //! KEEP 8 bytes offsets from here.
    CPU_STK            *OSTCBStkPtr;                //!< stack point

    OS_WAIT_NODE       *OSTCBWaitNode;

    OS_LIST_NODE        OSTCBList;                  //!< TCB list for scheduler.
    
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
#   if OS_MUTEX_OVERLAP_EN > 0u
    OS_LIST_NODE        OSTCBOwnMutexList;
#   else
    OS_MUTEX           *OSTCBOwnMutex;
#   endif
#endif
    
    UINT16              OSTCBTimeSlice;             //!< for time-slice schedule.
    UINT16              OSTCBTimeSliceCnt;
    
    
#if OS_TASK_PROFILE_EN > 0u
    CPU_STK             *OSTCBStkBase;               //!< Base address of the task stack
    UINT16              OSTCBStkSize;               //!< Size of task stack (in number of stack elements)
    UINT16              OSTCBStkUsed;               //!< Number of BYTES used from the stack
    UINT32              OSTCBCtxSwCtr;              //!< Number of times the task was switched in
    UINT32              OSTCBCyclesTot;             //!< Total number of ticks the task has been running
    UINT32              OSTCBCyclesStart;           //!< Snapshot of tick at start of task
#endif
    
};

/*!
 *! \Brief  FLAG INFO TYPE
 */
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
typedef struct {
    OS_LIST_NODE        OSWaitList;
    BOOL                OSFlagStatus;
    
    BOOL                OSFlagManualReset;
} OS_FLAG_INFO;
#endif

/*!
 *! \Brief  MUTEX INFO TYPE
 */
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
typedef struct {
    OS_LIST_NODE        OSWaitList;
    OS_TCB             *OSOwnerTCB;
    UINT8               OSOwnerPrio;
    
    UINT8               OSCeilingPrio;
} OS_MUTEX_INFO;
#endif

/*!
 *! \Brief  SEMAPHORE INFO TYPE
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)
typedef struct {
    OS_LIST_NODE        OSWaitList;
    UINT16              OSCnt;                      //!< Semaphore count
} OS_SEM_INFO;
#endif

/*!
 *! \Brief  GLOBAL VARIABLES
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)
OS_EXT  OS_LIST_NODE   *osSempFreeList;                         //!< Pointer to list of free semaphore control blocks
OS_EXT  OS_SEM          osSempFreeTbl[OS_MAX_SEMAPHORES];       //!< Table of semaphore control blocks
#endif

#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
OS_EXT  OS_LIST_NODE   *osMutexFreeList;                        //!< Pointer to list of free mutex control blocks
OS_EXT  OS_MUTEX        osMutexFreeTbl[OS_MAX_MUTEXES];         //!< Table of mutex control blocks
#endif

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
OS_EXT  OS_LIST_NODE   *osFlagFreeList;                         //!< Pointer to list of free flag control blocks
OS_EXT  OS_FLAG         osFlagFreeTbl[OS_MAX_FLAGS];            //!< Table of flag control blocks
#endif

OS_EXT  OS_LIST_NODE   *osTCBFreeList;                                  //!< List of free TCBs
OS_EXT  OS_TCB          osTCBFreeTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];    //!< Table of free TCBs
     
OS_EXT  OS_LIST_NODE    osWaitList;                             //!< list of waiting task.
OS_EXT  OS_LIST_NODE    osWaitRunoverList;
OS_EXT  volatile UINT32 osCoreTimerScanHand;
OS_EXT  volatile UINT32 osCoreTimerScanHandOld;


OS_EXT  OS_PRIO_BITMAP  osRdyBitmap;                            //!< Ready bitmap
OS_EXT  OS_LIST_NODE    osRdyList[OS_MAX_PRIO_LEVELS];          //!< Table of pointers to TCB of active task

OS_EXT  OS_TCB         *osTCBCur;                               //!< Pointer to currently running TCB
OS_EXT  OS_TCB         *osTCBNextRdy;                           //!< Pointer to highest priority TCB Ready-to-Run

OS_EXT  UINT16          osIntNesting;                           //!< Interrupt nesting level
OS_EXT  UINT16          osLockNesting;                          //!< Multitasking lock nesting level

OS_EXT  BOOL            osRunning;                              //!< Flag indicating that kernel is running

#if OS_STAT_EN > 0u
OS_EXT  volatile UINT32 osCtxSwCtr;                             //!< Counter of number of context switches
OS_EXT  UINT32          osIdleCtrMax;                           //!< Max. value that idle ctr can take in 1 sec.
OS_EXT  UINT16          osCPUUsage;                             //!< Percentage of CPU used
OS_EXT  BOOL            osTaskStatRunning;                      //!< Flag indicating that the statistic task is running
OS_EXT  CPU_STK         osTaskStatStk[OS_TASK_STAT_STK_SIZE];   //!< Statistics task stack
#endif

OS_EXT  volatile UINT32 osIdleCtr;                              //!< Idle counter
OS_EXT  CPU_STK         osTaskIdleStk[OS_TASK_IDLE_STK_SIZE];   //!< Idle task stack



/*!
 *! APPLICATION FUNCTION PROTOTYPES(APIs)
 */

/*!
 *! \Brief  EVENT FLAGS MANAGEMENT
 */
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)

OS_ERR      osFlagCreate           (OS_HANDLE      *pFlagHandle,
                                    BOOL            initValue,
                                    BOOL            manualReset);

#if OS_FLAG_DEL_EN > 0u
OS_ERR      osFlagDelete           (OS_HANDLE      *pFlagHandle,
                                    UINT8           opt);
#endif

OS_ERR      osFlagPend             (OS_HANDLE       hFlag,
                                    UINT32          timeout);

OS_ERR      osFlagSet              (OS_HANDLE       hFlag);

OS_ERR      osFlagReset            (OS_HANDLE       hFlag);

#if OS_FLAG_QUERY_EN > 0u
OS_ERR      osFlagQuery            (OS_HANDLE       hFlag,
                                    OS_FLAG_INFO   *pInfo);
#endif

#endif

/*!
 *! \Brief  MUTUAL EXCLUSION SEMAPHORE MANAGEMENT
 */
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)

OS_ERR      osMutexCreate          (OS_HANDLE      *pMutexHandle,
                                    UINT8           ceilingPrio);

#if OS_MUTEX_DEL_EN > 0u
OS_ERR      osMutexDelete          (OS_HANDLE      *pMutexHandle,
                                    UINT8           opt);
#endif

OS_ERR      osMutexPend            (OS_HANDLE       hMutex,
                                    UINT32          timeout);

OS_ERR      osMutexPost            (OS_HANDLE       hMutex);

#if OS_MUTEX_QUERY_EN > 0u
OS_ERR      osMutexQuery           (OS_HANDLE       hMutex,
                                    OS_MUTEX_INFO  *pInfo);
#endif

#endif

/*!
 *! \Brief  SEMAPHORE MANAGEMENT
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)

OS_ERR      osSemCreate            (OS_HANDLE      *pSemHandle,
                                    UINT16          cnt);

#if OS_SEM_DEL_EN > 0u
OS_ERR      osSemDelete            (OS_HANDLE      *pSemHandle,
                                    UINT8           opt);
#endif

OS_ERR      osSemPend              (OS_HANDLE       hSemaphore,
                                    UINT32          timeout);

OS_ERR      osSemPost              (OS_HANDLE       hSemaphore,
                                    UINT16          cnt);

#if OS_SEM_PEND_ABORT_EN > 0u
OS_ERR      osSemPendAbort         (OS_HANDLE       hSemaphore);
#endif

#if OS_SEM_SET_EN > 0u
OS_ERR      osSemSet               (OS_HANDLE       hSemaphore,
                                    UINT16          cnt);
#endif

#if OS_SEM_QUERY_EN > 0u
OS_ERR      osSemQuery             (OS_HANDLE       hSemaphore,
                                    OS_SEM_INFO    *pInfo);
#endif

#endif

/*!
 *! \Brief  TASK MANAGEMENT
 */
OS_ERR      osTaskCreate           (OS_HANDLE      *pHandle,
                                    OS_TASK        *task,
                                    void           *parg,
                                    UINT8           prio,
                                    CPU_STK        *pstk,
                                    UINT32          stkSize,
                                    UINT8           opt);

#if OS_TASK_CHANGE_PRIO_EN > 0u
OS_ERR      osTaskChangePrio       (OS_HANDLE       handle,
                                    UINT8           newprio);
#endif

void        osTaskSleep            (UINT32          ticks);

/*!
 *! \Brief  TIME MANAGEMENT
 */
void        osTimeTick             (void);

/*!
 *! \Brief  MISCELLANEOUS
 */
void        osInit                 (void);

void        osIntEnter             (void);
void        osIntExit              (void);

#if OS_SCHED_LOCK_EN > 0u
void        osLockSched            (void);
void        osUnlockSched          (void);
#endif

void        osStart                (void);

#if OS_STAT_EN > 0u
void        osStatInit             (void);
#endif

UINT16      osVersion              (void);

#define     osEnterCriticalSection(__CS)     OSEnterCriticalSection(__CS)
#define     osExitCriticalSection(__CS)      OSExitCriticalSection(__CS)

/*!
 *! TARGET SPECIFIC FUNCTION PROTOTYPES
 *! 
 *! Those functions will be called by os and should be implemented by you.
 */
void        OSStartTheFirst        (void);
void        OSIntCtxSw             (void);
void        OSCtxSw                (void);

CPU_STK     *OSTaskStkInit          (CPU_STK        *ptos,
                                    void            *wrap,
                                    void            *task,
                                    void            *parg);

void        OSInitHookBegin        (void);
void        OSInitHookEnd          (void);

void        OSTaskCreateHook       (OS_TCB          *ptcb);
void        OSTaskReturnHook       (OS_TCB          *ptcb);

#if OS_TASK_SW_HOOK_EN > 0u
void        OSTaskSwHook           (void);
#endif

void        OSTCBInitHook          (OS_TCB          *ptcb);

void        OSTaskIdleHook         (void);

void        OSTaskStatHook         (void);

#if OS_TIME_TICK_HOOK_EN > 0u
void        OSTimeTickHook         (void);
#endif

#if OS_DEBUG_EN > 0u
void        OSDebugInit            (void);
#endif

/*!
 *! INTERNAL FUNCTION PROTOTYPES
 *! (Your application MUST NOT call these functions)
 */

/*!
 *! OS LIST INTERFACE
 */
void        os_list_init_head      (OS_LIST_NODE   *list);
void        os_list_add            (OS_LIST_NODE   *node,
                                    OS_LIST_NODE   *head);
void        os_list_del            (OS_LIST_NODE   *entry);

bool        OS_ObjPoolFree         (OS_LIST_NODE  **ppObj,
                                    void           *pObj);
void       *OS_ObjPoolNew          (OS_LIST_NODE  **ppObj);

/*!
 *! OS SCHEDULE INTERFACE
 */
void        OS_SchedulerInit       (void);
void        OS_SchedulerReadyTask  (OS_TCB         *ptcb);
void        OS_SchedulerUnreadyTask(OS_TCB         *ptcb);
void        OS_SchedulerPrio       (void);
void        OS_SchedulerNext       (void);
void        OS_SchedulerRunPrio    (void);
void        OS_SchedulerRunNext    (void);
void        OS_LockSched           (void);
void        OS_UnlockSched         (void);

#if OS_STAT_TASK_STK_CHK_EN > 0u
void        OS_TaskStkChk          (OS_TCB         *ptcb);
#endif

void        OS_WaitNodeRemove      (OS_TCB         *ptcb);

void        OS_TCBInit             (OS_TCB         *ptcb,
                                    UINT8           prio,
                                    CPU_STK         *psp,
                                    CPU_STK         *pstk,
                                    UINT32          stk_size,
                                    UINT8           opt);

OS_TCB     *OS_WaitableObjRdyTask  (OS_WAITABLE_OBJ    *pobj,
                                    UINT8               pend_stat);
void        OS_WaitableObjAddTask  (OS_WAITABLE_OBJ    *pobj,
                                    OS_WAIT_NODE       *pnode,
                                    UINT32              timeout);

void        OS_ChangeTaskPrio      (OS_TCB         *ptcb,
                                    UINT8           newprio);

void        OS_BitmapSet           (OS_PRIO_BITMAP *pmap,
                                    UINT8           prio);
void        OS_BitmapClr           (OS_PRIO_BITMAP *pmap,
                                    UINT8           prio);
UINT8       OS_BitmapGetHigestPrio (OS_PRIO_BITMAP *pmap);
void        OS_MemClr              (char           *pdest,
                                    UINT32          size);

void        OS_MemCopy             (char           *pdest,
                                    char           *psrc,
                                    UINT32          size);

/*!
 *! LOOK FOR MISSING AND ERROR #define CONSTANTS
 *
 *! This section is used to generate ERROR messages at compile time if certain #define constants are
 *! MISSING or ERROR in OS_CFG.H.  This allows you to quickly determine the source of the error.
 *
 *! You SHOULD NOT change this section UNLESS you would like to add more comments as to the source of the
 *! compile time error.
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
#   ifndef OS_MUTEX_OVERLAP_EN
#       error "OS_CFG.H, Missing OS_MUTEX_OVERLAP_EN: Enable (1) or Disable (0) mutex overlap usage"
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

//! TIME MANAGEMENT
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

#if    OS_ARG_CHK_EN == 0u
#   error "OS_CFG.H, OS_ARG_CHK_EN must be enabled for safety-critical release code"
#endif

#if    OS_DEBUG_EN > 0u
#   error "OS_CFG.H, OS_DEBUG_EN must be disabled for safety-critical release code"
#endif

#if    OS_STAT_EN > 0u
#   error "OS_CFG.H, OS_STAT_EN must be disabled for safety-critical release code"
#endif

#if    OS_TASK_DEL_EN > 0u
#   error "OS_CFG.H, OS_TASK_DEL_EN must be disabled for safety-critical release code"
#endif

#if    OS_FLAG_EN > 0u
#   if OS_FLAG_DEL_EN > 0
#       error "OS_CFG.H, OS_FLAG_DEL_EN must be disabled for safety-critical release code"
#   endif
#endif

#if    OS_MUTEX_EN > 0u
#   if OS_MUTEX_DEL_EN > 0u
#       error "OS_CFG.H, OS_MUTEX_DEL_EN must be disabled for safety-critical release code"
#   endif
#endif

#if    OS_SEM_EN > 0u
#   if OS_SEM_DEL_EN > 0u
#       error "OS_CFG.H, OS_SEM_DEL_EN must be disabled for safety-critical release code"
#   endif
#endif

#if    OS_CRITICAL_METHOD != 3u
#   error "OS_CPU.H, OS_CRITICAL_METHOD must be type 3 for safety-critical release code"
#endif

#endif  //!< SAFETY_CRITICAL_RELEASE

#ifdef __cplusplus
}
#endif

#endif
