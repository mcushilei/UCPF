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


#include ".\source\os_private.h"

/*!
 *! \Brief       OS INITIALIZATION HOOK (BEGINNING)
 *!
 *! \Description This function is called by osInit() at the beginning of osInit().
 *!
 *! \Arguments   none
 *!
 *! \Notes       1) Interrupts should be disabled during this call.
 */
#if OS_HOOKS_EN > 0
void OSInitHookBegin(void)
{
}
#endif

/*!
 *! \Brief       OS INITIALIZATION HOOK (END)
 *!
 *! \Description This function is called by osInit() at the end of osInit().
 *!
 *! \Arguments   none
 *!
 *! \Notes       1) Interrupts should be disabled during this call.
 */
#if OS_HOOKS_EN > 0
WEAK void OSInitHookEnd(void)
{
}
#endif

/*!
 *! \Brief       TASK CREATION HOOK
 *!
 *! \Description This function is called when a task is created.
 *!
 *! \Arguments   ptcb   is a pointer to the task control block of the task being created.
 *!
 *! \Notes       1) Interrupts are disabled during this call.
 */
#if OS_HOOKS_EN > 0
void OSTaskCreateHook(OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*!
 *! \Brief       TASK RETURN HOOK
 *!
 *! \Description This function is called if a task returns. The task will be deleted if OS_TASK_DEL_EN
 *!              is set, in this case this hook can be used to free any dynamic resouce when the task
 *!              is created. Caution: hooks are ment to extent OS function so the app code
 *!              should NOT rely on this mechanism.
 *!
 *! \Arguments   ptcb       is a pointer to the task control block of the task that returned.
 *!
 *!              parg       this value is returned by the task. NOTE: it is a pointer type!
 *!
 *! \Notes       none
 */
#if OS_HOOKS_EN > 0u
void OSTaskReturnHook(OS_TCB *ptcb, void *parg)
{
    (void)ptcb;
    (void)parg;
}
#endif

/*!
 *! \Brief       IDLE TASK HOOK
 *!
 *! \Description This function is called by the idle task.  This hook has been added to allow you to do
 *!              such things as STOP the CPU to conserve power.
 *!
 *! \Arguments   none
 *!
 *! \Notes       1) Interrupts are enabled during this call.
 */
#if OS_HOOKS_EN > 0
void OSTaskIdleHook(void)
{
}
#endif

/*!
 *! \Brief       STATISTIC TASK HOOK
 *!
 *! \Description This function is called every second by OS's statistics task.  This allows your
 *!              application to add functionality to the statistics task.
 *!
 *! \Arguments   none
 */
#if OS_HOOKS_EN > 0
void OSTaskStatHook(void)
{
}
#endif

/*!
 *! \Brief       TASK SWITCH HOOK
 *!
 *! \Description This function is called when a task switch is performed.  This allows you to perform other
 *!              operations during a context switch.
 *!
 *! \Arguments   none
 *!
 *! \Notes       1) Interrupts are disabled during this call.
 *!              2) It is assumed that the global pointer 'osTCBNextRdy' points to the TCB of the task that
 *!                 will be 'switched in' (i.e. the highest priority task) and, 'osTCBCur' points to the
 *!                 task being switched out (i.e. the preempted task).
 */
#if (OS_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void OSTaskSwHook(void)
{
#if OS_TASK_PROFILE_EN > 0u
    osTCBNextRdy->OSTCBCtxSwCtr++;          //!< Inc. # of context switches to this task
#endif
#if OS_STAT_EN > 0u
    osCtxSwCtr++;                           //!< Increment context switch counter
#endif
}
#endif

/*!
 *! \Brief       OS_TCBInit() HOOK
 *!
 *! \Description This function is called by OS_TCBInit() after setting up most of the TCB.
 *!
 *! \Arguments   ptcb    is a pointer to the TCB of the task being created.
 *!
 *! \Notes       1) Interrupts may or may not be ENABLED during this call.
 */
#if OS_HOOKS_EN > 0
void OSTCBInitHook(OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*!
 *! \Brief       TICK HOOK
 *!
 *! \Description This function is called every tick.
 *!
 *! \Arguments   none
 *!
 *! \Notes       1) Interrupts may or may not be ENABLED during this call.
 */
#if (OS_HOOKS_EN > 0) && (OS_SYS_TICK_HOOK_EN > 0)
WEAK void OSSysTickHook(void)
{
}
#endif

