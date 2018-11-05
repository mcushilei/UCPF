/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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



#ifndef __QUEUE_PUBLIC_H__
#define __QUEUE_PUBLIC_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(queue_t)
    void               *Buffer;
    __queue_uint_t      Size;
    __queue_uint_t      ItemSize;
    __queue_uint_t      Head;
    __queue_uint_t      Tail;
    __queue_uint_t      Length;
    __queue_uint_t      Peek;
    __queue_uint_t      PeekLength;
END_DEF_STRUCTURE(queue_t)

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool queue_init          (void *obj, void *buffer, size_t size, size_t itemSize);
extern bool queue_deinit        (void *obj);
extern bool queue_enqueue       (void *obj, void *data);
extern bool queue_dequeue       (void *obj, void *data);
extern bool queue_peek          (void *obj, void *data);
extern void queue_get_all_peeked(void *obj);
extern void queue_reset_peek    (void *obj);
extern __queue_uint_t queue_get_length(void *obj);

#endif  //! #ifndef __QUEUE_PUBLIC_H__
/* EOF */
