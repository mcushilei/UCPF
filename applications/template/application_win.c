/*******************************************************************************
 *  Copyright(C)2015-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "ring_buffer/ring_buffer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
THIS_FILE_NAME("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

static void my_debug( void *ctx, int level, const char *file, int line, const char *str )
{
	printf(" \r\n %s:%04d: %s", file, line, str );
}



void print_mem( const void* data, unsigned int length )
{
    const unsigned char* pmem = data;
    for( uint32_t i = 0; i < length; i++ ) {
        if( ( i & 0x0F ) == 0 ) {
            printf( "\r\n" );
        }
        printf( "%02X ", pmem[i] );
    }
    printf( "\r\n" );
}



/*! \note user's application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 */
int app_init( void )
{
    CHAR currentPath[MAX_PATH] = { 0 };
    CHAR myPath[MAX_PATH];

    if( GetCurrentDirectory( MAX_PATH, currentPath ) == 0 ) {
        DBG_LOG( "GetCurrentDirectory fail: %u", GetLastError() );
        return -1;
    }
    strcat_s( currentPath, MAX_PATH - strlen( currentPath ), "\\" );
    printf( "%s\r\n", currentPath );

#if 0
    //! file operation example
    file_t source, dest;
    if( !file_api_open( &source, "C:\\Users\\zheng\\Desktop\\111.mp3" ) ) {
        printf( "\r\n file_api_open() fail!" );
        return -1;
    }
    uint64_t fileSize = 0;
    file_api_get_size( source, &fileSize );
    char *buf = malloc( fileSize );
    uint32_t len = fileSize;
    if( !file_api_read( source, 0, buf, &len ) ) {
        printf( "\r\n file_api_read() fail!" );
        return -1;
    }
    file_api_close( source );


    char pathNew[200] = "C:\\Users\\zheng\\Desktop\\test_files\\";
    int pathNewLen = strlen( pathNew );
    CreateDirectory( pathNew, NULL );
    for( uint32_t i = 0; i < 1000; i++ ) {
        sprintf( pathNew + pathNewLen, "%04u.mp3", i );
        printf( "\r\n%s", pathNew );
        if( !file_api_open( &dest, pathNew ) ) {
            printf( "\r\n file_api_open() fail!" );
            return -1;
        }
        file_api_write( dest, 0, buf, fileSize );
        file_api_close( dest );
    }
#endif

    DBG_MSG( DEBUG_ON | DEBUG_TRACE | DEBUG_LEVEL_NORMAL, "this is a test of DEBUG_MSG()" );

    UCTEST_BEGIN("try test");
    UCTEST_TRUE( GetCurrentDirectory( MAX_PATH, currentPath ) == 0 );
    UCTEST_NOT_NULL( NULL );
    UCTEST_EQ_HEX( 1, hex_str2uint("Fe2"));
    UCTEST_EQ_UINT( 4294967295, hex_str2uint( "Fe2" ) );
    UCTEST_EQ_INT( -1, hex_str2uint( "Fe2" ) );
    UCTEST_EQ_BIN( 0xFFFFu, 0xF0, 0x0F );
    UCTEST_EQ_STRING( "hello", "world" );
    UCTEST_END( __TIMESTAMP__ );

    return 0;
}









/* EOF */
