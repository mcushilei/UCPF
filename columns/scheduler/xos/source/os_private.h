/*******************************************************************************
 *  Copyright(C)2017-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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
#ifndef __XOS_SOURCE_PRIVATE_H__
#define __XOS_SOURCE_PRIVATE_H__



#include "./os_cfg.h"
#include "../../../list/list.h"
#include "../../../pool/pool.h"
#include "./os.h"

/*
 *  MISCELLANEOUS
 */
#ifdef  __OS_CORE_C__
#   define OS_EXT
#else
#   define OS_EXT  extern
#endif
    
#if OS_CFG_MAX_SCHEDULE_LOCK_NEST_CNT > 0 && OS_CFG_MAX_SCHEDULE_LOCK_NEST_CNT < 255
#   define OS_MAX_SCHEDULE_LOCK_NEST_CNT (OS_CFG_MAX_SCHEDULE_LOCK_NEST_CNT)
#else
#   define OS_MAX_SCHEDULE_LOCK_NEST_CNT (255u)
#endif


#if OS_STAT_EN > 0u
#   define OS_N_SYS_TASKS           (2u)        //!< Number of system tasks
#else
#   define OS_N_SYS_TASKS           (1u)
#endif

#define OS_TASK_IDLE_PRIO           (OS_MAX_PRIO_LEVELS - 1u)           //!< IDLE task priority
#define OS_TASK_STAT_PRIO           (OS_MAX_PRIO_LEVELS - 1u)           //!< Statistic task priority

#define OS_BITMAP_TBL_SIZE          ((OS_MAX_PRIO_LEVELS + 15u) / 16u)  //!< Size of bitmap table

    
/*
 *  TASK PEND STATUS (Status codes for OSTCBStatPend)
 */
#define OS_STAT_PEND_OK             (0u)      //!< Not pending, or pending complete
#define OS_STAT_PEND_TO             (1u)      //!< Pending timed out
#define OS_STAT_PEND_ABORT          (2u)      //!< Pending aborted

#define OS_SYS_TIMER_WHEEL_BIT_WIDTH    (4u)
#define OS_SYS_TIMER_WHEEL_NUM          (32u / OS_SYS_TIMER_WHEEL_BIT_WIDTH)
#define OS_SYS_TIMER_WHEEL_BUCKET_NUM   (1u << OS_SYS_TIMER_WHEEL_BIT_WIDTH)        //!< 2 ^ OS_SYS_TIMER_WHEEL_BIT_WIDTH

/*
 *  OBJECT TYPES
 *  
 *  NOTE: the least bits shall always are 1 to make it very different from pointer's value
 *        in case the objects have been freed.
 */
enum {
    OS_OBJ_TYPE_UNUSED  = 0,
    
    OS_OBJ_TYPE_QUEUE   = 1,
    OS_OBJ_TYPE_SEM     = 2,
    OS_OBJ_TYPE_MUTEX   = 3,
    
    OS_OBJ_TYPE_FLAG    = 5,
    OS_OBJ_TYPE_TCB     = 6,
    
    
};

#define OS_OBJ_TYPE_MSK             (0x07u << 0)
#define OS_OBJ_TYPE_SET(__OT)       ((UINT16)( ((UINT16)(__OT) << 0) & OS_OBJ_TYPE_MSK ))
#define OS_OBJ_TYPE_GET(__OT)       ((UINT16)( ((UINT16)(__OT) & OS_OBJ_TYPE_MSK) >> 0 ))

#define OS_OBJ_TYPE_WAITABLE_MSK    (1u << 7)

enum {
    OS_OBJ_PRIO_TYPE_LIST       = 0,        //! the wait list is FIFO type.
    OS_OBJ_PRIO_TYPE_PRIO_LIST,             //! the wait list is sorted by thread's priority.
};

#define OS_OBJ_PRIO_TYPE_MSK        (0x03u << 3)
#define OS_OBJ_PRIO_TYPE_SET(__OT)  ((UINT16)( ((UINT16)(__OT) << 3) & OS_OBJ_TYPE_MSK ))
#define OS_OBJ_PRIO_TYPE_GET(__OT)  ((UINT16)( ((UINT16)(__OT) & OS_OBJ_TYPE_MSK) >> 3 ))



#define OS_QUEUE_BUFFER_STATIC      (0x0000)
#define OS_QUEUE_BUFFER_ON_HEAP     (0x0001)
#define OS_QUEUE_STOP_ENQUEUE       (0x0004)



typedef struct list_node_t      OS_LIST_NODE;
typedef struct pool_t           OS_MEM_POOL;
typedef struct os_prio_bitmap   OS_PRIO_BITMAP;
typedef struct os_waitable_obj  OS_WAITABLE_OBJ;
typedef struct os_tcb           OS_TCB;
typedef struct os_flag          OS_FLAG;
typedef struct os_mutex         OS_MUTEX;
typedef struct os_sem           OS_SEM;
typedef struct os_queue         OS_QUEUE;
typedef struct os_wait_node     OS_WAIT_NODE;


