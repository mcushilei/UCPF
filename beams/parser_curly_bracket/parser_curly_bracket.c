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
#include "./app_cfg.h"
#include "./parser_curly_bracket.h"

/*============================ MACROS ========================================*/
THIS_FILE_NAME("paser_curly_bracket");

enum {
    WAIT_FOR_HEAD = 0,
    WAIT_FOR_TAIL,
};

/*============================ MACROFIED FUNCTIONS ===========================*/
#define PASER_Q_INIT()                  QUEUE_UINT8_INIT(                       \
                                                &((pasrer)->RcvQueue),          \
                                                (pasrer)->RcvQueueBuffer,       \
                                                CURLY_BRACKET_PARSER_MAX_LENGTH)
#define PASER_Q_PUT(__BYTE)             QUEUE_ENQUEUE(&pasrer->RcvQueue, __BYTE)
#define PASER_Q_GET(__PBYTE)            QUEUE_DEQUEUE(&pasrer->RcvQueue, __PBYTE)
#define PASER_Q_PEEK(__PBYTE)           QUEUE_PEEK(&pasrer->RcvQueue, __PBYTE)
#define PASER_Q_CLEAR_PEEK()            QUEUE_GET_ALL_PEEKED(&pasrer->RcvQueue)

/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
static const char DATAGRAM_HEAD_BYTE = '{';
static const char DATAGRAM_TAIL_BYTE = '}';

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/


bool curly_bracket_paser_init(curly_bracket_paser_t *pasrer, uint32_t maxLayer, curly_bracket_paser_callback_t *callback)
{
    if (NULL == pasrer) {
        return false;
    }
    if (0u == maxLayer) {
        return false;
    }

    memset(pasrer, 0, sizeof(curly_bracket_paser_t));
    PASER_Q_INIT();
    pasrer->MaxLayer = maxLayer;
    pasrer->Callback = callback;

    return true;
}

bool curly_bracket_paser_deinit(curly_bracket_paser_t *pasrer)
{
    return true;
}

void curly_bracket_paser(curly_bracket_paser_t *pasrer, char byte, bool timeout)
{
    uint8_t rubishCan;

    if (timeout) {                      //!< byte timeout.
        PASER_Q_GET(&rubishCan);
        pasrer->State = WAIT_FOR_HEAD;
    } else if (!PASER_Q_PUT(byte)) {    //!< queue full?
        PASER_Q_GET(&rubishCan);
        PASER_Q_PUT(byte);
        pasrer->State = WAIT_FOR_HEAD;
    }
            
    do {
        if (!PASER_Q_PEEK(&byte)) {
            break;
        }

        switch (pasrer->State) {
            case WAIT_FOR_HEAD:
                if (DATAGRAM_HEAD_BYTE == byte) {
                    pasrer->RcvWriteIndex = 0u;
                    pasrer->RcvFrameBuffer[pasrer->RcvWriteIndex++] = byte;
                    pasrer->State = WAIT_FOR_TAIL;
                    pasrer->Layer = 1u;
                } else {
                    PASER_Q_GET(&rubishCan);
                }
                break;
                        
            case WAIT_FOR_TAIL:
                pasrer->RcvFrameBuffer[pasrer->RcvWriteIndex++] = byte;
                if (DATAGRAM_HEAD_BYTE == byte) {
                    pasrer->Layer++; 
                    if (pasrer->MaxLayer < pasrer->Layer) {
                        PASER_Q_GET(&rubishCan);
                        pasrer->State = WAIT_FOR_HEAD;
                    }
                } else if (DATAGRAM_TAIL_BYTE == byte) {
                    if (pasrer->Layer == 0u) {
                        PASER_Q_GET(&rubishCan);
                        pasrer->State = WAIT_FOR_HEAD;
                    } else {
                        pasrer->Layer--;
                        if (pasrer->Layer == 0u) {
                            PASER_Q_CLEAR_PEEK();
                            pasrer->State = WAIT_FOR_HEAD;

                            //! handle the frame here.
                            pasrer->RcvFrameBuffer[pasrer->RcvWriteIndex] = '\0';
                            if (NULL != pasrer->Callback) {
                                pasrer->Callback(pasrer->RcvFrameBuffer, pasrer->RcvWriteIndex);
                            }
                        }
                    }
                }
                break;
        }
    } while (1);
}

/* EOF */
