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
#include "heap_memory/heap_memory.h"

extern int app_init(void);

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
THIS_FILE_NAME("framework");

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


#define configTOTAL_HEAP_SIZE           (1024 * 8 - 3)
char heapMemory1[ configTOTAL_HEAP_SIZE ];
char heapMemory2[ 21 ];

com_t myComPort;


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

    if (!rtc_api_init()) {
        DBG_LOG("%s", "rtc_api_init() fail!");
        return false;
    }

    if (!socket_api_init()) {
        DBG_LOG("%s", "socket_api_init() fail!");
    }

    enum_all_comm();
    com_cfg_t myComPortCfg = { .wBaudrate = 9600,.hwMode = UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH };
    //if (!com_open(&myComPort, "\\\\.\\COM11", &myComPortCfg, NULL)) {
    //    printf("\r\n cannot open com!");
    //    return false;
    //}


    app_init();

    while (1) {
        OS_TASK_SLEEP(1000);
    }

    return 0;
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


void mbedtls_param_failed ( const char *failure_condition,
							const char *file,
							int line )
{
	printf(" \r\n %s:%04d: %s", file, line, failure_condition );
}


/* EOF */
