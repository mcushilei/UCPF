
#define  __OS_CORE_C__
#include ".\os.h"

/*********************************************************************************************************
*                                      PRIORITY RESOLUTION TABLE
*
* Note: Index into table is bit pattern to resolve highest priority
*       Indexed value corresponds to highest priority bit position (i.e. 0..7)
*       Leading 0 algorithm.
*********************************************************************************************************/

static const UINT8 OSUnMapTbl[256] = {
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

/*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************/

static  void    os_init_tcb_list(void);

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
static  void    os_init_ecb_list(void);
#endif

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
static  void    os_init_flag_list(void);
#endif

static  void    os_init_misc(void);

static  void    os_init_ready_table(void);

static  UINT8   os_get_highest_prio(void);

#if OS_STAT_EN > 0u
static  void    os_init_statistics_task(void);
static  void    os_task_statistics(void *parg);
#endif

static  void    os_init_idle_task(void);
static  void    os_task_idle(void *parg);



/*********************************************************************************************************
*                                           INITIALIZATION
*
* Description: This function is used to initialize the internals of OS and MUST be called prior to
*              creating any OS object and, prior to calling osStart().
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

void    osInit (void)
{
#if OS_HOOKS_EN > 0
    OSInitHookBegin();                                          //!< Call port specific initialization code
#endif

    os_init_misc();                                              //!< Initialize miscellaneous variables

    os_init_ready_table();                                          //!< Initialize the Ready List

    os_init_tcb_list();                                           //!< Initialize the free list of OS_TCBs

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
    os_init_ecb_list();                                           //!< Initialize the free list of OS_EVENTs
#endif

#if (OS_FLAG_EN > 0u) && (OS_MAX_FLAGS > 0u)
    os_init_flag_list();                                          //!< Initialize the free list of event flag
#endif

    os_init_idle_task();                                          //!< Create the Idle Task
#if OS_STAT_EN > 0u
    os_init_statistics_task();                                          //!< Create the Statistic Task
#endif

#if OS_HOOKS_EN > 0u
    OSInitHookEnd();                                            //!< Call port specific init. code
#endif

#if OS_DEBUG_EN > 0u
    OSDebugInit();
#endif
}

/*********************************************************************************************************
*                                              SCHEDULER
*
* Description: This function is called by other OS services to determine whether a new, high
*              priority task has been made ready to run.  This function is invoked by TASK level code
*              and is not used to reschedule tasks from ISRs (see osIntExit() for ISR rescheduling).
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) This function is INTERNAL to OS and your application should not call it.
*              2) Rescheduling is prevented when the scheduler is locked (see OS_SchedLock())
*********************************************************************************************************/

void    OS_Schedule(void)
{
    UINT8       prio;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    OSEnterCriticalSection(cpu_sr);
    if (osIntNesting == 0u) {                           //!< Schedule only if all ISRs done and ...
        if (osLockNesting == 0u) {                      //!< ... scheduler is not locked
            prio = os_get_highest_prio();               //!< Get the next task
            osTCBHighRdy = osPrioTCBTbl[prio];
            if (osTCBHighRdy != osTCBCur) {             //!< No Ctx Sw if current task is highest rdy
                OSExitCriticalSection(cpu_sr);
                OSCtxSw();                              //!< Perform a context switch
                return;
            }
        }
    }
    OSExitCriticalSection(cpu_sr);
}

/*********************************************************************************************************
*                                              ENTER ISR
*
* Description: This function is used to notify OS that you are about to service an interrupt
*              service routine (ISR).  This allows OS to keep track of interrupt nesting and thus
*              only perform rescheduling at the last nested ISR.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) This function should be called with interrupts already DISABLED.
*              2) You MUST invoke osIntEnter() and osIntExit() in pair.  In other words, for every call
*                 to osIntEnter() at the beginning of the ISR you MUST have a call to osIntExit() at the
*                 end of the ISR.
*              3) You are allowed to nest interrupts up to 255 levels deep.
*********************************************************************************************************/

void    osIntEnter (void)
{
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif
    
    
    if (osRunning == FALSE) {
        return;
    }
    
    OSEnterCriticalSection(cpu_sr);
    if (osIntNesting < 255u) {
        osIntNesting++;                 //!< Increment ISR nesting level
    }
    OSExitCriticalSection(cpu_sr);
}

/*********************************************************************************************************
*                                              EXIT ISR
*
* Description: This function is used to notify os that you have completed servicing an ISR.  When
*              the last nested ISR has completed, OS will call the scheduler to determine whether
*              a new, high-priority task, is ready to run.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke osIntEnter() and osIntExit() in pair.  In other words, for every call
*                 to osIntEnter() at the beginning of the ISR you MUST have a call to osIntExit() at the
*                 end of the ISR.
*              2) Rescheduling is prevented when the scheduler is locked.
*********************************************************************************************************/

void    osIntExit  (void)
{
    UINT8       prio;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osRunning == FALSE) {
        return;
    }
    
    OSEnterCriticalSection(cpu_sr);
    if (osIntNesting > 0u) {                            //!< Prevent osIntNesting from wrapping
        osIntNesting--;
    }
    if (osIntNesting == 0u) {                           //!< Reschedule only if all ISRs complete ...
        if (osLockNesting == 0u) {                      //!< ... and scheduler is not locked.
            prio = os_get_highest_prio();
            osTCBHighRdy = osPrioTCBTbl[prio];
            if (osTCBHighRdy != osTCBCur) {             //!< No Ctx Sw if current task is highest rdy
                OSExitCriticalSection(cpu_sr);
                OSIntCtxSw();                           //!< Perform interrupt level ctx switch
                return;
            }
        }
    }
    OSExitCriticalSection(cpu_sr);
}


