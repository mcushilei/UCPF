/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/

uint32_t err_log[64];

/*============================ IMPLEMENTATION ================================*/

static uint32_t core_clock_get(void)
{
    return 1000000;
}

/*! \brief  hardware initialization
 *! \param  none
 *! \retval true run the default initialization
 *! \retval false ignore the default initialization
 */
ROOT bool ON_HW_INIT(void)
{
    //WDT.Disable();

    /*! you can put your hardware initialization code here */
    
    SysTick_Config(core_clock_get() / 100);
    
    return true;
}


bool framework_init(void)
{
    //！Put all hardware devices to initial status.
    if (!board_init()) {
        return false;
    }
    
#ifdef __LPC17XX__

    //! Debug uart init
    PM.Clock.Peripheral.Config(PCLK_UART0, PCLK_DIV_1);
    PM.Clock.Peripheral.Get(PCLK_UART0);
    PM.Power.Enable(PCONP_UART0);
    PIN_CFG(
        {PORT0, PIN2,   DRIVER_PIN_FUNC(1), DRIVER_PIN_MODE_PULL_UP},
        {PORT0, PIN3,   DRIVER_PIN_FUNC(1), DRIVER_PIN_MODE_PULL_UP},
    );

    static const uart_cfg_t uartCfg = {
        .Baudrate = 9600,
        .DataBits = DRIVER_UART_8_BITS_LENGTH,
        .StopBits = DRIVER_UART_1_STOPBITS,
        .Parity   = DRIVER_UART_NO_PARITY,
    };
    UART[0].Enable();
    UART[0].Config(&uartCfg);
    

#elif defined(__LPC12XX__)
    wdt_stop();

    GPIO0_REG.DIR |= PIN12_MSK;
    GPIO0_REG.OUTSET = PIN12_MSK;
    
    if (PMU_REG.PCON.DPDFLAG) {
        //! system boot becase wakeup from deep power down
        PMU_REG.PCON.DPDFLAG = 1;
    }
    
    PM.Clock.PLL.SelectSource(PLL_CLKSRC_SYSOSC);
# if PLL_ENABLED == ENABLED
    PM.Clock.PLL.Enable();
    PM.Clock.PLL.Config(PLL_MSEL, PLL_PSEL);
    PM.Clock.Core.Config(MAIN_CLKSRC_PLLOUT, PLL_OUT_CLK / CORE_CLK);
# else
    PM.Clock.PLL.SelectSource(PLL_CLKSRC_SYSOSC);
    PM.Clock.Core.Config(MAIN_CLKSRC_IRC, 1);
# endif

    //! wakeup pin
    IO_CFG(
        {PB3,  IO_WORKS_AS_FUNC0, IO_PULL_UP},
    );
    
    //! Debug uart init
    IO_CFG(
        {PA1,  IO_WORKS_AS_FUNC2, IO_PULL_UP},
        {PA2,  IO_WORKS_AS_FUNC2, IO_PULL_UP},
    );
    
    uart_cfg_t tCFG = {
        9600, 
        UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH
    };
    
    UART0.Enable();
    if (!UART0.Init(&tCFG)) {
    }
    
    //! RTC init
    RTC_CFG(
        RTC_START | RTC_SRC_RTC_PCLK | RTC_INTERRUPT_ENABLE,
        0
    );
    rtc_enable();
    rtc_set_match_value(rtc_get_match_value() + 0x10000);
    
    //! WDT init
    WDT_CFG(
#ifdef __DEBUG__
        WDT_FEED_ANYTIME | WDT_CASE_INT,
#else
        WDT_FEED_ANYTIME | WDT_CASE_RESET,
#endif
        WDT_CLK_SEL_WDT_OSC, //WDT_CLK_SEL_IRC
        1000000,    // 1s
        0,
        0xFFFFFF,
    );
    wdt_enable();
    wdt_start();
    
#elif defined(__LPC11XXX__)
    //! block deep power-down.
    PMU_REG.PCON |= 1u << PMU_PCON_NODPD_BIAS;
    
    GPIO1_REG.DIR |= PIN13_MSK;
    GPIO0_REG.DIR |= PIN7_MSK;
    GPIO0_REG.SET = PIN7_MSK;
    
    if (PMU_REG.PCON) {
        //! system boot becase wakeup from deep power down
        PMU_REG.PCON = 1;
    }
    
    pll_init(SYSPLL_CLKSRC_SYSOSC, 24MHz);
    core_clock_config(MAIN_CLKSRC_PLLOUT, 1);
    
    //! Debug uart init
    UART0.Enable();
    IO_CFG(
        {PORT0, PIN18,  IO_WORKS_AS_FUNC1, IO_PULL_UP},
        {PORT0, PIN19,  IO_WORKS_AS_FUNC1, IO_PULL_UP},
    );
    uart_cfg_t tCFG = {
        115200, 
        UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH
    };
    if (!UART0.Init(&tCFG)) {
    }
    GPIO0_REG.DIR |= PIN23_MSK;
    GPIO0_REG.CLR = PIN23_MSK;

    rtc_enable();
    rtc_start(0);
    
#elif defined(__LPC81X__)
    GPIO0_REG.DIR |= PIN1_MSK;
    
    if (PMU_REG.PCON) {
        //! system boot becase wakeup from deep power down
        PMU_REG.PCON = 1;
    }
    
    //! OSC pin allocating
    IO_CFG(
        {PORT0, PIN8,  PIN_FN_XTALIN,  PIN_MODE_PULL_UP},
        {PORT0, PIN9,  PIN_FN_XTALOUT, PIN_MODE_PULL_UP},
    );
    
    //! Debug uart pin
    IO_CFG(
        {PORT0, PIN0,  PIN_FN_U0_RXD, PIN_MODE_PULL_UP},
        {PORT0, PIN4,  PIN_FN_U0_TXD, PIN_MODE_PULL_UP},
    );
    
    //! 485 uart pin
    IO_CFG(
        {PORT0, PIN7,  PIN_FN_U1_RXD, PIN_MODE_PULL_UP},
        {PORT0, PIN6,  PIN_FN_U1_TXD, PIN_MODE_PULL_UP},
    );
    
    pll_init(SYSPLL_CKLSRC_IRC, 12MHz);//SYSPLL_CLKSRC_SYSOSC 
    core_clock_config(MAIN_CLKSRC_PLLIN, 1);
    uart_clock_config(1, 21);
    wdtosc_enable();
    
    UART0.Enable();
    uart_cfg_t tCFG = {
        9600, 
        UART_NO_PARITY | UART_1_STOPBIT | UART_8_BIT_LENGTH
    };
    if (!UART0.Config(&tCFG)) {
    }
    
#endif

//    NVIC_SetPriority(UART0_IRQn, 9);
//    NVIC_EnableIRQ(UART0_IRQn);
//    UART_INT_ENABLE(UART0, UART_IER_RBRIE_MSK);
    
//    NVIC_SetPriority(RTC_IRQn, 9);
//    NVIC_EnableIRQ(RTC_IRQn);

//    NVIC_SetPriority(WDT_IRQn, 9);
//    NVIC_EnableIRQ(WDT_IRQn);
    
    SysTick_Config(core_clock_get() / 100);
    
    ENABLE_GLOBAL_INTERRUPT();

    return true;
}


void debug_output_char(char cChar)
{
    while (false == UART[0].WriteByte(cChar));
}

int user_printf_output_char(char cChar)
{
    while (false == UART[0].WriteByte(cChar));
    return cChar;
}


/* EOF */
