/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __MQTT_CLIENT_PLUG_H__
#define __MQTT_CLIENT_PLUG_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "../frameworks/frameworks_port.h"


/*============================ MACROS ========================================*/
#define MQTT_TASK       //!< uncomment this to enable multithread support.


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern void MQTTMutexInit(MQTTMutex* mutex);
extern int  MQTTMutexLock(MQTTMutex* mutex);
extern int  MQTTMutexUnlock(MQTTMutex* mutex);

extern void MQTTDeadlineInit(MQTTDeadline* timer);
extern char MQTTDeadlineIsExpired(MQTTDeadline* timer);
extern void MQTTDeadlineSet(MQTTDeadline* timer, uint32_t timeout);
extern void MQTTDeadlineSetMS(MQTTDeadline* timer, uint32_t timeout);
extern int  MQTTDeadlineLeftMS(MQTTDeadline* timer);

#endif  //!< #ifndef __MQTT_CLIENT_PLUG_H__
/* EOF */
