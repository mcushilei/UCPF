
/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_wdt.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus = PM_AHB_CLK_GET_STATUS(AHBCLK_WDT);              \
        PM_AHB_CLK_ENABLE(AHBCLK_WDT);                                     \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME(AHBCLK_WDT, wStatus);                            \
    }

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

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
void wdt_osc_disable(void)
{
    power_config_disable(POWER_WDTOSC);
}

void wdt_osc_enable(void)
{
    power_config_enable(POWER_WDTOSC);
}

bool wdt_osc_config(uint8_t chDIV, uint8_t chFRQ)
{
    SYSCON_REG.WDTOSCCTRL.DIVSEL    = chDIV;
    SYSCON_REG.WDTOSCCTRL.FREQSEL   = chFRQ;
    
    return true;
}

bool wdt_start(void)
{
    SAFE_CLK_CODE (
        WDT_REG.MOD.Value |= WDT_MOD_WDEN_MSK;
        SAFE_ATOM_CODE (
            WDT_REG.FEED = 0x000000AA;
            WDT_REG.FEED = 0x00000055;
        )
    )
    
    return true;
}

bool wdt_stop(void)
{
    SAFE_CLK_CODE (
        WDT_REG.MOD.Value = WDT_REG.MOD.Value & (~(WDT_MOD_WDEN_MSK | WDT_MOD_WDTOF_MSK));
        SAFE_ATOM_CODE (
            WDT_REG.FEED = 0x000000AA;
            WDT_REG.FEED = 0x00000055;
        )
    )
        
    return true;
}

/*! \brief init watchdog
 *! \param wdt_cfg_t*
 *! \retval true : succeed
 *! \retval false: failed
 */
bool wdt_config(wdt_cfg_t *tCfg)
{
    SAFE_CLK_CODE(
        WDT_REG.MOD.Value     = tCfg->chMode;
        WDT_REG.CLKSEL.Value  = tCfg->chClockSource;
        WDT_REG.TC.Value      = tCfg->wReload;
        WDT_REG.WARNINT.Value = tCfg->wWarnIntCmpValue;
        WDT_REG.WINDOW.Value  = tCfg->wWindowCmpValue;
    )
    
    return true;
}

/*!\brief ensable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool wdt_enable(void)
{
    PM_AHB_CLK_ENABLE(AHBCLK_WDT);
    
    return true;
}

/*! \brief disable watchdog
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool wdt_disable(void)
{
    PM_AHB_CLK_DISABLE(AHBCLK_WDT);
    
    return true;
}

/*! \brief feed watchdog
 *! \param void
 *! \retval none
 */
void wdt_feed(void)
{
    SAFE_CLK_CODE(
        SAFE_ATOM_CODE (
            WDT_REG.FEED = 0x000000AA;
            WDT_REG.FEED = 0x00000055;
        )
    )
}

void wdt_clear_int_flag(void)
{
    SAFE_CLK_CODE(
        WDT_REG.MOD.Value = WDT_REG.MOD.Value
                        & (~(1u << 2))      //!< clear WDTOF
                        | (1u << 3);        //!< clear WDINT
    )
}

/*! \brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t wdt_get_count_value(void)
{
    uint32_t wTmp = 0;
    
    SAFE_CLK_CODE(
        wTmp = WDT_REG.TV.COUNT;
    )
        
    return wTmp;
}

