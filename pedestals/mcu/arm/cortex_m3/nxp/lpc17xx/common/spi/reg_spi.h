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

#ifndef __DRIVER_REG_SPI_H__
#define __DRIVER_REG_SPI_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SPI0_REG                        (*((ssp_reg_t *)SPI0_BASE_ADDRESS))
#define SPI1_REG                        (*((ssp_reg_t *)SPI1_BASE_ADDRESS))
#define SPI_REG                         (*((spi_reg_t *)SPI_BASE_ADDRESS))

/*============================ MACROFIED FUNCTIONS ===========================*/
/* spi datasize*/
#define     SPI_CR0_DSS0                        0
#define     SPI_CR0_DSS1                        1
#define     SPI_CR0_DSS2                        2
#define     SPI_CR0_DSS3                        3
#define     SPI_CR0_DSS_MSK                     (   (1<<SPI_CR0_DSS0)   |\
                                                    (1<<SPI_CR0_DSS1)   |\
                                                    (1<<SPI_CR0_DSS2)   |\
                                                    (1<<SPI_CR0_DSS3))

#define     SPI_DATASIZE_4                      0x3
#define     SPI_DATASIZE_5                      0x4
#define     SPI_DATASIZE_6                      0x5
#define     SPI_DATASIZE_7                      0x6
#define     SPI_DATASIZE_8                      0x7
#define     SPI_DATASIZE_9                      0x8
#define     SPI_DATASIZE_10                     0x9
#define     SPI_DATASIZE_11                     0xA
#define     SPI_DATASIZE_12                     0xB
#define     SPI_DATASIZE_13                     0xC
#define     SPI_DATASIZE_14                     0xD
#define     SPI_DATASIZE_15                     0xE
#define     SPI_DATASIZE_16                     0xF


/* spi format*/
#define     SPI_CR0_FRF0                        4
#define     SPI_CR0_FRF1                        5
#define     SPI_CR0_FRF_MSK                     (   (1<<SPI_CR0_FRF0)  |\
                                                    (1<<SPI_CR0_FRF1))

#define     SPI_FORMAT_SPI                      0x0
#define     SPI_FORMAT_TI                       0x1
#define     SPI_FORMAT_MICROWIRE                0x2


/* spi clk polarity*/
#define     SPI_CR0_CPOL                        6
#define     SPI_CR0_CPOL_MSK                    (1<<SPI_CR0_CPOL)

#define     SPI_CLK_IDLE_LOW                    0x0
#define     SPI_CLK_IDLE_HIGH                   0x1


/* spi sample*/
#define     SPI_CR0_CPHA                        7
#define     SPI_CR0_CPHA_MSK                    (1<<SPI_CR0_CPHA)

#define     SPI_SAMP_FIRST_EDGE                 0x0
#define     SPI_SAMP_SECOND_EDGE                0x1


/* spi serial clock rate*/
#define     SPI_CR0_SCR0                        8
#define     SPI_CR0_SCR1                        9
#define     SPI_CR0_SCR2                        10
#define     SPI_CR0_SCR3                        11
#define     SPI_CR0_SCR4                        12
#define     SPI_CR0_SCR5                        13
#define     SPI_CR0_SCR6                        14
#define     SPI_CR0_SCR7                        15
#define     SPI_CR0_SCR_MSK                     (   (1<<SPI_CR0_SCR0)      |\
                                                    (1<<SPI_CR0_SCR1)      |\
                                                    (1<<SPI_CR0_SCR2)      |\
                                                    (1<<SPI_CR0_SCR3)      |\
                                                    (1<<SPI_CR0_SCR4)      |\
                                                    (1<<SPI_CR0_SCR5)      |\
                                                    (1<<SPI_CR0_SCR6)      |\
                                                    (1<<SPI_CR0_SCR7))



/* spi loop back mode*/
#define     SPI_CR1_LBM                         0
#define     SPI_CR1_LBM_MSK                     (1<<SPI_CR1_LBM)

#define     SPI_NOT_LOOP_BACK                   0x0
#define     SPI_LOOP_BACK                       0x1


/* spi enable*/
#define     SPI_CR1_SSE                         1
#define     SPI_CR1_SSE_MSK                     (1<<SPI_CR1_SSE)

#define     SPI_DISABLE                         0x0
#define     SPI_ENABLE                          0x1


/* spi master or slave mode*/
#define     SPI_CR1_MS                          2
#define     SPI_CR1_MS_MSK                      (1<<SPI_CR1_MS)

#define     SPI_MASTER                          0x0
#define     SPI_SLAVE                           0x1


/* spi slave out disable*/
#define     SPI_CR1_SOD                         3
#define     SPI_CR1_SOD_MSK                     (1<<SPI_CR1_SOD)

#define     SPI_OUT_ENABLE                      0x0
#define     SPI_OUT_DISABLE                     0x1

/* spi transmit fifo empty*/
#define     SPI_SR_TFE                          0
#define     SPI_SR_TFE_MSK                      (1<<SPI_SR_TFE)

/* spi transmit fifo not full*/
#define     SPI_SR_TNF                          1
#define     SPI_SR_TNF_MSK                      (1<<SPI_SR_TNF)

/* spi receive fifo not empty*/
#define     SPI_SR_RNE                          2
#define     SPI_SR_RNE_MSK                      (1<<SPI_SR_RNE)

/* spi receive fifo full*/
#define     SPI_SR_RFF                          3
#define     SPI_SR_RFF_MSK                      (1<<SPI_SR_RFF)

