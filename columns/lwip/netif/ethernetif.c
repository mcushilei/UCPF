/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */
#include "opt.h"


#include "def.h"
#include "mem.h"
#include "pbuf.h"
#include "stats.h"
#include "snmp.h"
#include "etharp.h"
//#include "ppp_oe.h"
#include "sys_arch.h"
#include "sys.h"
#include "ethernetif.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

OS_STK ETHERNETIF_INPUT_STK[64];

static sys_sem_t TxDone;
static sys_sem_t RxDone;

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
static void         low_level_init(struct netif *pnetif);
static struct pbuf *low_level_input(struct netif *pnetif);
static err_t        low_level_output(struct netif *pnetif, struct pbuf *p);
static err_t        ethernetif_output(struct netif *pnetif, struct pbuf *pbuf, ip_addr_t *pIPAddr);
static void         ethernetif_TxDoneCallback(void);
static void         ethernetif_RxDoneCallback(void);

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *pnetif)
{
    struct ethernetif *pethernetif;
    
    LWIP_ASSERT("pnetif != NULL", (pnetif != NULL));
    
    pethernetif = mem_malloc(sizeof(struct ethernetif));
    if (pethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }
    
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    pnetif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */
    
    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    NETIF_INIT_SNMP(pnetif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);
    
    pnetif->state = pethernetif;
    pnetif->name[0] = IFNAME0;
    pnetif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
    * You can instead declare your own function an call etharp_output()
    * from it if you have to do some checks before sending (e.g. if link
    * is available...) */
    pnetif->output = ethernetif_output;
    pnetif->linkoutput = low_level_output;
    
    pethernetif->ethaddr = (struct eth_addr *)&(pnetif->hwaddr[0]);
    
    /* initialize the hardware */
    low_level_init(pnetif);
    etharp_init();
    
    return ERR_OK;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(void *p)
{
    struct netif      *pnetif;
    struct eth_hdr    *ethhdr;
    struct pbuf       *pbuf;

    pnetif = (struct netif *)p;
    
    while (1) {
        /* move received packet into a new pbuf */
        do {
            pbuf = low_level_input(pnetif);
            if (pbuf == NULL) {
                sys_arch_sem_wait(&RxDone, 0);
            }
        } while (pbuf == NULL);

        /* points to packet payload, which starts with an Ethernet header */
        ethhdr = (struct eth_hdr *)pbuf->payload;
        
        switch (htons(ethhdr->type)) {
            case ETHTYPE_IP:
            case ETHTYPE_ARP:
                //ethernet_input(pbuf, pnetif);
                /* full packet send to tcpip_thread to process */
                if (pnetif->input(pbuf, pnetif) != ERR_OK) { 
                    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                    pbuf_free(pbuf);
                    pbuf = NULL;
                }
                break;

#if PPPOE_SUPPORT 
            case ETHTYPE_PPPOEDISC:
            case ETHTYPE_PPPOE:
#endif
                            
            default:
                pbuf_free(pbuf);
                pbuf = NULL;
                break;
        }
    }
}

err_t ethernetif_output(struct netif *pnetif, struct pbuf *pbuf, ip_addr_t *pIPAddr)
{    
    return etharp_output(pnetif, pbuf, pIPAddr);     // 解析硬件地址，然后发送或排队信息包
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void low_level_init(struct netif *pnetif)
{
    u8_t ucOldPrio;
    u8_t ucErr;
    
    /* set MAC hardware address length */
    pnetif->hwaddr_len = ETHARP_HWADDR_LEN;
    
    /* set MAC hardware address */
    pnetif->hwaddr[0] = SettingInf->Mcu_Mac[0];
    pnetif->hwaddr[1] = SettingInf->Mcu_Mac[1];
    pnetif->hwaddr[2] = SettingInf->Mcu_Mac[2];
    pnetif->hwaddr[3] = SettingInf->Mcu_Mac[3];
    pnetif->hwaddr[4] = SettingInf->Mcu_Mac[4];
    pnetif->hwaddr[5] = SettingInf->Mcu_Mac[5];
    
    /* maximum transfer unit */
    pnetif->mtu = 1500;
    
    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    pnetif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
    
    /* Do whatever else is needed to initialize interface. */ 
    
    ucErr = sys_sem_new(&TxDone, 1);
    ucErr = sys_sem_new(&RxDone, 0);
    
    MAC_RegTxDoneCallback(ethernetif_TxDoneCallback);
    MAC_RegRxDoneCallback(ethernetif_RxDoneCallback);
    
    emac_init();

    OSTaskCreate(
                ethernetif_input,
                pnetif,
                &ETHERNETIF_INPUT_STK[64 - 1],
                LWIP_TASK_START_PRIO - 1
                );
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
struct pbuf *low_level_input(struct netif *pnetif)
{
    struct pbuf *p = NULL;
    static sys_mutex_t lock = NULL;

    if (lock == NULL) {
		sys_mutex_new(&lock);
    }

    sys_mutex_lock(&lock);
    MAC_RecFrameProcess(&p);
    sys_mutex_unlock(&lock);

    return (p);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

err_t low_level_output(struct netif *pnetif, struct pbuf *p)
{
    static sys_mutex_t lock = NULL;
    struct pbuf *pCurrentBuf;
    INT8U  pbufChainLen, usableDescLen;
    INT8U  i, index, counter, En_Int = 0;
      
    SYS_ARCH_DECL_PROTECT(lev);
    
    /* 注意：不要改动p里的内容, 更不能释放p */
    //如果pbuf没有使用引用（引用标志未置1），则所有数据都在一个pbuf中，否则可能有多个pbuf
    pbufChainLen = pbuf_clen(p);
    do {
        usableDescLen = MAC_GetUsableDescLen();
        if (usableDescLen < pbufChainLen) {
            sys_arch_sem_wait(&TxDone, 0);      //pend for space.
            En_Int = 1;
        } else {
            break;
        }
    } while (1);
    
    if (lock == NULL) {
		sys_mutex_new(&lock);
    }
    sys_mutex_lock(&lock);
    
    SYS_ARCH_PROTECT(lev);
    

    // 每层数据包作为一个片段
	pCurrentBuf = p;
    counter     = pbufChainLen - 1;
	index = EMAC_REG.TxProduceIndex;
    for (i = 0; i < counter; i++) {
        TX_DESC_PACKET(index) = (INT32U)pCurrentBuf->payload;    
		TX_DESC_CTRL(index)   = (INT32U)pCurrentBuf->len - 1; /* set TX descriptor control field */
		pCurrentBuf = pCurrentBuf->next;  /* 指向上层数据包 */
        index++;
        if (index >= TX_DESC_NUM) {
            index = 0;
        }
	}
           
    // 最后一个片段
    TX_DESC_PACKET(index) = (INT32U)pCurrentBuf->payload; 
    if (En_Int) {
    	TX_DESC_CTRL(index) = (INT32U)(TX_CTRL_INT | TX_CTRL_LAST | (pCurrentBuf->len - 1)); /* set TX descriptor control field */
    } else {
    	TX_DESC_CTRL(index) = (INT32U)(TX_CTRL_LAST | (pCurrentBuf->len - 1)); /* set TX descriptor control field */
    }
    pCurrentBuf = pCurrentBuf->next;  /* 指向上层数据包 */
    index++;
    if (index >= TX_DESC_NUM) {
        index = 0;
    }
    
    // 开始发送
    EMAC_REG.IntClear = 0xf0;
    EMAC_REG.TxProduceIndex = index;	// 更新发送产生寄存器后发送开始

    SYS_ARCH_UNPROTECT(lev);
    
    sys_mutex_unlock(&lock);    

    return ERR_OK;
}

void ethernetif_TxDoneCallback(void)
{
    sys_sem_signal(&TxDone);
}

void ethernetif_RxDoneCallback(void)
{
    sys_sem_signal(&RxDone);
}
