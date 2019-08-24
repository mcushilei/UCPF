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
#define __OS_SCHEDULER_C__

/*============================ INCLUDES ======================================*/
#include ".\os_private.h"
#include ".\os_port.h"
#include <string.h>

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
        list_init(&osRdyList[i]);
    }
    memset((char *)&osRdyBitmap, 0, sizeof(osRdyBitmap));

    osTCBCur      = NULL;
    osTCBNextRdy  = NULL;
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
    list_insert(&ptcb->OSTCBList, osRdyList[ptcb->OSTCBPrio].Prev); //!< add task to the end of ready task list.
    OS_BitmapSet(&osRdyBitmap, ptcb->OSTCBPrio);                    //!< set the flay to indicate that there are some threads ready to run.
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
    
    list_remove(&ptcb->OSTCBList);
    if (osRdyList[prio].Prev == &osRdyList[prio]) { //!< Is osRdyList empty?
        OS_BitmapClr(&osRdyBitmap, prio);           //!< Yes, clear the ready flag of this priority.
    }
}

/*!
 *! \Brief       FIND HIGHEST PRIORITY TASK IN TEH READY TABLE
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
        list_remove(node);
        list_insert(node, osRdyList[prio].Prev);
        osTCBNextRdy = CONTAINER_OF(node, OS_TCB, OSTCBList);
    }
}

/*!
 *! \Brief       FIND NEXT READY TASK
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
    list_remove(node);
    list_insert(node, osRdyList[prio].Prev);
    osTCBNextRdy = CONTAINER_OF(node, OS_TCB, OSTCBList);
}

/*!
 *! \Brief       MKAE THE HIGHEST PRIORITY READY TASK RUN
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
    if (osIntNesting != 0u) {                   //!< Can not be used in ISR and ...
        return;
    }
    
    if (osLockNesting != 0u) {                  //!< ... scheduler is not locked
        return;
    }

    OSEnterCriticalSection();
    OS_SchedulerPrio();
    if (osTCBNextRdy != osTCBCur) {             //!< in case that current task running has the highest priority.
        OSExitCriticalSection();
        OSCtxSw();
        return;
    }
    OSExitCriticalSection();
}

/*!
 *! \Brief       MAKE NEXT READY TASK RUN
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
    if (osIntNesting != 0u) {                   //!< Can not be used in ISR and ...
        return;
    }
    
    if (osLockNesting != 0u) {                  //!< ... scheduler is not locked
        return;
    }

    OSEnterCriticalSection();
    OS_SchedulerNext();
    if (osTCBNextRdy != osTCBCur) {
        OSExitCriticalSection();
        OSCtxSw();                              //!< Perform a context switch
        return;
    }
    OSExitCriticalSection();
}


/* EOF */
