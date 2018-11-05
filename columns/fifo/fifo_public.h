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



#ifndef __FIFO_PUBLIC_H__
#define __FIFO_PUBLIC_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    void        *Buffer;
    size_t       Size;           //!< buffer size.
    size_t       ItemSize;       //!< item size.
    size_t       Out;            //!< point to space filled.
    size_t       In;             //!< point to space empty.
} fifo_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool fifo_init       (void *obj, void *buffer, size_t size, size_t itemSize);
extern bool fifo_in         (void *obj, const void *buffer);
extern bool fifo_out        (void *obj, void *buffer);

extern bool fifo8_init      (void *obj, uint8_t *buffer, size_t size);
extern bool fifo8_in        (void *obj, const uint8_t *buffer);
extern bool fifo8_out       (void *obj, uint8_t *buffer);
//extern bool fifo8_in_burst  (void *obj, const uint8_t *buffer, size_t Size);
//extern bool fifo8_out_burst (void *obj, uint8_t *buffer, size_t Size);

#endif  //! #ifndef __FIFO_PUBLIC_H__
/* EOF */
