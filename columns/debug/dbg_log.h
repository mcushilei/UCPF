/*******************************************************************************
 *  Copyright(C)2015-2023 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __SERVICE_DBG_LOG_H__
#define __SERVICE_DBG_LOG_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

//! DEBUG_CONFIG_FILE should be defined to specify the path of configration file.
//! debug_config.h is the default one to prevent compiler aborting and shall not
//! be modifyed.
#if !defined(DEBUG_CONFIG_FILE)
#include "./debug_config.h"
#else
#include DEBUG_CONFIG_FILE
#endif


/*============================ MACROS ========================================*/
/*
 * filter structure:
 * 0bFEDCBA9876543210
 *   |||||||||||||+++level
 *   ||||||||+++++reserved
 *   ||||++++types
 *   |+++reserved
 *   +enable
 */

//! \brief level control. the greater the value is, more info output is enabled.
//! {
#define DEBUG_LEVEL_ALL     0x0000U
#define DEBUG_LEVEL_NONE    0x0007U
#define DEBUG_LEVEL_MASK    0x0007U

#define DEBUG_LEVEL_NORMAL  0x0000U
#define DEBUG_LEVEL_WARNING 0x0001U
#define DEBUG_LEVEL_ERROR   0x0002U
#define DEBUG_LEVEL_FATAL   0x0003U
//! }

//! \brief flags for debug types. 
//! {
#define DEBUG_TRACE         0x0800U /** flag for indicating a tracing message (to follow program flow) */
#define DEBUG_STATE         0x0400U /** flag for indicating a state debug message (to follow module states) */
#define DEBUG_FRESH         0x0200U /** flag for indicating newly added code (to debug new code) */
//! }

/* \brief flags for enable or disable an individual debug output.
 */
//! {
#define DEBUG_ON            0x8000U
#define DEBUG_OFF           0x0000U
//! }

#ifndef DEBUG_MIN_LEVEL
#define DEBUG_MIN_LEVEL     DEBUG_LEVEL_ALL
#endif

#ifndef DEBUG_TYPES_ON
#define DEBUG_TYPES_ON      (DEBUG_TRACE | DEBUG_STATE | DEBUG_FRESH)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

//! \brief debug log output redirection at run-time.
//! {
#if defined(DEBUG_PRINTF_DYN)
/*
 * The function pointers for printf
 */
extern int (*debug_printf)( const _CHAR *format, ... );

/**
 * \brief               This function dynamically configures the printf
 *                      function that is called when the debug_printf()
 *                      function is invoked.
 *
 * \param printf_func   The \c printf function implementation.
 *
 * \return              \c 0 on success.
 */
extern int debug_set_printf( int (*printf_func)( const _CHAR *, ... ) );

#elif !defined(debug_printf)
#   include <stdio.h>
#   define debug_printf     printf
#endif
//! }

#if !defined(DEBUG_EOL)
#   define DEBUG_EOL        "\n"
#endif

//! \brief debug log message
//! {
#if defined(__DEBUG__)
#   define DBG_LOG(fmt, ...)    debug_printf("[D][%s:%d]" fmt DEBUG_EOL, _ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#   define DBG_LOG(fmt, ...)
#endif
//! }

//! \brief debug tracing message with filter
//! {
#if DEBUG_MSG_ENABLE == ENABLED
#   define __DBG_MSG(filter, line, fmt, ...)    \
    do {                                        \
        if (( (filter) & (DEBUG_ON))            \
        &&  ( (filter) & (DEBUG_TYPES_ON))      \
        &&  (((filter) & (DEBUG_LEVEL_MASK)) >= DEBUG_MIN_LEVEL)) {\
            debug_printf("[D][%s:%d]" fmt DEBUG_EOL, _ThisFileName, line, ##__VA_ARGS__);\
        }                                       \
    } while(0)

#   define DBG_MSG(filter, fmt, ...)        __DBG_MSG(filter, __LINE__, fmt, ##__VA_ARGS__)
#else
#   define DBG_MSG(filter, fmt, ...)
#endif
//! }

/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif      //!< #ifndef __SERVICE_DBG_LOG_H__
/* EOF */
