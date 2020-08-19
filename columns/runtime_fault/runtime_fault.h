/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __BEAM_RUNTIME_FAULT_H__
#define __BEAM_RUNTIME_FAULT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#if defined(__DEBUG__)
#define RUNTIME_FAULT( error_code, error_string )	\
        runtime_fault( error_code, __ThisFileName, __LINE__, error_string )
#else
#define RUNTIME_FAULT( error_code, error_string )	\
        runtime_fault( error_code, error_string )
#endif


#if defined(__DEBUG__)
#define LOWLEVEL_RUNTIME_FAULT( error_code )		\
        runtime_fault( error_code, __ThisFileName, __LINE__, NULL )
#else
#define LOWLEVEL_RUNTIME_FAULT( error_code )		\
        runtime_fault( error_code, NULL )
#endif

                                        
#define VALIDATE_RET( cond, ret )	do {			\
        if( !(cond) ) {                             \
            RUNTIME_FAULT( 0, NULL );               \
            return( ret );                          \
        }                                           \
    } while( 0 )

#define VALIDATE( cond )			do {			\
        if( !(cond) ) {                             \
            RUNTIME_FAULT( 0, NULL );               \
            return;                                 \
        }                                           \
    } while( 0 )

/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
#if defined(__DEBUG__)
extern void runtime_fault( int ec, const char *file, int line, const char *info );
#else
extern void runtime_fault( int ec, const char *info );
#endif

#endif  //!< #ifndef __BEAM_RUNTIME_FAULT_H__
/* EOF */
