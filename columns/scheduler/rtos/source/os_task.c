
#include ".\os.h"

/*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************/

static void os_task_del        (void);

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr    (OS_STK         *pbos,
                                UINT32          size,
                                UINT8           opt);
#endif

static void os_task_wrapper    (void           *ptask,
                                void           *parg);

/*********************************************************************************************************
*                                      CHANGE PRIORITY OF A TASK
*
* Description: This function allows you to change the priority of a task dynamically.  Note that the new
*              priority MUST be available.
*
* Arguments  : ptcb     pointer to tcb
*
*              newp     is the new priority
*
* Returns    : OS_ERR_NONE            is the call was successful
*              OS_ERR_INVALID_TASK_HANDLE  ptcb is NULL.
*              OS_ERR_INVALID_PRIO    if the priority you specify is higher that the maximum allowed
*              OS_ERR_TASK_EXIST      if the new priority already has been specified to a task.
*              OS_ERR_TASK_NOT_EXIST  there is no task with the specified OLD priority (i.e. the OLD task does
*                                     not exist.
*********************************************************************************************************/

OS_ERR  osTaskChangePrio   (OS_HANDLE   handle,
                            UINT8       newprio)
{
    OS_TCB     *ptcb = (OS_TCB *)handle;
#if OS_CRITICAL_METHOD == 3u                //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif
    
    
#if OS_ARG_CHK_EN > 0u
    if (ptcb == NULL) {
        return OS_ERR_INVALID_TASK_HANDLE;
    }
#if OS_MAX_PRIO_LEVELS <= 255
    if (newprio >= OS_MAX_PRIO_LEVELS) {
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif

    OSEnterCriticalSection(cpu_sr);
    if (osPrioTCBTbl[newprio] != NULL) {             //!< New priority must not already exist.
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TASK_EXIST;
    }
    if (ptcb->OSTCBOwnMutex != NULL) {
        ptcb->OSTCBOwnMutex->OSEventOwnerPrio = newprio;
        if (newprio < ptcb->OSTCBPrio) {
            OS_TaskChangePrio(ptcb, newprio);
        }
    } else {
        OS_TaskChangePrio(ptcb, newprio);
    }
    OSExitCriticalSection(cpu_sr);
    
    if (osRunning) {
        OS_Schedule();                                  //!< Find new highest priority task
    }
    
    return OS_ERR_NONE;
}

/*********************************************************************************************************
*                                           CREATE A TASK
*
* Description: This function is used to have OS manage the execution of a task.  Tasks can either
*              be created prior to the start of multitasking or by a running task.  A task cannot be
*              created by an ISR.
*
* Arguments  : pHandle  is a pointer to the task's handle
*
*              task     is a pointer to the task's code
*
*              parg     is a pointer to an optional data area which can be used to pass parameters to
*                        the task when the task first executes.  Where the task is concerned it thinks
*                        it was invoked and passed the argument 'parg' as follows:
*
*                            void task (void *parg)
*                            {
*                                for (;;) {
*                                    Task code;
*                                }
*                            }
*
*              prio      is the task's priority.  A unique priority MUST be assigned to each task and the
*                        lower the number, the higher the priority.
*
*              pstk      point to the LOWEST (valid) memory location of the stack.
*
*              stkSize   is the size of the stack in number of elements.  If OS_STK is set to UINT8,
*                        'stkSize' corresponds to the number of bytes available.  If OS_STK is set to
*                        UINT32, 'stkSize' contains the number of 32-bit entries available on the stack.
*
*              opt       contains additional information (or options) about the behavior of the task.  The
*                        LOWER 8-bits are reserved by OS while the upper 8 bits can be application
*                        specific.  See OS_TASK_OPT_??? in OS.H.  Current choices are:
*
*                        OS_TASK_OPT_STK_CHK      Stack checking to be allowed for the task
*                        OS_TASK_OPT_STK_CLR      Clear the stack when the task is created
*                        OS_TASK_OPT_SAVE_FP      If the CPU has floating-point registers, save them
*                                                 during a context switch.
*
* Returns    : OS_ERR_NONE              if the function was successful.
*              OS_ERR_TASK_EXIST        if the task priority already exist
*                                       (each task MUST have a unique priority).
*              OS_ERR_INVALID_PRIO      if the priority you specify is higher that the maximum
*              OS_ERR_CREATE_ISR        if you tried to create a task from an ISR.
*********************************************************************************************************/

OS_ERR  osTaskCreate(   OS_HANDLE  *pHandle,
                        OS_TASK    *task,
                        void       *parg,
                        UINT8       prio,
                        OS_STK     *pstk,
                        UINT32      stkSize,
                        UINT8       opt)
{
    OS_TCB     *ptcb;
    OS_STK     *psp;
#if OS_CRITICAL_METHOD == 3u                //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    if (osIntNesting > 0u) {                //!< Make sure we don't create the task from within an ISR
        return OS_ERR_CREATE_ISR;
    }
#if OS_ARG_CHK_EN > 0u
#if OS_MAX_PRIO_LEVELS <= 255
    if (prio >= OS_MAX_PRIO_LEVELS) {       //!< Make sure priority is within allowable range
        return OS_ERR_INVALID_PRIO;
    }
#endif
#endif
    
    OSEnterCriticalSection(cpu_sr);
    if (osPrioTCBTbl[prio] == NULL) {        //!< Make sure task doesn't already exist at this priority
        ptcb = osTCBFreeList;                   //!< Get a free TCB from the free TCB list
        if (ptcb == NULL) {                  //!< See if pool of free TCB pool was empty
            OSExitCriticalSection(cpu_sr);
            return OS_ERR_EVENT_DEPLETED;       //!< No more task control blocks
        }
        osTCBFreeList = ptcb->OSTCBNext;

        osPrioTCBTbl[prio] = OS_TCB_RESERVED;   //!< Lock the priority to prevent others from doing ...
                                                //!< ... the same thing until task is created.
        OSExitCriticalSection(cpu_sr);

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
        os_task_stk_clr(pstk, stkSize, opt);    //!< Clear the task stack (if needed)
#endif

#if OS_STK_GROWTH_DOWN == 1u                            //!< Initialize the task's stack
        if (stkSize != 0u) {
            psp = OSTaskStkInit(pstk + stkSize - 1u, (void *)&os_task_wrapper, (void *)task, parg);
        } else {
            psp = OSTaskStkInit(pstk,                (void *)&os_task_wrapper, (void *)task, parg);
        }
#else
        psp = OSTaskStkInit(pstk, (void *)&os_task_wrapper, (void *)task, parg);
#endif
        
        OS_TCBInit(ptcb, prio, psp, pstk, stkSize, opt);
#if OS_CPU_HOOKS_EN > 0u
        OSTaskCreateHook(ptcb);
#endif

        OSEnterCriticalSection(cpu_sr);
        osPrioTCBTbl[prio] = ptcb;                  //!< Add task to prio table
        
        ptcb->OSTCBNext = osTaskList;               //!< Link into active TCB list.
        ptcb->OSTCBPrev = NULL;
        if (osTaskList != NULL) {
            osTaskList->OSTCBPrev = ptcb;
        }
        osTaskList = ptcb;

        osRdyGrp               |= 1u << ptcb->OSTCBY;  //!< Set task to ready.
        osRdyTbl[ptcb->OSTCBY] |= 1u << ptcb->OSTCBX;
        
        OSExitCriticalSection(cpu_sr);
        
        if (pHandle != NULL) {
            *pHandle = ptcb;
        }
        if (osRunning) {
            OS_Schedule();
        }
        return OS_ERR_NONE;
    } else {
        OSExitCriticalSection(cpu_sr);
        return OS_ERR_TASK_EXIST;
    }
}

/*********************************************************************************************************
*                                          DELETE CURRENT TASK
*
* Description: This function delete current running task. The deleted task is returned to the dormant
*              state and can be re-activated by creating the deleted task again. This function is
*              internal to OS and called by os_task_wrapper. Your application should terminate a task
*              by return.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/

static void os_task_del(void)
{
    UINT8       prio;
    OS_TCB     *ptcb;
#if OS_CRITICAL_METHOD == 3u                            //!< Allocate storage for CPU status register
    OS_CPU_SR   cpu_sr = 0u;
#endif


    OSEnterCriticalSection(cpu_sr);
    prio = osTCBCur->OSTCBPrio;
    ptcb = osPrioTCBTbl[prio];

    osRdyTbl[ptcb->OSTCBY] &= (OS_PRIO)~(1u << ptcb->OSTCBX);//!< Make task not ready
    if (osRdyTbl[ptcb->OSTCBY] == 0u) {
        osRdyGrp &= (OS_PRIO)~(1u << ptcb->OSTCBY);
    }

    if (ptcb->OSTCBWaitNode != NULL) {               //!< Remove this task from any event.
        OS_EventTaskRemove(ptcb);
    }

    ptcb->OSTCBDly      = 0u;                           //!< Prevent osTimeTick() from updating
    
    osPrioTCBTbl[prio] = NULL;                       //!< Clear old priority entry
    
    if (ptcb->OSTCBPrev == NULL) {                   //!< Remove from task list
        ptcb->OSTCBNext->OSTCBPrev = NULL;
        osTaskList                 = ptcb->OSTCBNext;
    } else {
        ptcb->OSTCBPrev->OSTCBNext = ptcb->OSTCBNext;
        ptcb->OSTCBNext->OSTCBPrev = ptcb->OSTCBPrev;
    }

    ptcb->OSTCBPrev     = NULL;
    ptcb->OSTCBNext     = osTCBFreeList;                //!< Return TCB to free TCB list
    osTCBFreeList       = ptcb;
    OSExitCriticalSection(cpu_sr);
    OS_Schedule();                                      //!< Find new highest priority task
}

/*********************************************************************************************************
*                                    CATCH ACCIDENTAL TASK RETURN
*
* Description: This function is wrapper of a task.  It mainly handles the return of the task and deletes
*              it after the task function returns.
*
* Arguments  : ptask         is a pointer to the task function.
*
*              parg          is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to OS and your application should not call it.
*********************************************************************************************************/

