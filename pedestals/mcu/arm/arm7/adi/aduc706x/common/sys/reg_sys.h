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

#ifndef __REG_SYS_H__
#define __REG_SYS_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IRQ_REG                 (*(irq_reg_t *)IRQ_BASE_ADDRESS)
#define SYS_REG                 (*(sys_reg_t *)SYS_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \brief irq register page
//! @{
typedef volatile struct {

    reg32_t     IRQSTA;
    reg32_t     IRQSIG;
    reg32_t     IRQEN;
    reg32_t     IRQCLR;
    reg32_t     SWICFG;
    reg32_t     IRQBASE;
    REG32_RSVD(1) //!< Reserved
    reg32_t     IRQVEC;
    reg32_t     IRQP0;
    reg32_t     IRQP1;
    reg32_t     IRQP2;
    REG32_RSVD(1) //!< Reserved
    reg32_t     IRQCONN;
    reg32_t     IRQCONE;
    reg32_t     IRQCLRE;
    reg32_t     IRQSTAN;
    REG32_RSVD(48) //!< Reserved
    reg32_t     FIQSTA;
    reg32_t     FIQSIG;
    reg32_t     FIQEN;
    reg32_t     FIQCLR;
    REG32_RSVD(3) //!< Reserved
    reg32_t     FIQVEC;
    REG32_RSVD(7) //!< Reserved
    reg32_t     FIQSTAN;

} irq_reg_t;
//! @}

//! \brief system control block register page
//! @{
typedef volatile struct {
    
    REG32_RSVD(8) //!< Reserved
    reg8_t      REMAP;

    REG8_RSVD(15) //!< Reserved
    reg8_t      RSTSTA;

    REG8_RSVD(3) //!< Reserved
    reg8_t      RSTCLR;
        
} sys_reg_t; 
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
