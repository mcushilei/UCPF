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



/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./upd.h"

/*============================ MACROS ========================================*/
enum {
    UPD_MSG_DATA_INPUT,
    UPD_MSG_TASK_QUIT,
    UPD_MSG_LINK_CONNECTED,
    UPD_MSG_LINK_DISCONNECTED,
};

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct {
    uint32_t    Type;
    uint32_t    P1;
    uint32_t    P2;
} upd_msg_t;

typedef struct {
	list_node_t                     ListNode;

    const void                     *MatchFor;

    void                           *UserData;
    upd_listener_callback_fn       *Callback;       //! for unsynchronized listener.

    OS_HANDLE                       Flag;           //! for synchronized listener. (if Callback == NULL)
    uint32_t                        ExpireTimeMS;

    char                           *Buffer;
    uint16_t                        BufferSize;
} upd_dynamic_listener_t;


/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
THIS_FILE_NAME("upd");


static int static_listener_matching( upd_context_t *ctx, const char *buf, uint16_t len )
{
    int rc = 0;
    const upd_static_listener_t *listener;

    for( uint32_t i = 0; i < ctx->StaticListenerNum; i++ ) {
        listener = &ctx->StaticListener[i];
        if( ctx->PackageMatch( listener->MatchFor, buf, len ) ) {
            DBG_LOG( "find matched static listener" );
            listener->Callback( listener->UserData, buf, len );
            rc++;
        }
    }

    //! the number of matched listeners.
    return rc;
}

static int dynamic_listener_matching( upd_context_t *ctx, const char *buf, uint16_t len )
{
    list_node_t *next, *node;
    upd_dynamic_listener_t *listener;

    OS_MUTEX_WAIT( ctx->ListenerListMux, OS_INFINITE );
    LIST_FOR_EACH_SAFE( node, next, &ctx->ListenerList )
    {
        listener = CONTAINER_OF( node, upd_dynamic_listener_t, ListNode );
        if( ctx->PackageMatch( listener->MatchFor, buf, len ) ) {
            DBG_LOG( "find matched dynamic listener" );
            list_remove( node );
            if( NULL != listener->Callback ) {
                listener->Callback( listener->UserData, buf, len );
            }
            if( NULL != listener->Buffer && listener->BufferSize >= len ) {
                memcpy( listener->Buffer, buf, len );
            }
            if( NULL != listener->Flag ) {
                OS_FLAG_SET( listener->Flag );
            }
        }
    }
    OS_MUTEX_RELEASE( ctx->ListenerListMux );

    return 0;
}

static void parser_callback_on_package_ok( upd_context_t *ctx, const char *buffer, int length )
{
    int rc = 0;
    rc = static_listener_matching( ctx, buffer, length );
    if( rc > 0 ) {
    } else {
        dynamic_listener_matching( ctx, buffer, length );
    }
}

static void send_task_msg( upd_context_t *ctx, uint32_t type )
{
    upd_msg_t msg = { 0 };

    msg.Type = type;
    if( OS_ERR_NONE != OS_QUEUE_WRITE( ctx->MsgQueue, &msg, 0 ) ) {
        DBG_LOG( "OS_QUEUE_WRITE() fail!" );
    }
}

int upd_input( upd_context_t *ctx, const void *data, uint16_t len )
{
    upd_msg_t msg;

    msg.Type = UPD_MSG_DATA_INPUT;
    msg.P1 = 0;
    msg.P2 = len;

    if( !fifo_burst_in( &ctx->FIFO, data, len ) ) {
        return UPD_ERR_FIFO_IN;
    }
    if( OS_QUEUE_WRITE( ctx->MsgQueue, &msg, 0 ) != OS_ERR_NONE ) {
        /*
         *  \note this is a fatal failure! it will cause the data in the FIFO to 
         *        be blocked. If this happens in the test version, the user whether increase the size
         *        of the message queue or adjust the speed of producer and consumer. If this happens
         *        in the release version, the app should stop using the UPD and restart the UPD by
         *        calling upd_deinit() and upd_init().
         */
        DBG_LOG( "OS_QUEUE_WRITE() fail" );
        return UPD_ERR_WRITE_MSG_QUEUE;
    }
    return UPD_ERR_NONE;
}

int upd_routine( upd_context_t *ctx )
{
    enum {
        LINK_NOT_OK = 0,
        LINK_OK,
    };
    int rc = 0;
    upd_msg_t msg;
    uint32_t recv_len;
    uint8_t status = LINK_NOT_OK;

    while( 1 ) {
        if( OS_ERR_NONE != OS_QUEUE_READ( ctx->MsgQueue, &msg, OS_INFINITE ) ) {
            continue;
        }

        if( msg.Type == UPD_MSG_TASK_QUIT ) {
            DBG_LOG("upd_task() quit.");
            break;
        }

        switch( status ) {
        default:
        case LINK_NOT_OK:

            if( msg.Type == UPD_MSG_LINK_CONNECTED ) {
                ctx->Parser( ctx->ParserContext, ctx->IOBuffer, 0, UPD_PARSER_FLAG_RESET, ctx, parser_callback_on_package_ok );
                status = LINK_OK;
                break;
            }

            break;

        case LINK_OK:

            if( msg.Type == UPD_MSG_LINK_DISCONNECTED ) {
                status = LINK_NOT_OK;
                break;
            }

            if( msg.Type != UPD_MSG_DATA_INPUT ) {
                DBG_LOG( "unknow msg: %u", msg.Type );
                break;
            }

            for( recv_len = msg.P2 <= ctx->IOBufferSize ? msg.P2 : ctx->IOBufferSize, msg.P2 -= recv_len;
                 recv_len;
                 recv_len = msg.P2 <= ctx->IOBufferSize ? msg.P2 : ctx->IOBufferSize, msg.P2 -= recv_len ) {
        
                rc = fifo_burst_out( &ctx->FIFO, ctx->IOBuffer, recv_len );
                if( rc <= 0 || rc != recv_len ) {
                    DBG_LOG( "FIFO out fail. len: %u", recv_len );
                    continue;
                }
        
                // parser input.
                if( 0 > ctx->Parser( ctx->ParserContext, ctx->IOBuffer, recv_len, 0, ctx, parser_callback_on_package_ok ) ) {
                    if( NULL != ctx->OnParserError ) {
                        ctx->OnParserError( ctx );
                    }
                    status = LINK_NOT_OK;
                    break;
                }
            }
            break;
        }
    }
    
    ctx->IsRunning = false;
    return 0;
}


/*
 *  \note  we just remove all lintener node from the list and left the cleaning up work to the listener's owner.
 */
static void listener_remove_all( upd_context_t *ctx )
{
    list_node_t *next, *node;

    LIST_FOR_EACH_SAFE( node, next, &ctx->ListenerList ) {
        list_remove( node );
    }
}

static int listener_init( upd_context_t *ctx, upd_dynamic_listener_t *p, uint32_t timeoutMS, const void *matchFor, char *buffer, uint16_t bufferSize )
{
    OS_ERR err;

    memset( p, 0, sizeof( upd_dynamic_listener_t ) );

    p->ExpireTimeMS = timeoutMS;
    p->MatchFor     = matchFor;
    p->Buffer       = buffer;
    p->BufferSize   = bufferSize;
    list_init( &p->ListNode );

    err = OS_FLAG_CREATE( p->Flag, false, false );
    if( OS_ERR_NONE != err ) {
        return UPD_ERR_FAIL;
    }

    return UPD_ERR_NONE;
}

static void listener_deinit( upd_context_t *ctx, upd_dynamic_listener_t *p )
{
    OS_FLAG_DELETE( p->Flag );
}

static int listener_wait_for( upd_context_t *ctx, upd_dynamic_listener_t *p )
{
    OS_ERR err;

    OS_MUTEX_WAIT( ctx->ListenerListMux, OS_INFINITE );
    list_insert( &p->ListNode, &ctx->ListenerList );
    OS_MUTEX_RELEASE( ctx->ListenerListMux );

    err = OS_FLAG_WAIT( p->Flag, p->ExpireTimeMS );

    OS_MUTEX_WAIT( ctx->ListenerListMux, OS_INFINITE );
    list_remove( &p->ListNode );
    OS_MUTEX_RELEASE( ctx->ListenerListMux );

    if( OS_ERR_TIMEOUT == err ) {
        return UPD_ERR_TIMEOUT;
    }
    else {
        return UPD_ERR_NONE;
    }
}

int upd_wait_for_package( upd_context_t *ctx, uint32_t timeoutMS, const void *matchFor, char *buf, uint16_t bufSize )
{
    upd_dynamic_listener_t listener;
    int rc = UPD_ERR_NONE;

    rc = listener_init( ctx, &listener, timeoutMS, matchFor, buf, bufSize );
    if( UPD_ERR_NONE != rc ) {
        return rc;
    }
    rc = listener_wait_for( ctx, &listener );
    listener_deinit( ctx, &listener );
    return rc;
}

