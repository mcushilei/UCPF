
#include ".\app_cfg.h"
#include ".\os_cpu_h.h"



/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

uint32_t OSCriticalNesting = 0;                      //!<  关中断计数器（开关中断要判断此值）

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either osTaskCreate() or osTaskCreate() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : ptos          is a pointer to the TOP of stack. It is assumed that 'ptos' points to
*                            the first accessable memory location.
*
*              pret          is a pointer to function will be called when task return. You can omit this
*                            if you do not implement this.
*
*              task          is a pointer to the task code
*
*              parg          is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*              2) All tasks run in THREAD mode, using process stack.
*********************************************************************************************************
*/

CPU_STK *OSTaskStkInit(CPU_STK *ptos, void *wrapper, void *task, void *parg)
{
    CPU_STK *stk;


    stk       = ptos;                               //!< Load stack pointer
    
    //! Create a stack frame as entering into an interrupt.

                                                    //!< ARM Cortex-M has a full-descending stack.
    *(  stk)  = (uint32_t)0x01000000;               //!< xPSR
    *(--stk)  = (uint32_t)wrapper;                  //!< PC: the task wrapper.
    *(--stk)  = (uint32_t)0x0D0D0D0D;               //!< R14 (LR)
    *(--stk)  = (uint32_t)0x0C0C0C0C;               //!< R12
    *(--stk)  = (uint32_t)0x03030303;               //!< R3
    *(--stk)  = (uint32_t)0x02020202;               //!< R2
    *(--stk)  = (uint32_t)parg;                     //!< R1: the argument passed to task.
    *(--stk)  = (uint32_t)task;                     //!< R0: the task entry.

    *(--stk)  = (uint32_t)0xFFFFFFFD;               //!< EXEC_RETURN: to select whether PSP or MSP to use and specify the CPU mode.
                                                    //!  Refer to DUI0552A on page 2-27, table 2-17.

    *(--stk)  = (uint32_t)0x0B0B0B0B;               //!< R11
    *(--stk)  = (uint32_t)0x0A0A0A0A;               //!< R10
    *(--stk)  = (uint32_t)0x09090909;               //!< R9
    *(--stk)  = (uint32_t)0x08080808;               //!< R8
    *(--stk)  = (uint32_t)0x07070707;               //!< R7
    *(--stk)  = (uint32_t)0x06060606;               //!< R6
    *(--stk)  = (uint32_t)0x05050505;               //!< R5
    *(--stk)  = (uint32_t)0x04040404;               //!< R4
    
    return stk;
}

void OSEnterCriticalSection(void)
{
//	__set_BASEPRI( OS_CPU_CFG_HIGHEST_INTERRUPT_PRIORITY_USED );
//	__DSB();
//	__ISB();
    __disable_irq();
    OSCriticalNesting++;
}

void OSExitCriticalSection(void)
{
	OSCriticalNesting--;
	if ( OSCriticalNesting == 0u ) {
//		__set_BASEPRI( 0 );
        __enable_irq();
	}
}

CPU_REG OSDisableInterrupt(void)
{
    CPU_REG current = __get_BASEPRI();
    __set_BASEPRI( OS_CPU_CFG_HIGHEST_INTERRUPT_PRIORITY_CALLED_FROM );
	__DSB();
	__ISB();
    return current;
}

void OSResumeInterrupt(CPU_REG level)
{
	__set_BASEPRI( level );
	__DSB();
	__ISB();
}

