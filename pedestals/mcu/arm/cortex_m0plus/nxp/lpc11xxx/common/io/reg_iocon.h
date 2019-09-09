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

#ifndef __REG_IOCON_H__
#define __REG_IOCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IOCON_REG               (*(iocon_reg_t *)IOCON_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name function selection macros
//! @{
#define IOCTRL_PIN_FUNSEL_BIAS          0
#define IOCTRL_PIN_FUNSEL_MASK          MASK(2, 0)
//! @}

//! \name pull-up selection macros
//! @{
#define IOCTRL_PIN_MODE_BIAS            3
#define IOCTRL_PIN_MODE_MASK            MASK(4, 3)
//! @}

//! \name invert input enable macros
//! @{
#define IOCTRL_PIN_INV_BIAS             6
#define IOCTRL_PIN_INV_MASK             BIT(6)
//! @}

//! \name input analog mode enable macros
//! @{
#define IOCTRL_PIN_ADMOD_BIAS           7
#define IOCTRL_PIN_ADMOD_MASK           BIT(7)
//! @}

//! \name input glitch filter macros
//! @{
#define IOCTRL_PIN_FILTR_BIAS           8
#define IOCTRL_PIN_FILTR_MASK           BIT(8)
//! @}

//! \name open-drain enable macros
//! @{
#define IOCTRL_PIN_ODE_BIAS             10
#define IOCTRL_PIN_ODE_MASK             BIT(10)
//! @}

//! \name input filter mode selection macros
//! @{
#define IOCTRL_PIN_SMPMOD_BIAS          11
#define IOCTRL_PIN_SMPMOD_MASK          MASK(12, 11)
//! @}

//! \name input filter clock source selection macros
//! @{
#define IOCTRL_PIN_CLKDIV_BIAS          13
#define IOCTRL_PIN_CLKDIV_MASK          MASK(15, 13)
//! @}

/*============================ TYPES =========================================*/
//! \brief IO functions
//! @{
typedef enum {
    IO_WORKS_AS_FUNC0   = 0x00,
    IO_WORKS_AS_FUNC1   = 0x01,
    IO_WORKS_AS_FUNC2   = 0x02,
    IO_WORKS_AS_FUNC3   = 0x03,
    IO_WORKS_AS_FUNC4   = 0x04,
    IO_WORKS_AS_FUNC5   = 0x05,
    IO_WORKS_AS_FUNC6   = 0x06,
    IO_WORKS_AS_FUNC7   = 0x07,
} em_io_func_sel_t;
//! @}

//! \name IO model
//! @{
typedef enum {
    IO_INACTIVE         = (0u << 3),
    IO_PULL_DOWN        = (1u << 3),
    IO_PULL_UP          = (2u << 3),
    IO_REPEAT           = (3u << 3),
    
    IO_HYSTERESIS       = (1u << 5),
    IO_INPUT_INVERT     = (1u << 6),
    IO_ANALOG_MODE      = (0u << 7),
    IO_GLITCH_FILTER    = (1u << 8),
    IO_OPEN_DRAIN       = (1u << 10),
    
    IO_FAST_I2C         = (0u << 8),
    IO_STANDARD_GPIO    = (1u << 8),
    IO_FAST_PLUS_I2C    = (2u << 8),
} em_io_mode_t;

typedef enum {
    IO_FILTER_BYPASS    = (0 << 11),           //!< filter is bypassed
    IO_FILTER_1CLK      = (1 << 11),           //!< levels should keep 1 clks
    IO_FILTER_2CLK      = (2 << 11),           //!< levels should keep 2 clks
    IO_FILTER_3CLK      = (3 << 11),           //!< levels should keep 3 clks
    
    IO_FILTER_CLK_DIV0  = (0 << 13),           //!< select clock divider 0 for filter
    IO_FILTER_CLK_DIV1  = (1 << 13),           //!< select clock divider 1 for filter
    IO_FILTER_CLK_DIV2  = (2 << 13),           //!< select clock divider 2 for filter
    IO_FILTER_CLK_DIV3  = (3 << 13),           //!< select clock divider 3 for filter
    IO_FILTER_CLK_DIV4  = (4 << 13),           //!< select clock divider 4 for filter
    IO_FILTER_CLK_DIV5  = (5 << 13),           //!< select clock divider 5 for filter
    IO_FILTER_CLK_DIV6  = (6 << 13),           //!< select clock divider 6 for filter
} em_io_filter_t;
//! @}

//! \brief IO Configuration Unit register block structure
//! @{
typedef volatile struct {
	reg32_t  PIO0[24];
	reg32_t  PIO1[32];
	REG32_RSVD(4)
	reg32_t  PIO2A[2];  /* PIO2_0/1 only */
	REG32_RSVD(1)
	reg32_t  PIO2B[22];	/* PIO2_2 to PIO_2_23 */
} iocon_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */