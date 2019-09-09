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


//! \note do not move this pre-processor statement to other places
#define __ATOM_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(atom_t)
    bool    bInISR;
    bool    bFlag;
    bool    bFlagISR;
END_DEF_STRUCTURE(atom_t)

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
static volatile atom_t s_tAtom;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void atom_init(void)
{
    s_tAtom.bInISR   = false;
    s_tAtom.bFlag    = false;
    s_tAtom.bFlagISR = false;
}

bool atom_flag_get(void)
{
    if (!(s_tAtom.bInISR)) {
        return s_tAtom.bFlag;
    } else {
        return s_tAtom.bFlagISR;
    }
}

void atom_flag_set(void)
{
    if (!(s_tAtom.bInISR)) {
        s_tAtom.bFlag = true;
    } else {
        s_tAtom.bFlagISR = true;
    }
}

void atom_flag_clear(void)
{
    if (!(s_tAtom.bInISR)) {
        s_tAtom.bFlag = false;
    } else {
        s_tAtom.bFlagISR = false;
    }
}

void atom_flag_resume(bool bFlag)
{
    if (!(s_tAtom.bInISR)) {
        s_tAtom.bFlag = bFlag;
    } else {
        s_tAtom.bFlagISR = bFlag;
    }
}

void atom_enter_isr(void)
{
    s_tAtom.bInISR   = true;
    s_tAtom.bFlagISR = false;
}

void atom_exit_isr(void)
{
    s_tAtom.bInISR   = false;
}




/* EOF */
