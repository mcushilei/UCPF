/*******************************************************************************
 *  Copyright(C)2015-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __FIFO_H__
#define __FIFO_H__

/*  \brief this is a lockless ring-buffer between one producer and one consumer
 *  with a limitation that the buffer size must be a exponent of 2.
 */

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define FIFO_INIT(__obj, __buf, __size)     fifo_init( (fifo_t *)(__obj), __buf, __size )
#define FIFO_IN(__obj, __byte)              fifo_in( (fifo_t *)(__obj), __byte )
#define FIFO_OUT(__obj, __buf)              fifo_out( (fifo_t *)(__obj), __buf )
#define FIFO_BUST_IN(__obj, __buf, __len)   fifo_burst_in( (fifo_t *)(__obj), __buf, __len )
#define FIFO_BUST_OUT(__obj, __buf, __len)  fifo_burst_out( (fifo_t *)(__obj), __buf, __len )
#define FIFO_LENGTH(__obj)                  fifo_length( (fifo_t *)(__obj) )
#define FIFO_FLUSH(__obj)                   fifo_flush( (fifo_t *)(__obj) )
#define FIFO_PEEK_HEAD(__obj, __buf)        fifo_peek_head( (fifo_t *)(__obj), __buf )

/*============================ TYPES =========================================*/
typedef struct {
    unsigned char *Buffer;
    size_t      Size;           //!< buffer size: the number of item it can store. 
                                //   It must be the value of 2 to he power n.
    size_t      Out;            //!< point to filled space.
    size_t      In;             //!< point to empty space.
} fifo_t;

//! FIFO with drip AND peek operation.
typedef struct {
    union {
        fifo_t;
        fifo_t FIFO;
    };
    size_t      Drip;
    size_t      Peek;
} fifo_dp_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     fifo_init       (fifo_t *obj, unsigned char *buf, size_t size);
extern size_t   fifo_in         (fifo_t *obj, unsigned char byte);
extern size_t   fifo_out        (fifo_t *obj, unsigned char *buf);
extern size_t   fifo_burst_in   (fifo_t *obj, const unsigned char *buf, size_t len);
extern size_t   fifo_burst_out  (fifo_t *obj, unsigned char *buf, size_t len);
extern size_t   fifo_length     (fifo_t *obj);
extern void     fifo_flush      (fifo_t *obj);
extern size_t   fifo_peek_head  (fifo_t *obj, unsigned char *buf);

/*  
    \note usage wanning:
    if there is any data dripped into the FIFO, no data should be inputted into the FIFO or the
    dripped data will be corrupted.
    to minimise the footprint and avoiding unnecessery performence loss, there is no chenking here
    to avoid this. the caller is committed to do this. eg:

    attempt to write FIFO here.
    if (0u == fifo_length_dripped()) {
        if (fifo_in(length)) {
            obj.Drip += length;
        }
    } else {
        error happens here.
    }
 */
extern void     fifo_start_drip     (fifo_dp_t *obj);
extern size_t   fifo_drip_byte      (fifo_dp_t *obj, unsigned char byte);
extern size_t   fifo_length_dripped (fifo_dp_t *obj);
extern void     fifo_flush_dripped  (fifo_dp_t *obj);

extern void     fifo_start_peek     (fifo_dp_t *obj);
extern size_t   fifo_peek_byte      (fifo_dp_t *obj, unsigned char *buf);
extern size_t   fifo_length_peeked  (fifo_dp_t *obj);
extern void     fifo_remove_peeked  (fifo_dp_t *obj);

#endif
/* EOF */
