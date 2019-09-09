#ifndef __RTC_H__
#define __RTC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_rtc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     rtc_enable(void);
extern bool     rtc_disable(void);
extern bool     rtc_start(uint32_t InitValue);
extern bool     rtc_stop(void);
extern uint32_t rtc_get_time_value(void);
extern bool     rtc_set_time_value(uint32_t Value);
extern uint32_t rtc_get_match_value(void);
extern bool     rtc_set_match_value(uint32_t Value);
extern bool     rtc_alarm_enable(void);
extern bool     rtc_alarm_disable(void);
extern bool     rtc_alarm_clear_intflag(void);

#endif
