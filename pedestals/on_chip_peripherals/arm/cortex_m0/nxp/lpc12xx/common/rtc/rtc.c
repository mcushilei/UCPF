
/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_rtc.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                                  \
    {                                                                       \
        uint32_t tAHBStatus = PM_AHB_CLK_GET_STATUS(AHBCLK_RTC);            \
        PM_AHB_CLK_ENABLE(AHBCLK_RTC);                                      \
        __VA_ARGS__;                                                        \
        PM_AHB_CLK_RESUME(AHBCLK_RTC,tAHBStatus);                           \
    }

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
} rtc_cfg_t;
//! @}

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief init real-time clock
 *! \param void
 *! \retval true : succeed
 *! \retval false: failed
 */
bool rtc_config(rtc_cfg_t *tCfg)
{
    SAFE_CLK_CODE (
        RTC_REG.CR.Value = 0;
        if (((tCfg->chMode >> 11) & 0x0Fu) == 0x04u) {
            SYSCON_REG.RTCCLKDIV.Value = 255;
        }
        PMU_REG.SYSCFG.RTCCLK = tCfg->chMode >> 11;
        
        RTC_REG.CR.Value    = tCfg->chMode & 0x01u;
        RTC_REG.IMSC.Value  = tCfg->chMode >> 7;
        RTC_REG.MATCH       = tCfg->wMatchValue;
        RTC_REG.LOAD        = 0;
    )

    return true;
}

/*! \brief enable the rtc AHBCLK
 *! \param none
 *! \retval none
 */
bool rtc_enable(void)
{
    PM_AHB_CLK_ENABLE(AHBCLK_RTC);
    
    return true;
}

/*! \brief disable the rtc AHBCLK
 *! \param none
 *! \retval none
 */
bool rtc_disable(void)
{
    PM_AHB_CLK_DISABLE(AHBCLK_RTC);
    
    return true;
}

/*! \brief Clear the rtc flag
 *! \param none
 *! \retval none
 */
void rtc_clear_int_flag(void)
{
    SAFE_CLK_CODE (
        RTC_REG.ICR.Value = 0x01u;
    )
}

/*! \brief get the rtc flag
 *! \param none
 *! \retval none
 */
bool rtc_get_int_flag(void)
{
    bool bResult;
    
    SAFE_CLK_CODE(
        bResult = RTC_REG.RIS.RTCRIS;
    )
        
    return bResult;
}

/*! \brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t rtc_get_time_value(void)
{
    uint32_t wResult;
    
    SAFE_CLK_CODE(
        wResult = RTC_REG.DATA;
    )
        
    return wResult;
}

/*! \brief set count value
 *! \param void
 *! \retval return geh time count value
 */
bool rtc_set_time_value(uint32_t wValue)
{
    SAFE_CLK_CODE (
        RTC_REG.LOAD = wValue;
    )
        
    return true;
}

/*! \brief Set match value
 *! \param time match value
 *! \retval none
 */
bool rtc_set_match_value(uint32_t wValue)
{
    SAFE_CLK_CODE(
        RTC_REG.MATCH = wValue;
    )
        
    return true;
}

/*! \brief get match value
 *! \param void
 *! \retval return match value
 */
uint32_t rtc_get_match_value(void)
{
    uint32_t wResult;
    
    SAFE_CLK_CODE(
        wResult = RTC_REG.MATCH;
    )
        
    return wResult;
}