/*********************************************************************************************************
*                                         PREVENT SCHEDULING
*
* Description: This function is used to prevent rescheduling to take place.  This allows your application
*              to prevent context switches until you are ready to permit context switching.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke osLockSched() and osUnlockSched() in pair.  In other words, for every
*                 call to osLockSched() you MUST have a call to osUnlockSched().
*********************************************************************************************************/

#if OS_SCHED_LOCK_EN > 0u
void    osLockSched(void)
{
#if OS_CRITICAL_METHOD == 3u                     //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    if (osRunning != FALSE) {                  //!< Make sure multitasking is running
        if (osIntNesting == 0u) {                //!< Can't call from an ISR
            OSEnterCriticalSection(cpu_sr);
            if (osLockNesting < 255u) {          //!< Prevent osLockNesting from wrapping back to 0
                osLockNesting++;                 //!< Increment lock nesting level
            }
            OSExitCriticalSection(cpu_sr);
        }
    }
}
#endif

/*********************************************************************************************************
*                                          ENABLE SCHEDULING
*
* Description: This function is used to re-allow rescheduling.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke osLockSched() and osUnlockSched() in pair.  In other words, for every
*                 call to osLockSched() you MUST have a call to osUnlockSched().
*********************************************************************************************************/

#if OS_SCHED_LOCK_EN > 0u
void    osUnlockSched  (void)
{
#if OS_CRITICAL_METHOD == 3u                               //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    if (osRunning != FALSE) {                  //!< Make sure multitasking is running
        if (osIntNesting == 0u) {                          //!< Can't call from an ISR
            OSEnterCriticalSection(cpu_sr);
            if (osLockNesting > 0u) {                      //!< Do not decrement if already 0
                osLockNesting--;                           //!< Decrement lock nesting level
            }
            OSExitCriticalSection(cpu_sr);
            OS_Schedule();
        }
    }
}
#endif

/*********************************************************************************************************
*                                         START MULTITASKING
*
* Description: This function is used to start the multitasking process which lets OS manages the
*              task that you have created.  Before you can call osStart(), you MUST have called osInit()
*              and you MUST have created at least one task.
*
* Arguments  : none
*
* Returns    : none
*
* Note       : OSStartHighRdy() MUST:
*                 a) Call OSTaskSwHook() then,
*                 b) Set osRunning to TRUE.
*                 c) Load the context of the task pointed to by osTCBHighRdy.
*                 d) Execute the task.
*                 e) Enable system-level interrupt.
*********************************************************************************************************/

void    osStart(void)
{
    UINT8   prio;
    
    
    if (osRunning == FALSE) {
        prio          = os_get_highest_prio();
        osTCBHighRdy  = osPrioTCBTbl[prio];     //!< Point to highest priority task ready to run
        osTCBCur      = osTCBHighRdy;
        OSStartHighRdy();                       //!< Execute target specific code to start task
    }
}

/*********************************************************************************************************
*                                         PROCESS SYSTEM TICK
*
* Description: This function is used to signal to OS the occurrence of a 'system tick' (also known
*              as a 'clock tick').  This function should be called by the ticker ISR but, can also be
*              called by a HIGH priority task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

void    osTimeTick (void)
{
    OS_TCB    *ptcb;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


#if (OS_HOOKS_EN > 0u) && (OS_TIME_TICK_HOOK_EN > 0u)
    OSTimeTickHook();
#endif
    
    if (osRunning) {
        OSEnterCriticalSection(cpu_sr);
        for (ptcb = osTaskList; ptcb != NULL; ptcb = ptcb->OSTCBNext) {      //!< Go through all task in TCB list.
            if ((ptcb->OSTCBDly != 0u) &&
                (ptcb->OSTCBDly != OS_INFINITE)) {
                ptcb->OSTCBDly--;
                if (ptcb->OSTCBDly == 0u) {                                     //!< If timeout
                    if (ptcb->OSTCBWaitNode != NULL) {                       //!< See if waiting for any event
                        ptcb->OSTCBWaitNode->OSWaitNodeRes = OS_STAT_PEND_TO;   //!< ...Indicate PEND timeout.
                        OS_EventTaskRemove(ptcb);                               //!< Yes, Remove from any event and...
                    }

                    osRdyGrp               |= 1u << ptcb->OSTCBY;                  //!< Make task ready-to-run
                    osRdyTbl[ptcb->OSTCBY] |= 1u << ptcb->OSTCBX;
                }
            }
        }
        OSExitCriticalSection(cpu_sr);
    }
}

/*********************************************************************************************************
*                                        DELAY TASK 'n' TICKS
*
* Description: This function is called to delay execution of the currently running task until the
*              specified number of system ticks expires.  This, of course, directly equates to delaying
*              the current task for some time to expire.  No delay will result If the specified delay is
*              0.  If the specified delay is greater than 0 then, a context switch will result.
*
* Arguments  : ticks     is the time delay that the task will be suspended in number of clock 'ticks'.
*                        Note that by specifying 0, the task will not be delayed.
*
* Returns    : none
*********************************************************************************************************/

void    osTimeDelay(UINT32 ticks)
{
    UINT8      y;
#if OS_CRITICAL_METHOD == 3u                        //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    if (osIntNesting != 0u) {                       //!< See if trying to call from an ISR
        return;
    }
    if (osLockNesting != 0u) {                      //!< See if called with scheduler locked
        return;
    }
    if ((ticks != 0u) && (ticks != OS_INFINITE)) {  //!< 0 means no delay!
        OSEnterCriticalSection(cpu_sr);
        y = osTCBCur->OSTCBY;                       //!< remove task from ready table
        osRdyTbl[y] &= (OS_PRIO)~(1u << osTCBCur->OSTCBX);
        if (osRdyTbl[y] == 0u) {
            osRdyGrp &= (OS_PRIO)~(1u << osTCBCur->OSTCBY);
        }
        osTCBCur->OSTCBDly = ticks;                 //!< Load ticks in TCB
        OSExitCriticalSection(cpu_sr);
        OS_Schedule();                                 //!< Find next task to run!
    }
}

/*********************************************************************************************************
*                                      STATISTICS INITIALIZATION
*
* Description: This function is called by your application to establish CPU usage by first determining
*              how high a 32-bit counter would count to in 1 second if no other tasks were to execute
*              during that time.  CPU usage is then determined by a low priority task which keeps track
*              of this 32-bit counter every second but this time, with other tasks running.  CPU usage is
*              determined by:
*
*                                             osIdleCtr
*                 CPU Usage (%) = 100 * (1 - ------------)
*                                            osIdleCtrMax
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

#if OS_STAT_EN > 0u
void    osStatInit (void)
{
#if OS_CRITICAL_METHOD == 3u                     //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    osTimeDelay(2u);                            //!< Synchronize with clock tick
    OSEnterCriticalSection(cpu_sr);
    osIdleCtr       = 0u;                       //!< Clear idle counter
    OSExitCriticalSection(cpu_sr);
    osTimeDelay(OS_TICKS_PER_SEC);              //!< Determine MAX. idle counter value for 1 second
    OSEnterCriticalSection(cpu_sr);
    osIdleCtrMax    = osIdleCtr;                //!< Store maximum idle counter count in 1 second
    osStatRunning   = TRUE;
    OSExitCriticalSection(cpu_sr);
}
#endif

/*********************************************************************************************************
*                                             GET VERSION
*
* Description: This function is used to return the version number of OS.  The returned value
*              corresponds to OS's version number multiplied by 10000.  In other words, version
*              2.01.00 would be returned as 20100.
*
* Arguments  : none
*
* Returns    : The version number of OS multiplied by 10000.
*********************************************************************************************************/

UINT16  osVersion (void)
{
    return OS_VERSION;
}

/*********************************************************************************************************
*                                             INITIALIZATION
*                                    INITIALIZE MISCELLANEOUS VARIABLES
*
* Description: This function is called by osInit() to initialize miscellaneous variables.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

static void os_init_misc   (void)
{
    osIntNesting             = 0u;                        //!< Clear the interrupt nesting counter
    osLockNesting            = 0u;                        //!< Clear the scheduling lock counter

    osRunning                = FALSE;                  //!< Indicate that multitasking not started

    osIdleCtr                = 0u;                        //!< Clear the idle counter

#if OS_STAT_EN > 0u
    osCtxSwCtr               = 0u;                        //!< Clear the context switch counter
    osCPUUsage               = 0u;
    osIdleCtrMax             = 0u;
    osStatRunning            = FALSE;                  //!< Statistic task is not ready
#endif
}

/*********************************************************************************************************
*                                             INITIALIZATION
*                                       INITIALIZE THE READY LIST
*
* Description: This function is called by osInit() to initialize the Ready List.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

static void os_init_ready_table(void)
{
    uint_fast8_t  i;


    osRdyGrp = 0u;
    for (i = 0u; i < OS_BITMAP_TBL_SIZE; i++) {
        osRdyTbl[i] = 0u;
    }

    osTCBCur      = NULL;
    osTCBHighRdy  = NULL;
}

/*********************************************************************************************************
*                               FIND HIGHEST PRIORITY TASK READY TO RUN
*
* Description: This function is called by other OS services to determine the highest priority task
*              that is ready to run.
*
* Arguments  : none
*
* Returns    : The priority of the highest ready task in ready table.
*
* Notes      : 1) This function is INTERNAL to OS and your application should not call it.
*              2) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************/

static UINT8    os_get_highest_prio(void)
{
    UINT8   y;
    UINT8   prio;
    OS_PRIO tbl_val;


#if OS_MAX_PRIO_LEVELS <= 64u                        //!< See if we support up to 64 tasks
    y       = OSUnMapTbl[osRdyGrp];
    tbl_val = osRdyTbl[y];
    prio    = (y << 3u) + OSUnMapTbl[tbl_val];
#else                                            //!< We support up to 256 tasks
    if ((osRdyGrp & 0xFFu) != 0u) {
        y = OSUnMapTbl[osRdyGrp & 0xFFu];
    } else {
        y = OSUnMapTbl[(osRdyGrp >> 8u) & 0xFFu] + 8u;
    }
    tbl_val = osRdyTbl[y];
    if ((tbl_val & 0xFFu) != 0u) {
        prio = (y << 4u) + OSUnMapTbl[tbl_val & 0xFFu];
    } else {
        prio = (y << 4u) + OSUnMapTbl[(tbl_val >> 8u) & 0xFFu] + 8u;
    }
#endif
    
    return prio;
}

