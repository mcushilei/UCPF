/*******************************************************************************
 *  Copyright(C)2022 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include "./bheap.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

typedef struct {
    struct bheap bheap;
    uint32_t *buf;
} my_bheap_t;

static my_bheap_t myBHeap;
static uint32_t myBuff[32 + 1];

bool my_compare( struct bheap *obj, int a, int b )
{
    return myBuff[a] <= myBuff[b];
}

void my_swap( struct bheap *obj, int a, int b )
{
    uint32_t t = myBuff[a];
    myBuff[a] = myBuff[b];
    myBuff[b] = t;
}

void my_set0( struct bheap *obj, int n )
{
    static uint32_t i = 0;
    static uint32_t data[] = {
        2,
        2,
        2,
        1,
        1,
        1,
        3,
        3,
        3,
        4,
        4,
        4,
        5,
        5,
        5,
    };
    myBuff[n] = data[i++];
}

void my_set1( struct bheap *obj, int n )
{
    myBuff[n] = rand();
}

void my_get( struct bheap *obj, int n )
{
    printf( "\r\n %05d", myBuff[n] );
}

void test_bheap( void )
{
    uint32_t seed;
    uint32_t n = 0x3FFF;

    myBHeap.buf = myBuff;

    bheap_init( &myBHeap.bheap, 4, 0 );
    seed = time( NULL );
    srand( seed );

    printf( "\r\n test bheap queue..." );
    while( bheap_push( &myBHeap.bheap, &my_set0, &my_compare, &my_swap ) );
    while( bheap_pop( &myBHeap.bheap, &my_get, &my_compare, &my_swap ) );

    printf( "\r\n test bheap topk..." );
    while( n-- ) {
        bheap_topk( &myBHeap.bheap, &my_set1, &my_compare, &my_swap );
    }
    while( bheap_pop( &myBHeap.bheap, &my_get, &my_compare, &my_swap ) );

    printf( "\r\n test bheap sort..." );
    srand( 0 );
    for( int i = 0; i < 10; i++ ) {
        myBuff[i] = rand();
        printf( "\r\n %05d", myBuff[i] );
    }
    bheap_init( &myBHeap.bheap, 20, 10 );
    bheap_sort( &myBHeap.bheap, &my_compare, &my_swap );
    printf( "\r\n after sort:" );
    for( int i = 0; i < 10; i++ ) {
        printf( "\r\n %05d", myBuff[i] );
    }

}

/* EOF */
