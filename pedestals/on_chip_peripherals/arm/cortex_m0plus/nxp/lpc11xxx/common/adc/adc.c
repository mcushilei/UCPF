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
#include "..\pm\pm.h"
#include ".\adc_public.h"

/*============================ MACROS ========================================*/
#if (!defined(ADC_CLOCK_FRQ)) || (ADC_CLOCK_FRQ > (13MHz))
#define ADC_CLOCK_FRQ       (1MHz)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)  {                                   \
        uint32_t tStatus = ahb_clock_get_status(AHBCLK_ADC);    \
        ahb_clock_enable(AHBCLK_ADC);                           \
        __VA_ARGS__                                             \
        ahb_clock_resume_status(AHBCLK_ADC, tStatus);           \
    }

/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool adc_init(void)
{
    uint32_t div;
    
    div = core_clock_get() / ADC_CLOCK_FRQ;
    if (div != 0u) {
        div--;
    }
    SAFE_CLK_CODE(
        ADC_REG.CTRL =  (div << 0)    //!< ADC clock division.
                      | (1u << 10)    //!< enable low-power mode.
                      | (0u << 30);   //!< stop calibration.
    )
    
    return true;
}

bool adc_enable(void)
{
    uint32_t regSaved;
    uint32_t div;
    
    power_enable(POWER_ADC);
    ahb_clock_enable(AHBCLK_ADC);
    
//    regSaved = ADC_REG.CTRL;
//    div = core_clock_get() / (500KHz);
//    if (div != 0u) {
//        div--;
//    }
//    ADC_REG.CTRL = (div << 0)   //!< ADC clock division.
//             | (0u << 10)       //!< disable low-power mode.
//             | (1u << 30);      //!< start calibration.
//    while (ADC_REG.CTRL & (1u << 30));
//    ADC_REG.CTRL = regSaved;
    
    return true;
}

bool adc_disable(void)
{
    power_disable(POWER_ADC);
    ahb_clock_disable(AHBCLK_ADC);
    
    return true;
}

bool adc_trige_single_convert(uint32_t wChannel)
{
    if (wChannel >= ADC_MAX_CHANNELS) {
        return false;
    }
    
    ADC_REG.SEQA_CTRL = (1u << wChannel)    //! channel selected.
                        | (0u << 12)        //!< hardware trigger source.
                        | (0u << 18)        //!< negative edge.
                        | (0u << 19)        //!< enable synchronization.
                        | (1u << 26)        //!< start.
                        | (0u << 27)        //!< single mode.
                        | (0u << 28)        //!< continue mode.
                        | (1u << 29)        //!< High priority.
                        | (0u << 30)        //!< end of each conversion will trigger interrupt.
                        | (1u << 31);       //!< enable sequence.
    return true;
}

bool adc_stop_convert(void)
{
    ADC_REG.SEQA_CTRL = ADC_REG.SEQA_CTRL & (~(1u << 31));      //!< disable sequence.
    return true;
}

bool adc_is_work_ongoing(uint32_t ch)
{
    uint32_t reg;
    
    reg = ADC_REG.DAT[ch];
    if (reg & (1u << 31)) {
        return true;
    }
    return false;         //!< resault not available.
}

uint32_t adc_get_resault(uint32_t ch)
{
    return ADC_REG.DAT[ch] & 0xFFFFu;
}


/* EOF */
