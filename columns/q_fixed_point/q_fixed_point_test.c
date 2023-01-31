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
#include "./q_fixed_point.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void test_u16q_pre( void )
{
    if( 0.5f == u16q_pre( 1 ) ) {
        printf( "\r\n u16q_pre() ok" );
    } else {
        printf( "\r\n u16q_pre() error" );
    }
}

void test_i16_2_q( void )
{
    if( (3u << 1) == i16_2_q( 3, 1 ) ) {
        printf( "\r\n i16_2_q() ok" );
    } else {
        printf( "\r\n i16_2_q() error" );
    }
}

void test_float_2_i16q( void )
{
    if( 1 == float_2_i16q( 0.5f, 1 ) ) {
        printf( "\r\n float_2_i16q() ok" );
    } else {
        printf( "\r\n float_2_i16q() error" );
    }
}

void test_i16q_2_float( void )
{
    if( 0.5f == i16q_2_float( 1, 1 ) ) {
        printf( "\r\n i16q_2_float() ok" );
    } else {
        printf( "\r\n i16q_2_float() error" );
    }
}

void test_q_limit( void )
{
    for( int i = 0; i < 16; i++ ) {
        printf( "\r\n Q%02d max: %.18f", i, i16q_max( i ) );
    }

    for( int i = 0; i < 17; i++ ) {
        printf( "\r\n Q%02d pre: %.18f", i, u16q_pre( i ) );
    }

}

void test_u16q_mul( void )
{
    if( float_2_i16q( 0.25f, 2 ) == u16q_mul( float_2_i16q( 0.5f, 2 ), float_2_i16q( 0.5f, 2 ), 2 ) ) {
        printf( "\r\n u16q_mul() ok" );
    } else {
        printf( "\r\n u16q_mul() error" );
    }
}

void test_i16q_mul( void )
{
    if( float_2_i16q( -0.25f, 2 ) == i16q_mul( float_2_i16q( -0.5f, 2 ), float_2_i16q( 0.5f, 2 ), 2 ) ) {
        printf( "\r\n i16q_mul() -0.25f ok" );
    } else {
        printf( "\r\n i16q_mul() -0.25f error" );
    }
    if( float_2_i16q( 0.25f, 2 ) == i16q_mul( float_2_i16q( -0.5f, 2 ), float_2_i16q( 0.5f, 2 ), 2 ) ) {
        printf( "\r\n i16q_mul() 0.25f error" );
    } else {
        printf( "\r\n i16q_mul() 0.25f ok" );
    }
}

/* EOF */
