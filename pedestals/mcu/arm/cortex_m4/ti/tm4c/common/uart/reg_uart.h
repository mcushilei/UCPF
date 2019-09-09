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


#ifndef __REG_USART_H__
#define __REG_USART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define UART0_REG               (*(uart_reg_t      *) UART0_BASE    )
#define UART1_REG               (*(uart1_reg_t     *) UART1_BASE    )
#define UART2_REG               (*(uart_reg_t      *) UART2_BASE    )
#define UART3_REG               (*(uart_reg_t      *) UART3_BASE    )

#define USART_RBR0                      0
#define USART_RBR1                      1
#define USART_RBR2                      2
#define USART_RBR3                      3
#define USART_RBR4                      4
#define USART_RBR5                      5
#define USART_RBR6                      6
#define USART_RBR7                      7
#define USART_RBR_MSK                  (   (1 << USART_RBR0) | \
                                            (1 << USART_RBR1) | \
                                            (1 << USART_RBR2) | \
                                            (1 << USART_RBR3) | \
                                            (1 << USART_RBR4) | \
                                            (1 << USART_RBR5) | \
                                            (1 << USART_RBR6) | \
                                            (1 << USART_RBR7)) 
#define USART_RBR_SET(__VAL)            ((__VAL) << USART_RBR0)


#define USART_THR0                      0
#define USART_THR1                      1
#define USART_THR2                      2
#define USART_THR3                      3
#define USART_THR4                      4
#define USART_THR5                      5
#define USART_THR6                      6
#define USART_THR7                      7
#define USART_THR_MSK                  (   (1 << USART_THR0) | \
                                            (1 << USART_THR1) | \
                                            (1 << USART_THR2) | \
                                            (1 << USART_THR3) | \
                                            (1 << USART_THR4) | \
                                            (1 << USART_THR5) | \
                                            (1 << USART_THR6) | \
                                            (1 << USART_THR7))
#define USART_THR_SET(__VAL)            ((__VAL) << USART_THR0)


#define USART_DLLSB0                    0
#define USART_DLLSB1                    1
#define USART_DLLSB2                    2
#define USART_DLLSB3                    3
#define USART_DLLSB4                    4
#define USART_DLLSB5                    5
#define USART_DLLSB6                    6
#define USART_DLLSB7                    7
#define USART_DLLSB_MSK                (   (1 << USART_DLLSB0) | \
                                            (1 << USART_DLLSB1) | \
                                            (1 << USART_DLLSB2) | \
                                            (1 << USART_DLLSB3) | \
                                            (1 << USART_DLLSB4) | \
                                            (1 << USART_DLLSB5) | \
                                            (1 << USART_DLLSB6) | \
                                            (1 << USART_DLLSB7))
#define USART_DLLSB_SET(__VAL)          ((__VAL) << USART_DLLSB0)


#define USART_DLMSB0                    0
#define USART_DLMSB1                    1
#define USART_DLMSB2                    2
#define USART_DLMSB3                    3
#define USART_DLMSB4                    4
#define USART_DLMSB5                    5
#define USART_DLMSB6                    6
#define USART_DLMSB7                    7
#define USART_DLMSB_MSK                (   (1 << USART_DLMSB0) | \
                                            (1 << USART_DLMSB1) | \
                                            (1 << USART_DLMSB2) | \
                                            (1 << USART_DLMSB3) | \
                                            (1 << USART_DLMSB4) | \
                                            (1 << USART_DLMSB5) | \
                                            (1 << USART_DLMSB6) | \
                                            (1 << USART_DLMSB7))
#define USART_DLMSB_SET(__VAL)          ((__VAL) << USART_DLMSB0)


#define USART_IER_RBRIE                 0     
#define USART_IER_RBRIE_MSK            (1 << USART_IER_RBRIE)     
#define USART_IER_RBRIE_SET(__VAL)      ((__VAL) << USART_IER_RBRIE)     

#define USART_IER_THREIE                1 
#define USART_IER_THREIE_MSK           (1 << USART_IER_THREIE)    
#define USART_IER_THREIE_SET(__VAL)     ((__VAL) << USART_IER_THREIE)    

