/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
ROOT int BEFORE_HW_INIT(void)
{
    /* Return 1 to indicate that normal segment */
    /* initialization should be performed. If   */
    /* normal segment initialization should not */
    /* be performed, return 0.                  */

    return 1;
}

/*! \brief  hardware initialization
 *! \param  none
 *! \retval true run the default initialization
 *! \retval false ignore the default initialization
 */
WEAK bool ON_HW_INIT(void)
{
    return true;
}


bool board_init(void)
{
    DISABLE_GLOBAL_INTERRUPT();
    
    //
    // Set the clock frequency.
    //
    driver_clkgen_sysclk_select(DRIVER_CLKGEN_SYSCLK_24MHZ);

    //
    // Set the default cache configuration
    //
    driver_cachectrl_enable(&driverCachectrlDefaults);

    //
    // Enable internal buck converters.
    //
    driver_pwrctrl_bucks_init();

    //
    // Initialize for low power in the power control block
    //
    driver_pwrctrl_low_power_init();

    //
    // Turn off the voltage comparator as this is enabled on reset.
    //
    driver_vcomp_disable();
    
    driver_clkgen_hfrc_adjust_enable(REG_CLKGEN_HFADJ_HFWARMUP_1SEC,
                                     REG_CLKGEN_HFADJ_HFADJCK_16SEC);

    return ON_HW_INIT();
}

/* EOF */
