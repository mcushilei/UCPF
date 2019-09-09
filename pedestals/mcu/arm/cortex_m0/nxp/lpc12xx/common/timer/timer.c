
/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_timer.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
//! \brief timer channel max counter
#define TC_MAX_CHANNEL          (2ul)

//! \brief Macro for timer interface init
#define __TIMER_OBJ(__N,__A)                                                    \
    {                                                                           \
        {                                                                       \
            .Init           = &timer##__N##_init,                               \
            .Idle           = &timer##__N##_idle,                               \
            .Enable         = &timer##__N##_enable,                             \
            .Disable        = &timer##__N##_disable,                            \
            .Start          = &timer##__N##_start,                              \
            .Stop           = &timer##__N##_stop,                               \
            {                                                                   \
                .Set        = &timer##__N##_frequency_set,                      \
                .Get        = &timer##__N##_frequency_get,                      \
            },                                                                  \
            .ptRegPage      = ((timer_reg_t *)(TIMER##__N##_BASE_ADDRESS)),     \
        },                                                                      \
        {                                                                       \
            {                                                                   \
                .Set        = &timer##__N##_interval_set,                       \
                .Get        = &timer##__N##_interval_get,                       \
            },                                                                  \
            {                                                                   \
                .Enable     = &timer##__N##_interval_interrupt_enable,          \
                .Disable    = &timer##__N##_interval_interrupt_disable,         \
            },                                                                  \
            .IsOverflow     = &timer##__N##_interval_overflow,                  \
        },                                                                      \
        {                                                                       \
            .Config         =   &tc##__N##_pwm_config,                          \
            {                                                                   \
                .Set        =   &tc##__N##_pwm_duty_set,                        \
                .Get        =   &tc##__N##_pwm_duty_get,                        \
            },                                                                  \
            {                                                                   \
                .Set        =   &tc##__N##_pwm_mode_set,                        \
                .Get        =   &tc##__N##_pwm_mode_get,                        \
            },                                                                  \
        },                                                                      \
        {                                                                       \
            .Set            =   &tc##__N##_match_set,                           \
            .Get            =   &tc##__N##_match_get,                           \
            {                                                                   \
                .Enable     =   &tc##__N##_match_int_enable,                    \
                .Disable    =   &tc##__N##_match_int_disable,                   \
            },                                                                  \
            .IsCompareMatch =   &tc##__N##_match_is_compare,                    \
        },                                                                      \
        {                                                                       \
            .Config         =   &tc##__N##_cap_config,                          \
            .Get            =   &tc##__N##_cap_get,                             \
            .IsCaptured     =   &tc##__N##_cap_is_captured,                     \
            {                                                                   \
                .Enable     =   &tc##__N##_cap_int_enable,                      \
                .Disable    =   &tc##__N##_cap_int_disable,                     \
            },                                                                  \
        },                                                                      \
    },

//! \brief timer internal info
#define __TIMER_INTERNAL(__N, __A)                                              \
    {                                                                           \
        ((tmr_reg_t *)(TIMER##__N##_BASE_ADDRESS)),                             \
        AHBCLK_TIMER##__N,                                                      \
    },

#define this        (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/   
//! \brief function tabla statement
#define TIMER_EXTERN_FUNCTION_TABLE(__N, __A)                                   \
    static bool timer##__N##_init(tc_cfg_t *ptTimerCfg);                        \
    static bool timer##__N##_idle(void);                                        \
    static bool timer##__N##_enable(void);                                      \
    static bool timer##__N##_disable(void);                                     \
    static bool timer##__N##_start(void);                                       \
    static bool timer##__N##_stop(void);                                        \
    static bool timer##__N##_frequency_set(uint_fast16_t hwValue);              \
    static uint_fast16_t timer##__N##_frequency_get(void);                      \
    static bool timer##__N##_interval_set(uint_fast16_t hwValue);               \
    static uint_fast16_t timer##__N##_interval_get(void);                       \
    static bool timer##__N##_interval_interrupt_enable(void);                   \
    static bool timer##__N##_interval_interrupt_disable(void);                  \
    static bool timer##__N##_interval_overflow(void);                           \
    static bool tc##__N##_pwm_config(                                           \
        em_tmr_chn_t tChannel,uint32_t wDuty,uint8_t chMode);                   \
    static uint32_t tc##__N##_pwm_duty_get (em_tmr_chn_t tChannel);             \
    static bool tc##__N##_pwm_duty_set (                                        \
        em_tmr_chn_t tChannel, uint32_t wDutVal);                               \
    static uint8_t tc##__N##_pwm_mode_get (em_tmr_chn_t tChannel);              \
    static bool tc##__N##_pwm_mode_set (                                        \
        em_tmr_chn_t tChannel, uint8_t chModeVal);                              \
    static bool tc##__N##_match_set (                                           \
        em_tmr_chn_t tChannel, uint32_t chMatchVal);                            \
    static uint32_t tc##__N##_match_get (em_tmr_chn_t tChannel);                \
    static bool tc##__N##_match_int_enable (em_tmr_chn_t tChannel);             \
    static bool tc##__N##_match_int_disable (em_tmr_chn_t tChannel);            \
    static bool tc##__N##_match_is_compare (em_tmr_chn_t tChannel);             \
    static bool tc##__N##_cap_config (em_cap_mode_t chMode);                    \
    static uint32_t tc##__N##_cap_get (void);                                   \
    static bool tc##__N##_cap_is_captured (void);                               \
    static bool tc##__N##_cap_int_enable (void);                                \
    static bool tc##__N##_cap_int_disable (void);     

//! \brief function tabla
#define TIMER_FUNCTION_TABLE(__N, __A)                                          \
    static bool timer##__N##_init(tc_cfg_t *ptTimerCfg)                         \
    {                                                                           \
        return timer_init((__timer_t *)&__TIMER[__N], ptTimerCfg);              \
    }                                                                           \
                                                                                \
    static bool timer##__N##_idle(void)                                         \
    {                                                                           \
        return timer_idle((__timer_t *)&__TIMER[__N]);                          \
    }                                                                           \
                                                                                \
    static bool timer##__N##_enable(void)                                       \
    {                                                                           \
        return timer_enable((__timer_t *)&__TIMER[__N]);                        \
    }                                                                           \
                                                                                \
    static bool timer##__N##_disable(void)                                      \
    {                                                                           \
        return timer_disable((__timer_t *)&__TIMER[__N]);                       \
    }                                                                           \
                                                                                \
    static bool timer##__N##_start(void)                                        \
    {                                                                           \
        return timer_start((__timer_t *)&__TIMER[__N]);                         \
    }                                                                           \
                                                                                \
    static bool timer##__N##_stop(void)                                         \
    {                                                                           \
        return timer_stop((__timer_t *)&__TIMER[__N]);                          \
    }                                                                           \
                                                                                \
    static bool timer##__N##_frequency_set(uint_fast16_t hwValue)               \
    {                                                                           \
        return timer_frequency_set((__timer_t *)&__TIMER[__N], hwValue);        \
    }                                                                           \
                                                                                \
    static uint_fast16_t timer##__N##_frequency_get(void)                       \
    {                                                                           \
        return timer_frequency_get((__timer_t *)&__TIMER[__N]);                 \
    }                                                                           \
                                                                                \
    static bool timer##__N##_interval_set(uint_fast16_t hwValue)                \
    {                                                                           \
        return timer_interval_set((__timer_t *)&__TIMER[__N], hwValue);         \
    }                                                                           \
                                                                                \
    static uint_fast16_t timer##__N##_interval_get(void)                        \
    {                                                                           \
        return timer_interval_get((__timer_t *)&__TIMER[__N]);                  \
    }                                                                           \
                                                                                \
    static bool timer##__N##_interval_interrupt_enable(void)                    \
    {                                                                           \
        return timer_interval_interrupt_enable((__timer_t *)&__TIMER[__N]);     \
    }                                                                           \
                                                                                \
    static bool timer##__N##_interval_interrupt_disable(void)                   \
    {                                                                           \
        return timer_interval_interrupt_disable((__timer_t *)&__TIMER[__N]);    \
    }                                                                           \
                                                                                \
    static bool timer##__N##_interval_overflow(void)                            \
    {                                                                           \
        return timer_interval_overflow((__timer_t *)&__TIMER[__N]);             \
    }                                                                           \
                                                                                \
    static bool tc##__N##_pwm_config(                                           \
        em_tmr_chn_t tChannel,uint32_t wDuty,uint8_t chMode)                    \
    {                                                                           \
        return tc_pwm_config ((__timer_t *)&__TIMER[__N],                       \
                              tChannel,                                         \
                              wDuty,                                            \
                              chMode);                                          \
    }                                                                           \
                                                                                \
    static uint32_t tc##__N##_pwm_duty_get (em_tmr_chn_t tChannel)              \
    {                                                                           \
        return tc_pwm_duty_get ((__timer_t *)&__TIMER[__N], tChannel);          \
    }                                                                           \
                                                                                \
    static bool tc##__N##_pwm_duty_set (em_tmr_chn_t tChannel,uint32_t wDutyVal)\
    {                                                                           \
        return tc_pwm_duty_set ((__timer_t *)&__TIMER[__N], tChannel,wDutyVal); \
    }                                                                           \
                                                                                \
    static uint8_t tc##__N##_pwm_mode_get (em_tmr_chn_t tChannel)               \
    {                                                                           \
        return tc_pwm_mode_get ((__timer_t *)&__TIMER[__N], tChannel);          \
    }                                                                           \
                                                                                \
    static bool tc##__N##_pwm_mode_set (em_tmr_chn_t tChannel,uint8_t chModeVal)\
    {                                                                           \
        return tc_pwm_mode_set ((__timer_t *)&__TIMER[__N],tChannel,chModeVal); \
    }                                                                           \
                                                                                \
    static bool tc##__N##_match_set (em_tmr_chn_t tChannel, uint32_t chMatchVal)\
    {                                                                           \
        return tc_match_set ((__timer_t *)&__TIMER[__N], tChannel, chMatchVal); \
    }                                                                           \
                                                                                \
    static uint32_t tc##__N##_match_get (em_tmr_chn_t tChannel)                 \
    {                                                                           \
        return tc_match_get ((__timer_t *)&__TIMER[__N], tChannel);             \
    }                                                                           \
                                                                                \
    static bool tc##__N##_match_int_enable (em_tmr_chn_t tChannel)              \
    {                                                                           \
        return tc_match_int_enable ((__timer_t *)&__TIMER[__N], tChannel);      \
    }                                                                           \
                                                                                \
    static bool tc##__N##_match_int_disable (em_tmr_chn_t tChannel)             \
    {                                                                           \
        return tc_match_int_disable ((__timer_t *)&__TIMER[__N], tChannel);     \
    }                                                                           \
                                                                                \
    static bool tc##__N##_match_is_compare (em_tmr_chn_t tChannel)              \
    {                                                                           \
        return tc_match_is_compare ((__timer_t *)&__TIMER[__N], tChannel);      \
    }                                                                           \
                                                                                \
    static bool tc##__N##_cap_config (em_cap_mode_t chMode)                     \
    {                                                                           \
        return tc_cap_config ((__timer_t *)&__TIMER[__N], 0, chMode);           \
    }                                                                           \
                                                                                \
    static uint32_t tc##__N##_cap_get (void)                                    \
    {                                                                           \
        return tc_cap_get ((__timer_t *)&__TIMER[__N], 0);                      \
    }                                                                           \
                                                                                \
    static bool tc##__N##_cap_is_captured (void)                                \
    {                                                                           \
        return tc_cap_is_captured ((__timer_t *)&__TIMER[__N], 0);              \
    }                                                                           \
                                                                                \
    static bool tc##__N##_cap_int_enable (void)                                 \
    {                                                                           \
        return tc_cap_int_enable ((__timer_t *)&__TIMER[__N], 0);               \
    }                                                                           \
                                                                                \
    static bool tc##__N##_cap_int_disable (void)                                \
    {                                                                           \
        return tc_cap_int_disable ((__timer_t *)&__TIMER[__N], 0);              \
    }                                                          


//! \brief safe clock code 
#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        ahbclk_status_t tStatus   = PM_GET_AHB_CLK_STATUS(this.tAHBClk);        \
        PM_AHB_CLK_ENABLE(this.tAHBClk);                                        \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME(this.tAHBClk, tStatus);                               \
    }

/*============================ TYPES =========================================*/
//! \name internal class
//! @{
DEF_CLASS(__timer_t)
    tmr_reg_t *const    ptReg;              //!< reference to register page
    em_ahb_clk_t        tAHBClk;            //!< ahbclk info
END_DEF_CLASS(__timer_t)
//! @}

//! \note timer capture mode
//! @{
typedef enum {
    TC_CAP_RISING_EDGE  =   (0ul),          //!< timer capture rising edge
    TC_CAP_FAILING_EDGE =   (1ul),          //!< timer capture failing edge
} em_cap_mode_t;
//! @}

//! \name timer configuration structure
//! @{
typedef struct{
    uint16_t            hwMode;             //!< timer working mode
} tc_cfg_t;
//! @}

//! \name timer capture structure
//! @{
typedef struct{
    uint16_t            hwMode;             //!< timer working mode
} tc_cap_cfg_t;
//! @}

typedef enum {
    TMR_CHN0    = 0,                        //!< timer channle 0
    TMR_CHN1    = 1,                        //!< timer channle 1
    TMR_CHN2    = 2,                        //!< timer channle 2

    TMR_PWM0    = 0,                        //!< timer pwm channle 0
    TMR_PWM1    = 1,                        //!< timer pwm channle 1
    TMR_PWM2    = 2                         //!< timer pwm channle 2
} em_tmr_chn_t;

