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


#ifndef __I_IO_SLEEP_H__
#define __I_IO_SLEEP_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PMU_REG                     (*(volatile pmu_reg_t *)PMU_BASE_ADDRESS)

#define PMU_PCON_DPDEN_BIT0         1
#define PMU_PCON_DPDEN_MSK          (1u << PMU_PCON_DPDEN_BIT0)

#define PMU_PCON_SLEEPFLAG_BIT0     8
#define PMU_PCON_SLEEPFLAG_MSK      (1u << PMU_PCON_SLEEPFLAG_BIT0)

#define PMU_PCON_DPDFLAG_BIT0       11
#define PMU_PCON_DPDFLAG_MSK        (1u << PMU_PCON_DPDFLAG_BIT0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type 
//! @{
typedef struct {
    DEF_REG32
        reg32_t             :1;
        reg32_t DPDEN       :1;
        reg32_t             :6;
        reg32_t SLEEPFLAG   :1;
        reg32_t             :2;
        reg32_t DPDFLAG     :1;
        reg32_t             :20;
    END_DEF_REG32(PCON) 
        
    reg32_t GPREG0;
    reg32_t GPREG1;
    reg32_t GPREG2;
    reg32_t GPREG3;
        
    DEF_REG32  
        reg32_t             :10;
        reg32_t WAKEUPHYS   :1;
        reg32_t RTCCLK      :4;
        reg32_t             :17;
    END_DEF_REG32(SYSCFG)        
} pmu_reg_t;
//! @}



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif