/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "../device.h"
#include "./reg_adc.h"
#include "./adc.h"
#include "../scon/pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*
 *  \brief  init peripheral dependence
 */
WEAK void __driver_adc_initpd(void)
{
    //! TODO: enable its clock, pins.
}

/*
 *  \brief  deinit peripheral dependence
 */
WEAK void __driver_adc_deinitpd(void)
{
    //! TODO: disable its clock, pins.
}

bool adc_init(driver_adc_cfg_t *cfg)
{
    __driver_adc_initpd();
    
    uint32_t clock = cfg->ClockFrq;
    if (clock > (13MHz) || 0u == clock) {
        clock = 13MHz;
    }
    
    uint32_t wPClock = peripheral_clock_get(PCLK_ADC);
    uint32_t div = (wPClock + clock - 1u) / clock;//!< ADC clock divide
    if (div > 255u) {
        div = 256u;
    }
    if (div != 0u) {
        div--;
    }
    
    uint32_t reg = ((div & 0xFFu) << 8) | cfg->Channels | cfg->TriggerSource | cfg->TriggerEdge;
    uint32_t intReg = cfg->EnableInterrupt;
    if (ADC_TRIGGER_BURST == cfg->TriggerSource) {
        reg |= 1u << 16;
        intReg &= ~(1u << 8);
    }
    reg |= 1u << 21;
    ADC_REG.CR = reg;
    ADC_REG.INTEN = intReg;
    
    return true;
}


bool adc_deinit(void)
{
    ADC_REG.CR = 0u;

    __driver_adc_deinitpd();
    
    return true;
}

uint16_t adc_get_channel_resault(uint32_t wChannel)
{
    return ADC_REG.DR[wChannel] & (~0xFFFF000Fu);
}


/* EOF */
