/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __REG_GPIO_H__
#define __REG_GPIO_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#ifdef IOCTRLA_BASE_ADDRESS
#define IOCTRL_PORTA_REG                     (*(ioctrl_reg_t *)IOCTRLA_BASE_ADDRESS)
#endif

#ifdef IOCTRLB_BASE_ADDRESS
#define IOCTRL_PORTB_REG                     (*(ioctrl_reg_t *)IOCTRLB_BASE_ADDRESS)
#endif

#ifdef IOCTRLC_BASE_ADDRESS
#define IOCTRL_PORTC_REG                     (*(ioctrl_reg_t *)IOCTRLC_BASE_ADDRESS)
#endif

#ifdef IOCTRLD_BASE_ADDRESS
#define IOCTRL_PORTD_REG                     (*(ioctrl_reg_t *)IOCTRLD_BASE_ADDRESS)
#endif

#ifdef IOCTRLE_BASE_ADDRESS
#define IOCTRL_PORTE_REG                     (*(ioctrl_reg_t *)IOCTRLE_BASE_ADDRESS)
#endif

#ifdef IOCTRLF_BASE_ADDRESS
#define IOCTRL_PORTF_REG                     (*(ioctrl_reg_t *)IOCTRLF_BASE_ADDRESS)
#endif

#ifdef IOCTRLG_BASE_ADDRESS
#define IOCTRL_PORTG_REG                     (*(ioctrl_reg_t *)IOCTRLG_BASE_ADDRESS)
#endif



#ifdef GPIOA_BASE_ADDRESS
#define GPIO_PORTA_REG                       (*(gpio_reg_t *)GPIOA_BASE_ADDRESS)
#endif

#ifdef GPIOB_BASE_ADDRESS
#define GPIO_PORTB_REG                       (*(gpio_reg_t *)GPIOB_BASE_ADDRESS)
#endif

#ifdef GPIOC_BASE_ADDRESS
#define GPIO_PORTC_REG                       (*(gpio_reg_t *)GPIOC_BASE_ADDRESS)
#endif

#ifdef GPIOD_BASE_ADDRESS
#define GPIO_PORTD_REG                       (*(gpio_reg_t *)GPIOD_BASE_ADDRESS)
#endif

#ifdef GPIOE_BASE_ADDRESS
#define GPIO_PORTE_REG                       (*(gpio_reg_t *)GPIOE_BASE_ADDRESS)
#endif

#ifdef GPIOF_BASE_ADDRESS
#define GPIO_PORTF_REG                       (*(gpio_reg_t *)GPIOF_BASE_ADDRESS)
#endif

#ifdef GPIOG_BASE_ADDRESS
#define GPIO_PORTG_REG                       (*(gpio_reg_t *)GPIOG_BASE_ADDRESS)
#endif

#define IOCTRL_MUX_MSK              (0x1u)
#define IOCTRL_DIR_MSK              (0x1u)
#define IOCTRL_QUAL_MSK             (0xFFu)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef volatile struct {
    reg16_t     MUX;
    reg16_t     DIR;

    DEF_REG16
        reg16_t QUALPRD:8;     // 0:7    Qualification Sampling Period
        reg16_t        :8;
    END_DEF_REG16(QUAL)

    REG16_RSVD(1)
} ioctrl_reg_t;

typedef volatile struct {
    reg16_t     MUX;
    reg16_t     DIR;

    DEF_REG16
        reg16_t QUALPRD:8;     // 0:7    Qualification Sampling Period
        reg16_t        :8;
    END_DEF_REG16(QUAL)

    REG16_RSVD(1)
} ioctrl_reg_array_t[IO_PORT_NUM];

typedef volatile struct {
    reg16_t     DAT;
    reg16_t     SET;
    reg16_t     CLEAR;
    reg16_t     TOGGLE;
} gpio_reg_t;

typedef volatile struct {
    reg16_t     DAT;
    reg16_t     SET;
    reg16_t     CLEAR;
    reg16_t     TOGGLE;
} gpio_reg_array_t[IO_PORT_NUM];

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
