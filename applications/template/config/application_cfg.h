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

#ifndef __TEMPLATE_APPLICATION_CFG_H__
#define __TEMPLATE_APPLICATION_CFG_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#if     defined(__LPC17XX__)
#define MAIN_CLK                    (SYS_OSC_FREQ  * 8)     //!< 
#define CORE_CLK                    MAIN_CLK
#define PLL_ENABLED                 ENABLED
#define PLL_IN_CLK                  SYS_OSC_FREQ
#define PLL_OUT_CLK                 (MAIN_CLK * 4)          //!< 275 MHz to 550 MHz
#define PLL_NVALUE                  1                       //!< 1~32
#define PLL_MVALUE                  ((PLL_OUT_CLK * PLL_NVALUE / PLL_IN_CLK) / 2)    //!< 6~512
#define SYSTEM_LOW_LEVEL_INIT       ENABLED


#elif   defined(__LPC12XX__)
#define PLL_IN_CLK                  SYS_OSC_FREQ
#define PLL_OUT_CLK                 (PLL_IN_CLK * 6)
#define PLL_MSEL                    ((PLL_OUT_CLK / PLL_IN_CLK) - 1)    //!< 0~31, M = PLL_MSEL + 1
#define PLL_PSEL                    1                                   //!< 0~3, P = 2^PLL_PSEL
#define PLL_ENABLED                 DISABLE//ENABLED
#define MAIN_CLK                    PLL_OUT_CLK             //!< max 100MHz
#define CORE_CLK                    (MAIN_CLK / 3)          //!< max 45MHz
#endif

#define SYSTEM_LOW_LEVEL_INIT       ENABLED

#define MESSAGE_MAX_LENGTH          (600)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
