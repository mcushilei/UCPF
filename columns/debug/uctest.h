/*******************************************************************************
 *  Copyright(C)2022      by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __SERVICE_UCTEST_H__
#define __SERVICE_UCTEST_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

//! UCTEST_CONFIG_FILE should be defined to specify the path of configration file.
//! uctest_config.h is the default one to prevent compiler aborting and shall not
//! be modifyed.
#if !defined(UCTEST_CONFIG_FILE)
#include "./uctest_config.h"
#else
#include UCTEST_CONFIG_FILE
#endif


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define __UCTEST(condition, line, ...)              \
    do {                                            \
        if (!(condition)) {                         \
            uctest_failure_captured((const char *)__ThisFileName, line);    \
            __VA_ARGS__                             \
        }                                           \
    } while (0);

#define UCTEST(condition, ...)     __UCTEST(condition, __LINE__, ##__VA_ARGS__)

#define UCTEST_TRUE     UCTEST


#define UCTEST_NOT_NULL(pointer, ...)                      \
        UCTEST(                                               \
            ((pointer) != NULL),                                    \
            uctest_print_null_point();                               \
            __VA_ARGS__                                             \
        )

#define UCTEST_EQUAL_HEX(expected, actual, ...)            \
        UCTEST(                                               \
            ((_UINT)(expected) == (_UINT)(actual)),                 \
            uctest_print_equal_number((_SINT)(expected), (_SINT)(actual), UCTEST_DISPLAY_STYLE_HEX);\
            __VA_ARGS__                                             \
        )

#define UCTEST_EQUAL_PTR    UCTEST_EQUAL_HEX

#define UCTEST_EQUAL_BIN(mask, expected, actual, ...)           \
        UCTEST(                                               \
            (((_UINT)mask & (_UINT)expected) == ((_UINT)mask & (_UINT)actual)),\
            uctest_print_equal_bits((_UINT)mask, (_UINT)expected, (_UINT)actual);\
            __VA_ARGS__                                             \
        )

#define UCTEST_EQUAL_UINT(expected, actual, ...)           \
        UCTEST(                                               \
            ((_UINT)(expected) == (_UINT)(actual)),                 \
            uctest_print_equal_number((_SINT)(expected), (_SINT)(actual), UCTEST_DISPLAY_STYLE_UINT);\
            __VA_ARGS__                                             \
        )

#define UCTEST_EQUAL_INT(expected, actual, ...)            \
        UCTEST(                                               \
            ((_SINT)(expected) == (_SINT)(actual)),                 \
            uctest_print_equal_number((_SINT)(expected), (_SINT)(actual), UCTEST_DISPLAY_STYLE_INT);\
            __VA_ARGS__                                             \
        )

#define UCTEST_EQUAL_STRING(expected, actual, ...)         \
        UCTEST(                                               \
            uctest_string_compare((expected), (actual)),             \
            uctest_print_expected_actual_string((expected), (actual));\
            __VA_ARGS__                                             \
        )


/*============================ TYPES =========================================*/
//-------------------------------------------------------
// Internal Structs Needed
//-------------------------------------------------------
enum {
    UCTEST_DISPLAY_STYLE_INT      = 0,
    UCTEST_DISPLAY_STYLE_UINT,
    UCTEST_DISPLAY_STYLE_HEX,
};

#if   (UCTEST_INT_WIDTH == 64)
typedef uint64_t _UINT;
typedef int64_t _SINT;
#elif (UCTEST_INT_WIDTH == 32)
typedef uint32_t _UINT;
typedef int32_t _SINT;
#elif (UCTEST_INT_WIDTH == 16)
typedef uint16_t _UINT;
typedef int16_t _SINT;
#elif (UCTEST_INT_WIDTH == 8)
typedef uint8_t _UINT;
typedef int8_t _SINT;
#else
#error "Invalid UCTEST_INT_WIDTH specified! (64, 32, 16 or 8 are supported)"
#endif

#if   (UCTEST_POINTER_WIDTH == 64)
typedef uint64_t _UP;
#elif (UCTEST_POINTER_WIDTH == 32)
typedef uint32_t _UP;
#elif (UCTEST_POINTER_WIDTH == 16)
typedef uint16_t _UP;
#else
#error "Invalid UCTEST_POINTER_WIDTH specified! (64, 32 or 16 are supported)"
#endif


/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
//  Use the macros above this section instead of calling
//  these directly. The macros have a consistent naming
//  convention and will pull in file and line information
//  for you.
extern void uctest_failure_captured( const char *file, const _UINT line );
extern void uctest_print_null_point( void );
extern void uctest_print_equal_number( const _SINT expected, const _SINT actual, const unsigned int style );
extern void uctest_print_equal_bits( const _UINT mask, const _UINT expected, const _UINT actual );
extern void uctest_print_expected_actual_string( const char *expected, const char *actual );
extern int  uctest_string_compare( const char *expected, const char *actual );

extern void uctest_output_char( char ch );

#endif      //!< #ifndef __SERVICE_UCTEST_H__
/* EOF */
