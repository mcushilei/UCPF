;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION HEAP:DATA:NOROOT(3)


        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        SECTION .intvec:CODE:NOROOT(2)
        DATA

__vector_table
        DCD SFE(CSTACK)
        DCD Reset_Handler
        DCD NMI_Handler
        DCD HardFault_Handler
        DCD 0
        DCD 0
        DCD 0
__vector_table_0x1c
        DCD 0
        DCD 0
        DCD 0
        DCD 0
        DCD SVC_Handler
        DCD 0
        DCD 0
        DCD PendSV_Handler
        DCD SysTick_Handler

		DCD PIN_INT0_IRQHandler           ; Pin interrupt 0
		DCD PIN_INT1_IRQHandler           ; Pin interrupt 1
		DCD PIN_INT2_IRQHandler           ; Pin interrupt 2
		DCD PIN_INT3_IRQHandler           ; Pin interrupt 3
		DCD PIN_INT4_IRQHandler           ; Pin interrupt 4
		DCD PIN_INT5_IRQHandler           ; Pin interrupt 5
		DCD PIN_INT6_IRQHandler           ; Pin interrupt 6
		DCD PIN_INT7_IRQHandler           ; Pin interrupt 7
		DCD GINT0_IRQHandler              ; Port interrupt group 0
		DCD GINT1_IRQHandler              ; Port interrupt group 1
		DCD I2C1_IRQHandler               ; I2C1 interrupt
		DCD USART1_4_IRQHandler           ; USARTS 1 and 4 shared interrupt
		DCD USART2_3_IRQHandler           ; USARTS 2 and 3 shared interrupt
		DCD SCT0_1_IRQHandler             ; SCT 0 and 1 shared interrupt
		DCD SSP1_IRQHandler               ; SSP1 interrupt
		DCD I2C0_IRQHandler               ; I2C0 interrupt
		DCD TIMER16_0_IRQHandler          ; CT16B0 (16-bit Timer 0)
		DCD TIMER16_1_IRQHandler          ; CT16B1 (16-bit Timer 1)
		DCD TIMER32_0_IRQHandler          ; CT32B0 (32-bit Timer 0)
		DCD TIMER32_1_IRQHandler          ; CT32B0 (32-bit Timer 1)
		DCD SSP0_IRQHandler               ; SSP0 interrupt interrupt
		DCD USART0_IRQHandler             ; USART 0 interrupt interrupt
		DCD USB_IRQHandler                ; USB IRQ interrupt
		DCD USB_FIQHandler                ; USB FIQ interrupt
		DCD ADCA_IRQHandler               ; ADC A sequence (A/D Converter) interrupt
		DCD RTC_IRQHandler                ; RTC interrupt
		DCD BOD_WDT_IRQHandler            ; Shared BOD (Brownout Detect) and WDT interrupts
		DCD FMC_IRQHandler                ; Flash Memory Controller interrupt
		DCD DMA_IRQHandler                ; DMA Controller interrupt
		DCD ADCB_IRQHandler               ; ADC B sequence interrupt
		DCD USBWakeup_IRQHandler          ; USB wake-up interrupt
		DCD Reserved_IRQHandler

__Vectors_End

__Vectors       EQU     __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        PUBWEAK Reset_Handler
        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler

        PUBWEAK PIN_INT0_IRQHandler
		PUBWEAK PIN_INT1_IRQHandler
		PUBWEAK PIN_INT2_IRQHandler
		PUBWEAK PIN_INT3_IRQHandler
		PUBWEAK PIN_INT4_IRQHandler
		PUBWEAK PIN_INT5_IRQHandler
		PUBWEAK PIN_INT6_IRQHandler
		PUBWEAK PIN_INT7_IRQHandler
		PUBWEAK GINT0_IRQHandler
		PUBWEAK GINT1_IRQHandler
		PUBWEAK I2C1_IRQHandler
		PUBWEAK	USART1_4_IRQHandler
		PUBWEAK	USART2_3_IRQHandler
		PUBWEAK SCT0_1_IRQHandler
		PUBWEAK SSP1_IRQHandler
		PUBWEAK I2C0_IRQHandler
		PUBWEAK	TIMER16_0_IRQHandler
		PUBWEAK	TIMER16_1_IRQHandler
		PUBWEAK	TIMER32_0_IRQHandler
		PUBWEAK	TIMER32_1_IRQHandler
		PUBWEAK	SSP0_IRQHandler
		PUBWEAK	USART0_IRQHandler
		PUBWEAK	USB_IRQHandler
		PUBWEAK	USB_FIQHandler
		PUBWEAK	ADCA_IRQHandler
		PUBWEAK	RTC_IRQHandler
		PUBWEAK	BOD_WDT_IRQHandler
		PUBWEAK	FMC_IRQHandler
		PUBWEAK	DMA_IRQHandler
		PUBWEAK	ADCB_IRQHandler
		PUBWEAK	USBWakeup_IRQHandler
        PUBWEAK Reserved_IRQHandler
        
        SECTION .text:CODE:REORDER:NOROOT(2)
        THUMB

Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

NMI_Handler
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

HardFault_Handler
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

SVC_Handler
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

PendSV_Handler
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

SysTick_Handler
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif


PIN_INT0_IRQHandler
PIN_INT1_IRQHandler
PIN_INT2_IRQHandler
PIN_INT3_IRQHandler
PIN_INT4_IRQHandler
PIN_INT5_IRQHandler
PIN_INT6_IRQHandler
PIN_INT7_IRQHandler
GINT0_IRQHandler
GINT1_IRQHandler
I2C1_IRQHandler
USART1_4_IRQHandler
USART2_3_IRQHandler
SCT0_1_IRQHandler
SSP1_IRQHandler
I2C0_IRQHandler
TIMER16_0_IRQHandler
TIMER16_1_IRQHandler
TIMER32_0_IRQHandler
TIMER32_1_IRQHandler
SSP0_IRQHandler
USART0_IRQHandler
USB_IRQHandler
USB_FIQHandler
ADCA_IRQHandler
RTC_IRQHandler
BOD_WDT_IRQHandler
FMC_IRQHandler
DMA_IRQHandler
ADCB_IRQHandler
USBWakeup_IRQHandler
Reserved_IRQHandler

Default_Handler:
#ifdef __DEBUG__
        B       .
#else
        B       Reset_Handler
#endif

        END
