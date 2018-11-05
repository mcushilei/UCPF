/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not, see http://www.gnu.org/licenses/.        *
*******************************************************************************/

#ifndef __OS_CPU_CFG__
#define __OS_CPU_CFG__


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
typedef uint32_t    CPU_SR;  //!< size of CPU status register.

/*
*********************************************************************************************************
*                                              Cortex-M
*                                      Critical Section Management
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section. This
*             critical section could not be nested.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  The disabling interrupt
*             will increase a counter and it will be decreased before restoring the interrupt state.
*             The interrupt will only be enabled if the counter reaches 0.
*             In other words, if interrupts were disabled before entering the critical section, they
*             will be disabled when leaving the critical section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of OS's functions that need to
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD     3

#define  OS_STK_GROWTH_DOWN     1       //!< Stack grows from HIGH to LOW on ARM.

#endif
