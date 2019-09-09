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

#ifndef __DRIVER_ARM_ARM7_COMMON_H__
#define __DRIVER_ARM_ARM7_COMMON_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\core\core_arm7.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define ENABLE_GLOBAL_INTERRUPT()   __enable_interrupt()
#define DISABLE_GLOBAL_INTERRUPT()  __disable_interrupt()

//! \brief Enter the safe atom operations
#define ENTER_SAFE_ATOM_CODE()  uint32_t __wState = __get_interrupt_state();    \
                                __disable_interrupt()

//! \brief Exit from the safe atom operations
#define EXIT_SAFE_ATOM_CODE()   __set_interrupt_state(__wState)

//! \brief The safe ATOM code section macro
#define SAFE_ATOM_CODE(...) {   \
        ENTER_SAFE_ATOM_CODE(); \
        __VA_ARGS__             \
        EXIT_SAFE_ATOM_CODE();  \
    }

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
