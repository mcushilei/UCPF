/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! \note do not move this pre-processor statement to other places
#define __DRIVER_LPC17XX_EMAC_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_emac.h"
#include ".\phy\dp83848.h"

/*============================ MACROS ========================================*/
//#define NET_DEBUG

#define MAC_RAM_BASE		0x2007c000
#define MAC_RAM_SIZE		0x00008000       

/* The Ethernet RAM is configured as below, the starting of MAC_DESC_BASE depends
   on the EMAC_DESC_COUNT or the TOTAL_DESC_SIZE, at this point, the 
   EMAC_DESC_COUNT for both TX and RX is set to 16:

   MAC_RAM_BASE	0x2007c000
   .
   .
   .
   EMAC_DMA_END		MAC_RAM_BASE + MAC_RAM_SIZE - TOTAL_DESC_SIZE 

   TX_DESC_ADDR = MAC_DESC_BASE = MAC_RAM_END(MAC_RAM_BASE+MAC_RAM_SIZE) - TOTAL_DESC 
   TX_STAT_BASE = TX_DESC_ADDR + TX_DESC_SIZE
   RX_DESC_BASE = TX_DESC_ADDR + TX_DESC_SIZE + TX_STAT_SIZE
   RX_STAT_BASE = RX_DESC_BASE + RX_STAT_SIZE
   ( RX_STAT_BASE + RX_STAT_SIZE = MAC_RAM_END )!!!!!
   EMAX_RAM_END	0x7FE04000

   Please note that, the descriptors have to be aligned to the 4-bytes boundary!!!
   Below descriptor addresses have been carefully aligned to the 4-bytes boundary. 
   If not, the descriptors have to be re-aligned!!! 
*/
#define TX_DESC_NUM	            (8)     //!< should be 1, 2, 4, 8, 16....
#define RX_DESC_NUM	            (8)
#define TX_DESC_SIZE	        (TX_DESC_NUM * 8)   
#define TX_STAT_SIZE		    (TX_DESC_NUM * 4)   
#define RX_DESC_SIZE	        (RX_DESC_NUM * 8)
#define RX_STAT_SIZE		    (RX_DESC_NUM * 8)
#define TOTAL_DESC_SIZE	        (TX_DESC_SIZE + TX_STAT_SIZE + RX_DESC_SIZE + RX_STAT_SIZE)     //336B
#define MAC_DESC_BASE	        (MAC_RAM_BASE + MAC_RAM_SIZE - TOTAL_DESC_SIZE)              
#define TX_DESC_BASE	        (MAC_DESC_BASE)
#define TX_STAT_BASE		    (TX_DESC_BASE + TX_DESC_SIZE)        
#define RX_DESC_BASE	        (TX_STAT_BASE + TX_STAT_SIZE)        
#define RX_STAT_BASE		    (RX_DESC_BASE + RX_DESC_SIZE)        

#define MAC_BLOCK_SIZE			((uint16_t)1536)
#define MAC_TX_BLOCK_NUM		(0)	          
#define MAC_RX_BLOCK_NUM		(0)              // 6kB
#define MAC_TOTAL_BLOCK_NUM	    (MAC_TX_BLOCK_NUM + MAC_RX_BLOCK_NUM)
#define MAC_TOTAL_BUFFER_SIZE	(MAC_BLOCK_SIZE * MAC_TOTAL_BLOCK_NUM)
#define TX_BUFFER_BASE	        (MAC_RAM_BASE)
#define RX_BUFFER_BASE	        (MAC_RAM_BASE + MAC_BLOCK_SIZE * MAC_TX_BLOCK_NUM)


/* RX and TX descriptor and status definitions. */
#define TX_DESC_PACKET(i)       (emacDscrptTx[i].Buffer)
#define TX_DESC_CTRL(i)         (emacDscrptTx[i].Control)
#define TX_STAT_INFO(i)         (emacStatusTx[i].Status)

#define RX_DESC_PACKET(i)       (emacDscrptRx[i].Buffer)
#define RX_DESC_CTRL(i)         (emacDscrptRx[i].Control)
#define RX_STAT_INFO(i)         (emacStatusRx[i].Status)
#define RX_STAT_HASHCRC(i)      (emacStatusRx[i].HashCRC)

