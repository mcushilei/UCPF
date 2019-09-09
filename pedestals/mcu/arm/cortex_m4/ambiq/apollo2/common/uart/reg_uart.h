/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_UART_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_UART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define REG_UART0           (*(uart_reg_t *)REG_UART0_BASEADDR)
#define REG_UART1           (*(uart_reg_t *)REG_UART1_BASEADDR)

#define REG_UARTn(n)        (*(uart_reg_t *)(REG_UART0_BASEADDR + 0x00001000 * n))

//*****************************************************************************
//
// UART_DR - UART Data Register
//
//*****************************************************************************
// This is the overrun error indicator.
#define REG_UART_DR_OEDATA_S                      11
#define REG_UART_DR_OEDATA_M                      0x00000800
#define REG_UART_DR_OEDATA(n)                     (((uint32_t)(n) << 11) & 0x00000800)
#define REG_UART_DR_OEDATA_NOERR                  0x00000000
#define REG_UART_DR_OEDATA_ERR                    0x00000800

// This is the break error indicator.
#define REG_UART_DR_BEDATA_S                      10
#define REG_UART_DR_BEDATA_M                      0x00000400
#define REG_UART_DR_BEDATA(n)                     (((uint32_t)(n) << 10) & 0x00000400)
#define REG_UART_DR_BEDATA_NOERR                  0x00000000
#define REG_UART_DR_BEDATA_ERR                    0x00000400

// This is the parity error indicator.
#define REG_UART_DR_PEDATA_S                      9
#define REG_UART_DR_PEDATA_M                      0x00000200
#define REG_UART_DR_PEDATA(n)                     (((uint32_t)(n) << 9) & 0x00000200)
#define REG_UART_DR_PEDATA_NOERR                  0x00000000
#define REG_UART_DR_PEDATA_ERR                    0x00000200

// This is the framing error indicator.
#define REG_UART_DR_FEDATA_S                      8
#define REG_UART_DR_FEDATA_M                      0x00000100
#define REG_UART_DR_FEDATA(n)                     (((uint32_t)(n) << 8) & 0x00000100)
#define REG_UART_DR_FEDATA_NOERR                  0x00000000
#define REG_UART_DR_FEDATA_ERR                    0x00000100

// This is the UART data port.
#define REG_UART_DR_DATA_S                        0
#define REG_UART_DR_DATA_M                        0x000000FF
#define REG_UART_DR_DATA(n)                       (((uint32_t)(n) << 0) & 0x000000FF)

//*****************************************************************************
//
// UART_RSR - UART Status Register
//
//*****************************************************************************
// This is the overrun error indicator.
#define REG_UART_RSR_OESTAT_S                     3
#define REG_UART_RSR_OESTAT_M                     0x00000008
#define REG_UART_RSR_OESTAT(n)                    (((uint32_t)(n) << 3) & 0x00000008)
#define REG_UART_RSR_OESTAT_NOERR                 0x00000000
#define REG_UART_RSR_OESTAT_ERR                   0x00000008

// This is the break error indicator.
#define REG_UART_RSR_BESTAT_S                     2
#define REG_UART_RSR_BESTAT_M                     0x00000004
#define REG_UART_RSR_BESTAT(n)                    (((uint32_t)(n) << 2) & 0x00000004)
#define REG_UART_RSR_BESTAT_NOERR                 0x00000000
#define REG_UART_RSR_BESTAT_ERR                   0x00000004

// This is the parity error indicator.
#define REG_UART_RSR_PESTAT_S                     1
#define REG_UART_RSR_PESTAT_M                     0x00000002
#define REG_UART_RSR_PESTAT(n)                    (((uint32_t)(n) << 1) & 0x00000002)
#define REG_UART_RSR_PESTAT_NOERR                 0x00000000
#define REG_UART_RSR_PESTAT_ERR                   0x00000002

// This is the framing error indicator.
#define REG_UART_RSR_FESTAT_S                     0
#define REG_UART_RSR_FESTAT_M                     0x00000001
#define REG_UART_RSR_FESTAT(n)                    (((uint32_t)(n) << 0) & 0x00000001)
#define REG_UART_RSR_FESTAT_NOERR                 0x00000000
#define REG_UART_RSR_FESTAT_ERR                   0x00000001

