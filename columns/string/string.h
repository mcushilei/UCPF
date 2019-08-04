/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __STRING_H__
#define __STRING_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include <string.h>
#include <ctype.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define CHAR_IS_HEX(__C)            (  ((__C) >= '0' && (__C) <= '9')       \
                                    || ((__C) >= 'A' && (__C) <= 'F')       \
                                    || ((__C) >= 'a' && (__C) <= 'f') )

#define CAHR_IS_UPPERCASE(__C)      ((__C) >= 'A' && (__C) <= 'Z')
#define CAHR_IS_LOWERCASE(__C)      ((__C) >= 'a' && (__C) <= 'z')

#define CHAR_IS_INT(__CHAR)         ((__CHAR) >= '0' && (__CHAR) <= '9')

#define CHAR_TO_INT(__CHAR)         ((__CHAR) - '0')
#define CHAR_IS_SPACE(__CHAR)       (' ' == (__CHAR))


#ifdef USE_STAND_STRING_LIB
#   define STRING_PRINTF(__string, __VA0, ...)          \
        printf(__string, __VA0, __VA_ARGS__)
#else
#   define STRING_PRINTF(__string, __VA0, ...)          \
        string_printf(__string, (unsigned int)__VA0)
#endif

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern bool         memory_is_same(const void *m1, const void *m2, size_t n);
extern bool         memory_copy(void *d, const void *s, size_t n);
extern bool         memory_set(void *m, unsigned int v, size_t n);
extern unsigned int hex_str2uint(const char *str);
extern int          int_str2int(const char *str);
extern float        dec_str2float(const char *decimalString);
extern void         string_to_lowercase(char *s);
extern void         string_to_uppercase(char *s);
extern unsigned int string_length(const char *s);
extern bool         string_is_same(const char *s1, const char *s2);
extern bool         string_is_same_by_n(const char *s1, const char *s2, unsigned int n);
extern unsigned int string_copy(char *d, const char *s, unsigned int n);
extern char        *string_find_string(const char *s1, const char *s2);
extern char        *strsep(char **strngRef, const char *delim);
extern uint32_t     itostr(int32_t value, char *integerString, int32_t radix);
extern void         string_printf(const char *formatString, unsigned int argument);
extern unsigned int string_printf_to_buffer(char *buffer, unsigned int bufferLength, const char *formatString, unsigned int argument);

#endif
/* EOF */
