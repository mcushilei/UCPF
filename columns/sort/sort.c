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

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

void bubble_sort(int *pData, uint32_t length)
{
    uint32_t R0, R1, i;

    if (length < 2) {
        return;
    }

    for (R0 = length - 1; R0 != 0; R0--) {
        //! let R1 point to the largest one.
        for (R1 = 0; R1 < R0; R1++) {
            i = R1 + 1;
            if (pData[R1] > pData[i]) {
                //! swap R1 + 1 and R1.
                int t = pData[i];
                pData[i] = pData[R1];
                pData[R1] = t;
            }
        }
    }
}

void selection_sort(int *pData, uint32_t length)
{
    uint32_t R0, R1, i;

    if (length < 2u) {
        return;
    }

    for (R0 = 0; R0 < length; R0++) {
        //! let 'R1' point to the smallest one from 'R0' to the last.
        R1 = R0;
        for (i = R0; i < length; i++) {
            if (pData[i] < pData[R1]) {
                R1 = i;
            }
        }
        //! SWAP R0 and R1.
        int t = pData[R0];
        pData[R0] = pData[R1];
        pData[R1] = t;
    }
}

void insertion_sort(int *pData, uint32_t length)
{
    uint32_t R0, R1, i;

    for (R0 = 1; R0 < length; R0++) {
        //! let R1 to the position for R0.
        for (R1 = 0; R1 < R0; R1++) {
            if (pData[R1] > pData[R0]) {
                break;
            }
        }
        //! MOVE R0 to R1.
        int t = pData[R0];
        for (i = R0; i > R1; i--) {
            pData[i] = pData[i - 1];
        }
        pData[R1] = t;
    }
}

void shell_sort(int *pData, uint32_t length)
{
    for (uint32_t gap = length / 2u; gap != 0u; gap = gap / 2u) {

		for (uint32_t i = gap; i < length; ++i) {
		    int temp = pData[i];
            uint32_t j;
            for (j = i - gap; j >= gap && pData[j] > temp; j -= gap) {
			    pData[j + gap] = pData[j];
		    }
            if (j != i - gap) {
			    pData[j + gap] = temp;
            }
	    }

	}
}

void shell_sort_to_index(int *pData, uint8_t index[], uint8_t length)
{

    for (uint32_t i = 0; i < length; i++) {
        index[i] = i;
    }

    for (uint32_t gap = length / 2u; gap != 0u; gap = gap / 2u) {

		for (uint32_t i = gap; i < length; ++i) {
            uint8_t tempIndex = index[i];
		    int tempData = pData[tempIndex];

            uint32_t j;
            for (j = i - gap; j >= gap && pData[index[j]] > tempData; j -= gap) {
			    index[j + gap] = index[j];
		    }
            if (j != i - gap) {
			    index[j + gap] = tempIndex;
            }
	    }

	}
}


void heap_sort(
    int *dat,
    int n,
    void ( *swap )( int *, int * ),
    int  ( *cmp )( const int *, const int * ) )
{
    /* pre-scale counters for performance */
    int i;
    int c, r;

    for( i = n / 2 - 1; i >= 0; i-- ) {
        /* heapify */
        for( r = i; ( c = r * 2 + 1 ) < n; r = c ) {
            if( c + 1 < n && cmp( &dat[c], &dat[c + 1] ) < 0 )
                c += 1;
            if( cmp( &dat[r], &dat[c] ) >= 0 )
                break;
            swap( &dat[r], &dat[c] );
        }
    }

    /* sort */
    for( i = n - 1; i > 0; i-- ) {
        swap( &dat[0], &dat[i] );
        for( r = 0; ( c = r * 2 + 1 ) < i; r = c ) {
            if( c + 1 < i && cmp( &dat[c], &dat[c + 1] ) < 0 )
                c += 1;
            if( cmp( &dat[r], &dat[c] ) >= 0 )
                break;
            swap( &dat[r], &dat[c] );
        }
    }
}

/* EOF */
