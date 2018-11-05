/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __X86_COMPILER_H__
#define __X86_COMPILER_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
//! \name The macros to identify the compiler
//! @{
//! \note for GCC
#if defined(__GNUC__)
    #define __IS_COMPILER_GCC__     1
//! \note for VS
#elif defined(_MSC_VER)
    #define __IS_COMPILER_VS__      1
#else
    #error "Not supported compiler."
#endif
//! @}

//! \brief none standard memory types
#if     __IS_COMPILER_VS__
#   define NO_INIT          
#   define INLINE           inline
#   define WEAK
#   define ROOT
#   define ALIGN_OF(__V)    __alignof(__V)
#elif   __IS_COMPILER_GCC__
#   define NO_INIT          
#   define INLINE           inline
#   define WEAK
#   define ROOT
#   define ALIGN_OF(__V)    alignof(__V)
#endif

#endif
