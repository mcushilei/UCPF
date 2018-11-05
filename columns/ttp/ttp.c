/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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
/*
|--HEAD--|--PORT--|-------LENGTH-------|------PAYLOAD------|--------CRC--------|
|---1----|----1---|----------2---------|---------n---------|---------2---------|
*/

//! \note do not move this pre-processor statement to other places
#define __TTP_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\debug\debug.h"
#include "..\hash\hash.h"
#include ".\ttp_public.h"

/*============================ MACROS ========================================*/
#define TTP_CRC16_POLLY                CRC16_POLY_CCITT

/*============================ MACROFIED FUNCTIONS ===========================*/
#define TTP_QUEUE_INIT()               QUEUE_UINT8_INIT(               \
                                                &obj->RcvQueue,            \
                                                obj->RcvQueueBuffer,         \
                                                TTP_PAYLOAD_MAX_SIZE + 6)
#define TTP_QUEUE_DEINIT()             QUEUE_DEINIT(&obj->RcvQueue)
#define TTP_ENQUEUE(__BYTE)            QUEUE_ENQUEUE(&obj->RcvQueue, __BYTE)
#define TTP_DEQUEUE(__PBYTE)           QUEUE_DEQUEUE(&obj->RcvQueue, __PBYTE)
#define TTP_PEEK_QUEUE(__PBYTE)        QUEUE_PEEK(&obj->RcvQueue, __PBYTE)
#define TTP_GET_ALL_PEEKED_QUEUE()     QUEUE_GET_ALL_PEEKED(&obj->RcvQueue)
#define TTP_REST_PEEK_QUEUE()          QUEUE_RESET_PEEK(&obj->RcvQueue)

#define TTP_CHECKSUM(__BYTE, __CS)      __CS = crc16_calculator(TTP_CRC16_POLLY, __CS, __BYTE)

#define TTP_OUTPUT_BYTE(__BYTE)         obj->OutputByte(__BYTE)

/*============================ TYPES =========================================*/
enum {
    RCV_PASER = 0,
    RCV_HANDLE,
};

enum {
    WAIT_FOR_HEAD = 0,
    WAIT_FOR_PORT,
    WAIT_FOR_LENGTH_0,
    WAIT_FOR_LENGTH_1,
    WAIT_FOR_DATA,
    WAIT_FOR_CHECKSUM_0,
    WAIT_FOR_CHECKSUM_1,
};

enum {
    SND_HEAD = 0,
    SND_PORT,
    SND_LENGTH_0,
    SND_LENGTH_1,
    SND_DATA,
    SND_CHECKSUM_0,
    SND_CHECKSUM_1,
};

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
DEBUG_DEFINE_THIS_FILE("TTP");

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

bool ttp_ini(   ttp_t               *obj,
                ttp_output_byte_t   *pOutputByte,
                ttp_poll_byte_t     *pPollByte)
{
    obj->OutputByte = pOutputByte;
    obj->PollByte   = pPollByte;

    obj->RcvState0 = RCV_PASER;
    obj->RcvState1 = WAIT_FOR_HEAD;
    obj->SndState  = SND_HEAD;

    return TTP_QUEUE_INIT();
}

