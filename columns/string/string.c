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


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\string.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
bool memory_is_same(const void *m1, const void *m2, size_t n)
{
    if ((NULL == m1) || (NULL == m2) || (0 == n)) {
        return false;
    }

    for (; n; --n) {
        if (*(char *)m1 != *(char *)m2) {
            return false;
        }
        m1 = (char *)m1 + 1;
        m2 = (char *)m2 + 1;
    }

    return false;
}

bool memory_copy(void *d, const void *s, size_t n)
{
    if (d == NULL || s == NULL) {
        return false;
    }
    
    for (; n; --n) {
        *(char *)d = *(char *)s;
        d = (char *)d + 1;
        s = (char *)s + 1;
    }

    return true;
}


unsigned int hex_str2uint(const char *str)
{
    unsigned int val = 0;

    if (NULL == str) {
        return 0;
    }

    for (; CHAR_IS_HEX(*str); str++) {
        val <<= 4;
        val += (*str > '9')? ((*str < 'a')? (10 + (*str - 'A')) : (10 + (*str - 'a'))) : (*str - '0');
    }

    return val;
}

int int_str2int(const char *str)
{
    int val = 0;
    bool neg = false;

    if (NULL == str) {
        return 0;
    }

    if ('-' == *str) {
        neg = true;
    }

    for (; CHAR_IS_INT(*str); str++) {
        val *= 10;
        val += CHAR_TO_INT(*str);
    }

    if (neg) {
        return -val;
    }
    return val;
}

float dec_str2float(const char *str)
{ 
    bool isNegative = false; 
    bool isDecimal = false;
    char letter = '0'; 
    float integer = 0.0f; 
    float decimal = 0.0f; 
    float power = 0.1f; 

    if (NULL == str) { 
        return 0.0; 
    } 

    if (str[0] == '-') { 
        isNegative = true; 
        str++; 
    } else { 
        isNegative = false; 
    } 
 
    for (; *str != '\0'; ++str) {
        letter = *str;

        if (!CHAR_IS_INT(letter)) {
            if (letter == '.') { 
                isDecimal = true;
                ++str;
            }
            break; 
        } else {
            integer = integer * 10.0f + CHAR_TO_INT(letter);
        }
    } 

    if (isDecimal) {
        for (; *str != '\0'; ++str) {
            letter = *str;

            if (!CHAR_IS_INT(letter)) {
                break;
            } else {
                decimal += CHAR_TO_INT(letter) * power; 
                power = power * 0.1f; 
            }
        } 
    }

    return (isNegative? -(integer + decimal) : (integer + decimal)); 
}

void string_to_uppercase(char *s)
{
    if (s == NULL) {
        return;
    }
    
    for (; *s != '\0'; s++) {
        if (CAHR_IS_LOWERCASE(*s)) {
            *s -= 'a' - 'A';
        }
    }
}

void string_to_lowercase(char *s)
{
    if (s == NULL) {
        return;
    }
    
    for (; *s != '\0'; s++) {
        if (CAHR_IS_UPPERCASE(*s)) {
            *s += 'a' - 'A';
        }
    }
}

unsigned int string_length(const char *s)
{
    unsigned int len = 0;
    
    if (s == NULL) {
        return 0;
    }

    for (; *s != '\0'; s++) {
        len++;
    }

    return len;
}

bool string_is_same(const char *s1, const char *s2)
{
    char c1, c2;
    
    if (s1 == NULL || s2 == NULL) {
        return false;
    }

    for (c1 = *s1, c2 = *s2; c1 != '\0' && c2 != '\0'; c1 = *(++s1), c2 = *(++s2)) {
        if (c1 != c2) {
            return false;
        }
    }
    if (*s1 == '\0' && *s2 == '\0') {
        return true;
    }

    return false;
}

bool string_is_same_by_n(const char *s1, const char *s2, unsigned int n)
{
    char c1, c2;
    
    if (NULL == s1 || NULL == s2 || 0 == n) {
        return false;
    }

    for (c1 = *s1, c2 = *s2; n != 0 && c1 != '\0' && c2 != '\0'; n--, c1 = *(++s1), c2 = *(++s2)) {
        if (c1 != c2) {
            return false;
        }
    }
    if (n == 0u) {
        return true;
    }
    if (c1 == '\0' && c2 == '\0') {
        return true;
    }

    return false;
}

unsigned int string_copy(char *d, const char *s, unsigned int n)
{
    unsigned int l = 0;
    
    if ((NULL == d) || (NULL == s) || (n < 1u)) {
        return 0;
    }

    --n;
    while ('\0' != *s && n != 0u) {
        *d = *s;
        d++;
        s++;
        n--;
        l++;
    }
    *d = '\0';

    return l;
}

//! find s2 in s1.
char *find_string(const char *s1, const char *s2)
{
    unsigned int n;
    
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    if (*s2 != '\0') {
        for (; *s1 != '\0'; s1++) {
            for (n = 0; s1[n] == s2[n]; n++) {
                if ('\0' == s2[n + 1]) {
                    return (char *)s1;
                }
            }
        }
    } else {
        for (; *s1 != '\0'; s1++);
        return (char *)s1;
    }
    
    return NULL;
}

//! find s2 in s1 by the first n chars.
char *find_string_by_n(const char *s1, const char *s2, size_t len)
{
    size_t len2;

    len2 = strlen(s2);
    if (0u == len2) {
        return (char *)s1;
    }
    
    while (len >= len2) {
        len--;
        if (0 == memcmp(s1, s2, len2)) {
            return (char *)s1;
        }
        s1++;
    }
    return NULL;
}


/*! \brief  Paser string to tokens, replace delim to '\0' and return pointer to token.
 *!         String should be end with '\0'. 
 *! \param ppStringRef address of point varible point to string that to paser.
 *!         ppStringRef will point the char next to the delim when this call return.
 *! \param pDelim point to delim string, every char in this sting represent as a delim.
 *! \retval address of token, this token may be empty string:"\0".
 *! \retval NULL there is no token in specify string.
 */
char *strsep(char **ppStringRef, const char *pDelim)
{
	char *s, *tok;
	const char *spanp;
	char c, sc;

    if (ppStringRef == NULL || pDelim == NULL) {
        return false;
    }

    s = *ppStringRef;
    if (s == NULL) {
        return NULL;
    }

    tok = s;
	for (;;) {
        c = *s;
        if (c == '\0') {
            *ppStringRef = NULL;
            return tok;
        } else {
		    for (spanp = pDelim; *spanp != '\0'; spanp++) {
                sc = *spanp;
			    if (sc == c) {
					*s = '\0';
				    *ppStringRef = s + 1;
                    return tok;
			    }
		    }
        }
        s++;
	}
}


uint32_t itostr(int32_t value, char *integerString, int32_t radix)
{
    const static unsigned char c[16] = "0123456789ABCDEF";
    uint32_t wValue = value;
    uint32_t l = 0;

    if (radix < 0) {
        radix = -radix;

        if (value < 0) {
            wValue = -value;
        }
    }

    if ((NULL == integerString)
    || ((10 != radix) && (16 != radix))) {
        return 0;
    }

    if (0 == wValue) {
        *integerString++ = c[0];
        l = 1;
    } else {
        uint32_t i = 0;
        uint32_t tmp;
        if (10 == radix) {         // decimal
            const static uint32_t muntable[10] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
            for (i = 0; i < 10u; i++) {
                tmp = wValue / muntable[i];
                if (tmp != 0u) {
                    break;
                }
            }
            l = 10 - i;
            for (; i < 10u; i++) {
                tmp = wValue / muntable[i];
                *integerString = c[tmp];
                wValue -= tmp * muntable[i];
                integerString++;
            }
        } else {                    // hex
            for (i = 0; i < 8u; i++) {
                tmp = (wValue >> ((7 - i) * 4)) & 0x0Fu;
                if (tmp != 0u) {
                    break;
                }
            }
            l = 8 - i;
            for (; i < 8u; i++) {
                tmp = (wValue >> ((7 - i) * 4)) & 0x0Fu;
                *integerString = c[tmp];
                integerString++;
            }
        }
    }
    *integerString = '\0';

    return l;
}


