#ifndef __CMC_H__
#define __CMC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_scon.h"
#include ".\reg_sleep.h"

/*============================ MACROS ========================================*/
#define DIV_(_N,_D)     DIV_##_N = (_N),

#define ON_SLEEP_TURN_ON(__MSK)             (__MSK)
#define ON_SLEEP_TURN_OFF(__MSK)            (~(__MSK))
#define ON_WAKE_UP_TURN_ON(__MSK)           (__MSK)
#define ON_WAKE_UP_TURN_OFF(__MSK)          (~(__MSK))
#define ON_SEMI_WAKE_UP_TURN_ON(__MSK)      (__MSK)
#define ON_SEMI_WAKE_UP_TURN_OFF(__MSK)     (~(__MSK))
#define ON_ZOMBIE_TURN_ON(__MSK)            (__MSK)
#define ON_ZOMBIE_TURN_OFF(__MSK)           (~(__MSK))
#define ON_SLEEP_WALK_TURN_ON(__MSK)        (__MSK)
#define ON_SLEEP_WALK_TURN_OFF(__MSK)       (~(__MSK))



/*============================ MACROFIED FUNCTIONS ===========================*/
#define PM_MAIN_CLK_GET()                   main_clock_get()
#define PM_CORE_CLK_GET()                   core_clock_get()
#define PM_AHB_CLK_GET_STATUS(__INDEX)      ahb_clk_get_status((__INDEX))
#define PM_AHB_CLK_ENABLE(__INDEX)          ahb_clk_enable((__INDEX))
#define PM_AHB_CLK_DISABLE(__INDEX)         ahb_clk_disable((__INDEX))
#define PM_AHB_CLK_RESUME(__INDEX, __STATUS) ahb_clk_resume_status((__INDEX), (__STATUS))
#define PM_PCLK_CFG(__INDEX, __DIV)         peripheral_clk_config((__INDEX), (__DIV))
#define PM_PCLK_GET(__INDEX)                peripheral_clk_get((__INDEX))
#define PM_PCLK_GET_STATUS(__INDEX)         peripheral_clk_get_div((__INDEX))
#define PM_PCLK_RESUME(__INDEX, __STATUS)   peripheral_clk_config((__INDEX), (__STATUS))

/*============================ TYPES =========================================*/

//! \name prescaler
//! @{
typedef enum {
    MREPEAT(256, DIV_, 0)
} em_pm_divider_t;
//! @}

//! \name pll post divider
//! @{
typedef enum {
    PLL_POST_DIV_1          = 0x00,     //!< pll post divider rate is 1
    PLL_POST_DIV_2          = 0x01,     //!< pll post divider rate is 2
    PLL_POST_DIV_4          = 0x02,     //!< pll post divider rate is 4
    PLL_POST_DIV_8          = 0x03,     //!< pll post divider rate is 8
} em_pll_post_div_t;
//! @}

//! \name clock source
//! @{
typedef enum {
    PLL_CLKSRC_IRC          = 0x0,      //!< pll source clk is IRC
    PLL_CLKSRC_SYSOSC       = 0x1,      //!< pll source clk is System oscilator
} em_pll_clk_src_t;

typedef enum {
    MAIN_CLKSRC_IRC         = 0x0,      //!< Maniclk source is IRC
    MAIN_CLKSRC_PLLIN       = 0x1,      //!< Maniclk source is PLLIN
    MAIN_CLKSRC_WDOSC       = 0x2,      //!< Maniclk source is Watchdog OSC
    MAIN_CLKSRC_PLLOUT      = 0x3,      //!< Maniclk source is PLLOUT
} em_main_clk_src_t;

typedef enum {
    CLKOUT_CLKSRC_IRC       = 0x0,      //!< Clockout source is IRC
    CLKOUT_CLKSRC_SYSOSC    = 0x1,      //!< Clockout source is System OSC
    CLKOUT_CLKSRC_WDOSC     = 0x2,      //!< Clockout source is Watchdog OSC
    CLKOUT_CLKSRC_MCLK      = 0x3,      //!< Clockout source is Main Clock
} em_out_clk_src_t;
//! @}

//! \name pll struct type
//! @{
DEF_INTERFACE(i_pll_t)
    bool                        (*Enable)(void);
    bool                        (*Disable)(void);
    bool                        (*SelectSource)(em_pll_clk_src_t tSrc);
    bool                        (*Config)(uint32_t wMsel, uint32_t wPsel);
    uint32_t                    (*GetClockOut)(void);
    uint32_t                    (*GetClockIn)(void);
END_DEF_INTERFACE(i_pll_t)
//! @}

//! \name main clock struct type
//! @{
DEF_INTERFACE(i_core_clk_t)
    bool                        (*Config)(em_main_clk_src_t tSrc, uint32_t wDiv);
    uint32_t                    (*Get)(void);
END_DEF_INTERFACE(i_core_clk_t)
//! @}

//! \name AHB Clock Management
//! @{
DEF_INTERFACE(i_ahb_clk_t)
    bool                        (*Enable)(em_ahb_clk_t tIndex);
    bool                        (*Disable)(em_ahb_clk_t tIndex);
    uint32_t                    (*GetStatus)(em_ahb_clk_t tIndex);
    bool                        (*Resume)(em_ahb_clk_t tIndex, uint32_t tStatus);
END_DEF_INTERFACE(i_ahb_clk_t)
//! @}

//! \name pclk
//! @{
DEF_INTERFACE(i_pclk_t)
    bool                        (*Config)(uint8_t chIndex , uint8_t chDiv);
    uint32_t                    (*GetClock)(uint8_t chIndex);
    uint32_t                    (*GetStatus)(uint8_t chIndex);
    bool                        (*Resume)(uint8_t chIndex , uint8_t chDiv);
END_DEF_INTERFACE(i_pclk_t)
//! @}

//! \name pclk
//! @{
DEF_INTERFACE(i_clk_t)
    i_pll_t                     PLL;
    i_core_clk_t                Core;
    i_ahb_clk_t                 AHB;
    i_pclk_t                    Peripheral;
    uint32_t                    (*GetMainClk)(void);
    bool                        (*OutCfg)(em_out_clk_src_t tSrc, uint32_t wDiv);
END_DEF_INTERFACE(i_clk_t)
//! @}

//! \name pclk
//! @{
DEF_INTERFACE(i_power_t)
    bool                        (*Enable)(em_power_t tIndex);
    bool                        (*Disable)(em_power_t tIndex);
    uint32_t                    (*GetStatus)(em_power_t tIndex);
    bool                        (*Resume)(em_power_t tIndex , uint32_t tStatus);
END_DEF_INTERFACE(i_power_t)
//! @}

//! \name pmu struct
//! @{
DEF_INTERFACE(i_pm_t)   
    i_power_t                   Power;
    i_clk_t                     Clock;
END_DEF_INTERFACE(i_pm_t)
//! @}
/*============================ GLOBAL VARIABLES ==============================*/
//! \brief declare the PM
extern const i_pm_t PM;

/*============================ PROTOTYPES ====================================*/
extern bool     power_config_enable(em_power_t tIndex);
extern bool     power_config_disable(em_power_t tIndex);
extern uint32_t power_config_status_get(em_power_t tIndex);
extern bool     power_config_status_resume(em_power_t tIndex , uint32_t tStatus);
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern bool     pll_clk_sel(em_pll_clk_src_t tClk);
extern bool     pll_cfg(uint32_t wMsel, uint32_t wPsel);
extern uint32_t pll_get_in_clock(void);
extern uint32_t pll_get_out_clock(void);
extern uint32_t main_clock_get(void);
extern bool     core_clock_config(em_main_clk_src_t tSrc, uint32_t wDiv);
extern uint32_t core_clock_get(void);
extern bool     ahb_clk_enable(em_ahb_clk_t tIndex);
extern bool     ahb_clk_disable(em_ahb_clk_t tIndex);
extern uint32_t ahb_clk_get_status(em_ahb_clk_t tIndex);
extern bool     ahb_clk_resume_status(em_ahb_clk_t tIndex, uint32_t tStatus);
extern bool     clock_out_config(em_out_clk_src_t tSrc, uint32_t wDiv);
extern bool     peripheral_clk_config(uint8_t chIndex , uint8_t chDiv);
extern uint32_t peripheral_clk_get_div(uint8_t chIndex);
extern uint32_t peripheral_clk_get(uint8_t chIndex);

#endif
