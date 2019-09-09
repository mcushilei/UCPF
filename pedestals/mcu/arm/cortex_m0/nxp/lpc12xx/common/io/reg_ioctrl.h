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

#ifndef __I_IO_IOCTRL_H__
#define __I_IO_IOCTRL_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IOCTRL_REG              (*(volatile ioctrl_reg_t *)IOCTRL_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name function selection macros
//! @{
#define IOCTRL_PIN_FUNSEL_BIT0          0
#define IOCTRL_PIN_FUNSEL_BIT1          1
#define IOCTRL_PIN_FUNSEL_BIT2          2
#define IOCTRL_PIN_FUNSEL_MSK           ((1ul << IOCTRL_PIN_FUNSEL_BIT0)        \
                                        |(1ul << IOCTRL_PIN_FUNSEL_BIT1)        \
                                        |(1ul << IOCTRL_PIN_FUNSEL_BIT2))
#define IOCTRL_PIN_FUNSEL(__FUN)        (((__FUN) << IOCTRL_PIN_FUNSEL_BIT0)    \
                                        & IOCTRL_PIN_FUNSEL_MSK)
//! @}

//! \name input invert enable macros
//! @{
#define IOCTRL_PIN_ADMOD_BIT0           7
#define IOCTRL_PIN_ADMOD_MSK            (1ul << IOCTRL_PIN_ADMOD_BIT0)
//! @}

//! \name pull-up selection macros
//! @{
#define IOCTRL_PIN_PUE_BIT0             4
#define IOCTRL_PIN_PUE_MSK              (1ul << IOCTRL_PIN_PUE_BIT0)
//! @}

//! \name open-drain enable macros
//! @{
#define IOCTRL_PIN_ODE_BIT0             10
#define IOCTRL_PIN_ODE_MSK              (1ul << IOCTRL_PIN_ODE_BIT0)
//! @}

//! \name input filter mode selection macros
//! @{
#define IOCTRL_PIN_SMPMOD_BIT0          11
#define IOCTRL_PIN_SMPMOD_BIT1          12
#define IOCTRL_PIN_SMPMOD_MSK           ((1ul << IOCTRL_PIN_DFSMPMOD_BIT0)      \
                                        |(1ul << IOCTRL_PIN_DFSMPMOD_BIT1))
#define IOCTRL_PIN_SMPMOD_SET(__MODE)   (((__MODE) << IOCTRL_PIN_SMPMOD_BIT0)   \
                                        & IOCTRL_PIN_SMPMOD_MSK)
//! @}

//! \name input filter clock source selection macros
//! @{
#define IOCTRL_PIN_CLKDIV_BIT0          13
#define IOCTRL_PIN_CLKDIV_BIT1          14
#define IOCTRL_PIN_CLKDIV_BIT2          15
#define IOCTRL_PIN_CLKDIV_MSK           ((1ul << IOCTRL_PIN_CLKDIV_BIT0)        \
                                        |(1ul << IOCTRL_PIN_CLKDIV_BIT1)        \
                                        |(1ul << IOCTRL_PIN_CLKDIV_BIT2))
#define IOCTRL_PIN_CLKDIV(__DIV)        (((__DIV) << IOCTRL_PIN_CLKDIV_BIT0)    \
                                        & IOCTRL_PIN_CLKDIV_MSK)
//! @}

/*============================ TYPES =========================================*/
//! \brief IO functions
//! @{
typedef enum {
    IO_WORKS_AS_FUNC0       = 0x00,
    IO_WORKS_AS_FUNC1       = 0x01,
    IO_WORKS_AS_FUNC2       = 0x02,
    IO_WORKS_AS_FUNC3       = 0x03,
    IO_WORKS_AS_FUNC4       = 0x04,
    IO_WORKS_AS_FUNC5       = 0x05,
    IO_WORKS_AS_FUNC6       = 0x06,
    IO_WORKS_AS_FUNC7       = 0x07,

    IO_WORKS_AS_GPIO        = 0x00,

} em_io_func_sel_t;
//! @}

//! \name IO model
//! @{
typedef enum {
    IO_PULL_UP              = (1 << 4),         //!< enable pull-up resistor
    IO_OPEN_DRAIN           = (1 << 10),        //!< enable open-drain mode
    IO_INVERT_INPUT         = (1 << 6),         //!< invert the input pin level 
    IO_ANALOG_MODE          = (1 << 7),         //!< disable input
    IO_HIGH_CURRENT_DRIVE   = (1 << 9),         //!< enable high drive strength

    IO_FILTER_BYPASS        = (0 << 11),           //!< filter is bypassed
    IO_FILTER_1CLK          = (1 << 11),           //!< levels should keep 1 clks
    IO_FILTER_2CLK          = (2 << 11),           //!< levels should keep 2 clks
    IO_FILTER_3CLK          = (3 << 11),           //!< levels should keep 3 clks
    
    IO_FILTER_CLK_DIV0      = (0 << 13),           //!< select clock divider 0 for filter
    IO_FILTER_CLK_DIV1      = (1 << 13),           //!< select clock divider 1 for filter
    IO_FILTER_CLK_DIV2      = (2 << 13),           //!< select clock divider 2 for filter
    IO_FILTER_CLK_DIV3      = (3 << 13),           //!< select clock divider 3 for filter
    IO_FILTER_CLK_DIV4      = (4 << 13),           //!< select clock divider 4 for filter
    IO_FILTER_CLK_DIV5      = (5 << 13),           //!< select clock divider 5 for filter
    IO_FILTER_CLK_DIV6      = (6 << 13),           //!< select clock divider 6 for filter
} em_io_mode_t;
//! @}

//! \name cross switch controller register page
//! @{
typedef struct {
    DEF_REG32
        reg32_t FUNSEL      : 3;       //!< function selection
        reg32_t             : 1;
        reg32_t PUE         : 1;       //!< Pull-up Enable
        reg32_t             : 1;
        reg32_t INV         : 1;       //!< Input Invert
        reg32_t ADMOD       : 1;       //!< Analog/Digital mode select
        reg32_t             : 1;
        reg32_t DRV         : 1;       //!< Drive Strength
        reg32_t ODE         : 1;       //!< Open Drain Enable
        reg32_t SMPMOD      : 2;       //!< Digital Filter Sample Mode
        reg32_t CLKDIV      : 3;       //!< Digital Filter Clock Source
        reg32_t             : 16;
    END_DEF_REG32(PIN[IO_PIN_COUNT])
} ioctrl_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */