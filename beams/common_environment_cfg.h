/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
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

#ifndef __COMMON_ENVIRONMENT_CFG_H__
#define __COMMON_ENVIRONMENT_CFG_H__

 /*============================ INCLUDES ======================================*/
#if     defined(__PRJ_TEMPLATE__)
#   include ".\frameworks\template\config\environment_cfg.h"
#elif   defined(__PRJ_WINDOWS__)
#   include ".\frameworks\windows\config\environment_cfg.h"
#elif   defined(__PRJ_SENSOR_VIBRATION__)
#   include ".\frameworks\sensor_vibration\config\environment_cfg.h"
#elif   defined(__PRJ_BATTERY_MONITOR__)
#   include ".\frameworks\battery_monitor\config\environment_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL__)
#   include ".\frameworks\beidou_ship_terminal\config\environment_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL_TEST__)
#   include ".\frameworks\beidou_ship_terminal_test\config\environment_cfg.h"
#elif   defined(__PRJ_LINE_VIBRATION__)
#   include ".\frameworks\line_vibration\config\environment_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL_USER_SIMULATOR__)
#   include ".\frameworks\beidou_ship_terminal_user_simulator\config\environment_cfg.h"
#elif   defined(__PRJ_SHIP_GPS__)
#   include ".\frameworks\ship_gps\config\environment_cfg.h"
#elif   defined(__PRJ_BEIDOU_HARBOUR__)
#   include ".\frameworks\ship_gps_g2\config\environment_cfg.h"
#elif   defined(__PRJ_LED_PANNEL__)
#   include ".\frameworks\led_panel\config\environment_cfg.h"
#elif   defined(__PRJ_BEIJING_NENGYUAN__)
#   include ".\frameworks\beijing_alternative_energy\config\environment_cfg.h"
#else
#   error "Unknow Project"
#endif
 
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

#endif
