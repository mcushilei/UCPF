/*******************************************************************************
 *  Copyright(C)2015-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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

bool fifo_init(fifo_t *obj, void *buffer, size_t size)
{
    if (NULL == obj || NULL == buffer || (!IS_POWER_OF_2(size))) {
        return false;
    }

    obj->Buffer    = buffer;
    obj->Size      = size;
    obj->Out       = 0u;
    obj->In        = 0u;

    return true;
}

size_t fifo_burst_in(fifo_t *obj, const char *buf, size_t len)
{
    size_t L1, L2;

    if (NULL == obj || NULL == buf) {
        return 0;
    }

    L1 = obj->Size - (obj->In - obj->Out);       //! calculate the free space
    L2 = obj->Size - (obj->In & (obj->Size - 1u));
    if (L1 < len) {     //!< no enough space.
        return 0;
    }

    L1 = len;
    L2 = MIN(L1,  L2);

    memcpy((char *)obj->Buffer + (obj->In & (obj->Size - 1u)), buf, L2);
    memcpy(obj->Buffer, buf + L2, L1 - L2);
    obj->In += L1;

    return L1;
}

size_t fifo_burst_out(fifo_t *obj, char *buf, size_t len)
{
    size_t L1, L2;

    if (NULL == obj) {
        return 0;
    }

    L1 = obj->In - obj->Out;      //! calculate the length of data in the fifo.
    L2 = obj->Size - (obj->Out & (obj->Size - 1u));
    if (L1 < len) {     //!< no enough data.
        return 0;
    }

    L1 = len;
    L2 = MIN(L1,  L2);

    if (NULL != buf) {
        memcpy(buf, (char *)obj->Buffer + (obj->Out & (obj->Size - 1u)), L2);
        memcpy(buf + L2, obj->Buffer, L1 - L2);
    }
    obj->Out += L1;

    return L1;
}

void fifo_flush(fifo_t *obj)
{
    obj->Out = obj->In;
}

/* EOF */
