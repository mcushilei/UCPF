/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __DRIVER_LPC17XX_REG_I2C_H__
#define __DRIVER_LPC17XX_REG_I2C_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define I2C0_REG                        (*((i2c_reg_t *)(I2C0_BASE_ADDRESS)))
#define I2C1_REG                        (*((i2c_reg_t *)(I2C1_BASE_ADDRESS)))

/*============================ MACROFIED FUNCTIONS ===========================*/
/* i2c CONSET bits */
//! \brief i2c ack flag
#define     I2C_CONSET_AA                   2
#define     I2C_CONSET_AA_MSK               (1 << I2C_CONSET_AA)

//! \brief i2c interrupt flag, is set when state change,should clr by user in CONCLR
#define     I2C_CONSET_SI                   3
#define     I2C_CONSET_SI_MSK               (1 << I2C_CONSET_SI)

//! \brief i2c stop flag
#define     I2C_CONSET_STO                  4
#define     I2C_CONSET_STO_MSK              (1 << I2C_CONSET_STO)

//! \brief i2c start flag
#define     I2C_CONSET_STA                  5
#define     I2C_CONSET_STA_MSK              (1 << I2C_CONSET_STA)

//! \brief i2c interface enable
#define     I2C_CONSET_I2EN                 6
#define     I2C_CONSET_I2EN_MSK             (1 << I2C_CONSET_I2EN)

#define     I2C_ENABLE                      0x1
#define     I2C_ASSERT_ACK                  0x1


/* i2c STAT bits */
#define     I2C_STAT_STATUS0                3
#define     I2C_STAT_STATUS1                4
#define     I2C_STAT_STATUS2                5
#define     I2C_STAT_STATUS3                6
#define     I2C_STAT_STATUS4                7
#define     I2C_STAT_STATUS_MSK             (   (1 << I2C_STAT_STATUS0)    |\
                                                (1 << I2C_STAT_STATUS1)     |\
                                                (1 << I2C_STAT_STATUS2)     |\
                                                (1 << I2C_STAT_STATUS3)     |\
                                                (1 << I2C_STAT_STATUS4))

/* i2c ADR0 bits */
#define     I2C_ADR_GC                     0
#define     I2C_ADR_GC_MSK                 (1 << I2C_ADR_GC)

#define     I2C_ADR_ADDRESS0               1
#define     I2C_ADR_ADDRESS1               2
#define     I2C_ADR_ADDRESS2               3
#define     I2C_ADR_ADDRESS3               4
#define     I2C_ADR_ADDRESS4               5
#define     I2C_ADR_ADDRESS5               6
#define     I2C_ADR_ADDRESS6               7
#define     I2C_ADR_ADDRESS_MSK            (   (1 << I2C_ADR_ADDRESS0)    |\
                                                (1 << I2C_ADR_ADDRESS1)    |\
                                                (1 << I2C_ADR_ADDRESS2)    |\
                                                (1 << I2C_ADR_ADDRESS3)    |\
                                                (1 << I2C_ADR_ADDRESS4)    |\
                                                (1 << I2C_ADR_ADDRESS5)    |\
                                                (1 << I2C_ADR_ADDRESS6))

/* i2c MASK0 bits */
#define     I2C_ADR_MASK0                   1
#define     I2C_ADR_MASK1                   2
#define     I2C_ADR_MASK2                   3
#define     I2C_ADR_MASK3                   4
#define     I2C_ADR_MASK4                   5
#define     I2C_ADR_MASK5                   6
#define     I2C_ADR_MASK6                   7
#define     I2C_ADR_MASK_MSK                    (   (1 << I2C_ADR_MASK0)    |\
                                                (1 << I2C_ADR_MASK1)    |\
                                                (1 << I2C_ADR_MASK2)    |\
                                                (1 << I2C_ADR_MASK3)    |\
                                                (1 << I2C_ADR_MASK4)    |\
                                                (1 << I2C_ADR_MASK5)    |\
                                                (1 << I2C_ADR_MASK6))

/* i2c SCLH bits */
#define     I2C_SCLH_SCLH0                  0
#define     I2C_SCLH_SCLH1                  1
#define     I2C_SCLH_SCLH2                  2
#define     I2C_SCLH_SCLH3                  3
#define     I2C_SCLH_SCLH4                  4
#define     I2C_SCLH_SCLH5                  5
#define     I2C_SCLH_SCLH6                  6
#define     I2C_SCLH_SCLH7                  7
#define     I2C_SCLH_SCLH8                  8
#define     I2C_SCLH_SCLH9                  9
#define     I2C_SCLH_SCLH10                 10
#define     I2C_SCLH_SCLH11                 11
#define     I2C_SCLH_SCLH12                 12
#define     I2C_SCLH_SCLH13                 13
#define     I2C_SCLH_SCLH14                 14
#define     I2C_SCLH_SCLH15                 15
#define     I2C_SCLH_SCLH_MSK               (   (1 << I2C_SCLH_SCLH0)   |\
                                                (1 << I2C_SCLH_SCLH1)   |\
                                                (1 << I2C_SCLH_SCLH2)   |\
                                                (1 << I2C_SCLH_SCLH3)   |\
                                                (1 << I2C_SCLH_SCLH4)   |\
                                                (1 << I2C_SCLH_SCLH5)   |\
                                                (1 << I2C_SCLH_SCLH6)   |\
                                                (1 << I2C_SCLH_SCLH7)   |\
                                                (1 << I2C_SCLH_SCLH8)   |\
                                                (1 << I2C_SCLH_SCLH9)   |\
                                                (1 << I2C_SCLH_SCLH10)  |\
                                                (1 << I2C_SCLH_SCLH11)  |\
                                                (1 << I2C_SCLH_SCLH12)  |\
                                                (1 << I2C_SCLH_SCLH13)  |\
                                                (1 << I2C_SCLH_SCLH14)  |\
                                                (1 << I2C_SCLH_SCLH15))

/* i2c SCLL bits */
#define     I2C_SCLL_SCLL0                  0
#define     I2C_SCLL_SCLL1                  1
#define     I2C_SCLL_SCLL2                  2
#define     I2C_SCLL_SCLL3                  3
#define     I2C_SCLL_SCLL4                  4
#define     I2C_SCLL_SCLL5                  5
#define     I2C_SCLL_SCLL6                  6
#define     I2C_SCLL_SCLL7                  7
#define     I2C_SCLL_SCLL8                  8
#define     I2C_SCLL_SCLL9                  9
#define     I2C_SCLL_SCLL10                 10
#define     I2C_SCLL_SCLL11                 11
#define     I2C_SCLL_SCLL12                 12
#define     I2C_SCLL_SCLL13                 13
#define     I2C_SCLL_SCLL14                 14
#define     I2C_SCLL_SCLL15                 15
#define     I2C_SCLL_SCLL_MSK               (   (1 << I2C_SCLL_SCLL0)   |\
                                                (1 << I2C_SCLL_SCLL1)   |\
                                                (1 << I2C_SCLL_SCLL2)   |\
                                                (1 << I2C_SCLL_SCLL3)   |\
                                                (1 << I2C_SCLL_SCLL4)   |\
                                                (1 << I2C_SCLL_SCLL5)   |\
                                                (1 << I2C_SCLL_SCLL6)   |\
                                                (1 << I2C_SCLL_SCLL7)   |\
                                                (1 << I2C_SCLL_SCLL8)   |\
                                                (1 << I2C_SCLL_SCLL9)   |\
                                                (1 << I2C_SCLL_SCLL10)  |\
                                                (1 << I2C_SCLL_SCLL11)  |\
                                                (1 << I2C_SCLL_SCLL12)  |\
                                                (1 << I2C_SCLL_SCLL13)  |\
                                                (1 << I2C_SCLL_SCLL14)  |\
                                                (1 << I2C_SCLL_SCLL15))

/* i2c CONCLR bits */
//! \brief i2c ack clear bit
#define     I2C_CONCLR_AAC                  2
#define     I2C_CONCLR_AAC_MSK              (1 << I2C_CONCLR_AAC)

//! \brief i2c interrupt clear bit
#define     I2C_CONCLR_SIC                  3
#define     I2C_CONCLR_SIC_MSK              (1 << I2C_CONCLR_SIC)

//! \brief i2c START flag clear bit
#define     I2C_CONCLR_STAC                 5
#define     I2C_CONCLR_STAC_MSK             (1 << I2C_CONCLR_STAC)

//! \brief i2c disable bit
#define     I2C_CONCLR_ENC                  6
#define     I2C_CONCLR_ENC_MSK              (1 << I2C_CONCLR_ENC)

#define     I2C_DISABLE                     0x0
#define     I2C_NOT_ASSERT_ACK              0x0

/* i2c MMCTRL bits */
//! \brief i2c monitor enable bit
#define     I2C_MMCTRL_MMENA                0
#define     I2C_MMCTRL_MMENA_MSK            (1 << I2C_MMCTRL_MMENA)

//! \brief i2c monitor out enable bit
#define     I2C_MMCTRL_ENASCL               1
#define     I2C_MMCTRL_ENASCL_MSK           (1 << I2C_MMCTRL_ENASCL)

//! \brief i2c monitor match all address bit
#define     I2C_MMCTRL_MATCHALL             2
#define     I2C_MMCTRL_MATCHALL_MSK         (1 << I2C_MMCTRL_MATCHALL)

#define     I2C_MONITOR_ENABLE              1
#define     I2C_MONITOR_OUT_ENABLE          1
#define     I2C_MONITOR_MATCH_ALL_ADDRESS   1


#define     I2C_SCLH_SET(__VALUE)           ((__VALUE) << I2C_SCLH_SCLH0)
#define     I2C_SCLL_SET(__VALUE)           ((__VALUE) << I2C_SCLL_SCLL0)
#define     I2C_MMCTRL_MMENA_SET(__VALUE)   ((__VALUE) << I2C_MMCTRL_MMENA)
#define     I2C_MMCTRL_ENASCL_SET(__VALUE)  ((__VALUE) << I2C_MMCTRL_ENASCL)
#define     I2C_MMCTRL_MATCHALL_SET(__VAL)  ((__VAL) << I2C_MMCTRL_MATCHALL)
#define     I2C_ADR_GC_SET(__VALUE)        ((__VALUE) << I2C_ADR_GC)
#define     I2C_ADR_ADDRESS_SET(__VALUE)   ((__VALUE) << I2C_ADR_ADDRESS0)
#define     I2C_ADR1_GC_SET(__VALUE)        ((__VALUE) << I2C_ADR1_GC)
#define     I2C_ADR1_ADDRESS_SET(__VALUE)   ((__VALUE) << I2C_ADR1_ADDRESS0)
#define     I2C_ADR2_GC_SET(__VALUE)        ((__VALUE) << I2C_ADR2_GC)
#define     I2C_ADR2_ADDRESS_SET(__VALUE)   ((__VALUE) << I2C_ADR2_ADDRESS0)
#define     I2C_ADR3_GC_SET(__VALUE)        ((__VALUE) << I2C_ADR3_GC)
#define     I2C_ADR3_ADDRESS_SET(__VALUE)   ((__VALUE) << I2C_ADR3_ADDRESS0)
#define     I2C_ADR_MASK_MSK_SET(__VALUE)       ((__VALUE) << I2C_ADR_MASK_MSK0)
#define     I2C_MSK1_MSK_SET(__VALUE)       ((__VALUE) << I2C_MSK1_MSK0)
#define     I2C_MSK2_MSK_SET(__VALUE)       ((__VALUE) << I2C_MSK2_MSK0)
#define     I2C_MSK3_MSK_SET(__VALUE)       ((__VALUE) << I2C_MSK3_MSK0)


/*============================ TYPES =========================================*/
/* Inter-Integrated Circuit (I2C) register page type*/
typedef volatile struct{
    reg32_t CONSET;
    reg32_t STAT;
    reg32_t DAT;
    reg32_t ADR0;
    reg32_t SCLH;
    reg32_t SCLL;
    reg32_t CONCLR;
    reg32_t MMCTRL;
    reg32_t ADR1;
    reg32_t ADR2;
    reg32_t ADR3;
    reg32_t DATABUFFER;
    reg32_t MASK0;
    reg32_t MASK1;
    reg32_t MASK2;
    reg32_t MASK3;
} i2c_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif