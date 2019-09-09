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

#ifndef __I_IO_TIMER_H__
#define __I_IO_TIMER_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define TIMER0_REG                       (*(timer0_reg_t *)TIMER0_BASE_ADDRESS)
#define TIMER1_REG                       (*(timer1_reg_t *)TIMER1_BASE_ADDRESS)
#define TIMER2_REG                       (*(timer2_reg_t *)TIMER2_BASE_ADDRESS)
#define TIMER3_REG                       (*(timer3_reg_t *)TIMER3_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/     
/*============================ TYPES =========================================*/
//! \brief timer control block register page
//! @{
typedef volatile struct {
    reg32_t     LD;
    reg32_t     VAL;
    reg32_t     CON;
    reg8_t      CLRI;
    REG8_RSVD(3) //!< Reserved
    reg32_t     CAP;
} timer0_reg_t;

typedef volatile struct {
    reg32_t     LD;
    reg32_t     VAL;
    reg16_t     CON;
    REG16_RSVD(1) //!< Reserved
    reg8_t      CLRI;
    REG8_RSVD(3) //!< Reserved
} timer1_reg_t;

typedef volatile struct {
    reg16_t     LD;
    REG16_RSVD(1) //!< Reserved
    reg16_t     VAL;
    REG16_RSVD(1) //!< Reserved
    reg16_t     CON;
    REG16_RSVD(1) //!< Reserved
    reg8_t      CLRI;
    REG8_RSVD(3) //!< Reserved
} timer2_reg_t;

typedef volatile struct {
    reg16_t     LD;
    REG16_RSVD(1) //!< Reserved
    reg16_t     VAL;
    REG16_RSVD(1) //!< Reserved
    reg32_t     CON;
    reg8_t      CLRI;
    REG8_RSVD(3) //!< Reserved
    reg16_t     CAP;
    REG16_RSVD(1) //!< Reserved
} timer3_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
