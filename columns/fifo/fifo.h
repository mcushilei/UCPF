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

#ifndef __FIFO_H__
#define __FIFO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    void        *Buffer;
    size_t       Size;           //!< buffer size: the number of item it can store. 
                                 //   It must be the value of 2 to he power n.
    size_t       Out;            //!< point to space filled.
    size_t       In;             //!< point to space empty.
} fifo_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     fifo_init       (fifo_t *obj, void *buffer, size_t size);
extern void     fifo_flush      (fifo_t *obj);
extern size_t   fifo_length     (fifo_t *obj);
extern size_t   fifo_burst_in   (fifo_t *obj, const char *buf, size_t len);
extern size_t   fifo_burst_out  (fifo_t *obj, char *buf, size_t len);

#endif
/* EOF */
