;********************************************************************************************************
; For       : ARMv7 Cortex-M3
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

    PUBLIC  CALL_SVC1
    PUBLIC  CALL_SVC2
    PUBLIC  DISABLE_ALL_INT
    PUBLIC  ENTER_CRITICAL
    PUBLIC  EXIT_CRITICAL
    PUBLIC  SET_INTERRUPT_MASK
    PUBLIC  TASK_SW
    PUBLIC  OSStartTheFirst
    PUBLIC  OSCtxSw
    PUBLIC  OSIntCtxSw
    PUBLIC  SVC_Handler
    PUBLIC  PendSV_Handler

;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

SCB_VTOR            EQU     0xE000ED08
NVIC_INT_CTRL       EQU     0xE000ED04              ; Interrupt control state register.
NVIC_PENDSVSET      EQU     0x10000000              ; Value to trigger PendSV exception.
NVIC_PENDSV_REG     EQU     0xE000ED22
NVIC_PENDSV_PRI     EQU           0xFF              ; PendSV priority value (lowest).
NVIC_SVC_REG        EQU     0xE000ED1F  
NVIC_SVC_PRI 	    EQU           0x00              ; SVC priority value (highest).
NVIC_SYSTICK_REG    EQU     0xE000ED23
NVIC_SYSTICK_PRI    EQU           0xFF
INT_PRIO_MASK       EQU       (1 << 3)              ; 屏蔽优先级值大于等于此值的中断，这里屏蔽除SVC外的所有中断
                                                    ; LPC17xx使用高5位表示优先级

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
; SVC_Handler
; Note(s) : 1) This is paper wrapping.
;              All process will be done in SVC_Process.
;********************************************************************************************************

SVC_Handler
    ;判断SVC中断前使用的哪个堆栈,并将堆栈指针保存到R0（作为SVC_Process的参数）
    TST     LR, #0x04                               ;按位与，并更新Z标志
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    PUSH    {LR}
    BL      SVC_Process
    POP     {PC}





DISABLE_ALL_INT
    LDR 	R1, =INT_PRIO_MASK                               
    MSR 	BASEPRI, R1
	BX      LR
    	
;********************************************************************************************************
;                                              进入临界区
;                                         void ENTER_CRITICAL(void)
;********************************************************************************************************

ENTER_CRITICAL	    	    	    	    	    ;关中断
    LDR 	R1, =INT_PRIO_MASK                               
    MSR 	BASEPRI, R1
                                                                                  	         	    	    
	LDR 	R1, =OSCriticalNesting	                ;OSCriticalNesting++
	LDR  	R2, [R1]
	CMP		R2, #255
	IT      NE
    ADDNE   R2, R2, #1
	STR  	R2, [R1]
	
    BX      LR

;********************************************************************************************************
;                                              退出临界区
;                                         void EXIT_CRITICAL(void)
;********************************************************************************************************

EXIT_CRITICAL	    	    	    	    	    ;开中断
	LDR     R1, =OSCriticalNesting                  ;OSCriticalNesting--
	LDR     R2, [R1]
	CMP		R2, #0
	IT      NE
    SUBNE   R2, R2, #1
	STR     R2, [R1]
	
    MOV     R1, #0	      
	CMP     R2, #0	    	                        ;如果 OSCriticalNesting = 0, 开中断                               
	IT      EQ
    MSREQ	BASEPRI, R1	                         	          
	
    BX      LR

;********************************************************************************************************
;                                            设定处理器中断屏蔽等级
;                                      INT32U SET_INTERRUPT_MASK(INT32U)
;********************************************************************************************************

SET_INTERRUPT_MASK
	MRS     R1, BASEPRI                             ;保存当前 BASEPRI
    MSR 	BASEPRI, R0
    MOV     R0, R1                                  ;返回 BASEPRI
    BX      LR
    
;********************************************************************************************************
;                                              触发任务调度
;                                           void TASK_SW(void)
;********************************************************************************************************

TASK_SW	    	    	    	    	            
	LDR     R0, =NVIC_INT_CTRL                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR







;********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartTheFirst(void)
;
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartTheFirst() MUST:
;              a) Setup PendSV exception priority to lowest;
;              b) Set initial PSP to 0, to tell context switcher this is first run;
;              c) Set osRunning to TRUE;
;              d) Trigger PendSV exception;
;              e) Enable interrupts (tasks will run with interrupts enabled).
;********************************************************************************************************

