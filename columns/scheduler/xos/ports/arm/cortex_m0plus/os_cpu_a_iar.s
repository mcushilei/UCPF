;********************************************************************************************************
; For       : ARMv7 Cortex-M0plus
; Toolchain : IAR EWARM
;********************************************************************************************************

;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

    EXTERN  osRunning
    EXTERN  osTCBCur
    EXTERN  osTCBNextRdy
    EXTERN  OSIntNesting
    EXTERN  OSIntExit
    EXTERN  OSTaskSwHook
    EXTERN  OSCriticalNesting
    EXTERN  SVC_Process

    PUBLIC  DISABLE_INTERRUPT
    PUBLIC  ENABLE_INTERRUPT
    PUBLIC  OS_CPU_ATOM_CODE_BEGIN
    PUBLIC  OS_CPU_ATOM_CODE_END
    PUBLIC  CALL_SVC1
    PUBLIC  CALL_SVC2
    PUBLIC  TRIGGER_PENDSV
    PUBLIC  OSStartTheFirst
    PUBLIC  OSCtxSw
    PUBLIC  OSIntCtxSw
    PUBLIC  SVC_Handler
    PUBLIC  PendSV_Handler

;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

SCB_ICSR_REG                    EQU     0xE000ED04              ; Interrupt control state register.
SCB_ICSR_REG_PENDSVSET_MSK      EQU     0x10000000              ; Value to trigger PendSV exception.
SCB_VTOR_REG                    EQU     0xE000ED08
SCB_SHPR2_REG                   EQU     0xE000ED1C  
SCB_SHPR3_REG                   EQU     0xE000ED20
SVC_PRI_VALUE 	                EQU           0x00              ; SVC priority value (highest).
PENDSV_PRI_VALUE                EQU           0xFF              ; PendSV priority value (lowest).
SYSTICK_PRI_VALUE               EQU           0xFF

;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

    SECTION .text:CODE:NOROOT(2)
;	AREA |.text|, CODE, READONLY, ALIGN=2

	THUMB
	REQUIRE8
	PRESERVE8





;********************************************************************************************************
;                                         void DISABLE_INTERRUPT(void)
;********************************************************************************************************

DISABLE_INTERRUPT
    CPSID   i
    BX      LR

;********************************************************************************************************
;                                         void ENABLE_INTERRUPT(void)
;********************************************************************************************************

ENABLE_INTERRUPT
    CPSIE   i
    BX      LR

;********************************************************************************************************
;                                         uint32_t OS_CPU_ATOM_CODE_BEGIN(void)
;********************************************************************************************************

OS_CPU_ATOM_CODE_BEGIN
    MRS     R0, PRIMASK
    CPSID   i
    BX      LR

;********************************************************************************************************
;                                         void OS_CPU_ATOM_CODE_END(uint32_t)
;********************************************************************************************************

OS_CPU_ATOM_CODE_END
    MOVS    R1, #0x00000001
    ANDS    R0, R0, R1
	BNE     __OS_CPU_ATOM_CODE_END_end
	CPSIE   i
__OS_CPU_ATOM_CODE_END_end                       	          
    BX      LR
    



;********************************************************************************************************
;                                           void TRIGGER_PENDSV(void)
; Note(s) : 1) This function is used by SVC1
;********************************************************************************************************

TRIGGER_PENDSV	    	    	    	    	            
	LDR     R0, =SCB_ICSR_REG                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =SCB_ICSR_REG_PENDSVSET_MSK
    STR     R1, [R0]
    BX      LR


;********************************************************************************************************
;  请求1号SVC调用
;********************************************************************************************************

CALL_SVC1
    SVC     #1
    BX      LR

;********************************************************************************************************
;  请求2号SVC调用
;********************************************************************************************************

CALL_SVC2
    SVC     #2
    BX      LR

;********************************************************************************************************
;                                           HANDLE SVC EXCEPTION
; Note(s) : 1) This is a paper wrapping. All actural process will be done in SVC_Process().
;********************************************************************************************************

SVC_Handler
    MOVS    R0, #0x04            ;to see which stack was used before we entering SVC by checking EXC_RETURN stored in LR.
    MOV     R1, LR
    TST     R1, R0
    BNE     __SVC_Handler_store_psp
    MRS     R0, MSP
    B       __SVC_Handler_run_SVC_Process
__SVC_Handler_store_psp
    MRS     R0, PSP
__SVC_Handler_run_SVC_Process
    PUSH    {LR}
    BL      SVC_Process
    POP     {PC}



;********************************************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch outside an ISR.
;              This function just triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

OSCtxSw
    PUSH    {LR}                                    ; Save LR(EXC_RETURN value)
    LDR     R0, =0
    LDR     R2, =CALL_SVC1                          ; Call: CALL_SVC1(0)
    BLX     R2
    POP     {PC}

;********************************************************************************************************
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;                                         void OSIntCtxSw(void)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed.
;              This function just triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

OSIntCtxSw
    LDR     R0, =SCB_ICSR_REG                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =SCB_ICSR_REG_PENDSVSET_MSK
    STR     R1, [R0]
    BX      LR

;********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartTheFirst(void)
;
; Note(s) : 1) OSStartTheFirst() MUST:
;              a) Setup priority of exceptions used by OS;
;              b) Set initial PSP to 0, this will tell context switcher that it is startup and to run the
;                 fist task;
;              c) Set osRunning to TRUE;
;              d) Trigger PendSV exception;
;              e) Enable interrupts (tasks will run with interrupts enabled).
;********************************************************************************************************

