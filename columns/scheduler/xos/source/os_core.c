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
#define __OS_CORE_C__

/*============================ INCLUDES ======================================*/
#include ".\os_private.h"
#include ".\os_port.h"
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#ifndef OS_COUNT_LEADING_ZERO
#define OS_COUNT_LEADING_ZERO(__B)      (osLZTbl[__B])
#endif

#define OS_SYS_TIMER_WHEEL_BIT_MASK        \
    ((1u << OS_SYS_TIMER_WHEEL_BIT_WIDTH) - 1u)
        
#define OS_SYS_TIMER_WHEEL_COUNTER_VALUE(c, n) \
    (( (c) >> ((n) * OS_SYS_TIMER_WHEEL_BIT_WIDTH) ) & OS_SYS_TIMER_WHEEL_BIT_MASK)

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static  void    os_init_obj_pool(void);

static  void    os_init_misc(void);

static  int     os_sys_timer_insert(OS_TCB *ptcb);

static  void    OS_PendTask(OS_TCB *ptcb);
static  void    OS_UnpendTask(OS_TCB *ptcb);

#if OS_STAT_EN > 0u
static  void    os_init_statistics_task(void);
static  void   *os_task_statistics(void *parg);
#endif

static  void    os_init_idle_task(void);
static  void   *os_task_idle(void *parg);

/*============================ LOCAL VARIABLES ===============================*/
/*
 *  \brief       LEADING ZERO LOOKUP TABLE
 * 
 *  \note        Index into table is bit pattern to resolve highest priority
 *               Indexed value corresponds to highest priority bit position (i.e. 0..7)
 *               Leading 0 algorithm.
 */
