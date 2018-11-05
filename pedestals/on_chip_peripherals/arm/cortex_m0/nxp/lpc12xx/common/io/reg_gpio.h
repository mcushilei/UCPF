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

#ifndef __I_IO_GPIO_H__
#define __I_IO_GPIO_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define GPIO_PORT0_REG                  (*(volatile gpio_reg_t *)GPIO0_BASE_ADDRESS)
#define GPIO0_REG                       (*(volatile gpio_reg_t *)GPIO0_BASE_ADDRESS)
#define GPIO_PORT1_REG                  (*(volatile gpio_reg_t *)GPIO1_BASE_ADDRESS)
#define GPIO1_REG                       (*(volatile gpio_reg_t *)GPIO1_BASE_ADDRESS)
#define GPIO_PORT2_REG                  (*(volatile gpio_reg_t *)GPIO2_BASE_ADDRESS)
#define GPIO2_REG                       (*(volatile gpio_reg_t *)GPIO2_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name cross switch controller register page
//! @{
typedef struct {
    reg32_t     MSK;                //!< Mask Register
    reg32_t     IN;                 //!< Input Value Register
    reg32_t     OUT;                //!< Data Output Value
    reg32_t     OUTSET;             //!< Data Output Value Set
    reg32_t     OUTCLR;             //!< Data Output Value Clear
    reg32_t     OUTTOG;             //!< Data Output Value Toggle
    REG32_RSVD(2)                   //!< Reserved
    reg32_t     DIR;                //!< Data Direction
    reg32_t     IS;                 //!< interrupt sense
    reg32_t     IEDG;               //!< interrupt edge sense
    reg32_t     IEV;                //!< interrupt event
    reg32_t     IEN;                //!< interrupt enable
    reg32_t     RIS;                //!< interrupt raw status
    reg32_t     EIS;                //!< enabled interrupt status
    reg32_t     ICLR;               //!< interrupt clear
    REG32_RSVD(1)                   //!< Reserved
} gpio_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */