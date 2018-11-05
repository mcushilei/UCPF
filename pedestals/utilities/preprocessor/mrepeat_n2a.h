/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __MREPEAT_N2A_H__
#define __MREPEAT_N2A_H__

#include ".\tpaste.h"


#define MREPEAT_N2A(cnt, macro, data)   TPASTE2(MREPEAT_N2A_, cnt)(macro, data)

#define MREPEAT_N2A_0(  macro, data)
#define MREPEAT_N2A_1(  macro, data)      MREPEAT_N2A_0(  macro, data)   macro( A, data)
#define MREPEAT_N2A_2(  macro, data)      MREPEAT_N2A_1(  macro, data)   macro( B, data)
#define MREPEAT_N2A_3(  macro, data)      MREPEAT_N2A_2(  macro, data)   macro( C, data)
#define MREPEAT_N2A_4(  macro, data)      MREPEAT_N2A_3(  macro, data)   macro( D, data)
#define MREPEAT_N2A_5(  macro, data)      MREPEAT_N2A_4(  macro, data)   macro( E, data)
#define MREPEAT_N2A_6(  macro, data)      MREPEAT_N2A_5(  macro, data)   macro( F, data)
#define MREPEAT_N2A_7(  macro, data)      MREPEAT_N2A_6(  macro, data)   macro( G, data)
#define MREPEAT_N2A_8(  macro, data)      MREPEAT_N2A_7(  macro, data)   macro( H, data)
#define MREPEAT_N2A_9(  macro, data)      MREPEAT_N2A_8(  macro, data)   macro( I, data)
#define MREPEAT_N2A_10( macro, data)      MREPEAT_N2A_9(  macro, data)   macro( J, data)
#define MREPEAT_N2A_11( macro, data)      MREPEAT_N2A_10( macro, data)   macro( K, data)
#define MREPEAT_N2A_12( macro, data)      MREPEAT_N2A_11( macro, data)   macro( L, data)
#define MREPEAT_N2A_13( macro, data)      MREPEAT_N2A_12( macro, data)   macro( M, data)
#define MREPEAT_N2A_14( macro, data)      MREPEAT_N2A_13( macro, data)   macro( N, data)
#define MREPEAT_N2A_15( macro, data)      MREPEAT_N2A_14( macro, data)   macro( O, data)
#define MREPEAT_N2A_16( macro, data)      MREPEAT_N2A_15( macro, data)   macro( P, data)
#define MREPEAT_N2A_17( macro, data)      MREPEAT_N2A_16( macro, data)   macro( Q, data)
#define MREPEAT_N2A_18( macro, data)      MREPEAT_N2A_17( macro, data)   macro( R, data)
#define MREPEAT_N2A_19( macro, data)      MREPEAT_N2A_18( macro, data)   macro( S, data)
#define MREPEAT_N2A_20( macro, data)      MREPEAT_N2A_19( macro, data)   macro( T, data)
#define MREPEAT_N2A_21( macro, data)      MREPEAT_N2A_20( macro, data)   macro( U, data)
#define MREPEAT_N2A_22( macro, data)      MREPEAT_N2A_21( macro, data)   macro( V, data)
#define MREPEAT_N2A_23( macro, data)      MREPEAT_N2A_22( macro, data)   macro( W, data)
#define MREPEAT_N2A_24( macro, data)      MREPEAT_N2A_23( macro, data)   macro( X, data)
#define MREPEAT_N2A_25( macro, data)      MREPEAT_N2A_24( macro, data)   macro( Y, data)
#define MREPEAT_N2A_26( macro, data)      MREPEAT_N2A_25( macro, data)   macro( Z, data)


#endif  //!< __MREPEAT_N2A_H__
/* EOF */
