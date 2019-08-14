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

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool queue_init(void *obj, void *buffer, size_t size, size_t itemSize)
{
    queue_t *queue = (queue_t *)obj;
    
    if (NULL == queue || NULL == buffer || 0u == size) {
        return false;
    }

    queue->Buffer       = buffer;
    queue->ItemSize     = itemSize;
    queue->Size         = size;
    queue->Head         = 0;
    queue->Tail         = 0;
    queue->Peek         = 0;
    queue->Length       = 0;
    queue->PeekLength   = 0;

    return true;
}

bool queue_deinit(void *obj)
{
    queue_t *queue = (queue_t *)obj;

    if (NULL == queue) {
        return false;
    }

    queue->Buffer       = NULL;
    queue->ItemSize     = 0;
    queue->Size         = 0;
    queue->Head         = 0;
    queue->Tail         = 0;
    queue->Peek         = 0;
    queue->Length       = 0;
    queue->PeekLength   = 0;

    return true;
}

bool queue_enqueue(void *obj, void *data)
{
    queue_t *queue = (queue_t *)obj;
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
            &&  (0 != queue->Length)) {
                break;
            }

            switch (queue->ItemSize) {
                case sizeof(uint8_t):
                    ((uint8_t *)(queue->Buffer))[
                                 queue->Tail] = *(uint8_t *)data;
                    break;
                case sizeof(uint16_t):
                    ((uint16_t *)(queue->Buffer))[
                                  queue->Tail] = *(uint16_t *)data;
                    break;
                case sizeof(uint32_t):
                    ((uint32_t *)(queue->Buffer))[
                                  queue->Tail] = *(uint32_t *)data;
                    break;
                default:
                    memcpy((void *)((uintptr_t)queue->Buffer + queue->Tail * queue->ItemSize),
                             data,
                             queue->ItemSize);
                    break;
            }
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

bool queue_dequeue(void *obj, void *data)
{
    queue_t *queue = (queue_t *)obj;
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
                switch (queue->ItemSize) {
                    case sizeof(uint8_t):
                        *(uint8_t *)data = ((uint8_t *)(queue->Buffer))[
                                                         queue->Head];
                        break;
                    case sizeof(uint16_t):
                        *(uint16_t *)data = ((uint16_t *)(queue->Buffer))[
                                                           queue->Head];
                        break;
                    case sizeof(uint32_t):
                        *(uint32_t *)data = ((uint32_t *)(queue->Buffer))[
                                                           queue->Head];
                        break;
                    default:
                        memcpy(data,
                                 (void *)((uintptr_t)queue->Buffer + queue->Head * queue->ItemSize),
                                 queue->ItemSize);
                        break;
                }
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

bool queue_peek(void *obj, void *data)
{
    queue_t *queue = (queue_t *)obj;
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
                switch (queue->ItemSize) {
                    case sizeof(uint8_t):
                        *(uint8_t *)data = ((uint8_t *)(queue->Buffer))[
                                                         queue->Peek];
                        break;
                    case sizeof(uint16_t):
                        *(uint16_t *)data = ((uint16_t *)(queue->Buffer))[
                                                           queue->Peek];
                        break;
                    case sizeof(uint32_t):
                        *(uint32_t *)data = ((uint32_t *)(queue->Buffer))[
                                                           queue->Peek];
                        break;
                    default:
                        memcpy(data,
                                 (void *)((uintptr_t)queue->Buffer + queue->Peek * queue->ItemSize),
                                 queue->ItemSize);
                        break;
                }
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

void queue_get_all_peeked(void *obj)
{
    queue_t *queue = (queue_t *)obj;

    if (NULL == queue) {
        return ;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
        queue->Head = queue->Peek;
        queue->Length = queue->PeekLength;
    QUEUE_CRITICAL_SECTION_END();
}

void queue_reset_peek(void *obj)
{
    queue_t *queue = (queue_t *)obj;

    if (NULL == queue) {
        return;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
        queue->Peek = queue->Head;
        queue->PeekLength = queue->Length;
    QUEUE_CRITICAL_SECTION_END();
}

__queue_uint_t queue_get_length(void *obj)
{
    queue_t *queue = (queue_t *)obj;
    __queue_uint_t Length;

    if (NULL == queue) {
        return 0;
    }

    QUEUE_CRITICAL_SECTION_BEGIN();
        Length = queue->Length;
    QUEUE_CRITICAL_SECTION_END();

    return Length;
}