/*********************************************************************************************************
*                                             INITIALIZATION
*                                         CREATING THE IDLE TASK
*
* Description: This function creates the Idle Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

static void os_init_idle_task  (void)
{
    (void)osTaskCreate(NULL,
                       os_task_idle,
                       NULL,                                     //!< No arguments passed to os_task_idle()
                       OS_TASK_IDLE_PRIO,                           //!< Lowest priority level
                       osTaskIdleStk,                               //!< Set Bottom-Of-Stack
                       OS_TASK_IDLE_STK_SIZE,
                       OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  //!< Enable stack checking + clear stack
}

/*********************************************************************************************************
*                                             INITIALIZATION
*                                      CREATING THE STATISTIC TASK
*
* Description: This function creates the Statistic Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

#if OS_STAT_EN > 0u
static void os_init_statistics_task(void)
{
    (void)osTaskCreate(NULL,
                       os_task_statistics,
                       NULL,                                     //!< No args passed to os_task_statistics()*/
                       OS_TASK_STAT_PRIO,                           //!< One higher than the idle task
                       osTaskStatStk,                               //!< Set Bottom-Of-Stack
                       OS_TASK_STAT_STK_SIZE,
                       OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  //!< Enable stack checking + clear
}
#endif

/*********************************************************************************************************
*                                              IDLE TASK
*
* Description: This task is internal to OS and executes whenever no other higher priority tasks
*              executes because they are ALL waiting for event(s) to occur.
*
* Arguments  : parg     this pointer is not used at this time.
*
* Returns    : none
*
* Note(s)    : 1) OSTaskIdleHook() is called after the critical section to ensure that interrupts will be
*                 enabled for at least a few instructions.  On some processors (ex. Philips XA), enabling
*                 and then disabling interrupts didn't allow the processor enough time to have interrupts
*                 enabled before they were disabled again.  OS would thus never recognize
*                 interrupts.
*              2) This hook has been added to allow you to do such things as STOP the CPU to conserve
*                 power.
*********************************************************************************************************/

static void os_task_idle   (void *parg)
{
#if OS_CRITICAL_METHOD == 3u                     //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    parg = parg;                               //!< Prevent compiler warning for not using 'parg'
    
    for (;;) {
        OSEnterCriticalSection(cpu_sr);
        osIdleCtr++;
        OSExitCriticalSection(cpu_sr);
        
#if OS_HOOKS_EN > 0u
        OSTaskIdleHook();                        //!< Call user definable HOOK
#endif
    }
}

/*********************************************************************************************************
*                                           STATISTICS TASK
*
* Description: This task is internal to OS and is used to compute some statistics about the
*              multitasking environment.  Specifically, os_task_statistics() computes the CPU usage.
*              CPU usage is determined by:
*
*                                          osIdleCtr
*                 osCPUUsage = 100 * (1 - ------------)     (units are in %)
*                                         osIdleCtrMax
*
* Arguments  : parg     this pointer is not used at this time.
*
* Returns    : none
*
* Notes      : 1) This task runs at a priority level higher than the idle task.  In fact, it runs at the
*                 next higher priority, OS_TASK_IDLE_PRIO-1.
*              2) You can disable this task by setting the configuration #define OS_STAT_EN to 0.
*              3) You MUST have at least a delay of 1 seconds to allow for the system to establish the
*                 maximum value for the idle counter.
*********************************************************************************************************/

#if OS_STAT_EN > 0u
static void os_task_statistics (void *parg)
{
    UINT32      idl_cnt;                        //!< Val. reached by idle ctr at run time in 1 sec.
    OS_TCB     *ptcb;
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    parg = parg;                                //!< Prevent compiler warning for not using 'parg'
    
    while (osStatRunning == FALSE) {         //!< Wait until osIdleCtrMax has been measured.
        osTimeDelay(OS_TICKS_PER_SEC);
    }
    osIdleCtrMax /= 100u;
    if (osIdleCtrMax == 0u) {
        osCPUUsage = 0u;
        osTimeDelay(OS_INFINITE);
    }
    
    OSEnterCriticalSection(cpu_sr);
    osIdleCtr = osIdleCtrMax * 100u;            //!< Initial CPU usage as 0%
    OSExitCriticalSection(cpu_sr);
    
    for (;;) {
        OSEnterCriticalSection(cpu_sr);
        idl_cnt     = osIdleCtr;                //!< Obtain the of the idle counter for the past second
        osIdleCtr   = 0u;                       //!< Reset the idle counter for the next second
        OSExitCriticalSection(cpu_sr);
        osCPUUsage  = 100u - idl_cnt / osIdleCtrMax;
        
#if (OS_STAT_TASK_STK_CHK_EN > 0u)
        OSEnterCriticalSection(cpu_sr);
        for (ptcb = osTaskList; ptcb != NULL; ptcb = ptcb->OSTCBNext) {
            OS_TaskStkChk(ptcb);
        }
        OSExitCriticalSection(cpu_sr);
#endif
        
#if OS_HOOKS_EN > 0u
        OSTaskStatHook();                       //!< Invoke user definable hook
#endif
        
        osTimeDelay(OS_TICKS_PER_SEC);          //!< Accumulate osIdleCtr for the next 1/10 second
    }
}
#endif