#define USART_IER_RXIE                  2 
#define USART_IER_RXIE_MSK             (1 << USART_IER_RXIE)      
#define USART_IER_RXIE_SET(__VAL)       ((__VAL) << USART_IER_RXIE)   

#define USART_IER_ABEOINTEN             8 
#define USART_IER_ABEOINTEN_MSK        (1 << USART_IER_ABEOINTEN) 
#define USART_IER_ABEOINTEN_SET(__VAL)  ((__VAL) << USART_IER_ABEOINTEN) 

#define USART_IER_ABTOINTEN             9 
#define USART_IER_ABTOINTEN_MSK        (1 << USART_IER_ABTOINTEN) 
#define USART_IER_ABTOINTEN_SET(__VAL)  ((__VAL) << USART_IER_ABTOINTEN) 


#define USART_IIR_INTSTATUS	            0
#define USART_IIR_INTSTATUS_MSK        (1 << USART_IIR_INTSTATUS)
#define USART_IIR_INTSTATUS_SET(VALUE)  ((__VAL) << USART_IIR_INTSTATUS)

#define USART_IIR_INTID0                1
#define USART_IIR_INTID1                2
#define USART_IIR_INTID2                3
#define USART_IIR_INTID_MSK            (   (1 << USART_IIR_INTID0) | \
                                            (1 << USART_IIR_INTID1) | \
                                            (1 << USART_IIR_INTID2))
#define USART_IIR_INTID_SET(__VAL)      ((__VAL) << USART_IIR_INTID0) 

#define USART_IIR_INTID_RLS	            ((3u << USART_IIR_INTID0))
#define USART_IIR_INTID_RDA	            ((2u << USART_IIR_INTID0))
#define USART_IIR_INTID_CTI	            ((6u << USART_IIR_INTID0))
#define USART_IIR_INTID_THRE	        ((1u << USART_IIR_INTID0))

#define USART_IIR_FIFOENABLE0           6
#define USART_IIR_FIFOENABLE1           7
#define USART_IIR_FIFOENABLE_MSK       (   (1 << USART_IIR_FIFOENABLE0) | \
                                            (1 << USART_IIR_FIFOENABLE0))
#define USART_IIR_FIFOENABLE_SET(__VAL) ((__VAL) << USART_IIR_FIFOENABLE0)  

#define USART_IIR_ABEOINT               8
#define USART_IIR_ABEOINT_MSK          (1 << USART_IIR_ABEOINT)     
#define USART_IIR_ABEOINT_SET(__VAL)    ((__VAL) << USART_IIR_ABEOINT)     

#define USART_IIR_ABTOINT               9 
#define USART_IIR_ABTOINT_MSK          (1 << USART_IIR_ABTOINT)     
#define USART_IIR_ABTOINT_SET(__VAL)    ((__VAL) << USART_IIR_ABTOINT)     
          

#define USART_FCR_FIFO_EN               0
#define USART_FCR_FIFO_EN_MSK          (1<<USART_FCR_FIFO_EN)
#define USART_FCR_FIFO_EN_SET(__VAL)    ((__VAL) << USART_FCR_FIFO_EN)

#define USART_FCR_RX_FIFO_RS            1
#define USART_FCR_RX_FIFO_RS_MSK       (1<<USART_FCR_RX_FIFO_RS)
#define USART_FCR_RX_FIFO_RS_SET(__VAL) ((__VAL) << USART_FCR_RX_FIFO_RS)

#define USART_FCR_TX_FIFO_RS            2
#define USART_FCR_TX_FIFO_RS_MSK       (1<<USART_FCR_TX_FIFO_RS)
#define USART_FCR_TX_FIFO_RS_SET(__VAL) ((__VAL) << USART_FCR_TX_FIFO_RS)

#define USART_FCR_DMA_MO                3
#define USART_FCR_DMA_MO_MSK           (1<<USART_FCR_DMA_MO)
#define USART_FCR_DMA_MO_SET(__VAL)     ((__VAL) << USART_FCR_DMA_MO)

#define USART_FCR_RXTL0                 6
#define USART_FCR_RXTL1                 7
#define USART_FCR_RTXL_MSK             (   (1<<USART_FCR_RXTL0) | \
                                            (1<<USART_FCR_RXTL1))
#define USART_FCR_RXTL_SET(__VAL)       ((__VAL) << USART_FCR_RXTL0)


#define USART_LCR_WLS0                  0
#define USART_LCR_WLS1                  1
#define USART_LCR_WLS_MSK              (   (1<<USART_LCR_WLS0)| \
                                            (1<<USART_LCR_WLS1))
#define USART_LCR_WLS_SET(__VAL)        ((__VAL) << USART_LCR_WLS0)                                 

#define USART_LCR_SBS                   2
#define USART_LCR_SBS_MSK              (1<<USART_LCR_SBS)
#define USART_LCR_SBS_SET(__VAL)        ((__VAL) << USART_LCR_SBS)

#define USART_LCR_PE                    3
#define USART_LCR_PE_MSK               (1<<USART_LCR_PE)
#define USART_LCR_PE_SET(__VAL)         ((__VAL) << USART_LCR_PE)

#define USART_LCR_PS0                   4
#define USART_LCR_PS1                   5
#define USART_LCR_PS_MSK               (   (1<<USART_LCR_PS0) | \
                                            (1<<USART_LCR_PS1))
#define USART_LCR_PS_SET(__VAL)         ((__VAL) << USART_LCR_PS0)

#define USART_LCR_BC                    6
#define USART_LCR_BC_MSK               (1<<USART_LCR_BC)
#define USART_LCR_BC_SET(__VAL)         ((__VAL) << USART_LCR_BC)

#define USART_LCR_DLAB                  7
#define USART_LCR_DLAB_MSK             (1<<USART_LCR_DLAB)
#define USART_LCR_DLAB_SET(__VAL)       ((__VAL << USART_LCR_DLAB))


#define USART_MCR_DTRCTR                0
#define USART_MCR_DTRCTR_MSK           (1 << USART_MCR_DTRCTR)
#define USART_MCR_DTRCTR_SET(__VAL)     ((__VAL) << USART_MCR_DTRCTR)

#define USART_MCR_RTSCTR                1
#define USART_MCR_RTSCTR_MSK           (1 << USART_MCR_RTSCTR)
#define USART_MCR_RTSCTR_SET(__VAL)     ((__VAL) << USART_MCR_RTSCTR)

#define USART_MCR_OUT1                  2
#define USART_MCR_OUT1_MSK             (1 << USART_MCR_OUT1)
#define USART_MCR_OUT1_SET(__VAL)       ((__VAL) << USART_MCR_OUT1)

#define USART_MCR_OUT2                  3
#define USART_MCR_OUT2_MSK             (1 << USART_MCR_OUT2)
#define USART_MCR_OUT2_SET(__VAL)       ((__VAL) << USART_MCR_OUT2)

#define USART_MCR_LMS                   4
#define USART_MCR_LMS_MSK              (1<<USART_MCR_LMS)
#define USART_MCR_LMS_SET(__VAL)        ((__VAL) << USART_MCR_LMS)  

#define USART_MCR_RTSEN                 6
#define USART_MCR_RTSEN_MSK            (1<<USART_MCR_RTSEN)
#define USART_MCR_RTSEN_SET(__VAL)      ((__VAL) << USART_MCR_RTSEN)

#define USART_MCR_CTSEN                 7
#define USART_MCR_CTSEN_MSK            (1<<USART_MCR_CTSEN)
#define USART_MCR_CTSEN_SET(__VAL)      ((__VAL) << USART_MCR_CTSEN)


#define USART_LSR_RDR                   0
#define USART_LSR_RDR_MSK              (1<<USART_LSR_RDR)
#define USART_LSR_RDR_SET(__VAL)        ((__VAL) << USART_LSR_RDR)

#define USART_LSR_OE                    1
#define USART_LSR_OE_MSK               (1<<USART_LSR_OE)
#define USART_LSR_OE_SET(__VAL)         ((__VAL) << USART_LSR_OE)

#define USART_LSR_PE                    2
#define USART_LSR_PE_MSK               (1<<USART_LSR_PE)
#define USART_LSR_PE_SET(__VAL)         ((__VAL) << USART_LSR_PE)

