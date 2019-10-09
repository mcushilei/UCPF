/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include "./num_string_array.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
enum {
    IS_INVALID =  0,
    IS_NUM =      1,
    IS_SIGN =     2,
    IS_BLANK =    3,
    IS_DOT   =    4,
    IS_DILIM =    5,
    IS_NUL =      6,
};

static const char TYPE_OF[1 << CHAR_BIT] = {
/*
    This array maps the 128 ASCII characters into character classes.
    The remaining Unicode characters should be mapped to C_ETC.
    Non-whitespace control characters are errors.
*/
    //NUL SOH STX ETX EOT ENQ ACK BEL
    //BS  HT  LF  VT  FF  CR  SO  SI
    //DLE DC1 DC2 DC3 DC4 NAK SYN ETB
    //CAN EM  SUB ESC FS  GS  RS  US
    IS_NUL,          IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_BLANK,        IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,

    //SP    !       "       #       $       %       &       '
    //(     )       *       +       ,       -       .       /
    //0     1       2       3       4       5       6       7
    //8     9       :       ;       <       =       >       ?
    IS_BLANK,       IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,
    IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_SIGN,        IS_DILIM,       IS_SIGN,        IS_DOT,         IS_INVALID,
    IS_NUM,         IS_NUM,         IS_NUM,         IS_NUM,         IS_NUM,         IS_NUM,         IS_NUM,         IS_NUM,
    IS_NUM,         IS_NUM,         IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,     IS_INVALID,

    //@     A       B       C       D       E       F       G
    //H     I       J       K       L       M       N       O
    //P     Q       R       S       T       U       V       W
    //X     Y       Z       [       \       ]       ^       _
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,

    //`     a       b       c       d       e       f       g
    //h     i       j       k       l       m       n       o
    //p     q       r       s       t       u       v       w
    //x     y       z       {       |       }       ~       DEL
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
    IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,      IS_INVALID,
};

enum {
    STATUS_STA = 0,
    STATUS_SIN,
    STATUS_NUM,
    STATUS_DIM,
    STATUS_DOT,
    STATUS_BLK_PRE,
    STATUS_BLK_SUR,
    STATUS_NUL,
    STATUS_END,
};

static const char state_to[][7] = {
    //! if char is  IS_INVALID,     IS_NUM,     IS_SIGN,    IS_BLANK,       IS_DOT,     IS_DILIM,   IS_NUL          and
                    {STATUS_END,    STATUS_NUM, STATUS_SIN, STATUS_BLK_PRE, STATUS_END, STATUS_END, STATUS_END},//! current state is STATUS_STA
                    {STATUS_END,    STATUS_NUM, STATUS_END, STATUS_END,     STATUS_END, STATUS_END, STATUS_END},//! current state is STATUS_SIN
                    {STATUS_END,    STATUS_NUM, STATUS_END, STATUS_BLK_SUR, STATUS_DOT, STATUS_DIM, STATUS_NUL},//! current state is STATUS_NUM
                    {STATUS_END,    STATUS_NUM, STATUS_SIN, STATUS_BLK_PRE, STATUS_END, STATUS_END, STATUS_END},//! current state is STATUS_DIM
                    {STATUS_END,    STATUS_NUM, STATUS_END, STATUS_END,     STATUS_END, STATUS_END, STATUS_END},//! current state is STATUS_DOT
                    {STATUS_END,    STATUS_NUM, STATUS_SIN, STATUS_BLK_PRE, STATUS_END, STATUS_END, STATUS_END},//! current state is STATUS_BLK_PRE
                    {STATUS_END,    STATUS_END, STATUS_END, STATUS_BLK_SUR, STATUS_END, STATUS_DIM, STATUS_NUL},//! current state is STATUS_BLK_SUR
};

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


/* the array must be delimited by ',' and terminated by '\0'. */
int check_num_array(const char *string)
{
    uint8_t next = STATUS_STA;
    int32_t length = 0;
    const char *pchar = string;

    do {
        next = state_to[next][TYPE_OF[*(pchar++)]];

        if (STATUS_END == next) {
            return -1;
        } else if (STATUS_NUL == next) {
            length++;
            return length;
        } else if (STATUS_DIM == next) {
            length++;
        }

    } while (1);
}


int num_array_string_to_int(const char *string, int *values, uint32_t valuesLength)
{
    uint8_t next = STATUS_STA;
    int32_t cnt = 0;
    const char *pchar = string;
    float value = 0.0f;

    do {
        next = state_to[next][TYPE_OF[*(pchar++)]];

        if (STATUS_END == next) {
            return -1;
        } else if (STATUS_NUL == next) {

            sscanf(string, "%f", &value);
            values[cnt] = value;

            cnt++;
            return cnt;
        } else if (STATUS_DIM == next) {

            sscanf(string, "%f", &value);
            values[cnt] = value;

            cnt++;
            if (cnt >= valuesLength) {
                return cnt;
            }
            string = pchar;
        }

    } while (1);
}

int num_array_string_to_float(const char *string, float *values, uint32_t valuesLength)
{
    uint8_t next = STATUS_STA;
    int32_t cnt = 0;
    const char *pchar = string;
    float value = 0.0f;

    do {
        next = state_to[next][TYPE_OF[*(pchar++)]];

        if (STATUS_END == next) {
            return -1;
        } else if (STATUS_NUL == next) {

            sscanf(string, "%f", &value);
            values[cnt] = value;

            cnt++;
            return cnt;
        } else if (STATUS_DIM == next) {

            sscanf(string, "%f", &value);
            values[cnt] = value;

            cnt++;
            if (cnt >= valuesLength) {
                return cnt;
            }
            string = pchar;
        }

    } while (1);
}

/* EOF */
