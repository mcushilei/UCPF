/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include <stdio.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void decode_hex_str(const char *str, int strlen, char *bufout)
{
    static const char tbl[256] = {
    //NUL SOH STX ETX EOT ENQ ACK BEL
    //BS  HT  LF  VT  FF  CR  SO  SI
    //DLE DC1 DC2 DC3 DC4 NAK SYN ETB
    //CAN EM  SUB ESC FS  GS  RS  US
    0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,
    0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,
    0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,
    0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,      0x00,

    //SP  !   "   #   $   %   &   '
    //(   )   *   +   ,   -   .   /
    //0   1   2   3   4   5   6   7
    //8   9   :   ;   <   =   >   ?
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x01,   0x02,   0x03,   0x04,   0x05,   0x06,   0x07,
    0x08,   0x09,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,

    //@   A   B   C   D   E   F   G
    //H   I   J   K   L   M   N   O
    //P   Q   R   S   T   U   V   W
    //X   Y   Z   [   \   ]   ^   _
    0x00,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,

    //`   a   b   c   d   e   f   g
    //h   i   j   k   l   m   n   o
    //p   q   r   s   t   u   v   w
    //x   y   z   {   |   }   ~   DEL
    0x00,   0x0a,   0x0b,   0x0c,   0x0d,   0x0e,   0x0f,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,
    0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00
    };
    char highByte, lowByte;
    for (int i = 0; i < strlen; i += 2) {
        highByte = tbl[(str[i])];
        lowByte  = tbl[(str[i + 1])];
        bufout[i >> 1] = (highByte << 4) | lowByte;
    }
}



void encode_hex_str(const char *data, int datalen, char *str)
{
    uint8_t *p = (uint8_t *)data;
    for (int i = 0; i < datalen; i++) {
        sprintf(str + i * 2, "%02X", p[i]);
    }
}

/* EOF */
