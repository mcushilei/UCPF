
#ifndef __AT_PLUG_H__
#define __AT_PLUG_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void    *at_malloc(size_t size);
extern void     at_free(void* ptr);
extern uint32_t at_get_time(void);

extern bool     at_usart_init(void);
extern void     at_usart_deinit(void);
extern void     at_transmit(const char *cmd, int32_t len, bool with_line_end);
extern int      at_read_resp(char *buf, uint32_t ori, uint32_t end);


#endif  //!< #ifndef __AT_PLUG_H__
/* EOF */
