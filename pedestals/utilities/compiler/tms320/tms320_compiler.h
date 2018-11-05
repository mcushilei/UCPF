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


#ifndef __TMS320_COMPILER_H__
#define __TMS320_COMPILER_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
//! \note for IAR
#if defined(__TI_COMPILER_VERSION__)
    #define __IS_COMPILER_TI__      1
#else
    #error "Not supported compiler."
#endif
//! @}

//! ALU integer width in byte
#define INTEGER_BYTE_SIZE           2

//! \brief The mcu memory align mode
#define MEM_ALIGN_SIZE              INTEGER_BYTE_SIZE

//! \brief The mcu memory endian mode
#define __BIG_ENDIAN__              0

//! \brief none standard memory types
#define FLASH           const
#define EEPROM          const
#define NO_INIT         __attribute__((section("noinit")))
#define INLINE          inline
#define ROOT            __attribute__((used))
#define WEAK
#define RAMFUNC         __attribute__((section("ramfuncs")))
#define ASM             __asm
#define ALIGN(__N)      __attribute__((aligned(__N)))
#define AT_ADDR(__ADDR)
#define SECTION(__SEC)  __attribute__((section(__SEC)))

#define ISR(__VECTOR)   __interrupt void __VECTOR(void)

typedef          short int8_t;
typedef unsigned short uint8_t;



#endif
