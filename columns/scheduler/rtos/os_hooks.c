
#include ".\source\os.h"

/*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by osInit() at the beginning of osInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSInitHookBegin (void)
{
}
#endif

/*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by osInit() at the end of osInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSInitHookEnd (void)
{
}
#endif

/*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************/
#if OS_HOOKS_EN > 0u
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif

/*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSTaskIdleHook (void)
{
}
#endif

/*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by OS's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSTaskStatHook (void)
{
}
#endif

/*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'osTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'osTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************/
#if (OS_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void)
{
#if OS_TASK_PROFILE_EN > 0u
    osTCBHighRdy->OSTCBCtxSwCtr++;          //!< Inc. # of context switches to this task
#endif
#if OS_STAT_EN > 0u
    osCtxSwCtr++;                           //!< Increment context switch counter
#endif
}
#endif

/*********************************************************************************************************
*                                           OS_TCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************/
#if OS_HOOKS_EN > 0
void  OSTCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************/
#if (OS_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void)
{
}
#endif