#define TX_BUFFER(i)            (TX_BUFFER_BASE + MAC_BLOCK_SIZE * i)
#define RX_BUFFER(i)            (RX_BUFFER_BASE + MAC_BLOCK_SIZE * i)


/* EMAC Descriptor TX Control fields */
#define TX_CTRL_INT		        ((uint32_t)(1 << 31))
#define TX_CTRL_LAST		    ((uint32_t)(1 << 30))
#define TX_CTRL_CRC		        ((uint32_t)(1 << 29))
#define TX_CTRL_PAD		        ((uint32_t)(1 << 28))
#define TX_CTRL_HUGE		    ((uint32_t)(1 << 27))
#define TX_CTRL_OVERRIDE	    ((uint32_t)(1 << 26))

/* EMAC Descriptor RX Control fields */
#define RX_CTRL_INT		        ((uint32_t)(1 << 31))

/* EMAC Descriptor TX Status related definition */
#define TX_STAT_ERR		        ((uint32_t)(1 << 31))
#define TX_STAT_NODESC	        ((uint32_t)(1 << 30))
#define TX_STAT_UNDERRUN        ((uint32_t)(1 << 29))
#define TX_STAT_LCOL	        ((uint32_t)(1 << 28))
#define TX_STAT_ECOL	        ((uint32_t)(1 << 27))
#define TX_STAT_EDEFER	        ((uint32_t)(1 << 26))
#define TX_STAT_DEFER	        ((uint32_t)(1 << 25))
#define TX_STAT_COLCNT	        ((uint32_t)0x01E00000)/* four bits, it's a mask, not exact count */

/* EMAC Descriptor RX Status related definition */
#define RX_STAT_ERR		        ((uint32_t)(1 << 31))
#define RX_STAT_LAST	        ((uint32_t)(1 << 30))
#define RX_STAT_NODESC	        ((uint32_t)(1 << 29))
#define RX_STAT_OVERRUN	        ((uint32_t)(1 << 28))
#define RX_STAT_ALGNERR	        ((uint32_t)(1 << 27))
#define RX_STAT_RNGERR	        ((uint32_t)(1 << 26))
#define RX_STAT_LENERR	        ((uint32_t)(1 << 25))
#define RX_STAT_SYMERR	        ((uint32_t)(1 << 24))
#define RX_STAT_CRCERR	        ((uint32_t)(1 << 23))
#define RX_STAT_BCAST	        ((uint32_t)(1 << 22))
#define RX_STAT_MCAST	        ((uint32_t)(1 << 21))
#define RX_STAT_FAILFLT	        ((uint32_t)(1 << 20))
#define RX_STAT_VLAN	        ((uint32_t)(1 << 19))
#define RX_STAT_CTLFRAM	        ((uint32_t)(1 << 18))
#define RX_STAT_RX_SIZE         ((uint32_t)0x000007FF)/* 11 bits for both TX and RX   */

/* EMAC interrupt controller related definition */
#define MAC_INT_RXOVERRUN	                ((uint32_t)1 << 0)
#define MAC_INT_RXERROR	                    ((uint32_t)1 << 1) 
#define MAC_INT_RXFINISHED	                ((uint32_t)1 << 2)
#define MAC_INT_RXDONE		                ((uint32_t)1 << 3) 
#define MAC_INT_TXUNDERRUN	                ((uint32_t)1 << 4)
#define MAC_INT_TXERROR	                    ((uint32_t)1 << 5) 
#define MAC_INT_TXFINISHED	                ((uint32_t)1 << 6)
#define MAC_INT_TXDONE		                ((uint32_t)1 << 7) 
#define MAC_INT_SOFTINT	                    ((uint32_t)1 << 12)
#define MAC_INT_WOL		                    ((uint32_t)1 << 13)
#define MAC_INT_ALL		                    ((uint32_t)0x30ff) 


#define MAC_MCMD_WRIT                       ((uint32_t)0x0)
#define MAC_MCMD_READ                       ((uint32_t)0x01)
#define MAC_MCMD_SCAN                       ((uint32_t)0x02)