/*********************************************************************************************************
*                                             INITIALIZATION
*                            INITIALIZE THE FREE LIST OF TASK CONTROL BLOCKS
*
* Description: This function is called by osInit() to initialize the free list of task control blocks.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

static void os_init_tcb_list   (void)
{
    uint_fast16_t   i;
    OS_TCB         *ptcb;
    OS_TCB        **pptcb;


    OS_MemClr((UINT8 *)&osTCBFreeTbl[0],     sizeof(osTCBFreeTbl)); //!< Clear all the TCBs
    OS_MemClr((UINT8 *)&osPrioTCBTbl[0], sizeof(osPrioTCBTbl));     //!< Clear the priority table
    osTaskList = NULL;                                           //!< TCB lists initializations

    pptcb = &osTCBFreeList;
    for (i = 0u; i < (OS_MAX_TASKS + OS_N_SYS_TASKS); i++) {        //!< Init. list of free TCBs
        ptcb   = &osTCBFreeTbl[i];
        *pptcb = ptcb;
        pptcb  = &ptcb->OSTCBNext;
    }
}

/*********************************************************************************************************
*                                           INITIALIZE TCB
*
* Description: This function is internal to OS and is used to get and initialize a Task Control Block when
*              a task is created (see osTaskCreate()).
*
* Arguments  : prio          is the priority of the task being created
*
*              psp           stack pointer when task begin to run. This value return by OSTaskStkInit.
*
*              pstk          Point to the LOWEST (valid) memory location of the stack. It's not stack
*                            pointer.
*
*              stk_size      is the size of the stack (in 'stack units').  If the stack units are INT8Us
*                            then, 'stk_size' contains the number of bytes for the stack.  If the stack
*                            units are INT32Us then, the stack contains '4 * stk_size' bytes.  The stack
*                            units are established by the #define constant OS_STK which is CPU
*                            specific.  'stk_size' is 0 if called by 'osTaskCreate()'.
*
*              opt           options as passed to 'osTaskCreate()' or,
*                            0 if called from 'osTaskCreate()'.
*
* Returns    : none
*
* Note       : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

void    OS_TCBInit (OS_TCB  *ptcb,
                    UINT8    prio,
                    OS_STK  *psp,
                    OS_STK  *pstk,
                    UINT32   stk_size,
                    UINT8    opt)
{
    ptcb->OSTCBStkPtr       = psp;                      //!< Load Stack Pointer in TCB

    ptcb->OSTCBWaitNode     = NULL;                  //!< Task is not pending on an flag
    
    ptcb->OSTCBDly          = 0u;                       //!< Task is not delayed
    ptcb->OSTCBOpt          = opt;                      //!< Store task options
    ptcb->OSTCBPrio         = prio;                     //!< Load task priority into TCB

#if OS_MAX_PRIO_LEVELS <= 64u                                   //!< Pre-compute X, Y
    ptcb->OSTCBY            = (UINT8)(prio >> 3u);
    ptcb->OSTCBX            = (UINT8)(prio & 0x07u);
#else                                                       //!< Pre-compute X, Y
    ptcb->OSTCBY            = (UINT8)((UINT8)(prio >> 4u) & 0xFFu);
    ptcb->OSTCBX            = (UINT8) (prio & 0x0Fu);
#endif
                                                        //!< Pre-compute BitX and BitY
#if OS_TASK_PROFILE_EN > 0u                                 //!< Initialize profiling variables
    ptcb->OSTCBStkBase      = pstk;
    ptcb->OSTCBStkSize      = stk_size;
    ptcb->OSTCBStkUsed      = 0u;
    ptcb->OSTCBCtxSwCtr     = 0u;
    ptcb->OSTCBCyclesStart  = 0u;
    ptcb->OSTCBCyclesTot    = 0u;
#endif

#if OS_HOOKS_EN > 0u
    OSTCBInitHook(ptcb);
#endif
}

/*********************************************************************************************************
*                                           STACK CHECKING
*
* Description: This function is called to check the amount of free memory left on the specified task's
*              stack.
*
* Arguments  : prio          is the task priority
*
* Returns    : OS_ERR_NONE            upon success
*              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum allowed
*              OS_ERR_TASK_NOT_EXIST  if the desired task has not been created or is assigned to a Mutex PIP
*              OS_ERR_TASK_OPT        if you did NOT specified OS_TASK_OPT_STK_CHK when the task was created
*              OS_ERR_PDATA_NULL      if 'p_stk_data' is a NULL pointer
*********************************************************************************************************/

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
void    OS_TaskStkChk  (OS_TCB    *ptcb)
{
    OS_STK    *pstk;
    UINT32     nfree;
    UINT32     size;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR  cpu_sr = 0u;
#endif


    OSEnterCriticalSection(cpu_sr);
    if ((ptcb->OSTCBOpt & OS_TASK_OPT_STK_CHK) == 0u) { //!< Make sure stack checking option is set
        OSExitCriticalSection(cpu_sr);
        return;
    }
    size  = ptcb->OSTCBStkSize;
    pstk  = ptcb->OSTCBStkBase;
#if OS_STK_GROWTH_DOWN == 1u
#else
    pstk += size;
#endif

    nfree = 0u;
#if OS_STK_GROWTH_DOWN == 1u
    while (*pstk++ == 0u) {                             //!< Compute the number of zero entries on the stk
        nfree++;
    }
#else
    while (*pstk-- == 0u) {
        nfree++;
    }
#endif
    ptcb->OSTCBStkUsed = size - nfree;                  //!< Compute number of entries used on the stk
    OSExitCriticalSection(cpu_sr);
}
#endif