OSStartTheFirst
    CPSID   i	                                    ; prevent exception
    
    LDR     R0, =SCB_SHPR2_REG                      ; Set the SVC exception priority
    LDR     R1, =SVC_PRI_VALUE
    LSLS    R1, R1, #24
    STRB    R1, [R0]
    
    LDR     R0, =SCB_SHPR3_REG                      ; Set the PendSVC exception priority
    LDR     R1, =PENDSV_PRI_VALUE
    LSLS    R1, R1, #16
    LDR     R2, [R0]
    LDR     R3, =0xFF00FFFF
    ANDS    R2, R2, R3
    ORRS    R1, R1, R2
    STRB    R1, [R0]
    
    LDR     R0, =SCB_SHPR3_REG                      ; Set the SYSTICK exception priority
    LDR     R1, =SYSTICK_PRI_VALUE
    LSLS    R1, R1, #24
    LDR     R2, [R0]
    LDR     R3, =0x00FFFFFF
    ANDS    R2, R2, R3
    ORRS    R1, R1, R2
    STRB    R1, [R0]
    
    MOVS    R0, #0                                  ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0
    
    LDR     R0, =SCB_VTOR_REG                       ; get vector table offset from VTOR register
    LDR     R0, [R0]                                ; load address of vector table
    LDR     R0, [R0, #0]                            ; load first entry of vector table which is the reset stack point
    MSR     MSP, R0
    
    LDR     R0, =osRunning                          ; osRunning = 1
    MOVS    R1, #1
    STRB    R1, [R0]
    
    LDR     R1, =OSCriticalNesting                  ; OSCriticalNesting = 0
    MOVS    R0, #0
    STR     R0, [R1]
    
    LDR     R0, =SCB_ICSR_REG                       ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =SCB_ICSR_REG_PENDSVSET_MSK
    STR     R1, [R0]
    
    CPSIE   i	                                    ; enable interrupts
        
__START_HANG
    B       __START_HANG                            ; should never get here

;********************************************************************************************************
;                                         HANDLE PendSV EXCEPTION
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 (by OSStartTheFirst() above) then skip (goto d) the saving 
;                 part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;              c) Save the process SP in its TCB, osTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, osTCBCur = osTCBNextRdy;
;              g) Get new process SP from TCB, SP = osTCBNextRdy->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) osTCBCur      points to the OS_TCB of the task to suspend
;                 osTCBNextRdy  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartTheFirst() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;********************************************************************************************************

PendSV_Handler
	CPSID   i                                       ; Prevent interruption during context switch
	
    MRS     R0, PSP                                 ; PSP is current thread's stack pointer
    CMP     R0, #0                                  ; to skip context saving if we are going to run the first thread, because
    BEQ     __NO_SAVE                               ; PSP == 0 mean there is no context of any thread.
                                                    

    SUBS    R0, R0, #0x20                           ; There are some threads running. Save regs r4-11 on current thread's stack. r0-r3 have been saved automatically
    STM     R0!, {R4-R7}
    MOV     R1, R8
    STR     R1,[R0]
    ADDS    R0, #4
    MOV     R1, R9
    STR     R1,[R0]
    ADDS    R0, #4
    MOV     R1, R10
    STR     R1,[R0]
    ADDS    R0, #4
    MOV     R1, R11
    STR     R1,[R0]
    ADDS    R0, #4
    SUBS    R0, R0, #0x20
    
	LDR     R1, =osTCBCur                           ; osTCBCur->OSTCBStkPtr = SP
    LDR     R1, [R1]
    STR     R0, [R1, #8]
                                                    ; At this point, entire context of current thread has been saved.
__NO_SAVE
    ;PUSH    {LR}                                    ; Call OSTaskSwHook()
    ;LDR     R0, =OSTaskSwHook                       
    ;BLX     R0
    ;POP     {LR}
    
    LDR     R0, =osTCBCur                           ; osTCBCur = osTCBNextRdy;
    LDR     R1, =osTCBNextRdy
    LDR     R2, [R1]
    STR     R2, [R0]
    
    LDR     R0, [R2, #8]                            ; R0 = osTCBCur->OSTCBStkPtr;
    LDM     R0!, {R4-R7}                            ; Restore r4-11 from new thread's stack.
    LDR     R1,[R0]
    MOV     R8, R1
    ADDS    R0, #4
    LDR     R1,[R0]
    MOV     R9, R1
    ADDS    R0, #4
    LDR     R1,[R0]
    MOV     R10, R1
    ADDS    R0, #4
    LDR     R1,[R0]
    MOV     R11, R1
    ADDS    R0, #4
    
    MSR     PSP, R0                                 ; Restor task's stack pointer.
    
    MOVS    R0, #0x04                               ; Change EXC_RETURN to uses PSP after return.
    MOV     R1, LR
    ORRS    R1, R1, R0
    MOV     LR, R1
    ;MOV     R0, #0x1                                ; Config CONTROL reg to uses unprivileged mode after return.
    ;MSR     CONTROL, R0

    CPSIE   i       	    	                    ; enable interruption.
    
    BX      LR                                      ; Exception return will restore other CPU register from new thread's stack.
    

    END

