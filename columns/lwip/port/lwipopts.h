#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/* NO_SYS==1: Provides VERY minimal functionality. Otherwise, use lwIP facilities.*/
#define NO_SYS                  0

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    1

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4
/* MEMP_NUM_ARP_QUEUE: the number of simulateously queued outgoing
 * packets (pbufs) that are waiting for an ARP request (to resolve
 * their destination address) to finish.
 * (requires the ARP_QUEUEING option)*/
#define MEMP_NUM_ARP_QUEUE      14
/* MEM_SIZE: the size of the heap memory. If the application will send
   a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                (12 * 1024)
/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           10
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
 * per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        0
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections. */
#define MEMP_NUM_TCP_PCB        16
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 14
#define MEMP_OVERFLOW_CHECK     2
/* MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)*/
#define MEMP_NUM_NETBUF         14
/* MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)*/
#define MEMP_NUM_NETCONN        (MEMP_NUM_TCP_PCB)


/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          26
/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       256


/* ---------- ARP options ---------- */
#define LWIP_ARP                1
#define ETHARP_TRUST_IP_MAC     1


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of interfaces. 
   DHCP is not implemented in lwIP 0.5.1, however, so turning this 
   on does currently not work. */
#define LWIP_DHCP               0


/* ---------- ICMP options ---------- */
#define LWIP_ICMP               1


/* ---------- TCP options ---------- */
/* TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
   you might want to increase this.)*/
#define TCP_MSS                 190
#define TCP_MAXRTX              3
#define TCP_SYNMAXRTX           3
#define TCP_LISTEN_BACKLOG      1
#define TCP_DEFAULT_LISTEN_BACKLOG      0x01
/* TCP_QUEUE_OOSEQ==1: TCP will queue segments that arrive out of order.
 * Define to 0 if your device is low on memory.*/
#define TCP_QUEUE_OOSEQ         0
/* TCP_WND: The size of a TCP window.  This must be at least 
 * (2 * TCP_MSS) for things to work well */
#define TCP_WND                 (2 * TCP_MSS)
/* TCP_SND_BUF: TCP sender buffer space (bytes).
 * To achieve good performance, this should be at least 2 * TCP_MSS.*/
#define TCP_SND_BUF             (2 * TCP_MSS)
/* LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.*/
#define LWIP_SO_RCVTIMEO        0
/* LWIP_TCP_KEEPALIVE==1: Enable TCP_KEEPIDLE, TCP_KEEPINTVL and TCP_KEEPCNT
 * options processing. Note that TCP_KEEPIDLE and TCP_KEEPINTVL have to be set
 * in seconds. (does not require sockets.c, and will affect tcp.c)*/
#define LWIP_TCP_KEEPALIVE      1

#if LWIP_TCP_KEEPALIVE
#define  TCP_KEEPIDLE_DEFAULT     20000UL   /* Default KEEPALIVE timer in milliseconds */
#define  TCP_KEEPINTVL_DEFAULT    5000UL   /* Default Time between KEEPALIVE probes in milliseconds */
#define  TCP_KEEPCNT_DEFAULT      3U    /* Default Counter for KEEPALIVE probes */
#endif /* LWIP_TCP_KEEPALIVE */


/* ---------- UDP options ---------- */
#define LWIP_UDP                0


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/* LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)*/
#define LWIP_NETCONN                    1


/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     0
#define LWIP_RAW                        0


/* ---------- Statistics options ---------- */
#define LWIP_STATS                      0



/*
   ------------------------------------
   ---------- Debug options ----------
   ------------------------------------
*/
/*不显示调试信息*/
#define LWIP_NOASSERT
//#define LWIP_DEBUG
                     
//#define LWIP_PROVIDE_ERRNO              1
//#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL   
//#define LWIP_DBG_TYPES_ON               (LWIP_DBG_ON | LWIP_DBG_STATE | LWIP_DBG_HALT)   //| LWIP_DBG_TRACE 
//#define ETHARP_DEBUG                    LWIP_DBG_ON
//#define NETIF_DEBUG                     LWIP_DBG_ON
//#define PBUF_DEBUG                      LWIP_DBG_ON
//#define MEM_DEBUG                       LWIP_DBG_ON
//#define MEMP_DEBUG                      LWIP_DBG_ON
//#define TCP_DEBUG                       LWIP_DBG_ON
//#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
//#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
//#define TCP_RTO_DEBUG                   LWIP_DBG_ON
//#define TCP_QLEN_DEBUG                  LWIP_DBG_ON
//#define API_LIB_DEBUG                   LWIP_DBG_ON
//#define API_MSG_DEBUG                   LWIP_DBG_ON

#endif /* __LWIPOPTS_H__ */

