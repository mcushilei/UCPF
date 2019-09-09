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
#include "..\sys\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool adc_enable(void)
{
    return true;
}

bool adc_disable(void)
{
	ADC_REG.ADCMDE  = (1u << 7)         //!< 512K clock.
                    | (0u << 5)         //!< ADC low power mode disable.
                    | (0x0u << 3)       //!< ADC low poer mode: normal.
                    | (0x00u << 0);     //!< ADC operation mode: Power down.
    return true;
}

bool adc_init(void)
{
    volatile uint32_t wTry, wDummyRead;

	ADC_REG.ADCMSKI = (0u << 6)				    // Disnable all ADC interrupt
                    | (0u << 4)
                    | (0u << 3)
                    | (0u << 1)
                    | (0u << 0);
    
	ADC_REG.ADCMDE  = (1u << 7)         //!< 512K clock.
                    | (0u << 5)         //!< ADC low power mode disable.
                    | (0x0u << 3)       //!< ADC low poer mode: normal.
                    | (0x03u << 0);     //!< ADC operation mode: Idle.
    
	ADC_REG.ADCFLT  = (1u << 15)        //!< CHOP enable.
                    | (1u << 14)        //!< AVG2 enable.
                    | (63u << 8)         //!< AF: 63 + 1
                    | (0u << 7)
                    | (0u << 0);
    
 	ADC_REG.ADCCFG  = (0u << 7)         //!< analog ground switch disable.
                    | (0u << 5)         //!< ADC0 accumulator mode: disable.
                    | (0u << 3)         //!< ADC0 comparator mode: disable.
                    | (0u << 2)         //!< ADC0 overrange detect disable.
                    | (0u << 1)         //!< 20K disable.
                    | (0u << 0);        //!< ADC0 result counter disable.

	ADC_REG.ADC0CON = (1u << 15)        //!< Power on ADC0.
                    | (0u << 13)        //!< current source off.
                    | (0u << 12)        //!< using internal reference.
                    | (0u << 11)        //!< 
                    | (1u << 10)        //!< resault coding as unipolar.
                    | (0x0u << 6)       //!< ADC channel: ADC0/ADC1 differential.
                    | (0u << 4)         //!< ADC reference: internal reference.
                    | (0u << 0);        //!< PGA gain: 1

	ADC_REG.ADC1CON = (1u << 15)        //!< Power on ADC1.
                    | (0u << 13)        //!< current source off.
                    | (0u << 12)        //!< using internal reference.
                    | (1u << 11)        //!< resault coding as unipolar.
                    | (0u << 7)         //!< ADC channel: ADC2/ADC3 differential.
                    | (0x0u << 4)       //!< ADC reference: internal reference.
                    | (0u << 2)         //!< Buffer bypass: full buffer on
                    | (0u << 0);        //!< PGA gain: 1

    wDummyRead = ADC_REG.ADC1DAT;
    wDummyRead = ADC_REG.ADC0DAT;
	ADC_REG.ADCMDE  = (1u << 7)         //!< 512K clock.
                    | (0u << 5)         //!< ADC low power mode disable.
                    | (0x0u << 3)       //!< ADC low poer mode: normal.
                    | (0x04u << 0);     //!< ADC operation mode: Offest Self Calibration
    wTry = 50000;
    while (!(ADC_REG.ADCSTA & (1u << 15)) && wTry) {   // Wait for Calibration routine to ready.
        wTry--;
    }
    if (0 == wTry) {
        return false;
    }

    wDummyRead = ADC_REG.ADC1DAT;
    wDummyRead = ADC_REG.ADC0DAT;
	ADC_REG.ADCMDE  = (1u << 7)         //!< 512K clock.
                    | (0u << 5)         //!< ADC low power mode disable.
                    | (0x0u << 3)       //!< ADC low poer mode: normal.
                    | (0x05u << 0);     //!< ADC operation mode: Self Gain Calibration
    wTry = 50000;
    while (!(ADC_REG.ADCSTA & (1u << 15)) && wTry) {// Wait for Calibration routine to complete
        wTry--;
    }
    if (0 == wTry) {
        return false;
    }

    return true;
}

bool adc_deinit(void)
{
    return true;
}

bool adc_config(void)
{
    return true;
}

bool adc_trigge(void)
{
    return true;
}

bool adc0_get_resault(uint32_t *pRes)
{
    return true;
}

bool adc1_get_resault(uint32_t *pRes)
{
    return true;
}

/* EOF */