//*****************************************************************************
//
// UART_FR - Flag Register
//
//*****************************************************************************
// This bit holds the transmit BUSY indicator.
#define REG_UART_FR_TXBUSY_S                      8
#define REG_UART_FR_TXBUSY_M                      0x00000100
#define REG_UART_FR_TXBUSY(n)                     (((uint32_t)(n) << 8) & 0x00000100)

// This bit holds the transmit FIFO empty indicator.
#define REG_UART_FR_TXFE_S                        7
#define REG_UART_FR_TXFE_M                        0x00000080
#define REG_UART_FR_TXFE(n)                       (((uint32_t)(n) << 7) & 0x00000080)
#define REG_UART_FR_TXFE_XMTFIFO_EMPTY            0x00000080

// This bit holds the receive FIFO full indicator.
#define REG_UART_FR_RXFF_S                        6
#define REG_UART_FR_RXFF_M                        0x00000040
#define REG_UART_FR_RXFF(n)                       (((uint32_t)(n) << 6) & 0x00000040)
#define REG_UART_FR_RXFF_RCVFIFO_FULL             0x00000040

// This bit holds the transmit FIFO full indicator.
#define REG_UART_FR_TXFF_S                        5
#define REG_UART_FR_TXFF_M                        0x00000020
#define REG_UART_FR_TXFF(n)                       (((uint32_t)(n) << 5) & 0x00000020)
#define REG_UART_FR_TXFF_XMTFIFO_FULL             0x00000020

// This bit holds the receive FIFO empty indicator.
#define REG_UART_FR_RXFE_S                        4
#define REG_UART_FR_RXFE_M                        0x00000010
#define REG_UART_FR_RXFE(n)                       (((uint32_t)(n) << 4) & 0x00000010)
#define REG_UART_FR_RXFE_RCVFIFO_EMPTY            0x00000010

// This bit holds the busy indicator.
#define REG_UART_FR_BUSY_S                        3
#define REG_UART_FR_BUSY_M                        0x00000008
#define REG_UART_FR_BUSY(n)                       (((uint32_t)(n) << 3) & 0x00000008)
#define REG_UART_FR_BUSY_BUSY                     0x00000008

// This bit holds the data carrier detect indicator.
#define REG_UART_FR_DCD_S                         2
#define REG_UART_FR_DCD_M                         0x00000004
#define REG_UART_FR_DCD(n)                        (((uint32_t)(n) << 2) & 0x00000004)
#define REG_UART_FR_DCD_DETECTED                  0x00000004

// This bit holds the data set ready indicator.
#define REG_UART_FR_DSR_S                         1
#define REG_UART_FR_DSR_M                         0x00000002
#define REG_UART_FR_DSR(n)                        (((uint32_t)(n) << 1) & 0x00000002)
#define REG_UART_FR_DSR_READY                     0x00000002

// This bit holds the clear to send indicator.
#define REG_UART_FR_CTS_S                         0
#define REG_UART_FR_CTS_M                         0x00000001
#define REG_UART_FR_CTS(n)                        (((uint32_t)(n) << 0) & 0x00000001)
#define REG_UART_FR_CTS_CLEARTOSEND               0x00000001

//*****************************************************************************
//
// UART_ILPR - IrDA Counter
//
//*****************************************************************************
// These bits hold the IrDA counter divisor.
#define REG_UART_ILPR_ILPDVSR_S                   0
#define REG_UART_ILPR_ILPDVSR_M                   0x000000FF
#define REG_UART_ILPR_ILPDVSR(n)                  (((uint32_t)(n) << 0) & 0x000000FF)

//*****************************************************************************
//
// UART_IBRD - Integer Baud Rate Divisor
//
//*****************************************************************************
// These bits hold the baud integer divisor.
#define REG_UART_IBRD_DIVINT_S                    0
#define REG_UART_IBRD_DIVINT_M                    0x0000FFFF
#define REG_UART_IBRD_DIVINT(n)                   (((uint32_t)(n) << 0) & 0x0000FFFF)

