/*******************************************************************************
 *  Copyright(C)2015-2022 by Dreistein<mcu_shilei@hotmail.com>                *
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



/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./debug.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

#if defined(DEBUG_PRINTF_DYN)

/*
 * Make dummy function to prevent NULL pointer dereferences before
 * debug_printf has been set by calling debug_set_printf().
 */
static int debug_printf_uninit( const _CHAR *format, ... )
{
    ((void) format);
    return( 0 );
}

int (*debug_printf)( const _CHAR *, ... ) = debug_printf_uninit;

int debug_set_printf( int (*printf_func)( const _CHAR *, ... ) )
{
    debug_printf = printf_func;
}
#endif /* DEBUG_PRINTF_ALT */

/* EOF */