static int upd_struct_init( upd_context_t *ctx,
    char *ioBuf, size_t ioBufSize,
    upd_parser_fn *fnParser, void *parserCtx,
    upd_callback_on_parser_error_fn *fnOnParserError,
    upd_matching_fn *fnMatch,
    void *fifoBuffer, size_t  fifoSize,
    const upd_static_listener_t *oob, uint8_t oobNum )
{
    OS_ERR err;
    int rc = UPD_ERR_NONE;
    
    memset( ctx, 0, sizeof( upd_context_t ) );

    if( !fifo_init( &ctx->FIFO, fifoBuffer, fifoSize ) ) {
        DBG_LOG( "fifo_init() fail!" );
        return UPD_ERR_INVALID_FIFO_CFG;
    }

    ctx->StaticListener = oob;
    ctx->StaticListenerNum = oobNum;
    ctx->Parser = fnParser;
    ctx->OnParserError = fnOnParserError;
    ctx->ParserContext = parserCtx;
    ctx->PackageMatch = fnMatch;
    ctx->IOBuffer = ioBuf;
    ctx->IOBufferSize = ioBufSize;
    
    list_init( &ctx->ListenerList );

    err = OS_QUEUE_CREATE( ctx->MsgQueue, 32, sizeof( upd_msg_t ) );
    if( err != OS_ERR_NONE ) {
        DBG_LOG( "OS_QUEUE_CREATE() failed!" );
        goto __upd_msg_queue_init_failed;
    }

    err = OS_MUTEX_CREATE( ctx->ListenerListMux );
    if (err != OS_ERR_NONE) {
        DBG_LOG( "OS_MUTEX_CREATE() failed!" );
        rc = UPD_ERR_INIT_MUTEX;
        goto __upd_cmd_mux_failed;
    }

    return UPD_ERR_NONE;

    OS_MUTEX_DELETE( ctx->ListenerListMux );
__upd_cmd_mux_failed:
    OS_QUEUE_DELETE( ctx->MsgQueue );
__upd_msg_queue_init_failed:
    return rc;
}

static int upd_struct_deinit( upd_context_t *ctx )
{
    OS_ERR err;

    OS_MUTEX_WAIT( ctx->ListenerListMux, OS_INFINITE );
    listener_remove_all( ctx );
    OS_MUTEX_RELEASE( ctx->ListenerListMux );
    
    err = OS_MUTEX_DELETE( ctx->ListenerListMux );
    if (err != OS_ERR_NONE) {
        DBG_LOG("delete ListenerListMux failed!");
    }

    err = OS_QUEUE_DELETE( ctx->MsgQueue );
    if( err != OS_ERR_NONE ) {
        DBG_LOG( "delete MsgQueue failed!" );
    }

    return UPD_ERR_NONE;
}

int upd_init( upd_context_t *ctx,
    char *ioBuf, size_t ioBufSize,
    upd_parser_fn *fnParser, void *parserCtx,
    upd_callback_on_parser_error_fn *fnOnParserError,
    upd_matching_fn *fnMatch,
    void *fifoBuffer, size_t fifoSize,
    const upd_static_listener_t *oob, uint8_t oobNum )
{
    int rc;

    rc = upd_struct_init( ctx, ioBuf, ioBufSize, fnParser, parserCtx, fnOnParserError, fnMatch, fifoBuffer, fifoSize, oob, oobNum );
    if( UPD_ERR_NONE != rc ) {
        return rc;
    }

    ctx->Parser( ctx->ParserContext, ctx->IOBuffer, 0, UPD_PARSER_FLAG_RESET, ctx, parser_callback_on_package_ok );

    ctx->IsRunning = true;
    
    return UPD_ERR_NONE;
}

int upd_deinit( upd_context_t *ctx )
{
    int cnt = 0;
    const int max_try_num = 10;

    //! send a msg to stop the routine.
    while( ctx->IsRunning && cnt < max_try_num ) {
        send_task_msg( ctx, UPD_MSG_TASK_QUIT );
        OS_TASK_SLEEP( 1000 );
        cnt++;
    }

    if( ctx->IsRunning ) {
        DBG_LOG( "delete routine failed!" );
    }

    if( UPD_ERR_NONE != upd_struct_deinit( ctx ) ) {
        DBG_LOG( "upd_struct_deinit() failed!" );
    }

    return UPD_ERR_NONE;
}

int upd_online( upd_context_t *ctx )
{
    send_task_msg( ctx, UPD_MSG_LINK_CONNECTED );
    return UPD_ERR_NONE;
}

int upd_offline( upd_context_t *ctx )
{
    send_task_msg( ctx, UPD_MSG_LINK_DISCONNECTED );
    return UPD_ERR_NONE;
}

/* EOF */
