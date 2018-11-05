
#ifndef __OS_CPU_CFG__
#define __OS_CPU_CFG__


/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef _Bool           BOOL;
typedef uint8_t         UINT8;                    /* Unsigned  8 bit quantity                           */
typedef int8_t          SINT8;                    /* Signed    8 bit quantity                           */
typedef uint16_t        UINT16;                   /* Unsigned 16 bit quantity                           */
typedef uint16_t        SINT16;                   /* Signed   16 bit quantity                           */
typedef uint32_t        UINT32;                   /* Unsigned 32 bit quantity                           */
typedef int32_t         SINT32;                   /* Signed   32 bit quantity                           */
typedef uint64_t        UINT64;                   /* Unsigned 64 bit quantity                           */
typedef int64_t         SINT64;                   /* Signed   64 bit quantity                           */
typedef float           FP32;                     /* Single precision floating point                    */
typedef double          FP64;                     /* Double precision floating point                    */

typedef uint32_t        OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef uint32_t        OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

/*
*********************************************************************************************************
*                                              Cortex-M
*                                      Critical Section Management
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*             NOT IMPLEMENTED
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*             NOT IMPLEMENTED
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of OS's functions that need to
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD     3

/*
*********************************************************************************************************
*                                        Cortex-M3 Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH_DOWN     1                   /* Stack grows from HIGH to LOW memory on ARM        */

#endif
