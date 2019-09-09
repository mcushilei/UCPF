/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not, see http://www.gnu.org/licenses/.        *
*******************************************************************************/


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include "..\sysctrl\sc.h"
#include ".\reg_timer.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define TIMER_CLASS(__N, __A)                                               \
    {                                                                       \
        ((timer_reg_t *)(CPU_TIMER##__N##_BASE_ADDRESS)),                   \
    },

#define TIMER_FUNCTION(__N, __A)                                            \
    bool timer##__N##_init(void)                                            \
    {                                                                       \
        return timer_init(__N);                                             \
    }                                                                       \
                                                                            \
    bool timer##__N##_check_idle(void)                                      \
    {                                                                       \
        return timer_check_idle(__N);                                       \
    }                                                                       \
                                                                            \
    bool timer##__N##_enable(void)                                          \
    {                                                                       \
        return timer_enable(__N);                                           \
    }                                                                       \
                                                                            \
    bool timer##__N##_disable(void)                                         \
    {                                                                       \
        return timer_disable(__N);                                          \
    }                                                                       \
                                                                            \
    bool timer##__N##_start(void)                                           \
    {                                                                       \
        return timer_start(__N);                                            \
    }                                                                       \
                                                                            \
    bool timer##__N##_stop(void)                                            \
    {                                                                       \
        return timer_stop(__N);                                             \
    }                                                                       \
                                                                            \
    bool timer##__N##_interrupt_enalbe(void)                                \
    {                                                                       \
        return timer_interrupt_enable(__N);                                 \
    }                                                                       \
                                                                            \
    bool timer##__N##_interrupt_disable(void)                               \
    {                                                                       \
        return timer_interrupt_disable(__N);                                \
    }                                                                       \
                                                                            \
    bool timer##__N##_overflow_flag_get(void)                               \
    {                                                                       \
        return timer_overflow_flag_get(__N);                                \
    }                                                                       \
                                                                            \
    bool timer##__N##_overflow_flag_clear(void)                             \
    {                                                                       \
        return timer_overflow_flag_clear(__N);                              \
    }                                                                       \
                                                                            \
    bool timer##__N##_period_set(uint32_t wPeriod)                          \
    {                                                                       \
        return timer_period_set(__N, wPeriod);                              \
    }                                                                       \
                                                                            \
    uint32_t timer##__N##_period_get(void)                                  \
    {                                                                       \
        return timer_period_get(__N);                                       \
    }                                                                       \
                                                                            \
    bool timer##__N##_prescale_set(uint16_t hwPrescale)                     \
    {                                                                       \
        return timer_prescale_set(__N, hwPrescale);                         \
    }                                                                       \
                                                                            \
    uint16_t timer##__N##_prescale_get(void)                                \
    {                                                                       \
        return timer_prescale_get(__N);                                     \
    }                                                                       \

#define TIMER_FUNCTION_PROTOTYPE(__N, __A)                          \
    extern bool timer##__N##_init(void);                            \
    extern bool timer##__N##_check_idle(void);                      \
    extern bool timer##__N##_enable(void);                          \
    extern bool timer##__N##_disable(void);                         \
    extern bool timer##__N##_start(void);                           \
    extern bool timer##__N##_stop(void);                            \
    extern bool timer##__N##_interrupt_enalbe(void);                \
    extern bool timer##__N##_interrupt_disable(void);               \
    extern bool timer##__N##_overflow_flag_get(void);               \
    extern bool timer##__N##_overflow_flag_clear(void);             \
    extern bool     timer##__N##_period_set(uint32_t wPeriod);          \
    extern uint32_t timer##__N##_period_get(void);                      \
    extern bool     timer##__N##_prescale_set(uint16_t hwPrescale);     \
    extern uint16_t timer##__N##_prescale_get(void);                    \

#define TIMER_INTERFACE(__N, __A)                                           \
    {                                                                       \
        &timer##__N##_init,                                                 \
        &timer##__N##_check_idle,                                           \
        &timer##__N##_enable,                                               \
        &timer##__N##_disable,                                              \
        &timer##__N##_start,                                                \
        &timer##__N##_stop,                                                 \
        {                                                                   \
            &timer##__N##_interrupt_enalbe,                                 \
            &timer##__N##_interrupt_disable,                                \
        },                                                                  \
        {                                                                   \
            &timer##__N##_overflow_flag_get,                                \
            &timer##__N##_overflow_flag_clear,                              \
        },                                                                  \
        {                                                                   \
            &timer##__N##_period_set,                                       \
            &timer##__N##_period_get,                                       \
        },                                                                  \
        {                                                                   \
            &timer##__N##_prescale_set,                                     \
            &timer##__N##_prescale_get,                                     \
        },                                                                  \
        ((timer_reg_t *)(CPU_TIMER##__N##_BASE_ADDRESS)),                   \
    },

/*============================ TYPES =========================================*/
DEF_CLASS(__timer_t)
    timer_reg_t *const    ptREG;
END_DEF_CLASS(__timer_t)

DEF_INTERFACE(i_interrupt_t)
    bool    (*Enable)(void);
    bool    (*Disable)(void);
END_DEF_INTERFACE(i_interrupt_t)

DEF_INTERFACE(i_overflow_flag_t)
    bool    (*Get)(void);
    bool    (*Clear)(void);
END_DEF_INTERFACE(i_overflow_flag_t)

DEF_INTERFACE(i_period_t)
    bool        (*Set)(uint32_t wPeriod);
    uint32_t    (*Get)(void);
END_DEF_INTERFACE(i_period_t)

DEF_INTERFACE(i_prescale_t)
    bool        (*Set)(uint16_t hwPrescale);
    uint16_t    (*Get)(void);
END_DEF_INTERFACE(i_prescale_t)

DEF_INTERFACE(i_timer_t)
    bool        (*Init)(void);
    bool        (*CheckIdle)(void);
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    bool        (*Start)(void);
    bool        (*Stop)(void);

    i_interrupt_t       Interrupt;
    i_overflow_flag_t   OverflowFlag;
    i_period_t          Period;
    i_prescale_t        Prescale;

    timer_reg_t *const RegPage;
END_DEF_INTERFACE(i_timer_t)


/*============================ PROTOTYPES ====================================*/
MREPEAT(TIMER_COUNT, TIMER_FUNCTION_PROTOTYPE, 0)

/*============================ LOCAL VARIABLES ===============================*/
static const CLASS(__timer_t) __TIMER[TIMER_COUNT] = {
    MREPEAT(TIMER_COUNT, TIMER_CLASS, 0)
};

/*============================ GLOBAL VARIABLES ==============================*/
const i_timer_t TIMER[TIMER_COUNT] = {
    MREPEAT(TIMER_COUNT, TIMER_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/

static bool timer_init(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR = TIMER_TCR_TIF_MSK | TIMER_TCR_TSS_MSK;

    return true;
}

static bool timer_check_idle(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    if (this.ptREG->TCR & TIMER_TCR_TSS_MSK) {
        return true;
    }

    return false;
}

static bool timer_enable(uint16_t hwTimer)
{
    return true;
}

static bool timer_disable(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR = (this.ptREG->TCR & ~TIMER_TCR_TIF_MSK)
                    | TIMER_TCR_TSS_MSK;

    return true;
}

static bool timer_start(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR &= ~(TIMER_TCR_TIF_MSK | TIMER_TCR_TSS_MSK);

    return true;
}

static bool timer_stop(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR = (this.ptREG->TCR & ~TIMER_TCR_TIF_MSK)
                    | TIMER_TCR_TSS_MSK;

    return true;
}

static bool timer_interrupt_enable(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR = (this.ptREG->TCR & ~TIMER_TCR_TIF_MSK)
                    | TIMER_TCR_TIE_MSK;

    return true;
}

static bool timer_interrupt_disable(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR &= ~(TIMER_TCR_TIF_MSK | TIMER_TCR_TIE_MSK);

    return true;
}

static bool timer_overflow_flag_get(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    if (this.ptREG->TCR & TIMER_TCR_TIF_MSK) {
        return true;
    }

    return false;
}

static bool timer_overflow_flag_clear(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TCR |= TIMER_TCR_TIF_MSK;

    return false;
}

static bool timer_period_set(uint16_t hwTimer, uint32_t wPeriod)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->PRD.Value = wPeriod;

    return true;
}

static uint32_t timer_period_get(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return 0;
    }

    return this.ptREG->PRD.Value;
}

static bool timer_prescale_set(uint16_t hwTimer, uint16_t hwPrescale)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return false;
    }

    this.ptREG->TPR.TDDR = hwPrescale;
    this.ptREG->TPRH.TDDRH = hwPrescale >> 8;

    return true;
}

static uint16_t timer_prescale_get(uint16_t hwTimer)
{
    CLASS(__timer_t) *ptThis = (CLASS(__timer_t) *)&__TIMER[hwTimer];

    if (hwTimer >= TIMER_COUNT) {
        return 0;
    }

    return this.ptREG->TPR.TDDR | (this.ptREG->TPRH.TDDRH << 8);
}

MREPEAT(TIMER_COUNT, TIMER_FUNCTION, 0)

/* EOF */