#define MAC_MIND_BUSY                       ((uint32_t)0x01)
#define MAC_MIND_SCANNING                   ((uint32_t)0x02)
#define MAC_MIND_NOTVALID                   ((uint32_t)0x04)
#define MAC_MIND_LINKFAIL                   ((uint32_t)0x08)


#define MAC_MAC1_RECEIVE_ENABLE             ((uint32_t)0x0001)
#define MAC_MAC1_PASS_ALL_RECEIVE_FRAMES    ((uint32_t)0x0002)
#define MAC_MAC1_RX_FLOW_CONTROL            ((uint32_t)0x0004)
#define MAC_MAC1_TX_FLOW_CONTROL            ((uint32_t)0x0008)
#define MAC_MAC1_LOOPBACK                   ((uint32_t)0x0010)
#define MAC_MAC1_RESET_TX                   ((uint32_t)0x0100)
#define MAC_MAC1_RESET_MCS_TX               ((uint32_t)0x0200)
#define MAC_MAC1_RESET_RX                   ((uint32_t)0x0400)
#define MAC_MAC1_RESET_MCS_RX               ((uint32_t)0x0800)
#define MAC_MAC1_SIMULATION_RESET           ((uint32_t)0x4000)
#define MAC_MAC1_SOFT_RESET                 ((uint32_t)0x8000)


#define MAC_MAC2_FULL_DUPLEX                ((uint32_t)0x0001)
#define MAC_MAC2_FRAME_LENGTH_CHECKONG      ((uint32_t)0x0002)
#define MAC_MAC2_HUGE_FRAME_ENABLE          ((uint32_t)0x0004)
#define MAC_MAC2_DELAYED_CRC                ((uint32_t)0x0008)
#define MAC_MAC2_CRC_ENABLE                 ((uint32_t)0x0010)
#define MAC_MAC2_PAD_CRC_ENABLE             ((uint32_t)0x0020)
#define MAC_MAC2_VLAN_PAD_ENABLE            ((uint32_t)0x0040)
#define MAC_MAC2_AUTO_DETECT_PAD_ENABLE     ((uint32_t)0x0080)
#define MAC_MAC2_PURE_PREAMBLE_ENFORCEMENT  ((uint32_t)0x0100)
#define MAC_MAC2_NO_BACKOFF                 ((uint32_t)0x0200)
#define MAC_MAC2_BACK_PRESSURE_NO_BACKOFF   ((uint32_t)0x1000)
#define MAC_MAC2_EXCESS_DEFER               ((uint32_t)0x2000)


#define MAC_COMMAND_RX_ENABLE               ((uint32_t)0x0001)
#define MAC_COMMAND_TX_ENABLE               ((uint32_t)0x0002)
#define MAC_COMMAND_REG_RESET               ((uint32_t)0x0008)
#define MAC_COMMAND_TX_RESET                ((uint32_t)0x0010)
#define MAC_COMMAND_RX_RESET                ((uint32_t)0x0020)
#define MAC_COMMAND_PASS_RUNT_FRAME         ((uint32_t)0x0040)
#define MAC_COMMAND_PASS_RX_FILTER          ((uint32_t)0x0080)
#define MAC_COMMAND_TX_FLOW_CONTROL         ((uint32_t)0x0100)
#define MAC_COMMAND_RMII_MODE               ((uint32_t)0x0200)
#define MAC_COMMAND_FULL_DUPLEX             ((uint32_t)0x0400)


#define MAC_STATUS_RX                       ((uint32_t)0x0001)
#define MAC_STATUS_TX                       ((uint32_t)0x0002)


#define MAC_RXFILTERCTRL_AC_UNICAST         ((uint32_t)0x0001)
#define MAC_RXFILTERCTRL_AC_BROADCAST       ((uint32_t)0x0002)
#define MAC_RXFILTERCTRL_AC_MULTICAST       ((uint32_t)0x0004)
#define MAC_RXFILTERCTRL_AC_UNICAST_HASH    ((uint32_t)0x0008)
#define MAC_RXFILTERCTRL_AC_MULTICAST_HASH  ((uint32_t)0x0010)
#define MAC_RXFILTERCTRL_AC_PERFECT         ((uint32_t)0x0020)
#define MAC_RXFILTERCTRL_MAGIC_PACKET_WOL   ((uint32_t)0x0100)
#define MAC_RXFILTERCTRL_FILTER_WOL         ((uint32_t)0x0200)


#define MAC_SUPP_SPEED                      ((uint32_t)0x0100)
#define MAC_SUPP_REST_RMII                  ((uint32_t)0x0800)


enum {
    FULL_DUPLES_100M = 1,
    HALF_DUPLES_100M,
    FULL_DUPLES_10M,
    HALF_DUPLES_10M,
};


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
struct pbuf *RxArray[RX_DESC_NUM];

static emac_tx_desc_t      emacDscrptTx[TX_DESC_NUM];
static emac_tx_status_t    emacStatusTx[TX_DESC_NUM];

static emac_rx_desc_t      emacDscrptRx[RX_DESC_NUM];
static emac_rx_status_t    emacStatusRx[RX_DESC_NUM];

volatile uint32_t RXOverrunCount  = 0;
volatile uint32_t RXErrorCount    = 0;
volatile uint32_t RxFinishedCount = 0;
volatile uint32_t TxDoneCount     = 0;
volatile uint32_t TXUnderrunCount = 0;
volatile uint32_t TXErrorCount    = 0;
volatile uint32_t TxFinishedCount = 0;
volatile uint32_t RxDoneCount     = 0;

static void (*emacTxDoneCallback)(void);
static void (*emacRxDoneCallback)(void);


/*============================ PROTOTYPES ====================================*/
static struct pbuf *MAC_pbuf_alloc(void);
static void   MAC_TxDescriptorInit(void);
static void   MAC_RxDescriptorInit(void);
static void   MAC_TxReset(void);
static void   MAC_RxReset(void);


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool emac_enable(void)
{
    return true;
}

bool emac_disable(void)
{
    return true;
}

void emac_enable_tx(void)
{
    EMAC_REG.Command |= MAC_COMMAND_TX_ENABLE;
}

void emac_disable_tx(void)
{
    EMAC_REG.Command &= ~MAC_COMMAND_TX_ENABLE;
}

void emac_enable_rx(void)
{                                                       // 注意顺序
    EMAC_REG.Command |= MAC_COMMAND_RX_ENABLE;          // DMA接收使能
    EMAC_REG.MAC1    |= MAC_MAC1_RECEIVE_ENABLE;        // 接收通道使能
}

void emac_disable_rx(void)
{                                                
    EMAC_REG.MAC1    &= ~MAC_MAC1_RECEIVE_ENABLE;
    EMAC_REG.Command &= ~MAC_COMMAND_RX_ENABLE;
}

