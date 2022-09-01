/*******************************************************************************
 *  Copyright(C)2015-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include <stdio.h>

//! DEBUG_CONFIG_FILE should be defined to specify the path of configration file.
//! debug_config.h is the default one to prevent compiler aborting and shall not
//! be modifyed.
#if !defined(DEBUG_CONFIG_FILE)
#include "./debug_config.h"
#else
#include DEBUG_CONFIG_FILE
#endif


/*============================ MACROS ========================================*/

#ifndef DEBUG_MIN_LEVEL
#define DEBUG_MIN_LEVEL     DEBUG_LEVEL_ALL
#endif

#ifndef DEBUG_TYPES_ON
#define DEBUG_TYPES_ON      (DEBUG_TRACE | DEBUG_STATE | DEBUG_FRESH | DEBUG_HALT)
#endif

//! \brief ocnditions used to control assert
//! {
#define DEBUG_LEVEL_NORMAL  0x0000U
#define DEBUG_LEVEL_WARNING 0x0001U /* eg: bad checksums, dropped packets, ... */
#define DEBUG_LEVEL_ERROR   0x0002U /* eg: memory allocation failures, ... */

#define DEBUG_LEVEL_ALL     0x0000U
#define DEBUG_LEVEL_NONE    0x0007U
#define DEBUG_LEVEL_MASK    0x0007U
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

/*============================ MACROFIED FUNCTIONS ===========================*/

#if defined(DEBUG_PRINTF_ALT)
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
int debug_set_printf( int (*printf_func)( const _CHAR *, ... ) );

#elif defined(DEBUG_PRINTF_MACRO)
#   define debug_printf     DEBUG_PRINTF_MACRO
#else
#   define debug_printf     printf
#endif

//! \brief run-time trace log message
//! {
#if defined(__DEBUG__)
#   define RTT_LOG(fmt, ...)    debug_printf("\r\n[I][%s:%d]"fmt, __ThisFileName, __LINE__, ##__VA_ARGS__)
#else
#   define RTT_LOG(fmt, ...)    debug_printf("\r\n[I]"fmt, ##__VA_ARGS__)
#endif
//! }

//! \brief debug log message
//! {
#if defined(__DEBUG__)
#   define DBG_LOG(fmt, ...)    debug_printf("\r\n[D][%s:%d]"fmt, __ThisFileName, __LINE__, ##__VA_ARGS__)
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
                debug_printf("\r\n[D][%s:%d]"fmt, __ThisFileName, line, ##__VA_ARGS__);\
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

//! \brief debug asserts
//! {
#if DEBUG_ASSERT_ENABLE == ENABLED

#   define __DEBUG_ASSERT(condition, line, ...)     \
    do {                                            \
        if (!(condition)) {                         \
            debug_failure_captured((const _CHAR *)__ThisFileName, line);    \
            __VA_ARGS__                             \
        }                                           \
    } while (0);

#   define DEBUG_ASSERT(condition, ...)     __DEBUG_ASSERT(condition, __LINE__, ##__VA_ARGS__)


#   define DEBUG_ASSERT_NOT_NULL(pointer, ...)                      \
        DEBUG_ASSERT(                                               \
            ((pointer) != NULL),                                    \
            debug_print_null_point();                               \
            __VA_ARGS__                                             \
        )

#   define DEBUG_ASSERT_EQUAL_HEX(expected, actual, ...)            \
        DEBUG_ASSERT(                                               \
            ((_UINT)(expected) == (_UINT)(actual)),                 \
            debug_print_equal_number((_SINT)(expected), (_SINT)(actual), DEBUG_DISPLAY_STYLE_POINTER);\
            __VA_ARGS__                                             \
        )

#   define DEBUG_ASSERT_EQUAL_PTR DEBUG_ASSERT_EQUAL_HEX

#   define DEBUG_ASSERT_EQUAL_UINT(expected, actual, ...)           \
        DEBUG_ASSERT(                                               \
            ((_UINT)(expected) == (_UINT)(actual)),                 \
            debug_print_equal_number((_SINT)(expected), (_SINT)(actual), DEBUG_DISPLAY_STYLE_UINT);\
            __VA_ARGS__                                             \
        )

#   define DEBUG_ASSERT_EQUAL_INT(expected, actual, ...)            \
        DEBUG_ASSERT(                                               \
            ((_SINT)(expected) == (_SINT)(actual)),                 \
            debug_print_equal_number((_SINT)(expected), (_SINT)(actual), DEBUG_DISPLAY_STYLE_INT);\
            __VA_ARGS__                                             \
        )

#   define DEBUG_ASSERT_EQUAL_STRING(expected, actual, ...)         \
        DEBUG_ASSERT(                                               \
            debug_string_compare((expected), (actual)),             \
            debug_print_expected_actual_string((expected), (actual));\
            __VA_ARGS__                                             \
        )

#   define DEBUG_ASSERT_BITS(mask, expected, actual, ...)           \
        DEBUG_ASSERT(                                               \
            (((_UINT)mask & (_UINT)expected) == ((_UINT)mask & (_UINT)actual)),\
            debug_print_equal_bits((_UINT)mask, (_UINT)expected, (_UINT)actual);\
            __VA_ARGS__                                             \
        )

#else
#   define DEBUG_ASSERT(condition, ...)
#   define DEBUG_ASSERT_NOT_NULL(pointer, ...)
#   define DEBUG_ASSERT_EQUAL_HEX(expected, actual, ...)
#   define DEBUG_ASSERT_EQUAL_PTR(expected, actual, ...)
#   define DEBUG_ASSERT_EQUAL_UINT(expected, actual, ...)
#   define DEBUG_ASSERT_EQUAL_INT(expected, actual, ...)
#   define DEBUG_ASSERT_EQUAL_STRING(expected, actual, ...)
#   define DEBUG_ASSERT_BITS(mask, expected, actual, ...)
#endif
//! }

/*============================ TYPES =========================================*/
//-------------------------------------------------------
// Internal Structs Needed
//-------------------------------------------------------
enum {
    DEBUG_DISPLAY_STYLE_INT      = 0,
    DEBUG_DISPLAY_STYLE_UINT,
    DEBUG_DISPLAY_STYLE_HEX,
    DEBUG_DISPLAY_STYLE_POINTER,
};

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

#if   (DEBUG_POINTER_WIDTH == 64)
typedef uint64_t _UP;
#elif (DEBUG_POINTER_WIDTH == 32)
typedef uint32_t _UP;
#elif (DEBUG_POINTER_WIDTH == 16)
typedef uint16_t _UP;
#else
#error "Invalid DEBUG_POINTER_WIDTH specified! (64, 32 or 16 are supported)"
#endif


/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
//-------------------------------------------------------
// Debug Fuctions
//-------------------------------------------------------
//  Use the macros above this section instead of calling
//  these directly. The macros have a consistent naming
//  convention and will pull in file and line information
//  for you.
extern void debug_print_string( const _CHAR *string );
extern void debug_failure_captured( const _CHAR *file, const _UINT line );
extern void debug_print_equal_number( const _SINT expected, const _SINT actual, const unsigned int style );
extern void debug_print_equal_bits( const _UINT mask, const _UINT expected, const _UINT actual );
extern void debug_print_expected_actual_string( const _CHAR *expected, const _CHAR *actual );
extern void debug_print_null_point( void );
extern int  debug_string_compare( const _CHAR *expected, const _CHAR *actual );
extern void debug_trap( void );
extern void debug_exit_trap( void );

#endif      //!< #ifndef __SERVICE_DEBUG_H__
/* EOF */
