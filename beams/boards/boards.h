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

#ifndef __BEAMS_BOARDS_BOARDS_H__
#define __BEAMS_BOARDS_BOARDS_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

#if     defined(BOARD_APOLLO_EVK)
#   include ".\apollo_evk\board.h"
#elif   defined(BOARD_BEIDOU_GANG_V21)
#   include ".\beidou_gang_v21\board.h"
#elif   defined(__BOARD_MSP432_LAUNCHPAD__)
#   include ".\msp432_alunchpad\board.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //!< __BEAMS_BOARDS_BOARDS_H__
/* EOF */
