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

#ifndef __FSM_CORE_H__
#define __FSM_CORE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\list\list.h"

/*============================ MACROS ========================================*/
#define FSM_INFINITE                (~0u)
                
#define FSM_EVENT_SINGNAL_BIT       (1u << 0)
#define FSM_EVENT_MANUAL_RESET_BIT  (1u << 1)

#define FSM_MUTEX_OWNED_BIT         (1u << 0)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
enum {
    FSM_ERR_NONE            = 0,
    FSM_ERR_INVALID_PARAM,
    FSM_ERR_INVALID_OPT,
    FSM_ERR_OBJ_DEPLETED,
    FSM_ERR_OBJ_NOT_SINGLED,
    FSM_ERR_OBJ_NOT_WAITABLE,
    FSM_ERR_OBJ_TYPE,
    FSM_ERR_TASK_NO_MORE_TCB,
    FSM_ERR_TASK_PEND_TIMEOUT,
    FSM_ERR_TASK_STACK_FULL,
    FSM_ERR_PEND_ISR,
    FSM_ERR_CREATE_ISR,
    FSM_ERR_CALL_IN_ISR,
    FSM_ERR_SEM_EXCEED,
    FSM_ERR_NOT_MUTEX_OWNER,
};

enum {
    FSM_OBJ_TYPE_INVALID    = 0,
    FSM_OBJ_TYPE_TASK       = 0x01,
    FSM_OBJ_TYPE_WAITABLE   = 0x80,
    FSM_OBJ_TYPE_FLAG       = 0x80,
    FSM_OBJ_TYPE_MUTEX      = 0x81,
    FSM_OBJ_TYPE_SEM        = 0x82,
};

enum {
    FSM_TASK_STATUS_READY   = 0,
    FSM_TASK_STATUS_PEND,
    FSM_TASK_STATUS_PEND_OK,
    FSM_TASK_STATUS_PEND_TIMEOUT,
};

typedef uint8_t     fsm_err_t;
typedef void       *fsm_handle_t;
typedef void        fsm_state_t(void *);

typedef struct {
    fsm_state_t        *State;         //!< routine
    void               *Arg;           //!< argument
} task_stack_t;

typedef union {
    void               *ObjNext;
    struct {
        list_node_t         ListNode;
        
        uint32_t            Delay;
        
        uint8_t             Flag;
        uint8_t             Status;

        uint8_t             StackSize;
        uint8_t             StackPoint;
        task_stack_t       *Stack;
        
//#if SAFE_TASK_THREAD_SYNC == ENABLED
        fsm_handle_t        Object;
        list_node_t         WaitNode;
//#endif
    };
} fsm_tcb_t;

typedef union {
    void               *ObjNext;
    struct {
        list_node_t     TaskQueue;
    };
} fsm_waitable_obj_t;

typedef struct {
    fsm_tcb_t              *CurrentTask;
    list_node_t             ReadyList;
    list_node_t             PendList;
    bool                    Running;
} scheduler_t;

/*============================ PUBLIC VARIABLES ==============================*/
extern uint8_t              fsmIntNesting;
extern scheduler_t          fsmScheduler;

/*============================ PUBLIC PROTOTYPES =============================*/
/*----------------------------- miscellaneous --------------------------------*/
extern void         fsm_tcb_pool_init   (void);
extern bool         fsm_scheduler       (void);
extern void         fsm_start           (void);

extern void         fsm_tick            (void);
extern fsm_err_t    fsm_task_delay      (uint32_t          wTimeout);

extern void         fsm_int_enter       (void);
extern void         fsm_int_exit        (void);

/*---------------------------- task management -------------------------------*/
extern fsm_err_t    fsm_task_create     (fsm_handle_t      *pptTask,
                                         fsm_state_t       *state,
                                         void              *arg,
                                         task_stack_t      *stack,
                                         uint8_t            stackSize);
extern bool         fsm_state_transfer  (fsm_state_t       *state,
                                         void              *arg);
extern fsm_err_t    fsm_call_sub        (fsm_state_t       *state,
                                         void              *arg);


/*--------------------------- internal use only ------------------------------*/
extern void         fsm_waitable_obj_pnd_task  (fsm_waitable_obj_t *pObj,
                                                uint32_t            timeDelay);
extern void         fsm_waitable_obj_remove_task(fsm_waitable_obj_t *pObj,
                                                fsm_tcb_t          *pTask);
extern fsm_tcb_t   *fsm_waitable_obj_rdy_task  (fsm_waitable_obj_t *pObj,
                                                uint8_t             pendStat);

#endif  //!< #ifndef __FSM_CORE_H__
/* EOF */
