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


#ifndef __REG_SCI_H__
#define __REG_SCI_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#ifdef  SCIA_BASE_ADDRESS
#define SCIA_REG       (*(sci_reg_t *)   SCIA_BASE_ADDRESS)
#endif

#ifdef  SCIB_BASE_ADDRESS
#define SCIB_REG       (*(sci_reg_t *)   SCIB_BASE_ADDRESS)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    // Communications control register
    DEF_REG16
        reg16_t SCICHAR         :3; // 2:0    Character length control
        reg16_t ADDRIDLE_MODE   :1; // 3      ADDR/IDLE Mode control
        reg16_t LOOPBKENA       :1; // 4      Loop Back enable
        reg16_t PARITYENA       :1; // 5      Parity enable
        reg16_t PARITY          :1; // 6      Even or Odd Parity
        reg16_t STOPBITS        :1; // 7      Number of Stop Bits
        reg16_t                 :8; // 15:8   reserved
    END_DEF_REG16(SCICCR)

#define SCICCR_SCICHAR_BIT0         0
#define SCICCR_SCICHAR_BIT1         1
#define SCICCR_SCICHAR_BIT2         2
#define SCICCR_SCICHAR_MSK          ( (1u << SCICCR_SCICHAR_BIT0)   \
                                    | (1u << SCICCR_SCICHAR_BIT1)   \
                                    | (1u << SCICCR_SCICHAR_BIT2) )
#define SCICCR_ADDRIDLE_MODE_BIT0   3
#define SCICCR_ADDRIDLE_MODE_MSK    (1u << SCICCR_ADDRIDLE_MODE_BIT0)
#define SCICCR_LOOPBKENA_BIT0       4
#define SCICCR_LOOPBKENA_MSK        (1u << SCICCR_LOOPBKENA_BIT0)
#define SCICCR_PARITYENA_BIT0       5
#define SCICCR_PARITYENA_MSK        (1u << SCICCR_PARITYENA_BIT0)
#define SCICCR_PARITY_BIT0          6
#define SCICCR_PARITY_MSK           (1u << SCICCR_PARITY_BIT0)
#define SCICCR_STOPBITS_BIT0        7
#define SCICCR_STOPBITS_MSK         (1u << SCICCR_STOPBITS_BIT0)

    // Control register 1
    DEF_REG16
        reg16_t RXENA       :1; // 0      SCI receiver enable
        reg16_t TXENA       :1; // 1      SCI transmitter enable
        reg16_t SLEEP       :1; // 2      SCI sleep
        reg16_t TXWAKE      :1; // 3      Transmitter wakeup method
        reg16_t             :1; // 4      reserved
        reg16_t SWRESET     :1; // 5      Software reset
        reg16_t RXERRINTENA :1; // 6      Recieve interrupt enable
        reg16_t             :9; // 15:7   reserved
    END_DEF_REG16(SCICTL1)

#define SCICTL1_RXENA_BIT0          0
#define SCICTL1_RXENA_MSK           (1u << SCICTL1_RXENA_BIT0)
#define SCICTL1_TXENA_BIT0          1
#define SCICTL1_TXENA_MSK           (1u << SCICTL1_TXENA_BIT0)
#define SCICTL1_SLEEP_BIT0          2
#define SCICTL1_SLEEP_MSK           (1u << SCICTL1_SLEEP_BIT0)
#define SCICTL1_TXWAKE_BIT0         3
#define SCICTL1_TXWAKE_MSK          (1u << SCICTL1_SWRESET_BIT0)
#define SCICTL1_SWRESET_BIT0        5
#define SCICTL1_SWRESET_MSK         (1u << SCICTL1_SWRESET_BIT0)
#define SCICTL1_RXERRINTENA_BIT0    6
#define SCICTL1_RXERRINTENA_MSK     (1u << SCICTL1_RXERRINTENA_BIT0)

    reg16_t     SCIHBAUD;   // Baud rate register (high)
    reg16_t     SCILBAUD;   // Baud rate register (low)

    // Control register 2
    DEF_REG16
        reg16_t TXINTENA    :1; // 0      Transmit interrupt enable
        reg16_t RXBKINTENA  :1; // 1      Receiver-buffer break enable
        reg16_t             :4; // 5:2    reserved
        reg16_t TXEMPTY     :1; // 6      Transmitter empty flag
        reg16_t TXRDY       :1; // 7      Transmitter ready flag
        reg16_t             :8; // 15:8   reserved
    END_DEF_REG16(SCICTL2)

