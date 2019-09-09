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

#ifndef __DRIVER_LPC17XX_REG_IOCTRL_H__
#define __DRIVER_LPC17XX_REG_IOCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PINCON_REG              (*(pincon_reg_t    *) PINCON_BASE   )

//! \name pin mode  macros
//! @{
enum {
    IO_PULL_UP      = 0x00,
    IO_PULL_REPEATER,
    IO_PULL_NONE,
    IO_PULL_DOWN,
};

#define IO_MOD_OPEN_DRAIN   (0x04)
//! @}

//! \brief IO functions
//! @{
enum io_func_sel_t {
    IO_WORKS_AS_FUNC0   = 0x00,
    IO_WORKS_AS_FUNC1   = 0x01,
    IO_WORKS_AS_FUNC2   = 0x02,
    IO_WORKS_AS_FUNC3   = 0x03,
};
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
typedef struct {
    reg32_t     PINSEL[11];
    REG32_RSVD(5)
    reg32_t     PINMODE[10];
    reg32_t     PINMODE_OD[5];
    reg32_t     I2CPADCFG;
} pincon_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */