/*******************************************************************************
 *  Copyright(C)2021 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __UPD_H__
#define __UPD_H__

/* universal package dispatcher */
/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/

enum {
    UPD_ERR_NONE = 0,
    UPD_ERR_FAIL = -1,
    UPD_ERR_NULL_POINTER = -2,
    UPD_ERR_TIMEOUT = -3,
    UPD_ERR_BUFFER_SIZE = -4,
    UPD_ERR_INIT_MUTEX = -5,
    UPD_ERR_PACKAGE_TYPE_UNMATCH = -7,
    UPD_ERR_WRITE_MSG_QUEUE = -8,
    UPD_ERR_INVALID_FIFO_CFG = -9,
    UPD_ERR_FIFO_IN = -10,
};

enum {
    UPD_PARSER_FLAG_RESET = 0x01,
    UPD_PARSER_FLAG_BYTE_TIMEOUT = 0x02,
};

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct upd_context_t upd_context_t;


typedef void upd_listener_callback_fn   ( void *userData, const char *data, uint16_t len );
typedef bool upd_matching_fn            ( const void *matchFor, const char *buf, uint16_t len );
typedef void upd_parser_callback_on_package_fn( upd_context_t *ctx, const char *buffer, int length );
typedef int  upd_parser_fn              ( void *parserCtx, const uint8_t *data, uint16_t len, uint8_t flags, upd_context_t *ctx, upd_parser_callback_on_package_fn *callback );
typedef void upd_callback_on_parser_error_fn( upd_context_t *ctx );

typedef struct {
    const void               *MatchFor;

    void                     *UserData;
    upd_listener_callback_fn *Callback;
} upd_static_listener_t;

struct upd_context_t {
    bool                IsRunning;
    void               *ParserContext;
    upd_parser_fn      *Parser;
    upd_callback_on_parser_error_fn *OnParserError;

    upd_matching_fn    *PackageMatch;

    fifo_t              FIFO;
    char               *IOBuffer;
    size_t              IOBufferSize;
       
	const upd_static_listener_t  *StaticListener;
	uint8_t                   StaticListenerNum;
    
    OS_HANDLE           MsgQueue;

    OS_HANDLE           ListenerListMux;
    list_node_t         ListenerList;
};


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
/*============================ IMPLEMENTATION ================================*/

extern int upd_init( upd_context_t *ctx,
    char *ioBuf, size_t ioBufSize,
    upd_parser_fn *fnParser, void *parserCtx,
    upd_callback_on_parser_error_fn *fnOnParserError,
    upd_matching_fn *fnMatch,
    void *fifoBuffer, size_t  fifoSize,
    const upd_static_listener_t *oob, uint8_t oobNum );
extern int upd_deinit( upd_context_t *ctx );

extern int upd_input( upd_context_t *ctx, const void *data, uint16_t len );

//! \note  this functin must NOT be called before calling upd_init() or after calling upd_deinit().
extern int upd_wait_for_package( upd_context_t *ctx,
    uint32_t timeoutMS,
    const void *matchFor,
    char *buf, uint16_t bufSize );

extern int upd_routine( upd_context_t *ctx );

extern int upd_online( upd_context_t *ctx );
extern int upd_offline( upd_context_t *ctx );


#endif  //!< #ifndef __UPD_H__
/* EOF */