bool emac_init(void)
{
	uint32_t tout;

    /* Reset: soft, simulation, mcs/rx, rx, mcs/tx, tx */
    EMAC_REG.MAC1   = MAC_MAC1_SOFT_RESET
                    | MAC_MAC1_SIMULATION_RESET
                    | MAC_MAC1_RESET_TX
                    | MAC_MAC1_RESET_RX
                    | MAC_MAC1_RESET_MCS_RX
                    | MAC_MAC1_RESET_MCS_TX;
    /* RegReset */
    EMAC_REG.Command    = MAC_COMMAND_REG_RESET
                        | MAC_COMMAND_TX_RESET
                        | MAC_COMMAND_RX_RESET;
    for (tout = 0; tout < 1000; tout++);  /* short delay after reset */
    EMAC_REG.MAC1 = 0;                         /* deassert all of the above soft resets in MAC1 */
    EMAC_REG.MAC2 = 0;                         /* initialize MAC2 register to default value */ 
    EMAC_REG.Command = 0;
    for (tout = 0; tout < 1000; tout++);  /* short delay after reset */

    /* RMII configuration */
    EMAC_REG.Command = MAC_COMMAND_RMII_MODE | MAC_COMMAND_PASS_RUNT_FRAME;    //选择RMII模式；接收小于64字节的短帧

    EMAC_REG.SUPP = MAC_SUPP_REST_RMII;
    for (tout = 0; tout < 100; tout++);  /* short delay after reset */

	/* Reset PHY */
    EMAC_REG.MCFG = 0x8000;
    for (tout = 0; tout < 100; tout++);  /* short delay after reset */

    /* MII Mgmt configuration */
    EMAC_REG.MCFG = 0x0028;		/* host clock divided by 10, no suppress preamble, no scan increment */
    if (EMAC_REG.MIND & MAC_MIND_LINKFAIL) {
        false;
    }

    /* Initialate PHY. */ 
    if (!ethernet_phy_init()) {
        return false;
    }
    
    /* Pass All Rx Frame */
#ifdef NET_DEBUG
    EMAC_REG.MAC1 = MAC_MAC1_PASS_ALL_RECEIVE_FRAMES | MAC_MAC1_LOOPBACK;
#else
    EMAC_REG.MAC1 = MAC_MAC1_PASS_ALL_RECEIVE_FRAMES;
#endif
    

    EMAC_REG.CLRT = 0x370F;    /* 冲突窗口/重试寄存器 */
    EMAC_REG.MAXF = 0x0600;    /* 1536字节 */

    /* Set RX filter */
    EMAC_REG.RxFilterCtrl   = MAC_RXFILTERCTRL_AC_MULTICAST_HASH
                            | MAC_RXFILTERCTRL_AC_UNICAST_HASH
                            | MAC_RXFILTERCTRL_AC_MULTICAST
                            | MAC_RXFILTERCTRL_AC_UNICAST
                            | MAC_RXFILTERCTRL_AC_BROADCAST; 

    
    if (!emac_net_mode_config(ethernet_phy_get_net_mode())) {
        return false;
    }
    
    /*Config Tx and Rx descriptor tables.*/
    MAC_TxDescriptorInit();
    MAC_RxDescriptorInit();

    /* MAC interrupt related register setting */
    EMAC_REG.IntClear  = MAC_INT_ALL;	/* clear all MAC interrupts */    
    EMAC_REG.IntEnable = MAC_INT_TXDONE | MAC_INT_TXFINISHED | MAC_INT_RXDONE | MAC_INT_RXFINISHED;//
    
    /*Enable Tx and Rx.*/
    emac_enable_tx();
    emac_enable_rx();	
        
    return true; 
}

bool emac_net_mode_config(uint8_t mode)  
{
    switch (mode) {
        case FULL_DUPLES_100M::
            EMAC_REG.MAC2 = 0x31;             /* full duplex, CRC and PAD enabled. */
            EMAC_REG.Command |= MAC_COMMAND_FULL_DUPLEX; 
            EMAC_REG.SUPP = MAC_SUPP_SPEED;   /* RMII Support Reg. speed is set to 100M */
            EMAC_REG.IPGT = 0x0015;           /* IPG setting in full duplex mode */
            break;
        
        case HALF_DUPLES_100M:
            EMAC_REG.MAC2 = 0x30;             /* half duplex, CRC and PAD enabled. */
            EMAC_REG.Command &= ~MAC_COMMAND_FULL_DUPLEX;
            EMAC_REG.SUPP = MAC_SUPP_SPEED;   /* RMII Support Reg. speed is set to 100M */
            EMAC_REG.IPGT = 0x0012;           /* IPG setting in half duplex mode */
            break;
        
        case FULL_DUPLES_10M:
            EMAC_REG.MAC2 = 0x31;             /* full duplex, CRC and PAD enabled. */
            EMAC_REG.Command |= MAC_COMMAND_FULL_DUPLEX;
            EMAC_REG.SUPP = 0;                /* RMII Support Reg. speed is set to 10M */
            EMAC_REG.IPGT = 0x0015;           /* IPG setting in full duplex mode */
            break;
        
        case HALF_DUPLES_10M:
            EMAC_REG.MAC2 = 0x30;             /* half duplex, CRC and PAD enabled. */
            EMAC_REG.Command &= ~MAC_COMMAND_FULL_DUPLEX;
            EMAC_REG.SUPP = 0;                /* RMII Support Reg. speed is set to 10M */
            EMAC_REG.IPGT = 0x0012;           /* IPG setting in half duplex mode */
            break;
        
        default:
            return false;
    }

    return true;
}

