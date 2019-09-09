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

#ifndef __DRIVER_REG_SSP_H__
#define __DRIVER_REG_SSP_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SSP0_REG                        (*((ssp_reg_t *)SSP0_BASE_ADDRESS))
#define SSP1_REG                        (*((ssp_reg_t *)SSP1_BASE_ADDRESS))

/*============================ MACROFIED FUNCTIONS ===========================*/
/* spi datasize*/
#define     SSP_CR0_DSS0                        0
#define     SSP_CR0_DSS1                        1
#define     SSP_CR0_DSS2                        2
#define     SSP_CR0_DSS3                        3
#define     SSP_CR0_DSS_MSK                     (   (1<<SSP_CR0_DSS0)   |\
                                                    (1<<SSP_CR0_DSS1)   |\
                                                    (1<<SSP_CR0_DSS2)   |\
                                                    (1<<SSP_CR0_DSS3))

#define     SSP_DATASIZE_4                      0x3
#define     SSP_DATASIZE_5                      0x4
#define     SSP_DATASIZE_6                      0x5
#define     SSP_DATASIZE_7                      0x6
#define     SSP_DATASIZE_8                      0x7
#define     SSP_DATASIZE_9                      0x8
#define     SSP_DATASIZE_10                     0x9
#define     SSP_DATASIZE_11                     0xA
#define     SSP_DATASIZE_12                     0xB
#define     SSP_DATASIZE_13                     0xC
#define     SSP_DATASIZE_14                     0xD
#define     SSP_DATASIZE_15                     0xE
#define     SSP_DATASIZE_16                     0xF


/* spi format*/
#define     SSP_CR0_FRF0                        4
#define     SSP_CR0_FRF1                        5
#define     SSP_CR0_FRF_MSK                     (   (1<<SSP_CR0_FRF0)  |\
                                                    (1<<SSP_CR0_FRF1))

#define     SSP_FORMAT_SPI                      0x0
#define     SSP_FORMAT_TI                       0x1
#define     SSP_FORMAT_MICROWIRE                0x2


/* spi clk polarity*/
#define     SSP_CR0_CPOL                        6
#define     SSP_CR0_CPOL_MSK                    (1<<SSP_CR0_CPOL)

#define     SSP_CLK_IDLE_LOW                    0x0
#define     SSP_CLK_IDLE_HIGH                   0x1


/* spi sample*/
#define     SSP_CR0_CPHA                        7
#define     SSP_CR0_CPHA_MSK                    (1<<SSP_CR0_CPHA)

#define     SSP_SAMP_FIRST_EDGE                 0x0
#define     SSP_SAMP_SECOND_EDGE                0x1


/* spi serial clock rate*/
#define     SSP_CR0_SCR0                        8
#define     SSP_CR0_SCR1                        9
#define     SSP_CR0_SCR2                        10
#define     SSP_CR0_SCR3                        11
#define     SSP_CR0_SCR4                        12
#define     SSP_CR0_SCR5                        13
#define     SSP_CR0_SCR6                        14
#define     SSP_CR0_SCR7                        15
#define     SSP_CR0_SCR_MSK                     (   (1<<SSP_CR0_SCR0)      |\
                                                    (1<<SSP_CR0_SCR1)      |\
                                                    (1<<SSP_CR0_SCR2)      |\
                                                    (1<<SSP_CR0_SCR3)      |\
                                                    (1<<SSP_CR0_SCR4)      |\
                                                    (1<<SSP_CR0_SCR5)      |\
                                                    (1<<SSP_CR0_SCR6)      |\
                                                    (1<<SSP_CR0_SCR7))



/* spi loop back mode*/
#define     SSP_CR1_LBM                         0
#define     SSP_CR1_LBM_MSK                     (1<<SSP_CR1_LBM)

#define     SSP_NOT_LOOP_BACK                   0x0
#define     SSP_LOOP_BACK                       0x1


/* spi enable*/
#define     SSP_CR1_SSE                         1
#define     SSP_CR1_SSE_MSK                     (1<<SSP_CR1_SSE)

#define     SSP_DISABLE                         0x0
#define     SSP_ENABLE                          0x1


