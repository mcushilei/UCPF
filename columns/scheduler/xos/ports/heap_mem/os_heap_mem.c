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




/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "../../../../heap_memory/heap_first_fit/heap_first_fit.h"
#include "../../source/os_port.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static ALIGN(8) char heapMemory0[1024 * 60];
//_Pragma("location=\"ahb_ram\"") static ALIGN(8) char heapMemory1[1024 * 30];

static const heap_memory_cfg_t heapCfg[] = {
    {&heapMemory0, sizeof(heapMemory0)},
//    {&heapMemory1, sizeof(heapMemory1)},
};

void OSHeapInit(void)
{
    heap_init(heapCfg, UBOUND(heapCfg));
}

void *OSHeapAlloc(size_t size)
{
    return heap_malloc(size);
}

void OSHeapFree(void *mem)
{
    heap_free(mem);
}

/* EOF */
