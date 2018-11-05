/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#ifndef KEY_FILTER_SAMPLE_PERIOD
#error "KEY_FILTER_SAMPLE_PERIOD is not defined!"
#endif

#ifndef KEY_LONG_PRESS_TIME
#error "KEY_LONG_PRESS_TIME is not defined!"
#endif

#ifndef KEY_REPEAT_PRESS_TIME
#error "KEY_REPEAT_PRESS_TIME is not defined!"
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
    KEY_NO_ACTIVE = 0,
    KEY_PRESSED,
    KEY_RELEASED,
    KEY_KNOCKED,
    KEY_LONG_PRESSED,
    KEY_REPEAT_PRESSED,
} key_action_t;

/*============================ PROTOTYPES ====================================*/
//! return 0, represent there is no key active.
//! other value, each represent one key. e.g. 0: represent key 'A', 1: represent key 'B', 2: 'Ctrl + C'...
extern uint16_t key_get_key_value(void);
extern void key_callback(uint16_t Key, uint8_t Action);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static uint16_t PreKey       = KEY_NO_ACTIVE;
static uint16_t LastKeyValue = KEY_NO_ACTIVE;
static uint8_t  FSMState = KEY_NO_ACTIVE;

/*============================ IMPLEMENTATION ================================*/
void key_init(void)
{
    SAFE_ATOM_CODE(
        PreKey       = KEY_NO_ACTIVE;
        LastKeyValue = KEY_NO_ACTIVE;
        FSMState     = KEY_NO_ACTIVE;
    )
}

static bool key_scan(uint16_t *pKeyValue)
{
    uint16_t        CurrentKey  = KEY_NO_ACTIVE;
    static int16_t  Count       = 0;

    CurrentKey = key_get_key_value();
    
    //! key must keep stable within KEY_FILTER_SAMPLE_PERIOD.
    if (CurrentKey != PreKey) {     //! there is key active.
        PreKey  = CurrentKey;
        Count = 0;
    }
    if (Count < KEY_FILTER_SAMPLE_PERIOD) {
        Count++;
    } else {
        Count = 0;
        *pKeyValue = CurrentKey;
        return true;
    }

    return false;
}

static void key_press_detector(uint16_t Value, uint8_t Action)
{
    static uint16_t Timer = 0;
    
    switch (FSMState) {
        case KEY_NO_ACTIVE:
            if (Action == KEY_PRESSED) {
                key_callback(Value, KEY_PRESSED);
                FSMState = KEY_PRESSED;
                Timer = KEY_LONG_PRESS_TIME;
            }
            break;

        case KEY_PRESSED:
            if (Action == KEY_RELEASED) {
                key_callback(Value, KEY_RELEASED);
                key_callback(Value, KEY_KNOCKED);
                FSMState = KEY_NO_ACTIVE;
            } else {
                if (Timer != 0) {
                    Timer--;
                    if (Timer == 0) {
                        key_callback(Value, KEY_LONG_PRESSED);
                        FSMState = KEY_REPEAT_PRESSED;
                        Timer = KEY_REPEAT_PRESS_TIME;
                    }
                }
            }
            break;

        case KEY_REPEAT_PRESSED:
            if (Action == KEY_RELEASED) {
                key_callback(Value, KEY_RELEASED);
                FSMState = KEY_NO_ACTIVE;
            } else {
                if (Timer != 0) {
                    Timer--;
                    if (Timer == 0) {
                        key_callback(Value, KEY_REPEAT_PRESSED);
                        Timer = KEY_REPEAT_PRESS_TIME;
                    }
                }
            }
            break;
    }
}

void key_poll(void)
{
    uint16_t KeyValue;

    if (key_scan(&KeyValue)) {
        if (KeyValue != LastKeyValue) {
            if (LastKeyValue != KEY_NO_ACTIVE) {
                key_press_detector(LastKeyValue, KEY_RELEASED);
            }
            if (KeyValue != KEY_NO_ACTIVE) {
                key_press_detector(KeyValue, KEY_PRESSED);
            }
            LastKeyValue = KeyValue;
        } else {
            key_press_detector(LastKeyValue, KEY_NO_ACTIVE);
        }
    } else {
        key_press_detector(LastKeyValue, KEY_NO_ACTIVE);
    }
}

