/***************************************************************************
 *   Copyright(C)2009-2014 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __TPASTE_H__
#define __TPASTE_H__


/*! \name Token Paste
 *
 * Paste N preprocessing tokens together.
 *
 * May be used only within macros with the tokens passed as arguments if the tokens are \#defined.
 *
 * For example:
 *      #define WIDTH           32
 *      #define UTYPE(x)        TPASTE2(U, x)
 * writing UTYPE(WIDTH) is equivalent to writing U32.
 */
//! @{
#define TPASTE2( a, b)                            a##b
#define TPASTE3( a, b, c)                         a##b##c
#define TPASTE4( a, b, c, d)                      a##b##c##d
#define TPASTE5( a, b, c, d, e)                   a##b##c##d##e
#define TPASTE6( a, b, c, d, e, f)                a##b##c##d##e##f
#define TPASTE7( a, b, c, d, e, f, g)             a##b##c##d##e##f##g
#define TPASTE8( a, b, c, d, e, f, g, h)          a##b##c##d##e##f##g##h
#define TPASTE9( a, b, c, d, e, f, g, h, i)       a##b##c##d##e##f##g##h##i
#define TPASTE10(a, b, c, d, e, f, g, h, i, j)    a##b##c##d##e##f##g##h##i##j
//! @}


/*! \name Absolute Token Paste
 *
 * Paste N preprocessing tokens together.
 *
 * No restriction.
 *
 * For example:
 *      #define WIDTH       32
 * writing ATPASTE2(U, WIDTH) is equivalent to writing U32.
 */
//! @{
#define ATPASTE2( a, b)                           TPASTE2( a, b)
#define ATPASTE3( a, b, c)                        TPASTE3( a, b, c)
#define ATPASTE4( a, b, c, d)                     TPASTE4( a, b, c, d)
#define ATPASTE5( a, b, c, d, e)                  TPASTE5( a, b, c, d, e)
#define ATPASTE6( a, b, c, d, e, f)               TPASTE6( a, b, c, d, e, f)
#define ATPASTE7( a, b, c, d, e, f, g)            TPASTE7( a, b, c, d, e, f, g)
#define ATPASTE8( a, b, c, d, e, f, g, h)         TPASTE8( a, b, c, d, e, f, g, h)
#define ATPASTE9( a, b, c, d, e, f, g, h, i)      TPASTE9( a, b, c, d, e, f, g, h, i)
#define ATPASTE10(a, b, c, d, e, f, g, h, i, j)   TPASTE10(a, b, c, d, e, f, g, h, i, j)
//! @}


#endif  //!< #ifndef __TPASTE_H__
