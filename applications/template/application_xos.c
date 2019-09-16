/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
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
DEBUG_DEFINE_THIS_FILE("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/


#define TOP         ((uint16_t)800)

/*! \brief set the 16-level led gradation
 *! \param hwLevel gradation
 *! \return none
 */
void breath_led(void)
{
    static uint16_t shwCounter = 0;
    static int16_t  snGray   = TOP >> 1;
    static uint16_t shwLevel = 0;
    
    if (shwCounter <= shwLevel) {
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











OS_HANDLE debugPortQueue = NULL;
void *debugPortQueueBuffer[4];

OS_HANDLE timeFlag  = NULL;
OS_HANDLE mutexLED0 = NULL;
OS_HANDLE mutexLED1 = NULL;
OS_HANDLE mutexLED2 = NULL;
OS_HANDLE ledSem = NULL;

#define MAIN_THREAD_PRIO                (3u)
#define MAIN_THREAD_STACK_SIZE          (70u)

#define DEBUG_PORT_THREAD_PRIO          (0u)
#define DEBUG_PORT_THREAD_STACK_SIZE    (60u)

#define LED_THREAD_PRIO                 (1u)
#define LED_THREAD_STACK_SIZE           (60u)

#define TIME_THREAD_PRIO                (2u)
#define TIME_THREAD_STACK_SIZE          (60u)

static OS_TASK_ENTRY(debug_port_thread)
{
    OS_ERR      err;
    uint8_t     byte;
    char *readBuffer;
        
    while (1) {
        err = OS_QUEUE_READ(debugPortQueue, &readBuffer, 100);
        if (err == OS_ERR_NONE) {
            STRING_PRINTF("%s", readBuffer);
        } else {
            STRING_PRINTF("%s", "debug task wait timer out.");
        }
    }
}

static OS_TASK_ENTRY(led_thread)
{
    OS_ERR err;
    UINT32 i;
        
    while (1) {
        err = osMutexPend(mutexLED0, OS_INFINITE);
        if (err != OS_ERR_NONE) {
        }
        
        for (i = 3; i != 0; i--) {
            OS_TASK_SLEEP(10);
            BREATH_LED_ON();
            OS_TASK_SLEEP(10);
            BREATH_LED_OFF();
            
            char * const stringArray[3] = {"string 1.", "srting 2", "string 3"};
            err = OS_QUEUE_WRITE(debugPortQueue, &stringArray[i - 1], 0);
            if (err != OS_ERR_NONE) {
                STRING_PRINTF("%s", "time thread write queue fail.");
            }
    
            osSemPost(ledSem, 1);
        }
        
        err = osMutexPost(mutexLED0);
        if (err != OS_ERR_NONE) {
        }
    }
}

static OS_TASK_ENTRY(time_thread)
{
    OS_ERR err;
    
    while (1) {
        err = osMutexPend(mutexLED0, OS_INFINITE);
        if (err != OS_ERR_NONE) {
        }
        err = osMutexPend(mutexLED1, OS_INFINITE);
        if (err != OS_ERR_NONE) {
        }
        err = osMutexPend(mutexLED2, OS_INFINITE);
        if (err != OS_ERR_NONE) {
        }
            
        while (OS_ERR_NONE == osSemPend(ledSem, 57)) {
            err = osFlagPend(timeFlag, 20);
            if (err == OS_ERR_NONE) {
            } else if (err == OS_ERR_TIMEOUT) {
            }
            BREATH_LED_ON();
            err = osFlagPend(timeFlag, 20);
            if (err == OS_ERR_NONE) {
            } else if (err == OS_ERR_TIMEOUT) {
            }
            BREATH_LED_OFF();
        }
        
        err = osMutexPost(mutexLED0);
        if (err != OS_ERR_NONE) {
        }
        err = osMutexPost(mutexLED1);
        if (err != OS_ERR_NONE) {
        }
        err = osMutexPost(mutexLED2);
        if (err != OS_ERR_NONE) {
        }
        OS_TASK_SLEEP(5);
        
//        
//        osSemPost(ledSem, 5);
//        osSemSet(ledSem, 5);
    }
}

static OS_TASK_ENTRY(main_thread)
{
    OS_HANDLE taskHandle;
        
    osStatInit();
    
    OS_TASK_CREATE(
        taskHandle,
        NULL,
        &led_thread,
        NULL,
        LED_THREAD_STACK_SIZE,
        LED_THREAD_PRIO,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
    OS_TASK_CREATE(
        taskHandle,
        NULL,
        &time_thread,
        NULL,
        TIME_THREAD_STACK_SIZE,
        TIME_THREAD_PRIO,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
    OS_TASK_CREATE(
        taskHandle,
        NULL,
        &debug_port_thread,
        NULL,
        DEBUG_PORT_THREAD_STACK_SIZE,
        DEBUG_PORT_THREAD_PRIO,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);


    while (1) {
        STRING_PRINTF("%s", "\r\nThis is main thread.");
        OS_TASK_SLEEP(100);
    }
}


/*! \note initialize application
 *  \param none
 *  \retval true hal initialization succeeded.
 *  \retval false hal initialization failed
 */
bool app_init(void)
{
    OS_ERR err;
    
    DEBUG_PRINT("\r\n--------Test--------");
//    string_printf("\x1B[1;40;32m", 0);
    STRING_PRINTF("\r\nHello World!", 0);
    
    
    err = osFlagCreate(&timeFlag, false, false);
    if (err != 0) {
        while (1);
    }
    
    err = OS_QUEUE_CREATE(debugPortQueue, UBOUND(debugPortQueueBuffer), sizeof(debugPortQueueBuffer[0]));
    if (err != 0) {
        while (1);
    }
    
    err = osMutexCreate(&mutexLED0, OS_TASK_LOWEST_PRIO);
    if (err != 0) {
        while (1);
    }
    
    err = osMutexCreate(&mutexLED1, OS_TASK_LOWEST_PRIO);
    if (err != 0) {
        while (1);
    }
    
    err = osMutexCreate(&mutexLED2, OS_TASK_LOWEST_PRIO);
    if (err != 0) {
        while (1);
    }
    
    err = osSemCreate(&ledSem, 8);
    if (err != 0) {
        while (1);
    }
    

    OS_HANDLE taskHandle;
    
    OS_TASK_CREATE(
        taskHandle,
        NULL,
        &main_thread,
        NULL,
        MAIN_THREAD_STACK_SIZE,
        MAIN_THREAD_PRIO,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    
    
    return true;
}


/*! \note deinitialize application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 *  \retval false hal deinitialization failed
 */
int app_main(void)
{
    osInit();
    
    app_init();
    
    osStart();
    
    while (1) {
        breath_led();
    }
    
    return 0;
}


ISR(SysTick_Handler)
{
    osIntEnter();
    osSysTick();
    osIntExit();
}

ISR(WDT_IRQHandler)
{
    BREATH_LED_OFF();
}



/* EOF */
