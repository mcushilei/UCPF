/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#ifdef __LPC17XX__
#elif defined(__LPC12XX__)
#define BREATH_LED_ON()     GPIO0_REG.OUTCLR = PIN12_MSK
#define BREATH_LED_OFF()    GPIO0_REG.OUTSET = PIN12_MSK
#elif defined(__LPC11E68__)
#define BREATH_LED_ON()     GPIO1_REG.CLR = PIN13_MSK
#define BREATH_LED_OFF()    GPIO1_REG.SET = PIN13_MSK
#elif defined(__LPC812__)
#define BREATH_LED_ON()     GPIO0_REG.CLR = PIN1_MSK
#define BREATH_LED_OFF()    GPIO0_REG.SET = PIN1_MSK
#endif
    
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
DEBUG_DEFINE_THIS_FILE("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/




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









static FSM_OBJ  ptTimeEvent;
static FSM_OBJ  ptTestEvent0;
static FSM_OBJ  testSem;

static FSM_TASK_STACK   stRTCTaskStack[2];
static PROTOTYPE_STATE(fsm_rtc_start);

DEF_STATE(fsm_rtc_start)
{
    uint8_t chError;
    
    chError = fsm_flag_wait(ptTimeEvent, 100);
    if (chError == FSM_ERR_OBJ_NOT_SINGLED) {
        return;
    }
    if (chError == FSM_ERR_TASK_PEND_TIMEOUT) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "RTC wait time out.");)
        fsm_semaphore_release(testSem, 3);
    } else if (chError == FSM_ERR_NONE) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "RTC wait time OK.");)
        fsm_flag_set(ptTestEvent0);
    }
    //FSM_CALL(fsm_test0_start, NULL);
}

static FSM_TASK_STACK   stTestTaskStack0[1];
static PROTOTYPE_STATE(fsm_test0_start);

DEF_STATE(fsm_test0_start)
{
    uint8_t chError;
    
    chError = fsm_flag_wait(ptTestEvent0, 100);
    if (chError == FSM_ERR_OBJ_NOT_SINGLED) {
        return;
    }
    if (chError == FSM_ERR_TASK_PEND_TIMEOUT) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "ptTestEvent0 wait time out.");)
    } else if (chError == FSM_ERR_NONE) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "ptTestEvent0 wait time OK.");)
    }
    
    //FSM_COMPLETE();
}



static FSM_OBJ    ptTestEvent3;
static FSM_OBJ    ptTestMutex;

static FSM_TASK_STACK   stTestTaskStack1[1];
static PROTOTYPE_STATE(fsm_test1_start);
static PROTOTYPE_STATE(fsm_test1_a);
static PROTOTYPE_STATE(fsm_test1_b);
static PROTOTYPE_STATE(fsm_test1_c);
static PROTOTYPE_STATE(fsm_test1_d);
static PROTOTYPE_STATE(fsm_test1_delay);

DEF_STATE(fsm_test1_start)
{
    uint8_t chError;
    
    chError = fsm_mutex_wait(ptTestMutex, 0);
    if (chError == FSM_ERR_OBJ_NOT_SINGLED) {
        return;
    } else if (chError == FSM_ERR_TASK_PEND_TIMEOUT) {
        return;
    } else if (chError == FSM_ERR_NONE) {
        STRING_PRINTF("\r\n", 0);
        FSM_TRANSFER_TO(fsm_test1_a, NULL);
    }
}

DEF_STATE(fsm_test1_a)
{
    STRING_PRINTF("a", 0);
    FSM_TRANSFER_TO(fsm_test1_b, NULL);
}

DEF_STATE(fsm_test1_b)
{
    STRING_PRINTF("b", 0);
    FSM_TRANSFER_TO(fsm_test1_c, NULL);
}

DEF_STATE(fsm_test1_c)
{
    STRING_PRINTF("c", 0);
    FSM_TRANSFER_TO(fsm_test1_d, NULL);
}

DEF_STATE(fsm_test1_d)
{
    STRING_PRINTF("d", 0);
    FSM_TRANSFER_TO(fsm_test1_delay, NULL);
}

DEF_STATE(fsm_test1_delay)
{
    fsm_err_t err;
    
    err = fsm_task_delay(100);
    if (err == FSM_ERR_TASK_PEND_TIMEOUT) {
        fsm_mutex_release(ptTestMutex);
        FSM_TRANSFER_TO(fsm_test1_start, NULL);
    }
}





static FSM_TASK_STACK   stTestTaskStack2[1];
static PROTOTYPE_STATE(fsm_test2_start);
static PROTOTYPE_STATE(fsm_test2_a);
static PROTOTYPE_STATE(fsm_test2_b);
static PROTOTYPE_STATE(fsm_test2_c);
static PROTOTYPE_STATE(fsm_test2_d);
static PROTOTYPE_STATE(fsm_test2_delay);

DEF_STATE(fsm_test2_start)
{
    uint8_t chError;
    
    chError = fsm_mutex_wait(ptTestMutex, 0);
    if (chError == FSM_ERR_OBJ_NOT_SINGLED) {
        return;
    } else if (chError == FSM_ERR_TASK_PEND_TIMEOUT) {
        return;
    } else if (chError == FSM_ERR_NONE) {
        STRING_PRINTF("\r\n", 0);
        FSM_TRANSFER_TO(fsm_test2_a, NULL);
    }
}

DEF_STATE(fsm_test2_a)
{
    STRING_PRINTF("A", 0);
    FSM_TRANSFER_TO(fsm_test2_b, NULL);
}

DEF_STATE(fsm_test2_b)
{
    STRING_PRINTF("B", 0);
    FSM_TRANSFER_TO(fsm_test2_c, NULL);
}

DEF_STATE(fsm_test2_c)
{
    STRING_PRINTF("C", 0);
    FSM_TRANSFER_TO(fsm_test2_d, NULL);
}

DEF_STATE(fsm_test2_d)
{
    STRING_PRINTF("D", 0);
    fsm_mutex_release(ptTestMutex);
    FSM_TRANSFER_TO(fsm_test2_delay, NULL);
}

DEF_STATE(fsm_test2_delay)
{
    fsm_err_t err;
    
    err = fsm_task_delay(530);
    if (err == FSM_ERR_TASK_PEND_TIMEOUT) {
        FSM_TRANSFER_TO(fsm_test2_start, NULL);
    }
}




static FSM_TASK_STACK   stTestTaskStack3[1];
static PROTOTYPE_STATE(fsm_test3_start);

DEF_STATE(fsm_test3_start)
{
    uint8_t chError;
    
    chError = fsm_semaphore_wait(testSem, 0);
    if (chError == FSM_ERR_OBJ_NOT_SINGLED) {
        return;
    } else if (chError == FSM_ERR_TASK_PEND_TIMEOUT) {
        return;
    } else if (chError == FSM_ERR_NONE) {
        STRING_PRINTF("\r\nhahaha.", 0);
    }
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

    if (FSM_ERR_NONE != fsm_flag_create(&ptTimeEvent, false, false)) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not initialise event.");)
    }
    if (FSM_ERR_NONE != fsm_flag_create(&ptTestEvent0, false, true)) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not initialise event.");)
    }
    if (FSM_ERR_NONE != fsm_flag_create(&ptTestEvent3, false, false)) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not initialise event.");)
    }
    if (FSM_ERR_NONE != fsm_mutex_create(&ptTestMutex)) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not initialise mutex.");)
    }
    if (FSM_ERR_NONE != fsm_semaphore_create(&testSem, 0)) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not initialise sem.");)
    }

    if (FSM_ERR_NONE != fsm_task_create(
                                        NULL,
                                        REF_STATE(fsm_rtc_start),
                                        NULL,
                                        stRTCTaskStack,
                                        ARRAY_LENGTH(stRTCTaskStack))) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not creat task fsm_rtc_start.");)
    }
    if (FSM_ERR_NONE != fsm_task_create(
                                        NULL,
                                        REF_STATE(fsm_test0_start),
                                        NULL,
                                        stTestTaskStack0,
                                        ARRAY_LENGTH(stTestTaskStack0))) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not creat task fsm_test0_start.");)
    }
    if (FSM_ERR_NONE != fsm_task_create(
                                        NULL,
                                        REF_STATE(fsm_test1_start),
                                        NULL,
                                        stTestTaskStack1,
                                        ARRAY_LENGTH(stTestTaskStack1))) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not creat task 1.");)
    }
    if (FSM_ERR_NONE != fsm_task_create(
                                        NULL,
                                        REF_STATE(fsm_test2_start),
                                        NULL,
                                        stTestTaskStack2,
                                        ARRAY_LENGTH(stTestTaskStack2))) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not creat task 2.");)
    }
    if (FSM_ERR_NONE != fsm_task_create(
                                        NULL,
                                        REF_STATE(fsm_test3_start),
                                        NULL,
                                        stTestTaskStack3,
                                        ARRAY_LENGTH(stTestTaskStack3))) {
        DEBUG_MSG(TEMPLATE_DEBUG, STRING_PRINTF("%s", "Can not creat task 3.");)
    }
    
    
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
        } else {
        }
        breath_led();
    }

    if (!app_deinit()) {
        return -2;
    }

    return 0;
}

static volatile uint32_t wCnt = 0;

ISR(SysTick_Handler)
{
    fsm_tick();
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
    
    STRING_PRINTF("\r\n%u", i);
    i++;
#elif defined(__LPC12XX__)
#elif defined(__LPC11XXX__)
    rtc_alarm_clear_intflag();

#endif
}

/* EOF */
