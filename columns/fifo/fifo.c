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
#include ".\fifo.h"
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool fifo_init(fifo_t *obj, unsigned char *buffer, size_t size)
{
    if (NULL == obj || NULL == buffer || (!IS_POWER_OF_2(size))) {
        return false;
    }

    obj->Buffer    = buffer;
    obj->Size      = size;
    obj->Out       = 0;
    obj->In        = 0;
    obj->Drip      = 0;

    return true;
}

/*
 *  \return 0: fail
 *  \rteurn 1: ok
 */
size_t fifo_drip_byte(fifo_t *obj, unsigned char byte)
{
    size_t L1, L2;

    L1 = obj->Size - (obj->Drip - obj->Out);       //! calculate the free space
    L2 = obj->Size - (obj->Drip & (obj->Size - 1));
    if (L1 < 1) {     //!< no enough space.
        return 0;
    }

    L1 = 1;
    L2 = MIN(1,  L2);
    
    if (L2) {
        obj->Buffer[obj->Drip & (obj->Size - 1)] = byte;
    } else {
        obj->Buffer[0] = byte;
    }
    obj->Drip += L1;

    return L1;
}

/*
 *  \return 0: fail
 *  \rteurn len: ok
 */
size_t fifo_burst_drip(fifo_t *obj, const unsigned char *buf, size_t len)
{
    size_t L1, L2;

    L1 = obj->Size - (obj->Drip - obj->Out);       //! calculate the free space
    L2 = obj->Size - (obj->Drip & (obj->Size - 1));
    if (L1 < len) {     //!< no enough space.
        return 0;
    }

    L1 = len;
    L2 = MIN(L1,  L2);

    memcpy((char *)obj->Buffer + (obj->Drip & (obj->Size - 1)), buf, L2);
    memcpy(obj->Buffer, buf + L2, L1 - L2);
    obj->Drip += L1;

    return L1;
}

size_t fifo_length_dripped(fifo_t *obj)
{
    return obj->Drip - obj->In;
}

void fifo_flush_dripped(fifo_t *obj)
{
    obj->In = obj->Drip;
}


/* \note 
    
    scenario 1(empty):
        out and in
        |
        v
    --------------------------
    ^                        ^
    |                        |
    0                      size-1
    
    scenario 2(only 1):
        out
        |+------in
        vv
    ----X---------------------
    
    scenario 3(most case):
       out            in
        |              |
        v              v
    ----XXXXXXXXXXXXXXX-------
    
    scenario 4(reverse):
        in             out
        |              |
        v              v
    XXXX---------------XXXXXXX
    
 */

size_t fifo_in(fifo_t *obj, unsigned char byte)
{
    if ((obj->Size - (obj->In - obj->Out)) == 0u) {     //!< no enough space.
        return 0;
    }

    obj->Buffer[obj->In & (obj->Size - 1)] = byte;
    obj->In++;

    return 1;
}

size_t fifo_out(fifo_t *obj, unsigned char *buf)
{
    if ((obj->In - obj->Out) == 0u) {     //!< no data.
        return 0;
    }

    if (NULL != buf) {
        *buf = obj->Buffer[obj->Out & (obj->Size - 1)];
    }
    obj->Out++;

    return 1;
}

size_t fifo_burst_in(fifo_t *obj, const unsigned char *buf, size_t len)
{
    size_t L1, L2;

    //! input is disabled if there is any data dripped in the FIFO.
    if (obj->In != obj->Drip) {
        return 0;
    }

    L1 = obj->Size - (obj->In - obj->Out);       //! calculate the free space
    L2 = obj->Size - (obj->In & (obj->Size - 1));
    if (L1 < len) {     //!< no enough space.
        return 0;
    }

    L1 = len;
    L2 = MIN(L1,  L2);

    memcpy((char *)obj->Buffer + (obj->In & (obj->Size - 1)), buf, L2);
    memcpy(obj->Buffer, buf + L2, L1 - L2);
    obj->In += L1;
    obj->Drip = obj->In;

    return L1;
}

size_t fifo_burst_out(fifo_t *obj, unsigned char *buf, size_t len)
{
    size_t L1, L2;

    L1 = obj->In - obj->Out;      //! calculate the length of data in the fifo.
    L2 = obj->Size - (obj->Out & (obj->Size - 1));
    if (0 == L1) {     //!< no data.
        return 0;
    }

    L1 = MIN(len, L1);
    L2 = MIN(L1,  L2);

    if (NULL != buf) {
        memcpy(buf, (char *)obj->Buffer + (obj->Out & (obj->Size - 1)), L2);
        memcpy(buf + L2, obj->Buffer, L1 - L2);
    }
    obj->Out += L1;

    return L1;
}

size_t fifo_length(fifo_t *obj)
{
    return obj->In - obj->Out;      //! calculate the length of data in the fifo.
}

void fifo_flush(fifo_t *obj)
{
    obj->Out = obj->In;
}

/* EOF */
