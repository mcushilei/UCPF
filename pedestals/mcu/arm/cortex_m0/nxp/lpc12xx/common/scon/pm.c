/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_scon.h"
#include ".\reg_sleep.h"

/*============================ MACROS ========================================*/
#ifndef MAX_PLL_OUTCLK_FREQ
#error No defined MAX_PLL_OUTCLK_FREQ
#endif

#ifndef MAX_SYSTEM_CLK_FREQ
#error No defined MAX_SYSTEM_CLK_FREQ
#endif

#ifndef IRC_OSC_FREQ
#error No defined IRC_OSC_FREQ
#endif


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
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

//! \name core clock struct type
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
    bool                        (*Enable)(em_power_t wMask);
    bool                        (*Disable)(em_power_t wMask);
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

/*============================ LOCAL VARIABLES ===============================*/

//! \brief define the Aon Oscilator  Frequce
static uint32_t s_wAonOscFreq  = 0;

/*============================ PROTOTYPES ====================================*/
extern bool     power_config_enable(em_power_t tIndex);
extern bool     power_config_disable(em_power_t tIndex);
extern uint32_t power_config_status_get(em_power_t tIndex);
extern bool     power_config_status_resume(em_power_t tIndex , uint32_t tStatus);
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern bool     pll_clk_sel(em_pll_clk_src_t tClk);
extern bool     pll_cfg(uint32_t chMsel, uint32_t chPsel);
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

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief define the CMC
const i_pm_t PM = {
    .Power = {
        &power_config_enable,       //!< enable power
        &power_config_disable,      //!< disable power
        &power_config_status_get,   //!< get status
        &power_config_status_resume //!< resume
    },
    .Clock = {
        .PLL = {
            &pll_enable,
            &pll_disable,
            &pll_clk_sel,
            &pll_cfg,                  //!< init the pll
            &pll_get_out_clock,         //!< get the out put clock of pll
            &pll_get_in_clock           //!< get the input clock of pll
        },
        .Core = {
            &core_clock_config,                //!< 
            &core_clock_get,              //!< get core clock
        },
        .AHB = {
            &ahb_clk_enable,            //!< enable specified ahb clock supply
            &ahb_clk_disable,           //!< disable specified ahb clock supply
            &ahb_clk_get_status,        //!< get ahbclk status
            &ahb_clk_resume_status      //!< resume ahbclk
        },
        .Peripheral = {
            &peripheral_clk_config,   //!< pclk config
            &peripheral_clk_get,      //!< pclk config
            &peripheral_clk_get_div,    //!< get status
            &peripheral_clk_config    //!< resume status
        },  
        &main_clock_get,
        &clock_out_config,
    },
};

/*============================ IMPLEMENTATION ================================*/
/*---------------------------- Analog Circuit Power Control ------------------*/
//! \brief enable specified power supply
bool power_config_enable(em_power_t tIndex)
{
    SYSCON_REG.PDRUNCFG.Value &= ~BIT(tIndex);

    return true;
}

//! \brief disable specified power supply
bool power_config_disable(em_power_t tIndex)
{
    SYSCON_REG.PDRUNCFG.Value |= BIT(tIndex);

    return true;
}

//! \brief get status of specified power supply
uint32_t power_config_status_get(em_power_t tIndex)
{
    return SYSCON_REG.PDRUNCFG.Value & BIT(tIndex);
}

//! \brief resume specified power supply
bool power_config_status_resume(em_power_t tIndex , uint32_t tStatus)
{
    SYSCON_REG.PDRUNCFG.Value = (SYSCON_REG.PDRUNCFG.Value & ~BIT(tIndex)) 
                              | (tStatus & BIT(tIndex));

    return true;
}

/*---------------------------- Peripheral Register Circuit Clock Control -----*/
//! \brief enable specified ahb clock supply
bool ahb_clk_enable(em_ahb_clk_t tIndex)
{
    if (tIndex > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL |= BIT(tIndex);

    return true;
}

//! \brief disable specified ahb clock supply
bool ahb_clk_disable(em_ahb_clk_t tIndex)
{
    if (tIndex > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL &= ~BIT(tIndex);

    return true;
}

//! \brief disable specified ahb clock supply
uint32_t ahb_clk_get_status(em_ahb_clk_t tIndex)
{
    if (tIndex > 31) {
        return 0;
    }

    return SYSCON_REG.SYSAHBCLKCTRL & BIT(tIndex);
}

//! \brief Resume specified ahb clock supply
bool ahb_clk_resume_status(em_ahb_clk_t tIndex, uint32_t tStatus)
{
    if (tIndex > 31) {
        return false;
    }

    SYSCON_REG.SYSAHBCLKCTRL = (SYSCON_REG.SYSAHBCLKCTRL & ~BIT(tIndex))
                             | (tStatus & BIT(tIndex));

    return true;
}




#define SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus   = ahb_clk_get_status(AHBCLK_FLASHREG);       \
        ahb_clk_enable(AHBCLK_FLASHREG);                                \
        __VA_ARGS__;                                                    \
        ahb_clk_resume_status(AHBCLK_FLASHREG, tStatus);                \
    }

bool core_clock_config(em_main_clk_src_t tSrc, uint32_t wDiv)
{
    if (0 == wDiv) {
        return false;
    }

    if (MAIN_CLKSRC_PLLOUT == tSrc) {
        if((pll_get_out_clock() / wDiv) > MAX_SYSTEM_CLK_FREQ) {
            return false;
        }
    } else if (MAIN_CLKSRC_PLLIN == tSrc) {
        if((pll_get_in_clock() / wDiv) > MAX_SYSTEM_CLK_FREQ) {
            return false;
        }
    }
    
    //! Switch main clock to internal RC oscilator.
    SYSCON_REG.MAINCLKSEL.Value = MAIN_CLKSRC_IRC;
    SYSCON_REG.MAINCLKUEN.Value = 0;
    SYSCON_REG.MAINCLKUEN.Value = 1;
    while (!SYSCON_REG.MAINCLKUEN.ENA);

    //! config flash access time to avoid error to switch a high frq clk.
    //! Note: IRC is 12MHz which below 24MHz, that is definite.
    SAFE_CLK_CODE(
        FLASH_REG.FLASHCFG.RDCFG = 1;       //! 0 clk wait state
        SYSCON_REG.PRESETCTRL.FLASH_OVERRIDE = 0;
        if (MAIN_CLKSRC_PLLOUT == tSrc) {
            if((pll_get_out_clock() / wDiv) > 24MHz) {  //! 2 clk wait state
                FLASH_REG.FLASHCFG.RDCFG = 0;
                SYSCON_REG.PRESETCTRL.FLASH_OVERRIDE = 0;
            }
        } else if (MAIN_CLKSRC_PLLIN == tSrc) {
            if((pll_get_in_clock() / wDiv) > 24MHz) {   //! 2 clk wait state
                FLASH_REG.FLASHCFG.RDCFG = 0;
                SYSCON_REG.PRESETCTRL.FLASH_OVERRIDE = 0;
            }
        }
    )
    
    //! Switch main clock to specify source.
    SYSCON_REG.SYSAHBCLKDIV.Value = wDiv;   //! set the system clock divider
    SYSCON_REG.MAINCLKSEL.Value   = tSrc;   //! set the main clock source
    SYSCON_REG.MAINCLKUEN.Value = 0;
    SYSCON_REG.MAINCLKUEN.Value = 1;
    while (!SYSCON_REG.MAINCLKUEN.ENA);

    return true;
}

/*!\brief 
*! \param void
*! \return the system clock
*/
uint32_t core_clock_get(void)
{
    return main_clock_get()/SYSCON_REG.SYSAHBCLKDIV.Value;
}

/*!\brief 
*! \param void
*! \return the main clock
*/
uint32_t main_clock_get(void)
{
    uint32_t wResult = 0;
    
    //! \brief check the main clock select register
    switch(SYSCON_REG.MAINCLKSEL.Value & SSC_MAINCLKSEL_SEL_MSK){
        case MAIN_CLKSRC_IRC:
            wResult = IRC_OSC_FREQ;
            break;
        case MAIN_CLKSRC_PLLIN:
            wResult = pll_get_in_clock();
            break;
        case MAIN_CLKSRC_WDOSC:
            wResult = s_wAonOscFreq;  
            break;
        case MAIN_CLKSRC_PLLOUT: 
            wResult = pll_get_out_clock();
            break;
    }

    return wResult;
}

bool pll_enable(void)
{
    SYSCON_REG.PDRUNCFG.SYSPLL = 0;         //!< power up PLL
    return true;
}

bool pll_disable(void)
{
    SYSCON_REG.PDRUNCFG.SYSPLL = 1;         //!< power down PLL
    return true;
}

bool pll_clk_sel(em_pll_clk_src_t tClk)
{   
    //! can not switch clock source when PLL is running.
    if (0 == SYSCON_REG.PDRUNCFG.SYSPLL) {
        return false;
    }
    
    //! enable ckock source.
    if (PLL_CLKSRC_SYSOSC == tClk) {
        //! System Oscillator Config.
        if (SYS_OSC_FREQ < 15MHz) {
            SYSCON_REG.SYSOSCCTRL.FREQRANGE = 0;
        } else {
            SYSCON_REG.SYSOSCCTRL.FREQRANGE = 1;
        }
        SYSCON_REG.SYSOSCCTRL.BYPASS = 0;
        SYSCON_REG.PDRUNCFG.SYSOSC = 0;
    } else {
        //! Enable IRC.
        SYSCON_REG.PDRUNCFG.IRC = 0;
    }
    
    SYSCON_REG.SYSPLLCLKSEL.SEL = tClk;     //!< cfg source
    SYSCON_REG.SYSPLLCLKUEN.Value = 0;      //!< source cfg update
    SYSCON_REG.SYSPLLCLKUEN.Value = 1;
    
    if (PLL_CLKSRC_SYSOSC == tClk) {
    } else {
        SYSCON_REG.PDRUNCFG.SYSOSC = 1;     //!< power down osilator
    }

    return true;
}

bool pll_cfg(uint32_t wMsel, uint32_t wPsel)
{
    if ((wMsel > 31) || (wPsel > 3) || (0 == wMsel)){
        return false;
    }
    
    SYSCON_REG.SYSPLLCTRL.Value = SSC_SYSPLLCTRL_MSEL(wMsel - 1)
                                | SSC_SYSPLLCTRL_PSEL(wPsel);

    while (!SYSCON_REG.SYSPLLSTAT.LOCK);
    return true;
}
/*!\brief 
 *! \param void
 *! \retval the pll input clock 
 */
uint32_t pll_get_in_clock(void)
{
    uint32_t wResult = 0;
    
    switch (SYSCON_REG.SYSPLLCLKSEL.SEL) {
        case 0: 
            wResult = IRC_OSC_FREQ;
            break;
        case 1: 
            wResult = SYS_OSC_FREQ;
            break;
        default:
            wResult = 0;
            break;
    }

    return wResult;
}

/*!\brief 
 *! \param  none
 *! \retval the pll output clock 
 */
uint32_t pll_get_out_clock(void)
{
    uint32_t wClk = 0;

    wClk = pll_get_in_clock();
    wClk *= ((SYSCON_REG.SYSPLLCTRL.Value & SSC_SYSPLLCTRL_MSEL_MSK)
            >> SSC_SYSPLLCTRL_MSEL_BIT0)
            + 1;

    return wClk;
}

/*!\brief config the clock out
*! \param config struct
*! \return the main clock
*/
bool clock_out_config(em_out_clk_src_t tSrc, uint32_t wDiv)
{
    SYSCON_REG.CLKOUTSEL.Value   =   tSrc;
    SYSCON_REG.CLKOUTUEN.ENA = 0;         //!< update clock
    SYSCON_REG.CLKOUTUEN.ENA = 1;
    while (!SYSCON_REG.CLKOUTUEN.ENA);
    SYSCON_REG.CLKOUTDIV.Value   =   wDiv;

    return true;
}

/*!\brief peripheral clock config
*! \param config struct
*! \return the main clock
*/
bool peripheral_clk_config(uint8_t chIndex , uint8_t chDiv)
{
    if (chIndex > PCLK_MAX_INDEX){
        return false;
    }

    ((reg32_t *)&SYSCON_REG.SSPCLKDIV)[chIndex] = chDiv;

    return true;
}

/*!\brief get peripheral clock status
*! \param config struct
*! \return the main clock
*/
uint32_t peripheral_clk_get_div(uint8_t chIndex)
{
    if (chIndex > PCLK_MAX_INDEX){
        return 0;
    }
    
    return (uint32_t)((reg32_t *)&SYSCON_REG.SSPCLKDIV)[chIndex];
}

/*!\brief get peripheral clock c
*! \param config struct
*! \return the main clock
*/
uint32_t peripheral_clk_get(uint8_t chIndex)
{
    uint8_t chDiv;
    
    if (chIndex > PCLK_MAX_INDEX){
        return 0;
    }
    
    chDiv = (uint8_t)((reg32_t *)&SYSCON_REG.SSPCLKDIV)[chIndex];

    if(0 == chDiv){
        return 0;
    }

    return  main_clock_get()/chDiv;
}

