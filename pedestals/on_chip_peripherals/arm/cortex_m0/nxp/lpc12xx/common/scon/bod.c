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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_BOD_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_scon.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool bod_init(uint32_t wLevel)
{
    SYSCON_REG.BODCTRL.BODRSTLEV = wLevel;

    return true;
}

bool bod_reset_level_set(uint32_t wLevel)
{
    SYSCON_REG.BODCTRL.BODRSTLEV = wLevel;

    return true;
}

uint32_t bod_reset_level_get(void)
{
    return SYSCON_REG.BODCTRL.BODRSTLEV;
}

bool bod_int_level_set(uint32_t wLevel)
{
    SYSCON_REG.BODCTRL.BODINTVAL = wLevel;

    return true;
}

uint32_t bod_int_level_get(void)
{
    return SYSCON_REG.BODCTRL.BODINTVAL;
}

bool bod_enable(void)
{
    SYSCON_REG.PDRUNCFG.BOD = 1;

    return true;
}

bool bod_disable(void)
{
    SYSCON_REG.PDRUNCFG.BOD = 0;

    return false;
}

/* EOF */
