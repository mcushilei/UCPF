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
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
THIS_FILE_NAME( "test_upd_test" );

extern void print_mem( const void *data, unsigned int length );


static upd_context_t  updCtx;
static uint32_t readLen = 0;
static uint32_t writeLen = 0;



const uint8_t auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

const uint8_t auchCRCLo[256] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };


static uint16_t crc_check( uint8_t *puchMsg, uint16_t usDataLen )
{
    uint8_t uchCRCHi;                // high byte of CRC initialized
    uint8_t uchCRCLo;                // low byte of CRC initialized
    uint16_t uIndex;                 // will index into CRC lookup table

    uchCRCHi = 0xFF;
    uchCRCLo = 0xFF;

    while( usDataLen-- ) {
        uIndex = uchCRCHi ^ ( *puchMsg++ );
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    return ( uchCRCHi << 8 | uchCRCLo );
}


typedef struct {
    queue_t     RcvQueue;
    char        RcvQueueBuffer[2048];
    char        RcvFrameBuffer[2048];
    uint8_t     State;
    uint8_t     Counter;
    uint16_t    DataLength;
    uint16_t    RcvWriteIndex;
} test_upd_frame_parser_t;

test_upd_frame_parser_t       packageParser;



#define PARSERQ_INIT(__POBJ, __BUF, __BUF_SIZE)    \
    QUEUE_UINT8_INIT( &((__POBJ)->RcvQueue), __BUF, __BUF_SIZE, NULL )

#define PARSERQ_ENQUEUE(__BYTE)             QUEUE_ENQUEUE(&obj->RcvQueue, __BYTE)
#define PARSERQ_DEQUEUE(__PBYTE)            QUEUE_DEQUEUE(&obj->RcvQueue, __PBYTE)
#define PARSERQ_PEEK_QUEUE(__PBYTE)         QUEUE_PEEK(&obj->RcvQueue, __PBYTE)
#define PARSERQ_GET_ALL_PEEKED_QUEUE()      QUEUE_GET_ALL_PEEKED(&obj->RcvQueue)
#define PARSERQ_RESET_PEEK()           QUEUE_RESET_PEEK(&obj->RcvQueue)
#define PARSERQ_RESET()                     QUEUE_RESET(&obj->RcvQueue)


#define PARSER_ERR_RESET()       \
do {                            \
    PARSERQ_DEQUEUE(&dummy);        \
    obj->State = WAIT_FOR_HEAD_0;   \
} while (0)

static int parser_package( void *parser, const uint8_t *data, uint16_t len, uint8_t flags, upd_context_t *ctx, upd_parser_callback_on_package_fn *callback )
{

    enum {
        WAIT_FOR_HEAD_0 = 0,
        WAIT_FOR_HEAD_1,
        WAIT_FOR_LENGTH,
        WAIT_FOR_DATA,
        WAIT_FOR_CHECKSUM,
        WAIT_FOR_TAIL,
    };
    uint8_t byte;
    uint8_t dummy;
    hword_value_t hword = { 0 };
    test_upd_frame_parser_t *obj = (test_upd_frame_parser_t *)parser;

    if( flags & UPD_PARSER_FLAG_RESET ) {
        PARSERQ_RESET();
        obj->State = WAIT_FOR_HEAD_0;
    }

    if( flags & UPD_PARSER_FLAG_BYTE_TIMEOUT ) {
        PARSERQ_DEQUEUE( &dummy );
        obj->State = WAIT_FOR_HEAD_0;
    }

    for( uint32_t i = 0; i < len; i++ ) {
        byte = data[i];

        if( !PARSERQ_ENQUEUE( byte ) ) {
            PARSERQ_DEQUEUE( &dummy );
            PARSERQ_ENQUEUE( byte );
            obj->State = WAIT_FOR_HEAD_0;
        }

        while( PARSERQ_PEEK_QUEUE( &byte ) ) {
            switch( obj->State ) {
            case WAIT_FOR_HEAD_0:
                if( 0xA5 == byte ) {
                    obj->RcvWriteIndex = 0u;
                    obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                    obj->State = WAIT_FOR_HEAD_1;
                    obj->Counter = 0;
                } else {
                    PARSER_ERR_RESET();
                }
                break;

            case WAIT_FOR_HEAD_1:
                if( 0x5A == byte ) {
                    obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                    obj->State = WAIT_FOR_LENGTH;
                    obj->Counter = 0;
                } else {
                    PARSER_ERR_RESET();
                }
                break;

            case WAIT_FOR_LENGTH:
                obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                obj->Counter++;

                if( obj->Counter >= 2 ) {
                    hword.LSB.B0 = obj->RcvFrameBuffer[2];
                    hword.LSB.B1 = obj->RcvFrameBuffer[3];
                    if( hword.Value <= 1417u ) {
                        obj->DataLength = hword.Value + 17 + 1 + 1 + 1;
                        obj->State = WAIT_FOR_DATA;
                        obj->Counter = 0;
                    } else {
                        PARSER_ERR_RESET();
                    }
                }
                break;

            case WAIT_FOR_DATA:
                obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                obj->Counter++;
                if( obj->Counter >= obj->DataLength ) {
                    obj->State = WAIT_FOR_CHECKSUM;
                    obj->Counter = 0;
                }
                break;

            case WAIT_FOR_CHECKSUM:
                obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                obj->Counter++;
                if( obj->Counter >= 2 ) {
                    uint16_t crc;
                    hword.LSB.B0 = obj->RcvFrameBuffer[obj->RcvWriteIndex - 2];
                    hword.LSB.B1 = obj->RcvFrameBuffer[obj->RcvWriteIndex - 1];
                    crc = crc_check( &obj->RcvFrameBuffer[2], 2 + obj->DataLength );
                    if( crc == hword.Value ) {
                        obj->State = WAIT_FOR_TAIL;
                        obj->Counter = 0;
                    } else {
                        PARSER_ERR_RESET();
                    }
                }
                break;

            case WAIT_FOR_TAIL:
                obj->RcvFrameBuffer[obj->RcvWriteIndex++] = byte;
                if( 0x96 == byte ) {
                    DBG_LOG( "Client rcv: %u", obj->RcvWriteIndex );

                    PARSERQ_GET_ALL_PEEKED_QUEUE();
                    obj->State = WAIT_FOR_HEAD_0;

                    obj->DataLength += 7;

                    callback( ctx, obj->RcvFrameBuffer, obj->DataLength );

                } else {
                    PARSER_ERR_RESET();
                }
                break;
            }
        }
    }
    return 0;
}


static const char testData1[] = {
    0xA5, 0x5A,
    0x01, 0x00,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x03,
    0xA6,
    0x03,
    0x00,
    0x6E, 0x8B,
    0x96
};

static const char testData2[] = {
    0xA5, 0x5A,
    0x06, 0x00,
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x0A,
    0xC1,
    0x03,
    0xFF, 0x00, 0x32, 0x59, 0x79, 0x60,
    0x35, 0x70,
    0x96
};












static const char matchTestData1[2] = { 0x03, 0xA6 };
static const char matchTestData2[2] = { 0x0A, 0xC1 };


OS_TASK_ENTRY( test_listner_task )
{
    char buf[128];
    int len = 0;


    while( true ) {
        memset( buf, 0, sizeof( buf ) );

        if( UPD_ERR_NONE != upd_wait_for_package( &updCtx, 5000, matchTestData2, buf, sizeof( buf ) ) ) {
            DBG_LOG( "upd_wait_for_package() fail!" );
        } else {
            DBG_LOG( "upd_wait_for_package() OK" );
            print_mem( buf, sizeof( buf ) );
        }

        OS_TASK_SLEEP( 5000 );
    }
    return 0;
}


static void oob_callback( void *userData, const char *data, uint16_t len )
{
    DBG_LOG("oob_callback() ok.");
    print_mem( data, len );
}

static const upd_static_listener_t test_updOOB[] = {
    { &matchTestData1, NULL, &oob_callback },
};



static int fixed_header_set_length( char *buf, uint16_t len )
{
    hword_value_t hword;

    hword.Value = len;
    buf[2] = hword.LSB.B0;
    buf[3] = hword.LSB.B1;

    return hword.Value;
}

static int fixed_header_get_length( const char *buf )
{
    hword_value_t hword = { 0 };

    hword.LSB.B0 = buf[2];
    hword.LSB.B1 = buf[3];

    return hword.Value;
}

static void fixed_header_set_id( char *buf, const char *id )
{
    memcpy( &buf[4], id, 17 );
}

static void fixed_header_set_frametype( char *buf, uint8_t syn )
{
    buf[21] = syn;
}

static uint8_t fixed_header_get_frametype( const char *buf )
{
    return buf[21];
}

static void fixed_header_set_msgtype( char *buf, uint8_t syn )
{
    buf[22] = syn;
}

static uint8_t fixed_header_get_msgtype( const char *buf )
{
    return buf[22];
}

static void fixed_header_set_syn( char *buf, uint8_t syn )
{
    buf[23] = syn;
}

static uint8_t fixed_header_get_syn( const char *buf )
{
    return buf[23];
}

static bool test_upd_match( const void *matchFor, const char *buf, uint16_t len )
{
    uint8_t *data = (uint8_t *)matchFor;
    if( data[0] == fixed_header_get_frametype( buf ) && data[1] == fixed_header_get_msgtype( buf ) ) {
        return true;
    }
    return false;
}


OS_TASK_ENTRY( test_upd_task )
{
    upd_routine( &updCtx );
    return 0;
}


static char iobuf[2048];
static char InBuffer[2048];

void test_upd( void )
{
    bool isOK = true;
    OS_HANDLE taskHandle;


    //! a queue is used to parser packages.
    PARSERQ_INIT( &packageParser, packageParser.RcvQueueBuffer, sizeof( packageParser.RcvQueueBuffer ) );


    if( UPD_ERR_NONE != upd_init( &updCtx,
        iobuf, sizeof( iobuf ),
        parser_package, &packageParser,
        NULL,
        test_upd_match,
        InBuffer, 2048,
        test_updOOB, UBOUND( test_updOOB ) ) ) {
        DBG_LOG( "upd_init() fail!" );
        isOK = false;
        goto exit;
    }

    upd_online( &updCtx );

    if( OS_ERR_NONE != OS_TASK_CREATE( taskHandle, "test_upd_task", test_upd_task, NULL, 512 + 512, 2, 0 ) ) {
        DBG_LOG( "OS_TASK_CREATE() fail!" );
        isOK = false;
    }


    if( OS_ERR_NONE != OS_TASK_CREATE( taskHandle, "test_listner_task", test_listner_task, NULL, 512 + 128, 2, 0 ) ) {
        DBG_LOG( "OS_TASK_CREATE() fail!" );
        isOK = false;
    }

    while( true ) {
        OS_TASK_SLEEP( 3000 );

        if( UPD_ERR_NONE != upd_input( &updCtx, testData1, sizeof( testData1 ) ) ) {
            DBG_LOG( "upd_input() fail!" );
        }

        if( UPD_ERR_NONE != upd_input( &updCtx, testData2, sizeof( testData2 ) ) ) {
            DBG_LOG( "upd_input() fail!" );
        }

    }

    OS_TASK_SLEEP( 3000 );

    //if( UPD_ERR_NONE != upd_deinit( &updCtx ) ) {
    //    DBG_LOG( "upd_deinit() fail!" );
    //    isOK = false;
    //}

exit:

    DBG_LOG( "readLen = %u", readLen );
    DBG_LOG( "writeLen = %u", writeLen );

    if( isOK ) {
        printf( "\r\n test ok." );
    } else {
        printf( "\r\n test fail." );
    }

}

/* EOF */
