
#ifndef __WDT_H__
#define __WDT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define WDT_CFG(...)    do{                     \
        wdt_cfg_t tCfg = {__VA_ARGS__};         \
        wdt_config(&tCfg);                    \
    } while (0)

/*============================ TYPES =========================================*/
//! \name watchdog initialization arguments defination
//! @{
enum {
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
};
//! @}


//! \name wdt config struct
//! @{
typedef struct {
    uint8_t     chMode;                 //!< Watchdog config ward
    uint8_t     chClockSource;
    uint32_t    wReload;               //!< Watchdog time-out value.
    uint32_t    wWarnIntCmpValue;       //!< Watchdog warning interrupt compare value.
    uint32_t    wWindowCmpValue;        //!< Watchdog window value.
} wdt_cfg_t;
//! @}


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     wdt_config(wdt_cfg_t *tCfg);
extern bool     wdt_disable(void);
extern bool     wdt_enable(void);
extern bool     wdt_start(void);
extern bool     wdt_stop(void);
extern void     wdt_feed(void);
extern uint32_t wdt_get_count_value(void);
extern void     wdt_osc_disable(void);
extern void     wdt_osc_enable(void);
extern bool     wdt_osc_config(uint8_t chDIV, uint8_t chFRQ);
extern void     wdt_clear_int_flag(void);

#endif
