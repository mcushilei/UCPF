/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#define __FRAMEWORKS_TEMPLATE_FRAMEWORK_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool framework_init(void)
{
    //！Put all devices on board to initial status.
    if (!board_init()) {
        return false;
    }
    
    //! malloc hardware resouse used by your application. such as clock, interrupt, pin, etc
    PIN_CFG(
        {0, 43, DRIVER_PIN_FUNC(3) | DRIVER_PIN_OUT_PUSHPULL},
        {0, 44, DRIVER_PIN_FUNC(3) | DRIVER_PIN_OUT_PUSHPULL},
        
        {0, 22, DRIVER_PIN_22_UART0TX},
        {0, 23, DRIVER_PIN_23_UART0RX},
    );
    
    GPIO.PORT1.SetOutput(BIT(43u - 32u) | BIT(44u - 32u));
    
    
    static const uart_cfg_t uartCfg = {
        .Baudrate = 9600,
        .DataBits = DRIVER_UART_8_BITS_LENGTH,
        .StopBits = DRIVER_UART_1_STOPBITS,
        .Parity   = DRIVER_UART_NO_PARITY,
    };
    UART[0].Enable();
    UART[0].Close();
    UART[0].Config(&uartCfg);
    UART[0].Open();
    
    driver_clkgen_osc_start(DRIVER_CLKGEN_OSC_XT);
    driver_rtc_osc_select(DRIVER_RTC_OSC_XT);
    driver_rtc_alarm_interval_set(DRIVER_RTC_ALM_RPT_SEC);
    driver_rtc_int_enable(DRIVER_RTC_INT_ALM | DRIVER_RTC_INT_OF);
    driver_rtc_start();
    NVIC_EnableIRQ(CLKGEN_IRQn);
    
    //! initiate software modules, such as RTOS

    
    //! return TRUE to start application.
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
