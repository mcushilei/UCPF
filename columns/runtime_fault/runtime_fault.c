/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include <stdio.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

extern void framework_reboot(void);

#if defined(__DEBUG__)
void runtime_fault(int ec, const char *file, int line, const char *info)
#else
void runtime_fault(int ec, const char *info)
#endif
{
    ec = ec;
    info = info;
#if defined(__DEBUG__)
    file = file;
    line = line;
    volatile int trap = 1;
    
    if (NULL != info) {
        printf("\r\n[Error: %u][%s:%u]%s", ec, file, line, info);
    }
    while (trap);
#else
    if (NULL != info) {
        printf("\r\n[Error: %u]%s", ec, info);
    }
    framework_reboot();
#endif
}

/* EOF */
