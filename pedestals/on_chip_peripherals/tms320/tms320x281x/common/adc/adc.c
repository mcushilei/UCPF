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
#define __ADC_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\sysctrl\sc.h"
#include ".\reg_adc.h"

/*============================ MACROS ========================================*/
#define ADC_PCLK_CFG    (10MHz)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                          \
    {                                                               \
        uint32_t wStatus = PM.Power.Peripheral.GetStatus(PCLKCR_ADCENCLK);  \
        PM.Power.Peripheral.Enable(PCLKCR_ADCENCLK);                \
        __VA_ARGS__;                                                \
        PM.Power.Peripheral.ResumeStatus(PCLKCR_ADCENCLK, wStatus); \
    }


/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
//! peripheral clock enable
bool adc_power_clock_enable(void)
{
    PM.Power.Peripheral.Enable(PCLKCR_ADCENCLK);

    return true;
}

//! peripheral clock disable
bool adc_power_clock_disable(void)
{
    PM.Power.Peripheral.Disable(PCLKCR_ADCENCLK);

    return true;
}

//! initialise mode, prescla...
fsm_rt_t adc_init(void)
{
    static enum {
        ADC_INIT_START  = 0,
        ADC_INIT_CFG_BANDGAP,
        ADC_INIT_DELAY_BANDGAP,
        ADC_INIT_POWER_ON,
        ADC_INIT_DELAY,
    } s_tState = ADC_INIT_START;
    static uint32_t s_wTimer = 0;

    switch (s_tState) {
        case ADC_INIT_START:
            ADC_REG.ADCTRL1.RESET = 1;      //!< reset ADC
            s_tState = ADC_INIT_CFG_BANDGAP;
            //break;    //!< omit intentionally
        case ADC_INIT_CFG_BANDGAP:
            ADC_REG.ADCTRL3.EXTREF = 0;     //!< use internal reference regulator
            ADC_REG.ADCTRL3.ADCBGRFDN = 0x3;//!< power on internal reference regulator
            s_wTimer = 7000000;
            s_tState = ADC_INIT_DELAY_BANDGAP;
            break;

        case ADC_INIT_DELAY_BANDGAP:        //!< watit internal reference regulator to stable
            if (s_wTimer) {
                --s_wTimer;
            } else {
                s_tState = ADC_INIT_POWER_ON;
            }
            break;

        case ADC_INIT_POWER_ON:
            ADC_REG.ADCTRL3.ADCPWDN = 1;    //!< power on other analog circuitry.
            ADC_REG.ADCTRL3.ADCCLKPS = (PM.Clock.Peripheral.Get(HIGH_SPEED_PCLK) / ADC_PCLK_CFG / 2);
            s_wTimer = 70000;
            s_tState = ADC_INIT_DELAY;
            break;

        case ADC_INIT_DELAY:        //!< watit internal analog circuitry to stable
            if (s_wTimer) {
                --s_wTimer;
            } else {
                s_tState = ADC_INIT_START;
                return fsm_rt_cpl;
            }

    }

    return fsm_rt_on_going;
}

bool adc_convert_trige(void)
{
    return true;
}



/* EOF */