#define SCICTL2_TXINTENA_BIT0       0
#define SCICTL2_TXINTENA_MSK        (1u << SCICTL2_TXINTENA_BIT0)
#define SCICTL2_RXBKINTENA_BIT0     1
#define SCICTL2_RXBKINTENA_MSK      (1u << SCICTL2_RXBKINTENA_BIT0)
#define SCICTL2_TXEMPTY_BIT0        6
#define SCICTL2_TXEMPTY_MSK         (1u << SCICTL2_TXEMPTY_BIT0)
#define SCICTL2_TXRDY_BIT0          7
#define SCICTL2_TXRDY_MSK           (1u << SCICTL2_TXRDY_BIT0)

    // Recieve status register
    DEF_REG16
        reg16_t             :1; // 0      reserved
        reg16_t RXWAKE      :1; // 1      Receiver wakeup detect flag
        reg16_t PE          :1; // 2      Parity error flag
        reg16_t OE          :1; // 3      Overrun error flag
        reg16_t FE          :1; // 4      Framing error flag
        reg16_t BRKDT       :1; // 5      Break-detect flag
        reg16_t RXRDY       :1; // 6      Receiver ready flag
        reg16_t RXERROR     :1; // 7      Receiver error flag
    END_DEF_REG16(SCIRXST)

#define SCIRXST_RXWAKE_BIT0         1
#define SCIRXST_RXWAKE_MSK          (1u << SCIRXST_RXWAKE_BIT0)
#define SCIRXST_PE_BIT0             2
#define SCIRXST_PE_MSK              (1u << SCIRXST_PE_BIT0)
#define SCIRXST_OE_BIT0             3
#define SCIRXST_OE_MSK              (1u << SCIRXST_OE_BIT0)
#define SCIRXST_FE_BIT0             4
#define SCIRXST_FE_MSK              (1u << SCIRXST_FE_BIT0)
#define SCIRXST_BRKDT_BIT0          5
#define SCIRXST_BRKDT_MSK           (1u << SCIRXST_BRKDT_BIT0)
#define SCIRXST_RXRDY_BIT0          6
#define SCIRXST_RXRDY_MSK           (1u << SCIRXST_RXRDY_BIT0)
#define SCIRXST_RXERROR_BIT0        7
#define SCIRXST_RXERROR_MSK         (1u << SCIRXST_RXERROR_BIT0)

    reg16_t     SCIRXEMU;   // Recieve emulation buffer register

    // Recieve data buffer
    DEF_REG16
        reg16_t RXDT        :8; // 7:0    Receive word
        reg16_t             :6; // 13:8   reserved
        reg16_t SCIFFPE     :1; // 14     SCI PE error in FIFO mode
        reg16_t SCIFFFE     :1; // 15     SCI FE error in FIFO mode
    END_DEF_REG16(SCIRXBUF)

#define SCIRXBUF_RXDT_BIT0          0
#define SCIRXBUF_RXDT_BIT1          1
#define SCIRXBUF_RXDT_BIT2          2
#define SCIRXBUF_RXDT_BIT3          3
#define SCIRXBUF_RXDT_BIT4          4
#define SCIRXBUF_RXDT_BIT5          5
#define SCIRXBUF_RXDT_BIT6          6
#define SCIRXBUF_RXDT_BIT7          7
#define SCIRXBUF_RXDT_MSK           ( (1u << SCIRXBUF_RXDT_BIT0)    \
                                    | (1u << SCIRXBUF_RXDT_BIT1)    \
                                    | (1u << SCIRXBUF_RXDT_BIT2)    \
                                    | (1u << SCIRXBUF_RXDT_BIT3)    \
                                    | (1u << SCIRXBUF_RXDT_BIT4)    \
                                    | (1u << SCIRXBUF_RXDT_BIT5)    \
                                    | (1u << SCIRXBUF_RXDT_BIT6)    \
                                    | (1u << SCIRXBUF_RXDT_BIT7) )
