/*******************************************************************************
 *  Copyright(C)2017-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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


//! \note do not move this pre-processor statement to other places
#define __OS_LIST_C__

/*============================ INCLUDES ======================================*/
#include ".\os_private.h"
#include ".\os_port.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
void os_list_init_head(OS_LIST_NODE *head)
{
    head->Next = head;
    head->Prev = head;
}

static void __os_list_add(OS_LIST_NODE *node, OS_LIST_NODE *prev, OS_LIST_NODE *next)
{
    next->Prev = node;
    node->Next = next;
    node->Prev = prev;
    prev->Next = node;
}

void os_list_add(OS_LIST_NODE *node, OS_LIST_NODE *head)
{
    __os_list_add(node, head, head->Next);
}
 
 
void __os_list_del(OS_LIST_NODE *prev, OS_LIST_NODE *next)
{
    next->Prev = prev;
    prev->Next = next;
}
 
void os_list_del(OS_LIST_NODE *entry)
{
    __os_list_del(entry->Prev, entry->Next);
    entry->Next = entry;
    entry->Prev = entry;
}


/* EOF */
