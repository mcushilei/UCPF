/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./short_telecom.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
DEBUG_DEFINE_THIS_FILE("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/


void clock_alarm_routine(rtc_alarm_t *alarm, void *arg)
{
    char stringBuf[32];
    printf("\r\nalarm at %s", rtc_api_get_time_string(stringBuf));
}

/*! \note user's application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 */
int app_main(void)
{
    printf("\r\nHello word.");




    date_time_t startTime = { .Year = 2000,.Month = 1,.Day = 1,.Hour = 0,.Minute = 0,.Second = 0 };
    date_time_t endTime = clock_get_time();

    uint32_t dayOfWeek = 0;
    int32_t days = count_days_between(&startTime.Date, &endTime.Date);
    dayOfWeek = (days + 6 - 1) % 7u;
    printf("\r\n dayOfWeek = %u", dayOfWeek);


    char buf[40] = {0};
    if (sscanf("[+2,3,4,5+--", "[%[^\]]", buf) < 1) {
        DBG_LOG("got data prefix invailed!");
    } else {
        printf("\r\n%s", buf);
    }

    //printf("\r\n");
    //curly_bracket_paser_t objPaser;
    //static char *test = "adjf098rfp}jaf{{{fj098du}}fup9ae{{{{ldhfa}djos{id}andf}dfoa}}{}hkds}hkd";
    //uint32_t bytesTrans = strlen(test);

    //curly_bracket_paser_init(&objPaser, 2, NULL);
    //for (uint32_t j = 0; j < 100; j++) {
    //    for (uint32_t i = 0; i < bytesTrans; i++) {
    //        curly_bracket_paser(&objPaser, test[i], false);
    //    }
    //}



    //for (uint32_t i = 0u; i < 3u; i++) {
    //    clock_alarm_t *alarm = malloc(sizeof(clock_alarm_t));
    //    startTime.Second += 2;
    //    clock_add_alarm(alarm, &startTime.Time, clock_alarm_routine);
    //}

    //for (uint32_t i = 0u; i < 3u; i++) {
    //    clock_alarm_t *alarm = malloc(sizeof(clock_alarm_t));
    //    endTime.Second += 3;
    //    clock_add_timer(alarm, &endTime, clock_alarm_routine);
    //}

    char stringBuf[32];
    rtc_alarm_t *myAlarm;
    myAlarm = rtc_alarm_creat(0xFFu, ~0u, &clock_alarm_routine, NULL);
    if (NULL == myAlarm) {
        printf("\r\n rtc_alarm_creat() fail!");
        return -1;
    }
    time24_t alarmTime = { .Hour = 6,.Minute = 30,.Second = 0 };
    if (!rtc_alarm_start(myAlarm, &alarmTime)) {
        printf("\r\n rtc_alarm_start() fail!");
        return -1;
    }

    uint8_t dataBuf[256];
    uint32_t len;
    while (1) {
        ModBus_Master_Send(0, 1, 3, 0x0000, 13, dataBuf, 0, 500);
        len = ModBus_Master_Recv(0, 1, 3, dataBuf, sizeof(dataBuf), 5000);
        if (len > 0) {
            for (uint32_t i = 0; i < len; i++) {
                printf("%02X ", dataBuf[i]);
            }
            printf("\r\n");
        }
        OS_TASK_SLEEP(5000);
        printf("\r\n%s", rtc_api_get_time_string(stringBuf));
    }

    return 0;
}









/* EOF */
