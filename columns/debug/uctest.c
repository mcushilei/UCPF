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



/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./uctest.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define PRINT_EOL() do {                       \
    uctest_output_char('\r');                         \
    uctest_output_char('\n');                         \
} while (0)

#if UCTEST_DISALLOW_FILE_INFO == ENABLED
#   define PRINT_LOCATION(__FILE, __LINE)
#else 
#   define PRINT_LOCATION(__FILE, __LINE)  do {\
        uctest_output_char('[');                     \
        print_string(__FILE);                 \
        uctest_output_char(':');                     \
        print_number_unsigned(__LINE);        \
        uctest_output_char(']');                     \
        uctest_output_char(' ');                     \
    } while (0)
#endif

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void print_number_signed(const _SINT number);
static void print_number_unsigned(const _UINT number);
static void print_number_hex(const _UINT number, const _UINT lengthToPrint);
static void print_bin(const _UINT mask, const _UINT number);

/*============================ LOCAL VARIABLES ===============================*/
static const char DebugStrError[]       = "[XXX]";
static const char DebugStrNull[]        = "NULL";
static const char DebugStrExpected[]    = "Expected ";
static const char DebugStrWas[]         = " Was ";
static const char DebugStrNullPointer[] = "NULL Pointer!";

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void print_string(const char *string)
{
    if( string == NULL ) {
        return;
    }
    for (; *string != '\0'; string++) {
        uctest_output_char(*string);
    }
}

//! basically do an itoa using as little ram as possible
static void print_number_unsigned(const _UINT number)
{
    _UINT divisor = 1;

    // figure out initial divisor by counting the length of the number.
    while (number / divisor > 9u) {
        divisor *= 10u;
    }

    // now mod and print, then divide divisor
    do {
        uctest_output_char('0' + (number / divisor % 10u));
        divisor /= 10u;
    } while (divisor > 0);
}

//! basically do an itoa using as little ram as possible
static void print_number_signed(const _SINT number)
{
    if (number < 0) {
        uctest_output_char('-');
        print_number_unsigned((_UINT)-number);
    } else {
        print_number_unsigned((_UINT)number);
    }
}

static void print_number_hex(const _UINT number, const _UINT lengthToPrint)
{
    _UINT nibble;
    _UINT l = lengthToPrint;

    if (l > (sizeof(_UINT) * 2u)) {
        l = sizeof(_UINT) * 2u;
    }

    for (; l; --l) {
        nibble = (number >> ((l - 1u) * 4u)) & 0x0Fu;
        if (nibble <= 9) {
            uctest_output_char('0' + nibble);
        } else {
            uctest_output_char('A' - 10u + nibble);
        }
    }
}

static void print_bin(const _UINT mask, const _UINT number)
{
    _UINT current_bit = (_UINT)1 << (UCTEST_INT_WIDTH - 1);
    _UINT i;

    for (i = 0; i < UCTEST_INT_WIDTH; i++) {
        if (current_bit & mask) {
            if (current_bit & number) {
                uctest_output_char('1');
            } else {
                uctest_output_char('0');
            }
        } else {
            uctest_output_char('X');
        }
        current_bit = current_bit >> 1;
    }
}

//-----------------------------------------------
// Debug Result Output Handlers
//-----------------------------------------------
void uctest_print_null_point(void)
{
    print_string(DebugStrNullPointer);
}

void uctest_print_expected_actual_string(const char *expected, const char *actual)
{
    print_string(DebugStrExpected);
    if (expected != NULL) {
        uctest_output_char('\"');
        print_string(expected);
        uctest_output_char('\"');
    } else {
      print_string(DebugStrNull);          
    }

    print_string(DebugStrWas);
    if (actual != NULL) {
        uctest_output_char('\"');
        print_string(actual);
        uctest_output_char('\"');
    } else {
      print_string(DebugStrNull);          
    }
}

void uctest_print_equal_bits(
    const _UINT mask,
    const _UINT expected,
    const _UINT actual)
{
    print_string(DebugStrExpected);
    print_bin(mask, expected);
    print_string(DebugStrWas);
    print_bin(mask, actual);
}

void uctest_print_equal_number(
    const _SINT expected, 
    const _SINT actual, 
    const unsigned int style)
{
    print_string(DebugStrExpected);
    switch (style) {
        case UCTEST_DISPLAY_STYLE_INT:
            print_number_signed(expected);
            break;

        case UCTEST_DISPLAY_STYLE_UINT:
            print_number_unsigned(expected);
            break;

        case UCTEST_DISPLAY_STYLE_HEX:
        default:
            print_number_hex(expected, 2u * sizeof(_UINT));
            break;
    }

    print_string(DebugStrWas);
    switch (style) {
        case UCTEST_DISPLAY_STYLE_INT:
            print_number_signed(actual);
            break;

        case UCTEST_DISPLAY_STYLE_UINT:
            print_number_unsigned(actual);
            break;

        case UCTEST_DISPLAY_STYLE_HEX:
        default:
            print_number_hex(actual, 2u * sizeof(_UINT));
            break;
    }
}

//-----------------------------------------------
// Control Functions
//-----------------------------------------------
int uctest_string_compare( const char *expected, const char *actual )
{
    if( expected && actual ) {
        for( ; *expected || *actual;) {
            if( *expected++ != *actual++ ) {
                return 0;
            }
        }
    } else if( expected || actual ) {
        return 0;
    }

    return 1;
}

void uctest_failure_captured( const char *file, const _UINT line )
{
    PRINT_EOL();
    print_string( DebugStrError );
    PRINT_LOCATION( file, line );
}

/* EOF */
