/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __HEAP_FIRST_FIT_H__
#define __HEAP_FIRST_FIT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    void       *Address;
    size_t      Size;
} heap_memory_cfg_t;


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void  heap_init(const heap_memory_cfg_t cfg[], size_t length);
extern void  heap_deinit(void);
extern void *heap_malloc(size_t xWantedSize);
extern void *heap_realloc(void *memory, size_t newSize);
extern void  heap_free(void *memory);


extern size_t heap_remaining(void);
extern size_t heap_minimum_remaining(void);


#endif  //!< #ifndef __HEAP_FIRST_FIT_H__
/* EOF */