#define SCIRXBUF_SCIFFPE_BIT0       14
#define SCIRXBUF_SCIFFPE_MSK        (1u << SCIRXBUF_SCIFFPE_BIT0)
#define SCIRXBUF_SCIFFFE_BIT0       15
#define SCIRXBUF_SCIFFFE_MSK        (1u << SCIRXBUF_SCIFFFE_BIT0)

    REG16_RSVD(1)      // reserved

    reg16_t     SCITXBUF;   // Transmit data buffer

    // FIFO transmit register
    DEF_REG16
        reg16_t TXFFILIL    :5; // 4:0    Interrupt level
        reg16_t TXFFIENA    :1; // 5      Interrupt enable
        reg16_t TXINTCLR    :1; // 6      Clear INT flag
        reg16_t TXFFINT     :1; // 7      INT flag
        reg16_t TXFFST      :5; // 12:8   FIFO status
        reg16_t TXFIFOXRESET:1; // 13     FIFO reset
        reg16_t SCIFFENA    :1; // 14     Enhancement enable
        reg16_t SCIRST      :1; // 15     SCI reset rx/tx channels
    END_DEF_REG16(SCIFFTX)

#define SCIFFTX_TXFFILIL_BIT0       0
#define SCIFFTX_TXFFILIL_BIT1       1
#define SCIFFTX_TXFFILIL_BIT2       2
#define SCIFFTX_TXFFILIL_BIT3       3
#define SCIFFTX_TXFFILIL_BIT4       4
#define SCIFFTX_TXFFILIL_MSK        ( (1u << SCIFFTX_TXFFILIL_BIT0) \
                                    | (1u << SCIFFTX_TXFFILIL_BIT1) \
                                    | (1u << SCIFFTX_TXFFILIL_BIT2) \
                                    | (1u << SCIFFTX_TXFFILIL_BIT3) \
                                    | (1u << SCIFFTX_TXFFILIL_BIT4) )
#define SCIFFTX_TXFFIENA_BIT0       5
#define SCIFFTX_TXFFIENA_MSK        (1u << SCIFFTX_TXFFIENA_BIT0)
#define SCIFFTX_TXINTCLR_BIT0       6
#define SCIFFTX_TXINTCLR_MSK        (1u << SCIFFTX_TXINTCLR_BIT0)
#define SCIFFTX_TXFFINT_BIT0        7
#define SCIFFTX_TXFFINT_MSK         (1u << SCIFFTX_TXFFINT_BIT0)
#define SCIFFTX_TXFFST_BIT0         8
#define SCIFFTX_TXFFST_BIT1         9
#define SCIFFTX_TXFFST_BIT2         10
#define SCIFFTX_TXFFST_BIT3         11
#define SCIFFTX_TXFFST_BIT4         12
#define SCIFFTX_TXFFST_MSK          ( (1u << SCIFFTX_TXFFST_BIT0)   \
                                    | (1u << SCIFFTX_TXFFST_BIT1)   \
                                    | (1u << SCIFFTX_TXFFST_BIT2)   \
                                    | (1u << SCIFFTX_TXFFST_BIT3)   \
                                    | (1u << SCIFFTX_TXFFST_BIT4) )
#define SCIFFTX_TXFIFOXRESET_BIT0   13
#define SCIFFTX_TXFIFOXRESET_MSK    (1u << SCIFFTX_TXFIFOXRESET_BIT0)
#define SCIFFTX_SCIFFENA_BIT0       14
#define SCIFFTX_SCIFFENA_MSK        (1u << SCIFFTX_SCIFFENA_BIT0)
#define SCIFFTX_SCIRST_BIT0         15
#define SCIFFTX_SCIRST_MSK          (1u << SCIFFTX_SCIRST_BIT0)

        // FIFO recieve register
    DEF_REG16
        reg16_t RXFFIL      :5; // 4:0    Interrupt level
        reg16_t RXFFIENA    :1; // 5      Interrupt enable
        reg16_t RXFFINTCLR  :1; // 6      Clear INT flag
        reg16_t RXFFINT     :1; // 7      INT flag
        reg16_t RXFIFST     :5; // 12:8   FIFO status
        reg16_t RXFIFORESET :1; // 13     FIFO reset
        reg16_t RXFFOVRCLR  :1; // 14     Clear overflow
        reg16_t RXFFOVF     :1; // 15     FIFO overflow
    END_DEF_REG16(SCIFFRX)

#define SCIFFRX_RXFFIL_BIT0         0
#define SCIFFRX_RXFFIL_BIT1         1
#define SCIFFRX_RXFFIL_BIT2         2
#define SCIFFRX_RXFFIL_BIT3         3
#define SCIFFRX_RXFFIL_BIT4         4
#define SCIFFRX_RXFFIL_MSK          ( (1u << SCIFFRX_RXFFIL_BIT0)   \
                                    | (1u << SCIFFRX_RXFFIL_BIT1)   \
                                    | (1u << SCIFFRX_RXFFIL_BIT2)   \
                                    | (1u << SCIFFRX_RXFFIL_BIT3)   \
                                    | (1u << SCIFFRX_RXFFIL_BIT4) )
