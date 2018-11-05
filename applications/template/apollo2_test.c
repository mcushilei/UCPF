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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define BREATH_LED_ON()         GPIO.PORT1.Clear(BIT(44u - 32u))
#define BREATH_LED_OFF()        GPIO.PORT1.Set(BIT(44u - 32u))

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
DEBUG_DEFINE_THIS_FILE("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief  hardware initialization
 *  \param  none
 *  \retval true run the default initialization
 *  \retval false ignore the default initialization
 */
ROOT bool ON_HW_INIT(void)
{
        
    return true;
}




#define TOP         ((uint16_t)400)

/*! \brief set the 16-level led gradation
 *! \param hwLevel gradation
 *! \return none
 */
void breath_led(void)
{
    static uint16_t shwCounter = 0;
    static int16_t  snGray   = TOP >> 1;
    static uint16_t shwLevel = 0;
    
    if ((shwCounter >> 1) <= shwLevel) {
        BREATH_LED_ON();
    } else {
        BREATH_LED_OFF();
    }
    shwCounter++;
    if (shwCounter >= TOP) {
        shwCounter = 0;
        snGray++;
        if (snGray >= TOP) {
            snGray = 0;
        }
        shwLevel = ABS(snGray - (int16_t)(TOP >> 1));
    }
}


static volatile uint8_t rtcEvent = 0;

ISR(clkgen_isr)
{
    uint32_t status;
    
    status = driver_rtc_int_status_get(true);
    
    if (status & DRIVER_RTC_INT_ALM) {
        //rtcEvent = 1;
    }
    
    if (status & DRIVER_RTC_INT_OF) {
    }
    
    driver_rtc_int_clear(status);
}





/*! \note initialize application
 *  \param none
 *  \retval true hal initialization succeeded.
 *  \retval false hal initialization failed
 */
bool app_init(void)
{
//    STRING_PRINTF("\x1B[1;40;32m", 0);
    STRING_PRINTF("%s", "\r\nHello World!");
    DEBUG_PRINT("\r\n--------Test--------");
    
    fsm_init();

    ENABLE_GLOBAL_INTERRUPT();
    
    return true;
}

/*! \note deinitialize application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 *  \retval false hal deinitialization failed
 */
bool app_deinit(void)
{
    return true;
}

/*! \note deinitialize application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 *  \retval false hal deinitialization failed
 */
int app_main(void)
{
    if (!app_init()) {
        return -1;
    }
    
    fsm_start();
    
    while (1) {
        if (!fsm_scheduler()) {          //!< FSM scheduler
            //! try to enter a max allowed sleep mode
            //enter_lowpower_mode(SLEEP);
            
            if (rtcEvent) {
                rtcEvent = 0;
                STRING_PRINTF("%s", "\r\nHello World!");
            }
        } else {
        }
        breath_led();
    }

    if (!app_deinit()) {
        return -2;
    }

    return 0;
}


/* EOF */
