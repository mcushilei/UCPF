/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./boards/boards.h"
#include "./deadline_type.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
THIS_FILE_NAME("framework");

uint32_t err_log[64];

volatile uint32_t sysTickCounter;

/*============================ IMPLEMENTATION ================================*/

static void dead_line_get_tick(uint32_t *value)
{
    *value = sysTickCounter;
}

static uint32_t dead_line_ms_to_tick(uint32_t timeMS)
{
    return timeMS / 10u;
}

static uint32_t dead_line_tick_to_ms(uint32_t ticks)
{
    return 10u * ticks;
}

void deadline_init(deadline_t *dlTimer)
{
	dlTimer->EndTime = 0;
    dlTimer->StartTime = 0;
}

bool deadline_is_expired(deadline_t *dlTimer)
{
	uint32_t now, deltaExpire, deltaDeadline;

	dead_line_get_tick(&now);
    deltaExpire = now - dlTimer->StartTime;
    deltaDeadline = dlTimer->EndTime - dlTimer->StartTime;
    if (deltaExpire >= deltaDeadline) {
        return true;
    } else {
        return false;
    }
}

void deadline_set_ms(deadline_t *dlTimer, uint32_t timeout)
{
	dead_line_get_tick(&dlTimer->StartTime);
    dlTimer->EndTime = dlTimer->StartTime + dead_line_ms_to_tick(timeout);
}

void deadline_set(deadline_t *dlTimer, uint32_t timeout)
{
    deadline_set_ms(dlTimer, timeout * 1000);
}

int deadline_left_ms(deadline_t *dlTimer)
{
	uint32_t now, deltaExpire, deltaDeadline;

	dead_line_get_tick(&now);
    deltaExpire = now - dlTimer->StartTime;
    deltaDeadline = dlTimer->EndTime - dlTimer->StartTime;
    if (deltaExpire >= deltaDeadline) {
        return 0;
    } else {
        return dead_line_tick_to_ms(deltaDeadline - deltaExpire);
    }
}


/** \brief  hardware initialization
 *  \param  none
 *  \retval true run the default initialization
 *  \retval false ignore the default initialization
 */
ROOT bool ON_HW_INIT(void)
{
    //WDT.Disable();

    /*! you can put your hardware initialization code here */
    
    return true;
}

#define TOP         ((uint16_t)400)

/** \brief set the 16-level led gradation
 *  \param hwLevel gradation
 *  \return none
 */
void breath_led(void)
{
    static uint16_t shwCounter = 0;
    static int16_t  snGray   = TOP >> 1;
    static uint16_t shwLevel = 0;
    
    if( (shwCounter >> 1) <= shwLevel ) {
        BREATH_LED_ON();
    } else {
        BREATH_LED_OFF();
    }
    shwCounter++;
    if( shwCounter >= TOP ) {
        shwCounter = 0;
        snGray++;
        if (snGray >= TOP) {
            snGray = 0;
        }
        shwLevel = ABS(snGray - (int16_t)(TOP >> 1));
    }
}

bool framework_init(void)
{
    //！Put all hardware devices to initial status.
    if (!board_init()) {
        return false;
    }
    
    DBG_LOG("Hello World!");

    SysTick_Config(core_clock_get() / 100);
    
    app_main();
    
    // should not run here.
    while (1) {
        breath_led();
    }
    
    return true;
}

//! lowl level fault handler, called from hardfault interrupt.
void framework_fault_handle(void)
{
#ifdef __DEBUG__
    
    uint32_t cpuclock = core_clock_get();
    
    while (1) {
        printf("\r\n********************* error *************************");
        for (uint32_t i = 0; i < UBOUND(err_log); i++) {
            printf("\r\n%08X", err_log[i]);
        }

        
        for (uint32_t i = cpuclock * 3; i; i--) {
            i = i;
        }        
    }
#else
    NVIC_SystemReset();
#endif
}

void debug_output_char(char ch)
{
    while (!UART0.WriteByte(ch));
}

int user_printf_output_char(char ch)
{
    while (!UART0.WriteByte(ch));
    return ch;
}

#if   defined ( __ICCARM__ )
int putchar(int ch)
{
    while (!UART0.WriteByte(ch));
    return ch;
}
#elif defined ( __CC_ARM )
int fputc(int ch, FILE *f)
{
    while (!UART0.WriteByte(ch));
    return ch;
}
#elif defined ( __GNUC__ )
__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    while (!UART0.WriteByte(*ptr));
    return len;
}
#endif


ISR(SysTick_Handler)
{
    sysTickCounter++;
}

/* EOF */
