/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./framework.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


#define configTOTAL_HEAP_SIZE           (1024 * 8 - 3)
char heapMemory1[ configTOTAL_HEAP_SIZE ];
char heapMemory2[ 21 ];


bool framework_init(void)
{
    osInit();

    heap_memory_cfg_t heapCfg[] = {
        {&heapMemory1, configTOTAL_HEAP_SIZE},
        {&heapMemory2, 21},
    };
    heap_init(heapCfg, UBOUND(heapCfg));
    uint32_t maxMemSize = heap_remaining();
    DBG_LOG("heap_remaining = %u", maxMemSize);
    DBG_LOG("heap start at: %p", heapMemory1);

    if (!socket_api_init()) {
        DBG_LOG("socket_api_init() fail!");
    }

    return true;
}


void debug_output_char(char ch)
{
    putchar(ch);
}

int user_printf_output_char(char ch)
{
    putchar(ch);
    return ch;
}


/* EOF */