static const UINT8 osLZTbl[256] = {
    0u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x00 to 0x0F
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x10 to 0x1F
    5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x20 to 0x2F
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x30 to 0x3F
    6u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x40 to 0x4F
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x50 to 0x5F
    5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x60 to 0x6F
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x70 to 0x7F
    7u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x80 to 0x8F
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0x90 to 0x9F
    5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0xA0 to 0xAF
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0xB0 to 0xBF
    6u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0xC0 to 0xCF
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0xD0 to 0xDF
    5u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, //!< 0xE0 to 0xEF
    4u, 0u, 1u, 0u, 2u, 0u, 1u, 0u, 3u, 0u, 1u, 0u, 2u, 0u, 1u, 0u  //!< 0xF0 to 0xFF
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*
 *  \brief      INITIALIZATION
 * 
 *  \remark     This function is used to initialize the internals of OS and MUST be called prior to
 *              creating any OS object and, prior to calling osStart().
 * 
 *  \param      none
 * 
 *  \return     none
 */
void osInit(void)
{
#if OS_HOOKS_EN > 0
    OSInitHookBegin();                                  //!< Call port specific initialization code
#endif
    
    os_init_misc();                                     //!< Initialize miscellaneous variables
    
#if OS_HEAP_MEM_EN > 0
    OSHeapInit();
#endif

    os_init_obj_pool();                                 //!< Initialize the free list of OS_TCBs

    OS_SchedulerInit();                                 //!< Initialize the Ready List

    os_init_idle_task();                                //!< Create the Idle Task
#if OS_STAT_EN > 0u
    os_init_statistics_task();                          //!< Create the Statistic Task
#endif

#if OS_HOOKS_EN > 0u
    OSInitHookEnd();                                    //!< Call port specific init. code
#endif

#if OS_DEBUG_EN > 0u
    OSDebugInit();
#endif
}

/*
 *  \brief      INITIALIZE MISCELLANEOUS VARIABLES
 * 
 *  \remark     This function is called by osInit() to initialize miscellaneous variables.
 * 
 *  \param      none
 * 
 *  \return     none
 */
static void os_init_misc(void)
{
    osRunning       = FALSE;            //!< Indicate that multitasking not started

    osTCBCur        = NULL;
    osTCBNextRdy    = NULL;

    osIntNesting            = 0u;                       //!< Clear the interrupt nesting counter
    osLockNesting           = 0u;                       //!< Clear the scheduling lock counter

    osIdleCtr               = 0u;                       //!< Clear the idle counter

#if OS_STAT_EN > 0u
    osCtxSwCtr              = 0u;                       //!< Clear the context switch counter
    osCPUUsage              = 0u;
    osIdleCtrMax            = 0u;
    osTaskStatRunning       = FALSE;                    //!< Statistic task is not ready
#endif
    
	for (int j = 0u; j < OS_SYS_TIMER_WHEEL_NUM; j++) {
		for (int i = 0u; i < OS_SYS_TIMER_WHEEL_BUCKET_NUM; i++) {
			list_init(&osSysTimerWheel[j][i]);
		}
	}
    osSysClockCounter = 0u;
}

static void os_init_obj_pool(void)
{
    memset((char *)osTCBFreeTbl, 0, sizeof(osTCBFreeTbl));
    pool_init(&osTCBFreePool, UBOUND(osTCBFreeTbl), osTCBFreeTbl, sizeof(OS_TCB));

#if (OS_QUEUE_EN)
    memset((char *)osQueueFreeTbl, 0, sizeof(osQueueFreeTbl));
    pool_init(&osQueueFreePool, UBOUND(osQueueFreeTbl), osQueueFreeTbl, sizeof(OS_QUEUE));
#endif

#if (OS_SEM_EN)
    memset((char *)osSemFreeTbl, 0, sizeof(osSemFreeTbl));
    pool_init(&osSemFreePool, UBOUND(osSemFreeTbl), osSemFreeTbl, sizeof(OS_SEM));
#endif

#if (OS_MUTEX_EN)
    memset((char *)osMutexFreeTbl, 0, sizeof(osMutexFreeTbl));
    pool_init(&osMutexFreePool, UBOUND(osMutexFreeTbl), osMutexFreeTbl, sizeof(OS_MUTEX));
#endif

#if (OS_FLAG_EN)
    memset((char *)osFlagFreeTbl, 0, sizeof(osFlagFreeTbl));
    pool_init(&osFlagFreePool, UBOUND(osFlagFreeTbl), osFlagFreeTbl, sizeof(OS_FLAG));
#endif
}

/*
 *  \brief      ENTER ISR
 * 
 *  \remark     This function is used to notify OS that you are about to service an interrupt
 *              service routine (ISR).  This allows OS to keep track of interrupt nesting and thus
 *              only perform rescheduling at the last nested ISR.
 * 
 *  \param      none
 * 
 *  \return     none
 * 
 *  \note       1) You MUST invoke osIntEnter() and osIntExit() in pair.  In other words, for every call
 *                 to osIntEnter() at the beginning of the ISR you MUST have a call to osIntExit() at the
 *                 end of the ISR.
 *              2) You are allowed to nest interrupts up to 255 levels deep.
 */
void osIntEnter(void)
{
    if (osRunning == FALSE) {
        return;
    }
    
    OSEnterCriticalSection();
    if (osIntNesting < 255u) {
        osIntNesting++;                                 //!< Increment ISR nesting level
    }
    OSExitCriticalSection();
}

/*
 *  \brief      EXIT ISR
 * 
 *  \remark     This function is used to notify os that you have completed servicing an ISR.  When
 *              the last nested ISR has completed, OS will call the scheduler to determine whether
 *              a new, high-priority task, is ready to run.
 * 
 *  \param      none
 * 
 *  \return     none
 * 
 *  \note       1) You MUST invoke osIntEnter() and osIntExit() in pair.  In other words, for every call
 *                 to osIntEnter() at the beginning of the ISR you MUST have a call to osIntExit() at the
 *                 end of the ISR.
 *              2) Rescheduling is prevented when the scheduler is locked.
 */
void osIntExit(void)
{
    if (osRunning == FALSE) {
        return;
    }
    
    OSEnterCriticalSection();
    do {
        if (osIntNesting > 0u) {
            osIntNesting--;
        }
        if (osIntNesting != 0u) {           //!< Reschedule only if all ISRs complete ...
            break;
        }
        if (osLockNesting != 0u) {          //!< ... and scheduler is not locked.
            break;
        }
        OS_SchedulerPrio();
        if (osTCBNextRdy != osTCBCur) {
            OSExitCriticalSection();
            OSIntCtxSw();                   //!< Perform interrupt level ctx switch
            return;
        }
    } while (0);
    OSExitCriticalSection();
}

#if OS_SCHED_LOCK_EN > 0u
/*
 *  \brief      Lock scheduler
 * 
 *  \remark     To stop scheduling to mutually run a critical section.
 *              This is intended to be used where the critical section is too short for a condition in which mutex is used.
 *              If the critical section is even shorter, disabling the interrupt is preferred.
 * 
 *  \param      none
 * 
 *  \return     none
 * 
 *  \note       1) This function shell not have an effect in a ISR.
 */
void osLockSched(void)
{
    if (osRunning == FALSE) {       //!< Make sure multitasking is running
        return;
    }
    
    if (osIntNesting == 0u) {       //!< This function will not work in an ISR.
        return;
    }
    
    OSEnterCriticalSection();
    if (osLockNesting < OS_MAX_SCHEDULE_LOCK_NEST_CNT) {    //!< Prevent osLockNesting from wrapping back to 0
        osLockNesting++;
    } else {
        //! TODO handle error, this is unreasonable to happen in a application so there must be a bug or incorrect usage in user's application code.
    }
    OSExitCriticalSection();
}

/*
 *  \brief      Unlock scheduler
 * 
 *  \remark     To stop scheduling to mutually run a critical section between threads.
 *              This is intended to be used where the critical section is too short for a condition in which mutex is used.
 *              If the critical section is even shorter, disabling the interrupt is preferred.
 * 
 *  \param      none
 * 
 *  \return     none
 * 
 *  \note       1) This function shell not have an effect in a ISR.
 */
void osUnlockSched(void)
{
    if (osRunning == FALSE) {       //!< Make sure multitasking is running
        return;
    }
    
    if (osIntNesting == 0u) {       //!< This function will not work in an ISR.
        return;
    }
    
    OSEnterCriticalSection();
    if (osLockNesting > 0u) {           //!< Do not decrement if already 0
        osLockNesting--;                //!< Decrement lock nesting level
    }
    OSExitCriticalSection();
    
    OS_SchedulerRunPrio();
}
#endif  //!< #if OS_SCHED_LOCK_EN > 0u

/*
 *  \brief      START MULTITASKING
 * 
 *  \remark     This function is used to start the multitasking process which lets OS manages the
 *              task that you have created.  Before you can call osStart(), you MUST have called osInit()
 *              and you MUST have created at least one task.
 * 
 *  \param      none
 * 
 *  \return     none
 * 
 *  \note       OSStartTheFirstThread() MUST:
 *              a) Call OSTaskSwHook() then,
 *              b) Set osRunning to TRUE.
 *              c) Load the context of the task pointed to by osTCBNextRdy.
 *              d) Execute the task.
 *              e) Enable system-level interrupt.
 */
void osStart(void)
{
    if (osRunning == FALSE) {           //!< os must NOT be running!
        osTCBCur = NULL;
        OS_SchedulerNext();
        OSStartTheFirstThread();
    }
}

/*
 *  \brief      Add a task to an event's wait list.
 * 
 *  \param      pobj     a pointer to the object that this wati-node wait for.
 *              plist    a pointer to the list this node will be inserted to.
 *              pnode    a pointer to the wait-node.
 * 
 *  \return     none
 */
static void OS_WaitNodeInsert(OS_WAITABLE_OBJ *pobj, OS_LIST_NODE *plist, OS_WAIT_NODE *pnode)
{
    OS_LIST_NODE *addTo;

    
    if (OS_OBJ_PRIO_TYPE_GET(pobj->OSWaitObjHeader.OSObjType) == OS_OBJ_PRIO_TYPE_PRIO_LIST) {
        //! find the node whose priority is lower than current's.
        for (addTo = plist->Next; addTo != plist; addTo = addTo->Next) {
            OS_WAIT_NODE *node = CONTAINER_OF(addTo, OS_WAIT_NODE, OSWaitNodeList);
            if (pnode->OSWaitNodeTCB->OSTCBPrio < node->OSWaitNodeTCB->OSTCBPrio) {
                break;
            }
        }
    } else {
        addTo = plist;
    }
    list_insert(&pnode->OSWaitNodeList, addTo->Prev);    //!< add wait node to the end of wait NODE list.
}

/*
 *  \brief      Remove a task from an event's wait list.
 * 
 *  \param      ptcb     a pointer to the task's TCB.
 * 
 *  \return     none
 */
void OS_WaitNodeRemove(OS_TCB *ptcb)
{
    OS_WAIT_NODE *pnode = ptcb->OSTCBWaitNode;
    

    list_remove(&pnode->OSWaitNodeList);    //!< remove from wait NODE list.
    ptcb->OSTCBWaitNode = NULL;
}

/*
 *  \brief      PEND TASK
 * 
 *  \param      ptcb     is a pointer to the task to remove.
 * 
 *  \return     none
 * 
 *  \note       1) interrupts shell be DISABLED.
 */
static void OS_PendTask(OS_TCB *ptcb)
{
    if (ptcb->OSTCBDly == OS_INFINITE) {
        return;
    }

    ptcb->OSTCBDly += osSysClockCounter;
    os_sys_timer_insert(ptcb);
}

/*
 *  \brief      UNPEND TASK
 * 
 *  \param      ptcb     is a pointer to the task to remove.
 * 
 *  \return     none
 * 
 *  \note       1) interrupts shell be DISABLED.
 */
static void OS_UnpendTask(OS_TCB *ptcb)
{
    list_remove(&ptcb->OSTCBList);      //!< remove from wait list.
    ptcb->OSTCBDly = 0u;
}

/*
 *  \brief      MAKE TASK WAIT FOR EVENT TO OCCUR
 * 
 *  \remark     This function is called by other OS services to suspend a task because an event has
 *              not occurred.
 * 
 *  \param      pevent   is a pointer to the event control block for which the task will be waiting for.
 * 
 *              pnode    is a pointer to a structure which contains data about the task waiting for
 *                       event to occur.
 *
 *              ticks    is the desired amount of ticks that the task will wait for the event to
 *                       occur.
 * 
 *  \return     none
 * 
 *  \note       1) This function assumes that interrupts are DISABLED.
 *              2) This function is INTERNAL to OS and your application should not call it.
 */
void OS_WaitableObjAddTask( OS_WAITABLE_OBJ    *pobj,
                            OS_WAIT_NODE       *pnode,
                            OS_LIST_NODE       *plist,
                            UINT32              ticks)
{
    //! initial wait node.
    pnode->OSWaitNodeTCB        = osTCBCur;
    pnode->OSWaitNodeECB        = pobj;
    pnode->OSWaitNodeListHead   = plist;
    pnode->OSWaitNodeRes        = OS_STAT_PEND_OK;
    list_init(&pnode->OSWaitNodeList);
    
    osTCBCur->OSTCBWaitNode = pnode;                    //!< Store node in task's TCB
    osTCBCur->OSTCBDly      = ticks;
    
    OS_WaitNodeInsert(pobj, plist, pnode);
    OS_SchedulerUnreadyTask(osTCBCur);
    OS_PendTask(osTCBCur);
}

/*
 *  \brief      MAKE TASK READY TO RUN BASED ON EVENT OCCURING
 * 
 *  \remark     This function is called by other OS services and is used to make a task ready-to-run because
 *              desired event occur. Only the thread in the head of the wait-node list will be set to ready.
 * 
 *  \param      pevent     is a pointer to the event control block corresponding to the event.
 * 
 *              plist      a pointer to the wait-node list of the event object.
 * 
 *              pendRes    is used to indicate the readied task's pending status:
 * 
 *                           OS_STAT_PEND_OK      Task ready due to a event-set, not a timeout or
 *                                                an abort.
 *                           OS_STAT_PEND_ABORT   Task ready due to an abort(or event was deleted).
 * 
 *  \return     none
 * 
 *  \note       1) This function assumes that interrupts are DISABLED.
 *              2) The list plist points to should not be empty!.
 */
OS_TCB *OS_WaitableObjRdyTask(OS_WAITABLE_OBJ *pobj, OS_LIST_NODE *plist, UINT8 pendRes)
{
    OS_WAIT_NODE   *pnode;
    OS_TCB         *ptcb;

    
    pnode   = CONTAINER_OF(plist->Next, OS_WAIT_NODE, OSWaitNodeList);
    ptcb    = pnode->OSWaitNodeTCB;
        
    pnode->OSWaitNodeRes        = pendRes;
    pnode->OSWaitNodeListHead   = NULL;
    pnode->OSWaitNodeECB        = NULL;
    pnode->OSWaitNodeTCB        = NULL;
    
    OS_WaitNodeRemove(ptcb);            //!< Remove this task from event's wait-node-list
    OS_UnpendTask(ptcb);                //!< Remove this task from wait-list.
    OS_SchedulerReadyTask(ptcb);        //!< Put task in the ready-list
    return ptcb;
}

static int os_sys_timer_insert(OS_TCB *ptcb)
{
    UINT32 wheel;
    UINT32 wheelCounter;
    UINT32 a;
    UINT32 b;
    
    if (osSysClockCounter == ptcb->OSTCBDly) {
        return -1;
    }
    
    //! to find which wheel the timer will be inserted.
    a = osSysClockCounter ^ ptcb->OSTCBDly;
    U32_COUNT_LEADING_ZEROS(a, b);
	b = 32u - 1u - b;
	U32_COUNT_TRAILING_ZEROS(OS_SYS_TIMER_WHEEL_BIT_WIDTH, a);
	wheel = b >> a;     //!< wheel = b / OS_SYS_TIMER_WHEEL_BIT_WIDTH

    wheelCounter = OS_SYS_TIMER_WHEEL_COUNTER_VALUE(ptcb->OSTCBDly, wheel);
    list_insert(&ptcb->OSTCBList, osSysTimerWheel[wheel][wheelCounter].Prev);
    return wheel;
}

/*
 *  \brief      PROCESS SYSTEM TICK
 * 
 *  \remark     This function is used to signal to OS the occurrence of a 'system tick' (also known
 *              as a 'clock tick').  This function should be called by the ticker ISR but, can also be
 *              called by a HIGH priority task.
 * 
 *  \param      none
 * 
 *  \return     none
 */
void osSysTick(void)
{
    OS_TCB             *ptcb;
    int                 wheel;
    UINT32              wheelCounter;


    if (osRunning == FALSE) {
        return;
    }
    
#if (OS_HOOKS_EN > 0u) && (OS_SYS_TICK_HOOK_EN > 0u)
    OSSysTickHook();
#endif
    
    OSEnterCriticalSection();

    ++osSysClockCounter;
    
    for (wheel = OS_SYS_TIMER_WHEEL_NUM - 1; wheel >= 0; wheel--) {
        wheelCounter = OS_SYS_TIMER_WHEEL_COUNTER_VALUE(osSysClockCounter, wheel);
        while (!LIST_IS_EMPTY(&osSysTimerWheel[wheel][wheelCounter])) {
            ptcb = CONTAINER_OF(osSysTimerWheel[wheel][wheelCounter].Next, OS_TCB, OSTCBList);
            list_remove(&ptcb->OSTCBList);
            
            if (os_sys_timer_insert(ptcb) < 0) {
                ptcb->OSTCBWaitNode->OSWaitNodeRes = OS_STAT_PEND_TO;     //! to indicate that it has been timeout.
                OS_WaitNodeRemove(ptcb);
                OS_UnpendTask(ptcb);
                OS_SchedulerReadyTask(ptcb);
            }
        }
    }

    OSExitCriticalSection();
}

/*
 *  \brief      SEND TASK TO SLEEP FOR 'n' TICKS
 * 
 *  \remark     This function is called to delay execution of the currently running task until the
 *              specified number of system ticks expires.  This, of course, directly equates to delaying
 *              the current task for some time to expire.  No delay will result If the specified delay is
 *              0.  If the specified delay is greater than 0 then, a context switch will result.
 * 
 *  \param      ticks       is the time delay that the task will be suspended in number of clock 'ticks'.
 *                          note that by specifying 0, the task will not be delayed.
 * 
 *  \return     none
 */
OS_ERR osTaskSleep(UINT32 ticks)
{
    OS_WAIT_NODE    node;
    
    
    if (osIntNesting != 0u) {                       //!< See if trying to call from an ISR
        return OS_ERR_USE_IN_ISR;
    }
    if (osLockNesting != 0u) {                      //!< See if called with scheduler locked
        return OS_ERR_PEND_LOCKED;
    }
    
    if (ticks == 0u || ticks == OS_INFINITE) {      //!< 0 or infinite is nonsense.
        return OS_ERR_INVALID_SLEEP_TIME;
    }
    
    //! initial wait node.
    node.OSWaitNodeTCB = osTCBCur;
    node.OSWaitNodeECB = NULL;
    node.OSWaitNodeRes = OS_STAT_PEND_OK;
    node.OSWaitNodeListHead = NULL;
    list_init(&node.OSWaitNodeList);
    
    OSEnterCriticalSection();
    osTCBCur->OSTCBDly      = ticks;
    osTCBCur->OSTCBWaitNode = &node;            //!< a node is used just as a tag that this task is not owned by the scheduler.
    OS_SchedulerUnreadyTask(osTCBCur);          //!< remove this task from scheduler's ready list.
    OS_PendTask(osTCBCur);                      //!< pend the task.
    OSExitCriticalSection();
    OS_SchedulerRunNext();                      //!< Find next task to run!

    return OS_ERR_NONE;
}

/*
 *  \brief      STATISTICS INITIALIZATION
 * 
 *  \remark     This function is called by your application to establish CPU usage by first determining
 *              how high a 32-bit counter would count to in 1 second if no other tasks were to execute
 *              during that time.  CPU usage is then determined by a low priority task which keeps track
 *              of this 32-bit counter every second but this time, with other tasks running.  CPU usage is
 *              determined by:
 * 
 *                                              osIdleCtr
 *                  CPU Usage (%) = 100 * (1 - ------------)
 *                                             osIdleCtrMax
 * 
 *  \param      none
 * 
 *  \return     none
 */
#if OS_STAT_EN > 0u
void osStatInit(void)
{
    osTaskSleep(2u);                            //!< Synchronize with clock tick
    OSEnterCriticalSection();
    osIdleCtr       = 0u;                       //!< Clear idle counter
    OSExitCriticalSection();
    
    osTaskSleep(OS_TICKS_PER_SEC);              //!< Determine MAX. idle counter value for 1 second

    OSEnterCriticalSection();
    osIdleCtrMax        = osIdleCtr;            //!< Store maximum idle counter count in 1 second
    osTaskStatRunning   = TRUE;
    OSExitCriticalSection();
}
#endif

/*
 *  \brief      CREATING THE STATISTIC TASK
 * 
 *  \remark     This function creates the Statistic Task.
 * 
 *  \param      none
 * 
 *  \return     none
 */
#if OS_STAT_EN > 0u
static void os_init_statistics_task(void)
{

    osTaskCreate(NULL,
                "os_task_statistics",
                os_task_statistics,
                NULL,
                osTaskStatStk,
                OS_TASK_STAT_STK_SIZE,
                OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR,
                OS_TASK_STAT_PRIO);
}
#endif

/*
 *  \brief      CREATING THE IDLE TASK
 * 
 *  \remark     This function creates the Idle Task.
 * 
 *  \param      none
 * 
 *  \return     none
 */
static void os_init_idle_task(void)
{

    osTaskCreate(NULL,
                 "os_task_idle",
                os_task_idle,
                NULL,
                osTaskIdleStk,
                OS_TASK_IDLE_STK_SIZE,
                OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR,
                OS_TASK_IDLE_PRIO);
}

/*
 *  \brief      IDLE TASK
 * 
 *  \remark     This task is internal to OS and executes whenever no other higher priority tasks
 *              executes because they are ALL waiting for event(s) to occur.
 * 
 *  \param      parg        this pointer is not used at this time.
 * 
 *  \return     none
 * 
 *  \note       1) OSTaskIdleHook() is called after the critical section to ensure that interrupts will be
 *                 enabled for at least a few instructions.  On some processors (ex. Philips XA), enabling
 *                 and then disabling interrupts didn't allow the processor enough time to have interrupts
 *                 enabled before they were disabled again.  OS would thus never recognize
 *                 interrupts.
 *              2) This hook has been added to allow you to do such things as STOP the CPU to conserve
 *                 power.
 */
static void *os_task_idle(void *parg)
{
    parg = parg;                               //!< Prevent compiler warning for not using 'parg'
    
    for (;;) {
        OSEnterCriticalSection();
        osIdleCtr++;
        OSExitCriticalSection();
        
#if OS_HOOKS_EN > 0u
        OSTaskIdleHook();                        //!< Call user definable HOOK
#endif
    }
}

/*
 *  \brief      STATISTICS TASK
 * 
 *  \remark     This task is internal to OS and is used to compute some statistics about the
 *              multitasking environment.  Specifically, os_task_statistics() computes the CPU usage.
 *              CPU usage is determined by:
 * 
 *                                           osIdleCtr
 *                  osCPUUsage = 100 * (1 - ------------)     (units are in %)
 *                                          osIdleCtrMax
 * 
 *  \param      parg     this pointer is not used at this time.
 * 
 *  \return     none
 * 
 *  \note       1) This task runs at a priority level higher than the idle task.  In fact, it runs at the
 *                 next higher priority, OS_TASK_IDLE_PRIO-1.
 *              2) You can disable this task by setting the configuration #define OS_STAT_EN to 0.
 *              3) You MUST have at least a delay of 1 seconds to allow for the system to establish the
 *                 maximum value for the idle counter.
 */
#if OS_STAT_EN > 0u
static void *os_task_statistics(void *parg)
{
    OS_LIST_NODE       *list;
    OS_TCB             *ptcb;


    parg = parg;                            //!< Prevent compiler warning for not using 'parg'
    
    while (osTaskStatRunning == FALSE) {    //!< Wait until osIdleCtrMax has been measured.
        osTaskSleep(OS_TICKS_PER_SEC);
    }
    osIdleCtrMax /= 100u;
    if (osIdleCtrMax == 0u) {
        osCPUUsage = 0u;
        osTaskSleep(OS_INFINITE);
    }
    
    for (;;) {
        osTaskSleep(OS_TICKS_PER_SEC);          //!< Accumulate osIdleCtr for the next 1/10 second

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
        OSEnterCriticalSection();
        for (int i = 0; i < OS_SYS_TIMER_WHEEL_NUM; i++) {
            for (int j = 0; j < OS_SYS_TIMER_WHEEL_BUCKET_NUM; j++) {
                for (list = osSysTimerWheel[i][j].Next; list != &osSysTimerWheel[i][j]; list = list->Next) {
                    ptcb = CONTAINER_OF(list, OS_TCB, OSTCBList);
                    OS_TaskStkChk(ptcb);
                }
            }
        }
        OSExitCriticalSection();
#endif
        
#if OS_HOOKS_EN > 0u
        OSTaskStatHook();                       //!< Invoke user definable hook
#endif
    }
}
#endif

/*
 *  \brief      INITIALIZE TCB
 * 
 *  \remark     This function is internal to OS and is used to get and initialize a Task Control Block when
 *              a task is created (see osTaskCreate()).
 * 
 *  \param      prio          is the priority of the task being created
 * 
 *              psp           stack pointer when task begin to run. This value return by OSTaskStkInit.
 *
 *              pstk          Point to the LOWEST (valid) memory location of the stack. It's not stack
 *                            pointer.
 * 
 *              stkSize       is the size of the stack (in 'stack units').  If the stack units are INT8Us
 *                            then, 'stkSize' contains the number of bytes for the stack.  If the stack
 *                            units are INT32Us then, the stack contains '4 * stkSize' bytes.  The stack
 *                            units are established by the #define constant CPU_STK which is CPU
 *                            specific.  'stkSize' is 0 if called by 'osTaskCreate()'.
 * 
 *              opt           options as passed to 'osTaskCreate()' or,
 *                            0 if called from 'osTaskCreate()'.
 * 
 *  \return     none
 * 
 *  \note       This function is INTERNAL to OS and your application should not call it.
 */
void OS_TCBInit(OS_TCB     *ptcb,
                const char *name,
                UINT8       prio,
                CPU_STK    *psp,
                CPU_STK    *pstk,
                UINT32      stkSize,
                UINT16      opt)
{
    ptcb->OSTCBObjHeader.OSObjType = OS_OBJ_TYPE_SET(OS_OBJ_TYPE_TCB);
    
    ptcb->OSTCBStkPtr       = psp;                      //!< Load Stack Pointer in TCB
#if OS_TASK_PROFILE_EN > 0u || OS_TASK_STACK_ON_HEAP_EN > 0
    ptcb->OSTCBStkBase      = pstk;
#endif
    
    ptcb->OSTCBWaitNode     = NULL;                     //!< Task is not pending on anay object.
    
    list_init(&ptcb->OSTCBList);
    
#if (OS_MUTEX_EN > 0u) && (OS_MAX_MUTEXES > 0u)
    list_init(&ptcb->OSTCBOwnMutexList);
#endif

    ptcb->OSTCBOpt          = opt;                      //!< Store task options
    ptcb->OSTCBPrio         = prio;                     //!< Load task priority into TCB
    
#if OS_TASK_PROFILE_EN > 0u                             //!< Initialize profiling variables
    ptcb->OSTCBName         = name;
    ptcb->OSTCBStkSize      = stkSize;
    ptcb->OSTCBStkUsed      = 0u;
    ptcb->OSTCBCtxSwCtr     = 0u;
#endif

#if OS_HOOKS_EN > 0u
    OSTCBInitHook(ptcb);
#endif
}

/*
 *  \brief      STACK CHECKING
 * 
 *  \remark     This function is called to check the amount of free memory left on the specified task's
 *              stack.
 * 
 *  \param      prio          is the task priority
 * 
 *  \return     OS_ERR_NONE            upon success
 *              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum allowed
 *              OS_ERR_TASK_NOT_EXIST  if the desired task has not been created or is assigned to a Mutex PIP
 *              OS_ERR_TASK_OPT        if you did NOT specified OS_TASK_OPT_STK_CHK when the task was created
 *              OS_ERR_PDATA_NULL      if 'p_stk_data' is a NULL pointer
 */
#if (OS_STAT_TASK_STK_CHK_EN > 0u)
void OS_TaskStkChk(OS_TCB *ptcb)
{
    CPU_STK    *pstk;
    CPU_STK    *pBotStk;
    UINT32     nfree;
    UINT32     size;


    OSEnterCriticalSection();
    if ((ptcb->OSTCBOpt & OS_TASK_OPT_STK_CHK) == 0u) { //!< Make sure stack checking option is set.
        OSExitCriticalSection();
        return;
    }
    size  = ptcb->OSTCBStkSize;
    pstk  = ptcb->OSTCBStkBase;
#if OS_CPU_STK_GROWTH_DOWN == 1u
    pBotStk = pstk + size;
#else
    pBotStk = pstk;
    pstk += size;
#endif

    nfree = 0u;
#if OS_CPU_STK_GROWTH_DOWN == 1u
    while (*pstk == OS_CPU_STK_CANARY_VALUE && pstk != pBotStk) {    //!< calculate the number of zero entries on the stack.
        nfree++;
        pstk++;
    }
#else
    while (*pstk == OS_CPU_STK_CANARY_VALUE && pstk != pBotStk) {
        nfree++;
        pstk--;
    }
#endif
    size -= nfree;  //!< calculate number of entries used on the stack.
    if (size > ptcb->OSTCBStkUsed) {
        ptcb->OSTCBStkUsed = size;
    }
    OSExitCriticalSection();
}
#endif

#if OS_TASK_DEL_EN > 0u
#if OS_MUTEX_EN > 0u
static void os_unlock_mutex(OS_MUTEX *pmutex)
{
    OS_TCB     *ptcb;
    
    
    pmutex->OSMutexCnt = 0u;

    list_remove(&pmutex->OSMutexOvlpList);
    
    if (!LIST_IS_EMPTY(&pmutex->OSMutexWaitList)) {                  //!< Is any task waiting for the mutex?
        ptcb = OS_WaitableObjRdyTask((OS_WAITABLE_OBJ *)pmutex,     //!< Yes, Make HPT waiting for mutex ready
                                    &pmutex->OSMutexWaitList,
                                    OS_STAT_PEND_OK);
        pmutex->OSMutexOwnerTCB  = ptcb;
        pmutex->OSMutexOwnerPrio = ptcb->OSTCBPrio;
    } else {                                                        //!< No.
        pmutex->OSMutexOwnerTCB  = NULL;
        pmutex->OSMutexOwnerPrio = 0u;
    }
}
#endif

/*
 *  \brief      STOP CURRENT TASK
 * 
 *  \remark     This function delete CURRENT running task. This function is
 *              internal to OS. Your task should be terminated by a return.
 *               
 * 
 *  \param      none
 * 
 *  \return     none
 *
 *  \note       In XOS, the only way to delete a task is by returning. That is to say a task can only
 *              be deleted by itself in the application. There is no API to terminate or delete a task.
 */
void OS_TaskStop(void)
{
    OS_TCB         *ptcb = osTCBCur;


    //! set free from any object that it suspend for.
    if (ptcb->OSTCBWaitNode != NULL) {      //!< Is this task suspend for any object?
        OS_WaitNodeRemove(ptcb);            //!< Yes, set it free from that object ...
        OS_UnpendTask(ptcb);            //!< ... and set it free from wait list.
    } else {                                //!< NO. It's owned by scheduler ...
        OS_SchedulerUnreadyTask(ptcb);      //!< ... set it free from scheduler.
    }

    //! to ensure task releases all mutex(es) that it has had got. This should be a fatal error???
#if OS_MUTEX_EN > 0u
    while (!LIST_IS_EMPTY(&ptcb->OSTCBOwnMutexList)) {
        OS_LIST_NODE *list = ptcb->OSTCBOwnMutexList.Next;
        OS_MUTEX *pmutex = CONTAINER_OF(list, OS_MUTEX, OSMutexOvlpList);
        os_unlock_mutex(pmutex);
    }
#endif
}
#endif      //!< #if OS_TASK_DEL_EN > 0u

/*
 *  \brief      CHANGE PRIORITY OF A TASK
 * 
 *  \remark     This function changes the priority of a task.
 * 
 *  \param      ptcb     pointer to tcb
 * 
 *              newp     is the new priority
 * 
 *  \return     none
 * 
 *  \note       1) This function assumes that interrupts are disabled.
 *              2) This function is INTERNAL to OS and your application should not call it.
 */
void OS_ChangeTaskPrio(OS_TCB *ptcb, UINT8 newprio)
{
    OS_WAITABLE_OBJ    *pobj;
    OS_WAIT_NODE       *pnode;


    pnode = ptcb->OSTCBWaitNode;
    if (pnode != NULL) {                        //!< if the task is waiting for any object or sleep?
        pobj = pnode->OSWaitNodeECB;
        if (pobj != NULL) {                     //!< Is this task waiting for any object?
            if (OS_OBJ_PRIO_TYPE_GET(pobj->OSWaitObjHeader.OSObjType) == OS_OBJ_PRIO_TYPE_PRIO_LIST) {  //!< Yes. Has this object a prio-wait list?
                OS_LIST_NODE *listNode;                                                                 //!< Yes...
                OS_LIST_NODE *pList = pnode->OSWaitNodeListHead;
                OS_WAIT_NODE *waitNode;
                
                list_remove(&pnode->OSWaitNodeList);                                                    //!< ...remove wait node from old priority.
                                                                                                        //!  then find and put on the new position.
                for (listNode = pList->Next; listNode != pList; listNode = listNode->Next) {
                    waitNode = CONTAINER_OF(listNode, OS_WAIT_NODE, OSWaitNodeList);
                    if (newprio < waitNode->OSWaitNodeTCB->OSTCBPrio) {
                        break;
                    }
                }
                list_insert(&pnode->OSWaitNodeList, listNode->Prev);
            }
        }
        ptcb->OSTCBPrio = newprio;              //!< Set new task priority
    } else {                                    //!< Task is owned by scheduler.
        OS_SchedulerUnreadyTask(ptcb);          //!< Remove TCB from old priority
        ptcb->OSTCBPrio = newprio;              //!< Set new task priority
        OS_SchedulerReadyTask(ptcb);            //!< Place TCB @ new priority
    }
}

void OS_BitmapSet(OS_PRIO_BITMAP *pmap, UINT8 bit)
{
    UINT16 y;
    UINT16 mskX, mskY;
    
    y = (bit >> 4) & 0x000Fu;
    mskX = 1u << (bit & 0x000Fu);
    mskY = 1u << y;
    
    pmap->Y    |= mskY;
    pmap->X[y] |= mskX;
}

void OS_BitmapClr(OS_PRIO_BITMAP *pmap, UINT8 bit)
{
    UINT16 y;
    UINT16 mskX, mskY;
    
    y = (bit >> 4) & 0x000Fu;
    mskX = 1u << (bit & 0x000Fu);
    mskY = 1u << y;

    pmap->X[y] &= (UINT16)~mskX;
    if (pmap->X[y] == 0u) {
        pmap->Y &= (UINT16)~mskY;
    }
}

UINT8 OS_BitmapGetLeadingZero(OS_PRIO_BITMAP *pmap)
{
    UINT16 y;
    UINT16 bit;
    UINT16 valX;


    //! find the highest priority of ready task. We support up to 256 levels.
    if ((pmap->Y & 0x00FFu) != 0u) {
        y =      OS_COUNT_LEADING_ZERO(pmap->Y & 0x00FFu);
    } else {
        y = 8u + OS_COUNT_LEADING_ZERO((pmap->Y >> 8u) & 0x00FFu);
    }
    valX = pmap->X[y];
    if ((valX & 0x00FFu) != 0u) {
        bit = (y * 16u) +      OS_COUNT_LEADING_ZERO(valX & 0x00FFu);
    } else {
        bit = (y * 16u) + 8u + OS_COUNT_LEADING_ZERO((valX >> 8u) & 0x00FFu);
    }
    
    return bit;
}

UINT16 osVersion(void)
{
    return OS_VERSION;
}

UINT32 osGetSysTickCount(void)
{
    UINT32 value;
    
    OSEnterCriticalSection();
    value = osSysClockCounter;
    OSExitCriticalSection();
    return value;
}