//*****************************************************************************
//
// UART_FBRD - Fractional Baud Rate Divisor
//
//*****************************************************************************
// These bits hold the baud fractional divisor.
#define REG_UART_FBRD_DIVFRAC_S                   0
#define REG_UART_FBRD_DIVFRAC_M                   0x0000003F
#define REG_UART_FBRD_DIVFRAC(n)                  (((uint32_t)(n) << 0) & 0x0000003F)

//*****************************************************************************
//
// UART_LCRH - Line Control High
//
//*****************************************************************************
// This bit holds the stick parity select.
#define REG_UART_LCRH_SPS_S                       7
#define REG_UART_LCRH_SPS_M                       0x00000080
#define REG_UART_LCRH_SPS(n)                      (((uint32_t)(n) << 7) & 0x00000080)

// These bits hold the write length.
#define REG_UART_LCRH_WLEN_S                      5
#define REG_UART_LCRH_WLEN_M                      0x00000060
#define REG_UART_LCRH_WLEN(n)                     (((uint32_t)(n) << 5) & 0x00000060)

// This bit holds the FIFO enable.
#define REG_UART_LCRH_FEN_S                       4
#define REG_UART_LCRH_FEN_M                       0x00000010
#define REG_UART_LCRH_FEN(n)                      (((uint32_t)(n) << 4) & 0x00000010)

// This bit holds the two stop bits select.
#define REG_UART_LCRH_STP2_S                      3
#define REG_UART_LCRH_STP2_M                      0x00000008
#define REG_UART_LCRH_STP2(n)                     (((uint32_t)(n) << 3) & 0x00000008)

// This bit holds the even parity select.
#define REG_UART_LCRH_EPS_S                       2
#define REG_UART_LCRH_EPS_M                       0x00000004
#define REG_UART_LCRH_EPS(n)                      (((uint32_t)(n) << 2) & 0x00000004)

// This bit holds the parity enable.
#define REG_UART_LCRH_PEN_S                       1
#define REG_UART_LCRH_PEN_M                       0x00000002
#define REG_UART_LCRH_PEN(n)                      (((uint32_t)(n) << 1) & 0x00000002)

// This bit holds the break set.
#define REG_UART_LCRH_BRK_S                       0
#define REG_UART_LCRH_BRK_M                       0x00000001
#define REG_UART_LCRH_BRK(n)                      (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// UART_CR - Control Register
//
//*****************************************************************************
// This bit enables CTS hardware flow control.
#define REG_UART_CR_CTSEN_S                       15
#define REG_UART_CR_CTSEN_M                       0x00008000
#define REG_UART_CR_CTSEN(n)                      (((uint32_t)(n) << 15) & 0x00008000)

// This bit enables RTS hardware flow control.
#define REG_UART_CR_RTSEN_S                       14
#define REG_UART_CR_RTSEN_M                       0x00004000
#define REG_UART_CR_RTSEN(n)                      (((uint32_t)(n) << 14) & 0x00004000)

// This bit holds modem Out2.
#define REG_UART_CR_OUT2_S                        13
#define REG_UART_CR_OUT2_M                        0x00002000
#define REG_UART_CR_OUT2(n)                       (((uint32_t)(n) << 13) & 0x00002000)

// This bit holds modem Out1.
#define REG_UART_CR_OUT1_S                        12
#define REG_UART_CR_OUT1_M                        0x00001000
#define REG_UART_CR_OUT1(n)                       (((uint32_t)(n) << 12) & 0x00001000)

// This bit enables request to send.
#define REG_UART_CR_RTS_S                         11
#define REG_UART_CR_RTS_M                         0x00000800
#define REG_UART_CR_RTS(n)                        (((uint32_t)(n) << 11) & 0x00000800)

// This bit enables data transmit ready.
#define REG_UART_CR_DTR_S                         10
#define REG_UART_CR_DTR_M                         0x00000400
#define REG_UART_CR_DTR(n)                        (((uint32_t)(n) << 10) & 0x00000400)

// This bit is the receive enable.
#define REG_UART_CR_RXE_S                         9
#define REG_UART_CR_RXE_M                         0x00000200
#define REG_UART_CR_RXE(n)                        (((uint32_t)(n) << 9) & 0x00000200)

