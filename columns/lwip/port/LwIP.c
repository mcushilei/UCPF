 
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_DHCP_TRIES        4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct netif LWIP_netif;
static uint32_t IPaddress = 0;

/* Private function prototypes -----------------------------------------------*/
static void TcpipInitDone(void *arg);

/* Private functions ---------------------------------------------------------*/

/** 
 * @brief TcpipInitDone wait for tcpip init being done
 * 
 * @param arg the semaphore to be signaled
 */
static void TcpipInitDone(void *arg)
{
    sys_sem_t *sem;
    sem = arg;
    sys_sem_signal(sem);
}

/** 
 * @brief Display_IPAddress Display IP Address
 * 
 */
void Display_IPAddress(void)
{
    if (IPaddress != LWIP_netif.ip_addr.addr)
    {
        /* IP 地址发生改变*/
        volatile uint8_t iptab[4];
        
        /* read the new IP address */
        IPaddress = LWIP_netif.ip_addr.addr;
        
        iptab[0] = (uint8_t)(IPaddress >> 24);
        iptab[1] = (uint8_t)(IPaddress >> 16);
        iptab[2] = (uint8_t)(IPaddress >> 8);
        iptab[3] = (uint8_t)(IPaddress);
    }
#if LWIP_DHCP
    else if (IPaddress == 0)
    {   
        // 等待DHCP分配IP
        /* If no response from a DHCP server for MAX_DHCP_TRIES times */
        /* stop the dhcp client and set a static IP address */
        if(LWIP_netif.dhcp->tries > MAX_DHCP_TRIES) 
        {   /* 超出DHCP重试次数，改用静态IP设置 */
            struct ip_addr ipaddr;
            struct ip_addr netmask;
            struct ip_addr gw;
            
            dhcp_stop(&LWIP_netif);
            
            IP4_ADDR(&ipaddr, 10, 21, 11, 245);
            IP4_ADDR(&netmask, 255, 255, 255, 0);
            IP4_ADDR(&gw, 10, 21, 11, 254);
            
            netif_set_addr(&LWIP_netif, &ipaddr , &netmask, &gw);
        }
    }
#endif
}


/**
* @brief Init_LwIP initialize the LwIP
*/
void Init_LwIP(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    sys_sem_t sem;


    /* 初始化LwIP */
    sys_init();
    mem_init();       /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
    memp_init();      /* Initializes the memory pools defined by MEMP_NUM_x.*/
    pbuf_init();
    netif_init();
    
    sys_sem_new(&sem, 0);
    tcpip_init(TcpipInitDone, &sem);
    sys_sem_wait(&sem);
    sys_sem_free(&sem);

    
    /* 初始化网络设置 */
#if LWIP_DHCP    
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else            
    IP4_ADDR(&ipaddr, SettingInf->Mcu_Ip[0], SettingInf->Mcu_Ip[1], SettingInf->Mcu_Ip[2], SettingInf->Mcu_Ip[3]);
    IP4_ADDR(&netmask, SettingInf->Ip_Mask[0], SettingInf->Ip_Mask[1], SettingInf->Ip_Mask[2], SettingInf->Ip_Mask[3]);       
    IP4_ADDR(&gw, SettingInf->Gateway_Ip[0], SettingInf->Gateway_Ip[1], SettingInf->Gateway_Ip[2], SettingInf->Gateway_Ip[3]);
#endif

    

    /*初始化网卡硬件*/
    netif_add(&LWIP_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
    netif_set_default(&LWIP_netif);
#if LWIP_DHCP
    dhcp_start(&LWIP_netif);
#endif
    netif_set_up(&LWIP_netif);
}