/* spi master or slave mode*/
#define     SSP_CR1_MS                          2
#define     SSP_CR1_MS_MSK                      (1<<SSP_CR1_MS)

#define     SSP_MASTER                          0x0
#define     SSP_SLAVE                           0x1


/* spi slave out disable*/
#define     SSP_CR1_SOD                         3
#define     SSP_CR1_SOD_MSK                     (1<<SSP_CR1_SOD)

#define     SSP_OUT_ENABLE                      0x0
#define     SSP_OUT_DISABLE                     0x1

/* spi transmit fifo empty*/
#define     SSP_SR_TFE                          0
#define     SSP_SR_TFE_MSK                      (1<<SSP_SR_TFE)

/* spi transmit fifo not full*/
#define     SSP_SR_TNF                          1
#define     SSP_SR_TNF_MSK                      (1<<SSP_SR_TNF)

/* spi receive fifo not empty*/
#define     SSP_SR_RNE                          2
#define     SSP_SR_RNE_MSK                      (1<<SSP_SR_RNE)

/* spi receive fifo full*/
#define     SSP_SR_RFF                          3
#define     SSP_SR_RFF_MSK                      (1<<SSP_SR_RFF)

/* spi is busy*/
#define     SSP_SR_BUSY                         4
#define     SSP_SR_BSY_MSK                      (1<<SSP_SR_BUSY)


/* spi clock prescale*/
#define     SSP_CPSR_CPSDVSR0                   0
#define     SSP_CPSR_CPSDVSR1                   1
#define     SSP_CPSR_CPSDVSR2                   2
#define     SSP_CPSR_CPSDVSR3                   3
#define     SSP_CPSR_CPSDVSR4                   4
#define     SSP_CPSR_CPSDVSR5                   5
#define     SSP_CPSR_CPSDVSR6                   6
#define     SSP_CPSR_CPSDVSR7                   7
#define     SSP_CPSR_CPSDVSR_MSK                (   (1<<SSP_CPSR_CPSDVSR0) |\
                                                    (1<<SSP_CPSR_CPSDVSR1) |\
                                                    (1<<SSP_CPSR_CPSDVSR2) |\
                                                    (1<<SSP_CPSR_CPSDVSR3) |\
                                                    (1<<SSP_CPSR_CPSDVSR4) |\
                                                    (1<<SSP_CPSR_CPSDVSR5) |\
                                                    (1<<SSP_CPSR_CPSDVSR6) |\
                                                    (1<<SSP_CPSR_CPSDVSR7))


#define     SSP_CPSR_SET(__VALUE)               ((__VALUE) << SSP_CPSR_CPSDVSR0)
#define     SSP_SLAVE_OUT_SET(__MODE)           ((__MODE) << SSP_CR1_SOD)
#define     SSP_DATASIZE_SET(__SIZE)            ((__SIZE) << SSP_CR0_DSS0)
#define     SSP_FORMAT_SET(__MODE)              ((__MODE) << SSP_CR0_FRF0)
#define     SSP_CLK_POLARITY_SET(__MODE)        ((__MODE) << SSP_CR0_CPOL)
#define     SSP_CLK_SAMP_SET(__MODE)            ((__MODE) << SSP_CR0_CPHA)
#define     SSP_SCR_SET(__VALUE)                ((__VALUE) << SSP_CR0_SCR0)
#define     SSP_LOOP_BACK_SET(__MODE)           ((__MODE) << SSP_CR1_LBM)
#define     SSP_ENABLE_SET(__MODE)              ((__MODE) << SSP_CR1_SSE)
#define     SSP_MODE_SET(__MODE)                ((__MODE) << SSP_CR1_MS)

/*============================ TYPES =========================================*/
/* spi spi mode register page type*/
typedef volatile struct{
    reg32_t CR0;
    reg32_t CR1;
    reg32_t DR;
    reg32_t SR;
    reg32_t CPSR;
    reg32_t IMSC;
    reg32_t RIS;
    reg32_t MIS;
    const reg32_t ICR;
    reg32_t DMACR;
} ssp_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
