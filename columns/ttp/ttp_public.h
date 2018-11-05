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



#ifndef __TTP_PUBLIC_H__
#define __TTP_PUBLIC_H__



/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\queue\queue.h"

/*============================ MACROS ========================================*/
#define TTP_QUEUE                      queue_t

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef bool ttp_output_byte_t(uint8_t byte);
typedef bool ttp_poll_byte_t(uint8_t *pByte, bool *pIsTimeout);

typedef struct {
    ttp_output_byte_t   *OutputByte;
    ttp_poll_byte_t     *PollByte;

    uint8_t     RcvState0;
    uint8_t     RcvState1;
    uint16_t    RcvDataLength;
    uint16_t    RcvWritePoint;
    uint16_t    RcvChecksum;
    uint8_t     RcvPort;
    uint8_t     RcvQueueBuffer[TTP_PAYLOAD_MAX_SIZE + 6];
    TTP_QUEUE   RcvQueue;

    uint8_t     SndState;
    uint16_t    SndWritePoint;
    uint16_t    SndChecksum;
} ttp_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern bool ttp_ini(ttp_t               *obj,
                    ttp_output_byte_t   *pOutputByte,
                    ttp_poll_byte_t     *pPollByte);
extern uint8_t ttp_rcv_fsm( ttp_t       *obj,
                            uint8_t     *pPort,
                            uint8_t     *pBuffer,
                            uint16_t    *pLength);
extern uint8_t ttp_snd_fsm( ttp_t          *obj,
                            uint8_t         port,
                            const uint8_t  *pBuffer,
                            uint16_t        length);

#endif  //! #ifndef __TTP_PUBLIC_H__
/* EOF */
