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
#define ADC_REG         (*(adc_reg_t *)SYSTEM_BASE_ADDRESS)

#define ADC_ADCTRL3_SMODE_SEL_BIT0      0
#define ADC_ADCTRL3_SMODE_SEL_MSK       (1u << ADC_ADCTRL3_SMODE_SEL_BIT0)
#define ADC_ADCTRL3_ADCCLKPS_BIT0       1
#define ADC_ADCTRL3_ADCCLKPS_BIT1       2
#define ADC_ADCTRL3_ADCCLKPS_BIT2       3
#define ADC_ADCTRL3_ADCCLKPS_BIT3       4
#define ADC_ADCTRL3_ADCCLKPS_MSK        ((1u << ADC_ADCTRL3_ADCCLKPS_BIT0)      \
                                        |(1u << ADC_ADCTRL3_ADCCLKPS_BIT1)      \
                                        |(1u << ADC_ADCTRL3_ADCCLKPS_BIT2)      \
                                        |(1u << ADC_ADCTRL3_ADCCLKPS_BIT3))
#define ADC_ADCTRL3_ADCPWDN_BIT0        5
#define ADC_ADCTRL3_ADCPWDN_MSK         (1u << ADC_ADCTRL3_ADCPWDN_BIT0)
#define ADC_ADCTRL3_ADCBGRFDN_BIT0      6
#define ADC_ADCTRL3_ADCBGRFDN_BIT1      7
#define ADC_ADCTRL3_ADCBGRFDN_MSK       ((1u << ADC_ADCTRL3_ADCBGRFDN_BIT0)     \
                                        |(1u << ADC_ADCTRL3_ADCBGRFDN_BIT1))
