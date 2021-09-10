/*******************************************************************************
 *  Copyright(C)2015-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
THIS_FILE_NAME("application_arm");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

/*! \note deinitialize application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 *  \retval false hal deinitialization failed
 */
int app_main(void)
{
    deadline_t delay;
    
    deadline_init(&delay);
    deadline_set_ms(&delay, 2000);
    
    DBG_LOG("Core Clock: %u", core_clock_get());

    
    while (1) {
        if( deadline_is_expired(&delay) ) {
            deadline_set_ms(&delay, 2000);
            DBG_LOG("Hello World!");
        }
    }
    
    return 0;
}


ISR(WDT_IRQHandler)
{
    BREATH_LED_OFF();
}

ISR(RTC_IRQHandler)
{
#ifdef __LPC17XX__
    uint32_t reg;
    static uint32_t i = 0;
    
    reg = RTC_REG.ILR;
    RTC_REG.ILR = reg & (
           (1u << 0) 
         | (1u << 1));
    
    if (reg & (1u << 0)) {      //!< counter increment interrupt.
    }
    
    if (reg & (1u << 1)) {      //!< alarm interrupt.
    }
    
    DBG_LOG("\r\n%u", i);
    i++;
#elif defined(__LPC12XX__)
#elif defined(__LPC11XXX__)
    rtc_alarm_clear_intflag();

#endif
}

/* EOF */
