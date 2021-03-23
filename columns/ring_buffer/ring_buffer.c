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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\ring_buffer.h"
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define RING_BUFFER_LENGTH(__OBJ, __LEN) \
do {\
    if ((__OBJ)->IsFull) {\
        (__LEN) = (__OBJ)->Size;\
        break;\
    }\
    if ((__OBJ)->Tail >= (__OBJ)->Head) {\
        (__LEN) = (__OBJ)->Tail - (__OBJ)->Head;\
    } else {\
        (__LEN) = (__OBJ)->Size - ((__OBJ)->Head - (__OBJ)->Tail);\
    }\
} while (0)

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
static void lock_set( ring_buffer_lock_t *lock );
static void lock_reset( ring_buffer_lock_t *lock );

/*============================ LOCAL VARIABLES ===============================*/

static const ring_buffer_lock_t defaultLock = {
    NULL,
    lock_set,
    lock_reset,
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void lock_set( ring_buffer_lock_t *lock )
{
}

static void lock_reset( ring_buffer_lock_t *lock )
{
}

bool ring_buffer_init(
    ring_buffer_t  *obj,
    void           *buffer,
    size_t          bufferSize,
    const ring_buffer_lock_t   *lock )
{
    if( NULL == obj || NULL == buffer || 0u == bufferSize ) {
        return false;
    }

    obj->Buffer = (char *)buffer;
    obj->Size   = bufferSize;
    obj->Tail   = 0;
    obj->Head   = 0;
    obj->Drip   = 0;
    obj->DripLength = 0;
    obj->IsFull = false;
    if( NULL == lock ) {
        obj->Lock = defaultLock;
    } else {
        obj->Lock = *lock;
    }

    return true;
}

int ring_buffer_drip_byte( ring_buffer_t *obj, char byte )
{
    size_t len = 0;
    size_t tmp = 0;

    if( NULL == obj ) {
        return 0;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        RING_BUFFER_LENGTH( obj, tmp );
        tmp += obj->DripLength;
        if( tmp >= obj->Size ) {
            len = 0;
            break;
        }

        len = 1;
        obj->DripLength++;
        obj->Buffer[obj->Drip++] = byte;
        if (obj->Drip >= obj->Size) {
            obj->Drip = 0;
        }
    } while( 0 );
    obj->Lock.Reset( &obj->Lock );

    return len;
}

void ring_buffer_flush_dripped( ring_buffer_t *obj )
{
    obj->Lock.Set( &obj->Lock );
    do {
        obj->Tail = obj->Drip;
        if( obj->Tail == obj->Head ) {
            obj->IsFull = true;
        }
        obj->DripLength = 0;
    } while( 0 );
    obj->Lock.Reset( &obj->Lock );
}

int ring_buffer_write_byte( ring_buffer_t *obj, char byte )
{
    size_t len = 0;

    if( NULL == obj ) {
        return 0;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        if( obj->IsFull || obj->DripLength ) {
            len = 0;
            break;
        }

        len = 1;
        obj->Buffer[obj->Tail++] = byte;
        if (obj->Tail >= obj->Size) {
            obj->Tail = 0;
        }
        if( obj->Tail == obj->Head ) {
            obj->IsFull = true;
        }
        
        obj->Drip = obj->Tail;
    } while( 0 );
    obj->Lock.Reset( &obj->Lock );

    return len;
}

int ring_buffer_write( ring_buffer_t *obj, const void *buf, size_t len )
{
    size_t tmp = 0;
    size_t ori;
    size_t end;

    if( NULL == obj ) {
        return 0;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        RING_BUFFER_LENGTH( obj, tmp );
        tmp = obj->Size - tmp;
        if( len > tmp || obj->DripLength ) {
            len = 0;
            break;
        }
        if( len == tmp ) {
            obj->IsFull = true;
        }

        ori = obj->Tail;
        end = obj->Head;
        if( end > ori ) {
            memcpy( &obj->Buffer[ori], buf, len );
            obj->Tail += len;
        } else {
            tmp = obj->Size - ori;
            if( tmp > len ) {
                memcpy( &obj->Buffer[ori], buf, len );
                obj->Tail += len;
            } else {
                memcpy( &obj->Buffer[ori], buf, tmp );
                memcpy( &obj->Buffer[0], (const char *)buf + tmp, len - tmp );
                obj->Tail = len - tmp;
            }
        }
        
        obj->Drip = obj->Tail;
    } while( 0 );
    obj->Lock.Reset( &obj->Lock );

    return len;
}

int ring_buffer_read( ring_buffer_t *obj, void *buf, size_t len )
{
    size_t tmp = 0;
    size_t ori;
    size_t end;

    if( NULL == obj || NULL == buf ) {
        return 0;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        RING_BUFFER_LENGTH( obj, tmp );
        if( len > tmp ) {
            len = 0;
            break;
        }

        ori = obj->Head;
        end = obj->Tail;
        if( end > ori ) {
            memcpy( buf, &obj->Buffer[ori], len );
            obj->Head += len;
        } else {
            tmp = obj->Size - ori;
            if( tmp > len ) {
                memcpy( buf, &obj->Buffer[ori], len );
                obj->Head += len;
            } else {
                memcpy( buf, &obj->Buffer[ori], tmp );
                memcpy( (char *)buf + tmp, &obj->Buffer[0], len - tmp );
                obj->Head = len - tmp;
            }
        }

        obj->IsFull = false;

    } while( false );
    obj->Lock.Reset( &obj->Lock );

    return len;
}

bool ring_buffer_get_info( ring_buffer_t *obj, ring_buffer_info_t *info )
{
    if( NULL == obj ) {
        return false;
    }

    obj->Lock.Set( &obj->Lock );
    do {
        RING_BUFFER_LENGTH( obj, info->Length );
        info->Head = obj->Head;
        info->Tail = obj->Tail;
        info->Drip = obj->Drip;
        info->DripLength = obj->DripLength;
    } while( false );
    obj->Lock.Reset( &obj->Lock );

    return true;
}

