#ifndef __REG_RTC_H__
#define __REG_RTC_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define RTC_REG                         (*(rtc_reg_t *)RTC_BASE_ADDRESS)

#define RTC_CTRL_SWRESERT_BIAS          0u
#define RTC_CTRL_SWRESERT_MASK          BIT(RTC_CTRL_SWRESERT_BIAS)
#define RTC_CTRL_OFD_BIAS               1u
#define RTC_CTRL_OFD_MASK               BIT(RTC_CTRL_OFD_BIAS)
#define RTC_CTRL_ALARM1HZ_BIAS          2u
#define RTC_CTRL_ALARM1HZ_MASK          BIT(RTC_CTRL_ALARM1HZ_BIAS)
#define RTC_CTRL_WAKE1KHZ_BIAS          3u
#define RTC_CTRL_WAKE1KHZ_MASK          BIT(RTC_CTRL_WAKE1KHZ_BIAS)
#define RTC_CTRL_ALARMDPD_EN_BIAS       4u
#define RTC_CTRL_ALARMDPD_EN_MASK       BIT(RTC_CTRL_ALARMDPD_EN_BIAS)
#define RTC_CTRL_WAKEDPD_EN_BIAS        5u
#define RTC_CTRL_WAKEDPD_EN_MASK        BIT(RTC_CTRL_WAKEDPD_EN_BIAS)
#define RTC_CTRL_RTC1KHZ_EN_BIAS        6u
#define RTC_CTRL_RTC1KHZ_EN_MASK        BIT(RTC_CTRL_RTC1KHZ_EN_BIAS)
#define RTC_CTRL_RTC_EN_BIAS            7u
#define RTC_CTRL_RTC_EN_MASK            BIT(RTC_CTRL_RTC_EN_BIAS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type
//! @{
typedef volatile struct {
    reg32_t     CTRL;
    reg32_t     MATCH;
    reg32_t     COUNT;
    reg32_t     WAKE;
} rtc_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
