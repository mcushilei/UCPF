#ifndef __I_IO_RTC_H__
#define __I_IO_RTC_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define RTC_REG                         (*(volatile rtc_reg_t *)RTC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type
//! @{
typedef struct {
    reg32_t     DATA;
    reg32_t     MATCH;
    reg32_t     LOAD;

    DEF_REG32
        reg32_t RTCSTART                : 1;
        reg32_t                         :31;
    END_DEF_REG32(CR) 

    DEF_REG32
        reg32_t  RTCIC                  : 1;
        reg32_t                         :31;
    END_DEF_REG32(IMSC)

    DEF_REG32
        reg32_t  RTCRIS                 : 1;
        reg32_t                         :31;
    END_DEF_REG32(RIS)

    DEF_REG32
        reg32_t  RTCMIS                 : 1;
        reg32_t                         :31;
    END_DEF_REG32(MIS)

    DEF_REG32
        reg32_t  RTCICR                 : 1;
        reg32_t                         :31;
    END_DEF_REG32(ICR)
} rtc_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