uint8_t ttp_rcv_fsm(ttp_t *obj, uint8_t *pPort, uint8_t *pBuffer, uint16_t *pLength)
{
    uint8_t byte, dummy;

    switch (obj->RcvState0) {
        case RCV_PASER: {
            bool bReturn = false;
            bool timeout = 0u;

            if (obj->PollByte(&byte, &timeout)) {
                if (timeout) {                      //!< byte timeout.
                    TTP_DEQUEUE(&dummy);
                    obj->RcvState1 = WAIT_FOR_HEAD;
                } else if (!TTP_ENQUEUE(byte)) {   //!< byte received.
                    TTP_DEQUEUE(&dummy);
                    TTP_ENQUEUE(byte);
                    obj->RcvState1 = WAIT_FOR_HEAD;
                }
            }
                
            do {
                if (!TTP_PEEK_QUEUE(&byte)) {
                    return FSM_RT_ONGOING;
                }

                switch (obj->RcvState1) {
                    case WAIT_FOR_HEAD:
                        if (TTP_HEAD_BYTE == byte) {
                            obj->RcvChecksum   = 0xFFFF;
                            obj->RcvDataLength = 0;
                            obj->RcvWritePoint = 0;
                            obj->RcvState1 = WAIT_FOR_PORT;
                        } else {
                            TTP_DEQUEUE(&dummy);
                        }
                        break;
                        
                    case WAIT_FOR_PORT:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        obj->RcvPort     = byte;
                        obj->RcvState1   = WAIT_FOR_LENGTH_0;
                        break;

                    case WAIT_FOR_LENGTH_0:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        obj->RcvDataLength = byte << 8;
                        obj->RcvState1     = WAIT_FOR_LENGTH_1;
                        break;

                    case WAIT_FOR_LENGTH_1:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        obj->RcvDataLength |= byte;
                        if ((obj->RcvDataLength == 0) || (obj->RcvDataLength >= TTP_PAYLOAD_MAX_SIZE)) {
                            TTP_DEQUEUE(&dummy);
                            obj->RcvState1 = WAIT_FOR_HEAD;
                        } else {
                            obj->RcvState1 = WAIT_FOR_DATA;
                        }
                        break;

                    case WAIT_FOR_DATA:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        pBuffer[obj->RcvWritePoint] = byte;
                        obj->RcvWritePoint++;
                        obj->RcvDataLength--;
                        if (0 == obj->RcvDataLength) {
                            obj->RcvState1 = WAIT_FOR_CHECKSUM_0;
                        }
                        break;

                    case WAIT_FOR_CHECKSUM_0:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        obj->RcvState1 = WAIT_FOR_CHECKSUM_1;
                        break;

                    case WAIT_FOR_CHECKSUM_1:
                        TTP_CHECKSUM(byte, obj->RcvChecksum);
                        if (0 == obj->RcvChecksum) {
                            TTP_GET_ALL_PEEKED_QUEUE();
                            obj->RcvState0 = RCV_HANDLE;
                            bReturn = true;
                        } else {
                            TTP_DEQUEUE(&dummy);
                        }
                        obj->RcvState1 = WAIT_FOR_HEAD;
                        break;
                }
            } while (!bReturn);
        }
        //break;    //!< omitted intentionally.

        case RCV_HANDLE:
            *pPort      = obj->RcvPort;
            *pLength    = obj->RcvWritePoint;
            obj->RcvState0 = RCV_PASER;
            return FSM_RT_CPL;
    }

    return FSM_RT_ONGOING;
}

uint8_t ttp_snd_fsm(ttp_t *obj, uint8_t port, const uint8_t *pBuffer, uint16_t length)
{
    switch (obj->SndState) {
        case SND_HEAD:
            if (0 == length) {
                return FSM_RT_CPL;
            }
            if (TTP_OUTPUT_BYTE(TTP_HEAD_BYTE)) {
                obj->SndChecksum    = 0xFFFF;
                obj->SndWritePoint  = 0;
                obj->SndState       = SND_PORT;
            } else {
                break;
            }
            
        case SND_PORT:
            if (TTP_OUTPUT_BYTE(port)) {
                TTP_CHECKSUM(port, obj->SndChecksum);
                obj->SndState = SND_LENGTH_0;
            } else {
                break;
            }

        case SND_LENGTH_0:
            if (TTP_OUTPUT_BYTE(length >> 8)) {
                TTP_CHECKSUM((length >> 8), obj->SndChecksum);
                obj->SndState = SND_LENGTH_1;
            } else {
                break;
            }

        case SND_LENGTH_1:
            if (TTP_OUTPUT_BYTE(length)) {
                TTP_CHECKSUM(length, obj->SndChecksum);
                obj->SndState = SND_DATA;
            } else {
                break;
            }

        case SND_DATA:
            for (; obj->SndWritePoint < length; obj->SndWritePoint++) {
                if (!TTP_OUTPUT_BYTE(pBuffer[obj->SndWritePoint])) {
                    break;
                }
                TTP_CHECKSUM(pBuffer[obj->SndWritePoint], obj->SndChecksum);
            }
            if (obj->SndWritePoint >= length) {
                obj->SndState = SND_CHECKSUM_0;
            } else {
                break;
            }

        case SND_CHECKSUM_0:
            if (TTP_OUTPUT_BYTE(obj->SndChecksum >> 8)) {
                obj->SndState = SND_CHECKSUM_1;
            } else {
                break;
            }

        case SND_CHECKSUM_1:
            if (TTP_OUTPUT_BYTE(obj->SndChecksum)) {
                obj->SndState = SND_HEAD;
                return FSM_RT_CPL;
            }
            break;
    }
    
    return FSM_RT_ONGOING;
}

/* EOF */
