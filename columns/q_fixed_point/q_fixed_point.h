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

#ifndef __Q_FIXED_POINT_H__
#define __Q_FIXED_POINT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
/*============================ IMPLEMENTATION ================================*/

//! \group converter
//! \{
static inline int16_t i16_2_q( int16_t v, int p )
{
    if( p > 15 ) {
        p = 15;
    }

    return v << p;
}

static inline float u16q_2_float( uint16_t v, int p )
{
    if( p > 16 ) {
        p = 16;
    }

    return (float)v / ((uint32_t)1 << p);
}

static inline float i16q_2_float( int16_t v, int p )
{
    if( p > 15 ) {
        p = 15;
    }

    return (float)v / (1u << p);
}

static inline int16_t float_2_i16q( float v, int p )
{
    if( p > 15 ) {
        p = 15;
    }

    return (int16_t)(v * (1u << p));
}

static inline double u32q_2_double( uint32_t v, int p )
{
    if( p > 32 ) {
        p = 32;
    }

    return (double)v / ((uint64_t)1 << p);
}

static inline int32_t i32_2_q( int32_t v, int p )
{
    if( p > 31 ) {
        p = 31;
    }

    return v << p;
}

static inline double i32q_2_double( int32_t v, int p )
{
    if( p > 31 ) {
        p = 31;
    }

    return (double)v / ((uint32_t)1 << p);
}

static inline int32_t double_2_i32q( double v, int p )
{
    if( p > 31 ) {
        p = 31;
    }

    return (int32_t)(v * ((uint32_t)1 << p));
}
//! \}

//! \group limit
//! \{
static inline float u16q_pre( int p )
{
    return u16q_2_float( 1, p );
}

static inline float u16q_max( int p )
{
    return u16q_2_float( 0xFFFF, p );
}

static inline float i16q_max( int p )
{
    return i16q_2_float( 0x7FFF, p );
}

static inline float i16q_min( int p )
{
    return i16q_2_float( 0x8000, p );
}

static inline double u32q_pre( int p )
{
    return u32q_2_double( 1, p );
}

static inline double u32q_max( int p )
{
    return u32q_2_double( 0xFFFFFFFF, p );
}

static inline double i32q_max( int p )
{
    return i32q_2_double( 0x7FFFFFFF, p );
}

static inline double i32q_min( int p )
{
    return i32q_2_double( 0x80000000, p );
}
//! \}

static inline int16_t u16q_mul( uint16_t a, uint16_t b, int p )
{
    uint32_t v = (uint32_t)a * (uint32_t)b;
    return v >> p;
}

static inline int16_t i16q_mul( int16_t a, int16_t b, int p )
{
    int32_t v = (int32_t)a * (int32_t)b;
    return v >> p;
}

#endif  //!< #ifndef __Q_FIXED_POINT_H__
/* EOF */
