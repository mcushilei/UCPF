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

extern void test_upd( void );


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


unsigned char test1[] = { 21, 8, 30, 22, 23, 24 };
//char test1[] = { 0,0,0,0,0,0 };
char test2[32];
char test3[32];

extern uint32_t base64_encode( uint8_t in[], uint32_t inLen, uint8_t out[] );

/*! \note user's application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 */
int app_init( void )
{

    uint32_t len;

    len = base64_encode( test1, 6, test2 );
    printf( "%s, %d \r\n", test2, len );


    //test_upd();


    return 0;
}









/* EOF */
