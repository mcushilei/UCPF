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

//! \note do not move this pre-processor statement to other places
#define __LIST_C__

//! \brief options for circular doubly-linked list.

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\list_public.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
static void __list_init  (list_node_t *node);
static void __list_insert(list_node_t *node, list_node_t *prev, list_node_t *next);
static void __list_remove(list_node_t *prev, list_node_t *next);

/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
static void __list_init(list_node_t *node)
{
    node->Next = node;
    node->Prev = node;
}

static void __list_insert(list_node_t *node, list_node_t *prev, list_node_t *next)
{
    next->Prev = node;
    node->Next = next;
    node->Prev = prev;
    prev->Next = node;
}
 
static void __list_remove(list_node_t *prev, list_node_t *next)
{
    next->Prev = prev;
    prev->Next = next;
}

void list_init(list_node_t *head)
{
    head->Next = head;
    head->Prev = head;
}

void list_insert(list_node_t *node, list_node_t *ahead)
{
    __list_insert(node, ahead, ahead->Next);
}
 
void list_remove(list_node_t *entry)
{
    __list_remove(entry->Prev, entry->Next);
    entry->Next = entry;
    entry->Prev = entry;
}


/* EOF */
