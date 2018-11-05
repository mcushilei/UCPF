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


#ifndef __STRINGZ_H__
#define __STRINGZ_H__


/*! \brief Stringize.
 *
 * Stringize a token. Any tokens passed as argument whill be invoked as string.
 *
 * May be used only within macros with the tokens passed as arguments if the tokens are \#defined.
 *
 * For example:
 *      #defined PIN0           A0
 * writing STRINGZ(PIN0) is equivalent to writing "PIN0".
 * writing STRINGZ(1234) is equivalent to writing "1234".
*/
#define STRINGZ(x)          #x


/*! \brief Absolute stringize.
 *
 * Stringize a token.
 *
 * No restriction.
 *
 * For example:
 *      #defined PIN0           A0
 * writing ASTRINGZ(PIN0) is equivalent to writing "A0".
 */
#define ASTRINGZ(x)         STRINGZ(x)


#endif  //!< #ifndef __STRINGZ_H__
