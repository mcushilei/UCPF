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

#ifndef __DRIVER_REG_FLASH_H__
#define __DRIVER_REG_FLASH_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define FLASH_REG                         (*(flash_reg_t *)FLASH_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
//! Flash Register Page
DEF_REGPAGE
    reg16_t         FEESTA;
    REG16_RSVD(1)
        
    reg16_t         FEEMOD;
    REG16_RSVD(1)

    reg8_t          FEECON;
    REG8_RSVD(1)
    REG16_RSVD(1)

    reg16_t         FEEDAT;
    REG16_RSVD(1)

    reg32_t         FEEADR;

    REG32_RSVD(1)

    reg32_t         FEESIG;

    reg32_t         FEEPRO;

    reg32_t         FEEHID;

END_DEF_REGPAGE(flash_reg_t)

#endif
/* EOF */