/*********************************************************************************************************
*                                      CHANGE PRIORITY OF A TASK
*
* Description: This function changes the priority of a task.
*
* Arguments  : ptcb     pointer to tcb
*
*              newp     is the new priority
*
* Returns    : none
*
* Note       : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

void    OS_TaskChangePrio  (OS_TCB *ptcb,
                            UINT8   newprio)
{
#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
    OS_EVENT  *pevent;
#endif
    OS_WAIT_NODE *pnode;
    UINT8      oldprio;
    UINT8      y_new;
    UINT8      x_new;
    UINT8      y_old;
    UINT8      x_old;
#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR  cpu_sr = 0u;                                 //!< Storage for CPU status register
#endif


    OSEnterCriticalSection(cpu_sr);
#if OS_MAX_PRIO_LEVELS <= 64u
    y_new                 = (UINT8)(newprio >> 3u);
    x_new                 = (UINT8)(newprio & 0x07u);
#else
    y_new                 = (UINT8)((UINT8)(newprio >> 4u) & 0x0Fu);
    x_new                 = (UINT8)(newprio & 0x0Fu);
#endif

    oldprio  = ptcb->OSTCBPrio;
    y_old    = ptcb->OSTCBY;
    x_old    = ptcb->OSTCBX;
    
    osPrioTCBTbl[oldprio] = NULL;                        //!< Remove TCB from old priority
    osPrioTCBTbl[newprio] = ptcb;                           //!< Place TCB @ new priority
    
    if ((osRdyTbl[y_old] &   (1u << x_old)) != 0u) {             //!< Is task ready?
         osRdyTbl[y_old] &= (OS_PRIO)~(1u << x_old);             //!< Yes, clear old ready bit...
         if (osRdyTbl[y_old] == 0u) {
             osRdyGrp &= (OS_PRIO)~(1u << y_old);
         }
         osRdyGrp        |= 1u << y_new;                       //!!< ... and set new.
         osRdyTbl[y_new] |= 1u << x_new;
    }

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
    pnode = ptcb->OSTCBWaitNode;
    if (pnode != NULL) {                                 //!< Is this task pending for any event?
        pevent = pnode->OSWaitNodeECB;
        switch (pevent->OSObjType & OS_OBJ_PRIO_TYPE_MSK) {
            case OS_OBJ_PRIO_TYPE_PRIO_LIST:
                break;
                
            case OS_OBJ_PRIO_TYPE_BITMAP:
                pevent->OSEventTbl[y_old] &= (OS_PRIO)~(1u << x_old);    //!< Remove old task prio from bitmap
                if (pevent->OSEventTbl[y_old] == 0u) {
                    pevent->OSEventGrp    &= (OS_PRIO)~(1u << y_old);
                }
                pevent->OSEventGrp        |= 1u << y_new;              //!< Add new task prio to bitmap
                pevent->OSEventTbl[y_new] |= 1u << x_new;
                break;
                
            case OS_OBJ_PRIO_TYPE_LIST:
            default:
                break;
        }
    }
#endif

    ptcb->OSTCBPrio = newprio;                                  //!< Set new task priority
    ptcb->OSTCBY    = y_new;
    ptcb->OSTCBX    = x_new;
    OSExitCriticalSection(cpu_sr);
}

/*********************************************************************************************************
*                                  INITIALIZE THE EVENT FLAG MODULE
*
* Description: This function is called by OS to initialize the event flag module.  Your application
*              MUST NOT call this function.  In other words, this function is internal to OS.
*
* Arguments  : none
*
* Returns    : none
*
* WARNING    : You MUST NOT call this function from your code.  This is an INTERNAL function to OS.
*********************************************************************************************************/

static void os_init_flag_list (void)
{
    uint_fast16_t   i;
    OS_FLAG        *pflag;
    OS_FLAG       **ppflag;


    OS_MemClr((UINT8 *)&osFlagFreeTbl[0], sizeof(osFlagFreeTbl));   //!< Clear the flag group table

    ppflag = &osFlagFreeList;
    for (i = 0u; i < OS_MAX_FLAGS; i++) {   //!< Init. list of free EVENT FLAGS
        pflag   = &osFlagFreeTbl[i];
        *ppflag = pflag;
        ppflag  = (OS_FLAG **)&pflag->OSFlagWaitList;
    }
}