static void os_task_wrapper(void *ptask, void *parg)
{
    (void)((OS_TASK *)ptask)(parg);
    
#if OS_CPU_HOOKS_EN > 0
    OSTaskReturnHook(osTCBCur);     //!< Call hook to let user decide on what to do
#endif

    os_task_del();                  //!< Delete task if it returns!
}

/*********************************************************************************************************
*                                          CLEAR TASK STACK
*
* Description: This function is used to clear the stack of a task (i.e. write all zeros)
*
* Arguments  : pbos     is a pointer to the task's bottom of stack.  If the configuration constant
*                       OS_STK_GROWTH_DOWN is set to 1, the stack is assumed to grow downward (i.e. from high
*                       memory to low memory).  'pbos' will thus point to the lowest (valid) memory
*                       location of the stack.  If OS_STK_GROWTH_DOWN is set to 0, 'pbos' will point to the
*                       highest memory location of the stack and the stack will grow with increasing
*                       memory locations.  'pbos' MUST point to a valid 'free' data item.
*
*              size     is the number of 'stack elements' to clear.
*
*              opt      contains additional information (or options) about the behavior of the task.  The
*                       LOWER 8-bits are reserved by OS while the upper 8 bits can be application
*                       specific.  See OS_TASK_OPT_??? in OS.H.
*
* Returns    : none
*********************************************************************************************************/

#if (OS_STAT_TASK_STK_CHK_EN > 0u)
static void os_task_stk_clr(OS_STK  *pstk,
                            UINT32   size,
                            UINT8    opt)
{
    if ((opt & OS_TASK_OPT_STK_CHK) != 0x00u) {      //!< See if stack checking has been enabled
        if ((opt & OS_TASK_OPT_STK_CLR) != 0x00u) {  //!< See if stack needs to be cleared
            while (size > 0u) {
                size--;
                *pstk++ = (OS_STK)0;
            }
        }
    }
}
#endif
