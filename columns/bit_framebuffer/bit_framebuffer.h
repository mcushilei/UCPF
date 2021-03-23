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

#ifndef __BIT_FRAMEBUFFER_H__
#define __BIT_FRAMEBUFFER_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __BFB_LSB(v)    (v)
#define __BFB_MSB(v)    (~(v))

/*
    BFB_DRAW_POINT(gram, x, y, color, RC, LSB);
    BFB_DRAW_POINT(gram, x, y, color, RC, MSB);
 */
#define BFB_DRAW_POINT(gram, width, high, x, y, color, scan_order, bit_order)   \
    BFB_DRAW_POINT_##scan_order(gram, width, high, x, y, color, bit_order)

#define BFB_SCAN(gram, width, high, sop, pro, scan_order)    \
    BFB_SCAN_##scan_order(gram, width, high, sop, pro)

/* column-column, LSB
  gram[0][0] gram[1][0] ...     gram[127][0]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

  gram[0][1] gram[1][1] ...     gram[127][1]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

      .
      .
      .

  gram[0][7] gram[1][7] ...     gram[127][7]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

*/
#define BFB_DRAW_POINT_CC( gram, width, high, x, y, color, bit_order )  \
    do {                                \
        uint8_t byte, bit, mask, nmask; \
        uint32_t index;     \
                            \
        if (x >= width) {   \
            break;          \
        }                   \
        if (y >= high) {    \
            break;          \
        }                   \
                            \
        byte = y >> 3;      \
        index = x * (high / 8u) + byte;         \
        bit  = __BFB_##bit_order(y) & 0x07u;    \
        mask = 1 << bit;            \
        nmask = ~mask;              \
                                    \
        if( color ) {               \
            gram[index] |= mask;    \
        }                           \
        else {                      \
            gram[index] &= nmask;   \
        }                           \
    } while (0)

#define BFB_SCAN_CC( gram, width, high, sop, pro )  \
    do {                \
        uint32_t i, j;  \
        uint8_t *page;  \
                        \
        for (i = 0; i < width; i++) {   \
            sop(i);                     \
            page = gram + i * (high / 8u);      \
            for (j = 0; j < (high / 8u); j++) { \
                pro(page[j]);                   \
            }                                   \
        }                                       \
    } while (0)

#define BFB_SCAN_CC_AS_RC( gram, width, high, sop, pro )  \
    do {                \
        uint32_t i, j;  \
                        \
        for (i = 0; i < (high / 8u); i++) { \
            sop(i);                         \
            for (j = 0; j < width; j++) {   \
                pro(gram[j * (high / 8u) + i]); \
            }                               \
        }                                   \
    } while (0)


/* row-column, LSB
  gram[0][0] gram[0][1] ...     gram[0][127]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

  gram[1][0] gram[1][1] ...     gram[1][127]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

      .
      .
      .

  gram[7][0] gram[7][1] ...    gram[7][127]
      b0         b0                 b0
      b1         b1                 b1
      .          .                  .
      .          .                  .
      b7         b7                 b7

*/
#define BFB_DRAW_POINT_RC( gram, width, high, x, y, color, bit_order )   \
    do {                                        \
        uint8_t byte, bit, mask, nmask;         \
        uint32_t index;                 \
                            \
        if (x >= width) {   \
            break;          \
        }                   \
        if (y >= high) {    \
            break;          \
        }                   \
                                \
        byte = x;           \
        index = (y >> 3) * width + byte;        \
        bit  = __BFB_##bit_order(y) & 0x07u;    \
        mask = 1 << bit;            \
        nmask = ~mask;              \
                                    \
        if( color ) {               \
            gram[index] |= mask;    \
        }                           \
        else {                      \
            gram[index] &= nmask;   \
        }                           \
    } while (0)

#define BFB_SCAN_RC( gram, width, high, sop, pro )  \
    do {                \
        uint32_t i, j;  \
        uint8_t *page;  \
                        \
        for (i = 0; i < (high / 8u); i++) { \
            sop(i);                         \
            page = gram + i * width;        \
            for (j = 0; j < width; j++) {   \
                pro(page[j]);               \
            }                               \
        }                                   \
    } while (0)

#define BFB_SCAN_RC_AS_CC( gram, width, high, sop, pro )  \
    do {                \
        uint32_t i, j;  \
                        \
        for (i = 0; i < width; i++) {   \
            sop(i);                     \
            for (j = 0; j < (high / 8u); j++) { \
                pro(gram[i * width + j]);       \
            }                                   \
        }                                       \
    } while (0)


/* column-row, LSB
  gram[0][0] gram[1][0] ...     gram[16][0]
  b0 ... b7  b0 ... b7          b0 ... b7

  gram[0][1] gram[1][1] ...     gram[16][1]
  b0 ... b7  b0 ... b7          b0 ... b7

      .
      .
      .

  gram[0][63] gram[1][63] ... gram[16][63]
  b0 ... b7  b0 ... b7          b0 ... b7

*/
#define BFB_DRAW_POINT_CR( gram, width, high, x, y, color, bit_order )   \
    BFB_DRAW_POINT_RC( gram, high, width, y, x, color, bit_order )

#define BFB_SCAN_CR( gram, width, high, sop, pro )  \
    BFB_SCAN_RC( gram, high, width, sop, pro )

#define BFB_SCAN_CR_AS_RR( gram, width, high, sop, pro )  \
    BFB_SCAN_RC_AS_CC( gram, high, width, sop, pro )

/* row-row, LSB
  gram[0][0] gram[0][1] ...     gram[0][16]
  b0 ... b7  b0 ... b7          b0 ... b7

  gram[1][0] gram[1][1] ...     gram[1][16]
  b0 ... b7  b0 ... b7          b0 ... b7

      .
      .
      .

  gram[63][0] gram[63][1] ...  gram[63][16]
  b0 ... b7  b0 ... b7          b0 ... b7

*/
#define BFB_DRAW_POINT_RR( gram, width, high, x, y, color, bit_order )   \
    BFB_DRAW_POINT_CC( gram, high, width, y, x, color, bit_order )

#define BFB_SCAN_RR( gram, width, high, sop, pro )  \
    BFB_SCAN_CC( gram, high, width, sop, pro )

#define BFB_SCAN_RR_AS_CR( gram, width, high, sop, pro )  \
    BFB_SCAN_CC_AS_RC( gram, high, width, sop, pro )

/*============================ TYPES =========================================*/
typedef struct {
    uint32_t Width;
    uint32_t High;
} bfb_info_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/

#endif  //!< #ifndef __BIT_FRAMEBUFFER_H__
/* EOF */
