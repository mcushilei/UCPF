
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
    UINT16  i;


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

void os_sched_ready_task(OS_TCB *ptcb)
{
    UINT8 prio, g;
    OS_PRIO x, y;
    
    prio = ptcb->OSTCBPrio;
    
#if OS_MAX_PRIO_LEVELS <= 64u                        //!< See if we support up to 64 tasks
    g = (prio >> 3) & 0x07u;
    x = 1u << (prio & 0x07u);
#else
    g = (prio >> 4) & 0x0Fu;
    x = 1u << (prio & 0x0Fu);
#endif
    y = 1u << g;
    
    osRdyGrp    |= y;                  //!< Make this priority has task ready-to-run.
    osRdyTbl[g] |= x;
}

void os_sched_unready_task(OS_TCB *ptcb)
{
    UINT8 prio, g;
    OS_PRIO x, y;
    
    prio = ptcb->OSTCBPrio;
    
#if OS_MAX_PRIO_LEVELS <= 64u                        //!< See if we support up to 64 tasks
    g = (prio >> 3) & 0x07u;
    x = 1u << (prio & 0x07u);
#else
    g = (prio >> 4) & 0x0Fu;
    x = 1u << (prio & 0x0Fu);
#endif
    y = 1u << g;

    osRdyTbl[g] &= (OS_PRIO)~x;
    if (osRdyTbl[g] == 0u) {
        osRdyGrp &= (OS_PRIO)~y;
    }
}

