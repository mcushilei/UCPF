
#ifndef __LPC12XX_FLASH_H__
#define __LPC12XX_FLASH_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern uint32_t flash_write(uint32_t wDstAddr, uint32_t wSrcAddr, uint32_t wLen);
extern uint32_t IAP_ReadUID(uint32_t *pu32byte0, uint32_t *pu32byte1,
                            uint32_t *pu32byte2, uint32_t *pu32byte3);
extern uint32_t IAP_ReadPartID(uint32_t *pu32PartID);

#endif
/* EOF */
