/*******************************************************************************
 *  Copyright(C)2016-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __FSM_H__
#define __FSM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fsm_core.h"
#include ".\fsm_flag.h"
#include ".\fsm_mutex.h"
#include ".\fsm_sem.h"

/*============================ MACROS ========================================*/
#define FSM_TASK            fsm_tcb_t *
#define FSM_TASK_STACK      task_stack_t
#define FSM_OBJ             fsm_handle_t

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \brief start define a FSM state
#define DEF_STATE(__NAME)       void __NAME(void *Arg)
#define REF_STATE(__NAME)       __NAME
#define PROTOTYPE_STATE(__NAME) void __NAME(void *Arg)

//! \brief state transfering
#define FSM_TRANSFER_TO(__ROUTINE, __ARG_ADDR)                                  \
            fsm_state_transfer(__ROUTINE, __ARG_ADDR)

//! \brief state complete
#define FSM_COMPLETE()                                                          \
            fsm_state_transfer(NULL, NULL)

//! \brief call sub sfm and return to current state when sub sfm run complete
#define FSM_CALL(__ROUTINE, __ARG_ADDR)                                         \
            fsm_call_sub(__ROUTINE, __ARG_ADDR)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void         fsm_init            (void);

#endif  //! #ifndef __FSM_H__
/* EOF */
