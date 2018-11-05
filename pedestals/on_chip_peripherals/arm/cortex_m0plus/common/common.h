/***************************************************************************
 *   Copyright(C)2009-2013 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __DRIVER_ARM_M0P_COMMON_H__
#define __DRIVER_ARM_M0P_COMMON_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\core\core_cm0plus.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define ENABLE_GLOBAL_INTERRUPT()   __set_PRIMASK(0)
#define DISABLE_GLOBAL_INTERRUPT()  __set_PRIMASK(1)

//! \brief Enter the safe atom operations
#define ENTER_SAFE_ATOM_CODE()  volatile uint32_t __wState = __get_PRIMASK();    \
                                __set_PRIMASK(1)

//! \brief Exit from the safe atom operations
#define EXIT_SAFE_ATOM_CODE()   __set_PRIMASK(__wState)

//! \brief The safe ATOM code section macro
#define SAFE_ATOM_CODE(...) {   \
        ENTER_SAFE_ATOM_CODE(); \
        __VA_ARGS__             \
        EXIT_SAFE_ATOM_CODE();  \
    }

#define CPU_RESET()             do {\
    SCB->AIRCR = (0x05FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;\
} while (0)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */