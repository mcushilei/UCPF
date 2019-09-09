#ifndef __I_IO_UART_H__
#define __I_IO_UART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define UART0_REG                      (*(volatile uart_reg_t *)UART0_BASE_ADDRESS)
#define UART1_REG                      (*(volatile uart_reg_t *)UART1_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define UART_RBR0                      0
#define UART_RBR1                      1
#define UART_RBR2                      2
#define UART_RBR3                      3
#define UART_RBR4                      4
#define UART_RBR5                      5
#define UART_RBR6                      6
#define UART_RBR7                      7
#define UART_RBR_MSK                  (   (1 << UART_RBR0) | \
                                            (1 << UART_RBR1) | \
                                            (1 << UART_RBR2) | \
                                            (1 << UART_RBR3) | \
                                            (1 << UART_RBR4) | \
                                            (1 << UART_RBR5) | \
                                            (1 << UART_RBR6) | \
                                            (1 << UART_RBR7)) 
#define UART_RBR_SET(__VAL)            ((__VAL) << UART_RBR0)


#define UART_THR0                      0
#define UART_THR1                      1
#define UART_THR2                      2
#define UART_THR3                      3
#define UART_THR4                      4
#define UART_THR5                      5
#define UART_THR6                      6
#define UART_THR7                      7
#define UART_THR_MSK                  (   (1 << UART_THR0) | \
                                            (1 << UART_THR1) | \
                                            (1 << UART_THR2) | \
                                            (1 << UART_THR3) | \
                                            (1 << UART_THR4) | \
                                            (1 << UART_THR5) | \
                                            (1 << UART_THR6) | \
                                            (1 << UART_THR7))
#define UART_THR_SET(__VAL)            ((__VAL) << UART_THR0)


#define UART_DLLSB0                    0
#define UART_DLLSB1                    1
#define UART_DLLSB2                    2
#define UART_DLLSB3                    3
#define UART_DLLSB4                    4
#define UART_DLLSB5                    5
#define UART_DLLSB6                    6
#define UART_DLLSB7                    7
#define UART_DLLSB_MSK                (   (1 << UART_DLLSB0) | \
                                            (1 << UART_DLLSB1) | \
                                            (1 << UART_DLLSB2) | \
                                            (1 << UART_DLLSB3) | \
                                            (1 << UART_DLLSB4) | \
                                            (1 << UART_DLLSB5) | \
                                            (1 << UART_DLLSB6) | \
                                            (1 << UART_DLLSB7))
#define UART_DLLSB_SET(__VAL)          ((__VAL) << UART_DLLSB0)


#define UART_DLMSB0                    0
#define UART_DLMSB1                    1
#define UART_DLMSB2                    2
#define UART_DLMSB3                    3
#define UART_DLMSB4                    4
#define UART_DLMSB5                    5
#define UART_DLMSB6                    6
#define UART_DLMSB7                    7
#define UART_DLMSB_MSK                (   (1 << UART_DLMSB0) | \
                                            (1 << UART_DLMSB1) | \
                                            (1 << UART_DLMSB2) | \
                                            (1 << UART_DLMSB3) | \
                                            (1 << UART_DLMSB4) | \
                                            (1 << UART_DLMSB5) | \
                                            (1 << UART_DLMSB6) | \
                                            (1 << UART_DLMSB7))
#define UART_DLMSB_SET(__VAL)          ((__VAL) << UART_DLMSB0)


#define UART_IER_RBRIE                 0     
#define UART_IER_RBRIE_MSK            (1 << UART_IER_RBRIE)     
#define UART_IER_RBRIE_SET(__VAL)      ((__VAL) << UART_IER_RBRIE)     

#define UART_IER_THREIE                1 
#define UART_IER_THREIE_MSK           (1 << UART_IER_THREIE)    
#define UART_IER_THREIE_SET(__VAL)     ((__VAL) << UART_IER_THREIE)    

#define UART_IER_RXIE                  2 
#define UART_IER_RXIE_MSK             (1 << UART_IER_RXIE)      
#define UART_IER_RXIE_SET(__VAL)       ((__VAL) << UART_IER_RXIE)   

#define UART_IER_MSINTEN               3 
#define UART_IER_MSINTEN_MSK          (1 << UART_IER_MSINTEN)      
#define UART_IER_MSINTENE_SET(__VAL)   ((__VAL) << UART_IER_MSINTEN) 

#define UART_IER_CTSINTEN              7 
#define UART_IER_CTSINTEN_MSK         (1 << UART_IER_CTSINTEN)      
#define UART_IER_CTSINTEN_SET(__VAL)   ((__VAL) << UART_IER_CTSINTEN) 

#define UART_IER_ABEOINTEN             8 
#define UART_IER_ABEOINTEN_MSK        (1 << UART_IER_ABEOINTEN) 
#define UART_IER_ABEOINTEN_SET(__VAL)  ((__VAL) << UART_IER_ABEOINTEN) 

#define UART_IER_ABTOINTEN             9 
#define UART_IER_ABTOINTEN_MSK        (1 << UART_IER_ABTOINTEN) 
#define UART_IER_ABTOINTEN_SET(__VAL)  ((__VAL) << UART_IER_ABTOINTEN) 

#define UART_IER_WAKEUPINTEN               10 
#define UART_IER_WAKEUPINTEN_MSK          (1 << UART_IER_WAKEUPINTEN) 
#define UART_IER_WAKEUPINTEN_SET(__VAL)    ((__VAL) << UART_IER_WAKEUPINTEN) 


#define UART_IIR_INTSTATUS	            0
#define UART_IIR_INTSTATUS_MSK        (1 << UART_IIR_INTSTATUS)
#define UART_IIR_INTSTATUS_SET(VALUE)  ((__VAL) << UART_IIR_INTSTATUS)

#define UART_IIR_INTID0                1
#define UART_IIR_INTID1                2
#define UART_IIR_INTID2                3
#define UART_IIR_INTID_MSK            (   (1 << UART_IIR_INTID0) | \
                                            (1 << UART_IIR_INTID1) | \
                                            (1 << UART_IIR_INTID2))
#define UART_IIR_INTID_SET(__VAL)      ((__VAL) << UART_IIR_INTID0)       

#define UART_IIR_INTID_RLS	            ((3u << UART_IIR_INTID0))
#define UART_IIR_INTID_RDA	            ((2u << UART_IIR_INTID0))
#define UART_IIR_INTID_CTI	            ((6u << UART_IIR_INTID0))
#define UART_IIR_INTID_THRE	        ((1u << UART_IIR_INTID0))

#define UART_IIR_FIFOENABLE0           6
#define UART_IIR_FIFOENABLE1           7
#define UART_IIR_FIFOENABLE_MSK       (   (1 << UART_IIR_FIFOENABLE0) | \
                                            (1 << UART_IIR_FIFOENABLE0))
#define UART_IIR_FIFOENABLE_SET(__VAL) ((__VAL) << UART_IIR_FIFOENABLE0)  

#define UART_IIR_ABEOINT               8
#define UART_IIR_ABEOINT_MSK          (1 << UART_IIR_ABEOINT)     
#define UART_IIR_ABEOINT_SET(__VAL)    ((__VAL) << UART_IIR_ABEOINT)     

#define UART_IIR_ABTOINT               9 
#define UART_IIR_ABTOINT_MSK          (1 << UART_IIR_ABTOINT)     
#define UART_IIR_ABTOINT_SET(__VAL)    ((__VAL) << UART_IIR_ABTOINT)     

#define UART_IIR_WAKEUPINT               10 
#define UART_IIR_WAKEUPINT_MSK          (1 << UART_IIR_WAKEUPINT)     
#define UART_IIR_WAKEUPINT_SET(__VAL)    ((__VAL) << UART_IIR_WAKEUPINT)    
          

#define UART_FCR_FIFO_EN               0
#define UART_FCR_FIFO_EN_MSK          (1<<UART_FCR_FIFO_EN)
#define UART_FCR_FIFO_EN_SET(__VAL)    ((__VAL) << UART_FCR_FIFO_EN)

#define UART_FCR_RX_FIFO_RS            1
#define UART_FCR_RX_FIFO_RS_MSK       (1<<UART_FCR_RX_FIFO_RS)
#define UART_FCR_RX_FIFO_RS_SET(__VAL) ((__VAL) << UART_FCR_RX_FIFO_RS)

#define UART_FCR_TX_FIFO_RS            2
#define UART_FCR_TX_FIFO_RS_MSK       (1<<UART_FCR_TX_FIFO_RS)
#define UART_FCR_TX_FIFO_RS_SET(__VAL) ((__VAL) << UART_FCR_TX_FIFO_RS)

#define UART_FCR_DMA_MO                3
#define UART_FCR_DMA_MO_MSK           (1<<UART_FCR_DMA_MO)
#define UART_FCR_DMA_MO_SET(__VAL)     ((__VAL) << UART_FCR_DMA_MO)

#define UART_FCR_RXTL0                 6
#define UART_FCR_RXTL1                 7
#define UART_FCR_RTXL_MSK             (   (1<<UART_FCR_RXTL0) | \
                                            (1<<UART_FCR_RXTL1))
#define UART_FCR_RXTL_SET(__VAL)       ((__VAL) << UART_FCR_RXTL0)


#define UART_BIT_LENGTH_5              0x0
#define UART_BIT_LENGTH_6              0x1
#define UART_BIT_LENGTH_7              0x2
#define UART_BIT_LENGTH_8              0x3

#define UART_LCR_WLS0                  0
#define UART_LCR_WLS1                  1
#define UART_LCR_WLS_MSK              (   (1<<UART_LCR_WLS0)| \
                                            (1<<UART_LCR_WLS1))
#define UART_LCR_WLS_SET(__VAL)        ((__VAL) << UART_LCR_WLS0)                                 
#define UART0_BIT_LENGTH(__LENGTH)      ((__LENGTH) << UART_BIT_LENGTH_5)

#define UART_LCR_SBS                   2
#define UART_LCR_SBS_MSK              (1<<UART_LCR_SBS)
#define UART_LCR_SBS_SET(__VAL)        ((__VAL) << UART_LCR_SBS)

#define UART_LCR_PE                    3
#define UART_LCR_PE_MSK               (1<<UART_LCR_PE)
#define UART_LCR_PE_SET(__VAL)         ((__VAL) << UART_LCR_PE)

#define UART_LCR_PS0                   4
#define UART_LCR_PS1                   5
#define UART_LCR_PS_MSK               (   (1<<UART_LCR_PS0) | \
                                            (1<<UART_LCR_PS1))
#define UART_LCR_PS_SET(__VAL)         ((__VAL) << UART_LCR_PS0)

#define UART_LCR_BC                    6
#define UART_LCR_BC_MSK               (1<<UART_LCR_BC)
#define UART_LCR_BC_SET(__VAL)         ((__VAL) << UART_LCR_BC)

#define UART_LCR_DLAB                  7
#define UART_LCR_DLAB_MSK             (1<<UART_LCR_DLAB)
#define UART_LCR_DLAB_SET(__VAL)       ((__VAL << UART_LCR_DLAB))


#define UART_MCR_DTRCTR                0
#define UART_MCR_DTRCTR_MSK           (1 << UART_MCR_DTRCTR)
#define UART_MCR_DTRCTR_SET(__VAL)     ((__VAL) << UART_MCR_DTRCTR)

#define UART_MCR_RTSCTR                1
#define UART_MCR_RTSCTR_MSK           (1 << UART_MCR_RTSCTR)
#define UART_MCR_RTSCTR_SET(__VAL)     ((__VAL) << UART_MCR_RTSCTR)

#define UART_MCR_OUT1                  2
#define UART_MCR_OUT1_MSK             (1 << UART_MCR_OUT1)
#define UART_MCR_OUT1_SET(__VAL)       ((__VAL) << UART_MCR_OUT1)

#define UART_MCR_OUT2                  3
#define UART_MCR_OUT2_MSK             (1 << UART_MCR_OUT2)
#define UART_MCR_OUT2_SET(__VAL)       ((__VAL) << UART_MCR_OUT2)

#define UART_MCR_LMS                   4
#define UART_MCR_LMS_MSK              (1<<UART_MCR_LMS)
#define UART_MCR_LMS_SET(__VAL)        ((__VAL) << UART_MCR_LMS)  

#define UART_MCR_RTSEN                 6
#define UART_MCR_RTSEN_MSK            (1<<UART_MCR_RTSEN)
#define UART_MCR_RTSEN_SET(__VAL)      ((__VAL) << UART_MCR_RTSEN)

#define UART_MCR_CTSEN                 7
#define UART_MCR_CTSEN_MSK            (1<<UART_MCR_CTSEN)
#define UART_MCR_CTSEN_SET(__VAL)      ((__VAL) << UART_MCR_CTSEN)


#define UART_LSR_RDR                   0
#define UART_LSR_RDR_MSK              (1<<UART_LSR_RDR)
#define UART_LSR_RDR_SET(__VAL)        ((__VAL) << UART_LSR_RDR)

#define UART_LSR_OE                    1
#define UART_LSR_OE_MSK               (1<<UART_LSR_OE)
#define UART_LSR_OE_SET(__VAL)         ((__VAL) << UART_LSR_OE)

#define UART_LSR_PE                    2
#define UART_LSR_PE_MSK               (1<<UART_LSR_PE)
#define UART_LSR_PE_SET(__VAL)         ((__VAL) << UART_LSR_PE)

#define UART_LSR_FE                    3
#define UART_LSR_FE_MSK               (1<<UART_LSR_FE)
#define UART_LSR_FE_SET(__VAL)         ((__VAL) << UART_LSR_FE)

#define UART_LSR_BI                    4
#define UART_LSR_BI_MSK               (1<<UART_LSR_BI)
#define UART_LSR_BI_SET(__VAL)         ((__VAL) << UART_LSR_BI)

#define UART_LSR_THRE                  5
#define UART_LSR_THRE_MSK             (1<<UART_LSR_THRE)
#define UART_LSR_THRE_SET(__VAL)       ((__VAL) << UART_LSR_THRE)

#define UART_LSR_TEMT                  6
#define UART_LSR_TEMT_MSK             (1<<UART_LSR_TEMT)
#define UART_LSR_TEMT_SET(__VAL)       ((__VAL) << UART_LSR_TEMT)

#define UART_LSR_RXFE                  7
#define UART_LSR_RXFE_MSK             (1<<UART_LSR_RXFE)
#define UART_LSR_RXFE_SET(__VAL)       ((__VAL) << UART_LSR_RXFE)

#define UART_LSR_TXERR                 8 
#define UART_LSR_TXERR_MSK            (1 << UART_LSR_TXERR)
#define UART_LSR_TXERR_SET(__VAL)      ((__VAL) << UART_LSR_TXERR)


#define UART_MSR_DCTS                  0
#define UART_MSR_DCTS_MSK             (1<<UART_MSR_DCTS)
#define UART_MSR_DCTS_SET(__VAL)       ((__VAL) << UART_MSR_DCTS)

#define UART_MSR_DDSR                  1
#define UART_MSR_DDSR_MSK             (1<<UART_MSR_DDSR)
#define UART_MSR_DDSR_SET(__VAL)       ((__VAL) << UART_MSR_DDSR)

#define UART_MSR_TERI                  2
#define UART_MSR_TERI_MSK             (1<<UART_MSR_TERI)
#define UART_MSR_TERI_SET(__VAL)       ((__VAL) << UART_MSR_TERI)

#define UART_MSR_DDCD                  3
#define UART_MSR_DDCD_MSK             (1<<UART_MSR_DDCD)
#define UART_MSR_DDCD_SET(__VAL)       ((__VAL) << UART_MSR_DDCD)

#define UART_MSR_CTS                   4
#define UART_MSR_CTS_MSK              (1<<UART_MSR_CTS)
#define UART_MSR_CTS_SET(__VAL)        ((__VAL) << UART_MSR_CTS)

#define UART_MSR_DSR                   5
#define UART_MSR_DSR_MAKS              (1<<UART_MSR_DSR)
#define UART_MSR_DSR_SET(__VAL)        ((__VAL) << UART_MSR_DSR)

#define UART_MSR_RI                    6
#define UART_MSR_RI_MSK               (1<<UART_MSR_RI)
#define UART_MSR_RI_SET(__VAL)         ((__VAL) << UART_MSR_RI)

#define UART_MSR_DCD                   7
#define UART_MSR_DCD_MSK              (1<<UART_MSR_DCD)
#define UART_MSR_DCD_SET(__VAL)        ((__VAL) << UART_MSR_DCD)


#define UART_SCR_PAD0                  0
#define UART_SCR_PAD1                  1
#define UART_SCR_PAD2                  2
#define UART_SCR_PAD3                  3
#define UART_SCR_PAD4                  4
#define UART_SCR_PAD5                  5
#define UART_SCR_PAD6                  6
#define UART_SCR_PAD7                  7
#define UART_SCR_PAD_MSK              (   (1 << UART_SCR_PAD0) | \
                                            (1 << UART_SCR_PAD1) | \
                                            (1 << UART_SCR_PAD2) | \
                                            (1 << UART_SCR_PAD3) | \
                                            (1 << UART_SCR_PAD4) | \
                                            (1 << UART_SCR_PAD5) | \
                                            (1 << UART_SCR_PAD6) | \
                                            (1 << UART_SCR_PAD7))
#define UART_SCR_PAD_SET(__VAL)        ((__VAL) << UART_SCR_PAD0)


#define UART_ACR_AUTO_BAUD_START       0
#define UART_ACR_AUTO_BAUD_START_MSK  (1<<UART_ACR_AUTO_BAUD_START)

#define UART_ACR_MODE                  1
#define UART_ACR_MODE_MSK             (1<<UART_ACR_MODE)

#define UART_ACR_AUTO_RESTART          2
#define UART_ACR_AUTO_RESTART_MSK     (1<<UART_ACR_AUTO_RESTART)

#define UART_ACR_ABEOINTCLR            8
#define UART_ACR_ABEOINTCLR_MSK       (1<<UART_ACR_ABEOINTCLR)

#define UART_ACR_ABTOINTCLR            9
#define UART_ACR_ABTOINTCLR_MSK       (1<<UART_ACR_ABTOINTCLR)


#define UART_IRDAEN                    0 
#define UART_IRDAEN_MSK               (1 << UART_IRDAEN)      
#define UART_IRDAEN_SET(__VAL)         ((__VAL) << UART_IRDAEN)      

#define UART_IRDAINV                   1
#define UART_IRDAINV_MSK              (1 << UART_IRDAINV)      
#define UART_IRDAINV_SET(__VAL)        ((__VAL) << UART_IRDAINV)      


#define UART_RS485CTRL_NMMEN               0
#define UART_RS485CTRL_NMMEN_MSK          (1<<UART_RS485CTRL_NMMEN)
#define UART_RS485CTRL_NMMEN_SET(__VAL)    ((__VAL) << UART_RS485CTRL_NMMEN) 

#define UART_RS485CTRL_RXDIS               1
#define UART_RS485CTRL_RXDIS_MSK          (1<<UART_RS485CTRL_RXDIS)
#define UART_RS485CTRL_RXDIS_SET(__VAL)    ((__VAL) << UART_RS485CTRL_RXDIS)

#define UART_RS485CTRL_AADEN               2
#define UART_RS485CTRL_AADEN_MSK          (1<<UART_RS485CTRL_AADEN)
#define UART_RS485CTRL_AADEN_SET(__VAL)    ((__VAL) << UART_RS485CTRL_AADEN)

#define UART_RS485CTRL_SEL                 3
#define UART_RS485CTRL_SEL_MSK            (1<<UART_RS485CTRL_SEL)
#define UART_RS485CTRL_SEL_SET(__VAL)      ((__VAL) << UART_RS485CTRL_SEL)

#define UART_RS485CTRL_DCTRL               4
#define UART_RS485CTRL_DCTRL_MSK          (1<<UART_RS485CTRL_DCTRL)
#define UART_RS485CTRL_DCTRL_SET(__VAL)    ((__VAL) << UART_RS485CTRL_DCTRL)

#define UART_RS485CTRL_OINV                5
#define UART_RS485CTRL_OINV_MSK           (1<<UART_RS485CTRL_OINV)
#define UART_RS485CTRL_OINV_SET(__VAL)     ((__VAL) << UART_RS485CTRL_OINV)


#define UART_RS485ADRMATCH0           0
#define UART_RS485ADRMATCH1           1
#define UART_RS485ADRMATCH2           2
#define UART_RS485ADRMATCH3           3
#define UART_RS485ADRMATCH4           4
#define UART_RS485ADRMATCH5           5
#define UART_RS485ADRMATCH6           6
#define UART_RS485ADRMATCH7           7
#define UART_RS485ADRMATCH_MSK       ((1<<UART_RS485ADRMATCH0) | \
                                        (1<<UART_RS485ADRMATCH1)  | \
                                        (1<<UART_RS485ADRMATCH2)  | \
                                        (1<<UART_RS485ADRMATCH3)  | \
                                        (1<<UART_RS485ADRMATCH4)  | \
                                        (1<<UART_RS485ADRMATCH5)  | \
                                        (1<<UART_RS485ADRMATCH6)  | \
                                        (1<<UART_RS485ADRMATCH7))

#define UART_RE485ADRMATCH(__VALUE)   ((__VALUE) << UART_RS485ADRMATCH0)


#define UART_RS485_DLY0                0
#define UART_RS485_DLY1                1
#define UART_RS485_DLY2                2
#define UART_RS485_DLY3                3
#define UART_RS485_DLY4                4
#define UART_RS485_DLY5                5
#define UART_RS485_DLY6                6
#define UART_RS485_DLY7                7
#define UART_RS485_DLY_MSK            ((1<<UART_RS485_DLY0) | \
                                        (1<<UART_RS485_DLY1)  | \
                                        (1<<UART_RS485_DLY2)  | \
                                        (1<<UART_RS485_DLY3)  | \
                                        (1<<UART_RS485_DLY4)  | \
                                        (1<<UART_RS485_DLY5)  | \
                                        (1<<UART_RS485_DLY6)  | \
                                        (1<<UART_RS485_DLY7))
#define UART_RS485_DLY_VALUE(__VALUE)  ((__VALUE) << UART_RS485_DLY0)


//! UART TER TXEN 
#define UART_TER_TXEN                      7
#define UART_TER_TXEN_MSK                 (1<<UART_TER_TXEN)
#define UART_TER_TXEN_SET(__VAL)           ((__VAL) << UART_TER_TXEN)



#define UART_FIFOLVL_RX_BIT0           0
#define UART_FIFOLVL_RX_MSK            (0x0fu << UART_FIFOLVL_RX_BIT0)
#define UART_FIFOLVL_TX_BIT0           8
#define UART_FIFOLVL_TX_MSK            (0x0fu << UART_FIFOLVL_TX_BIT0)

/*============================ TYPES =========================================*/
//! UART REGISTER PAGE TYPE 
//! \name uart_t
//! @{
typedef struct {
    union {
        //!< Receiver Buffer Register
        DEF_REG32
            reg32_t RBR         :8;                     //!< Receiver Buffer Register
            reg32_t             :24;                    //!< Reserved
        END_DEF_REG32(RBR)

        //!< Transmit Holding Register
        DEF_REG32
            reg32_t THR         :8;                     //!< Transmit Holding Register
            reg32_t             :24;                    //!< Reserved
        END_DEF_REG32(THR)

        //!< Divisor Latch LSB
        DEF_REG32
            reg32_t DLLSB       :8;                     //!< Divisor Latch LSB
            reg32_t             :24;                    //!< Reserved
        END_DEF_REG32(DLL)
    };  
    
    union {
        //!< Divisor Latch MSB
        DEF_REG32
            reg32_t DLMSB       :8;                     //!< Divisor Latch MSB
            reg32_t             :24;                    //!< Reserved
        END_DEF_REG32(DLM)

        //!< Interrupt Enable Register
        DEF_REG32
            reg32_t RBRIE       :1;                     //!< RBR Interrupt Enable
            reg32_t THREIE      :1;                     //!< THRE Interrupt Enable
            reg32_t RXIE        :1;                     //!< RX Line Interrupt Enable
            reg32_t MSINtEn     :1;                     //!< Modem Status interrupt enable
            reg32_t             :3;                     //!< Reserved
            reg32_t CTSIntEn    :1;                     //!< CTS Interrupt Enable
            reg32_t ABEOINTEN   :1;                     //!< Enables the end of auto-baud interrupt
            reg32_t ABTOINTEN   :1;                     //!< Enables the auto-baud time-out interrupt
            reg32_t WakeUpIntEn :1;                     //!< Wake-up Interrupt Enable
            reg32_t             :21;                    //!< Reserved
        END_DEF_REG32(IER)
    };
    
    union {
        //!< Interrupt ID Register
        DEF_REG32
            reg32_t INTSTATUS	:1;                     //!< Interrupt status
            reg32_t INTID       :3;                     //!< Interrupt identification                          
            reg32_t             :2;                     //!< Reserved           
            reg32_t FIFOENABLE  :2;                     //!< Copies of FCR[0]              
            reg32_t ABEOINT     :1;                     //!< End of auto-baud interrupt               
            reg32_t ABTOINT     :1;                     //!< Auto-baud time-out interrupt           
            reg32_t WAKEUPINT   :1;                     //!< Wake-up interrupt           
            reg32_t             :21;                    //!< Reserved
        END_DEF_REG32(IIR)

        //!< FIFO Control Register
        DEF_REG32
            reg32_t FIFOEN      :1;                     //!< FIFO Enable
            reg32_t RXFIFORS    :1;                     //!< RX FIFO Reset
            reg32_t TXFIFORS    :1;                     //!< TX FIFO Reset 
            reg32_t DMAMO       :1;                     //!< DMA Mode Select
            reg32_t             :2;                     //!< Reserved
            reg32_t RXTL        :2;                     //!< RX Trigger Level
            reg32_t             :24;                    //!< Reserved
        END_DEF_REG32(FCR)
    };
    
    //!< Line Control Register    
    DEF_REG32
        reg32_t WLS             :2;                     //!< Word Length Select   
        reg32_t SBS             :1;                     //!< Stop Bit Select   
        reg32_t PE              :1;                     //!< Parity Enable   
        reg32_t PS              :2;                     //!< Parity Select   
        reg32_t BC              :1;                     //!< Break Control
        reg32_t DLAB            :1;                     //!< Divisor Latch Access Bit   
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(LCR)

    //!< Modem Control Register 
    DEF_REG32
        reg32_t DTRCTR          :1;                     //!< DTR Control 
        reg32_t RTSCTR          :1;                     //!< RTS Control
        reg32_t OUT1            :1;                     //!< Inverse control for the out1_an output
        reg32_t OUT2            :1;                     //!< Inverse control for the out2_an outputs
        reg32_t LMS             :1;                     //!< Loopback Mode Select
        reg32_t                 :1;                     //!< Reserved
        reg32_t RTSEN           :1;                     //!< RTS enable
        reg32_t CTSEN           :1;                     //!< CTS enable
        reg32_t                 :24;                    //!< Reserved        
    END_DEF_REG32(MCR)

    //!< Line Status Register  
    DEF_REG32
        reg32_t RDR             :1;                     //!< Receiver Data Ready
        reg32_t OE              :1;                     //!< Overrun Error
        reg32_t PE              :1;                     //!< Parity Error
        reg32_t FE              :1;                     //!< Framing Error
        reg32_t BI              :1;                     //!< Break Interrupt
        reg32_t THRE            :1;                     //!< Transmitter Holding Register Empty
        reg32_t TEMT            :1;                     //!< Transmitter Empty
        reg32_t RXFE            :1;                     //!< Error in RX FIFO
        reg32_t TXERR           :1;                     //!< Error in transmitted character
        reg32_t                 :23;                    //!< Reserved
    END_DEF_REG32(LSR)

    //!< Modem Status Register   
    DEF_REG32
        reg32_t DCTS            :1;                     //!< Delta CTS
        reg32_t DDSR            :1;                     //!< Delta DSR
        reg32_t TERI            :1;                     //!< Trailing Edge RI
        reg32_t DDCD            :1;                     //!< Delta DCD
        reg32_t CTS             :1;                     //!< Clear To Send State 
        reg32_t DSR             :1;                     //!< Data Set Ready State 
        reg32_t RI              :1;                     //!< Ring Indicator State
        reg32_t DCD             :1;                     //!< Data Carrier Detect State
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(MSR)

    //!< Scratch Pad Register
    DEF_REG32
        reg32_t PAD             :8;                     //!< Scratch pad. A readable, writable byte
        reg32_t                 :24;                    //!< Reserved         
    END_DEF_REG32(SCR)

    //!< Auto-baud Control Register
    DEF_REG32
        reg32_t START           :1;                      //!< Start bit
        reg32_t MODE            :1;                      //!< Auto-baud mode select bit
        reg32_t AUTORESTART     :1;                      //!< Restart bit
        reg32_t                 :5;                      //!< Reserved
        reg32_t ABEOINTCLR      :1;                      //!< End of auto-baud interrupt clear bit (write-only)
        reg32_t ABTOINTCLR      :1;                      //!< Auto-baud time-out interrupt clear bit (write-only)
        reg32_t                 :22;                     //!< Reserved
    END_DEF_REG32(ACR)

    //!< IrDA control register
    DEF_REG32
        reg32_t IRDAEN          :1;                     //!< IrDA mode enable                               
        reg32_t IRDAINV         :1;                     //!< Serial input direction                
        reg32_t FIXPULSEEN      :1;                     //!< IrDA fixed pulse width mode                
        reg32_t PULSEDIV        :3;                     //!< Configures the pulse when FixPulseEn = 1      
        reg32_t                 :26;                    //!< Reserved
    END_DEF_REG32(ICR)

    //!< Fractional Divider Register
    DEF_REG32
        reg32_t DIVADDVAL       :4;                     //!< Baud rate generation pre-scaler divisor value.
        reg32_t MULVAL          :4;                     //!< Baud rate pre-scaler multiplier value.
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(FDR)

    REG32_RSVD(1)                   //!< Reserved

    //! Transmit Enable Register
    DEF_REG32
        reg32_t                 :7;                     //!< Reserved
        reg32_t TXEN            :1;                     //!< Transmit enable     
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(TER)

    REG32_RSVD(6)                   //!< Reserved

    //!< RS-485/EIA-485 Control
    DEF_REG32
        reg32_t NMMEN           :1;                     //!< NMM enable
        reg32_t RXDIS           :1;                     //!< Receiver enable
        reg32_t AADEN           :1;                     //!< AAD enable
        reg32_t SEL             :1;                     //!< Reserved
        reg32_t DCTRL           :1;                     //!< Direction control for DIR pin
        reg32_t OINV            :1;                     //!< Direction control pin polarity
        reg32_t                 :26;                    //!< Reserved
    END_DEF_REG32(RS485CTRL)

    //!< RS-485/EIA-485 address match
    DEF_REG32
        reg32_t ADRMATCH        :8;                     //!< Contains the address match value
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(RS485ADRMATCH)

    //!< RS-485/EIA-485 direction control delay               
    DEF_REG32
        reg32_t DLY             :8;                     //!< Contains the direction control delay value    
        reg32_t                 :24;                    //!< Reserved
    END_DEF_REG32(RS485DLY)
    
    //! FIFO level register
//    DEF_REG32
//        reg32_t RXFIFOLVL       :4;                     //!< current level of the UART receiver FIFO
//        reg32_t                 :4;                     //!< Reserved
//        reg32_t TXFIFOLVL       :4;                     //!< current level of the UART transmitter FIFO
//        reg32_t                 :20;                    //!< Reserved        
//    END_DEF_REG32(FIFOLVL)

} uart_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
