#ifndef __WDT_H__
#define __WDT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name watchdog initialization arguments defination
//! @{
typedef enum {
    //! A watchdog time-out will not cause reset
    WDT_CASE_INT        = 0x00,
    //! A watchdog time-out will cause reset
    WDT_CASE_RESET      = 0x02,    
    
    //! change time-out value(TC) at any time
    WDT_FEED_ANYTIME    = 0x00,
    //! change time-out value(TC) only after counter is below WDWARNINT and WDWINDOW
    WDT_FEED_WINDOW     = 0x10,

    //! change watchdog oscillator at any time
    WDT_CLK_IS_NOT_LOCKED   = 0x00,
    //! watchdog oscillator is locked
    WDT_CLK_IS_LOCKED       = 0x20,
    
    WDT_CLK_SEL_IRC         = 0x00,
    WDT_CLK_SEL_WDT_OSC     = 0x01,
} em_wdt_cfg_mode_t;
//! @}


//! \name wdt config struct
//! @{
typedef struct {
    uint32_t    wTCCount;       //!< Watchdog time-out value.
    uint32_t    wWarnIntCmpValue;       //!< Watchdog warning interrupt compare value.
    uint32_t    wWindowCmpValue;       //!< Watchdog window value.
    uint32_t    chMode;                          //!< Watchdog config ward
    uint8_t     chCLKSEL;
} wdt_cfg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern bool watchdog_disable(void);
extern bool watchdog_enable(void);
extern bool watchdog_init(void);
extern void watchdog_feed(void);
extern uint32_t watchdog_get_count_value(void);

extern void wdt_osc_stop(void);
extern void wdt_osc_run(void);
extern bool wdt_osc_cfg(uint8_t chDIV, uint8_t chFRQ);

#endif