#define PRINT(__PFORMAT, __PRINT_OUT, __ARG)                                    \
    for (;;) {                                                                  \
        if ('\0' == *__PFORMAT) {                                               \
            break;                                                              \
        } else if ('%' == *__PFORMAT) {                                         \
            char buf[11];                                                       \
            char fill = ' ';                                                    \
            int i = 0, pw = 0;                                                  \
            int pl = -1;                                                        \
                                                                                \
            __PFORMAT++;                                                        \
            if (*__PFORMAT >= '0' && *__PFORMAT <= '9') {                       \
                if (*__PFORMAT == '0') {                                        \
                    fill = '0';                                                 \
                    __PFORMAT++;                                                \
                }                                                               \
                                                                                \
                while (*__PFORMAT >= '0' && *__PFORMAT <= '9') {                \
                    pl = pl * 10 + (*__PFORMAT - '0');                          \
                    ++__PFORMAT;                                                \
                }                                                               \
            }                                                                   \
            if ('.' == *__PFORMAT) {                                            \
                __PFORMAT++;                                                    \
                pl = 0;                                                         \
                if (*__PFORMAT >= '0' && *__PFORMAT <= '9') {                   \
                    while (*__PFORMAT >= '0' && *__PFORMAT <= '9') {            \
                        pl = pl * 10 + (*__PFORMAT - '0');                      \
                        ++__PFORMAT;                                            \
                    }                                                           \
                }                                                               \
            }                                                                   \
            if ('\0' == *__PFORMAT) {                                           \
                break;                                                          \
            }                                                                   \
                                                                                \
            switch (*__PFORMAT) {                                               \
                case 'c':                                                       \
                {                                                               \
                    char c = (char)__ARG;                                       \
                    __PRINT_OUT(c);                                             \
                    break;                                                      \
                }                                                               \
                case 's':                                                       \
                {                                                               \
                    const char *s = (const char *)__ARG;                        \
                    if (NULL == s) {                                            \
                        break;                                                  \
                    }                                                           \
                                                                                \
                    i = string_length(s);                                       \
                    if (pl >= 0) {                                              \
                        pl = pl < i ? pl : i;                                   \
                    } else {                                                    \
                        pl = i;                                                 \
                    }                                                           \
                                                                                \
                    if (pw < pl) {                                              \
                        pw = pl;                                                \
                    }                                                           \
                                                                                \
                    pw -= pl;                                                   \
                    for (i = 0; i < pw; ++i) {                                  \
                        __PRINT_OUT(fill);                                      \
                    }                                                           \
                                                                                \
                    for (; pl; pl--) {                                          \
                        __PRINT_OUT(*s);                                        \
                        s++;                                                    \
                    }                                                           \
                    break;                                                      \
                }                                                               \
                case 'u':                                                       \
                {                                                               \
                    unsigned int d = (unsigned int)__ARG;                       \
                    itostr(d, buf, 10);                                         \
                                                                                \
                    i = string_length(buf);                                     \
                    if (pl < i) {                                               \
                        pl = i;                                                 \
                    }                                                           \
                                                                                \
                    pl -= i;                                                    \
                    for (i = 0; i < pl; ++i) {                                  \
                        __PRINT_OUT(fill);                                      \
                    }                                                           \
                                                                                \
                    for (i = 0; buf[i]; ++i) {                                  \
                        __PRINT_OUT(buf[i]);                                    \
                    }                                                           \
                    break;                                                      \
                }                                                               \
                case 'i':                                                       \
                case 'd':                                                       \
                {                                                               \
                    int n = (int)__ARG;                                         \
                    itostr(n, buf, -10);                                        \
                                                                                \
                    i = string_length(buf);                                     \
                    if (n < 0) {                                                \
                        i += 1;                                                 \
                    }                                                           \
                    if (pl < i) {                                               \
                        pl = i;                                                 \
                    }                                                           \
                                                                                \
                    pl -= i;                                                    \
                    if (n >= 0) {                                               \
                        for (i = 0; i < pl; ++i) {                              \
                            __PRINT_OUT(fill);                                  \
                        }                                                       \
                    } else {                                                    \
                        if (fill == '0') {                                      \
                            __PRINT_OUT('-');                                   \
                            for (i = 0; i < pl; ++i) {                          \
                                __PRINT_OUT('0');                               \
                            }                                                   \
                        } else {                                                \
                            for (i = 0; i < pl; ++i) {                          \
                                __PRINT_OUT(' ');                               \
                            }                                                   \
                            __PRINT_OUT('-');                                   \
                        }                                                       \
                    }                                                           \
                                                                                \
                    for (i = 0; buf[i]; ++i) {                                  \
                        __PRINT_OUT(buf[i]);                                    \
                    }                                                           \
                    break;                                                      \
                }                                                               \
                case 'x':                                                       \
                case 'X':                                                       \
                {                                                               \
                    unsigned int d = (unsigned int)__ARG;                       \
                    itostr(d, buf, 16);                                         \
                                                                                \
                    i = string_length(buf);                                     \
                    if (pl < i) {                                               \
                        pl = i;                                                 \
                    }                                                           \
                                                                                \
                    pl -= i;                                                    \
                    for (i = 0; i < pl; ++i) {                                  \
                        __PRINT_OUT(fill);                                      \
                    }                                                           \
                                                                                \
                    if ('X' == *__PFORMAT) {                                    \
                        for (i = 0; buf[i]; ++i) {                              \
                            __PRINT_OUT(buf[i]);                                \
                        }                                                       \
                    } else {                                                    \
                        for (i = 0; buf[i]; ++i) {                              \
                            if (buf[i] > '@') {                                 \
                                buf[i] = ' ' + buf[i];                          \
                            }                                                   \
                            __PRINT_OUT(buf[i]);                                \
                        }                                                       \
                    }                                                           \
                    break;                                                      \
                }                                                               \
                case 'p':                                                       \
                {                                                               \
                    uintptr_t d = (uintptr_t)__ARG;                             \
                    itostr(d, buf, 16);                                         \
                                                                                \
                    if (pl < (sizeof(uintptr_t) * 2)) {                         \
                        pl = (sizeof(uintptr_t) * 2);                           \
                    }                                                           \
                    pl -= (sizeof(uintptr_t) * 2);                              \
                    for (i = 0; i < pl; ++i) {                                  \
                        __PRINT_OUT(' ');                                       \
                    }                                                           \
                                                                                \
                    i = string_length(buf);                                     \
                    pl = (sizeof(uintptr_t) * 2) - i;                           \
                    for (i = 0; i < pl; i++) {                                  \
                        __PRINT_OUT('0');                                       \
                    }                                                           \
                                                                                \
                    for (i = 0; buf[i]; ++i) {                                  \
                        __PRINT_OUT(buf[i]);                                    \
                    }                                                           \
                    break;                                                      \
                }                                                               \
                default:                                                        \
                    __PRINT_OUT(*__PFORMAT);                                    \
                    break;                                                      \
            }                                                                   \
        } else {                                                                \
            __PRINT_OUT(*__PFORMAT);                                            \
        }                                                                       \
        __PFORMAT++;                                                            \
    }                                                                           \


extern int user_printf_output_char(char cChar);

void string_printf(const char *formatString, void *argument)
{
    if (formatString == NULL) {
        return;
    }
    PRINT(formatString, user_printf_output_char, argument)
}

#define USER_SPRINTFN_OUT(__C)                                                  \
    do {                                                                        \
        if (wCnt >= bufferLength) {                                             \
            return wCnt;                                                        \
        }                                                                       \
                                                                                \
        buffer[wCnt] = __C;                                                     \
        wCnt++;                                                                 \
    } while (0)

//! \brief  print string to a buffer and return the string's length.
unsigned int string_printf_to_buffer(char          *buffer,
                                     unsigned int   bufferLength,
                                     const char    *formatString,
                                     void          *argument)
{
    unsigned int wCnt = 0;

    if (NULL == buffer || 0 == bufferLength || formatString == NULL) {
        return 0;
    }

    PRINT(formatString, USER_SPRINTFN_OUT, argument)

    return wCnt;
}


/* EOF */
