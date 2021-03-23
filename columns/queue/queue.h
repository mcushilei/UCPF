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

#ifndef __QUEUE_H__
#define __QUEUE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define QUEUE_UINT8_INIT(__PQUEUE, __PBUF, __BUF_SIZE, __LOCK)      \
    queue_init(__PQUEUE, __PBUF, __BUF_SIZE, sizeof(uint8_t), __LOCK)

#define QUEUE_UINT16_INIT(__PQUEUE, __PBUF, __BUF_SIZE, __LOCK)     \
    queue_init(__PQUEUE, __PBUF, __BUF_SIZE, sizeof(uint16_t), __LOCK)

#define QUEUE_UINT32_INIT(__PQUEUE, __PBUF, __BUF_SIZE, __LOCK)     \
    queue_init(__PQUEUE, __PBUF, __BUF_SIZE, sizeof(uint32_t), __LOCK)

#define QUEUE_ENQUEUE(__PQUEUE, __OBJ)      queue_enqueue(__PQUEUE, &(__OBJ))
#define QUEUE_DEQUEUE(__PQUEUE, __POBJ)     queue_dequeue(__PQUEUE, __POBJ)
#define QUEUE_PEEK(__PQUEUE, __POBJ)        queue_peek(__PQUEUE, __POBJ)
#define QUEUE_GET_ALL_PEEKED(__PQUEUE)      queue_flush_peeked(__PQUEUE)
#define QUEUE_RESET_PEEK(__PQUEUE)          queue_reset_peek(__PQUEUE)
#define QUEUE_GET_OBJECT_COUNT(__PQUEUE)    queue_get_length(__PQUEUE)

/*============================ TYPES =========================================*/
        
typedef struct queue_lock_t {
    void  *Lock;
    void( *Set )(struct queue_lock_t *lock);
    void( *Reset )(struct queue_lock_t *lock);
} queue_lock_t;

typedef struct {
    queue_lock_t Lock;
    void   *Buffer;
    size_t  ItemSize;
    size_t  Size;
    size_t  Head;
    size_t  Tail;
    size_t  Length;
    size_t  Peek;
    size_t  PeekLength;
} queue_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     queue_init          ( queue_t *obj,
                                      void *buffer,
                                      size_t bufferLength,
                                      size_t itemSize,
                                      const queue_lock_t *lock );
extern bool     queue_enqueue       ( queue_t *obj, void *data );
extern bool     queue_dequeue       ( queue_t *obj, void *data );
extern bool     queue_peek          ( queue_t *obj, void *data );
extern void     queue_flush_peeked  ( queue_t *obj );
extern void     queue_reset_peek    ( queue_t *obj );
extern size_t   queue_get_length    ( queue_t *obj );

#endif
/* EOF */
