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

#ifndef __LIST_H__
#define __LIST_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define LIST_IS_EMPTY(__LIST)               ( (__LIST)->Next == (__LIST) )

#define LIST_NODE_IS_HEAD(__NODE, __LIST)   ( (__NODE) == (__LIST) )

#define LIST_FOR_EARCH(__NODE, __LIST) \
    for (__NODE = (__LIST)->Next; __NODE != (__LIST); __NODE = __NODE->Next)

#define LIST_FOR_EACH_SAFE(__NODE, __NEXT, __LIST)          \
	for (__NODE = (__LIST)->Next, __NEXT = __NODE->Next;  \
        __NODE != (__LIST);                                   \
		__NODE = __NEXT, __NEXT = __NODE->Next)
        
/*============================ TYPES =========================================*/
typedef struct list_node_t list_node_t;
struct list_node_t {
    list_node_t       *Prev;
    list_node_t       *Next;
};

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void list_init   (list_node_t *head);
extern void list_insert (list_node_t *node, list_node_t *ahead);
extern void list_remove (list_node_t *node);

#endif
/* EOF */
