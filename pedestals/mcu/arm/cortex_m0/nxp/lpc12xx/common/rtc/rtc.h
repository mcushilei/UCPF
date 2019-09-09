
#ifndef __RTC_H__
#define __RTC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_rtc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define RTC_CFG(...)    do{                 \
        rtc_cfg_t tCfg = { __VA_ARGS__ };   \
        rtc_config(&tCfg);                    \
    } while(false)

/*============================ TYPES =========================================*/
//! \name RTC initialization arguments defination
//! @{
enum {
    RTC_STOP                = 0x00,     //! Enable the RTC
    RTC_START               = 0x01,     //! Enable the RTC
    RTC_SRC_32KOSC_1HZ      = 0x00,             //! Select 1HZ 32KOSC output
    RTC_SRC_32KOSC_1KHZ     = 0x0Au << 11,      //! Select 1KHz 32KOSC output
    RTC_SRC_RTC_PCLK        = 0x04u << 11,      //! Select RTC PLCK clock source
    RTC_INTERRUPT_DISABLE   = 0x00,     //! Disable the RTC interrupt
    RTC_INTERRUPT_ENABLE    = 0x80,     //! Enable the RTC interrupt
};
//! @}

//! \name RTC config struct
//! @{
typedef struct {
    uint32_t        chMode;
    uint32_t        wMatchValue;
}rtc_cfg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     rtc_config(rtc_cfg_t *tCfg);
extern bool     rtc_enable(void);
extern bool     rtc_disable(void);
extern void     rtc_clear_int_flag(void);
extern bool     rtc_get_int_flag(void);
extern uint32_t rtc_get_time_value(void);
extern bool     rtc_set_time_value(uint32_t wValue);
extern bool     rtc_set_match_value(uint32_t wValue);
extern uint32_t rtc_get_match_value(void);

#endif