// This bit is the transmit enable.
#define REG_UART_CR_TXE_S                         8
#define REG_UART_CR_TXE_M                         0x00000100
#define REG_UART_CR_TXE(n)                        (((uint32_t)(n) << 8) & 0x00000100)

// This bit is the loopback enable.
#define REG_UART_CR_LBE_S                         7
#define REG_UART_CR_LBE_M                         0x00000080
#define REG_UART_CR_LBE(n)                        (((uint32_t)(n) << 7) & 0x00000080)

// This bitfield is the UART clock select.
#define REG_UART_CR_CLKSEL_S                      4
#define REG_UART_CR_CLKSEL_M                      0x00000070
#define REG_UART_CR_CLKSEL(n)                     (((uint32_t)(n) << 4) & 0x00000070)
#define REG_UART_CR_CLKSEL_NOCLK                  0x00000000
#define REG_UART_CR_CLKSEL_24MHZ                  0x00000010
#define REG_UART_CR_CLKSEL_12MHZ                  0x00000020
#define REG_UART_CR_CLKSEL_6MHZ                   0x00000030
#define REG_UART_CR_CLKSEL_3MHZ                   0x00000040
#define REG_UART_CR_CLKSEL_RSVD5                  0x00000050
#define REG_UART_CR_CLKSEL_RSVD6                  0x00000060
#define REG_UART_CR_CLKSEL_RSVD7                  0x00000070

// This bit is the UART clock enable.
#define REG_UART_CR_CLKEN_S                       3
#define REG_UART_CR_CLKEN_M                       0x00000008
#define REG_UART_CR_CLKEN(n)                      (((uint32_t)(n) << 3) & 0x00000008)

// This bit is the SIR low power select.
#define REG_UART_CR_SIRLP_S                       2
#define REG_UART_CR_SIRLP_M                       0x00000004
#define REG_UART_CR_SIRLP(n)                      (((uint32_t)(n) << 2) & 0x00000004)

// This bit is the SIR ENDEC enable.
#define REG_UART_CR_SIREN_S                       1
#define REG_UART_CR_SIREN_M                       0x00000002
#define REG_UART_CR_SIREN(n)                      (((uint32_t)(n) << 1) & 0x00000002)

// This bit is the UART enable.
#define REG_UART_CR_UARTEN_S                      0
#define REG_UART_CR_UARTEN_M                      0x00000001
#define REG_UART_CR_UARTEN(n)                     (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// UART_IFLS - FIFO Interrupt Level Select
//
//*****************************************************************************
// These bits hold the receive FIFO interrupt level.
#define REG_UART_IFLS_RXIFLSEL_S                  3
#define REG_UART_IFLS_RXIFLSEL_M                  0x00000038
#define REG_UART_IFLS_RXIFLSEL(n)                 (((uint32_t)(n) << 3) & 0x00000038)

// These bits hold the transmit FIFO interrupt level.
#define REG_UART_IFLS_TXIFLSEL_S                  0
#define REG_UART_IFLS_TXIFLSEL_M                  0x00000007
#define REG_UART_IFLS_TXIFLSEL(n)                 (((uint32_t)(n) << 0) & 0x00000007)

//*****************************************************************************
//
// UART_IER - Interrupt Enable
//
//*****************************************************************************
// This bit holds the overflow interrupt enable.
#define REG_UART_IER_OEIM_S                       10
#define REG_UART_IER_OEIM_M                       0x00000400
#define REG_UART_IER_OEIM(n)                      (((uint32_t)(n) << 10) & 0x00000400)

// This bit holds the break error interrupt enable.
#define REG_UART_IER_BEIM_S                       9
#define REG_UART_IER_BEIM_M                       0x00000200
#define REG_UART_IER_BEIM(n)                      (((uint32_t)(n) << 9) & 0x00000200)

// This bit holds the parity error interrupt enable.
#define REG_UART_IER_PEIM_S                       8
#define REG_UART_IER_PEIM_M                       0x00000100
#define REG_UART_IER_PEIM(n)                      (((uint32_t)(n) << 8) & 0x00000100)

// This bit holds the framing error interrupt enable.
#define REG_UART_IER_FEIM_S                       7
#define REG_UART_IER_FEIM_M                       0x00000080
#define REG_UART_IER_FEIM(n)                      (((uint32_t)(n) << 7) & 0x00000080)

// This bit holds the receive timeout interrupt enable.
#define REG_UART_IER_RTIM_S                       6
#define REG_UART_IER_RTIM_M                       0x00000040
#define REG_UART_IER_RTIM(n)                      (((uint32_t)(n) << 6) & 0x00000040)

// This bit holds the transmit interrupt enable.
#define REG_UART_IER_TXIM_S                       5
#define REG_UART_IER_TXIM_M                       0x00000020
#define REG_UART_IER_TXIM(n)                      (((uint32_t)(n) << 5) & 0x00000020)

// This bit holds the receive interrupt enable.
#define REG_UART_IER_RXIM_S                       4
#define REG_UART_IER_RXIM_M                       0x00000010
#define REG_UART_IER_RXIM(n)                      (((uint32_t)(n) << 4) & 0x00000010)

// This bit holds the modem DSR interrupt enable.
#define REG_UART_IER_DSRMIM_S                     3
#define REG_UART_IER_DSRMIM_M                     0x00000008
#define REG_UART_IER_DSRMIM(n)                    (((uint32_t)(n) << 3) & 0x00000008)

// This bit holds the modem DCD interrupt enable.
#define REG_UART_IER_DCDMIM_S                     2
#define REG_UART_IER_DCDMIM_M                     0x00000004
#define REG_UART_IER_DCDMIM(n)                    (((uint32_t)(n) << 2) & 0x00000004)

// This bit holds the modem CTS interrupt enable.
#define REG_UART_IER_CTSMIM_S                     1
#define REG_UART_IER_CTSMIM_M                     0x00000002
#define REG_UART_IER_CTSMIM(n)                    (((uint32_t)(n) << 1) & 0x00000002)

// This bit holds the modem TXCMP interrupt enable.
#define REG_UART_IER_TXCMPMIM_S                   0
#define REG_UART_IER_TXCMPMIM_M                   0x00000001
#define REG_UART_IER_TXCMPMIM(n)                  (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// UART_IES - Interrupt Status
//
//*****************************************************************************
// This bit holds the overflow interrupt status.
#define REG_UART_IES_OERIS_S                      10
#define REG_UART_IES_OERIS_M                      0x00000400
#define REG_UART_IES_OERIS(n)                     (((uint32_t)(n) << 10) & 0x00000400)

// This bit holds the break error interrupt status.
#define REG_UART_IES_BERIS_S                      9
#define REG_UART_IES_BERIS_M                      0x00000200
#define REG_UART_IES_BERIS(n)                     (((uint32_t)(n) << 9) & 0x00000200)

// This bit holds the parity error interrupt status.
#define REG_UART_IES_PERIS_S                      8
#define REG_UART_IES_PERIS_M                      0x00000100
#define REG_UART_IES_PERIS(n)                     (((uint32_t)(n) << 8) & 0x00000100)

// This bit holds the framing error interrupt status.
#define REG_UART_IES_FERIS_S                      7
#define REG_UART_IES_FERIS_M                      0x00000080
#define REG_UART_IES_FERIS(n)                     (((uint32_t)(n) << 7) & 0x00000080)

// This bit holds the receive timeout interrupt status.
#define REG_UART_IES_RTRIS_S                      6
#define REG_UART_IES_RTRIS_M                      0x00000040
#define REG_UART_IES_RTRIS(n)                     (((uint32_t)(n) << 6) & 0x00000040)

// This bit holds the transmit interrupt status.
#define REG_UART_IES_TXRIS_S                      5
#define REG_UART_IES_TXRIS_M                      0x00000020
#define REG_UART_IES_TXRIS(n)                     (((uint32_t)(n) << 5) & 0x00000020)

// This bit holds the receive interrupt status.
#define REG_UART_IES_RXRIS_S                      4
#define REG_UART_IES_RXRIS_M                      0x00000010
#define REG_UART_IES_RXRIS(n)                     (((uint32_t)(n) << 4) & 0x00000010)

// This bit holds the modem DSR interrupt status.
#define REG_UART_IES_DSRMRIS_S                    3
#define REG_UART_IES_DSRMRIS_M                    0x00000008
#define REG_UART_IES_DSRMRIS(n)                   (((uint32_t)(n) << 3) & 0x00000008)

// This bit holds the modem DCD interrupt status.
#define REG_UART_IES_DCDMRIS_S                    2
#define REG_UART_IES_DCDMRIS_M                    0x00000004
#define REG_UART_IES_DCDMRIS(n)                   (((uint32_t)(n) << 2) & 0x00000004)

// This bit holds the modem CTS interrupt status.
#define REG_UART_IES_CTSMRIS_S                    1
#define REG_UART_IES_CTSMRIS_M                    0x00000002
#define REG_UART_IES_CTSMRIS(n)                   (((uint32_t)(n) << 1) & 0x00000002)

// This bit holds the modem TXCMP interrupt status.
#define REG_UART_IES_TXCMPMRIS_S                  0
#define REG_UART_IES_TXCMPMRIS_M                  0x00000001
#define REG_UART_IES_TXCMPMRIS(n)                 (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// UART_MIS - Masked Interrupt Status
//
//*****************************************************************************
// This bit holds the overflow interrupt status masked.
#define REG_UART_MIS_OEMIS_S                      10
#define REG_UART_MIS_OEMIS_M                      0x00000400
#define REG_UART_MIS_OEMIS(n)                     (((uint32_t)(n) << 10) & 0x00000400)

// This bit holds the break error interrupt status masked.
#define REG_UART_MIS_BEMIS_S                      9
#define REG_UART_MIS_BEMIS_M                      0x00000200
#define REG_UART_MIS_BEMIS(n)                     (((uint32_t)(n) << 9) & 0x00000200)

// This bit holds the parity error interrupt status masked.
#define REG_UART_MIS_PEMIS_S                      8
#define REG_UART_MIS_PEMIS_M                      0x00000100
#define REG_UART_MIS_PEMIS(n)                     (((uint32_t)(n) << 8) & 0x00000100)

// This bit holds the framing error interrupt status masked.
#define REG_UART_MIS_FEMIS_S                      7
#define REG_UART_MIS_FEMIS_M                      0x00000080
#define REG_UART_MIS_FEMIS(n)                     (((uint32_t)(n) << 7) & 0x00000080)

// This bit holds the receive timeout interrupt status masked.
#define REG_UART_MIS_RTMIS_S                      6
#define REG_UART_MIS_RTMIS_M                      0x00000040
#define REG_UART_MIS_RTMIS(n)                     (((uint32_t)(n) << 6) & 0x00000040)

// This bit holds the transmit interrupt status masked.
#define REG_UART_MIS_TXMIS_S                      5
#define REG_UART_MIS_TXMIS_M                      0x00000020
#define REG_UART_MIS_TXMIS(n)                     (((uint32_t)(n) << 5) & 0x00000020)

// This bit holds the receive interrupt status masked.
#define REG_UART_MIS_RXMIS_S                      4
#define REG_UART_MIS_RXMIS_M                      0x00000010
#define REG_UART_MIS_RXMIS(n)                     (((uint32_t)(n) << 4) & 0x00000010)

// This bit holds the modem DSR interrupt status masked.
#define REG_UART_MIS_DSRMMIS_S                    3
#define REG_UART_MIS_DSRMMIS_M                    0x00000008
#define REG_UART_MIS_DSRMMIS(n)                   (((uint32_t)(n) << 3) & 0x00000008)

// This bit holds the modem DCD interrupt status masked.
#define REG_UART_MIS_DCDMMIS_S                    2
#define REG_UART_MIS_DCDMMIS_M                    0x00000004
#define REG_UART_MIS_DCDMMIS(n)                   (((uint32_t)(n) << 2) & 0x00000004)

// This bit holds the modem CTS interrupt status masked.
#define REG_UART_MIS_CTSMMIS_S                    1
#define REG_UART_MIS_CTSMMIS_M                    0x00000002
#define REG_UART_MIS_CTSMMIS(n)                   (((uint32_t)(n) << 1) & 0x00000002)