/* spi is busy*/
#define     SPI_SR_BUSY                         4
#define     SPI_SR_BSY_MSK                      (1<<SPI_SR_BUSY)


/* spi clock prescale*/
#define     SPI_CPSR_CPSDVSR0                   0
#define     SPI_CPSR_CPSDVSR1                   1
#define     SPI_CPSR_CPSDVSR2                   2
#define     SPI_CPSR_CPSDVSR3                   3
#define     SPI_CPSR_CPSDVSR4                   4
#define     SPI_CPSR_CPSDVSR5                   5
#define     SPI_CPSR_CPSDVSR6                   6
#define     SPI_CPSR_CPSDVSR7                   7
#define     SPI_CPSR_CPSDVSR_MSK                (   (1<<SPI_CPSR_CPSDVSR0) |\
                                                    (1<<SPI_CPSR_CPSDVSR1) |\
                                                    (1<<SPI_CPSR_CPSDVSR2) |\
                                                    (1<<SPI_CPSR_CPSDVSR3) |\
                                                    (1<<SPI_CPSR_CPSDVSR4) |\
                                                    (1<<SPI_CPSR_CPSDVSR5) |\
                                                    (1<<SPI_CPSR_CPSDVSR6) |\
                                                    (1<<SPI_CPSR_CPSDVSR7))


#define     SPI_CPSR_SET(__VALUE)               ((__VALUE) << SPI_CPSR_CPSDVSR0)
#define     SPI_SLAVE_OUT_SET(__MODE)           ((__MODE) << SPI_CR1_SOD)
#define     SPI_DATASIZE_SET(__SIZE)            ((__SIZE) << SPI_CR0_DSS0)
#define     SPI_FORMAT_SET(__MODE)              ((__MODE) << SPI_CR0_FRF0)
#define     SPI_CLK_POLARITY_SET(__MODE)        ((__MODE) << SPI_CR0_CPOL)
#define     SPI_CLK_SAMP_SET(__MODE)            ((__MODE) << SPI_CR0_CPHA)
#define     SPI_SCR_SET(__VALUE)                ((__VALUE) << SPI_CR0_SCR0)
#define     SPI_LOOP_BACK_SET(__MODE)           ((__MODE) << SPI_CR1_LBM)
#define     SPI_ENABLE_SET(__MODE)              ((__MODE) << SPI_CR1_SSE)
#define     SPI_MODE_SET(__MODE)                ((__MODE) << SPI_CR1_MS)

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
    reg32_t ICR;
    reg32_t DMACR;
} ssp_reg_t;

#define SPI_SPCR_BIT_LEN_EN_BIT0    2
#define SPI_SPCR_BIT_LEN_EN_MSK     (1u << SPI_SPCR_ENABLE_BIT0)
#define SPI_SPCR_CPHA_BIT0          3
#define SPI_SPCR_CPHA_MSK           (1u << SPI_SPCR_CPHA_BIT0)
#define SPI_SPCR_CPOL_BIT0          4
#define SPI_SPCR_CPOL_MSK           (1u << SPI_SPCR_CPOL_BIT0)
#define SPI_SPCR_MSTR_BIT0          5
#define SPI_SPCR_MSTR_MSK           (1u << SPI_SPCR_MSTR_BIT0)
#define SPI_SPCR_LSBF_BIT0          6
#define SPI_SPCR_LSBF_MSK           (1u << SPI_SPCR_LSBF_BIT0)
#define SPI_SPCR_SPIE_BIT0          7
#define SPI_SPCR_SPIE_MSK           (1u << SPI_SPCR_SPIE_BIT0)
#define SPI_SPCR_BITS_BIT0          8
#define SPI_SPCR_BITS_BIT1          9
#define SPI_SPCR_BITS_BIT2          10
#define SPI_SPCR_BITS_BIT3          11
#define SPI_SPCR_BITS_MSK           ((1u << SPI_SPCR_BITS_BIT0)\
                                    |(1u << SPI_SPCR_BITS_BIT1)\
                                    |(1u << SPI_SPCR_BITS_BIT2)\
                                    |(1u << SPI_SPCR_BITS_BIT3))

#define SPI_SPSR_ABRT_BIT0          3
#define SPI_SPSR_ABRT_MSK           (1u << SPI_SPSR_ABRT_BIT0)
#define SPI_SPSR_MODF_BIT0          4
#define SPI_SPSR_MODF_MSK           (1u << SPI_SPSR_MODF_BIT0)
#define SPI_SPSR_ROVR_BIT0          5
#define SPI_SPSR_ROVR_MSK           (1u << SPI_SPSR_ROVR_BIT0)
#define SPI_SPSR_WCOL_BIT0          6
#define SPI_SPSR_WCOL_MKS           (1u << SPI_SPSR_WCOL_BIT0)
#define SPI_SPSR_SPIF_BIT0          7
#define SPI_SPSR_SPIF_MSK           (1u << SPI_SPSR_SPIF_BIT0)

#define SPI_SPINT_SPIF_BIT0         0
#define SPI_SPINT_SPIF_MSK          (1u << SPI_SPINT_SPIF_BIT0)

typedef volatile struct {
    reg32_t SPCR;
    reg32_t SPSR;
    reg32_t SPDR;
    reg32_t SPCCR;
    REG32_RSVD(3)
    reg32_t SPINT;
} spi_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