/*****************************************************************************
** Function name:		MAC_TxDesciptorInit
**
** Descriptions:		initialize MAC TX descriptor table
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void MAC_TxDescriptorInit(void)
{
    /*-----------------------------------------------------------------------------      
     * setup the Tx status,descriptor registers -- 
     * Note, the actual tx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/ 
    EMAC_REG.TxDescriptor       = TX_DESC_BASE;     /* Base addr of tx descriptor array */
    EMAC_REG.TxStatus           = TX_STAT_BASE;     /* Base addr of tx status array*/
    EMAC_REG.TxDescriptorNumber = TX_DESC_NUM - 1;	/* Number of tx descriptors */

    EMAC_REG.TxProduceIndex = 0;	                /* TX descriptors point to zero */
}

void MAC_RxDescriptorInit(void)
{
    uint8_t i;
   
    /*-----------------------------------------------------------------------------      
     * setup the Rx status,descriptor registers -- 
     * Note, the actual rx packet data is loaded into the ahb2_sram16k memory as part
     * of the simulation
     *----------------------------------------------------------------------------*/ 
    EMAC_REG.RxDescriptor       = RX_DESC_BASE;	        /* Base addr of rx descriptor array */
    EMAC_REG.RxStatus           = RX_STAT_BASE;			/* Base addr of rx status */
    EMAC_REG.RxDescriptorNumber = RX_DESC_NUM - 1;	    /* Number of rx descriptors tirm */

    for (i = 0; i < RX_DESC_NUM; i++) {
        RxArray[i] = MAC_pbuf_alloc();
        RX_DESC_PACKET(i) = (uint32_t)RxArray[i]->payload;
        RX_DESC_CTRL(i)   = (uint32_t)RX_CTRL_INT | (RxArray[i]->len - 1);
    }

    EMAC_REG.RxConsumeIndex = 0;	                    /* RX descriptor points to zero */
}

/******************************************************************************
** Function name:		MAC_TxReset/MAC_RxReset
** Descriptions:		MAC TX API modules
** parameters:			None
** Returned value:		None
******************************************************************************/
void MAC_TxReset(void)
{
    emac_disable_tx();
    EMAC_REG.MAC1    |= MAC_MAC1_RESET_MCS_TX;
    EMAC_REG.Command |= MAC_COMMAND_TX_RESET;
    EMAC_REG.MAC1    &= ~MAC_MAC1_RESET_MCS_TX;
}

void MAC_RxReset(void)
{        
    uint8_t i;    

    emac_disable_rx();
    EMAC_REG.MAC1    |= MAC_MAC1_RESET_MCS_RX;
    EMAC_REG.Command |= MAC_COMMAND_RX_RESET;
    EMAC_REG.MAC1    &= ~MAC_MAC1_RESET_MCS_RX;

    for (i = 0; i < RX_DESC_NUM; i++) {
        pbuf_free(RxArray[i]);
        RxArray[i] = NULL;
    }
    MAC_RxDescriptorInit();
}

void emac_set_mac_addr(uint8_t *mac)   
{
	//把MAC地址写入Station Address寄存器中
	EMAC_REG.SA0 = ((uint16_t)mac[0] << 8) + mac[1];
    EMAC_REG.SA1 = ((uint16_t)mac[2] << 8) + mac[3];
    EMAC_REG.SA2 = ((uint16_t)mac[4] << 8) + mac[5];
}

uint8_t MAC_GetUsableDescLen(void)
{
    uint8_t produceIndex, consumeIndex;
    
    produceIndex = EMAC_REG.TxProduceIndex;
    consumeIndex = EMAC_REG.TxConsumeIndex;
    return TX_DESC_NUM - (produceIndex - consumeIndex);
}

struct pbuf *MAC_pbuf_alloc(void)
{
    struct pbuf *p;
    
    do {
        p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
        if (p != NULL) {
            break;
        }
        OSTimeDly(5);
    } while (1);
    
    return p;
}


