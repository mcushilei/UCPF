/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_COMMON_DEVICE_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_COMMON_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \brief Register bitfield value operations. Those macro is used for creating a
//!             value, they do not modify any register.
//! REG_BFVS    Move a value to a bitfield.
//! REG_BFVG    Extract the value of a bitfield from a 32-bit value, such as that
//!             read from a register.
//! REG_BFVM    Mask a bitfield.
//!
#define REG_BFVS(part, reg, field, value)                               \
    ( ((uint32_t)(value) << REG_##part##_##reg##_##field##_S) &         \
      REG_##part##_##reg##_##field##_M )

#define REG_BFVG(part, reg, field, value)                               \
    ( ((uint32_t)(value) & REG_##part##_##reg##_##field##_M) >>         \
      REG_##part##_##reg##_##field##_S )

#define REG_BFVM(part, reg, field)                                      \
    ( REG_##part##_##reg##_##field##_M )
        
//! \brief Register access macros for single-instance modules
//! REG_BFW     Write a value to a bitfield of a register.
//! REG_BFS     Set a bitfield of a register.
//! REG_BFC     Clear a bitfield of a register.
//!
//! REG_BFR     Read a bitfield value from a register.
//! REG_BFM     Read and mask a bitfield from a register.
//!
#define REG_BFW(part, reg, field, value)                                \
    REG_##part.reg =                                                    \
    ( REG_##part.reg & (~REG_##part##_##reg##_##field##_M) )  |         \
    ( ((uint32_t)(value) << REG_##part##_##reg##_##field##_S) &         \
      REG_##part##_##reg##_##field##_M )
        
#define REG_BFS(part, reg, field)                                       \
    REG_##part.reg |= REG_##part##_##reg##_##field##_M

#define REG_BFC(part, reg, field)                                       \
    REG_##part.reg &= ~REG_##part##_##reg##_##field##_M

#define REG_BFR(part, reg, field)                                       \
    ( ((uint32_t)REG_##part.reg & REG_##part##_##reg##_##field##_M) >>  \
      REG_##part##_##reg##_##field##_S )

#define REG_BFM(part, reg, field)                                       \
    ( REG_##part.reg & REG_##part##_##reg##_##field##_M)

//! \brief Register access macros for multi-instance modules
#define REG_M_BFW(part, instance, reg, field, value)                        \
    REG_##part##n(instance).reg =                                           \
    ( REG_##part##n(instance).reg & (~REG_##part##_##reg##_##field##_M) ) | \
    ( ((uint32_t)(value) << REG_##part##_##reg##_##field##_S)             & \
      REG_##part##_##reg##_##field##_M )
        
#define REG_M_BFS(part, instance, reg, field)                               \
    REG_##part##n(instance).reg |= REG_##part##_##reg##_##field##_M

#define REG_M_BFC(part, instance, reg, field)                               \
    REG_##part##n(instance).reg &= ~REG_##part##_##reg##_##field##_M

#define REG_M_BFR(part, instance, reg, field)                               \
    ( ((uint32_t)REG_##part##n(instance).reg & REG_##part##_##reg##_##field##_M) >>   \
      REG_##part##_##reg##_##field##_S )

#define REG_M_BFM(part, instance, reg, field)                               \
    ( REG_##part##n(instance).reg & REG_##part##_##reg##_##field##_M)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