// This bit holds the modem TXCMP interrupt status masked.
#define REG_UART_MIS_TXCMPMMIS_S                  0
#define REG_UART_MIS_TXCMPMMIS_M                  0x00000001
#define REG_UART_MIS_TXCMPMMIS(n)                 (((uint32_t)(n) << 0) & 0x00000001)

//*****************************************************************************
//
// UART_IEC - Interrupt Clear
//
//*****************************************************************************
// This bit holds the overflow interrupt clear.
#define REG_UART_IEC_OEIC_S                       10
#define REG_UART_IEC_OEIC_M                       0x00000400
#define REG_UART_IEC_OEIC(n)                      (((uint32_t)(n) << 10) & 0x00000400)

// This bit holds the break error interrupt clear.
#define REG_UART_IEC_BEIC_S                       9
#define REG_UART_IEC_BEIC_M                       0x00000200
#define REG_UART_IEC_BEIC(n)                      (((uint32_t)(n) << 9) & 0x00000200)

// This bit holds the parity error interrupt clear.
#define REG_UART_IEC_PEIC_S                       8
#define REG_UART_IEC_PEIC_M                       0x00000100
#define REG_UART_IEC_PEIC(n)                      (((uint32_t)(n) << 8) & 0x00000100)

// This bit holds the framing error interrupt clear.
#define REG_UART_IEC_FEIC_S                       7
#define REG_UART_IEC_FEIC_M                       0x00000080
#define REG_UART_IEC_FEIC(n)                      (((uint32_t)(n) << 7) & 0x00000080)

// This bit holds the receive timeout interrupt clear.
#define REG_UART_IEC_RTIC_S                       6
#define REG_UART_IEC_RTIC_M                       0x00000040
#define REG_UART_IEC_RTIC(n)                      (((uint32_t)(n) << 6) & 0x00000040)

// This bit holds the transmit interrupt clear.
#define REG_UART_IEC_TXIC_S                       5
#define REG_UART_IEC_TXIC_M                       0x00000020
#define REG_UART_IEC_TXIC(n)                      (((uint32_t)(n) << 5) & 0x00000020)

// This bit holds the receive interrupt clear.
#define REG_UART_IEC_RXIC_S                       4
#define REG_UART_IEC_RXIC_M                       0x00000010
#define REG_UART_IEC_RXIC(n)                      (((uint32_t)(n) << 4) & 0x00000010)

// This bit holds the modem DSR interrupt clear.
#define REG_UART_IEC_DSRMIC_S                     3
#define REG_UART_IEC_DSRMIC_M                     0x00000008
#define REG_UART_IEC_DSRMIC(n)                    (((uint32_t)(n) << 3) & 0x00000008)

// This bit holds the modem DCD interrupt clear.
#define REG_UART_IEC_DCDMIC_S                     2
#define REG_UART_IEC_DCDMIC_M                     0x00000004
#define REG_UART_IEC_DCDMIC(n)                    (((uint32_t)(n) << 2) & 0x00000004)

// This bit holds the modem CTS interrupt clear.
#define REG_UART_IEC_CTSMIC_S                     1
#define REG_UART_IEC_CTSMIC_M                     0x00000002
#define REG_UART_IEC_CTSMIC(n)                    (((uint32_t)(n) << 1) & 0x00000002)

// This bit holds the modem TXCMP interrupt clear.
#define REG_UART_IEC_TXCMPMIC_S                   0
#define REG_UART_IEC_TXCMPMIC_M                   0x00000001
#define REG_UART_IEC_TXCMPMIC(n)                  (((uint32_t)(n) << 0) & 0x00000001)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart_reg_t
//! @{
typedef volatile struct {
    reg32_t     DR;
    reg32_t     RSR;
    REG32_RSVD(4)
    reg32_t     FR;
    REG32_RSVD(1)
    reg32_t     ILPR;
    reg32_t     IBRD;
    reg32_t     FBRD;
    reg32_t     LCRH;
    reg32_t     CR;
    reg32_t     IFLS;
    reg32_t     IER;
    reg32_t     IES;
    reg32_t     MIS;
    reg32_t     IEC;
} uart_reg_t;
//! @}


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
