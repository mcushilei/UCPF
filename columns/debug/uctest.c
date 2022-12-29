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
#if !defined(UCTEST_EOL)
#   define UCTEST_EOL        "\r\n"
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#if UCTEST_ALLOW_FILE_INFO == 1
#   define PRINT_LOCATION(__FILE, __LINE)  do {\
        uctest_output_char('[');                     \
        print_string(__FILE);                 \
        uctest_output_char(':');                     \
        print_number_unsigned(__LINE);        \
        uctest_output_char(']');                     \
        uctest_output_char(' ');                     \
    } while (0)
#else 
#   define PRINT_LOCATION(__FILE, __LINE)
#endif

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void print_number_signed(const _SINT number);
static void print_number_unsigned(const _UINT number);
static void print_number_hex(const _UINT number, const _UINT lengthToPrint);
static void print_bin(const _UINT mask, const _UINT number);

/*============================ LOCAL VARIABLES ===============================*/
static const char strEOL[]          = UCTEST_EOL;
static const char strError[]        = "[XXX]";
static const char strNull[]         = "NULL";
static const char strExpected[]     = "Expected ";
static const char strWas[]          = " Was ";
static const char strNullPointer[]  = "NULL Pointer!";
static const char strNotTrue[]      = "Not true!";
static const char strReport[]       = "============== Test Report ==============";
static const char strTestName[]     = "    Test     : ";
static const char strHitTest[]      = "    Hit      : ";
static const char strFailTest[]     = "    Filure   : ";
static const char strReportEnd[]    = "=========================================";

static const char *testName = NULL;
static _UINT hitCount = 0;
static _UINT errorCount = 0;

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
void uctest_print_eol( void )
{
    print_string( strEOL );
}

void uctest_print_not_true( void )
{
    print_string( strNotTrue );
}

void uctest_print_null_point(void)
{
    print_string(strNullPointer);
}

void uctest_print_expected_actual_string(const char *expected, const char *actual)
{
    print_string(strExpected);
    if (expected != NULL) {
        uctest_output_char('\"');
        print_string(expected);
        uctest_output_char('\"');
    } else {
      print_string(strNull);          
    }

    print_string(strWas);
    if (actual != NULL) {
        uctest_output_char('\"');
        print_string(actual);
        uctest_output_char('\"');
    } else {
      print_string(strNull);          
    }
}

void uctest_print_equal_bits(
    const _UINT mask,
    const _UINT expected,
    const _UINT actual)
{
    print_string(strExpected);
    print_bin(mask, expected);
    print_string(strWas);
    print_bin(mask, actual);
}

void uctest_print_equal_number(
    const _SINT expected, 
    const _SINT actual, 
    const unsigned int style)
{
    print_string(strExpected);
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

    print_string(strWas);
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
void uctest_init( const char *name )
{
    testName = name;
    hitCount = 0;
    errorCount = 0;
}

void uctest_hit( void )
{
    hitCount++;
}

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
    print_string( strError );
    PRINT_LOCATION( file, line );

    errorCount++;
}

void uctest_report( const char *extraInfo )
{
    print_string( strReport );
    uctest_print_eol();
    if( NULL != testName ) {
        print_string( strTestName );
        print_string( testName );
        uctest_print_eol();
    }
    print_string( strHitTest );
    print_number_unsigned( hitCount );
    uctest_print_eol();
    print_string( strFailTest );
    print_number_unsigned( errorCount );
    uctest_print_eol();
    if( NULL != extraInfo ) {
        print_string( extraInfo );
        uctest_print_eol();
    }
    print_string( strReportEnd );
    uctest_print_eol();
}

/* EOF */
