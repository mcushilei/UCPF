/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus = PM_AHB_CLK_GET_STATUS(AHBCLK_WWDT);              \
        PM_AHB_CLK_ENABLE(AHBCLK_WWDT);                                     \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME(AHBCLK_WWDT, wStatus);                            \
    }

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

//! \name wdt struct
//! @{
typedef struct {
    bool        (*Init)(void);         //!< initialize the wdt
    bool        (*Enable)(void);                    //!< enable the watchdog
    bool        (*Disable)(void);                   //!< disable the watchdog
    void        (*Feed)(void);                     //!< reset the watchdog
    uint32_t    (*GetCountValue)(void);             //!< get the time count value
} wdt_t;
//! @}

/*============================ PROTOTYPES ====================================*/
extern bool watchdog_disable(void);
extern bool watchdog_enable(void);
extern bool watchdog_init(void);
extern bool watchdog_config(wdt_cfg_t *tCfg);
extern void watchdog_feed(void);
extern uint32_t watchdog_get_count_value(void);
extern void wdt_osc_stop(void);
extern void wdt_osc_run(void);
extern bool wdt_osc_cfg(uint8_t chDIV, uint8_t chFRQ);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
void wdt_osc_stop(void)
{
    power_disable(POWER_WDTOSC);
}

void wdt_osc_run(void)
{
    power_enable(POWER_WDTOSC);
}

bool wdt_osc_init(void)
{
    SYSCON_REG.WDTOSCCTRL = (0 << 0) | (1 << 5);
    
    return true;
}

/*! \brief disable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_disable(void)
{
    return false;
}

/*!\brief ensable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_enable(void)
{
    SAFE_CLK_CODE(
        SAFE_ATOM_CODE (
            PM_AHB_CLK_ENABLE(AHBCLK_WWDT);
            WWDT_REG.MOD |= 1u << 0;
            WWDT_REG.FEED = 0x000000AA;
            WWDT_REG.FEED = 0x00000055;
            PM_AHB_CLK_DISABLE(AHBCLK_WWDT);
        )
    )
        
    return true;
}

/*! \brief init watchdog
 *! \param wdt_cfg_t*
 *! \retval true : succeed
 *! \retval false: failed
 */
bool watchdog_init(void)
{
    wdt_cfg_t tWDTCfg;
#ifdef __DEBUG__
    tWDTCfg.chMode   = WDT_FEED_ANYTIME | WDT_CASE_INT;
#else
    tWDTCfg.chMode   = WDT_FEED_ANYTIME | WDT_CASE_RESET;
#endif
    tWDTCfg.chCLKSEL = WDT_CLK_SEL_WDT_OSC; //WDT_CLK_SEL_IRC
    tWDTCfg.wTCCount = 1000000;    // 1s
    tWDTCfg.wWarnIntCmpValue    = 0;
    tWDTCfg.wWindowCmpValue     = 0xFFFFFF;
    
    return watchdog_config(&tWDTCfg);
}

static bool watchdog_config(wdt_cfg_t *tCfg)
{
    if ((tCfg->wWindowCmpValue > 0x00FFFFFFu) || (tCfg->wWindowCmpValue < 0x100u)) {
        return false;
    }
    
    if (tCfg->wWarnIntCmpValue > 0x3FFu) {
        return false;
    }
    
    if (tCfg->chCLKSEL) {
        wdt_osc_cfg(0, 7);
        wdt_osc_run();
    } else {
        power_enable(POWER_IRC);
    }
    
    SAFE_CLK_CODE(
        WWDT_REG.MOD     = tCfg->chMode;
        WWDT_REG.TC      = tCfg->wTCCount;
        WWDT_REG.CLKSEL  = tCfg->chCLKSEL;
        WWDT_REG.WARNINT = tCfg->wWarnIntCmpValue;
        WWDT_REG.WINDOW  = tCfg->wWindowCmpValue;
    )
    
    return true;
}

/*! \brief feed watchdog
 *! \param void
 *! \retval none
 */
void watchdog_feed(void)
{
    SAFE_CLK_CODE(
        SAFE_ATOM_CODE (
            WWDT_REG.FEED = 0x000000AA;
            WWDT_REG.FEED = 0x00000055;
        )
    )
}

/*! \brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t watchdog_get_count_value(void)
{
    uint32_t wTmp = 0;
    
    SAFE_CLK_CODE(
        wTmp = WWDT_REG.TV;
    )
        
    return wTmp;
}