/*
 *  \brief  all kernel's object shall begin with this header.
 *  \note   This structure shall be always kept aligned to sizeof(void *).
 */
typedef union {
    UINT16              OSObjType;
    void               *OSObjPointer;
} OS_OBJ_HEAD;

//! Ready bitmap
struct os_prio_bitmap {
    UINT16      Y;
    UINT16      X[OS_BITMAP_TBL_SIZE];
};

//! wait-node object.
struct os_wait_node {
    OS_TCB             *OSWaitNodeTCB;              //!< Pointer to TCB.
    OS_WAITABLE_OBJ    *OSWaitNodeECB;              //!< Pointer to ECB.
    OS_LIST_NODE       *OSWaitNodeListHead;         //!< which one of the ECB's wait list this wait-node is in. in case there are multiple wait lists in a waitable object.
    OS_LIST_NODE        OSWaitNodeList;             //!< list-node.
    UINT8               OSWaitNodeRes;              //!< Wait resault.
};

/*
 *  \brief  Waitable object type.
 *  \note   For casting use, do NOT define any varibles.
 */
struct os_waitable_obj {
    OS_OBJ_HEAD         OSWaitObjHeader;
};

/*
 *  \brief  SEMAPHORE CONTROL BLOCK
 */
#if (OS_QUEUE_EN > 0u) && (OS_MAX_QUEUES > 0u)
struct os_queue {
    OS_OBJ_HEAD         OSQueueObjHead;
    
    OS_LIST_NODE        OSQueueEnqueueWaitList;     //!< List of wait-node of task waiting on it.
    OS_LIST_NODE        OSQueueDequeueWaitList;     //!< List of wait-node of task waiting on it.

    void const *       *OSQueueBuffer;
    size_t              OSQueueElementSize;
    UINT16              OSQueueSize;                //!< The amount of items that the queue can contain.
    UINT16              OSQueueHead;
    UINT16              OSQueueTail;
    UINT16              OSQueueLength;              //!< The amount of items in queue.

    UINT16              OSQueueOpt;
    
    UINT16              OSQueueWriteToken;
    UINT16              OSQueueReadToken;
};
#endif

/*
 *  \brief  SEMAPHORE CONTROL BLOCK
 */
#if (OS_SEM_EN > 0u) && (OS_MAX_SEMAPHORES > 0u)
struct os_sem {
    OS_OBJ_HEAD         OSSemObjHead;
    
    OS_LIST_NODE        OSSemWaitList;              //!< List of wait-node of task waiting on it.

    UINT16              OSSemToken;                 //!< Semaphore count.
};
#endif

/*
 *  \brief  MUTEX CONTROL BLOCK
 */
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
struct os_mutex {
    OS_OBJ_HEAD         OSMutexObjHeader;
    
    OS_LIST_NODE        OSMutexWaitList;            //!< List of wait-node of task waiting on this mutex.

    OS_LIST_NODE        OSMutexOvlpList;            //!< list of mutex. to allow a task to own multiple mutex.
    
    OS_TCB             *OSMutexOwnerTCB;            //!< Pointer to mutex owner's TCB

    UINT16              OSMutexCnt;                 //!< recursive counter. to allow a task to get the same mutex multiple times.

    UINT8               OSMutexOwnerPrio;           //!< Backup of mutex owner's prio, for owner's prio may be changed to a higher one.
    UINT8               OSMutexCeilingPrio;         //!< Mutex's ceiling prio.
};
#endif

/*
 *  \brief  EVENT FLAGS CONTROL BLOCK
 */
#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
struct os_flag {
    OS_OBJ_HEAD         OSFlagObjHeader;
    
    OS_LIST_NODE        OSFlagWaitList;             //!< List of wait-node of task waiting on it.

    UINT16              OSFlagFlags;                //!< Flag bits.
};
#endif

/*
 *  \brief  TASK CONTROL BLOCK
 */
struct os_tcb {
    OS_OBJ_HEAD         OSTCBObjHeader;

    //! Caution: always keeping 8 bytes offsets from here!
    CPU_STK            *OSTCBStkPtr;                //!< Stack point
#if (OS_TASK_PROFILE_EN > 0u) || (OS_TASK_STACK_ON_HEAP_EN > 0u)
    CPU_STK            *OSTCBStkBase;               //!< Base address of the task stack
#endif
    
    OS_WAIT_NODE       *OSTCBWaitNode;
    
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
    OS_LIST_NODE        OSTCBOwnMutexList;
#endif

    OS_LIST_NODE        OSTCBList;                  //!< TCB list for scheduler.
    
    UINT32              OSTCBDly;                   //!< Ticks to pend for.
    
    UINT16              OSTCBOpt;                   //!< Task options (as passed by osTaskCreate())
    
