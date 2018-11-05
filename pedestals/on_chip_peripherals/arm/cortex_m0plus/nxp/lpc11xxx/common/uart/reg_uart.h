#ifndef __REG_USART_H__
#define __REG_USART_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define USART0_REG                      (*(usart0_reg_t *)USART0_BASE_ADDRESS)
#define USART1_REG                      (*(usart_reg_t *)USART1_BASE_ADDRESS)
#define USART2_REG                      (*(usart_reg_t *)USART2_BASE_ADDRESS)
#define USART3_REG                      (*(usart_reg_t *)USART3_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define USART_RBR_BIAS                  0
#define USART_RBR_MASK                  BITMASK(7, 0)

#define USART_THR_BIAS                  0
#define USART_THR_MASK                  BITMASK(7, 0)

#define USART_DLLSB_BIAS                0
#define USART_DLLSB_MASK                BITMASK(7, 0)

#define USART_DLMSB_BIAS                0
#define USART_DLMSB_MASK                BITMASK(7, 0)

#define USART_IER_RBRIE_BIAS            0     
#define USART_IER_RBRIE_MASK            BIT(USART_IER_RBRIE_BIAS)     
#define USART_IER_THREIE_BIAS           1 
#define USART_IER_THREIE_MASK           BIT(USART_IER_THREIE_BIAS)    
#define USART_IER_RXIE_BIAS             2 
#define USART_IER_RXIE_MASK             BIT(USART_IER_RXIE_BIAS)      
#define USART_IER_MSINTEN_BIAS          3 
#define USART_IER_MSINTEN_MASK          BIT(USART_IER_MSINTEN_BIAS)      
#define USART_IER_CTSINTEN_BIAS         7 
#define USART_IER_CTSINTEN_MASK         BIT(USART_IER_CTSINTEN_BIAS)      
#define USART_IER_ABEOINTEN_BIAS        8 
#define USART_IER_ABEOINTEN_MASK        BIT(USART_IER_ABEOINTEN_BIAS) 
#define USART_IER_ABTOINTEN_BIAS        9 
#define USART_IER_ABTOINTEN_MASK        BIT(USART_IER_ABTOINTEN_BIAS) 
#define USART_IER_WAKEUPINTEN_BIAS      10 
#define USART_IER_WAKEUPINTEN_MASK      BIT(USART_IER_WAKEUPINTEN_BIAS) 

#define USART_IIR_INTSTATUS_BIAS	    0
#define USART_IIR_INTSTATUS_MASK        BIT(USART_IIR_INTSTATUS_BIAS)
#define USART_IIR_INTID_BIAS            1
#define USART_IIR_INTID_MASK            BITMASK(3, 1)
#define USART_IIR_FIFOENABLE_BIAS       6
#define USART_IIR_FIFOENABLE_MASK       BITMASK(7, 6)
#define USART_IIR_ABEOINT_BIAS          8
#define USART_IIR_ABEOINT_MASK          BIT(USART_IIR_ABEOINT_BIAS)     
#define USART_IIR_ABTOINT_BIAS          9 
#define USART_IIR_ABTOINT_MASK          BIT(USART_IIR_ABTOINT_BIAS)     
#define USART_IIR_WAKEUPINT_BIAS        10 
#define USART_IIR_WAKEUPINT_MASK        BIT(USART_IIR_WAKEUPINT_BIAS)       

#define USART_FCR_FIFO_EN_BIAS          0
#define USART_FCR_FIFO_EN_MASK          BIT(USART_FCR_FIFO_EN_BIAS)
#define USART_FCR_RX_FIFO_RS_BIAS       1
#define USART_FCR_RX_FIFO_RS_MASK       BIT(USART_FCR_RX_FIFO_RS_BIAS)
#define USART_FCR_TX_FIFO_RS_BIAS       2
#define USART_FCR_TX_FIFO_RS_MASK       BIT(USART_FCR_TX_FIFO_RS_BIAS)
#define USART_FCR_DMA_MO_BIAS           3
#define USART_FCR_DMA_MO_MASK           BIT(USART_FCR_DMA_MO_BIAS)
#define USART_FCR_RXTL_BIAS             6
#define USART_FCR_RTXL_MASK             BITMASK(7, 6)

#define USART_LCR_WLS_BIAS              0
#define USART_LCR_WLS_MASK              BITMASK(1, 0)
#define USART_LCR_SBS_BIAS              2
#define USART_LCR_SBS_MASK              BIT(USART_LCR_SBS_BIAS)
#define USART_LCR_PE_BIAS               3
#define USART_LCR_PE_MASK               BIT(USART_LCR_PE_BIAS)
#define USART_LCR_PS_BIAS               4
#define USART_LCR_PS_MASK               BITMASK(5, 4)
#define USART_LCR_BC_BIAS               6
#define USART_LCR_BC_MASK               BIT(USART_LCR_BC_BIAS)
#define USART_LCR_DLAB_BIAS             7
#define USART_LCR_DLAB_MASK             BIT(USART_LCR_DLAB_BIAS)

#define USART_MCR_DTRCTR_BIAS           0
#define USART_MCR_DTRCTR_MASK           BIT(USART_MCR_DTRCTR_BIAS)
#define USART_MCR_RTSCTR_BIAS           1
#define USART_MCR_RTSCTR_MASK           BIT(USART_MCR_RTSCTR_BIAS)
#define USART_MCR_OUT1_BIAS             2
#define USART_MCR_OUT1_MASK             BIT(USART_MCR_OUT1_BIAS)
#define USART_MCR_OUT2_BIAS             3
#define USART_MCR_OUT2_MASK             BIT(USART_MCR_OUT2_BIAS)
#define USART_MCR_LMS_BIAS              4
#define USART_MCR_LMS_MASK              BIT(USART_MCR_LMS_BIAS)
#define USART_MCR_RTSEN_BIAS            6
#define USART_MCR_RTSEN_MASK            BIT(USART_MCR_RTSEN_BIAS)
#define USART_MCR_CTSEN_BIAS            7
#define USART_MCR_CTSEN_MASK            BIT(USART_MCR_CTSEN_BIAS)

#define USART_LSR_RDR_BIAS              0
#define USART_LSR_RDR_MASK              BIT(USART_LSR_RDR_BIAS)
#define USART_LSR_OE_BIAS               1
#define USART_LSR_OE_MASK               BIT(USART_LSR_OE_BIAS)
#define USART_LSR_PE_BIAS               2
#define USART_LSR_PE_MASK               BIT(USART_LSR_PE_BIAS)
#define USART_LSR_FE_BIAS               3
#define USART_LSR_FE_MASK               BIT(USART_LSR_FE_BIAS)
#define USART_LSR_BI_BIAS               4
#define USART_LSR_BI_MASK               BIT(USART_LSR_BI_BIAS)
#define USART_LSR_THRE_BIAS             5
#define USART_LSR_THRE_MASK             BIT(USART_LSR_THRE_BIAS)
#define USART_LSR_TEMT_BIAS             6
#define USART_LSR_TEMT_MASK             BIT(USART_LSR_TEMT_BIAS)
#define USART_LSR_RXFE_BIAS             7
#define USART_LSR_RXFE_MASK             BIT(USART_LSR_RXFE_BIAS)
#define USART_LSR_TXERR_BIAS            8
#define USART_LSR_TXERR_MASK            BIT(USART_LSR_TXERR_BIAS)

#define USART_MSR_DCTS_BIAS             0
#define USART_MSR_DCTS_MASK             BIT(USART_MSR_DCTS_BIAS)
#define USART_MSR_DDSR_BIAS             1
#define USART_MSR_DDSR_MASK             BIT(USART_MSR_DDSR_BIAS)
#define USART_MSR_TERI_BIAS             2
#define USART_MSR_TERI_MASK             BIT(USART_MSR_TERI_BIAS)
#define USART_MSR_DDCD_BIAS             3
#define USART_MSR_DDCD_MASK             BIT(USART_MSR_DDCD_BIAS)
#define USART_MSR_CTS_BIAS              4
#define USART_MSR_CTS_MASK              BIT(USART_MSR_CTS_BIAS)
#define USART_MSR_DSR_BIAS              5
#define USART_MSR_DSR_MAKS              BIT(USART_MSR_DSR_BIAS)
#define USART_MSR_RI_BIAS               6
#define USART_MSR_RI_MASK               BIT(USART_MSR_RI_BIAS)
#define USART_MSR_DCD_BIAS              7
#define USART_MSR_DCD_MASK              BIT(USART_MSR_DCD_BIAS)

#define USART_SCR_PAD_BIAS              0
#define USART_SCR_PAD_MASK              BITMASK(7, 0)

#define USART_ACR_AUTO_BAUD_START_BIAS  0
#define USART_ACR_AUTO_BAUD_START_MASK  BIT(USART_ACR_AUTO_BAUD_START_BIAS)
#define USART_ACR_MODE_BIAS             1
#define USART_ACR_MODE_MASK             BIT(USART_ACR_MODE_BIAS)
#define USART_ACR_AUTO_RESTART_BIAS     2
#define USART_ACR_AUTO_RESTART_MASK     BIT(USART_ACR_AUTO_RESTART_BIAS)
#define USART_ACR_ABEOINTCLR_BIAS       8
#define USART_ACR_ABEOINTCLR_MASK       BIT(USART_ACR_ABEOINTCLR_BIAS)
#define USART_ACR_ABTOINTCLR_BIAS       9
#define USART_ACR_ABTOINTCLR_MASK       BIT(USART_ACR_ABTOINTCLR_BIAS)

//! USART TER TXEN 
#define USART_TER_TXEN_BIAS             7
#define USART_TER_TXEN_MASK             BIT(USART_TER_TXEN_BIAS)

#define USART_STAT_RXRDY_BIAS           0
#define USART_STAT_RXRDY_MASK           BIT(0)
#define USART_STAT_RXIDLE_BIAS          1
#define USART_STAT_RXIDLE_MASK          BIT(1)
#define USART_STAT_TXRDY_BIAS           2
#define USART_STAT_TXRDY_MASK           BIT(2)
#define USART_STAT_TXIDLE_BIAS          3
#define USART_STAT_TXIDLE_MASK          BIT(3)
#define USART_STAT_CTS_BIAS             4
#define USART_STAT_CTS_MASK             BIT(4)
#define USART_STAT_DELTACTS_BIAS        5
#define USART_STAT_DELTACTS_MASK        BIT(5)
#define USART_STAT_TXDISSTAT_BIAS       6
#define USART_STAT_TXDISSTAT_MASK       BIT(6)
#define USART_STAT_OVERRUNINT_BIAS      8
#define USART_STAT_OVERRUNINT_MASK      BIT(8)
#define USART_STAT_RXBRK_BIAS           10
#define USART_STAT_RXBRK_MASK           BIT(10)
#define USART_STAT_DELTARXBRK_BIAS      11
#define USART_STAT_DELTARXBRK_MASK      BIT(11)
#define USART_STAT_START_BIAS           12
#define USART_STAT_START_MASK           BIT(12)
#define USART_STAT_FRAMERRINT_BIAS      13
#define USART_STAT_FRAMERRINT_MASK      BIT(13)
#define USART_STAT_PARITYERRINT_BIAS    14
#define USART_STAT_PARITYERRINT_MASK    BIT(14)
#define USART_STAT_RXNOISEINT_BIAS      15
#define USART_STAT_RXNOISEINT_MASK      BIT(15)
#define USART_STAT_ABERR_BIAS           16
#define USART_STAT_ABERR_MASK           BIT(16)

#define USART_CFG_ENABLE_BIAS           0
#define USART_CFG_ENABLE_MASK           BIT(0)
#define USART_CFG_DATALEN_BIAS          2
#define USART_CFG_DATALEN_MASK          BITMASK(3, 2)
#define USART_CFG_PARITYSEL_BIAS        4
#define USART_CFG_PARITYSEL_MASK        BITMASK(5, 4)
#define USART_CFG_STOPLEN_BIAS          6
#define USART_CFG_STOPLEN_MASK          BIT(6)
#define USART_CFG_MODE32K_BIAS          7
#define USART_CFG_MODE32K_MASK          BIT(7)
#define USART_CFG_CTSEN_BIAS            9
#define USART_CFG_CTSEN_MASK            BIT(9)
#define USART_CFG_SYNCEN_BIAS           11
#define USART_CFG_SYNCEN_MASK           BIT(11)
#define USART_CFG_CLKPOL_BIAS           12
#define USART_CFG_CLKPOL_MASK           BIT(12)
#define USART_CFG_SYNCMST_BIAS          14
#define USART_CFG_SYNCMST_MASK          BIT(14)
#define USART_CFG_LOOP_BIAS             15
#define USART_CFG_LOOP_MASK             BIT(15)
#define USART_CFG_OETA_BIAS             18
#define USART_CFG_OETA_MASK             BIT(18)
#define USART_CFG_AUTOADDR_BIAS         19
#define USART_CFG_AUTOADDR_MASK         BIT(19)
#define USART_CFG_OESEL_BIAS            20
#define USART_CFG_OESEL_MASK            BIT(20)
#define USART_CFG_OEPOL_BIAS            21
#define USART_CFG_OEPOL_MASK            BIT(21)
#define USART_CFG_RXPOL_BIAS            22
#define USART_CFG_RXPOL_MASK            BIT(22)
#define USART_CFG_TXPOL_BIAS            23
#define USART_CFG_TXPOL_MASK            BIT(23)

#define USART_CTL_TXBRKEN_BIAS          1
#define USART_CTL_TXBRKEN_MASK          BIT(1)
#define USART_CTL_ADDRDET_BIAS          2
#define USART_CTL_ADDRDET_MASK          BIT(2)
#define USART_CTL_TXDIS_BIAS            6
#define USART_CTL_TXDIS_MASK            BIT(6)
#define USART_CTL_CC_BIAS               8
#define USART_CTL_CC_MASK               BIT(8)
#define USART_CTL_CLRCCONRX_BIAS        9
#define USART_CTL_CLRCCONRX_MASK        BIT(9)
#define USART_CTL_AUTOBAUD_BIAS         16
#define USART_CTL_AUTOBAUD_MASK         BIT(16)

/*============================ TYPES =========================================*/
//! USART REGISTER PAGE TYPE 
//! \name usart0_t
//! @{
typedef struct {
	union {
		reg32_t         DLL;			/*!< Divisor Latch LSB. Least significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
		reg32_t         THR;			/*!< Transmit Holding Register. The next character to be transmitted is written here (DLAB = 0). */
		const reg32_t   RBR;			/*!< Receiver Buffer Register. Contains the next received character to be read (DLAB = 0). */
	};

	union {
		reg32_t         IER;			/*!< Interrupt Enable Register. Contains individual interrupt enable bits for the 7 potential USART interrupts (DLAB = 0). */
		reg32_t         DLM;			/*!< Divisor Latch MSB. Most significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
	};

	union {
		reg32_t         FCR;			/*!< FIFO Control Register. Controls USART FIFO usage and modes. */
		const reg32_t   IIR;			/*!< Interrupt ID Register. Identifies which interrupt(s) are pending. */
	};

	reg32_t         LCR;				/*!< Line Control Register. Contains controls for frame formatting and break generation. */
	reg32_t         MCR;				/*!< Modem Control Register. Only present on USART ports with full modem support. */
	const reg32_t   LSR;				/*!< Line Status Register. Contains flags for transmit and receive status, including line errors. */
	const reg32_t   MSR;				/*!< Modem Status Register. Only present on USART ports with full modem support. */
	reg32_t         SCR;				/*!< Scratch Pad Register. Eight-bit temporary storage for software. */
	reg32_t         ACR;				/*!< Auto-baud Control Register. Contains controls for the auto-baud feature. */
	reg32_t         ICR;				/*!< IrDA control register (not all USARTS) */
	reg32_t         FDR;				/*!< Fractional Divider Register. Generates a clock input for the baud rate divider. */
	reg32_t         OSR;				/*!< Oversampling Register. Controls the degree of oversampling during each bit time. Only on some USARTS. */
	reg32_t         TER;				/*!< Transmit Enable Register. Turns off USART transmitter for use with software flow control. */
	REG32_RSVD(3)
	reg32_t         HDEN;				/*!< Half-duplex enable Register- only on some USARTs */
	REG32_RSVD(1)
	reg32_t         SCICTRL;			/*!< Smart card interface control register- only on some USARTs */
	reg32_t         RS485CTRL;		    /*!< RS-485/EIA-485 Control. Contains controls to configure various aspects of RS-485/EIA-485 modes. */
	reg32_t         RS485ADRMATCH;	    /*!< RS-485/EIA-485 address match. Contains the address match value for RS-485/EIA-485 mode. */
	reg32_t         RS485DLY;			/*!< RS-485/EIA-485 direction control delay. */
	reg32_t         SYNCCTRL;			/*!< Synchronous mode control register. Only on USARTs. */
} usart0_reg_t;
//! @}

//! USART REGISTER PAGE TYPE 
//! \name usart_t
//! @{
typedef struct {
	reg32_t         CFG;				/*!< Configuration register */
	reg32_t         CTL;			/*!< Control register */
	reg32_t         STAT;			/*!< Status register */
	reg32_t         INTENSET;		/*!< Interrupt Enable read and set register */
	reg32_t         INTENCLR;		/*!< Interrupt Enable clear register */
	const reg32_t   RXDAT;			/*!< Receive Data register */
	const reg32_t   RXDATSTAT;		/*!< Receive Data with status register */
	reg32_t         TXDAT;			/*!< Transmit data register */
	reg32_t         BRG;				/*!< Baud Rate Generator register */
	reg32_t         INTSTAT;			/*!< Interrupt status register */
	reg32_t         OSR;				/*!< Oversample selection register for asynchronous communication */
	reg32_t         ADDR;			/*!< Address register for automatic address matching */
} usart_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
