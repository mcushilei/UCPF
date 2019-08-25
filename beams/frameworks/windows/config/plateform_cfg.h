/***************************************************************************
 *   Copyright(C)2009-2013 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
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

#ifndef __TEMPLATE_PLATEFORM_CFG_H__
#define __TEMPLATE_PLATEFORM_CFG_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/


#ifdef __DEBUG__
#   define DEBUG_MSG_ENABLE             ENABLED
#   define DEBUG_ASSERT_ENABLE          ENABLED
#   define DEBUG_DISALLOW_FILE_INFO     DISABLED
#else
#   define DEBUG_MSG_ENABLE             ENABLED
#   define DEBUG_ASSERT_ENABLE          ENABLED
#endif
#define DEBUG_FOMART_STRING         DISABLED
#define DEBUG_MIN_LEVEL             DEBUG_LEVEL_ALL // DEBUG_LEVEL_WARNING, DEBUG_LEVEL_SERIOUS, 
#define DEBUG_TYPES_ON              DEBUG_ANY       // DEBUG_STATE | DEBUG_TRACE | DEBUG_HALT |  | DEBUG_FRESH

#define TEMPLATE_DEBUG              DEBUG_ON
#define FIFO_DEBUG                  DEBUG_OFF
#define QUEUE_DEBUG                 DEBUG_OFF
#define POOL_DEBUG                  DEBUG_OFF
#define FRAME_DEBUG                 DEBUG_OFF


#define FIFO_UINT_TYPE              uint16_t

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
