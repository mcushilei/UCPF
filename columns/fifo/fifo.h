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

#ifndef __FIFO_H__
#define __FIFO_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    char       *Buffer;
    size_t      Size;           //!< buffer size: the number of item it can store. 
                                //   It must be the value of 2 to he power n.
    size_t      Out;            //!< point to filled space.
    size_t      In;             //!< point to empty space.
    size_t      Drip;
} fifo_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     fifo_init       (fifo_t *obj, char *buffer, size_t size);
extern size_t   fifo_burst_in   (fifo_t *obj, const char *buf, size_t len);
extern size_t   fifo_burst_out  (fifo_t *obj, char *buf, size_t len);
extern size_t   fifo_length     (fifo_t *obj);
extern void     fifo_flush      (fifo_t *obj);
extern size_t   fifo_drip_byte  (fifo_t *obj, char byte);
extern size_t   fifo_burst_drip (fifo_t *obj, const char *buf, size_t len);
extern size_t   fifo_length_dripped(fifo_t *obj);
extern void     fifo_flush_dripped(fifo_t *obj);

#endif
/* EOF */