    UINT8               OSTCBPrio;                  //!< Task priority (0 -> highest)

    
#if OS_TASK_PROFILE_EN > 0u
    const char         *OSTCBName;
    UINT32              OSTCBCtxSwCtr;              //!< Number of times the task was switched in
    UINT16              OSTCBStkSize;               //!< Size of task stack (in number of stack elements)
    UINT16              OSTCBStkUsed;               //!< Number of BYTES used from the stack
#endif
    
};


/*
 *  \brief  GLOBAL VARIABLES
 */
#if OS_QUEUE_EN > 0u
OS_EXT  OS_MEM_POOL     osQueueFreePool;                        //!< Pointer to list of free semaphore control blocks
OS_EXT  OS_QUEUE        osQueueFreeTbl[OS_MAX_QUEUES];          //!< Table of semaphore control blocks
#endif

#if OS_SEM_EN > 0u
OS_EXT  OS_MEM_POOL     osSemFreePool;                          //!< Pointer to list of free semaphore control blocks
OS_EXT  OS_SEM          osSemFreeTbl[OS_MAX_SEMAPHORES];        //!< Table of semaphore control blocks
#endif

#if OS_MUTEX_EN > 0u
OS_EXT  OS_MEM_POOL     osMutexFreePool;                        //!< Pointer to list of free mutex control blocks
OS_EXT  OS_MUTEX        osMutexFreeTbl[OS_MAX_MUTEXES];         //!< Table of mutex control blocks
#endif

#if OS_FLAG_EN > 0u
OS_EXT  OS_MEM_POOL     osFlagFreePool;                         //!< Pointer to list of free flag control blocks
OS_EXT  OS_FLAG         osFlagFreeTbl[OS_MAX_FLAGS];            //!< Table of flag control blocks
#endif

OS_EXT  OS_MEM_POOL     osTCBFreePool;                                  //!< List of free TCBs
OS_EXT  OS_TCB          osTCBFreeTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];    //!< Table of free TCBs


//! TODO: adjust the value when it wakes up from sleep.
OS_EXT  OS_LIST_NODE    osSysTimerWheel[OS_SYS_TIMER_WHEEL_NUM][OS_SYS_TIMER_WHEEL_BUCKET_NUM];
OS_EXT  volatile UINT32 osSysClockCounter;
    
OS_EXT  OS_PRIO_BITMAP  osRdyBitmap;                            //!< bitmap that indicates which priority has threads that are ready to run.
OS_EXT  OS_LIST_NODE    osRdyList[OS_MAX_PRIO_LEVELS];          //!< Table of pointers to TCB of active task

OS_EXT  OS_TCB         *osTCBCur;                               //!< Pointer to currently running thread's TCB
OS_EXT  OS_TCB         *osTCBNextRdy;                           //!< Pointer to the TCB of thread that the scheduler want to run when the context switch hanpens.

OS_EXT  UINT16          osIntNesting;                           //!< Interrupt nesting level
OS_EXT  UINT16          osLockNesting;                          //!< schedule-lock nesting level

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


/*
 *  INTERNAL FUNCTION PROTOTYPES
 *  (Your application MUST NOT call these functions)
 */

/*
 *  OS SCHEDULE INTERFACE
 */
void        OS_SchedulerInit       (void);
void        OS_SchedulerReadyTask  (OS_TCB         *ptcb);
void        OS_SchedulerUnreadyTask(OS_TCB         *ptcb);
void        OS_SchedulerPrio       (void);
void        OS_SchedulerNext       (void);
void        OS_SchedulerRunPrio    (void);
void        OS_SchedulerRunNext    (void);

#if OS_STAT_TASK_STK_CHK_EN > 0u
void        OS_TaskStkChk          (OS_TCB         *ptcb);
#endif

void        OS_WaitNodeRemove      (OS_TCB         *ptcb);

void        OS_TCBInit             (OS_TCB         *ptcb,
                                    const char     *name,
                                    UINT8           prio,
                                    CPU_STK        *psp,
                                    CPU_STK        *pstk,
                                    UINT32          stk_size,
                                    UINT16          opt);

OS_TCB     *OS_WaitableObjRdyTask  (OS_WAITABLE_OBJ    *pobj,
                                    OS_LIST_NODE       *plist,
                                    UINT8               pend_stat);

void        OS_WaitableObjAddTask  (OS_WAITABLE_OBJ    *pobj,
                                    OS_WAIT_NODE       *pnode,
                                    OS_LIST_NODE       *plist,
                                    UINT32              timeout);

void        OS_TaskStop            (void);

void        OS_ChangeTaskPrio      (OS_TCB         *ptcb,
                                    UINT8           newprio);

void        OS_BitmapSet           (OS_PRIO_BITMAP *pmap,
                                    UINT8           prio);

void        OS_BitmapClr           (OS_PRIO_BITMAP *pmap,
                                    UINT8           prio);

UINT8       OS_BitmapGetLeadingZero(OS_PRIO_BITMAP *pmap);


#endif