OSStartTheFirst
    LDR     R0, =NVIC_SVC_REG                       ; Set the SVC exception priority
    LDR     R1, =NVIC_SVC_PRI
    STRB    R1, [R0]
    
    LDR     R0, =NVIC_PENDSV_REG                    ; Set the PEND exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]
    
    LDR     R0, =NVIC_SYSTICK_REG                   ; Set the SYSTICK exception priority
    LDR     R1, =NVIC_SYSTICK_PRI
    STRB    R1, [R0]
    
    MOV     R0, #0                                  ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0
    
    LDR     R0, =SCB_VTOR                           ; get vector table offset from VTOR register
    LDR     R0, [R0]                                ; load address of vector table
    LDR     R0, [R0, #0]                            ; load first entry of vector table whick is the reset stack point
    MSR     MSP, R0
    
    LDR     R0, =osRunning                          ; osRunning = 1
    MOV     R1, #1
    STRB    R1, [R0]
    
    LDR     R1, =OSCriticalNesting                  ; OSCriticalNesting = 0
    MOV     R0, #0
    STR     R0, [R1]
    
    LDR     R0, =NVIC_INT_CTRL                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    
    MOV     R0, #0	                                ; enable interrupts
    MSR	    BASEPRI, R0	                            
        
__START_HANG
    B       __START_HANG                            ; should never get here

;********************************************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function just
;              triggers the PendSV exception which is where the real work is done.
;********************************************************************************************************

OSCtxSw
    PUSH    {LR}                                    ; Save LR(EXC_RETURN value)
    LDR     R0, =0
    LDR     R2, =CALL_SVC1                          ; Call: CALL_SVC1(0)
    BLX     R2
    POP     {LR}
    BX      LR

;********************************************************************************************************
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;                                         void OSIntCtxSw(void)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
;              the result of an interrupt.  This function simply triggers a PendSV exception which will
;              be handled when there are no more interrupts active and interrupts are enabled.
;********************************************************************************************************

OSIntCtxSw
    LDR     R0, =NVIC_INT_CTRL                      ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;********************************************************************************************************
;                                         HANDLE PendSV EXCEPTION
;                                     void OS_CPU_PendSVHandler(void)
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
	LDR 	R0, =INT_PRIO_MASK                      ; Prevent interruption during context switch
    MSR 	BASEPRI, R0                             
	
    MRS     R0, PSP                                 ; PSP is current thread's stack pointer
    CBZ     R0, __NO_SAVE                           ; Skip context save if we are going to run the first thread, because
                                                    ; PSP = 0 is invalid and there is no context of any thread.

    SUBS    R0, R0, #0x20                           ; There are some threads running. Save regs r4-11 on current thread's stack. r0-r3 have been saved automatically
    STM     R0, {R4 - R11}
    
	LDR     R1, =osTCBCur
    LDR     R1, [R1]
    STR     R0, [R1, #8]                            ; osTCBCur->OSTCBStkPtr = SP
                                                    ; At this point, entire context of current thread has been saved.
__NO_SAVE
    ;PUSH    {LR}                                    ; Save LR(EXC_RETURN value)
    ;LDR     R0, =OSTaskSwHook                       ; Call OSTaskSwHook()
    ;BLX     R0
    ;POP     {LR}
    
    LDR     R0, =osTCBCur                           ; osTCBCur = osTCBNextRdy;
    LDR     R1, =osTCBNextRdy
    LDR     R2, [R1]
    STR     R2, [R0]
    
    LDR     R0, [R2, #8]                            ; R0 = osTCBCur->OSTCBStkPtr;
    
    LDM     R0, {R4-R11}                            ; Restore r4-11 from new thread's stack.
    ADDS    R0, R0, #0x20                       
    MSR     PSP, R0                                 ; Let PSP as new thread's SP.
        	                                                                            
    ORR     LR, LR, #0x04                           ; Change EXC_RETURN to uses PSP after return. Note: current use MSP.
    ;MOV     R0, #0x1                                ; Config CONTROL reg to uses unprivileged mode after return. Note: now we are in PendSV's ISR.
    ;MSR     CONTROL, R0

    MOV     R0, #0       	    	                ; enable interruption.
    MSR 	BASEPRI, R0
    
    BX      LR                                      ; Exception return will restore other CPU register from new thread's stack.
    

    END

