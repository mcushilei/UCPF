/*******************************************************************************
 *  Copyright(C)2015-2017 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __SERVICE_DEBUG_C__
#ifndef __SERVICE_DEBUG_H__
#define __SERVICE_DEBUG_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if DEBUG_FOMART_STRING == ENABLED
#   include <stdio.h>
#else
#   include "..\string\string.h"
#endif

/*============================ MACROS ========================================*/
/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 */
#define DEBUG_LEVEL_ALL     0x0000U
#define DEBUG_LEVEL_WARNING 0x0001U /* eg: bad checksums, dropped packets, ... */
#define DEBUG_LEVEL_SERIOUS 0x0002U /* eg: memory allocation failures, ... */
#define DEBUG_LEVEL_MASK    0x0007U

/** flag for DEBUGF to enable that debug message */
#define DEBUG_ON            0x8000U
/** flag for DEBUGF to disable that debug message */
#define DEBUG_OFF           0x0000U

/** flag for DEBUGF indicating a tracing message (to follow program flow) */
#define DEBUG_TRACE         0x0800U
/** flag for DEBUGF indicating a state debug message (to follow module states) */
#define DEBUG_STATE         0x0400U
/** flag for DEBUGF indicating newly added code, not thoroughly debuged yet */
#define DEBUG_FRESH         0x0200U
/** flag for DEBUGF to halt after printing this debug message */
#define DEBUG_HALT          0x0100U

#define DEBUG_ANY           DEBUG_ON

/*============================ MACROFIED FUNCTIONS ===========================*/
#if ((DEBUG_MSG_ENABLE == ENABLED) || (DEBUG_ASSERT_ENABLE == ENABLED)) && DEBUG_DISALLOW_FILE_INFO == DISABLED
#   define DEBUG_DEFINE_THIS_FILE(string)       \
        static DEBUG_ROM_VAR_TYPE const _CHAR __ThisFileName[] = string
#else
#   define DEBUG_DEFINE_THIS_FILE(string)
#endif

//-------------------------------------------------------
// Debug Asserts
//-------------------------------------------------------
/** print debug message only if debug message type is enabled...
 *  AND is of correct type AND is at least DEBUG_LEVEL
 */
#if DEBUG_FOMART_STRING == ENABLED
#   define __DEBUG_PRINT(...)                   printf(__VA_ARGS__)
#else
#   define __DEBUG_PRINT(__STR, ...)            debug_print_string(__STR)
#endif

#if DEBUG_DISALLOW_FILE_INFO == ENABLED
#   define __DEBUG_ASSERT(condition, line, ...) do {\
        if (!(condition)) {                         \
            debug_failure_captured((void *)0, 0);   \
            __VA_ARGS__                             \
        }                                           \
    } while (0);

#   define __DEBUG_MSG(ctrl, line, ...)         do {\
        if (( (ctrl) & (DEBUG_ON)) &&               \
            ( (ctrl) & (DEBUG_TYPES_ON)) &&         \
            (((ctrl) & (DEBUG_LEVEL_MASK)) >= DEBUG_MIN_LEVEL)) {\
                debug_msg_output((void *)0, 0);     \
                __VA_ARGS__                         \
                if ((ctrl) & DEBUG_HALT) {          \
                    debug_trap();                   \
                }                                   \
        }                                           \
    } while(0);
#else
#   define __DEBUG_ASSERT(condition, line, ...) do {    \
        if (!(condition)) {                             \
            debug_failure_captured(__ThisFileName, line);\
            __VA_ARGS__                                 \
        }                                               \
    } while (0);

#   define __DEBUG_MSG(ctrl, line, ...)         do {    \
        if (( (ctrl) & (DEBUG_ON)) &&                   \
            ( (ctrl) & (DEBUG_TYPES_ON)) &&             \
            (((ctrl) & (DEBUG_LEVEL_MASK)) >= DEBUG_MIN_LEVEL)) {\
                debug_msg_output(__ThisFileName, line); \
                __VA_ARGS__                             \
                if ((ctrl) & DEBUG_HALT) {              \
                    debug_trap();                       \
                }                                       \
        }                                               \
    } while(0);
#endif

#if defined(__DEBUG__)
#   define DEBUG_PRINT(...)                 __DEBUG_PRINT(__VA_ARGS__)
#else
#   define DEBUG_PRINT(...)
#endif

#if DEBUG_MSG_ENABLE == ENABLED
#   define DEBUG_MSG(ctrl, ...)             __DEBUG_MSG(ctrl, __LINE__, __VA_ARGS__)
#else
#   define DEBUG_MSG(ctrl, ...)
#endif
            
#if DEBUG_ASSERT_ENABLE == ENABLED
#   define DEBUG_ASSERT(condition, ...)     __DEBUG_ASSERT(condition, __LINE__, __VA_ARGS__)

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
extern void debug_print_string      (const _CHAR *string);
extern void debug_failure_captured  (const _CHAR *file, const _UINT line);
extern void debug_msg_output        (const _CHAR *file, const _UINT line);
extern void debug_print_equal_number(const _SINT expected, const _SINT actual, const unsigned int style);
extern void debug_print_equal_bits  (const _UINT mask, const _UINT expected, const _UINT actual);
extern void debug_print_expected_actual_string(const _CHAR *expected, const _CHAR *actual);
extern void debug_print_null_point  (void);
extern int  debug_string_compare    (const _CHAR *expected, const _CHAR *actual);
extern void debug_trap              (void);
extern void debug_exit_trap         (void);

#endif      //!< #ifndef __SERVICE_DEBUG_H__
#endif      //!< #ifndef __SERVICE_DEBUG_C__
/* EOF */
