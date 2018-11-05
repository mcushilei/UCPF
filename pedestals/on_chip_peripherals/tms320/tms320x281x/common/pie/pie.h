/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 2 of the License_IRQn, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful_IRQn, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not_IRQn, write to the Free Software Foundation_IRQn,   *
 *  Inc._IRQn, 59 Temple Place - Suite 330_IRQn, Boston_IRQn, MA  02111-1307_IRQn, USA.           *
*******************************************************************************/

#ifndef __PIE_H__
#define __PIE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void(vector_t)(void);


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool pie_init(vector_t **pptInitValue);
extern bool pie_enable(void);
extern bool pie_disable(void);
extern bool pie_interrupt_enable(uint16_t hwIndex);
extern bool pie_interrupt_disable(uint16_t hwIndex);

#endif
/* EOF */
