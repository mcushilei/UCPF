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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\queue.h"
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void lock_set( queue_lock_t *lock );
static void lock_reset( queue_lock_t *lock );

/*============================ LOCAL VARIABLES ===============================*/

static const queue_lock_t defaultLock = {
    NULL,
    lock_set,
    lock_reset,
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void lock_set( queue_lock_t *lock )
{
}

static void lock_reset( queue_lock_t *lock )
{
}

bool queue_init( queue_t *obj,
                 void *buffer,
                 size_t bufferLength,
                 size_t itemSize,
                 const queue_lock_t *lock )
{
    if( NULL == obj || NULL == buffer || 0u == bufferLength ) {
        return false;
    }

    obj->Buffer = buffer;
    obj->ItemSize = itemSize;
    obj->Size = bufferLength;
    obj->Head = 0;
    obj->Tail = 0;
    obj->Peek = 0;
    obj->Length = 0;
    obj->PeekLength = 0;
    if( NULL == lock ) {
        obj->Lock = defaultLock;
    } else {
        obj->Lock = *lock;
    }

    return true;
}

bool queue_enqueue( queue_t *obj, void *data )
{
    bool rc = false;

    if( NULL == obj ) {
        return false;
    }

    if( NULL == data ) {
        return false;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        if( (obj->Head == obj->Tail) && (0 != obj->Length) ) {
            break;
        }

        memcpy( (void *)((uintptr_t)obj->Buffer + obj->Tail * obj->ItemSize),
                data,
                obj->ItemSize );
        obj->Tail++;
        if( obj->Tail >= obj->Size ) {
            obj->Tail = 0;
        }
        obj->Length++;
        obj->PeekLength++;
        rc = true;
    } while( false );
    obj->Lock.Reset( &obj->Lock );

    return rc;
}

bool queue_dequeue( queue_t *obj, void *data )
{
    bool rc = false;

    if( NULL == obj ) {
        return false;
    }

    if( NULL == data ) {
        return false;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        if( (obj->Head == obj->Tail) && (0u == obj->Length) ) {
            break;
        }

        if( NULL != data ) {
            memcpy( data,
                    (void *)((uintptr_t)obj->Buffer + obj->Head * obj->ItemSize),
                    obj->ItemSize );
        }
        obj->Head++;
        if( obj->Head >= obj->Size ) {
            obj->Head = 0;
        }
        obj->Length--;
        obj->Peek = obj->Head;
        obj->PeekLength = obj->Length;
        rc = true;
    } while( false );
    obj->Lock.Reset( &obj->Lock );

    return rc;
}

bool queue_peek( queue_t *obj, void *data )
{
    bool rc = false;

    if( NULL == obj ) {
        return false;
    }

    if( NULL == data ) {
        return false;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        if( (obj->Peek == obj->Tail) && (0u == obj->PeekLength) ) {
            break;
        }
        if( NULL != data ) {
            memcpy( data,
                    (void *)((uintptr_t)obj->Buffer + obj->Peek * obj->ItemSize),
                    obj->ItemSize );
        }
        obj->Peek++;
        obj->PeekLength--;
        if( obj->Peek >= obj->Size ) {
            obj->Peek = 0;
        }
        rc = true;
    } while( false );
    obj->Lock.Reset( &obj->Lock );

    return rc;
}

void queue_flush_peeked( queue_t *obj )
{
    if( NULL == obj ) {
        return;
    }

    obj->Lock.Set( &obj->Lock );
    obj->Head = obj->Peek;
    obj->Length = obj->PeekLength;
    obj->Lock.Reset( &obj->Lock );
}

void queue_reset_peek( queue_t *obj )
{
    if( NULL == obj ) {
        return;
    }

    obj->Lock.Set( &obj->Lock );
    obj->Peek = obj->Head;
    obj->PeekLength = obj->Length;
    obj->Lock.Reset( &obj->Lock );
}

size_t queue_get_length( queue_t *obj )
{
    size_t len;

    if( NULL == obj ) {
        return 0;
    }

    obj->Lock.Set( &obj->Lock );
    len = obj->Length;
    obj->Lock.Reset( &obj->Lock );

    return len;
}

