/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_rtc.h"
#include "..\pm\pm.h"

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
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/
bool rtc_enable(void)
{
    bool Resault = false;
    
    SAFE_CLK_CODE(
        do {
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is reseted.
                RTC_REG.CTRL = 0u;
            }
            Resault = true;
        } while (false);
    )
    
    return Resault;
}

bool rtc_disable(void)
{
    SAFE_CLK_CODE(
        RTC_REG.CTRL = 1u << RTC_CTRL_SWRESERT_BIAS;
    )
    
    return true;
}

bool rtc_start(uint32_t InitValue)
{
    bool Resault = false;
    
    SAFE_CLK_CODE(
        do {
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is reseted.
                break;
            }
            RTC_REG.COUNT = InitValue;
            RTC_REG.CTRL = RTC_REG.CTRL
                         & (~((1u << RTC_CTRL_ALARM1HZ_BIAS) | (1u << RTC_CTRL_WAKE1KHZ_BIAS)))
                         | (1u << RTC_CTRL_RTC_EN_BIAS);
            if (RTC_REG.CTRL & RTC_CTRL_OFD_MASK) {         //! RTC osc if failed.
                break;
            }
            Resault = true;
        } while (false);
    )
    
    return Resault;
}

bool rtc_stop(void)
{
    bool Resault = false;
    
    SAFE_CLK_CODE(
        do {
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is reseted.
                RTC_REG.CTRL = 0;
            }
            RTC_REG.CTRL = RTC_REG.CTRL & ~((1u << RTC_CTRL_RTC_EN_BIAS) |
                                            (1u << RTC_CTRL_ALARM1HZ_BIAS) |
                                            (1u << RTC_CTRL_WAKE1KHZ_BIAS));
            Resault = true;
        } while (false);
    )
    
    return Resault;
}

/*! \brief set count value
 *! \param void
 *! \retval return geh time count value
 */
bool rtc_set_time_value(uint32_t wValue)
{
    bool Resault = false;
    
    SAFE_CLK_CODE (
        do {
            uint32_t RTCStatus = RTC_REG.CTRL & RTC_CTRL_RTC_EN_MASK;
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is reseted.
                break;
            }
            RTC_REG.CTRL = RTC_REG.CTRL & ~((1u << RTC_CTRL_RTC_EN_BIAS) |
                                            (1u << RTC_CTRL_ALARM1HZ_BIAS) |
                                            (1u << RTC_CTRL_WAKE1KHZ_BIAS));
            RTC_REG.COUNT = wValue;
            RTC_REG.CTRL = RTC_REG.CTRL
                         & (~((1u << RTC_CTRL_ALARM1HZ_BIAS) | (1u << RTC_CTRL_WAKE1KHZ_BIAS)))
                         | RTCStatus;
            Resault = true;
        } while (false);
    )
        
    return Resault;
}

/*! \brief get count value
 *! \param void
 *! \retval return geh time count value
 */
uint32_t rtc_get_time_value(void)
{
    uint32_t wResult;
    
    SAFE_CLK_CODE(
        wResult = RTC_REG.COUNT;
    )
        
    return wResult;
}

/*! \brief Set match value
 *! \param time match value
 *! \retval none
 */
bool rtc_set_match_value(uint32_t wValue)
{
    bool Resault = false;
    
    SAFE_CLK_CODE (
        do {
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is reseted.
                break;
            }
            RTC_REG.MATCH = wValue;
            Resault = true;
        } while (false);
    )
        
    return Resault;
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

bool rtc_alarm_enable(void)
{
    bool Resault = false;
    
    SAFE_CLK_CODE (
        do {
            uint32_t RTCStatus = RTC_REG.CTRL & RTC_CTRL_RTC_EN_MASK;
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is not enabled.
                break;
            }
            RTC_REG.CTRL = RTC_REG.CTRL & (~(1u << RTC_CTRL_WAKE1KHZ_BIAS))
                                        | (1u << RTC_CTRL_ALARMDPD_EN_BIAS)
                                        | (1u << RTC_CTRL_ALARM1HZ_BIAS);
            Resault = true;
        } while (false);
    )
        
    return Resault;
}

bool rtc_alarm_disable(void)
{
    bool Resault = false;
    
    SAFE_CLK_CODE (
        do {
            uint32_t RTCStatus = RTC_REG.CTRL & RTC_CTRL_RTC_EN_MASK;
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is not enabled.
                break;
            }
            RTC_REG.CTRL = RTC_REG.CTRL & (~((1u << RTC_CTRL_WAKE1KHZ_BIAS) | (1u << RTC_CTRL_ALARMDPD_EN_BIAS)))
                                        | (1u << RTC_CTRL_ALARM1HZ_BIAS);
            Resault = true;
        } while (false);
    )
        
    return Resault;
}

bool rtc_alarm_clear_intflag(void)
{
    bool Resault = false;
    
    SAFE_CLK_CODE (
        do {
            uint32_t RTCStatus = RTC_REG.CTRL & RTC_CTRL_RTC_EN_MASK;
            if (RTC_REG.CTRL & RTC_CTRL_SWRESERT_MASK) {    //! RTC is not enabled.
                break;
            }
            RTC_REG.CTRL = RTC_REG.CTRL & (~(1u << RTC_CTRL_WAKE1KHZ_BIAS))
                                        | (1u << RTC_CTRL_ALARM1HZ_BIAS);
            Resault = true;
        } while (false);
    )
        
    return Resault;
}