#define ADC_ADCTRL3_EXTREF_BIT0         8
#define ADC_ADCTRL3_EXTREF_MSK          (1u << ADC_ADCTRL3_EXTREF_BIT0)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct {
    // ADC Control 1
    DEF_REG16
        reg16_t             :4;     // 3:0   reserved
        reg16_t SEQ_CASC    :1;     // 4     Cascaded sequencer mode
        reg16_t SEQ_OVRD    :1;     // 5     Sequencer override
        reg16_t CONT_RUN    :1;     // 6     Continuous run
        reg16_t CPS         :1;     // 7     ADC core clock pre-scalar
        reg16_t ACQ_PS      :4;     // 11:8  Acquisition window size
        reg16_t SUSMOD      :2;     // 13:12 Emulation suspend mode
        reg16_t RESET       :1;     // 14    ADC reset
        reg16_t             :1;     // 15    reserved
    END_DEF_REG16(ADCTRL1)

    // ADC Control 2
    DEF_REG16
        reg16_t EVB_SOC_SEQ2:1;     // 0     Event manager B SOC mask for SEQ2
        reg16_t             :1;     // 1     reserved
        reg16_t INT_MOD_SEQ2:1;     // 2     SEQ2 Interrupt mode
        reg16_t INT_ENA_SEQ2:1;     // 3     SEQ2 Interrupt enable
        reg16_t             :1;     // 4     reserved
        reg16_t SOC_SEQ2    :1;     // 5     Start of conversion for SEQ2
        reg16_t RST_SEQ2    :1;     // 6     Reset SEQ2
        reg16_t EXT_SOC_SEQ1:1;     // 7     External start of conversion for SEQ1
        reg16_t EVA_SOC_SEQ1:1;     // 8     Event manager A SOC mask for SEQ1
        reg16_t             :1;     // 9     reserved
        reg16_t INT_MOD_SEQ1:1;     // 10    SEQ1 Interrupt mode
        reg16_t INT_ENA_SEQ1:1;     // 11    SEQ1 Interrupt enable
        reg16_t             :1;     // 12    reserved
        reg16_t SOC_SEQ1    :1;     // 13    Start of conversion trigger for SEQ1
        reg16_t RST_SEQ1    :1;     // 14    Restart sequencer 1
        reg16_t EVB_SOC_SEQ :1;     // 15    EVB SOC enable
    END_DEF_REG16(ADCTRL2)

    // Max conversions
    DEF_REG16
        reg16_t MAX_CONV1:4;       // 3:0   Max number of conversions
        reg16_t MAX_CONV2:3;       // 6:4   Max number of conversions
        reg16_t          :9;       // 15:7  reserved
    END_DEF_REG16(ADCMAXCONV)

    // Channel select sequencing control 1
    DEF_REG16
        reg16_t CONV00:4;         // 3:0    Conversion selection 00
        reg16_t CONV01:4;         // 7:4    Conversion selection 01
        reg16_t CONV02:4;         // 11:8   Conversion selection 02
        reg16_t CONV03:4;         // 15:12  Conversion selection 03
    END_DEF_REG16(ADCCHSELSEQ1)

    // Channel select sequencing control 2
    DEF_REG16
        reg16_t CONV04:4;         // 3:0    Conversion selection 04
        reg16_t CONV05:4;         // 7:4    Conversion selection 05
        reg16_t CONV06:4;         // 11:8   Conversion selection 06
        reg16_t CONV07:4;         // 15:12  Conversion selection 07
    END_DEF_REG16(ADCCHSELSEQ2)

    // Channel select sequencing control 3
    DEF_REG16
        reg16_t CONV08:4;         // 3:0    Conversion selection 08
        reg16_t CONV09:4;         // 7:4    Conversion selection 09
        reg16_t CONV10:4;         // 11:8   Conversion selection 10
        reg16_t CONV11:4;         // 15:12  Conversion selection 11
    END_DEF_REG16(ADCCHSELSEQ3)

    // Channel select sequencing control 4
    DEF_REG16
        reg16_t CONV12:4;         // 3:0    Conversion selection 12
        reg16_t CONV13:4;         // 7:4    Conversion selection 13
        reg16_t CONV14:4;         // 11:8   Conversion selection 14
        reg16_t CONV15:4;         // 15:12  Conversion selection 15
    END_DEF_REG16(ADCCHSELSEQ4)

    // Autosequence status register
    DEF_REG16
        reg16_t SEQ1_STATE:4;       // 3:0    SEQ1 state
        reg16_t SEQ2_STATE:3;       // 6:4    SEQ2 state
        reg16_t           :1;       // 7      reserved
        reg16_t SEQ_CNTR  :4;       // 11:8   Sequencing counter status
        reg16_t           :4;       // 15:12  reserved
    END_DEF_REG16(ADCASEQSR)

    reg16_t     ADCRESULT[16];      // Conversion Result Buffer 0 ~ 15
    
    // ADC Control 3
    DEF_REG16
        reg16_t SMODE_SEL   :1;     // 0      Sampling mode select
        reg16_t ADCCLKPS    :4;     // 4:1    ADC core clock divider
        reg16_t ADCPWDN     :1;     // 5      ADC powerdown
        reg16_t ADCBGRFDN   :2;     // 7:6    ADC bandgap/ref power down
        reg16_t EXTREF      :1;     // 8      ADC external reference
        reg16_t             :7;     // 15:9   reserved
    END_DEF_REG16(ADCTRL3)

    // ADC Status Register
    DEF_REG16
        reg16_t INT_SEQ1    :1;     // 0      SEQ1 Interrupt flag
        reg16_t INT_SEQ2    :1;     // 1      SEQ2 Interrupt flag
        reg16_t SEQ1_BSY    :1;     // 2      SEQ1 busy status
        reg16_t SEQ2_BSY    :1;     // 3      SEQ2 busy status
        reg16_t INT_SEQ1_CLR:1;     // 4      SEQ1 Interrupt clear
        reg16_t INT_SEQ2_CLR:1;     // 5      SEQ2 Interrupt clear
        reg16_t EOS_BUF1    :1;     // 6      End of sequence buffer1
        reg16_t EOS_BUF2    :1;     // 7      End of sequence buffer2
        reg16_t             :8;     // 15:8   reserved
    END_DEF_REG16(ADCST)
} adc_reg_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __REG_ADC_H__
/* EOF */
