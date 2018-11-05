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
#define __OS_SCHEDULER_C__

/*============================ INCLUDES ======================================*/
#include ".\os.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

/*!
 *! \Brief       INITIALIZE THE READY LIST
 *!
 *! \Description This function is called by osInit() to initialize the Ready List.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 */
void OS_SchedulerInit(void)
{
    UINT16 i;


    for (i = 0u; i < OS_MAX_PRIO_LEVELS; i++) {
        os_list_init_head(&osRdyList[i]);
    }
    OS_MemClr((char *)&osRdyBitmap, sizeof(osRdyBitmap));

    osTCBCur      = NULL;
    osTCBNextRdy  = NULL;
}

void OS_LockSched(void)
{
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    CPU_SR  cpu_sr = 0u;
#endif


    if (osRunning != FALSE) {                   //!< Make sure multitasking is running
        if (osIntNesting == 0u) {               //!< Can't call from an ISR
            OSEnterCriticalSection(cpu_sr);
            if (osLockNesting < 255u) {         //!< Prevent osLockNesting from wrapping back to 0
                osLockNesting++;                //!< Increment lock nesting level
            }
            OSExitCriticalSection(cpu_sr);
        }
    }
}

void OS_UnlockSched(void)
{
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    CPU_SR  cpu_sr = 0u;
#endif


    if (osRunning != FALSE) {                   //!< Make sure multitasking is running
        if (osIntNesting == 0u) {               //!< Can't call from an ISR
            OSEnterCriticalSection(cpu_sr);
            if (osLockNesting > 0u) {           //!< Do not decrement if already 0
                osLockNesting--;                //!< Decrement lock nesting level
            }
            OSExitCriticalSection(cpu_sr);
        }
    }
}

/*!
 *! \Brief       READY TASK TO RUN
 *!
 *! \Description This function add task in scheduler's ready list and make it can be
 *!              scheduled.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Interrupts are assumed to be DISABLED when this function is called.
 */
void OS_SchedulerReadyTask(OS_TCB *ptcb)
{
    os_list_add(&ptcb->OSTCBList, osRdyList[ptcb->OSTCBPrio].Prev); //!< add task to the end of ready task list.
    OS_BitmapSet(&osRdyBitmap, ptcb->OSTCBPrio);
}

/*!
 *! \Brief       READY TASK TO RUN
 *!
 *! \Description This function remove task from scheduler's ready list, so the task can not be
 *!              scheduled.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Interrupts are assumed to be DISABLED when this function is called.
 */
void OS_SchedulerUnreadyTask(OS_TCB *ptcb)
{
    UINT8 prio;
    
    
    prio = ptcb->OSTCBPrio;
    
    os_list_del(&ptcb->OSTCBList);
    if (osRdyList[prio].Prev == &osRdyList[prio]) { //!< Is this list empty?
        OS_BitmapClr(&osRdyBitmap, prio);           //!< Yes, clear the flag.
    }
}

/*!
 *! \Brief       FIND HIGHEST PRIORITY TASK READY TO RUN
 *!
 *! \Description This function try determining the task that has the highest priority to run.
 *!              It will not determine the next task if currnt priority is the hightest.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Interrupts are assumed to be DISABLED when this function is called.
 */
void OS_SchedulerPrio(void)
{
    UINT8           prio;
    OS_LIST_NODE   *node;


    prio = OS_BitmapGetHigestPrio(&osRdyBitmap);
    if (prio != osTCBCur->OSTCBPrio) {
        node = osRdyList[prio].Next;
        //! move this task to the end of the ready queue.
        os_list_del(node);
        os_list_add(node, osRdyList[prio].Prev);
        osTCBNextRdy = OS_CONTAINER_OF(node, OS_TCB, OSTCBList);
    }
}

/*!
 *! \Brief       FIND NEXT TASK READY TO RUN
 *!
 *! \Description This function is called by other OS services to determine the next ready task to
 *!              run. The next task's priority might be the same with current task's.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Interrupts are assumed to be DISABLED when this function is called.
 */
void OS_SchedulerNext(void)
{
    UINT8           prio;
    OS_LIST_NODE   *node;


    prio = OS_BitmapGetHigestPrio(&osRdyBitmap);
    node = osRdyList[prio].Next;
    //! move this task to the end of the ready queue.
    os_list_del(node);
    os_list_add(node, osRdyList[prio].Prev);
    osTCBNextRdy = OS_CONTAINER_OF(node, OS_TCB, OSTCBList);
}

/*!
 *! \Brief       FIND HIGHEST PRIORITY TASK READY AND RUN IT
 *!
 *! \Description This function try determining the task that has the highest priority to run.
 *!              It will not determine the next task if currnt priority is the hightest.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Rescheduling is prevented when the scheduler is locked (see OS_SchedLock())
 */
void OS_SchedulerRunPrio(void)
{
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif

    if (osIntNesting != 0u) {                   //!< Can not be used in ISR and ...
        return;
    }
    
    if (osLockNesting != 0u) {                  //!< ... scheduler is not locked
        return;
    }

    OSEnterCriticalSection(cpu_sr);
    OS_SchedulerPrio();
    if (osTCBNextRdy != osTCBCur) {
        OSExitCriticalSection(cpu_sr);
        OSCtxSw();                              //!< Perform a context switch
        return;
    }
    OSExitCriticalSection(cpu_sr);
}

/*!
 *! \Brief       FIND NEXT TASK READY AND RUN IT
 *!
 *! \Description This function is called by other OS services to determine the next ready task to
 *!              run, beacuse current task has been pend. The next task's priority might be the
 *!              same with current task's.
 *!
 *! \Arguments   none
 *!
 *! \Returns     none
 *!
 *! \Notes       1) This function is INTERNAL to OS and your application should not call it.
 *!              2) Rescheduling is prevented when the scheduler is locked (see OS_SchedLock())
 */
void OS_SchedulerRunNext(void)
{
#if OS_CRITICAL_METHOD == 3u                    //!< Allocate storage for CPU status register
    CPU_SR   cpu_sr = 0u;
#endif

    if (osIntNesting != 0u) {                   //!< Can not be used in ISR and ...
        return;
    }
    
    if (osLockNesting != 0u) {                  //!< ... scheduler is not locked
        return;
    }

    OSEnterCriticalSection(cpu_sr);
    OS_SchedulerNext();
    if (osTCBNextRdy != osTCBCur) {
        OSExitCriticalSection(cpu_sr);
        OSCtxSw();                              //!< Perform a context switch
        return;
    }
    OSExitCriticalSection(cpu_sr);
}


/* EOF */
