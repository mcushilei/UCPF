/*******************************************************************************
 *  Copyright(C)2021 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct {
    size_t  Head;
    size_t  Tail;
    size_t  Drip;
    size_t  Length;
    size_t  DripLength;
} ring_buffer_info_t;

typedef struct ring_buffer_lock_t {
    void  *Lock;
    void( *Set )(struct ring_buffer_lock_t *lock);
    void( *Reset )(struct ring_buffer_lock_t *lock);
} ring_buffer_lock_t;

typedef struct {
    ring_buffer_lock_t Lock;
    char   *Buffer;
    size_t  Size;
    size_t  Tail;
    size_t  Head;
    size_t  Drip;
    size_t  DripLength;
    bool    IsFull;
} ring_buffer_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool ring_buffer_init        ( ring_buffer_t  *obj,
                                      void           *buffer,
                                      size_t          bufferSize,
                                      const ring_buffer_lock_t   *lock );
extern int  ring_buffer_write_byte  ( ring_buffer_t *obj, char byte );
extern int  ring_buffer_write       ( ring_buffer_t *obj, const void *buf, size_t len );
extern int  ring_buffer_read        ( ring_buffer_t *obj, void *buf, size_t len );
extern bool ring_buffer_get_info    ( ring_buffer_t *obj, ring_buffer_info_t *info );
extern int  ring_buffer_drip_byte   ( ring_buffer_t *obj, char byte );
extern void ring_buffer_flush_dripped( ring_buffer_t *obj );

#endif
/* EOF */
