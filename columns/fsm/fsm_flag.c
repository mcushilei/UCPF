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
#include ".\fsm_flag.h"

#if SAFE_TASK_THREAD_SYNC == ENABLED

/*============================ MACROS ========================================*/
#if ((!defined(FSM_MAX_FLAGS)) || (FSM_MAX_FLAGS < 1))
#   define FSM_MAX_FLAGS           (1u)
#   warning "FSM_MAX_FLAGS is invalid, use default value 1."
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
static fsm_flag_t   *fsmFlagFreeList;               //! Head of event ocb pool
static fsm_flag_t    fsmFlagPool[FSM_MAX_FLAGS];    //! event ocb pool

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
void fsm_flag_init(void)
{
    uint_fast16_t   n;
    fsm_flag_t    **p;

    p = &fsmFlagFreeList;
    
    //! add event OCBs to the free list
    for (n = 0; n < ARRAY_LENGTH(fsmFlagPool); n++) {
        *p = &fsmFlagPool[n];
        p = (fsm_flag_t **)&((*p)->ObjNext);
    }
}

/*! \brief initialize task event
 *! \param pFlag event object
 *! \param isManualReset flag that indicates whether the event should reset to 
 *!        inactived state automatically.
 *! \param initialState event initial state, either set or not.
 *! \return pointer for event object
 */
fsm_err_t fsm_flag_create  (fsm_handle_t   *pptEvent,
                            bool            isManualReset,
                            bool            initialState)
{
    uint8_t     flag;
    fsm_flag_t *pFlag;
    
    if (NULL == pptEvent) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    if (fsmIntNesting != 0) {
        return FSM_ERR_CREATE_ISR;
    }

    if (NULL == fsmFlagFreeList) {
        *pptEvent = NULL;
        return FSM_ERR_OBJ_DEPLETED;
    }
    pFlag           = fsmFlagFreeList;
    fsmFlagFreeList = (fsm_flag_t *)pFlag->ObjNext;
    
    flag = 0;
    if (isManualReset) {
        flag |= FSM_EVENT_MANUAL_RESET_BIT;
    }
    if (initialState) {
        flag |= FSM_EVENT_SINGNAL_BIT;
    }
    
    list_init(&pFlag->TaskQueue);
    pFlag->EventFlag    = flag;

    *pptEvent = pFlag;

    return FSM_ERR_NONE;
}

/*! \brief  wait for a specified task event
 *! \param  pObject target event item
 *! \param  Task parasitifer task
 *! \retval true event raised
 *! \retval false event haven't raised yet.
 */
fsm_err_t fsm_flag_wait(fsm_handle_t hObject, uint32_t timeDelay)
{
    uint8_t         err;
    fsm_tcb_t      *pTask = fsmScheduler.CurrentTask;
    fsm_flag_t     *pFlag = (fsm_flag_t *)hObject;

    if (NULL == hObject) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    if (fsmIntNesting != 0) {
        return FSM_ERR_PEND_ISR;
    }
    
    switch (pTask->Status) {
        case FSM_TASK_STATUS_READY:
            FSM_SAFE_ATOM_CODE(
                if (pFlag->EventFlag & FSM_EVENT_SINGNAL_BIT) {
                    if (!(pFlag->EventFlag & FSM_EVENT_MANUAL_RESET_BIT)) {
                        pFlag->EventFlag &= ~FSM_EVENT_SINGNAL_BIT;
                    }
                    err = FSM_ERR_NONE;
                } else {
                    if (timeDelay == 0u) {
                        err = FSM_ERR_TASK_PEND_TIMEOUT;
                    } else {
                        //! add task to the object's wait queue.
                        fsm_waitable_obj_pnd_task(hObject, timeDelay);
                        err = FSM_ERR_OBJ_NOT_SINGLED;
                    }
                }
            )
            break;
            
        case FSM_TASK_STATUS_PEND_OK:
            pTask->Status    = FSM_TASK_STATUS_READY;
            err         = FSM_ERR_NONE;
            break;
        
        case FSM_TASK_STATUS_PEND_TIMEOUT:
            pTask->Status    = FSM_TASK_STATUS_READY;
            err         = FSM_ERR_TASK_PEND_TIMEOUT;
            break;
        
        case FSM_TASK_STATUS_PEND:
        default:
            err = FSM_ERR_OBJ_NOT_SINGLED;
            break;
    }

    return err;
}

/*! \brief set task event
 *! \param pFlag pointer for task event
 *! \return none
 */
fsm_err_t fsm_flag_set  (fsm_handle_t hObject) 
{
    fsm_flag_t  *pFlag = (fsm_flag_t *)hObject;
    
    if (NULL == pFlag) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    FSM_SAFE_ATOM_CODE(
        pFlag->EventFlag |= FSM_EVENT_SINGNAL_BIT;
        if (LIST_IS_EMPTY(pFlag->TaskQueue)) {
            if (!(pFlag->EventFlag & FSM_EVENT_MANUAL_RESET_BIT)) {
                pFlag->EventFlag &= ~FSM_EVENT_SINGNAL_BIT;
            }
            //! wake up all blocked tasks.
            while (!LIST_IS_EMPTY(pFlag->TaskQueue)) {
                fsm_waitable_obj_rdy_task(hObject, FSM_TASK_STATUS_PEND_OK);
            }
        }
    )
        
    return FSM_ERR_NONE;
}

/*! \brief reset specified task event
 *! \param pFlag task event pointer
 *! \return none
 */
fsm_err_t fsm_flag_reset(fsm_handle_t hObject)
{
    fsm_flag_t *pFlag = (fsm_flag_t *)hObject;

    if (NULL == pFlag) {
        return FSM_ERR_INVALID_PARAM;
    }
    
    FSM_SAFE_ATOM_CODE(
        pFlag->EventFlag &= ~FSM_EVENT_SINGNAL_BIT;
    )
        
    return FSM_ERR_NONE;
}
#endif

/* EOF */
