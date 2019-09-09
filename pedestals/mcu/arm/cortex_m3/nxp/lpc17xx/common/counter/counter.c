/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./app_cfg.h"
#include "../device.h"
#include "./reg_counter.h"
#include "./counter.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/   
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

/*
 *  \brief  init peripheral dependence
 */
WEAK void __driver_counter_initpd(tmr_reg_t *instance)
{
    //! TODO: enable its clock, pins.
}

/*
 *  \brief  deinit peripheral dependence
 */
WEAK void __driver_counter_deinitpd(tmr_reg_t *instance)
{
    //! TODO: disable its clock, pins.
}


/*! \brief initialize counter
 *! \param instance pointer to the counter
 *! \param cfg counter configuration
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
bool counter_init(tmr_reg_t *instance, driver_counter_cfg_t *cfg)
{
    if (NULL == instance) {
        return false;
    }
    
    __driver_counter_initpd(instance);
    
    instance->TCR = (1u << 1);
    instance->TCR = 0;
    instance->PR    = cfg->ClockPrescal;
    instance->CTCR  = cfg->ClockSource;
    instance->MR0 = cfg->Match0Value;
    instance->MR1 = cfg->Match1Value;
    instance->MR2 = cfg->Match2Value;
    instance->MR3 = cfg->Match3Value;
    instance->MCR = (cfg->Match0Action << 0) | (cfg->Match1Action << 3) | (cfg->Match2Action << 6) | (cfg->Match3Action << 9);
    instance->EMR = (cfg->Match0PinAction << 0) | (cfg->Match1PinAction << 3) | (cfg->Match2PinAction << 6) | (cfg->Match3PinAction << 9);
    
    return true;
}

/*! \brief deinit counter
 *! \param instance pointer to the counter
 *! \retval true counter disable succeed
 *! \retval false counter disable failed
 */
bool counter_deinit(tmr_reg_t *instance)
{
    if (NULL == instance) {
        return false;
    }
    
    
    instance->TCR = 0;

    __driver_counter_deinitpd(instance);
    
    return true;
}  

bool counter_start(tmr_reg_t *instance)
{
    if (NULL == instance) {
        return false;
    }
    
    instance->TCR = 1;
    
    return true;
}

bool counter_stop(tmr_reg_t *instance)
{
    if (NULL == instance) {
        return false;
    }
    
    instance->TCR = 0;
    
    return true;
}

bool counter_clear_int(tmr_reg_t *instance, uint32_t interrupt)
{
    if (NULL == instance) {
        return false;
    }
    
    instance->IR = interrupt;
    
    return true;
}
