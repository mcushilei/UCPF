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
#elif   defined(__PRJ_TEMPLATE_WIN__)
#   include "./frameworks/template_win/ports/environment_cfg.h"
#elif   defined(__PRJ_KANGJI_GATEWAY_WIN__)
#   include "./frameworks/kangji_gateway_win/ports/environment_cfg.h"
#elif   defined(__PRJ_KANGJI_GATEWAY__)
#   include "./frameworks/kangji_gateway/ports/environment_cfg.h"
#elif   defined(__PRJ_WINDOWS__)
#   include ".\frameworks\windows\config\environment_cfg.h"
#elif   defined(__PRJ_MSP432_TEMPLATE__)
#   include ".\frameworks\ti_msp432_template\config\environment_cfg.h"
#elif   defined(__PRJ_XOS_DEVELOP__)
#   include ".\frameworks\xos_develop\config\environment_cfg.h"
#elif   defined(__PRJ_JIEDIXIANGKONGZHIQI__)
#   include ".\frameworks\jiedixiangkongzhiqi\ports\environment_cfg.h"
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
