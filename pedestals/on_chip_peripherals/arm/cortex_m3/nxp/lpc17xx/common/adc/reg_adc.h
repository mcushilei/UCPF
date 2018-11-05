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
DEF_REGPAGE
    reg32_t     CR;
    reg32_t     GDR;
    REG32_RSVD(1)
    reg32_t     INTEN;
    union{
        reg32_t DR[8];
        struct{
            reg32_t DR0;
            reg32_t DR1;
            reg32_t DR2;
            reg32_t DR3;
            reg32_t DR4;
            reg32_t DR5;
            reg32_t DR6;
            reg32_t DR7;
        };
    };
    reg32_t     STAT;
    reg32_t     TRM;
END_DEF_REGPAGE(adc_reg_t)


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __DRIVER_REG_ADC_H__
/* EOF */
