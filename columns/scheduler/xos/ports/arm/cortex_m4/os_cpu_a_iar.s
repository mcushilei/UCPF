;********************************************************************************************************
; For       : ARMv7 Cortex-M4 with FPU
; Toolchain : IAR EWARM
;********************************************************************************************************

;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

    EXTERN  osRunning
    EXTERN  osTCBCur
    EXTERN  osTCBNextRdy
    EXTERN  OSTaskSwHook
    EXTERN  OSCriticalNesting

    PUBLIC  OSStartTheFirstThread
    PUBLIC  OSCtxSw
    PUBLIC  OSIntCtxSw
    
    PUBLIC  PendSV_Handler

;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

SCB_ICSR_REG        EQU     0xE000ED04              ; Interrupt Control State Register.
SCB_VTOR_REG        EQU     0xE000ED08
SCB_SHPR3_REG       EQU     0xE000ED20
FPU_CPACR_REG       EQU     0xE000ED88              ; Coprocessor Access Control Register.
NVIC_PENDSVSET      EQU     0x10000000              ; Value to trigger PendSV exception.
NVIC_PENDSV_PRI     EQU     0xFF << 16              ; PendSV priority value (lowest).
NVIC_SYSTICK_PRI    EQU     0xFF << 24

;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

    SECTION .text:CODE:NOROOT(2)
;	AREA |.text|, CODE, READONLY, ALIGN=2           ;AREA |.text| 选择段 |.text|。
                                          	        ;CODE表示代码段，READONLY表示只读（缺省）
                                          	        ;ALIGN=2表示4字节对齐。若ALIGN=n，这2^n对齐
	THUMB                                           ;Thumb 代码
	REQUIRE8                                        ;指定当前文件要求堆栈八字节对齐
	PRESERVE8                                       ;令指定当前文件保持堆栈八字节对齐



;********************************************************************************************************
;                                       void OSStartTheFirstThread(void)
;
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartTheFirstThread() MUST:
;              a) Setup PendSV exception priority to lowest;
;              b) Set initial PSP to 0, to tell context switcher this is first run;
;              c) Set osRunning to TRUE;
;              d) Trigger PendSV exception;
;              e) Enable interrupts.
;********************************************************************************************************

OSStartTheFirstThread
    LDR     R0, =SCB_SHPR3_REG                      ; Set the PendSV exception priority
    LDR     R1, [R0]
    ORR     R1, R1, #(NVIC_PENDSV_PRI)
    STR     R1, [R0]
    
    LDR     R0, =SCB_SHPR3_REG                      ; Set the Systick exception priority
    LDR     R1, [R0]
    ORR     R1, R1, #(NVIC_SYSTICK_PRI)
    STR     R1, [R0]
    
    LDR     R0, =SCB_VTOR_REG                       ; get vector table offset from VTOR register
    LDR     R0, [R0]                                ; load address of vector table
    LDR     R0, [R0, #0]                            ; Reset the MSP to the start address.
    MSR     MSP, R0
    
    LDR     R0, =osRunning                          ; osRunning = 1
    MOV     R1, #1
    STRB    R1, [R0]
    
    LDR     R1, =OSCriticalNesting                  ; OSCriticalNesting = 0
    MOV     R0, #0
    STR     R0, [R1]

	
	LDR.W   R0, =FPU_CPACR_REG                      ; Enable FPU.
	LDR	    R1, [R0]
	ORR	    R1, R1, #( 0x0F << 20 )                 ; Full access
	STR     R1, [R0]
    
    LDR     R0, =SCB_ICSR_REG                       ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    
    MOV     R0, #0	                                ; enable interrupts
    MSR	    BASEPRI, R0
    
    CPSIE   I
    CPSIE   F
    
__START_HANG
    B       __START_HANG                            ; should never get here

;********************************************************************************************************
;                                           void OSCtxSw(void)
;                               PERFORM A CONTEXT SWITCH (From task level)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function just
;              triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

OSCtxSw
    LDR     R0, =SCB_ICSR_REG                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    DSB
    ISB
    BX      LR

;********************************************************************************************************
;                                         void OSIntCtxSw(void)
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
;              the result of an interrupt.  This function simply triggers a PendSV exception which will
;              be handled when there are no more interrupts active and interrupts are enabled.
;********************************************************************************************************

OSIntCtxSw
    LDR     R0, =SCB_ICSR_REG                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;********************************************************************************************************
;                                     void PendSV_Handler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 (by OSStartTheFirstThread() above) then skip (goto d) the saving 
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
;           4) Since PendSV is set to lowest priority in the system (by OSStartTheFirstThread() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;********************************************************************************************************

PendSV_Handler
	LDR         R1, =osTCBCur
    LDR         R1, [R1]
    CBZ         R1, __NO_SAVE

    MRS         R0, PSP                             ; PSP is current thread's stack pointer.
    
    TST         LR, #0x10                           ; If FPU is used, then PUSH VFP registers.
	IT          EQ
	VSTMDBEQ    R0!, {S16-S31}

    STMDB       R0!, {R4 - R11, LR}                 ; PUSH regs r4-11 and LR on current thread's stack. R0-r3 have been saved automatically
    
    STR         R0, [R1, #8]                        ; osTCBCur->OSTCBStkPtr = SP

__NO_SAVE
    PUSH    {LR}                                    ; Save LR(EXC_RETURN value)
    LDR     R0, =OSTaskSwHook                       ; Call OSTaskSwHook()
    BLX     R0
    POP     {LR}
    
    LDR         R0, =osTCBCur                       ; osTCBCur = osTCBNextRdy;
    LDR         R1, =osTCBNextRdy
    LDR         R2, [R1]
    STR         R2, [R0]
    
    LDR         R0, [R2, #8]                        ; R0 = osTCBCur->OSTCBStkPtr;
    
    LDMIA       R0!, {R4-R11, LR}                   ; POP r4-11 and r14 from new thread's stack.

    TST         LR, #0x10                           ; If FPU is used, then POP VFP registers.
	IT          EQ
	VLDMIAEQ    R0!, {S16-S31}
    
    MSR         PSP, R0                             ; Write back PSP.
    ISB
        	                                                                            
    BX          LR                                  ; Exception return will restore other CPU register from new thread's stack.
    

    END