#define SCIFFRX_RXFFIENA_BIT0       5
#define SCIFFRX_RXFFIENA_MSK        (1u << SCIFFRX_RXFFIENA_BIT0)
#define SCIFFRX_RXFFINTCLR_BIT0     6
#define SCIFFRX_RXFFINTCLR_MSK      (1u << SCIFFRX_RXFFINTCLR_BIT0)
#define SCIFFRX_RXFFINT_BIT0        7
#define SCIFFRX_RXFFINT_MSK         (1u << SCIFFRX_RXFFINT_BIT0)
#define SCIFFRX_RXFIFST_BIT0        8
#define SCIFFRX_RXFIFST_BIT1        9
#define SCIFFRX_RXFIFST_BIT2        10
#define SCIFFRX_RXFIFST_BIT3        11
#define SCIFFRX_RXFIFST_BIT4        12
#define SCIFFRX_RXFIFST_MSK         ( (1u << SCIFFRX_RXFIFST_BIT0)  \
                                    | (1u << SCIFFRX_RXFIFST_BIT1)  \
                                    | (1u << SCIFFRX_RXFIFST_BIT2)  \
                                    | (1u << SCIFFRX_RXFIFST_BIT3)  \
                                    | (1u << SCIFFRX_RXFIFST_BIT4) )
#define SCIFFRX_RXFIFORESET_BIT0    13
#define SCIFFRX_RXFIFORESET_MSK     (1u << SCIFFRX_RXFIFORESET_BIT0)
#define SCIFFRX_RXFFOVRCLR_BIT0     14
#define SCIFFRX_RXFFOVRCLR_MSK      (1u << SCIFFRX_RXFFOVRCLR_BIT0)
#define SCIFFRX_RXFFOVF_BIT0        15
#define SCIFFRX_RXFFOVF_MSK         (1u << SCIFFRX_RXFFOVF_BIT0)

    // FIFO control register
    DEF_REG16
        reg16_t FFTXDLY     :8; // 7:0    FIFO transmit delay
        reg16_t             :5; // 12:8   reserved
        reg16_t CDC         :1; // 13     Auto baud mode enable
        reg16_t ABDCLR      :1; // 14     Auto baud clear
        reg16_t ABD         :1; // 15     Auto baud detect
    END_DEF_REG16(SCIFFCT)

#define SCIFFCT_FFTXDLY_BIT0        0
#define SCIFFCT_FFTXDLY_BIT1        1
#define SCIFFCT_FFTXDLY_BIT2        2
#define SCIFFCT_FFTXDLY_BIT3        3
#define SCIFFCT_FFTXDLY_BIT4        4
#define SCIFFCT_FFTXDLY_BIT5        5
#define SCIFFCT_FFTXDLY_BIT6        6
#define SCIFFCT_FFTXDLY_BIT7        7
#define SCIFFCT_FFTXDLY_MSK         ( (1u << SCIFFCT_FFTXDLY_BIT0)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT1)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT2)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT3)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT4)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT5)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT6)  \
                                    | (1u << SCIFFCT_FFTXDLY_BIT7) )
#define SCIFFCT_CDC_BIT0            13
#define SCIFFCT_CDC_MSK             (1u << SCIFFCT_CDC_BIT0)
#define SCIFFCT_ABDCLR_BIT0         14
#define SCIFFCT_ABDCLR_MSK          (1u << SCIFFCT_ABDCLR_BIT0)
#define SCIFFCT_ABD_BIT0            15
#define SCIFFCT_ABD_MSK             (1u << SCIFFCT_ABD_BIT0)

    REG16_RSVD(2)      // reserved

    // FIFO Priority control
    DEF_REG16
        reg16_t             :3; // 2:0    reserved
        reg16_t FREE        :1; // 3      Free emulation suspend mode
        reg16_t SOFT        :1; // 4      Soft emulation suspend mode
        reg16_t             :3; // 7:5    reserved
    END_DEF_REG16(SCIPRI)

#define SCIPRI_FREE_BIT0            3
#define SCIPRI_FREE_MSK             (1u << SCIPRI_FREE_BIT0)
#define SCIPRI_SOFT_BIT0            4
#define SCIPRI_SOFT_MSK             (1u << SCIPRI_SOFT_BIT0)
} sci_reg_t;


/*============================ PROTOTYPES ====================================*/


#endif
