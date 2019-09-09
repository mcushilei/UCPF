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

#ifndef __REG_IOCTRL_H__
#define __REG_IOCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PINCON_REG              (*(pincon_reg_t    *) PINCON_BASE   )

//! \name pin mode  macros
//! @{
typedef enum {
    IO_PULL_UP      = 0x00,
    IO_PULL_REPEATER,
    IO_PULL_NONE,
    IO_PULL_DOWN,
} io_mode_sel_t;

#define IO_MOD_OPEN_DRAIN   (0x04)
//! @}

//! \brief IO functions
//! @{
typedef enum io_func_sel_t {
    IO_WORKS_AS_FUNC0   = 0x00,
    IO_WORKS_AS_FUNC1   = 0x01,
    IO_WORKS_AS_FUNC2   = 0x02,
    IO_WORKS_AS_FUNC3   = 0x03,
} em_io_func_sel_t;
//! @}

//! \name PIN name
//! @{
typedef enum {
    PA0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PA8,
    PA9,
    PA10,
    PA11,
    PA12,
    PA13,
    PA14,
    PA15,
    PA16,
    PA17,
    PA18,
    PA19,
    PA20,
    PA21,
    PA22,
    PA23,
    PA24,
    PA25,
    PA26,
    PA27,
    PA28,
    PA29,
    PA30,
    PA31,
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PB5,
    PB6,
    PB7,
    PB8,
    PB9,
    PB10,
    PB11,
    PB12,
    PB13,
    PB14,
    PB15,
    PB16,
    PB17,
    PB18,
    PB19,
    PB20,
    PB21,
    PB22,
    PB23,
    PB24,
    PB25,
    PB26,
    PB27,
    PB28,
    PB29,
    PB30,
    PB31,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PC8,
    PC9,
    PC10,
    PC11,
    PC12,
    PC13,
    PC14,
    PC15,
    PC16,
    PC17,
    PC18,
    PC19,
    PC20,
    PC21,
    PC22,
    PC23,
    PC24,
    PC25,
    PC26,
    PC27,
    PC28,
    PC29,
    PC30,
    PC31,
    PD0,
    PD1,
    PD2,
    PD3,
    PD4,
    PD5,
    PD6,
    PD7,
    PD8,
    PD9,
    PD10,
    PD11,
    PD12,
    PD13,
    PD14,
    PD15,
    PD16,
    PD17,
    PD18,
    PD19,
    PD20,
    PD21,
    PD22,
    PD23,
    PD24,
    PD25,
    PD26,
    PD27,
    PD28,
    PD29,
    PD30,
    PD31,
    PE0,
    PE1,
    PE2,
    PE3,
    PE4,
    PE5,
    PE6,
    PE7,
    PE8,
    PE9,
    PE10,
    PE11,
    PE12,
    PE13,
    PE14,
    PE15,
    PE16,
    PE17,
    PE18,
    PE19,
    PE20,
    PE21,
    PE22,
    PE23,
    PE24,
    PE25,
    PE26,
    PE27,
    PE28,
    PE29,
    PE30,
    PE31,
} em_io_pin_t;
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
typedef struct {
    reg32_t PINSEL[11];
    REG32_RSVD(5)
    reg32_t PINMODE[10];
    reg32_t PINMODE_OD[5];
    reg32_t I2CPADCFG;
} pincon_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */