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


#ifndef __ATOM_C__
#ifndef __ATOM_H__
#define __ATOM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \brief Enter the safe atom operations
#define ENTER_SAFE_ATOM_CODE()  bool __bState = atom_flag_get();        \
                                CPU_GLOBAL_INTERRUPT_DISABLE();         \
                                atom_flag_clear()

//! \brief Exit from the safe atom operations
#define EXIT_SAFE_ATOM_CODE()   atom_flag_resume(__bState);             \
                                do {                                    \
                                    if (__bState) {                     \
                                        CPU_GLOBAL_INTERRUPT_ENABLE();  \
                                    } else {                            \
                                        CPU_GLOBAL_INTERRUPT_DISABLE(); \
                                    }                                   \
                                } while (false)

//! \brief The safe ATOM code section macro
#define SAFE_ATOM_CODE(...) {   \
        ENTER_SAFE_ATOM_CODE(); \
        __VA_ARGS__             \
        EXIT_SAFE_ATOM_CODE();  \
    }

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void atom_init(void);
extern bool atom_flag_get(void);
extern void atom_flag_set(void);
extern void atom_flag_clear(void);
extern void atom_flag_resume(bool bFlag);
extern void atom_enter_isr(void);
extern void atom_exit_isr(void);


#endif  //! #ifndef __ATOM_H__
#endif  //! #ifndef __ATOM_C__
/* EOF */
