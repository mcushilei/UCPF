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
#define GPIO0_REG               (*(gpio_reg_t      *) GPIO0_BASE    )
#define GPIO1_REG               (*(gpio_reg_t      *) GPIO1_BASE    )
#define GPIO2_REG               (*(gpio_reg_t      *) GPIO2_BASE    )
#define GPIO3_REG               (*(gpio_reg_t      *) GPIO3_BASE    )
#define GPIO4_REG               (*(gpio_reg_t      *) GPIO4_BASE    )
#define GPIOINT_REG             (*(gpioint_reg_t   *) GPIOINT_BASE  )

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @brief General Purpose Input/Output (GPIO) register structure definition */
typedef struct {
    union {
        reg32_t FIODIR;
        struct {
            reg16_t FIODIRL;
            reg16_t FIODIRH;
        };
        struct {
            reg8_t  FIODIR0;
            reg8_t  FIODIR1;
            reg8_t  FIODIR2;
            reg8_t  FIODIR3;
        };
    };
    REG32_RSVD(3)
    union {
        reg32_t FIOMASK;
        struct {
            reg16_t FIOMASKL;
            reg16_t FIOMASKH;
        };
        struct {
            reg8_t  FIOMASK0;
            reg8_t  FIOMASK1;
            reg8_t  FIOMASK2;
            reg8_t  FIOMASK3;
        };
    };
    union {
        reg32_t FIOPIN;
        struct {
            reg16_t FIOPINL;
            reg16_t FIOPINH;
        };
        struct {
            reg8_t  FIOPIN0;
            reg8_t  FIOPIN1;
            reg8_t  FIOPIN2;
            reg8_t  FIOPIN3;
        };
    };
    union {
        reg32_t FIOSET;
        struct {
            reg16_t FIOSETL;
            reg16_t FIOSETH;
        };
        struct {
            reg8_t  FIOSET0;
            reg8_t  FIOSET1;
            reg8_t  FIOSET2;
            reg8_t  FIOSET3;
        };
    };
    union {
        reg32_t FIOCLR;
        struct {
            reg16_t FIOCLRL;
            reg16_t FIOCLRH;
        };
        struct {
            reg8_t  FIOCLR0;
            reg8_t  FIOCLR1;
            reg8_t  FIOCLR2;
            reg8_t  FIOCLR3;
        };
    };
} gpio_reg_t;

/** @brief General Purpose Input/Output interrupt (GPIOINT) register structure definition */
typedef struct {
    reg32_t IntStatus;
    reg32_t IO0IntStatR;
    reg32_t IO0IntStatF;
    reg32_t IO0IntClr;
    reg32_t IO0IntEnR;
    reg32_t IO0IntEnF;
    REG32_RSVD(3)
    reg32_t IO2IntStatR;
    reg32_t IO2IntStatF;
    reg32_t IO2IntClr;
    reg32_t IO2IntEnR;
    reg32_t IO2IntEnF;
} gpioint_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */