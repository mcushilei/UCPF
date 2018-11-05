
#include ".\app_cfg.h"
#include ".\os_cpu_cfg.h"

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
*                            a 'FREE' wrap on the task stack.
*
*              wrap          is a pointer to function will be called when task return. You can omit this
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
*              2) All tasks run in Thread mode, using process stack.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit(OS_STK *ptos, void *wrap, void *task, void *parg)
{
    OS_STK *stk;


    stk       = ptos;                               //!< Load stack pointer

                                                    //!< Registers stacked as if auto-saved on exception
    *(stk)    = (uint32_t)0x01000000u;              //!< xPSR
    *(--stk)  = (uint32_t)wrap;                     //!< PC: wrap point of task
    *(--stk)  = (uint32_t)0xFFFFFFFFu;              //!< R14 (LR)
    *(--stk)  = (uint32_t)0x12121212u;              //!< R12
    *(--stk)  = (uint32_t)0x03030303u;              //!< R3
    *(--stk)  = (uint32_t)0x02020202u;              //!< R2
    *(--stk)  = (uint32_t)parg;                     //!< R1: argument of task
    *(--stk)  = (uint32_t)task;                     //!< R0: task function

                                                    //!< Remaining registers saved on thread's stack
    *(--stk)  = (uint32_t)0x11111111u;              //!< R11
    *(--stk)  = (uint32_t)0x10101010u;              //!< R10
    *(--stk)  = (uint32_t)0x09090909u;              //!< R9
    *(--stk)  = (uint32_t)0x08080808u;              //!< R8
    *(--stk)  = (uint32_t)0x07070707u;              //!< R7
    *(--stk)  = (uint32_t)0x06060606u;              //!< R6
    *(--stk)  = (uint32_t)0x05050505u;              //!< R5
    *(--stk)  = (uint32_t)0x04040404u;              //!< R4
    
    return stk;
}





static uint32_t SVC0_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3);
static uint32_t SVC1_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3);
static uint32_t SVC2_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3);

typedef uint32_t fnSVCHandle_t(uint32_t, uint32_t, uint32_t, uint32_t);

static const fnSVCHandle_t *SVC_ProcessFunArray[] = {
    SVC0_Process, SVC1_Process, SVC2_Process,
};

void SVC_Process(uint32_t *pstack)
{
    uint32_t svc_number = 0;
    uint32_t svc_r0 = 0;
    uint32_t svc_r1 = 0;
    uint32_t svc_r2 = 0;
    uint32_t svc_r3 = 0;
    uint32_t retVal = 0;
    
    svc_number = ((uint8_t *)pstack[6])[-2];
    svc_r0     = ((uint32_t) pstack[0]);
    svc_r1     = ((uint32_t) pstack[1]);
    svc_r2     = ((uint32_t) pstack[2]);
    svc_r3     = ((uint32_t) pstack[3]);
    
    if (svc_number < 
       (sizeof(SVC_ProcessFunArray) / sizeof(SVC_ProcessFunArray[0]))) {
        retVal = SVC_ProcessFunArray[svc_number](svc_r0, svc_r1, svc_r2, svc_r3);
    }
    
    pstack[0] = retVal;
}

//! reserved.
uint32_t SVC0_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3)
{
    return 0;
}

uint32_t SVC1_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3)
{
    extern void TASK_SW(void);
    extern void ENTER_CRITICAL(void);
    extern void EXIT_CRITICAL(void);
    
    switch (R0) {
        case 0:
            TASK_SW();
            break;
        case 1:
            ENTER_CRITICAL();
            break;
        case 2:
            EXIT_CRITICAL();
            break;
        default:
            break;
    }
    
    return 0;
}

uint32_t SVC2_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3)
{
    extern uint32_t SET_INTERRUPT_MASK(uint32_t);
    
    switch (R0) {
        case 0:
            break;
        case 1:
            return SET_INTERRUPT_MASK(R1);
        default:
            break;
    }
    
    return 0;
}
