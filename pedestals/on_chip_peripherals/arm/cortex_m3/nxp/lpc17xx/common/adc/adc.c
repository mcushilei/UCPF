/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#define __DRIVER_ADC_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_adc.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
#if (!defined(ADC_CLOCK_FRQ)) || (ADC_CLOCK_FRQ > (13MHz))
#define ADC_CLOCK_FRQ       (13MHz)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)  {\
        uint32_t tStatus = peripheral_clock_get_status(PCONP_ADC);        \
        peripheral_clock_enable(PCONP_ADC);                                 \
        __VA_ARGS__;                                                        \
        peripheral_clock_resume_status(PCONP_ADC, tStatus);                        \
    }

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool adc_init(void)
{
    uint32_t wPClock;
    
    wPClock = peripheral_clock_get(PCLK_ADC);

    wPClock = (wPClock + ADC_CLOCK_FRQ - 1) / ADC_CLOCK_FRQ;//!< ADC clock divide
    if (wPClock != 0) {
        wPClock--;
    }
    
    SAFE_CLK_CODE(
        ADC_REG.INTEN   = 0;
        ADC_REG.CR      = (wPClock << 8);    //!< config ADC clock divisor.
    )
    
    return true;
}

bool adc_enable(void)
{
    peripheral_clock_enable(PCONP_ADC);
    
    ADC_REG.CR = ADC_REG.CR
                | (1u << 21)        //!< to enable power ADC.
                & (~((0x0Fu << 17) | (0x3u << 22) | (0x0Fu << 28)));    //!< clear reserved bits.

    return true;
}

bool adc_disable(void)
{
    ADC_REG.CR = ADC_REG.CR
                & (~(1u << 21))     //!< to disable power ADC.
                & (~((0x0Fu << 17) | (0x3u << 22) | (0x0Fu << 28)));    //!< clear reserved bits.

    peripheral_clock_disable(PCONP_ADC);
    
    return true;
}

bool adc_trige_single_convert(uint32_t wChannel)
{
    
    return true;
}

bool adc_stop_convert(void)
{
    
    return true;
}

bool adc_check_channel_statue(uint32_t wChannel)
{
    return false;
}

uint32_t adc_get_channel_resault(uint32_t wChannel)
{
    return ADC_REG.DR[wChannel];
}


/* EOF */
