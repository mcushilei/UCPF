/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void watch_dog_enable(void)
{
    EALLOW();
    WDT_REG.WDCR |= WDCR_WDDIS_MSK | WDCR_WDCHK_VALUE;
    EDIS();
}

void watch_dog_disable(void)
{
    EALLOW();
    WDT_REG.WDCR &= (~WDCR_WDDIS_MSK) | WDCR_WDCHK_VALUE;
    EDIS();
}

void watch_dog_feed(void)
{
    EALLOW();
    WDT_REG.WDKEY = 0x0055;
    WDT_REG.WDKEY = 0x00AA;
    EDIS();
}

/* EOF */
