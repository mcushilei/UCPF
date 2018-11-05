/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_TIMER_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_counter.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
//! \brief counter channel max counter
#define TC_MAX_CHANNEL          (2ul)


#define this        (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/   

//! \brief safe clock code 
#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t tStatus = peripheral_clock_get_status(this.tPCON);             \
        peripheral_clock_enable(this.tPCON);                                    \
        __VA_ARGS__;                                                            \
        peripheral_clock_resume_status(this.tPCON, tStatus);                    \
    }

#define __TIMER_OBJ(__N, __A)                                                \
    {                                                                       \
        ((tmr_reg_t *)(TIM##__N##_BASE_ADDRESS)),                           \
        PCONP_TIM##__N,                                                    \
        PCLK_TIM##__N,                                                     \
    },

#define __TIMER_FUNCTION(__N, __A)                                              \
    bool counter##__N##_init(void)                                                \
    {                                                                           \
        return counter_init(&__TIMER[__N]);                                       \
    }                                                                           \
    bool counter##__N##_enable(void)  \
    {                                   \
        return counter_enable(&__TIMER[__N]);    \
    }                                           \
    bool counter##__N##_disable(void) \
    {                                   \
        return counter_disable(&__TIMER[__N]);    \
    }                                           \
    bool counter##__N##_start(void)   \
    {                                               \
        return counter_start(&__TIMER[__N]);         \
    }                                           \
    bool counter##__N##_stop(void)                            \
    {                                       \
        return counter_stop(&__TIMER[__N]);              \
    }                                           \

/*============================ TYPES =========================================*/
//! \name internal class
//! @{
typedef struct {
    tmr_reg_t * const   ptReg;              //!< reference to register page
    const uint32_t      tPCON;              //!< ahbclk info
    const uint32_t      tPCLK;              //!<
} __counter_t;
//! @}

/*============================ PROTOTYPES ====================================*/
static bool counter_init(__counter_t *ptThis);
static bool counter_enable(__counter_t *ptThis);
static bool counter_disable(__counter_t *ptThis);
static bool counter_start(__counter_t *ptThis);
static bool counter_stop(__counter_t *ptThis);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal counter object
static __counter_t __TIMER[] = {
    MREPEAT(TIMER_COUNT, __TIMER_OBJ, NULL)
};

/*============================ IMPLEMENTATION ================================*/

/*! \brief initialize counter
 *! \param ptThis counter object
 *! \param ptTimerCfg counter configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool counter_init(__counter_t *ptThis)
{
    return true;
}

/*! \brief counter enable
 *! \param ptThis counter object
 *! \retval true counter enable succeed
 *! \retval false counter enable failed
 */
static bool counter_enable(__counter_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }

    //! Enable AHB Clock
    peripheral_clock_enable(this.tPCON);

    return true;
}

/*! \brief counter disable
 *! \param ptThis counter object
 *! \retval true counter disable succeed
 *! \retval false counter disable failed
 */
static bool counter_disable(__counter_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }

    //! Disable AHB Clock
    peripheral_clock_disable(this.tPCON);
    
    return true;
}  

/*! \brief counter start count
 *! \param ptThis counter object
 *! \retval true counter start count succeed
 *! \retval false counter start count failed
 */
static bool counter_start(__counter_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }

    SAFE_CLK_CODE(
        this.ptReg->TCR = (1u << 0);
    );

    return true;
}

/*! \brief counter stop count
 *! \param ptThis counter object
 *! \retval true counter stop count succeed
 *! \retval false counter stop count failed
 */
static bool counter_stop(__counter_t *ptThis)
{
    if (NULL == ptThis) {
        return false;
    }

    SAFE_CLK_CODE(
        this.ptReg->TCR = 0;
    );

    return true;
}    


//! function list
MREPEAT(TIMER_COUNT, __TIMER_FUNCTION, NULL)
            
