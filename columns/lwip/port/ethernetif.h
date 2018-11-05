#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "err.h"
#include "netif.h"

extern void ethernetif_input(void *p);
extern err_t ethernetif_init(struct netif *pnetif);

#endif 
