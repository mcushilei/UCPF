
ReWrite_CRP     EQU     0
CRP_Value       EQU     0xFFFFFFFF


    EXTERN  __iar_program_start
    PUBLIC  __vector_table
                        
    PUBLIC  Reset_Handler
    PUBWEAK NMI_Handler         
    PUBWEAK HardFault_Handler   
    PUBWEAK MemManage_Handler   
    PUBWEAK BusFault_Handler    
    PUBWEAK UsageFault_Handler  
    PUBWEAK SVC_Handler         
    PUBWEAK DebugMon_Handler    
    PUBWEAK PendSV_Handler      
    PUBWEAK SysTick_Handler     

    PUBWEAK WDT_IRQHandler      
    PUBWEAK TIMER0_IRQHandler   
    PUBWEAK TIMER1_IRQHandler   
    PUBWEAK TIMER2_IRQHandler   
    PUBWEAK TIMER3_IRQHandler   
    PUBWEAK UART0_IRQHandler    
    PUBWEAK UART1_IRQHandler    
    PUBWEAK UART2_IRQHandler    
    PUBWEAK UART3_IRQHandler    
    PUBWEAK PWM1_IRQHandler     
    PUBWEAK I2C0_IRQHandler     
    PUBWEAK I2C1_IRQHandler     
    PUBWEAK I2C2_IRQHandler     
    PUBWEAK SPI_IRQHandler      
    PUBWEAK SSP0_IRQHandler     
    PUBWEAK SSP1_IRQHandler     
    PUBWEAK PLL0_IRQHandler     
    PUBWEAK RTC_IRQHandler      
    PUBWEAK EINT0_IRQHandler    
    PUBWEAK EINT1_IRQHandler    
    PUBWEAK EINT2_IRQHandler    
    PUBWEAK EINT3_IRQHandler    
    PUBWEAK ADC_IRQHandler      
    PUBWEAK BOD_IRQHandler      
    PUBWEAK USB_IRQHandler      
    PUBWEAK CAN_IRQHandler      
    PUBWEAK DMA_IRQHandler      
    PUBWEAK I2S_IRQHandler      
    PUBWEAK ENET_IRQHandler     
    PUBWEAK RIT_IRQHandler      
    PUBWEAK MCPWM_IRQHandler    
    PUBWEAK QEI_IRQHandler      
    PUBWEAK PLL1_IRQHandler     
    PUBWEAK USBActivity_IRQHandler
    PUBWEAK CANActivity_IRQHandler
    

    SECTION CSTACK:DATA:NOROOT(3)
    SECTION HEAP:DATA:NOROOT(3)
    

    SECTION .intvec:CODE:NOROOT (2)               
    DATA
__vector_table  
        DCD     SFE(CSTACK)               ; Top of Stack
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
        DCD     WDT_IRQHandler            ; 16: Watchdog Timer
        DCD     TIMER0_IRQHandler         ; 17: Timer0
        DCD     TIMER1_IRQHandler         ; 18: Timer1
        DCD     TIMER2_IRQHandler         ; 19: Timer2
        DCD     TIMER3_IRQHandler         ; 20: Timer3
        DCD     UART0_IRQHandler          ; 21: UART0
        DCD     UART1_IRQHandler          ; 22: UART1
        DCD     UART2_IRQHandler          ; 23: UART2
        DCD     UART3_IRQHandler          ; 24: UART3
        DCD     PWM1_IRQHandler           ; 25: PWM1
        DCD     I2C0_IRQHandler           ; 26: I2C0
        DCD     I2C1_IRQHandler           ; 27: I2C1
        DCD     I2C2_IRQHandler           ; 28: I2C2
        DCD     SPI_IRQHandler            ; 29: SPI
        DCD     SSP0_IRQHandler           ; 30: SSP0
        DCD     SSP1_IRQHandler           ; 31: SSP1
        DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
        DCD     RTC_IRQHandler            ; 33: Real Time Clock
        DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
        DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
        DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
        DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
        DCD     ADC_IRQHandler            ; 38: A/D Converter
        DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
        DCD     USB_IRQHandler            ; 40: USB
        DCD     CAN_IRQHandler            ; 41: CAN
        DCD     DMA_IRQHandler            ; 42: General Purpose DMA
        DCD     I2S_IRQHandler            ; 43: I2S
        DCD     ENET_IRQHandler           ; 44: Ethernet
        DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
        DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
        DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
        DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
        DCD		USBActivity_IRQHandler	  ; 49: USB Activity interrupt to wakeup
        DCD		CANActivity_IRQHandler	  ; 50: CAN Activity interrupt to wakeup


    IF      ReWrite_CRP != 0
    SECTION .crp :CODE:ROOT(2)
    DATA
        DCD     CRP_Value
    ENDIF
    

    SECTION .text:CODE:NOROOT(2)
    THUMB
    PRESERVE8
Reset_Handler   
        LDR     R0, = __iar_program_start
        BX      R0

HardFault_Handler
    IMPORT  err_log
        TST     LR, #0x04 
        ITE     EQ 
        MRSEQ   R0, MSP 
        MRSNE   R0, PSP 
        
        LDR     R3, =err_log
        LDR     R1, [R0, #0]                ;R0
        STR     R1, [R3, #0]
        LDR     R1, [R0, #4]                ;R1
        STR     R1, [R3, #4]
        LDR     R1, [R0, #8]                ;R2
        STR     R1, [R3, #8]
        LDR     R1, [R0, #12]               ;R3
        STR     R1, [R3, #12]
        LDR     R1, [R0, #16]               ;R12
        STR     R1, [R3, #16]
        LDR     R1, [R0, #20]               ;LR
        STR     R1, [R3, #20]
        LDR     R1, [R0, #24]               ;PC
        STR     R1, [R3, #24]
        LDR     R1, [R0, #28]               ;XPSR
        STR     R1, [R3, #28]
        
        LDR     R1, =0xE000ED28             ;MFSR
        LDRB    R1, [R1, #0]
        STRB    R1, [R3, #32]
        LDR     R1, =0xE000ED29             ;BFSR
        LDRB    R1, [R1, #0]
        STRB    R1, [R3, #33]
        LDR     R1, =0xE000ED2A             ;UFSR
        LDRH    R1, [R1, #0]
        STRH    R1, [R3, #34]
        LDR     R1, =0xE000ED2C             ;HFSR
        LDR     R1, [R1, #0]
        STR     R1, [R3, #36]
        LDR     R1, =0xE000ED30             ;DFSR
        LDR     R1, [R1, #0]
        STR     R1, [R3, #40]
        LDR     R1, =0xE000ED3C             ;AFSR
        LDR     R1, [R1, #0]
        STR     R1, [R3, #44]
        LDR     R1, =0xE000ED34             ;MMAR
        LDR     R1, [R1, #0]
        STR     R1, [R3, #48]
        LDR     R1, =0xE000ED38             ;BFAR
        LDR     R1, [R1, #0]
        STR     R1, [R3, #52]
        B       .
                
NMI_Handler     
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler     
DebugMon_Handler
PendSV_Handler  
SysTick_Handler 
WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler
Default_Handler
#ifdef __DEBUG__
        B       Default_Handler
#else
        B       Reset_Handler
#endif


        END
