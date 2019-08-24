/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool queue_init(queue_t *queue, void *buffer, queue_uint_t bufferLength, size_t itemSize)
{
    if (NULL == queue || NULL == buffer || 0u == bufferLength) {
        return false;
    }

    queue->Buffer       = buffer;
    queue->ItemSize     = itemSize;
    queue->Size         = bufferLength;
    queue->Head         = 0;
    queue->Tail         = 0;
    queue->Peek         = 0;
    queue->Length       = 0;
    queue->PeekLength   = 0;

    return true;
}

bool queue_enqueue(queue_t *queue, void *data)
{
    bool bResult = false;

    if (NULL == queue) {
        return false;
    }

    if (NULL == queue->Buffer) {
        return false;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    do {
        if ((queue->Head ==  queue->Tail) &&  (0 != queue->Length)) {
            break;
        }

        memcpy( (void *)((uintptr_t)queue->Buffer + queue->Tail * queue->ItemSize),
                data,
                queue->ItemSize);
        queue->Tail++;
        if (queue->Tail >= queue->Size) {
            queue->Tail = 0;
        }
        queue->Length++;
        queue->PeekLength++;
        bResult = true;
    } while (false);
    QUEUE_CRITICAL_SECTION_END();

    return bResult;
}

bool queue_dequeue(queue_t *queue, void *data)
{
    bool bResult = false;

    if (NULL == queue) {
        return false;
    }

    if (NULL == queue->Buffer) {
        return false;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    do {
        if ((queue->Head ==  queue->Tail)
        &&  (0u == queue->Length)) {
            break;
        }

        if (NULL != data) {
            memcpy( data,
                    (void *)((uintptr_t)queue->Buffer + queue->Head * queue->ItemSize),
                    queue->ItemSize);
        }
        queue->Head++;
        if (queue->Head >= queue->Size) {
            queue->Head = 0;
        }
        queue->Length--;
        queue->Peek = queue->Head;
        queue->PeekLength = queue->Length;
        bResult = true;
    } while (false);
    QUEUE_CRITICAL_SECTION_END();

    return bResult;
}

bool queue_peek(queue_t *queue, void *data)
{
    bool bResult = false;

    if (NULL == queue) {
        return false;
    }

    if (NULL == queue->Buffer) {
        return false;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    do {
        if ((queue->Peek == queue->Tail)
        &&  (0u == queue->PeekLength)) {
            break;
        }
        if (NULL != data) {
            memcpy( data,
                    (void *)((uintptr_t)queue->Buffer + queue->Peek * queue->ItemSize),
                    queue->ItemSize);
        }
        queue->Peek++;
        queue->PeekLength--;
        if (queue->Peek >= queue->Size) {
            queue->Peek = 0;
        }
        bResult = true;
    } while (false);
    QUEUE_CRITICAL_SECTION_END();

    return bResult;
}

void queue_get_all_peeked(queue_t *queue)
{
    if (NULL == queue) {
        return ;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    queue->Head     = queue->Peek;
    queue->Length   = queue->PeekLength;
    QUEUE_CRITICAL_SECTION_END();
}

void queue_reset_peek(queue_t *queue)
{
    if (NULL == queue) {
        return;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    queue->Peek         = queue->Head;
    queue->PeekLength   = queue->Length;
    QUEUE_CRITICAL_SECTION_END();
}

queue_uint_t queue_get_length(queue_t *queue)
{
    queue_uint_t Length;

    if (NULL == queue) {
        return 0;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
    Length = queue->Length;
    QUEUE_CRITICAL_SECTION_END();

    return Length;
}

