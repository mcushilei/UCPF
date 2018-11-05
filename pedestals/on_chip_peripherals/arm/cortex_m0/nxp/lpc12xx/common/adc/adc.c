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
#define __DRIVER_ADC_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_adc.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
#define ADC_CHANNEL_COUNT               (8u)
#define ADC_CHANNEL_COUNT_MSK           (ADC_CHANNEL_COUNT - 1)

#define ADC_MAX_CLOCK_FRQ               (9MHz)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus   = PM_AHB_CLK_GET_STATUS(AHBCLK_ADC);        \
        PM_AHB_CLK_ENABLE(AHBCLK_ADC);                                 \
        __VA_ARGS__;                                                        \
        PM_AHB_CLK_RESUME(AHBCLK_ADC, tStatus);                        \
    }
/*============================ TYPES =========================================*/
typedef enum {
    ADC_CHANNEL_0   = 0,
    ADC_CHANNEL_1   = 1,
    ADC_CHANNEL_2   = 2,
    ADC_CHANNEL_3   = 3,
    ADC_CHANNEL_4   = 4,
    ADC_CHANNEL_5   = 5,
    ADC_CHANNEL_6   = 6,
    ADC_CHANNEL_7   = 7,
} em_adc_channel_t;

/*============================ PROTOTYPES ====================================*/
extern bool adc_enable(void);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool adc_init(void)
{
    uint32_t wCoreClock = PM_CORE_CLK_GET();

    //!< ADC peripheral clock divide
    if (wCoreClock > ADC_MAX_CLOCK_FRQ) {
        wCoreClock = (wCoreClock + ADC_MAX_CLOCK_FRQ - 1) / ADC_MAX_CLOCK_FRQ;
        wCoreClock--;
    } else {
        wCoreClock = 0;
    }
    
    SAFE_CLK_CODE(
        ADC_REG.INTEN.Value = 0;
        ADC_REG.CR.CLKDIV = wCoreClock;
        ADC_REG.CR.PDN = 1;
    )
    
    return true;
}

bool adc_enable(void)
{
    PM.Power.Enable(POWER_ADC);
    PM_AHB_CLK_ENABLE(AHBCLK_ADC);

    return true;
}

bool adc_disable(void)
{
    PM_AHB_CLK_DISABLE(AHBCLK_ADC);
    PM.Power.Disable(POWER_ADC);

    return true;
}

bool adc_trige_single_convert(uint32_t wChannel)
{
    if (wChannel >= ADC_CHANNEL_COUNT) {
        return false;
    }

    ADC_REG.CR.START = 0;       //!< stop conversion
    ADC_REG.CR.BURST = 0;       //!< single mode
    ADC_REG.CR.SEL = BIT(wChannel);     //!< select channel
    ADC_REG.CR.START = 1;       //!< start conversion
    
    return true;
}

bool adc_stop_convert(void)
{
    ADC_REG.CR.START = 0;       //!< stop conversion
    
    return true;
}

bool adc_check_channel_statue(uint32_t wChannel)
{
    if (ADC_REG.DR[wChannel & ADC_CHANNEL_COUNT_MSK].DONE) {
        return true;
    }

    return false;
}

uint32_t adc_get_channel_resault(uint32_t wChannel)
{
    return ADC_REG.DR[wChannel & ADC_CHANNEL_COUNT_MSK].RESULT;
}


/* EOF */
