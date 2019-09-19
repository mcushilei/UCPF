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

#ifndef __PARSER_CURLY_BRACKET_H__
#define __PARSER_CURLY_BRACKET_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
#define CURLY_BRACKET_PARSER_MAX_LENGTH         (256u)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef void curly_bracket_paser_callback_t(char *buf, uint32_t len);

typedef struct {
    curly_bracket_paser_callback_t  *Callback;
    char        RcvQueueBuffer[CURLY_BRACKET_PARSER_MAX_LENGTH];
    char        RcvFrameBuffer[CURLY_BRACKET_PARSER_MAX_LENGTH];
    queue_t     RcvQueue;
    uint32_t    RcvWriteIndex;
    uint8_t     State;
    uint8_t     Layer;
    uint8_t     MaxLayer;
} curly_bracket_paser_t;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool curly_bracket_paser_init(curly_bracket_paser_t *pasrer, uint32_t maxLayer, curly_bracket_paser_callback_t *callback);
extern bool curly_bracket_paser_deinit(curly_bracket_paser_t *pasrer);
extern void curly_bracket_paser(curly_bracket_paser_t *pasrer, char byte, bool timeout);

#endif  //!< #ifndef __PARSER_CURLY_BRACKET_H__
/* EOF */
