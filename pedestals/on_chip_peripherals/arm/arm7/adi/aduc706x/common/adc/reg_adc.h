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


#ifndef __REG_ADC_H__
#define __REG_ADC_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define ADC_REG                         (*(adc_reg_t *)ADC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! ADC Register File
DEF_REGPAGE
    reg16_t ADCSTA;
    REG16_RSVD(1)
        
    reg16_t ADCMSKI;
    REG16_RSVD(1)

    reg8_t ADCMDE;
    REG8_RSVD(3)

    reg16_t ADC0CON;
    REG16_RSVD(1)

    reg16_t ADC1CON;
    REG16_RSVD(1)

    reg16_t ADCFLT;
    REG16_RSVD(1)

    reg8_t ADCCFG;
    REG8_RSVD(3)

    reg32_t ADC0DAT;

    reg32_t ADC1DAT;

    reg16_t ADC0OF;
    REG16_RSVD(1)

    reg16_t ADC1OF;
    REG16_RSVD(1)

    reg16_t ADC0GN;
    REG16_RSVD(1)

    reg16_t ADC1GN;
    REG16_RSVD(1)

    reg16_t ADC0RCR;
    REG16_RSVD(1)

    reg16_t ADC0RCV;
    REG16_RSVD(1)

    reg16_t ADC0TH;
    REG16_RSVD(1)

    reg16_t ADC0THC;
    REG16_RSVD(1)

    reg16_t ADC0THV;
    REG16_RSVD(1)

    reg32_t ADC0ACC;

    reg32_t ADC0ATH;

    REG32_RSVD(8)

    reg8_t IEXCON;
    REG8_RSVD(3)

END_DEF_REGPAGE(adc_reg_t)


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __REG_ADC_H__
/* EOF */
