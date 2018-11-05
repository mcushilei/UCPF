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
#include ".\reg_pie.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void(vector_t)(void);

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
static vector_t ** const s_pptPieVectorTable = (vector_t **)PIE_VECT_BASE_ADDRESS;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool pie_init(vector_t **pptInitValue)
{
    uint32_t wCnt;

    if (NULL == pptInitValue) {
        return false;
    }

    EALLOW();
    for (wCnt = 0; wCnt < 128; ++wCnt) {
        s_pptPieVectorTable[wCnt] = pptInitValue[wCnt];
    }
    EDIS();

    return true;
}

bool pie_enable(void)
{
    PIE_REG.PIECTRL.ENPIE = 1;

    return true;
}

bool pie_disable(void)
{
    PIE_REG.PIECTRL.ENPIE = 0;

    return true;
}

bool pie_interrupt_enable(uint16_t hwIndex)
{
    uint16_t hwGroup = 0;

    if (hwIndex >= PIE_VECTOR_TABLE_SIZE) {
        return false;
    }

    if (hwIndex < 32) {
        cpu_interrupt_enable(hwIndex);
        return true;
    }

    hwIndex -= 32;
    hwGroup  = (hwIndex & 0x7) >> 3;
    if (hwGroup >= 12) {
        return false;
    }
    hwIndex &= 0x7;
    PIE_REG.GROUP[hwGroup].PIEIER.Value |= 1u << hwIndex;

    return true;
}

bool pie_interrupt_disable(uint16_t hwIndex)
{
    uint16_t hwGroup = 0;

    if (hwIndex >= PIE_VECTOR_TABLE_SIZE) {
        return false;
    }

    if (hwIndex < 32) {
        cpu_interrupt_disable(hwIndex);
        return true;
    }

    hwIndex -= 32;
    hwGroup  = (hwIndex & 0x7) >> 3;
    if (hwGroup >= 12) {
        return false;
    }
    hwIndex &= 0x7;
    PIE_REG.GROUP[hwGroup].PIEIER.Value &= ~(1u << hwIndex);

    return false;
}

/* EOF */
