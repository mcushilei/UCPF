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

#ifndef __MREPEAT_ALPHABETICAL_H__
#define __MREPEAT_ALPHABETICAL_H__

#include ".\tpaste.h"


#define MREPEAT_ALPHABETICAL(end, macro, data)  TPASTE2(MREPEAT, end)(macro, data)

#define MREPEATA( macro, data)                               macro( A, data)
#define MREPEATB( macro, data)      MREPEATA( macro, data)   macro( B, data)
#define MREPEATC( macro, data)      MREPEATB( macro, data)   macro( C, data)
#define MREPEATD( macro, data)      MREPEATC( macro, data)   macro( D, data)
#define MREPEATE( macro, data)      MREPEATD( macro, data)   macro( E, data)
#define MREPEATF( macro, data)      MREPEATE( macro, data)   macro( F, data)
#define MREPEATG( macro, data)      MREPEATF( macro, data)   macro( G, data)
#define MREPEATH( macro, data)      MREPEATG( macro, data)   macro( H, data)
#define MREPEATI( macro, data)      MREPEATH( macro, data)   macro( I, data)
#define MREPEATJ( macro, data)      MREPEATI( macro, data)   macro( J, data)
#define MREPEATK( macro, data)      MREPEATJ( macro, data)   macro( K, data)
#define MREPEATL( macro, data)      MREPEATK( macro, data)   macro( L, data)
#define MREPEATM( macro, data)      MREPEATL( macro, data)   macro( M, data)
#define MREPEATN( macro, data)      MREPEATM( macro, data)   macro( N, data)
#define MREPEATO( macro, data)      MREPEATN( macro, data)   macro( O, data)
#define MREPEATP( macro, data)      MREPEATO( macro, data)   macro( P, data)
#define MREPEATQ( macro, data)      MREPEATP( macro, data)   macro( Q, data)
#define MREPEATR( macro, data)      MREPEATQ( macro, data)   macro( R, data)
#define MREPEATS( macro, data)      MREPEATR( macro, data)   macro( S, data)
#define MREPEATT( macro, data)      MREPEATS( macro, data)   macro( T, data)
#define MREPEATU( macro, data)      MREPEATT( macro, data)   macro( U, data)
#define MREPEATV( macro, data)      MREPEATU( macro, data)   macro( V, data)
#define MREPEATW( macro, data)      MREPEATV( macro, data)   macro( W, data)
#define MREPEATX( macro, data)      MREPEATW( macro, data)   macro( X, data)
#define MREPEATY( macro, data)      MREPEATX( macro, data)   macro( Y, data)
#define MREPEATZ( macro, data)      MREPEATY( macro, data)   macro( Z, data)


#endif  //!< __MREPEAT_ALPHABETICAL_H__
/* EOF */
