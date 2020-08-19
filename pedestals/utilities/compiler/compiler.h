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

#ifndef __COMPILER_H__
#define __COMPILER_H__

/*============================ INCLUDES ======================================*/
#if   defined(__CPU_ARM__)                      //!< ARM series
    #include ".\arm\arm_compiler.h"
#elif defined(__CPU_AVR__)                      //!< AVR series
    #include ".\avr\avr_compiler.h"
#elif defined(__CPU_X86__)                      //!< x86 series
    #include ".\x86\x86_compiler.h"
#elif defined(__CPU_TMS320__)                   //!< TMS320 series
    #include ".\tms320\tms320_compiler.h"
#else
    #error "No specified CPU type!"
#endif

#include <stdint.h>     //!< integer type
#include <stddef.h>     //!< NULL, size_t, offsetof
#include <stdbool.h>    //!< bool
#include <limits.h>     //!< CHAR_BIT, xxx_MAX, xxx_MIN
#include <string.h>     //!< memcpy, memset, strlen, strcpy

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif  //! #ifndef __COMPILER_H__
