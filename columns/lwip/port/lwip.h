#ifndef __LW_IP_H
#define __LW_IP_H


/* Includes ------------------------------------------------------------------*/
#include "memp.h"
#include "LwIP.h"
#include "tcp.h"
#include "udp.h"
#include "tcpip.h"
#include "etharp.h"
#include "dhcp.h"
#include "ethernetif.h"
#include "sys_arch.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void Init_LwIP(void);
void Display_IPAddress(void);



/*
#error section
-- The standard C preprocessor directive #error should be used to notify the
programmer when #define constants or macros are not present and to indicate
that a #define value is out of range. These statements are normally found in
a module's .H file. The #error directive will display the message within the
double quotes when the condition is not met.
*/


#endif /* #ifndef __LW_IP_H */
/*-- File end --*/