//! \name class: timer base
//! @{
DEF_INTERFACE(i_tmr_base_t)
   
    bool        (*Init)(tc_cfg_t *ptCfg);   //!< initialize timer
    bool        (*Idle)(void);              //!< get busy flag status
    bool        (*Enable)(void);            //!< enable timer 
    bool        (*Disable)(void);           //!< disable timer
    bool        (*Start)(void);             //!< start timer
    bool        (*Stop)(void);              //!< stop timer

    u16_property_t  Frequency;              //!< frequency function

    timer_reg_t * const ptRegPage;          //!< reference to register page
END_DEF_INTERFACE(i_tmr_base_t)
//! @}

//! \name class: timer interval
//! @{
DEF_INTERFACE(i_tmr_interval_t)

    u16_property_t      TOP;                //!< top function
    en_property_t       Interrupt;          //!< top interrupt function
    bool                (*IsOverflow)(void);//!< get top overflow status

END_DEF_INTERFACE(i_tmr_interval_t)
//! @}

//! \name i_tmr_match_int_t
//! @{
DEF_INTERFACE (i_tmr_match_int_t)
    bool    (*Enable)   (em_tmr_chn_t);     //!< match interrupt enable
    bool    (*Disable)  (em_tmr_chn_t);     //!< match interrupt disable
END_DEF_INTERFACE (i_tmr_match_int_t)
//! @}

//! \name i_tmr_match_t
//! @{
DEF_INTERFACE (i_tmr_match_t)
    //! setup timer channel match value
    bool        (*Set)  (em_tmr_chn_t tChannel, uint32_t wMatchVal);
    //! get timer channel match value
    uint32_t    (*Get)  (em_tmr_chn_t tChannel);
    //! match interrupt
    i_tmr_match_int_t   Interrupt;
    //! match compare status
    bool        (*IsCompareMatch)(em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_match_t)
//! @}

//! \name i_tmr_pwm_t
//! @{
DEF_INTERFACE (i_tmr_duty_t)
    //! setup PWm duty
    bool        (*Set)  (em_tmr_chn_t tChannel, uint32_t wDutyVal);
    //! get PWm duty
    uint32_t    (*Get)  (em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_duty_t)
//! @}

//! \name i_tmr_mode_t
//! @{
DEF_INTERFACE (i_tmr_mode_t)
    //! setup timer PWM mode
    bool        (*Set)  (em_tmr_chn_t tChannel, uint8_t chModeVal);
    //! get timer PWM mode
    uint8_t     (*Get)  (em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_mode_t)
//! @}

//! \name i_tmr_pwm_t
//! @{
DEF_INTERFACE (i_tmr_pwm_t)
    //! PWM configure
    bool            (*Config)(em_tmr_chn_t tChannel,uint32_t wDuty,uint8_t chMode);
    //! PWM Duty interface
    i_tmr_duty_t     Duty;
    //! PWM Mode interface
    i_tmr_mode_t     Mode;
END_DEF_INTERFACE (i_tmr_pwm_t)
//! @}

//! \name i_tmr_capture_t;
//! @{
DEF_INTERFACE (i_tmr_capture_t)
    //! timer capture configure
    bool                (*Config)       (em_cap_mode_t chMode);
    //! get timer capture value
    uint32_t            (*Get)          (void);
    //! get timer capture status
    bool                (*IsCaptured)   (void);
    //! time capture interrupt
    en_property_t       Interrupt;
END_DEF_INTERFACE (i_tmr_capture_t)
//! @}

//! \name class: timer 
//! @{
DEF_INTERFACE(i_tmr_t) IMPLEMENT(i_tmr_base_t)

    i_tmr_interval_t     Interval;                  //!< timer interval function
    i_tmr_pwm_t          PWM;                       //!< timer PWM function
    i_tmr_match_t        Match;                     //!< timer match function
    i_tmr_capture_t      Capture;                   //!< timer capture function

END_DEF_INTERFACE(i_tmr_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief timer function statement
MREPEAT(TIMER_COUNT, TIMER_EXTERN_FUNCTION_TABLE, NULL)

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief tiemr object
const i_tmr_t TIMER[] = {
    MREPEAT(TIMER_COUNT, __TIMER_OBJ, NULL)
};

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal timer object
static CLASS(__timer_t) __TIMER[] = {
    MREPEAT(TIMER_COUNT, __TIMER_INTERNAL, NULL)
};

/*============================ IMPLEMENTATION ================================*/


/*! \brief initialize timer
 *! \param ptTimer timer object
 *! \param ptTimerCfg timer configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool timer_init(__timer_t *ptTimer,tc_cfg_t *ptTimerCfg)
{
    return true;
}

/*! \brief is timer idle
 *! \param ptTimer timer object
 *! \retval true timer idle
 *! \retval false timer busy
 */
static bool timer_idle(__timer_t *ptTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    bool bReturn = false;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        uint32_t wOldTC = this.ptReg->TC;   
        uint32_t wNewTC = this.ptReg->TC;

        if (wOldTC == wNewTC) {
            bReturn = true;
        }
    );
    
    return bReturn;
}  

/*! \brief timer enable
 *! \param ptTimer timer object
 *! \retval true timer enable succeed
 *! \retval false timer enable failed
 */
static bool timer_enable(__timer_t *ptTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    //! Enable AHB Clock
    PM_AHB_CLK_ENABLE(this.tAHBClk);

    return true;
}

/*! \brief timer disable
 *! \param ptTimer timer object
 *! \retval true timer disable succeed
 *! \retval false timer disable failed
 */
static bool timer_disable(__timer_t *ptTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    //! Disable AHB Clock
    PM_AHB_CLK_DISABLE(this.tAHBClk);
    
    return true;
}  

/*! \brief timer start count
 *! \param ptTimer timer object
 *! \retval true timer start count succeed
 *! \retval false timer start count failed
 */
static bool timer_start(__timer_t *ptTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! counter enable
        this.ptReg->TCR.Value = TIMER_TCR_CEN_MSK;
    );

    return true;
} 

/*! \brief timer stop count
 *! \param ptTimer timer object
 *! \retval true timer stop count succeed
 *! \retval false timer stop count failed
 */
static bool timer_stop(__timer_t *ptTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! counter disable
        this.ptReg->TCR.Value = TIMER_TCR_CRST_MSK;
    );

    return true;
}    

/*! \brief set timer interval
 *! \param ptTimer timer object
 *! \param hwValue interval value
 *! \retval true set timer interval succeed
 *! \retval false set timer interval failed
 */
static bool timer_interval_set(__timer_t *ptTimer, uint_fast16_t hwValue)                         
{           
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! update top value
        this.ptReg->MATCH[3]     = hwValue;
        //! set prescale value
        this.ptReg->PR           = 0;
        //! set overflow flag
        this.ptReg->PWMC.Value  &= ~PWMC_PWMEN3_MSK;
        
        //! update EMR
        do {
            uint32_t wTempEMR = this.ptReg->EMR.Value;

            wTempEMR &= ~(EMR_EMC3_MSK | EMR_EM3_MSK);
            wTempEMR |= EMR_EMC3_SET(0x02);

            this.ptReg->EMR.Value = wTempEMR;
        } while (false);  

        //! update MCR register
        do {
            uint32_t wTempMCR = this.ptReg->MCR.Value;

            wTempMCR |=  MCR_MR3R_MSK;
            wTempMCR &= ~MCR_MR3S_MSK;

            this.ptReg->MCR.Value = wTempMCR;
        } while (false);        
    )
                                                                   
    return true;
}                                                                              
         
/*! \brief get timer interval
 *! \param ptTimer timer object
 *! \return timer interval
 */                                                                      
static uint16_t timer_interval_get(__timer_t *ptTimer)                                
{                                                                              
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    uint16_t hwInterval;

    if (NULL == ptTimer) {
        return 0;
    }

    SAFE_CLK_CODE(
        //! get top value
        hwInterval = this.ptReg->MATCH[3];       
    );
                                                                   
    return hwInterval;
}                                                                              
                  
/*! \brief enable timer interval interrupt
 *! \param ptTimer timer object
 *! \retval true set timer interval interrupt succeed
 *! \retval false set timer interval interrupt failed
 */                                                             
static bool timer_interval_interrupt_enable(__timer_t *ptTimer)                   
{                                                                              
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! update MCR register
        this.ptReg->MCR.Value |= MCR_MR3I_MSK;    
    );
                                                                   
    return true;
}                                                                              
                 
/*! \brief disable timer interval interrupt
 *! \param ptTimer timer object
 *! \return interrupt status
 */   
static bool timer_interval_interrupt_disable(__timer_t *ptTimer)                          
{                                                                              
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! update MCR register
        this.ptReg->MCR.Value &= ~MCR_MR3I_MSK;  
    );
                                                                   
    return true;
}    

/*! \brief get timer overflow status
 *! \param ptTimer timer object
 *! \return overflow status
 */   
static bool timer_interval_overflow(__timer_t *ptTimer)                          
{                                                                              
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    bool bStatus;

    if (NULL == ptTimer) {
        return false;
    }

    SAFE_CLK_CODE(
        //! update MCR register
        if (this.ptReg->EMR.Value & EMR_EM3_MSK) {
            this.ptReg->EMR.Value &= ~EMR_EM3_MSK;
            this.ptReg->IR.Value  |= TIMER0_IR_MR3INT_MSK;
            bStatus = true;
        } else {
            bStatus = false;
        }
    );
                                                                   
    return bStatus;
}

/*! \brief set timer frequency 
 *! \param ptTimer timer object
 *! \param hwValue frequency value
 *! \return interrupt status
 */   
static bool timer_frequency_set(__timer_t *ptTimer, uint16_t hwValue)                        
{                                                                              
    uint32_t wPrescale;

    if (NULL == ptTimer || 0 == hwValue) {
        return false;
    }

    //! get prescal value
    wPrescale = PM_GET_SYS_CLK() / hwValue;

    //! frequency overflow
    if (0 == wPrescale || wPrescale > UINT16_MAX) {
        return false;
    }

    return timer_interval_set(ptTimer, wPrescale);
                                                                   
}                                                                              
 
/*! \brief set timer frequency 
 *! \param ptTimer timer object
 *! \return interrupt status
 */                                                                                 
static uint16_t timer_frequency_get(__timer_t *ptTimer)                               
{                                                                              
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    uint16_t hwInterval;

    do {
        if (NULL == ptTimer) {
            break;
        }

        SAFE_CLK_CODE(
            //! get top value
            hwInterval = this.ptReg->MATCH[3];       
        );

        if (0 == hwInterval) {
            break;
        } 

        hwInterval = (PM_GET_SYS_CLK() / hwInterval);
    } while(false);

    return hwInterval;
}                              

/*!\brief tc compare match set
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \param wMatchVal match value
 *! \return bool
 */
static bool tc_match_set (__timer_t *ptTimer, uint8_t chChannel, uint32_t wMatchVal)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
        return false;
    }

    SAFE_CLK_CODE (
        this.ptReg->MATCH[chChannel] = wMatchVal;  
    )

    return true;
}

/*!\brief tc compare match get
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return match value
 */
static uint32_t tc_match_get (__timer_t *ptTimer, uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    uint32_t wResult = 0;

    do {
        if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
            break;
        }   
        SAFE_CLK_CODE (
            wResult = this.ptReg->MATCH[chChannel];
        )
    } while(false);

    return wResult;
}

/*!\brief tc tc_match_int_enable
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return bool
 */
static bool tc_match_int_enable (__timer_t *ptTimer, uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
        return false;
    }

    SAFE_CLK_CODE(
        switch (chChannel) {
            case 0:
                this.ptReg->MCR.Value   |=  MCR_MR0I_MSK; 
                break;
                
            case 1:
                this.ptReg->MCR.Value   |=  MCR_MR1I_MSK;
                break;
                
            case 2:
                this.ptReg->MCR.Value   |=  MCR_MR2I_MSK;
                break;
                
            case 3:
                this.ptReg->MCR.Value   |=  MCR_MR3I_MSK;
                break;
        }
    )
    
    return true;
}

/*!\brief tc tc_match_int_disable
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return bool
 */
static bool tc_match_int_disable (__timer_t *ptTimer, uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
        return false;
    }
    
    SAFE_CLK_CODE(
        switch (chChannel) {
            case 0:
                this.ptReg->MCR.Value   &=  ~MCR_MR0I_MSK; 
                break;
                
            case 1:
                this.ptReg->MCR.Value   &=  ~MCR_MR1I_MSK;
                break;
                
            case 2:
                this.ptReg->MCR.Value   &=  ~MCR_MR2I_MSK;
                break;
                
            case 3:
                this.ptReg->MCR.Value   &=  ~MCR_MR3I_MSK;
                break;
        }       
    )
    
    return true;
}

/*!\brief tc tc_match_is_compare
 *! \param ptTimer timet object
 *! \param chChannal channel number
 *! \return bool
 */
static bool tc_match_is_compare (__timer_t *ptTimer, uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;   
    bool bResult = false;
    do {
        if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
            break;
        }
    
        SAFE_CLK_CODE(
            uint8_t chMsk = 1 << chChannel;

            if (this.ptReg->IR.Value & chMsk) {
                this.ptReg->IR.Value |= chMsk;
                bResult = true;
            } 
        )
    } while (false);

    return bResult;
}


/*!\brief pwm config
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \param chMode mode value
 *! \return bool
 */

#define TC_MAX_DUTY_CYCLE   (65535ul)

static bool tc_pwm_config (__timer_t *ptTimer, 
                           uint8_t chChannel,
                           uint32_t wDuty,
                           uint8_t chMode)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    
    if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
        return false;
    }
    
    if ((TC_MAX_CHANNEL < chChannel)  ||  (TC_MAX_DUTY_CYCLE < wDuty)) {
        return false;
    }
    SAFE_CLK_CODE(
        this.ptReg->PWMC.Value  |=  (1 << chChannel);
        this.ptReg->MATCH[chChannel]  =   wDuty;
    )
//    (uint32_t *)(&(this.ptReg->MR0))[chChannel] =   wDuty;
//    switch (chChannel) {
//        case TC_PWM_CHANNEL0:
//            this.ptReg->MR0     =   wDuty;
//            break;
//            
//        case TC_PWM_CHANNEL1:
//            this
//            break;
//    }
    
    return true;
}

/*!\brief tc_pwm_duty_get
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return duty value
 */
static uint32_t tc_pwm_duty_get (__timer_t *ptTimer, uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    uint32_t wResult = 0;
    do {
        if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
            break;
        }
        SAFE_CLK_CODE(
            wResult = this.ptReg->MATCH[chChannel];
        )
    } while(false);
    return wResult;
}

/*!\brief tc_pwm_duty_set
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \param wDuty duty cycle
 *! \return bool
 */
static bool tc_pwm_duty_set (__timer_t *ptTimer, 
                             uint8_t chChannel, 
                             uint32_t wDuty)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;
    
    if (NULL == ptThis || chChannel > TC_MAX_CHANNEL) {
        return false;
    }

    SAFE_CLK_CODE (
        this.ptReg->MATCH[chChannel]   =   wDuty;
    )

    return true;
}

/*!\brief tc_pwm_mode_get
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return bool
 */
static uint8_t tc_pwm_mode_get (__timer_t *ptTimer, uint8_t chChannel)
{
#if 0
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if ((NULL == ptThis) || (chChannel > TC_MAX_CHANNEL)) {
        return 1;
    }    
    
#else
    return 1;
#endif
}

/*!\brief tc_pwm_mode_set
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \param chModeVal mode value
 *! \return bool
 */
static bool tc_pwm_mode_set (__timer_t *ptTimer, 
                             uint8_t chChannel, 
                             uint8_t chModeVal)
{
#if 0    
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;

    if ((NULL == ptThis) || (chChannel > TC_MAX_CHANNEL)) {
        return false;
    }
    /*! add code here */
    return true;
#else
    return false;
#endif
}


/*!\brief tc capture config
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \param tMode capture mode
 *! \return bool
 */
static bool tc_cap_config (__timer_t *ptTimer,
                           uint8_t chChannel, 
                           em_cap_mode_t chMode)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;    
    
    if (NULL == ptThis || chChannel > 0) {
        return false;
    }
    SAFE_CLK_CODE (
        this.ptReg->CCR.Value   |=  ((1 << chMode));
    )
    
    return true;
    
}

/*!\brief tc capture get
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return capture tc value
 */
static uint32_t tc_cap_get (__timer_t *ptTimer,
                            uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;    
    uint32_t wResult = 0;
    do {
        if (NULL == ptThis || chChannel > 0) {
            break;
        }
        SAFE_CLK_CODE (
            wResult = ((uint32_t *)(&(this.ptReg->CR0)))[chChannel];
        )
    } while(false);

    return wResult;
}

/*!\brief tc capture interrupt enable
 *! \param ptTimer timet object
 *! \param chChannel channel number
 *! \return bool
 */
static bool tc_cap_is_captured (__timer_t *ptTimer,
                                uint8_t chChannel)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;    
    bool bResult = false;
    do {
        if (NULL == ptThis || chChannel > 0) {
            break;
        }

        SAFE_CLK_CODE (
            if (this.ptReg->IR.Value & TIMER0_IR_CR0INT_MSK) {
                this.ptReg->IR.Value |= TIMER0_IR_CR0INT_MSK;
                bResult = true;
            } 
        )
    } while(false);

    return bResult;

}

/*!\brief tc capture interrupt enable
 *! \param ptTimer timet object
 *! \return bool
 */
static bool tc_cap_int_enable (__timer_t *ptTimer,
                               uint8_t chMsk)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;    
    
    if (NULL == ptThis) {
        return false;
    }    
    SAFE_CLK_CODE (
        this.ptReg->CCR.Value   |=  (1 << chMsk);
    )
    return true;
    
}

/*!\brief tc cap interrupt disable
 *! \param ptTimer timer object 
 *! \return bool
 */
static bool tc_cap_int_disable (__timer_t *ptTimer,
                                uint8_t chMsk)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)ptTimer;    
    
    if (NULL == ptThis) {
        return false;
    }    
    SAFE_CLK_CODE (
        this.ptReg->CCR.Value   &=  ~(1 << chMsk);
    )
    
    return true;
}                                                


//! function list
MREPEAT(TIMER_COUNT, TIMER_FUNCTION_TABLE, NULL)
            
