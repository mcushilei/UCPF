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

#ifndef __REG_TIMER_H__
#define __REG_TIMER_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define TIMER0_REG                (*(tmr_reg_t *) TIM0_BASE_ADDRESS)
#define TIMER1_REG                (*(tmr_reg_t *) TIM1_BASE_ADDRESS)
#define TIMER2_REG                (*(tmr_reg_t *) TIM2_BASE_ADDRESS)
#define TIMER3_REG                (*(tmr_reg_t *) TIM3_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/     
/*============================ TYPES =========================================*/

//! \brief system control block register page
//! @{
typedef volatile struct {
    reg32_t IR;
    reg32_t TCR;
    reg32_t TC;
    reg32_t PR;
    reg32_t PC;
    reg32_t MCR;
    reg32_t MR0;
    reg32_t MR1;
    reg32_t MR2;
    reg32_t MR3;
    reg32_t CCR;
    reg32_t CR0;
    reg32_t CR1;
    REG32_RSVD(2)
    reg32_t EMR;
    REG32_RSVD(12)
    reg32_t CTCR;
} tmr_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
