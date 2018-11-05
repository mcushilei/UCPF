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

#ifndef __REG_GPIO_H__
#define __REG_GPIO_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define GPIO0_REG           (*(gpio_reg_page_t *)(GPIO_BASE_ADDRESS + 0x2000u))
#define GPIO1_REG           (*(gpio_reg_page_t *)(GPIO_BASE_ADDRESS + 0x2004u))
#define GPIO2_REG           (*(gpio_reg_page_t *)(GPIO_BASE_ADDRESS + 0x2008u))

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef volatile struct {
    reg32_t     DIR;         //!< Data Direction
    REG32_RSVD(31)
    reg32_t     MASK;        //!< port pin mask
    REG32_RSVD(31)
    reg32_t     PIN;         //!< port pin value
    REG32_RSVD(31)
    reg32_t     MPIN;        //!< masked port pin value
    REG32_RSVD(31)
    reg32_t     SET;         //!< set output
    REG32_RSVD(31)
    reg32_t     CLR;         //!< clear output
    REG32_RSVD(31)
    reg32_t     NOT;         //!< toggle output
} gpio_reg_page_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */