/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#ifndef __FSM_MUTEX_PUBLIC_H__
#define __FSM_MUTEX_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fsm_core.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    fsm_waitable_obj_t;
    fsm_tcb_t              *MutexOwner;
} fsm_mutex_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
/*------------------------- mutual exclusion semaphore -----------------------*/
extern void         fsm_mutex_init(void);
extern fsm_err_t    fsm_mutex_create       (fsm_handle_t       *pptMutex);
extern fsm_err_t    fsm_mutex_wait         (fsm_handle_t        hObject,
                                            uint32_t            timeDelay);
extern fsm_err_t    fsm_mutex_release      (fsm_handle_t        hObject);


#endif  //!< #ifndef __FSM_MUTEX_PUBLIC_H__
/* EOF */
