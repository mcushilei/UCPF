#include "mem.h"
#include "pbuf.h"
#include "usermem.h"


struct pbuf *MAC_MemAlloc(INT8U n)
{
    struct pbuf *p; 
 
    p = pbuf_alloc(PBUF_RAW, n * PBUF_POOL_BUFSIZE, PBUF_POOL);
    if (p == NULL) {
#if LINK_STATS
		lwip_stats.link.memerr++;
		lwip_stats.link.drop++;
#endif
        return (NULL);
    }
    
    return (p); 
}



