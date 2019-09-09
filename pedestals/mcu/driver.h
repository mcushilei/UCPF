/*******************************************************************************
*  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __PEDESTAS_DRIVER_H__
#define __PEDESTAS_DRIVER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if     defined(__CPU_ARM__)
#   include ".\arm\driver.h"
#elif   defined(__CPU_TMS320__)
#   include ".\tms320\driver.h"
#endif



/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //!< __PEDESTAS_DRIVER_H__
/* EOF */
