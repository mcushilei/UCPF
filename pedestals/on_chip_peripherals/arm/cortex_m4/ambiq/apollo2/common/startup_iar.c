
#include ".\app_cfg.h"
#include ".\device.h"

//*****************************************************************************
//
// Weak function links.
//
//*****************************************************************************
#pragma weak MemManage_Handler  = fault_isr
#pragma weak BusFault_Handler   = fault_isr
#pragma weak UsageFault_Handler = fault_isr
#pragma weak SVC_Handler        = default_isr
#pragma weak DebugMon_Handler   = default_isr
#pragma weak PendSV_Handler     = default_isr
#pragma weak SysTick_Handler    = default_isr

#pragma weak brownout_isr        = default_isr
#pragma weak watchdog_isr        = default_isr
#pragma weak clkgen_isr          = default_isr
#pragma weak vcomp_isr           = default_isr
#pragma weak ioslave_ios_isr     = default_isr
#pragma weak ioslave_acc_isr     = default_isr
#pragma weak iomaster0_isr       = default_isr
#pragma weak iomaster1_isr       = default_isr
#pragma weak iomaster2_isr       = default_isr
#pragma weak iomaster3_isr       = default_isr
#pragma weak iomaster4_isr       = default_isr
#pragma weak iomaster5_isr       = default_isr
#pragma weak gpio_isr            = default_isr
#pragma weak ctimer_isr          = default_isr
#pragma weak uart_isr            = default_isr
#pragma weak uart1_isr           = default_isr
#pragma weak adc_isr             = default_isr
#pragma weak pdm_isr             = default_isr
#pragma weak stimer_isr          = default_isr
#pragma weak stimer_cmpr0_isr    = default_isr
#pragma weak stimer_cmpr1_isr    = default_isr
#pragma weak stimer_cmpr2_isr    = default_isr
#pragma weak stimer_cmpr3_isr    = default_isr
#pragma weak stimer_cmpr4_isr    = default_isr
#pragma weak stimer_cmpr5_isr    = default_isr
#pragma weak stimer_cmpr6_isr    = default_isr
#pragma weak stimer_cmpr7_isr    = default_isr
#pragma weak flash_isr           = default_isr
#pragma weak software0_isr       = default_isr
#pragma weak software1_isr       = default_isr
#pragma weak software2_isr       = default_isr
#pragma weak software3_isr       = default_isr


//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
static void default_isr(void);

static void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

extern void brownout_isr(void);
extern void watchdog_isr(void);
extern void clkgen_isr(void);
extern void vcomp_isr(void);
extern void ioslave_ios_isr(void);
extern void ioslave_acc_isr(void);
extern void iomaster0_isr(void);
extern void iomaster1_isr(void);
extern void iomaster2_isr(void);
extern void iomaster3_isr(void);
extern void iomaster4_isr(void);
extern void iomaster5_isr(void);
extern void gpio_isr(void);
extern void ctimer_isr(void);
extern void uart_isr(void);
extern void uart1_isr(void);
extern void adc_isr(void);
extern void pdm_isr(void);
extern void stimer_isr(void);
extern void stimer_cmpr0_isr(void);
extern void stimer_cmpr1_isr(void);
extern void stimer_cmpr2_isr(void);
extern void stimer_cmpr3_isr(void);
extern void stimer_cmpr4_isr(void);
extern void stimer_cmpr5_isr(void);
extern void stimer_cmpr6_isr(void);
extern void stimer_cmpr7_isr(void);
extern void flash_isr(void);
extern void software0_isr(void);
extern void software1_isr(void);
extern void software2_isr(void);
extern void software3_isr(void);

//*****************************************************************************
//
// The entry point for the application startup code.
//
//*****************************************************************************
extern void __iar_program_start(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
SECTION("CSTACK") ALIGN(8) static uint32_t stack[0x200];

//*****************************************************************************
//
// A type that describes the entries of the vector table.
//
//*****************************************************************************
typedef void (*vector_entry_t)(void);

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
// Note: Aliasing and weakly exporting MemManage_Handler, BusFault_Handler, and
// UsageFault_Handler does not work if fault_isr is defined externally.
// Therefore, we'll explicitly use fault_isr in the table for those vectors.
//
//*****************************************************************************
SECTION(".intvec") ROOT static const vector_entry_t __vector_table[] =
{
    (vector_entry_t)((uint32_t)stack + sizeof(stack)),   // The initial stack pointer
    
    Reset_Handler,                          // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    MemManage_Handler,                      // The MPU fault handler
    BusFault_Handler,                       // The bus fault handler
    UsageFault_Handler,                     // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVC_Handler,                            // SVCall handle
    DebugMon_Handler,                       // Debug monitor handler
    0,                                      // Reserved
    PendSV_Handler,                         // The PendSV handler
    SysTick_Handler,                        // The SysTick handler

    brownout_isr,                            //  0: Brownout
    watchdog_isr,                            //  1: Watchdog
    clkgen_isr,                              //  2: CLKGEN
    vcomp_isr,                               //  3: Voltage Comparator
    ioslave_ios_isr,                         //  4: I/O Slave general
    ioslave_acc_isr,                         //  5: I/O Slave access
    iomaster0_isr,                           //  6: I/O Master 0
    iomaster1_isr,                           //  7: I/O Master 1
    iomaster2_isr,                           //  8: I/O Master 2
    iomaster3_isr,                           //  9: I/O Master 3
    iomaster4_isr,                           // 10: I/O Master 4
    iomaster5_isr,                           // 11: I/O Master 5
    gpio_isr,                                // 12: GPIO
    ctimer_isr,                              // 13: CTIMER
    uart_isr,                                // 14: UART0
    uart1_isr,                               // 15: UART1
    adc_isr,                                 // 16: ADC
    pdm_isr,                                 // 17: PDM
    stimer_isr,                              // 18: STIMER
    stimer_cmpr0_isr,                        // 19: STIMER COMPARE0
    stimer_cmpr1_isr,                        // 20: STIMER COMPARE1
    stimer_cmpr2_isr,                        // 21: STIMER COMPARE2
    stimer_cmpr3_isr,                        // 22: STIMER COMPARE3
    stimer_cmpr4_isr,                        // 23: STIMER COMPARE4
    stimer_cmpr5_isr,                        // 24: STIMER COMPARE5
    stimer_cmpr6_isr,                        // 25: STIMER COMPARE6
    stimer_cmpr7_isr,                        // 26: STIMER COMPARE7
    flash_isr,                               // 27: FLASH
    software0_isr,                           // 28: SOFTWARE0
    software1_isr,                           // 29: SOFTWARE1
    software2_isr,                           // 30: SOFTWARE2
    software3_isr                            // 31: SOFTWARE3
};

WEAK int BEFORE_HW_INIT(void)
{
    /* Return 1 to indicate that normal segment */
    /* initialization should be performed. If   */
    /* normal segment initialization should not */
    /* be performed, return 0.                  */

    return 1;
}

//*****************************************************************************
//
// see IAR's low_level_init.c for more information.
//
//*****************************************************************************
int __low_level_init(void)
{
    SCB->VTOR = (uint32_t)&__vector_table;
    
    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not.           */
    /* Return: 0 to omit seg_init       */
    /*         1 to run seg_init        */
    /*==================================*/
    return BEFORE_HW_INIT();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void default_isr(void)
{
    while (1) {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.
//
//*****************************************************************************
static void Reset_Handler(void)
{
    __iar_program_start();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
WEAK void NMI_Handler(void)
{
    while (1) {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
WEAK void fault_isr(void)
{
    while (1) {
    }
}