bool mii_phy_reg_read(uint16_t phyadd, uint16_t phyreg, uint16_t *value) 
{
    uint32_t tout;
    
    EMAC_REG.MCMD = MAC_MCMD_READ;	  	
    EMAC_REG.MADR = ((phyadd & 0X1F) << 8) | (phyreg & 0X1F); //Mgmt周期的5位PHY地址 | Mgmt周期的5位寄存器地址
    
    /* Wait until operation completed */
    for (tout = 0x50000; (tout != 0) && (EMAC_REG.MIND & MAC_MIND_BUSY); tout--); //如果为真,表示MII Mgmt当前正在执行MII Mgmt读或写周期,这里防止超时
    EMAC_REG.MCMD = 0;
    if (0 == tout) {
        return false;
    }
    *value = (uint16_t)EMAC_REG.MRDD;
    
    return true;
}

bool mii_phy_reg_write(uint16_t phyadd, uint16_t phyreg, uint16_t *value)
{
    uint32_t tout;

    EMAC_REG.MCMD = MAC_MCMD_WRIT;
    EMAC_REG.MADR = ((phyadd & 0X1F) << 8) | (phyreg & 0X1F);
    EMAC_REG.MWTD = *value;

    /* Wait utill operation completed */
    for (tout = 0x50000; (tout != 0) && (EMAC_REG.MIND & MAC_MIND_BUSY); tout--);
    if (0 == tout) {
        return false;
    }

    return true;
}

/**********************************************************************
**函数原型：    MAC_RecFrameProcess
**入口参数:     无
**出口参数:		无
**返 回 值：	NULL          			没有新数据包
**              unsigned char *         接收到新数据包
**说    明：	查询(没用中断)是否有新数据包并接收进缓冲区
************************************************************************/
uint8_t MAC_RecFrameProcess(struct pbuf **rValue) 
{
	struct pbuf *pBufPkg = NULL;
	uint8_t  flag, index, bufcnt;
	uint16_t rxSize;
    
__do_again:
    if (EMAC_REG.RxConsumeIndex == EMAC_REG.RxProduceIndex) {
        return (1);
    }

    bufcnt = 0;

    //找出一个帧的最后一个片段
    index = EMAC_REG.RxConsumeIndex;
    while (!(RX_STAT_INFO(index) & RX_STAT_LAST) && (index != EMAC_REG.RxProduceIndex)) {
        index = (index + 1) % RX_DESC_NUM;
        bufcnt++;
    }
    if (!(RX_STAT_INFO(index) & RX_STAT_LAST)) {   //接收数组里没有接收到完整的一帧数据
        return (2);
    }
    bufcnt++;

    //这个帧有错误
    if (RX_STAT_INFO(index) & RX_STAT_NODESC) {
        if (EMAC_REG.RxProduceIndex == index) {
            EMAC_REG.RxConsumeIndex = index;
        } else {
            EMAC_REG.RxConsumeIndex = (index + 1) & (RX_DESC_NUM - 1);
        }
        goto __do_again;
    } else if (RX_STAT_INFO(index) & (RX_STAT_OVERRUN | RX_STAT_ALGNERR | RX_STAT_SYMERR | RX_STAT_CRCERR)){
        EMAC_REG.RxConsumeIndex = (index + 1) & (RX_DESC_NUM - 1);
        goto __do_again;
    } else if (RX_STAT_INFO(index) & (RX_STAT_RNGERR | RX_STAT_LENERR)) {//这个帧超出了帧长度的最大限制（0x600, 1536）
        //这帧数据有Q-Tag标签，或者是Ethernet II
    }
    
    *rValue = NULL;
    index   = EMAC_REG.RxConsumeIndex;
    flag    = 1;
    while (index != EMAC_REG.RxProduceIndex) {
        if (RX_STAT_INFO(index) & RX_STAT_LAST) {
            if (flag) {
                flag = 0;
    
                pBufPkg = RxArray[index];
                rxSize  = (RX_STAT_INFO(index) & RX_STAT_RX_SIZE) + 1;
                pBufPkg->tot_len = rxSize;
                pBufPkg->len     = rxSize;
    
                RxArray[index] = MAC_pbuf_alloc();
                RX_DESC_PACKET(index) = (uint32_t)RxArray[index]->payload;
                RX_DESC_CTRL(index)   = (uint32_t)RX_CTRL_INT | (RxArray[index]->len - 1);
                index = (index + 1) & (RX_DESC_NUM - 1);
            	EMAC_REG.RxConsumeIndex = index; 
            } else {
                rxSize = (RX_STAT_INFO(index) & RX_STAT_RX_SIZE) + 1; 
                RxArray[index]->len  = rxSize;
                pbuf_cat(pBufPkg, RxArray[index]);
        
                RxArray[index] = MAC_pbuf_alloc();
                RX_DESC_PACKET(index) = (uint32_t)RxArray[index]->payload;
                RX_DESC_CTRL(index)   = (uint32_t)RX_CTRL_INT | (RxArray[index]->len - 1);
                index = (index + 1) & (RX_DESC_NUM - 1);
                EMAC_REG.RxConsumeIndex = index;
            }
            break;
        }
        
        if (flag) {
            flag = 0;

            pBufPkg = RxArray[index];
            rxSize = (RX_STAT_INFO(index) & RX_STAT_RX_SIZE) + 1;
            pBufPkg->tot_len = rxSize;
            pBufPkg->len     = rxSize;

            RxArray[index] = MAC_pbuf_alloc();
            RX_DESC_PACKET(index) = (uint32_t)RxArray[index]->payload;
            RX_DESC_CTRL(index)   = (uint32_t)RX_CTRL_INT | (RxArray[index]->len - 1);
            index = (index + 1) & (RX_DESC_NUM - 1);
        	EMAC_REG.RxConsumeIndex = index; 
        } else {
            rxSize = (RX_STAT_INFO(index) & RX_STAT_RX_SIZE) + 1; 
            RxArray[index]->len  = rxSize;
            pbuf_cat(pBufPkg, RxArray[index]);

            RxArray[index] = MAC_pbuf_alloc();
            RX_DESC_PACKET(index) = (uint32_t)RxArray[index]->payload;
            RX_DESC_CTRL(index)   = (uint32_t)RX_CTRL_INT | (RxArray[index]->len - 1);
            index = (index + 1) & (RX_DESC_NUM - 1);
            EMAC_REG.RxConsumeIndex = index; 
        }
    }
    
    *rValue = pBufPkg;
    
    if (!(MAC_STATUS_RX & EMAC_REG.Status)) {
        emac_enable_rx();
    }

	return(0);
} 

