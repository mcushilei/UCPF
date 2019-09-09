

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
    PUBWEAK WAKEUP_IRQHandler
    PUBWEAK I2C_IRQHandler        
    PUBWEAK TIMER16_0_IRQHandler        
    PUBWEAK TIMER16_1_IRQHandler        
    PUBWEAK TIMER32_0_IRQHandler        
    PUBWEAK TIMER32_1_IRQHandler        
    PUBWEAK SSP_IRQHandler        
    PUBWEAK UART0_IRQHandler        
    PUBWEAK UART1_IRQHandler        
    PUBWEAK COMP_IRQHandler        
    PUBWEAK ADC_IRQHandler        
    PUBWEAK WDT_IRQHandler        
    PUBWEAK BOD_IRQHandler        
    PUBWEAK FMC_IRQHandler        
    PUBWEAK PIOINT0_IRQHandler       
    PUBWEAK PIOINT1_IRQHandler
    PUBWEAK PIOINT2_IRQHandler
    PUBWEAK PMU_IRQHandler
    PUBWEAK DMA_IRQHandler
    PUBWEAK RTC_IRQHandler


    SECTION CSTACK:DATA:NOROOT(3)
    
    
    SECTION .intvec:CODE:NOROOT(2)
    DATA
__vector_table
        DCD     sfe(CSTACK)                 ; Top of Stack
        DCD     Reset_Handler               ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; MPU Fault Handler
        DCD     BusFault_Handler            ; Bus Fault Handler
        DCD     UsageFault_Handler          ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     DebugMon_Handler            ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Handler
        DCD     SysTick_Handler             ; SysTick Handler
        
        ; External Interrupts
        DCD     WAKEUP_IRQHandler         ; 16+ 0: Wakeup PIO0.0
        DCD     WAKEUP_IRQHandler         ; 16+ 1: Wakeup PIO0.1
        DCD     WAKEUP_IRQHandler         ; 16+ 2: Wakeup PIO0.2
        DCD     WAKEUP_IRQHandler         ; 16+ 3: Wakeup PIO0.3
        DCD     WAKEUP_IRQHandler         ; 16+ 4: Wakeup PIO0.4
        DCD     WAKEUP_IRQHandler         ; 16+ 5: Wakeup PIO0.5
        DCD     WAKEUP_IRQHandler         ; 16+ 6: Wakeup PIO0.6
        DCD     WAKEUP_IRQHandler         ; 16+ 7: Wakeup PIO0.7
        DCD     WAKEUP_IRQHandler         ; 16+ 8: Wakeup PIO0.8
        DCD     WAKEUP_IRQHandler         ; 16+ 9: Wakeup PIO0.9
        DCD     WAKEUP_IRQHandler         ; 16+10: Wakeup PIO0.10
        DCD     WAKEUP_IRQHandler         ; 16+11: Wakeup PIO0.11
        DCD     I2C_IRQHandler            ; 16+12: I2C
        DCD     TIMER16_0_IRQHandler      ; 16+13: 16-bit Timer0
        DCD     TIMER16_1_IRQHandler      ; 16+14: 16-bit Timer1
        DCD     TIMER32_0_IRQHandler      ; 16+15: 32-bit Timer0
        DCD     TIMER32_1_IRQHandler      ; 16+16: 32-bit Timer1
        DCD     SSP_IRQHandler            ; 16+17: SSP
        DCD     UART0_IRQHandler          ; 16+18: UART0                
        DCD     UART1_IRQHandler          ; 16+19: UART1
        DCD     COMP_IRQHandler           ; 16+20: Comparator                
        DCD     ADC_IRQHandler            ; 16+21: A/D Converter
        DCD     WDT_IRQHandler            ; 16+22: Watchdog timer                
        DCD     BOD_IRQHandler            ; 16+23: Brown Out Detect
        DCD     FMC_IRQHandler            ; 16+24: IP2111 Flash Memory Controller
        DCD     PIOINT0_IRQHandler        ; 16+25: PIO INT0
        DCD     PIOINT1_IRQHandler        ; 16+26: PIO INT1
        DCD     PIOINT2_IRQHandler        ; 16+27: PIO INT2
        DCD     PMU_IRQHandler            ; 16+28: PMU/Wakeup
        DCD     DMA_IRQHandler            ; 16+29: DMA
        DCD     RTC_IRQHandler            ; 16+30: RTC


    SECTION .text:CODE:NOROOT(2)
    THUMB
Reset_Handler
        LDR     R0, = __iar_program_start
        BX      R0
      
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
WAKEUP_IRQHandler
I2C_IRQHandler
TIMER16_0_IRQHandler
TIMER16_1_IRQHandler
TIMER32_0_IRQHandler
TIMER32_1_IRQHandler
SSP_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
COMP_IRQHandler
ADC_IRQHandler
WDT_IRQHandler
BOD_IRQHandler
FMC_IRQHandler
PIOINT0_IRQHandler
PIOINT1_IRQHandler
PIOINT2_IRQHandler
PMU_IRQHandler
DMA_IRQHandler
RTC_IRQHandler
Default_IRQHandler
#ifdef __DEBUG__
        B       Default_IRQHandler
#else
        B       Reset_Handler
#endif


        END

