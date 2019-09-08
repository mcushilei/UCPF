
#ifndef __OS_CPU_H__
#define __OS_CPU_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define OS_CPU_STK_GROWTH_DOWN     1       //!< Stack grows from HIGH to LOW memory on ARM.

#define OS_CPU_CFG_HIGHEST_INTERRUPT_PRIORITY_USED          (0x01u)
#define OS_CPU_CFG_HIGHEST_INTERRUPT_PRIORITY_CALLED_FROM   (0x01u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

/*!
 *! \Brief       DATA TYPES (Compiler and Hardware Specific)
 *!
 */
typedef uint8_t     UINT8;      //!< Unsigned at least 8 bit quantity, it might be 16 bits on some CPUs or compilers.
typedef int8_t      SINT8;      //!< Signed   at least 8 bit quantity
typedef uint16_t    UINT16;     //!< Unsigned at least 16 bit quantity
typedef uint16_t    SINT16;     //!< Signed   at least 16 bit quantity
typedef uint32_t    UINT32;     //!< Unsigned at least 32 bit quantity
typedef int32_t     SINT32;     //!< Signed   at least 32 bit quantity
typedef uint64_t    UINT64;     //!< Unsigned at least 64 bit quantity
typedef int64_t     SINT64;     //!< Signed   at least 64 bit quantity
typedef float       FP32;       //!< Single precision floating point
typedef double      FP64;       //!< Double precision floating point

typedef _Bool       BOOL;

typedef uint32_t    CPU_STK;    //!< stack unite used by target CPU.
typedef uint32_t    CPU_SR;     //!< size of CPU status register.
typedef uint32_t    CPU_REG;    //!< size of CPU register.

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
