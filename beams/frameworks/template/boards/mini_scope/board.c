/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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


/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./board.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
ROOT int BEFORE_HW_INIT(void)
{
    /* Return 1 to indicate that normal segment */
    /* initialization should be performed. If   */
    /* normal segment initialization should not */
    /* be performed, return 0.                  */

    return 1;
}

/*! \brief  hardware initialization
 *! \param  none
 *! \retval true run the default initialization
 *! \retval false ignore the default initialization
 */
WEAK bool ON_HW_INIT(void)
{
    return true;
}

static void board_error(void)
{
    while (1) {
        BREATH_LED_ON();
    }
}

bool board_init(void)
{
    /* init the power, clock etc here. */
    
    GPIO0_REG.DIR |= LED_PIN_MSK;
    GPIO0_REG.DIR |= OLED_SPI_MOSI_PIN_MSK;
    GPIO0_REG.DIR |= OLED_SPI_CLK_PIN_MSK;
    GPIO0_REG.DIR |= OLED_RESET_PIN_MSK;
    GPIO0_REG.DIR |= OLED_CD_PIN_MSK;
    
    if (PMU_REG.PCON) {
        //! system boot becase wakeup from deep power down
        PMU_REG.PCON = 1;
    }
    
    IO_CFG(
        //! Debug uart pin
        {PORT0, PIN0,  PIN_FN_U0_RXD, PIN_MODE_PULL_UP},
        {PORT0, PIN4,  PIN_FN_U0_TXD, PIN_MODE_PULL_UP},
        
    );
    
    pll_init(SYSPLL_CKLSRC_IRC, 12MHz);//SYSPLL_CLKSRC_SYSOSC 
    core_clock_config(MAIN_CLKSRC_PLLIN, 1);
    uart_clock_config(1, 21);
    wdtosc_enable();
    
    //! debug uart
    uart_cfg_t tCFG = {
        9600, 
        UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH
    };
    if (!UART0.Init(&tCFG)) {
        board_error();
    }

    
    return ON_HW_INIT();
}

/* EOF */
