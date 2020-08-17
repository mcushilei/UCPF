/*******************************************************************************
 *  Copyright(C)2015-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./debug_plug.h"
#include "./debug.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define DEBUG_PRINT_EOL {                            \
    debug_output_char('\r');                         \
    debug_output_char('\n');                         \
}

#if DEBUG_DISALLOW_FILE_INFO == ENABLED
#   define DEBUG_PRINT_LOCATION(__FILE, __LINE)
#else 
#   define DEBUG_PRINT_LOCATION(__FILE, __LINE) {   \
        debug_print_string(__FILE);                 \
        debug_output_char(':');                     \
        debug_print_number_unsigned(__LINE);        \
        debug_output_char('>');                     \
    }
#endif

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void debug_print_number_signed(const _SINT number);
static void debug_print_number_unsigned(const _UINT number);
static void debug_print_number_hex(const _UINT number, const _UINT lengthToPrint);
static void debug_print_mask(const _UINT mask, const _UINT number);

/*============================ LOCAL VARIABLES ===============================*/
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrFail[]      = "[Err]";
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrMessage[]   = "[Msg]";
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrNull[]      = "NULL";
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrExpected[]  = "Expected ";
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrWas[]       = " Was ";
static DEBUG_ROM_VAR_TYPE const _CHAR DebugStrNullPointer[]   = "Pointer was NULL.";

static volatile _CHAR exitTrap = 0; 

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
int debug_string_compare(const _CHAR *expected, const _CHAR *actual)
{
    if (expected && actual) {
        for (; *expected || *actual;) {
            if (*expected++ != *actual++) {
                return 0;
            }
        }
    } else if (expected || actual) {
        return 0;
    }

    return 1;
}

//-----------------------------------------------
// Pretty Printers
//-----------------------------------------------
void debug_print_string(const _CHAR *string)
{
    if (string != NULL) {
        for (; *string != '\0'; string++) {
            debug_output_char(*string);
        }
    }
}

//-----------------------------------------------
//! basically do an itoa using as little ram as possible
static void debug_print_number_unsigned(const _UINT number)
{
    _UINT divisor = 1;
    _UINT nextDivisor;

    // figure out initial divisor
    while (number / divisor > 9) {
        nextDivisor = divisor * 10;
        if (nextDivisor > divisor) {
            divisor = nextDivisor;
        } else {
            break;
        }
    }

    // now mod and print, then divide divisor
    do {
        debug_output_char('0' + (number / divisor % 10));
        divisor /= 10;
    } while (divisor > 0);
}

//-----------------------------------------------
//! basically do an itoa using as little ram as possible
static void debug_print_number_signed(const _SINT number)
{
    if (number < 0) {
        debug_output_char('-');
        debug_print_number_unsigned((_UINT)-number);
    } else {
        debug_print_number_unsigned((_UINT)number);
    }
}

//-----------------------------------------------
static void debug_print_number_hex(const _UINT number, const _UINT lengthToPrint)
{
    _UINT nibble;
    _UINT l = lengthToPrint;

    if (l > (sizeof(_UINT) * 2u)) {
        l = sizeof(_UINT) * 2u;
    }

    for (; l; --l) {
        nibble = (number >> ((l - 1u) * 4u)) & 0x0Fu;
        if (nibble <= 9) {
            debug_output_char('0' + nibble);
        } else {
            debug_output_char('A' - 10u + nibble);
        }
    }
}

//-----------------------------------------------
static void debug_print_number_by_style(const _SINT number, const unsigned int style)
{
    switch (style) {
        case DEBUG_DISPLAY_STYLE_INT:
            debug_print_number_signed((_SINT)number);
            break;

        case DEBUG_DISPLAY_STYLE_UINT:
            debug_print_number_unsigned((_UINT)number);
            break;

        case DEBUG_DISPLAY_STYLE_HEX:
        case DEBUG_DISPLAY_STYLE_POINTER:
        default:
            debug_print_number_hex((_UINT)number, 2u * sizeof(_UINT));
            break;
    }
}

//-----------------------------------------------
static void debug_print_mask(const _UINT mask, const _UINT number)
{
    _UINT current_bit = (_UINT)1 << (DEBUG_INT_WIDTH - 1);
    _UINT i;

    for (i = 0; i < DEBUG_INT_WIDTH; i++) {
        if (current_bit & mask) {
            if (current_bit & number) {
                debug_output_char('1');
            } else {
                debug_output_char('0');
            }
        } else {
            debug_output_char('X');
        }
        current_bit = current_bit >> 1;
    }
}

//-----------------------------------------------
// Debug Result Output Handlers
//-----------------------------------------------
//-----------------------------------------------
void debug_print_null_point(void)
{
    debug_print_string(DebugStrNullPointer);
}

//-----------------------------------------------
void debug_print_expected_actual_string(const _CHAR *expected, const _CHAR *actual)
{
    debug_print_string(DebugStrExpected);
    if (expected != NULL) {
        debug_output_char('\"');
        debug_print_string(expected);
        debug_output_char('\"');
    } else {
      debug_print_string(DebugStrNull);          
    }

    debug_print_string(DebugStrWas);
    if (actual != NULL) {
        debug_output_char('\"');
        debug_print_string(actual);
        debug_output_char('\"');
    } else {
      debug_print_string(DebugStrNull);          
    }
}

//-----------------------------------------------
void debug_print_equal_bits(
    const _UINT mask,
    const _UINT expected,
    const _UINT actual)
{
    debug_print_string(DebugStrExpected);
    debug_print_mask(mask, expected);
    debug_print_string(DebugStrWas);
    debug_print_mask(mask, actual);
}

//-----------------------------------------------
void debug_print_equal_number(
    const _SINT expected, 
    const _SINT actual, 
    const unsigned int style)
{
    debug_print_string(DebugStrExpected);
    switch (style) {
        case DEBUG_DISPLAY_STYLE_INT:
            debug_print_number_signed(expected);
            break;

        case DEBUG_DISPLAY_STYLE_UINT:
            debug_print_number_unsigned(expected);
            break;

        case DEBUG_DISPLAY_STYLE_HEX:
        case DEBUG_DISPLAY_STYLE_POINTER:
        default:
            debug_print_number_hex(expected, 2u * sizeof(_UINT));
            break;
    }

    debug_print_string(DebugStrWas);
    switch (style) {
        case DEBUG_DISPLAY_STYLE_INT:
            debug_print_number_signed(actual);
            break;

        case DEBUG_DISPLAY_STYLE_UINT:
            debug_print_number_unsigned(actual);
            break;

        case DEBUG_DISPLAY_STYLE_HEX:
        case DEBUG_DISPLAY_STYLE_POINTER:
        default:
            debug_print_number_hex(actual, 2u * sizeof(_UINT));
            break;
    }
}

//-----------------------------------------------
// Control Functions
//-----------------------------------------------
void debug_failure_captured(const _CHAR *file, const _UINT line)
{
    DEBUG_PRINT_EOL
    debug_print_string(DebugStrFail);
    DEBUG_PRINT_LOCATION(file, line)
}

void debug_msg_output(const _CHAR *file, const _UINT line)
{
    DEBUG_PRINT_EOL
    debug_print_string(DebugStrMessage);
    DEBUG_PRINT_LOCATION(file, line)
}

void debug_trap(void)
{
    while (!exitTrap);
}

void debug_exit_trap(void)
{
    exitTrap = 1;
}

/* EOF */
