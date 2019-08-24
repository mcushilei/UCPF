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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\fsm_mutex.h"

#if SAFE_TASK_THREAD_SYNC == ENABLED

/*============================ MACROS ========================================*/
#if ((!defined(FSM_MAX_MUTEXES)) || (FSM_MAX_MUTEXES < 1))
#   define FSM_MAX_MUTEXES           (1u)
#   warning "FSM_MAX_MUTEXES is invalid, use default value 1."
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
static fsm_mutex_t   *fsmMutexList;                     //! Head of mutex ocb pool
static fsm_mutex_t    fsmMutexPool[FSM_MAX_MUTEXES];    //! Mutex ocb pool

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
void fsm_mutex_init(void)
{
    uint_fast16_t n;
    fsm_mutex_t **p;

    p = &fsmMutexList;
    
    //! add mutex OCBs to the free list
    for (n = 0; n < ARRAY_LENGTH(fsmMutexPool); n++) {
        *p = &fsmMutexPool[n];
        p = (fsm_mutex_t **)&((*p)->ObjNext);
    }
}

fsm_err_t    fsm_mutex_create  (fsm_handle_t *pptMutex)
{
    fsm_mutex_t *pMutex;
    
    if (NULL == pptMutex) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    if (fsmIntNesting != 0) {
        return FSM_ERR_CREATE_ISR;
    }

    if (NULL == fsmMutexList) {
        *pptMutex = NULL;
        return FSM_ERR_OBJ_DEPLETED;
    }
    pMutex      = fsmMutexList;
    fsmMutexList = (fsm_mutex_t *)pMutex->ObjNext;
    
    list_init(&pMutex->TaskQueue);
    pMutex->MutexOwner = NULL;

    *pptMutex = pMutex;

    return FSM_ERR_NONE;
}

/*! \brief  wait for a specified task event
 *! \param  pObject target event item
 *! \param  Task parasitifer task
 *! \retval true event raised
 *! \retval false event haven't raised yet.
 */
fsm_err_t fsm_mutex_wait(fsm_handle_t hObject, uint32_t timeDelay)
{
    uint8_t         err;
    fsm_tcb_t      *pTask = fsmScheduler.CurrentTask;
    fsm_mutex_t    *pMutex = (fsm_mutex_t *)hObject;

    if (NULL == hObject) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    if (fsmIntNesting != 0) {
        return FSM_ERR_PEND_ISR;
    }
    
    switch (pTask->Status) {
        case FSM_TASK_STATUS_READY:
            err = FSM_ERR_OBJ_NOT_SINGLED;
            if (pMutex->MutexOwner != NULL) {
                if (pMutex->MutexOwner == pTask) {
                    err = FSM_ERR_NONE;
                } else if (timeDelay == 0u) {
                    err = FSM_ERR_TASK_PEND_TIMEOUT;
                } else {
                    //! add task to the object's wait queue.
                    fsm_waitable_obj_pnd_task(hObject, timeDelay);
                    err = FSM_ERR_OBJ_NOT_SINGLED;
                }
            } else {
                pMutex->MutexOwner = pTask;
                err = FSM_ERR_NONE;
            }
            break;
            
        case FSM_TASK_STATUS_PEND_OK:
            pTask->Status    = FSM_TASK_STATUS_READY;
            err         = FSM_ERR_NONE;
            break;
            
        case FSM_TASK_STATUS_PEND_TIMEOUT:
            pTask->Status    = FSM_TASK_STATUS_READY;
            err        = FSM_ERR_TASK_PEND_TIMEOUT;
            break;
            
        case FSM_TASK_STATUS_PEND:
        default:
            err = FSM_ERR_OBJ_NOT_SINGLED;
            break;
    }

    return err;
}

fsm_err_t fsm_mutex_release(fsm_handle_t hObject)
{
    fsm_mutex_t *pMutex = (fsm_mutex_t *)hObject;
    fsm_tcb_t   *pTask = fsmScheduler.CurrentTask;

    if (NULL == pMutex) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    if (fsmIntNesting != 0) {
        return FSM_ERR_CALL_IN_ISR;
    }
    
    if (pTask != pMutex->MutexOwner) {
        return FSM_ERR_NOT_MUTEX_OWNER;
    }
    
    pMutex->MutexOwner = NULL;
    
    //! wake up the first blocked task.
    if (!LIST_IS_EMPTY(pMutex->TaskQueue)) {
        pTask = fsm_waitable_obj_rdy_task(hObject, FSM_TASK_STATUS_PEND_OK);
        pMutex->MutexOwner = pTask;
    }
        
    return FSM_ERR_NONE;
}

#endif
/* EOF */
