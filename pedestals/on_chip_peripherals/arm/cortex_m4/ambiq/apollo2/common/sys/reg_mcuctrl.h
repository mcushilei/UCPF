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


#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_MCUCTRL_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_REG_MCUCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define MCUCTRL_REG                     (*(mcuctrl_reg_t *)REG_MCUCTRL_BASEADDR)

//*****************************************************************************
//
// PWRCTRL_MEMEN - Disables individual banks of the MEMORY array
//
//*****************************************************************************

// Enables power for selected SRAM banks (else an access to its address space to
// generate a Hard Fault).
#define REG_PWRCTRL_MEMEN_SRAMEN(n)               ( ( (uint32_t)(n) <<              \
                                                    REG_PWRCTRL_MEMEN_SRAMEN_S ) &  \
                                                    REG_PWRCTRL_MEMEN_SRAMEN_M )
#define REG_PWRCTRL_MEMEN_SRAMEN_NONE             REG_PWRCTRL_MEMEN_SRAMEN(0x0)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM0     REG_PWRCTRL_MEMEN_SRAMEN(0x1)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM1     REG_PWRCTRL_MEMEN_SRAMEN(0x2)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM2     REG_PWRCTRL_MEMEN_SRAMEN(0x4)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM3     REG_PWRCTRL_MEMEN_SRAMEN(0x8)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP1           REG_PWRCTRL_MEMEN_SRAMEN(0x10)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP2           REG_PWRCTRL_MEMEN_SRAMEN(0x20)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP3           REG_PWRCTRL_MEMEN_SRAMEN(0x40)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP4           REG_PWRCTRL_MEMEN_SRAMEN(0x80)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP5           REG_PWRCTRL_MEMEN_SRAMEN(0x100)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP6           REG_PWRCTRL_MEMEN_SRAMEN(0x200)
#define REG_PWRCTRL_MEMEN_SRAMEN_GROUP7           REG_PWRCTRL_MEMEN_SRAMEN(0x400)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM8K           REG_PWRCTRL_MEMEN_SRAMEN(0x1)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM16K          REG_PWRCTRL_MEMEN_SRAMEN(0x3)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM32K          REG_PWRCTRL_MEMEN_SRAMEN(0xF)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM64K          REG_PWRCTRL_MEMEN_SRAMEN(0x1F)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K         REG_PWRCTRL_MEMEN_SRAMEN(0x7F)
#define REG_PWRCTRL_MEMEN_SRAMEN_SRAM256K         REG_PWRCTRL_MEMEN_SRAMEN(0x7FF)
#define REG_PWRCTRL_MEMEN_SRAMEN_ALL              REG_PWRCTRL_MEMEN_SRAMEN(0x7FF)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name register page type 
//! @{
typedef volatile struct {
	reg32_t CHIP_INFO;
	reg32_t CHIPID0;
	reg32_t CHIPID1;
	reg32_t CHIPREV;
	reg32_t VENDORID;
	reg32_t DEBUGGER;
    REG32_RSVD(18)
    
	reg32_t BUCK;
	reg32_t BUCK2;
	reg32_t BUCK3;
    REG32_RSVD(6)
    
	reg32_t LDOREG2;
    REG32_RSVD(30)
    
	reg32_t BODPORCTRL;
    REG32_RSVD(2)
    
	reg32_t ADCCAL;
	reg32_t ADCBATTLOAD;
    REG32_RSVD(2)
	reg32_t ADCREFCOMP;
    REG32_RSVD(1)
    
	reg32_t XTALGENCTRL;
    REG32_RSVD(30)
    
	reg32_t BOOTLOADERLOW;
	reg32_t SHADOWVALID;
    REG32_RSVD(6)
    
	reg32_t ICODEFAULTADDR;
	reg32_t DCODEFAULTADDR;
	reg32_t SYSFAULTADDR;
	reg32_t FAULTSTATUS;
	reg32_t FAULTCAPTUREEN;
    REG32_RSVD(11)
    
	reg32_t DBGR1;
	reg32_t DBGR2;
    REG32_RSVD(6)
        
	reg32_t PMUENABLE;
    REG32_RSVD(11)
        
	reg32_t TPIUCTRL;
} mcuctrl_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