void MAC_RegRxDoneCallback(void (*fun)(void))
{
    emacRxDoneCallback = fun;
}  

void MAC_RegTxDoneCallback(void (*fun)(void))
{
    emacTxDoneCallback = fun;
}


ISR(ENET_IRQHandler)
{
	osIntEnter();

    if (EMAC_REG.IntStatus & MAC_INT_RXDONE) {     //接收到一个片段
        EMAC_REG.IntClear = MAC_INT_RXDONE;

        if (RX_STAT_INFO(EMAC_REG.RxConsumeIndex) & RX_STAT_LAST) {  //如果这是一帧数据的最后一个片段
            if (emacRxDoneCallback != NULL) {
                emacRxDoneCallback();
            }
        }
    }

    if (EMAC_REG.IntStatus & MAC_INT_RXFINISHED) {     // all RX descriptor has been processed.
        EMAC_REG.IntClear = MAC_INT_RXFINISHED;
    }

    if (EMAC_REG.IntStatus & MAC_INT_RXOVERRUN) {
        EMAC_REG.IntClear = MAC_INT_RXOVERRUN;
        MAC_RxReset();
        emac_enable_rx();
    }
    
    if (EMAC_REG.IntStatus & MAC_INT_TXDONE) {
        EMAC_REG.IntClear = MAC_INT_TXDONE;
        if (emacTxDoneCallback != NULL) {
            emacTxDoneCallback();
        }
    }
    
    if (EMAC_REG.IntStatus & MAC_INT_TXFINISHED) {      // all TX descriptor has been processed.
        EMAC_REG.IntClear = MAC_INT_TXFINISHED;
        if (emacTxDoneCallback != NULL) {
            emacTxDoneCallback();
        }
    }

    if (EMAC_REG.IntStatus & MAC_INT_TXUNDERRUN) {
        EMAC_REG.IntClear = MAC_INT_TXUNDERRUN;
        MAC_TxReset();
        emac_enable_tx();
    }
	
    osIntExit();
}

