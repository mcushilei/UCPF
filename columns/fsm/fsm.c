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
#define __FSM_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fsm_core.h"
#include ".\fsm_flag.h"
#include ".\fsm_mutex.h"
#include ".\fsm_sem.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

/*! \brief fsm init.
 */
void fsm_init(void)
{
    fsmIntNesting = 0;
    fsmScheduler.CurrentTask    = NULL;
    fsmScheduler.Running        = false;
    list_init(&fsmScheduler.PendList);
    list_init(&fsmScheduler.ReadyList);
    fsm_tcb_pool_init();
#if SAFE_TASK_THREAD_SYNC == ENABLED
    fsm_flag_init();
    fsm_mutex_init();
    fsm_semaphore_init();
#endif
}

/* EOF */
