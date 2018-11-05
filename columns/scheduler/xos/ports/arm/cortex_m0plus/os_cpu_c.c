
#include ".\app_cfg.h"
#include ".\os_cpu_cfg.h"

//! those functions are in os_cpu_a.s
extern void     TRIGGER_PENDSV(void);
extern void     DISABLE_INTERRUPT(void);
extern void     ENABLE_INTERRUPT(void);
extern uint32_t OS_CPU_ATOM_CODE_BEGIN(void);
extern void     OS_CPU_ATOM_CODE_END(uint32_t);

/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

uint32_t OSCriticalNesting = 0;                      //!< count the nesting of overlapped ctrtical sections

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
*              2) All tasks run in THREAD mode, using process stack.
*********************************************************************************************************
*/

CPU_STK *OSTaskStkInit(CPU_STK *ptos, void *wrap, void *task, void *parg)
{
    CPU_STK *stk;


    stk       = ptos;                               //!< Load stack pointer

                                                    //!< Registers stacked as if auto-saved on exception
    *(stk)    = (uint32_t)0x01000000u;              //!< xPSR
    *(--stk)  = (uint32_t)wrap;                     //!< PC: wrap point of task
    *(--stk)  = (uint32_t)0xFFFFFFFFu;              //!< LR
    *(--stk)  = (uint32_t)0x12121212u;              //!< R12
    *(--stk)  = (uint32_t)0x03030303u;              //!< R3
    *(--stk)  = (uint32_t)0x02020202u;              //!< R2
    *(--stk)  = (uint32_t)parg;                     //!< R1: argument of task
    *(--stk)  = (uint32_t)task;                     //!< R0: task function

                                                    //!< Remaining registers saved on task's stack
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

static fnSVCHandle_t * const SVC_ProcessFunArray[] = {
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
    switch (R0) {
        case 0:
            TRIGGER_PENDSV();
            break;
            
        case 1:
            break;
            
        case 2:
            break;
            
        default:
            break;
    }
    
    return 0;
}

uint32_t SVC2_Process(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3)
{
    extern void DISABLE_ALL_INT(void);
    
    switch (R0) {
        case 0:
            DISABLE_INTERRUPT();
            break;
            
        case 1:
            break;
            
        default:
            break;
    }
    
    return 0;
}

#if     OS_CRITICAL_METHOD == 3
#elif   OS_CRITICAL_METHOD == 2
void os_cpu_enter_critical_section(void)
{
    DISABLE_INTERRUPT();
    if (OSCriticalNesting <= 255u) {
        OSCriticalNesting++;
    }
}

void os_cpu_exit_critical_section(void)
{
    if (OSCriticalNesting != 0u) {
        OSCriticalNesting--;
    }
    if (OSCriticalNesting == 0u) {
        ENABLE_INTERRUPT();
    }
}
#endif