#define USART_LSR_FE                    3
#define USART_LSR_FE_MSK               (1<<USART_LSR_FE)
#define USART_LSR_FE_SET(__VAL)         ((__VAL) << USART_LSR_FE)

#define USART_LSR_BI                    4
#define USART_LSR_BI_MSK               (1<<USART_LSR_BI)
#define USART_LSR_BI_SET(__VAL)         ((__VAL) << USART_LSR_BI)

#define USART_LSR_THRE                  5
#define USART_LSR_THRE_MSK             (1<<USART_LSR_THRE)
#define USART_LSR_THRE_SET(__VAL)       ((__VAL) << USART_LSR_THRE)

#define USART_LSR_TEMT                  6
#define USART_LSR_TEMT_MSK             (1<<USART_LSR_TEMT)
#define USART_LSR_TEMT_SET(__VAL)       ((__VAL) << USART_LSR_TEMT)

#define USART_LSR_RXFE                  7
#define USART_LSR_RXFE_MSK             (1<<USART_LSR_RXFE)
#define USART_LSR_RXFE_SET(__VAL)       ((__VAL) << USART_LSR_RXFE)


#define USART_MSR_DCTS                  0
#define USART_MSR_DCTS_MSK             (1<<USART_MSR_DCTS)
#define USART_MSR_DCTS_SET(__VAL)       ((__VAL) << USART_MSR_DCTS)

#define USART_MSR_DDSR                  1
#define USART_MSR_DDSR_MSK             (1<<USART_MSR_DDSR)
#define USART_MSR_DDSR_SET(__VAL)       ((__VAL) << USART_MSR_DDSR)

#define USART_MSR_TERI                  2
#define USART_MSR_TERI_MSK             (1<<USART_MSR_TERI)
#define USART_MSR_TERI_SET(__VAL)       ((__VAL) << USART_MSR_TERI)

#define USART_MSR_DDCD                  3
#define USART_MSR_DDCD_MSK             (1<<USART_MSR_DDCD)
#define USART_MSR_DDCD_SET(__VAL)       ((__VAL) << USART_MSR_DDCD)

#define USART_MSR_CTS                   4
#define USART_MSR_CTS_MSK              (1<<USART_MSR_CTS)
#define USART_MSR_CTS_SET(__VAL)        ((__VAL) << USART_MSR_CTS)

#define USART_MSR_DSR                   5
#define USART_MSR_DSR_MAKS              (1<<USART_MSR_DSR)
#define USART_MSR_DSR_SET(__VAL)        ((__VAL) << USART_MSR_DSR)

#define USART_MSR_RI                    6
#define USART_MSR_RI_MSK               (1<<USART_MSR_RI)
#define USART_MSR_RI_SET(__VAL)         ((__VAL) << USART_MSR_RI)

#define USART_MSR_DCD                   7
#define USART_MSR_DCD_MSK              (1<<USART_MSR_DCD)
#define USART_MSR_DCD_SET(__VAL)        ((__VAL) << USART_MSR_DCD)



#define USART_ACR_AUTO_BAUD_START       0
#define USART_ACR_AUTO_BAUD_START_MSK  (1<<USART_ACR_AUTO_BAUD_START)

#define USART_ACR_MODE                  1
#define USART_ACR_MODE_MSK             (1<<USART_ACR_MODE)

#define USART_ACR_AUTO_RESTART          2
#define USART_ACR_AUTO_RESTART_MSK     (1<<USART_ACR_AUTO_RESTART)

#define USART_ACR_ABEOINTCLR            8
#define USART_ACR_ABEOINTCLR_MSK       (1<<USART_ACR_ABEOINTCLR)

#define USART_ACR_ABTOINTCLR            9
#define USART_ACR_ABTOINTCLR_MSK       (1<<USART_ACR_ABTOINTCLR)


#define USART_IRDAEN                    0 
#define USART_IRDAEN_MSK               (1 << USART_IRDAEN)      
#define USART_IRDAEN_SET(__VAL)         ((__VAL) << USART_IRDAEN)      

#define USART_IRDAINV                   1
#define USART_IRDAINV_MSK              (1 << USART_IRDAINV)      
#define USART_IRDAINV_SET(__VAL)        ((__VAL) << USART_IRDAINV)      

