/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#ifndef __DRIVER_LPC17XX_EMAC_C__
#ifndef __DRIVER_LPC17XX_EMAC_H__
#define __DRIVER_LPC17XX_EMAC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_emac.h"

/*============================ MACROS ========================================*/

enum {
    FULL_DUPLES_100M = 1,
    HALF_DUPLES_100M,
    FULL_DUPLES_10M,
    HALF_DUPLES_10M,
};

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern uint8_t MAC_Init(void);
extern void  MAC_SetMacAddr(uint8_t *mac_ptr);
extern void  MAC_TxDisable(void);
extern void  MAC_TxEnable(void);
extern void  MAC_RxDisable(void);
extern void  MAC_RxEnable(void);
extern uint8_t MAC_GetUsableDescLen(void);
extern uint8_t MAC_RecFrameProcess(struct pbuf **rValue);
extern void  MAC_RegTxDoneCallback(void (*CallbackFun)(void));
extern void  MAC_RegRxDoneCallback(void (*CallbackFun)(void));

#endif
#endif
