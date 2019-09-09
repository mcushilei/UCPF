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


#ifndef __DRIVER_REG_ADC_H__
#define __DRIVER_REG_ADC_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define ADC_REG                         (*(adc_reg_t *)ADC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! ADC Register File
typedef volatile struct {
    reg32_t     CTRL;
    REG32_RSVD(1)
    reg32_t     SEQA_CTRL;
    reg32_t     SEQB_CTRL;
    reg32_t     SEQA_GDAT;
    reg32_t     SEQB_GDAT;
    REG32_RSVD(2)
    union{
        reg32_t DAT[12];
        struct{
            reg32_t DAT0;
            reg32_t DAT1;
            reg32_t DAT2;
            reg32_t DAT3;
            reg32_t DAT4;
            reg32_t DAT5;
            reg32_t DAT6;
            reg32_t DAT7;
            reg32_t DAT8;
            reg32_t DAT9;
            reg32_t DAT10;
            reg32_t DAT11;
        };
    };
    reg32_t     THR0_LOW;
    reg32_t     THR1_LOW;
    reg32_t     THR0_HIGH;
    reg32_t     THR1_HIGH;
    reg32_t     CHAN_THRSEL;
    reg32_t     INTEN;
    reg32_t     FLAGS;
    reg32_t     TRM;
} adc_reg_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __DRIVER_REG_ADC_H__
/* EOF */
