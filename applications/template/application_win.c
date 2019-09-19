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

void utc_to_beijing(date_time_t *pDateTime)
{
    int32_t  d;
    uint32_t s;
    
    s  = time_to_seconds(&pDateTime->Time);
    s += 8u * SECONDS_OF_HOUR;
    
    d = seconds_to_time(&pDateTime->Time, s);
    date_plus_days(&pDateTime->Date, d);
}

void clock_alarm_routine(clock_alarm_t *alarm, bool isTimeout)
{
    if (isTimeout) {
        date_time_t time = rtc_api_get_time();
        printf("\r\n%u-%02u-%02u %02u:%02u:%02u", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);

        if (time.Hour == 12u && time.Minute == 0u && time.Second == 6u) {
            time.Hour = 23;
            time.Minute = 59;
            clock_set_time(&time);
        }
    }
    if (alarm->Time >= SECONDS_OF_DAY) {
        free(alarm);
    }
}



/*! \note user's application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 */
int app_main(void)
{
    printf("\r\nHello word.");


    static date_time_t startTime = { .Year = 2000, .Month = 1, .Day = 1,  .Hour = 0, .Minute = 0, .Second = 0 };
    static date_time_t endTime   = { .Year = 2019, .Month = 9, .Day = 12, .Hour = 10, .Minute = 10, .Second = 0 };

    int32_t days = seconds_to_time(&startTime.Time, 622031716);
    date_plus_days(&startTime.Date, days);
    printf("\r\n%u-%02u-%02u %02u:%02u:%02u", startTime.Year, startTime.Month, startTime.Day, startTime.Hour, startTime.Minute, startTime.Second);


    printf("\r\n");
    curly_bracket_paser_t objPaser;
    static char *test = "{{{{{{{ldhfa}djos{id}andf}dfoa}}{}hkds}hkd";
    uint32_t bytesTrans = strlen(test);

    curly_bracket_paser_init(&objPaser, 1, NULL);
    for (uint32_t i = 0; i < bytesTrans; i++) {
        curly_bracket_paser(&objPaser, test[i], false);
    }



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


    //while (1) {
    //    OS_TASK_SLEEP(10000);
    //    printf("\r\n%u", rtc_api_get_ticktock());
    //    date_time_t time = rtc_api_get_time();
    //    printf("\r\n%u-%02u-%02u %02u:%02u:%02u", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
    //}

    return 0;
}









/* EOF */
