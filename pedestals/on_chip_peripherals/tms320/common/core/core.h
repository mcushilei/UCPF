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



#ifndef __TMS320_CORE_H__
#define __TMS320_CORE_H__


#define CPU_GLOBAL_INTERRUPT_ENABLE()   ASM(" clrc INTM")
#define CPU_GLOBAL_INTERRUPT_DISABLE()  ASM(" setc INTM")
#define DEBUG_ENABLE()                  ASM(" clrc DBGM")
#define DEBUG_DISABLE()                 ASM(" setc DBGM")
#define EALLOW()                        ASM(" EALLOW")
#define EDIS()                          ASM(" EDIS")
#define ESTOP0()                        ASM(" ESTOP0")

extern __cregister volatile unsigned int IFR;
extern __cregister volatile unsigned int IER;


static inline void cpu_interrupt_enable(uint16_t hwIndex)
{
    if (hwIndex == 0) {
        return;
    }
    IER |= (1u << --hwIndex);
}

static inline void cpu_interrupt_disable(uint16_t hwIndex)
{
    if (hwIndex == 0) {
        return;
    }
    IER &= ~(1u << --hwIndex);
}

#endif
