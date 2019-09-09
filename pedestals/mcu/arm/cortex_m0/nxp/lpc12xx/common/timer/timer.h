
#ifndef __DRIVER_CORTEX_M0_ES_COMMON_TIMER_H__
#define __DRIVER_CORTEX_M0_ES_COMMON_TIMER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_timer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \note timer capture mode
typedef enum {
    TC_CAP_RISING_EDGE  = (0ul),
    TC_CAP_FAILING_EDGE = (1ul),
} em_cap_mode_t;

//! \name timer configuration structure
typedef struct{
    uint16_t            hwMode;                     //!< timer working mode
} tc_cfg_t;

//! \name timer capture structure
typedef struct{
    uint16_t            hwMode;                     //!< timer working mode
} tc_cap_cfg_t;

typedef enum {
    TMR_CHN0    = 0,
    TMR_CHN1    = 1,
    TMR_CHN2    = 2,

    TMR_PWM0    = 0,
    TMR_PWM1    = 1,
    TMR_PWM2    = 2
} em_tmr_chn_t;

//! \name class: timer interval
DEF_INTERFACE(i_tmr_interval_t)
    u16_property_t      TOP;                        //!< top function
    en_property_t       Interrupt;                  //!< top interrupt function
    bool                (*IsOverflow)(void);        //!< get top overflow status
END_DEF_INTERFACE(i_tmr_interval_t)

//! \name i_tmr_pwm_t
DEF_INTERFACE (i_tmr_duty_t)
    bool        (*Set)(em_tmr_chn_t tChannel, uint32_t wDutyVal);
    uint32_t    (*Get)(em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_duty_t)

//! \name i_tmr_mode_t
DEF_INTERFACE (i_tmr_mode_t)
    bool        (*Set)(em_tmr_chn_t tChannel, uint8_t chModeVal);
    uint8_t     (*Get)(em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_mode_t)

//! \name i_tmr_pwm_t
DEF_INTERFACE (i_tmr_pwm_t)
    bool            (*Config)(em_tmr_chn_t tChannel,uint32_t wDuty,uint8_t chMode);
    i_tmr_duty_t    Duty;
    i_tmr_mode_t    Mode;
END_DEF_INTERFACE (i_tmr_pwm_t)

//! \name i_tmr_match_int_t
DEF_INTERFACE (i_tmr_match_int_t)
    bool    (*Enable)(em_tmr_chn_t);             //!< match interrupt enable
    bool    (*Disable)(em_tmr_chn_t);             //!< match interrupt disable
END_DEF_INTERFACE (i_tmr_match_int_t)

//! \name i_tmr_match_t
DEF_INTERFACE (i_tmr_match_t)
    bool        (*Set)(em_tmr_chn_t tChannel, uint32_t wMatchVal);
    uint32_t    (*Get)(em_tmr_chn_t tChannel);
    i_tmr_match_int_t   Interrupt;
    bool        (*IsCompareMatch)(em_tmr_chn_t tChannel);
END_DEF_INTERFACE (i_tmr_match_t)

//! \name i_tmr_capture_t;
DEF_INTERFACE (i_tmr_capture_t)
    bool            (*Config)(em_cap_mode_t chMode);
    uint32_t        (*Get)(void);
    bool            (*IsCaptured)(void);
    en_property_t   Interrupt;
END_DEF_INTERFACE (i_tmr_capture_t)

//! \name class: timer base
DEF_INTERFACE(i_tmr_base_t)
    bool (*Init)(tc_cfg_t *ptCfg);           //!< initialize timer
    bool (*Idle)(void);                      //!< get busy flag status
    bool (*Enable)(void);                    //!< enable timer 
    bool (*Disable)(void);                   //!< disable timer
    bool (*Start)(void);                     //!< start timer
    bool (*Stop)(void);                      //!< stop timer

    u16_property_t  Frequency;                      //!< frequency function

    timer_reg_t * const ptRegPage;                  //!< reference to register page
END_DEF_INTERFACE(i_tmr_base_t)

//! \name class: timer 
DEF_INTERFACE(i_tmr_t) IMPLEMENT(i_tmr_base_t)
    i_tmr_interval_t    Interval;                  //!< timer interval function
    i_tmr_pwm_t         PWM;                       //!< timer PWM function
    i_tmr_match_t       Match;                     //!< timer match function
    i_tmr_capture_t     Capture;                   //!< timer capture function
END_DEF_INTERFACE(i_tmr_t)

/*============================ PROTOTYPES ====================================*/
extern const i_tmr_t TIMER[TIMER_COUNT];


#endif
/* EOF */