#define USART_FIXPULSEEN                2
#define USART_FIXPULSEEN_MSK           (1 << USART_FIXPULSEEN)      
#define USART_FIXPULSEEN_SET(__VAL)     ((__VAL) << USART_FIXPULSEEN)      

#define USART_PULSEDIV0                 3
#define USART_PULSEDIV1                 4
#define USART_PULSEDIV2                 5
#define USART_PULSEDIV_MSK             (   (1 << USART_PULSEDIV0) | \
                                            (1 << USART_PULSEDIV1) | \
                                            (1 << USART_PULSEDIV2))
#define USART_PULSEDIV_SET(__VAL)       ((__VAL) << USART_PULSEDIV0)



#define USART_FDR_DIVADDVAL0            0
#define USART_FDR_DIVADDVAL1            1
#define USART_FDR_DIVADDVAL2            2
#define USART_FDR_DIVADDVAL3            3
#define USART_FDR_DIVADDVAL_MSK        (   (1<<USART_FDR_DIVADDVAL0) | \
                                            (1<<USART_FDR_DIVADDVAL1) | \
                                            (1<<USART_FDR_DIVADDVAL2) | \
                                            (1<<USART_FDR_DIVADDVAL3))
#define USART_FDR_DIVADDVAL(__VALUE)    ((__VALUE) << USART_FDR_DIVADDVAL0)

#define USART_FDR_MULVAL0               4
#define USART_FDR_MULVAL1               5
#define USART_FDR_MULVAL2               6
#define USART_FDR_MULVAL3               7
#define USART_FDR_MULVAL_MSK           (   (1<<USART_FDR_MULVAL0) | \
                                            (1<<USART_FDR_MULVAL1) | \
                                            (1<<USART_FDR_MULVAL2) | \
                                            (1<<USART_FDR_MULVAL3))
#define USART_FDR_MULVAL(__VALUE)       ((__VALUE) << USART_FDR_MULVAL0)



#define USART_RS485CTRL_NMMEN               0
#define USART_RS485CTRL_NMMEN_MSK          (1<<USART_RS485CTRL_NMMEN)
#define USART_RS485CTRL_NMMEN_SET(__VAL)    ((__VAL) << USART_RS485CTRL_NMMEN) 

#define USART_RS485CTRL_RXDIS               1
#define USART_RS485CTRL_RXDIS_MSK          (1<<USART_RS485CTRL_RXDIS)
#define USART_RS485CTRL_RXDIS_SET(__VAL)    ((__VAL) << USART_RS485CTRL_RXDIS)

#define USART_RS485CTRL_AADEN               2
#define USART_RS485CTRL_AADEN_MSK          (1<<USART_RS485CTRL_AADEN)
#define USART_RS485CTRL_AADEN_SET(__VAL)    ((__VAL) << USART_RS485CTRL_AADEN)

#define USART_RS485CTRL_SEL                 3
#define USART_RS485CTRL_SEL_MSK            (1<<USART_RS485CTRL_SEL)
#define USART_RS485CTRL_SEL_SET(__VAL)      ((__VAL) << USART_RS485CTRL_SEL)

#define USART_RS485CTRL_DCTRL               4
#define USART_RS485CTRL_DCTRL_MSK          (1<<USART_RS485CTRL_DCTRL)
#define USART_RS485CTRL_DCTRL_SET(__VAL)    ((__VAL) << USART_RS485CTRL_DCTRL)

#define USART_RS485CTRL_OINV                5
#define USART_RS485CTRL_OINV_MSK           (1<<USART_RS485CTRL_OINV)
#define USART_RS485CTRL_OINV_SET(__VAL)     ((__VAL) << USART_RS485CTRL_OINV)


