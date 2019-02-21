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


#ifndef __EVENT_FSM_H__
#define __EVENT_FSM_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
#ifndef __EFSM_USER_CONFIG__
typedef uint8_t  event_code_t;
#endif

typedef uint8_t efsm_state_t(event_code_t event, void *arg);
typedef efsm_state_t *efsm_stack_t;

typedef struct {
    efsm_stack_t   *Stack;          //!< stack of layer.
    uint8_t         StackSize;
    uint8_t         StackLevel;     //!< stack level.
    uint8_t         CurrentLevel;   //!< current level.
    uint8_t         Status;
} efsm_t;

enum {
    EFSM_STATUS_RUNNING     = 0,
    EFSM_STATUS_TRANSFER_STATE,
    EFSM_STATUS_ADD_LAYER,
};

enum {
    EFSM_EVENT_ENTRY_STATE = 0,
    EFSM_EVENT_EXIT_STATE,
    EFSM_EVENT_USER_DEFINED,
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     efsm_init  (efsm_t         *EFSM,
                            efsm_stack_t   *Stack,
                            uint8_t         StackSize,
                            efsm_state_t   *InitState);
extern uint8_t  efsm_dispatch_event (efsm_t *EFSM, event_code_t event, void *arg);

extern bool efsm_to_state       (efsm_t *EFSM, efsm_state_t *State);
extern bool efsm_to_upper       (efsm_t *EFSM, efsm_state_t *State);
extern bool efsm_to_current     (efsm_t *EFSM);
extern bool efsm_current_layer_to_state(efsm_t *EFSM, efsm_state_t *State);


#endif  //! #ifndef __EVENT_FSM_H__
/* EOF */
