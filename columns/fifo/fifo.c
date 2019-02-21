/*******************************************************************************
 *  Copyright(C)2015-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include ".\fifo.h"
#include "..\string\string.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool fifo_init(void *obj, void *buffer, size_t size, size_t itemSize)
{
    fifo_t *FIFO = (fifo_t *)obj;
    
    if (NULL == FIFO || NULL == buffer || (!IS_POWER_OF_2(size))) {
        return false;
    }

    FIFO->Buffer    = buffer;
    FIFO->ItemSize  = itemSize;
    FIFO->Size      = size;
    FIFO->Out       = 0u;
    FIFO->In        = 0u;

    return true;
}

bool fifo_in(void *obj, const void *buffer)
{
    fifo_t *FIFO = (fifo_t *)obj;
    size_t L1;

    if (NULL == FIFO || NULL == buffer) {
        return false;
    }

    L1 = FIFO->Size - (FIFO->In - FIFO->Out);
    if (L1 == 0u) {      //!< fifo is full.
        return false;
    }

    L1 = MIN(1u, L1);

    memory_copy((void *)((uintptr_t)FIFO->Buffer + (FIFO->In & (FIFO->Size - 1u)) * FIFO->ItemSize), buffer, FIFO->ItemSize);
    FIFO->In += L1;

    return true;
}

bool fifo_out(void *obj, void *buffer)
{
    fifo_t *FIFO = (fifo_t *)obj;
    size_t L1;

    if (NULL == FIFO) {
        return false;
    }

    L1 = FIFO->In - FIFO->Out;
    if (L1 == 0u) {      //!< fifo is empty.
        return false;
    }

    L1 = MIN(1u, L1);

    if (NULL != buffer) {
        memory_copy(buffer, (void *)((uintptr_t)FIFO->Buffer + (FIFO->Out & (FIFO->Size - 1u)) * FIFO->ItemSize), FIFO->ItemSize);
    }
    FIFO->Out += L1;

    return true;
}

bool fifo8_init(void *obj, uint8_t *buffer, size_t size)
{
    fifo_t *FIFO = (fifo_t *)obj;
    
    if (NULL == FIFO || NULL == buffer || (!IS_POWER_OF_2(size))) {
        return false;
    }

    FIFO->Buffer    = buffer;
    FIFO->ItemSize  = sizeof(uint8_t);
    FIFO->Size      = size;
    FIFO->Out       = 0u;
    FIFO->In        = 0u;

    return true;
}

bool fifo8_in(void *obj, const uint8_t *buffer)
{
    fifo_t *FIFO = (fifo_t *)obj;
    size_t L1;

    if (NULL == FIFO || NULL == buffer) {
        return false;
    }

    L1 = FIFO->Size - (FIFO->In - FIFO->Out);
    if (L1 == 0u) {      //!< fifo is full.
        return false;
    }

    L1 = MIN(1u, L1);

    ((uint8_t *)FIFO->Buffer)[FIFO->In & (FIFO->Size - 1u)] = *buffer;
    FIFO->In += L1;

    return true;
}

bool fifo8_out(void *obj, uint8_t *buffer)
{
    fifo_t *FIFO = (fifo_t *)obj;
    size_t L1;

    if (NULL == FIFO) {
        return false;
    }

    L1 = FIFO->In - FIFO->Out;
    if (L1 == 0u) {      //!< fifo is empty.
        return false;
    }

    L1 = MIN(1u, L1);

    if (NULL != buffer) {
        *buffer = ((uint8_t *)FIFO->Buffer)[FIFO->Out & (FIFO->Size - 1u)];
    }
    FIFO->Out += L1;

    return true;
}

//size_t fifo8_in_burst(void *obj, const uint8_t *buffer, size_t bufferSize)
//{
//    fifo_t *FIFO = (fifo_t *)obj;
//    size_t L1, L2;
//
//    if (NULL == FIFO || NULL == buffer) {
//        return 0;
//    }
//
//    L1 = FIFO->Size - (FIFO->In - FIFO->Out);
//    L2 = FIFO->Size - (FIFO->In & (FIFO->Size - 1u));
//    if (L1 == 0u) {     //!< fifo is full.
//        return 0;
//    }
//
//    L1 = MIN(bufferSize, L1); //!< all those without data, include back around
//    L2 = MIN(L1, L2);   //!< those can be access at once.
//
//    memory_copy(FIFO->Buffer + (FIFO->In & (FIFO->Size - 1u)), buffer, L2);
//    memory_copy(FIFO->Buffer, buffer + L2, L1 - L2);
//    FIFO->In += L1;
//
//    return L1;
//}
//
//size_t fifo8_out_burst(void *obj, uint8_t *buffer, size_t bufferSize)
//{
//    fifo_t *FIFO = (fifo_t *)obj;
//    size_t L1, L2;
//
//    if (NULL == FIFO) {
//        return 0;
//    }
//
//    L1 = FIFO->In - FIFO->Out;
//    L2 = FIFO->Size - (FIFO->Out & (FIFO->Size - 1u));
//    if (L1 == 0u) {     //!< fifo is empty.
//        return 0;
//    }
//
//    L1 = MIN(bufferSize, L1); //!< all those with data, include back around
//    L2 = MIN(L1, L2);   //!< those can be access at once.
//
//    if (NULL != buffer) {
//        memory_copy(buffer, FIFO->Buffer + (FIFO->Out & (FIFO->Size - 1u)), L2);
//        memory_copy(buffer + L2, FIFO->Buffer, L1 - L2);
//    }
//    FIFO->Out += L1;
//
//    return L1;
//}

/* EOF */