#define USART_RS485ADRMATCH0           0
#define USART_RS485ADRMATCH1           1
#define USART_RS485ADRMATCH2           2
#define USART_RS485ADRMATCH3           3
#define USART_RS485ADRMATCH4           4
#define USART_RS485ADRMATCH5           5
#define USART_RS485ADRMATCH6           6
#define USART_RS485ADRMATCH7           7
#define USART_RS485ADRMATCH_MSK       ((1<<USART_RS485ADRMATCH0) | \
                                        (1<<USART_RS485ADRMATCH1)  | \
                                        (1<<USART_RS485ADRMATCH2)  | \
                                        (1<<USART_RS485ADRMATCH3)  | \
                                        (1<<USART_RS485ADRMATCH4)  | \
                                        (1<<USART_RS485ADRMATCH5)  | \
                                        (1<<USART_RS485ADRMATCH6)  | \
                                        (1<<USART_RS485ADRMATCH7))

#define USART_RE485ADRMATCH(__VALUE)   ((__VALUE) << USART_RS485ADRMATCH0)


#define USART_RS485_DLY0                0
#define USART_RS485_DLY1                1
#define USART_RS485_DLY2                2
#define USART_RS485_DLY3                3
#define USART_RS485_DLY4                4
#define USART_RS485_DLY5                5
#define USART_RS485_DLY6                6
#define USART_RS485_DLY7                7
#define USART_RS485_DLY_MSK            ((1<<USART_RS485_DLY0) | \
                                        (1<<USART_RS485_DLY1)  | \
                                        (1<<USART_RS485_DLY2)  | \
                                        (1<<USART_RS485_DLY3)  | \
                                        (1<<USART_RS485_DLY4)  | \
                                        (1<<USART_RS485_DLY5)  | \
                                        (1<<USART_RS485_DLY6)  | \
                                        (1<<USART_RS485_DLY7))
#define USART_RS485_DLY_VALUE(__VALUE)  ((__VALUE) << USART_RS485_DLY0)



//! USART TER TXEN 
#define USART_TER_TXEN                      7
#define USART_TER_TXEN_MSK                 (1<<USART_TER_TXEN)
#define USART_TER_TXEN_SET(__VAL)           ((__VAL) << USART_TER_TXEN)


#define USART_FIFOLVL_RX0               0
#define USART_FIFOLVL_RX_MSK           (0x0f << USART_FIFOLVL_RX0)
#define USART_FIFOLVL_TX0               8
#define USART_FIFOLVL_TX_MSK           (0x0f << USART_FIFOLVL_TX0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @brief  Universal Asynchronous Receiver Transmitter (UART) register structure definition */
typedef struct
{
    union {
        reg8_t  RBR;
        reg8_t  THR;
        reg8_t  DLL;
        reg32_t RESERVED0;
    };
    union {
        reg8_t  DLM;
        reg32_t IER;
    };
    union {
        reg32_t IIR;
        reg8_t  FCR;
    };
    reg8_t  LCR;
    REG8_RSVD(7)
    reg8_t  LSR;
    REG8_RSVD(7)
    reg8_t  SCR;
    REG8_RSVD(3)
    reg32_t ACR;
    reg8_t  ICR;
    REG8_RSVD(3)
    reg8_t  FDR;
    REG8_RSVD(7)
    reg8_t  TER;
    REG8_RSVD(39)
    reg32_t  FIFOLVL;
} uart_reg_t;

/** @brief  Universal Asynchronous Receiver Transmitter 1 (UART1) register structure definition */
typedef struct
{
    union {
        reg8_t  RBR;
        reg8_t  THR;
        reg8_t  DLL;
        reg32_t RESERVED0;
    };
    union {
        reg8_t  DLM;
        reg32_t IER;
    };
    union {
        reg32_t IIR;
        reg8_t  FCR;
    };
    reg8_t  LCR;
    REG8_RSVD(3)
    reg8_t  MCR;
    REG8_RSVD(3)
    reg8_t  LSR;
    REG8_RSVD(3)
    reg8_t  MSR;
    REG8_RSVD(3)
    reg8_t  SCR;
    REG8_RSVD(3)
    reg32_t ACR;
    REG8_RSVD(1)
    reg32_t FDR;
    REG8_RSVD(1)
    reg8_t  TER;
    REG8_RSVD(27)
    reg8_t  RS485CTRL;
    REG8_RSVD(3)
    reg8_t  ADRMATCH;
    REG8_RSVD(3)
    reg8_t  RS485DLY;
    REG8_RSVD(3)
    reg32_t  FIFOLVL;
} uart1_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
