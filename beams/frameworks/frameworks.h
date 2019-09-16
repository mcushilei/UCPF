/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __BEAMS_FRAMEWORKS_H__
#define __BEAMS_FRAMEWORKS_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if     defined(__PRJ_TEMPLATE__)
#   include ".\template\framework.h"
#elif   defined(__PRJ_TEMPLATE_WIN__)
#   include "./template_win/framework.h"
#elif   defined(__PRJ_KANGJI_GATEWAY_WIN__)
#   include "./kangji_gateway_win/framework.h"
#elif   defined(__PRJ_KANGJI_GATEWAY__)
#   include "./kangji_gateway/framework.h"
#elif   defined(__PRJ_WINDOWS__)
#   include ".\windows\framework.h"
#elif   defined(__PRJ_XOS_DEVELOP__)
#   include ".\xos_develop\framework.h"
#elif   defined(__PRJ_JIEDIXIANGKONGZHIQI__)
#   include ".\jiedixiangkongzhiqi\framework.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/



#endif  //!< __BEAMS_FRAMEWORKS_H__
/* EOF */
