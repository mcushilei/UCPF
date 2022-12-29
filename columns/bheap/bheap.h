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

#ifndef __BHEAP_H__
#define __BHEAP_H__

/*
 * \brief this is an implementation of a binary heap optimised for little memory
 * devices. 
 * reference:
 *  [The Algorithm Design Manual] Skiena
 *  http://en.wikipedia.org/wiki/Binary_heap
 * 
 * to get a min-heap, compare() should return ture if a <(=) b.
 * to get a max-heap, compare() should return ture if a >(=) b.
 */
/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

struct bheap {
    unsigned int size;
    unsigned int tail;  //! \NOTE in empty-increase/decrease manner
};

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
/*============================ IMPLEMENTATION ================================*/

static inline unsigned int _bheap_parent( unsigned int n )
{
    return (n - 1) >> 1;  //! (n - 1) / 2u;
}

static inline unsigned int _bheap_left_child( unsigned int n )
{
    return (n << 1) + 1u;   //! n * 2u + 1u;
}

static inline unsigned int _bheap_right_child( unsigned int n )
{
    return (n << 1) + 2u;   //! n * 2u + 2u;
}

static inline void bheap_bubble_up( struct bheap *obj,
    unsigned int n,
    bool (*compare)(struct bheap *obj, int a, int b),
    void (*swap)(struct bheap *obj, int a, int b) )
{
    unsigned int p;
    while( n ) {
        p = _bheap_parent( n );
        if( compare( obj, p, n ) ) {   //! the parent dominate the current one.
            return;
        }
        swap( obj, p, n );
        n = p;
    }
}

static inline void bheap_bubble_down( struct bheap *obj,
    unsigned int n,
    bool (*compare)(struct bheap *obj, int a, int b),
    void (*swap)(struct bheap *obj, int a, int b) )
{
    unsigned int l, r, d;
    while( 1 ) {
        r = _bheap_right_child( n );
        l = _bheap_left_child( n );
        if( l >= obj->tail ) {
            return;
        }

        //! find the dominated one between l and r.
        d = l;
        if( r >= obj->tail ) {  //! there is no right child.
            //! nothing to do.
        } else if( !compare( obj, l, r ) ) {    //! l not dominate r.
            d = r;
        }

        if( !compare( obj, n, d ) ) {   //! the current one NOT dominate children.
            swap( obj, n, d );
            n = d;
            continue;
        }
        return;
    }
}

static inline int bheap_push( struct bheap *obj,
    void (*set)(struct bheap *obj, int n),
    bool (*compare)(struct bheap *obj, int a, int b),
    void (*swap)(struct bheap *obj, int a, int b) )
{
    if( obj->tail >= obj->size ) {
        return 0;
    }
    set( obj, obj->tail );
    bheap_bubble_up( obj, obj->tail, compare, swap );
    obj->tail++;

    return obj->tail;
}

static inline int bheap_pop( struct bheap *obj,
    void (*get)(struct bheap *obj, int n),
    bool (*compare)(struct bheap *obj, int a, int b),
    void (*swap)(struct bheap *obj, int a, int b) )
{
    if( 0u == obj->tail ) {
        return 0;
    }
    get( obj, 0 );
    obj->tail--;
    swap( obj, 0, obj->tail );
    bheap_bubble_down( obj, 0, compare, swap );
    return 1;
}

//! \brief to find the top k largest(smallest) numbers by applying a min(max)-heap.
//! \NOTE make sure the length of data buffer should be heap.size + 1. because
//! the last one of the buffer is used to store the inserted one.
static inline void bheap_sort( struct bheap *obj,
    void (*set)(struct bheap *obj, int n),
    bool (*compare)(struct bheap *obj, int a, int b),
    void (*swap)(struct bheap *obj, int a, int b) )
{
    if( bheap_push( obj, set, compare, swap ) ) {
        return;
    }
    //! \NOTE this is a full tree from here.
    set( obj, obj->size );  //! insert the new one at index obj->size.

    if( !compare( obj, obj->size, 0 ) ) {    //! the new one dose NOT dominate the most dominated one in the heap.
        swap( obj, obj->size, 0 );
        bheap_bubble_down( obj, 0, compare, swap );
    }
}

static inline int bheap_init( struct bheap *obj, unsigned int size )
{
    obj->tail = 0;
    obj->size = size;
    return size;
}


#endif  //!< #ifndef __BHEAP_H__
/* EOF */
