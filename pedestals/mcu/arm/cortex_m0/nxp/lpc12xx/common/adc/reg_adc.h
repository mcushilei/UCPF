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
#define ADC_REG                         (*(volatile adc_reg_t *)ADC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
TYPEDEF_REG32
    reg32_t         :6;
    reg32_t RESULT  :10;
    reg32_t         :14;
    reg32_t OVERRUN :1;
    reg32_t DONE    :1;
END_TYPEDEF_REG32(adc_reg_dr_t)

//! ADC Register File
DEF_REGPAGE
    DEF_REG32                   //!< Offset: 0x000 A/D Control Register
        reg32_t SEL     :8;
        reg32_t CLKDIV  :8;
        reg32_t BURST   :1;
        reg32_t         :4;
        reg32_t PDN     :1;
        reg32_t         :2;
        reg32_t START   :3;
        reg32_t EDGE    :1;
        reg32_t         :4;
    END_DEF_REG32(CR)

    DEF_REG32                   //!< Offset: 0x004 A/D Global Data Register
        reg32_t         :6;
        reg32_t RESULT  :10;
        reg32_t         :8;
        reg32_t CHN     :3;
        reg32_t         :3;
        reg32_t OVERRUN :1;
        reg32_t DONE    :1;
    END_DEF_REG32(GDR)

    REG16_RSVD(1)

    DEF_REG32                   //!< Offset: 0x00C A/D Interrupt Enable Register
        reg32_t ADINTEN :8;
        reg32_t ADGINTEN:1;
        reg32_t         :16;
    END_DEF_REG32(INTEN)

    union{
        adc_reg_dr_t DR[8];     //!< Offset: 0x010-0x02C A/D Channel 0..7 Data Register (R/W) */
        struct{
            adc_reg_dr_t DR0;					//!< Offset: 0x010 A/D Channel Register 0 (R/W) */
            adc_reg_dr_t DR1;					//!< Offset: 0x014 A/D Channel Register 1 (R/W) */
            adc_reg_dr_t DR2;					//!< Offset: 0x018 A/D Channel Register 2 (R/W) */
            adc_reg_dr_t DR3;					//!< Offset: 0x01C A/D Channel Register 3 (R/W) */
            adc_reg_dr_t DR4;					//!< Offset: 0x020 A/D Channel Register 4 (R/W) */
            adc_reg_dr_t DR5;					//!< Offset: 0x024 A/D Channel Register 5 (R/W) */
            adc_reg_dr_t DR6;					//!< Offset: 0x028 A/D Channel Register 6 (R/W) */
            adc_reg_dr_t DR7;					//!< Offset: 0x02C A/D Channel Register 7 (R/W) */
        };
    };

    DEF_REG32                   //!< Offset: 0x030       A/D Status Register
        reg32_t DONE    :8;
        reg32_t OVERRUN :8;
        reg32_t ADINT   :1;
        reg32_t         :15;
    END_DEF_REG32(STAT)

    DEF_REG32                   //!< Offset: 0x030       A/D Status Register
        reg32_t         :4;
        reg32_t ADCOFFS :4;
        reg32_t         :24;
    END_DEF_REG32(TRM)
END_DEF_REGPAGE(adc_reg_t)


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __REG_ADC_H__
/* EOF */