/*********************************************************************************************************
*                                  MAKE TASK WAIT FOR EVENT TO OCCUR
*
* Description: This function is called by other OS services to suspend a task because an event has
*              not occurred.
*
* Arguments  : pevent   is a pointer to the event control block for which the task will be waiting for.
*
*              pnode         is a pointer to a structure which contains data about the task waiting for
*                            event flag bit(s) to be set.
*
*              timeout       is the desired amount of time that the task will wait for the event flag
*                            bit(s) to be set.
*
* Returns    : none
*
* Called by  : osFlagPend()  os_flag.c
*
* Note       : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

void    OS_EventTaskWait   (void           *pecb,
                            OS_WAIT_NODE   *pnode,
                            UINT32          timeout)
{
    OS_OBJ_HEAD *pObjHead = (OS_OBJ_HEAD *)pecb;
    UINT8  y = osTCBCur->OSTCBY;


    pnode->OSWaitNodeECB    = pecb;                 //!< Link to node
    pnode->OSWaitNodeTCB    = osTCBCur;             //!< Link to task's TCB
    pnode->OSWaitNodeRes    = OS_STAT_PEND_OK;

    switch (pObjHead->OSObjType & OS_OBJ_PRIO_TYPE_MSK) {
        case OS_OBJ_PRIO_TYPE_BITMAP: {
            OS_EVENT       *pevent = (OS_EVENT *)pecb;
            
            
            pevent->OSEventGrp    |= 1u << osTCBCur->OSTCBY;       //!< Put task in event's waiting table
            pevent->OSEventTbl[y] |= 1u << osTCBCur->OSTCBX;
        }
        break;
        
        case OS_OBJ_PRIO_TYPE_LIST: {
            OS_FLAG       *pflag = (OS_FLAG *)pecb;
            OS_WAIT_NODE  *pNodeNext;
            
            
            pNodeNext = pflag->OSFlagWaitList;                  //!< Put task in flag's wait list
            pnode->OSWaitNodePrev   = NULL;
            pnode->OSWaitNodeNext   = pNodeNext;
            if (pNodeNext != NULL) {
                pNodeNext->OSWaitNodePrev = pnode;
            }
            pflag->OSFlagWaitList = pnode;
        }
        break;
        
        default:
        break;
    }

    osTCBCur->OSTCBDly        = timeout;                //!< Store timeout in task's TCB
    osTCBCur->OSTCBWaitNode   = pnode;                  //!< Store node in task's TCB
    
    osRdyTbl[y] &= (OS_PRIO)~(1u << osTCBCur->OSTCBX);       //!< Suspend current task
    if (osRdyTbl[y] == 0u) {
        osRdyGrp &= (OS_PRIO)~(1u << osTCBCur->OSTCBY);
    }
}

/*********************************************************************************************************
*                                  REMOVE TASK FROM EVENT WAIT LIST
*
* Description: Remove a task from an event's wait list.
*
* Arguments  : ptcb     is a pointer to the task to remove.
*
* Returns    : none
*
* Called by  : OS_FlagTaskRdy() os_flag.c
*              osFlagPend()     os_flag.c
*              OS_TaskDel()     os_task.c
*
* Note       : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

void    OS_EventTaskRemove (OS_TCB   *ptcb)
{
    OS_WAIT_NODE   *pnode  = ptcb->OSTCBWaitNode;
    OS_OBJ_HEAD    *pobj   = (OS_OBJ_HEAD *)pnode->OSWaitNodeECB;
    
    switch (pobj->OSObjType & OS_OBJ_PRIO_TYPE_MSK) {
        case OS_OBJ_PRIO_TYPE_BITMAP: {
            OS_EVENT   *pevent = pnode->OSWaitNodeECB;
            UINT8       y = ptcb->OSTCBY;


            pevent->OSEventTbl[y]  &= (OS_PRIO)~(1u << ptcb->OSTCBX);    //!< Remove task from event's wait table
            if (pevent->OSEventTbl[y] == 0u) {
                pevent->OSEventGrp &= (OS_PRIO)~(1u << ptcb->OSTCBY);
            }
        }
        break;
        
        case OS_OBJ_PRIO_TYPE_LIST: {
            OS_FLAG      *pflag;
            OS_WAIT_NODE *pNodePrev;
            OS_WAIT_NODE *pNodeNext;


            pNodePrev = pnode->OSWaitNodePrev;                      //!< Remove task from flag's wait list
            pNodeNext = pnode->OSWaitNodeNext;
            if (pNodePrev == NULL) {                             //!< Is it first node in wait list?
                pflag                 = pnode->OSWaitNodeECB;       //!< Yes...
                pflag->OSFlagWaitList = pNodeNext;                  //!< Update list for new 1st node
                if (pNodeNext != NULL) {
                    pNodeNext->OSWaitNodePrev = NULL;            //!< Link new 1st node PREV to NULL
                }
            } else {                                                //!< No, A node somewhere in the list
                pNodePrev->OSWaitNodeNext = pNodeNext;              //!< Link around the node to unlink
                if (pNodeNext != NULL) {                         //!< Is this the LAST node?
                    pNodeNext->OSWaitNodePrev = pNodePrev;          //!< No, Link around current node
                }
            }
        }
        break;
        
        default:
        break;
    }
    
    ptcb->OSTCBWaitNode = NULL;                          //!< Unlink event from OS_TCB
}

/*********************************************************************************************************
*                           MAKE TASK READY TO RUN BASED ON EVENT OCCURING
*
* Description: This function is called by other OS services and is used to make a task ready-to-run because
*              desired event occur.
*
* Arguments  : pevent      is a pointer to the event control block corresponding to the event.
*
*              pend_stat   is used to indicate the readied task's pending status:
*
*                          OS_STAT_PEND_OK      Task ready due to a event-set, not a timeout or
*                                               an abort.
*                          OS_STAT_PEND_ABORT   Task ready due to an abort(or event was deleted).
*
* Returns    : none
*
* Called by  : OSFlagsPost() os_flag.c
*              osFlagDelete  os_flag.c
*
* Note(s)    : 1) This function assumes that interrupts are disabled.
*              2) This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

OS_TCB *OS_EventTaskRdy(void   *pecb,
                        UINT8   pend_stat)
{
    OS_OBJ_HEAD    *pobj = (OS_OBJ_HEAD *)pecb;
    OS_TCB         *ptcb;
    OS_WAIT_NODE   *pnode;
    UINT8     y;
    UINT8     x;

    
    switch (pobj->OSObjType & OS_OBJ_PRIO_TYPE_MSK) {
        case OS_OBJ_PRIO_TYPE_BITMAP: {
            OS_EVENT *pevent = (OS_EVENT *)pecb;
            OS_PRIO   ptbl;
            UINT8     prio;

#if OS_MAX_PRIO_LEVELS <= 64u
            y    = OSUnMapTbl[pevent->OSEventGrp];              //!< Find HPT waiting for event
            ptbl = pevent->OSEventTbl[y];
            x    = OSUnMapTbl[ptbl];
            prio = (UINT8)((y << 3u) + x);                      //!< Find priority of HPT
#else
            if ((pevent->OSEventGrp & 0xFFu) != 0u) {           //!< Find HPT waiting for event
                y = OSUnMapTbl[ pevent->OSEventGrp & 0xFFu];
            } else {
                y = OSUnMapTbl[(OS_PRIO)(pevent->OSEventGrp >> 8u) & 0xFFu] + 8u;
            }
            ptbl = pevent->OSEventTbl[y];
            if ((ptbl & 0xFFu) != 0u) {
                x = OSUnMapTbl[ptbl & 0xFFu];
            } else {
                x = OSUnMapTbl[(OS_PRIO)(ptbl >> 8u) & 0xFFu] + 8u;
            }
            prio = (UINT8)((y << 4u) + x);                      //!< Find priority of HPT
#endif

            ptcb                 = osPrioTCBTbl[prio];      //!< Point to HPT's TCB
            pnode                = ptcb->OSTCBWaitNode;
        }
        break;
        
        case OS_OBJ_PRIO_TYPE_LIST: {
            OS_FLAG *pflag = (OS_FLAG *)pecb;
            
            
            pnode                = pflag->OSFlagWaitList;
            ptcb                 = pnode->OSWaitNodeTCB;    //!< Point to waiting task's TCB
            y                    = ptcb->OSTCBY;
        }
        break;
        
        default:
        break;
    }
        
    ptcb->OSTCBDly       = 0u;                          //!< Prevent osTimeTick() from readying task
    pnode->OSWaitNodeRes = pend_stat;                   //!< Set pend status of post or abort
    OS_EventTaskRemove(ptcb);                           //!< Remove this task from event wait table

    osRdyGrp    |=  1u << ptcb->OSTCBY;                    //!< Put task in the ready table
    osRdyTbl[y] |=  1u << ptcb->OSTCBX;
    return ptcb;
}

/*********************************************************************************************************
*                             INITIALIZE EVENT CONTROL BLOCK'S WAIT LIST
*
* Description: This function is called by other OS services to initialize the event wait list.
*
* Arguments  : pevent    is a pointer to the event control block allocated to the event.
*
* Returns    : none
*
* Note       : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
void    OS_EventWaitTableInit  (OS_EVENT *pevent)
{
    uint_fast8_t  i;


    pevent->OSEventGrp = 0u;                     //!< No task waiting on event
    for (i = 0u; i < OS_BITMAP_TBL_SIZE; i++) {
        pevent->OSEventTbl[i] = 0u;
    }
}
#endif

/*********************************************************************************************************
*                                             INITIALIZATION
*                           INITIALIZE THE FREE LIST OF EVENT CONTROL BLOCKS
*
* Description: This function is called by osInit() to initialize the free list of event control blocks.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

#if (OS_EVENT_EN) && (OS_MAX_EVENTS > 0u)
static void os_init_ecb_list (void)
{
    uint_fast16_t   i;
    OS_EVENT       *pevent;
    OS_EVENT      **ppevent;


    OS_MemClr((UINT8 *)&osEventFreeTbl[0], sizeof(osEventFreeTbl)); //!< Clear the event table

    ppevent = &osEventFreeList;
    for (i = 0u; i < OS_MAX_EVENTS; i++) {  //!< Init. list of free EVENT control blocks
        pevent   = &osEventFreeTbl[i];
        *ppevent = pevent;
        ppevent  = (OS_EVENT **)&pevent->OSEventPtr;
    }
}
#endif

/*********************************************************************************************************
*                                      CLEAR A SECTION OF MEMORY
*
* Description: This function is called by other OS services to clear a contiguous block of RAM.
*
* Arguments  : pdest    is the start of the RAM to clear (i.e. write 0x00 to)
*
*              size     is the number of bytes to clear.
*
* Returns    : none
*
* Notes      : 1) This function is INTERNAL to OS and your application should not call it.
*              2) Note that we can only clear up to 0xFFFFFFFF bytes of RAM at once.
*              3) The clear is done one byte at a time since this will work on any processor irrespective
*                 of the alignment of the destination.
*********************************************************************************************************/

void    OS_MemClr  (UINT8  *pdest,
                    size_t  size)
{
    while (size > 0u) {
        *pdest++ = 0u;
        size--;
    }
}

/*********************************************************************************************************
*                                       COPY A BLOCK OF MEMORY
*
* Description: This function is called by other OS services to copy a block of memory from one
*              location to another.
*
* Arguments  : pdest    is a pointer to the 'destination' memory block
*
*              psrc     is a pointer to the 'source'      memory block
*
*              size     is the number of bytes to copy.
*
* Returns    : none
*
* Notes      : 1) This function is INTERNAL to OS and your application should not call it.  There is
*                 no provision to handle overlapping memory copy.  However, that's not a problem since this
*                 is not a situation that will happen.
*              2) Note that we can only copy up to 0xFFFFFFFF bytes of RAM at once.
*              3) The copy is done one byte at a time since this will work on any processor irrespective
*                 of the alignment of the source and destination.
*********************************************************************************************************/

void    OS_MemCopy (UINT8  *pdest,
                    UINT8  *psrc,
                    size_t  size)
{
    while (size > 0u) {
        *pdest++ = *psrc++;
        size--;
    }
}

