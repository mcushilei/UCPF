/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef __ARM_COMPILER_H__
#define __ARM_COMPILER_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
//! \name The macros to identify the compiler
//! @{
//! \note for GCC
#if defined(__GNUC__)
    #define __IS_COMPILER_GCC__     1
//! \note for MDK
#elif defined(__CC_ARM)
    #define __IS_COMPILER_MDK__     1
//! \note for IAR
#elif defined(__ICCARM__)
    #define __IS_COMPILER_IAR__     1
#else
    #error "Not supported compiler."
#endif
//! @}

//! anonymous struct
#if     __IS_COMPILER_MDK__
    #pragma anon_unions
#elif   __IS_COMPILER_IAR__
    #pragma language = extended
#elif   __IS_COMPILER_GCC__
    /* anonymous unions are enabled by default */
#endif

//! \brief none standard memory types
#if     __IS_COMPILER_IAR__
#   define NO_INIT          __no_init
#   define WEAK             __weak
#   define ROOT             __root
#   define RAMFUNC          __ramfunc
#   define ALIGN(__N)       _Alignas(__N)
#   define ALIGN_OF(__V)    __ALIGNOF__(__V)
#   define AT_ADDR(__ADDR)  _Pragma(STRINGZ(location=__ADDR))
#   define SECTION(__SEC)   _Pragma(STRINGZ(location=__SEC))
#   define __asm__          asm
#   define barrier()        asm volatile("":::"memory")
#elif   __IS_COMPILER_GCC__
#   define NO_INIT          __attribute__((section("noinit"))
#   define WEAK             __attribute__((weak))
#   define ROOT             __attribute__((used))
#   define RAMFUNC          __attribute__((section("textrw")))
#   define ALIGN(__N)       __attribute__((aligned(__N)))
#   define ALIGN_OF(__V)    __alignof__(__V)
#   define SECTION(__SEC)   __attribute__((section(__SEC)))
#   define barrier()        __asm__ __volatile__("":::"memory")
#elif   __IS_COMPILER_MDK__
#   define NO_INIT          __attribute__((section("noinit"),zero_init))
#   define WEAK             __attribute__((weak))
#   define ROOT             __attribute__((used))
#   define RAMFUNC          __attribute__((section("textrw")))
#   define ALIGN(__N)       __attribute__((aligned(__N)))
#   define ALIGN_OF(__V)    __ALIGNOF__(__V)
#   define AT_ADDR(__ADDR)  __attribute__((at(__ADDR)))
#   define SECTION(__SEC)   __attribute__((section(__SEC)))
#   define __asm__          __asm
#   define inline           __inline
#   define barrier()        __asm volatile("":::"memory")
#endif

#define ISR(__VECTOR)       ROOT void __VECTOR(void)


#endif
