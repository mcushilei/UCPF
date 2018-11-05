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


//! \note do not move this pre-processor statement to other places
#include "..\app_cfg.h"

#ifndef __SERVICE_DEBUG_APP_CFG__
#define __SERVICE_DEBUG_APP_CFG__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define DEBUG_POINTER_WIDTH     (32)
#define DEBUG_INT_WIDTH         (32)
#define DEBUG_ROM_VAR_TYPE      

/*============================ MACROFIED FUNCTIONS ===========================*/
//-------------------------------------------------------
// Output Method
//-------------------------------------------------------
#ifndef DEBUG_OUTPUT_CHAR
    #define DEBUG_OUTPUT_CHAR(a) debug_output_char(a)
#endif

//-------------------------------------------------------
// Int Support
//-------------------------------------------------------
#ifndef DEBUG_INT_WIDTH
    #define DEBUG_INT_WIDTH     (32)
#endif

//-------------------------------------------------------
// Pointer Support
//-------------------------------------------------------
#ifndef DEBUG_POINTER_WIDTH
    #define DEBUG_POINTER_WIDTH (32)
#endif

//-------------------------------------------------------
// Float Support
//-------------------------------------------------------
#ifndef DEBUG_INCLUDE_FLOAT
    #undef DEBUG_FLOAT_PRECISION
#else
    #define DEBUG_FLOAT_PRECISION (0.00001f)
#endif

/*============================ TYPES =========================================*/
//-------------------------------------------------------
// Char Support
//-------------------------------------------------------
typedef char _CHAR;

//-------------------------------------------------------
// Int Support
//-------------------------------------------------------

#if   (DEBUG_INT_WIDTH == 64)
    typedef uint64_t _UINT;
    typedef int64_t _SINT;
#elif (DEBUG_INT_WIDTH == 32)
    typedef uint32_t _UINT;
    typedef int32_t _SINT;
#elif (DEBUG_INT_WIDTH == 16)
    typedef uint16_t _UINT;
    typedef int16_t _SINT;
#elif (DEBUG_INT_WIDTH == 8)
    typedef uint8_t _UINT;
    typedef int8_t _SINT;
#else
    #error "Invalid DEBUG_INT_WIDTH specified! (64, 32, 16 or 8 are supported)"
#endif

//-------------------------------------------------------
// Pointer Support
//-------------------------------------------------------
#if   (DEBUG_POINTER_WIDTH == 64)
    typedef uint64_t _UP;
#elif (DEBUG_POINTER_WIDTH == 32)
    typedef uint32_t _UP;
#elif (DEBUG_POINTER_WIDTH == 16)
    typedef uint16_t _UP;
#else
    #error "Invalid DEBUG_POINTER_WIDTH specified! (64, 32 or 16 are supported)"
#endif

//-------------------------------------------------------
// Float Support
//-------------------------------------------------------
#ifndef DEBUG_EXCLUDE_FLOAT 
    typedef float _UF;
#endif

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void DEBUG_OUTPUT_CHAR(_CHAR);

#endif
/* EOF */
