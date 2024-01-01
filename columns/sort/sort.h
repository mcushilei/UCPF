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

#ifndef __SORT_H__
#define __SORT_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define SELECTION_SORT(__TYPE, __DATA, __LENGTH)    \
do {                                                \
    int R0, R1;                                     \
                                                    \
    if( ( __LENGTH ) < 2u ) {                       \
        break;                                      \
    }                                               \
                                                    \
    for( R0 = 0; R0 < ( __LENGTH ); R0++ ) {        \
        R1 = R0;                                    \
        for( int i = R0; i < ( __LENGTH ); i++ ) {  \
            if( ( __DATA )[i] < ( __DATA )[R1] ) {  \
                R1 = i;                             \
            }                                       \
        }                                           \
                                        \
        __TYPE t = ( __DATA )[R0];      \
        ( __DATA )[R0] = ( __DATA )[R1];\
        ( __DATA )[R1] = t;             \
    }                                   \
} while (0)


#define SHELL_SORT(__TYPE, __DATA, __LENGTH)            \
do {                                                    \
    for (int gap = (__LENGTH) / 2u; gap != 0u; gap = gap / 2u) {\
		for (int i = gap; i < (__LENGTH); ++i) {        \
		    __TYPE t = (__DATA)[i];                     \
            int j;                                      \
            for (j = i - gap; j >= gap && (__DATA)[j] > t; j -= gap) {\
			    (__DATA)[j + gap] = (__DATA)[j];        \
		    }                                           \
            if (j != (i - gap)) {                       \
			    (__DATA)[j + gap] = t;                  \
            }                                           \
	    }                                               \
	}                                                   \
} while (0)

/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC INTERFACES =============================*/

#endif  //!< #ifndef __SORT_H__
/* EOF */
