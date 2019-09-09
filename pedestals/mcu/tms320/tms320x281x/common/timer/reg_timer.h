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


#ifndef __REG_TIMER_H__
#define __REG_TIMER_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#ifdef  CPU_TIMER0_BASE_ADDRESS
#define TIMER0_REG          (*(timer_reg_t *)CPU_TIMER0_BASE_ADDRESS)
#endif

#ifdef  CPU_TIMER1_BASE_ADDRESS
#define TIMER1_REG          (*(timer_reg_t *)CPU_TIMER1_BASE_ADDRESS)
#endif

#ifdef  CPU_TIMER2_BASE_ADDRESS
#define TIMER2_REG          (*(timer_reg_t *)CPU_TIMER2_BASE_ADDRESS)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct {
    // Timer counter register
    DEF_REG32
        reg32_t     LSW:16;
        reg32_t     MSW:16;
    END_DEF_REG32(TIM)

    // Period register
    DEF_REG32
        reg32_t     LSW:16;
        reg32_t     MSW:16;
    END_DEF_REG32(PRD)

    // Timer control register
    reg16_t     TCR;

#define TIMER_TCR_TSS_BIT0      4
#define TIMER_TCR_TSS_MSK       (1u << TIMER_TCR_TSS_BIT0)
#define TIMER_TCR_TRB_BIT0      5
#define TIMER_TCR_TRB_MSK       (1u << TIMER_TCR_TRB_BIT0)
#define TIMER_TCR_SOFT_BIT0     10
#define TIMER_TCR_SOFT_MSK      (1u << TIMER_TCR_SOFT_BIT0)
#define TIMER_TCR_FREE_BIT0     11
#define TIMER_TCR_FREE_MSK      (1u << TIMER_TCR_FREE_BIT0)
#define TIMER_TCR_TIE_BIT0      14
#define TIMER_TCR_TIE_MSK       (1u << TIMER_TCR_TIE_BIT0)
#define TIMER_TCR_TIF_BIT0      15
#define TIMER_TCR_TIF_MSK       (1u << TIMER_TCR_TIF_BIT0)

    REG16_RSVD(1)       // reserved

    // Timer pre-scale low
    DEF_REG16
        reg16_t     TDDR:8;     // 7:0   Divide-down low
        reg16_t     PSC :8;     // 15:8  Prescale counter low
    END_DEF_REG16(TPR)

    // Timer pre-scale high
    DEF_REG16
        reg16_t     TDDRH:8;    // 7:0   Divide-down high
        reg16_t     PSCH :8;    // 15:8  Prescale counter high
    END_DEF_REG16(TPRH)
} timer_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
