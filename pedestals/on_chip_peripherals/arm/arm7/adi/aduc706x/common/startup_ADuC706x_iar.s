
; Standard definitions of mode bits and Interrupt (I & F) flags in PSRs
;31 30  29  28   ---   7   6   5    4   3   2   1   0 
;N   Z   C   V         I   F   T   M4  M3  M2  M1  M0 
;                                   1   0   0   0   0     USR
;                                   1   0   0   0   1     FIQ
;                                   1   0   0   1   0     IRQ
;                                   1   0   0   1   1     SVC
;                                   1   0   1   1   1     ABT
;                                   1   1   0   1   1     UND
;                                   1   1   1   1   1     SYS
;c - control field mask byte (PSR[7:0]) 
;x - extension field mask byte (PSR[15:8]) 
;s - status field mask byte (PSR[23:16) 
;f - flags field mask byte (PSR[31:24]).
ARM_MODE_USR    EQU     0x10
ARM_MODE_FIQ    EQU     0x11
ARM_MODE_IRQ    EQU     0x12
ARM_MODE_SVC    EQU     0x13
ARM_MODE_ABT    EQU     0x17
ARM_MODE_UND    EQU     0x1B
ARM_MODE_SYS    EQU     0x1F
I_BIT           EQU     0x80            ; when I bit is set, IRQ is disabled
F_BIT           EQU     0x40            ; when F bit is set, FIQ is disabled

; MMR definitions
MMR_BASE        EQU     0xFFFF0000      ; MMR Base Address
REMAP_OFFSET    EQU         0x0220

        EXTERN  __iar_program_start

        PUBLIC  Reset_Handler
        PUBWEAK Undefined_Handler
        PUBWEAK SWI_Handler
        PUBWEAK Prefetch_Handler
        PUBWEAK Abort_Handler
        PUBWEAK IRQ_Handler
        PUBWEAK FIQ_Handler
        
        ; Forward declaration of sections.
        SECTION UND_STACK:DATA:NOROOT(3)
        SECTION SVC_STACK:DATA:NOROOT(3)
        SECTION ABT_STACK:DATA:NOROOT(3)
        SECTION IRQ_STACK:DATA:NOROOT(3)
        SECTION FIQ_STACK:DATA:NOROOT(3)
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION HEAP:DATA:NOROOT(3)

;  Startup Code must be linked first at Address at which it expects to run.
        SECTION    .vectors:CODE:ROOT(2)
        PRESERVE8
        ARM

;  Exception Vectors
;  Mapped to Address 0.
;  Absolute addressing mode must be used.
Vectors
        LDR     PC, Reset_Addr          ; Reset
        LDR     PC, Undefined_Addr      ; Undefined instructions
        LDR     PC, SWI_Addr            ; Software interrupt (SWI/SYS)
        LDR     PC, Prefetch_Addr       ; Prefetch abort
        LDR     PC, Abort_Addr          ; Data abort
        B       .                       ; RESERVED
        LDR     PC, IRQ_Addr            ; IRQ
        LDR     PC, FIQ_Addr            ; FIQ

Reset_Addr      DCD Reset_Handler
Undefined_Addr  DCD Undefined_Handler
SWI_Addr        DCD SWI_Handler
Prefetch_Addr   DCD Prefetch_Handler
Abort_Addr      DCD Abort_Handler
                DCD 0
IRQ_Addr        DCD IRQ_Handler_Wrapping
FIQ_Addr        DCD FIQ_Handler_Wrapping


        SECTION .text:CODE:NOROOT(2)
        PRESERVE8
        ARM

Reset_Handler
        ;  Enter System mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_SYS | I_BIT | F_BIT
        LDR     sp, =SFE(CSTACK)
        
        ;  Enter Undefined Instruction mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_UND | I_BIT | F_BIT
        LDR     sp, =SFE(UND_STACK)

        ;  Enter Abort mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_ABT | I_BIT | F_BIT
        LDR     sp, =SFE(ABT_STACK)

        ;  Enter FIQ mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_FIQ | I_BIT | F_BIT
        LDR     sp, =SFE(FIQ_STACK)

        ;  Enter IRQ mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_IRQ | I_BIT | F_BIT
        LDR     sp, =SFE(IRQ_STACK)

        ;  Enter Supervisor mode and set its Stack Pointer
        MSR     cpsr_c, #ARM_MODE_SVC | I_BIT | F_BIT
        LDR     sp, =SFE(SVC_STACK)

        ; Branch to upper
        LDR     r0, =__iar_program_start
        MOV     lr, pc
        BX      r0
        B       .
        
IRQ_Handler_Wrapping
        ; Save LR and SPSR on the stack to allow nesting.
        SUB     lr, lr, #4
        STMFD   sp!, {lr}
        MRS     lr, SPSR
        STMFD   sp!, {lr}

        MSR     cpsr_c, #ARM_MODE_SYS | I_BIT   ; Branch to interrupt handler in System mode, and DISABLE IRQ
        STMFD   sp!, {r0-r3, r12, lr}           ; Save workreg on stack.Ensure 8-byte stack alignment.
        LDR     r0, =IRQ_Handler                ; Call C interrupt dispatch function.
        MOV     lr, pc
        BX      r0
        LDMFD   sp!, {r0-r3, r12, lr}           ; Restore workreg saved on stack.
        MSR     cpsr_c, #ARM_MODE_IRQ | I_BIT   ; Restore IRQ mode, and interrupt state.

        ; Restore interrupt context and branch back to calling code
        LDMFD   sp!, {lr}
        MSR     spsr_cxsf, lr
        LDMFD   sp!, {pc}^

FIQ_Handler_Wrapping
        ; Save interrupt context on the stack to allow nesting
        SUB     lr, lr, #4
        STMFD   sp!, {lr}
        MRS     lr, SPSR
        STMFD   sp!, {lr}

        ; Branch to interrupt handler in Supervisor mode
        MSR     cpsr_c, #ARM_MODE_SYS | I_BIT | F_BIT
        STMFD   sp!, {r0-r3, r12, lr}
        LDR     r0, =FIQ_Handler
        MOV     lr, pc
        BX      r0
        LDMFD   sp!, {r0-r3, r12, lr}
        MSR     cpsr_c, #ARM_MODE_IRQ | I_BIT | F_BIT

        ; Restore interrupt context and branch back to calling code
        LDMFD   sp!, {lr}
        MSR     spsr_cxsf, lr
        LDMFD   sp!, {pc}^

Undefined_Handler
SWI_Handler
Prefetch_Handler
Abort_Handler
FIQ_Handler
IRQ_Handler

Default_Handler
    #ifdef __DEBUG__
        B       Default_Handler
    #else
        B       Reset_Handler
    #endif


        END
