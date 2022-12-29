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


#ifndef __SERVICE_DEBUG_H__
#define __SERVICE_DEBUG_H__

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
 *   |||||||||||||---level
 *   ||||||||-----reserved
 *   ||||----types
 *   |---reserved
 *   -enable
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
#define DEBUG_FRESH         0x0200U /** flag for indicating newly added code, not thoroughly debuged yet */
#define DEBUG_HALT          0x0100U /** flag for to halt after printing this debug message */
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
#define DEBUG_TYPES_ON      (DEBUG_TRACE | DEBUG_STATE | DEBUG_FRESH | DEBUG_HALT)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

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

#if !defined(DEBUG_EOL)
#   define DEBUG_EOL        "\r\n"
#endif

//! \brief run-time trace log message
//! {
#if defined(__DEBUG__)
#   define RTT_LOG(fmt, ...)    debug_printf("[I][%s:%d]" fmt DEBUG_EOL, __ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#   define RTT_LOG(fmt, ...)    debug_printf("[I]" fmt DEBUG_EOL, ##__VA_ARGS__)
#endif
//! }

//! \brief debug log message
//! {
#if defined(__DEBUG__)
#   define DBG_LOG(fmt, ...)    debug_printf("[D][%s:%d]" fmt DEBUG_EOL, __ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#   define DBG_LOG(fmt, ...)
#endif
//! }

//! \brief debug log message with filter
//! {
#if DEBUG_MSG_ENABLE == ENABLED
#   define __DEBUG_MSG_FLT(filter, line, fmt, ...)       \
    do {                                        \
        if (( (filter) & (DEBUG_ON)) &&         \
            ( (filter) & (DEBUG_TYPES_ON)) &&   \
            (((filter) & (DEBUG_LEVEL_MASK)) >= DEBUG_MIN_LEVEL)) {\
                debug_printf("[D][%s:%d]" fmt DEBUG_EOL, __ThisFileName, line, ##__VA_ARGS__);\
                if ((filter) & DEBUG_HALT) {    \
                    debug_trap();               \
                }                               \
        }                                       \
    } while(0)

#   define DBG_MSG(filter, fmt, ...)        __DEBUG_MSG_FLT(filter, __LINE__, fmt, ##__VA_ARGS__)
#else
#   define DBG_MSG(filter, fmt, ...)
#endif
//! }

/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void debug_trap( void );
extern void debug_exit_trap( void );

#endif      //!< #ifndef __SERVICE_DEBUG_H__
/* EOF */
