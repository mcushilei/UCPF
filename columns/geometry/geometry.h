/*******************************************************************************
 *  Copyright(C)2017-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    geonum_t    X;
    geonum_t    Y;
} dot2d_t;

typedef struct {
    dot2d_t     A;
    dot2d_t     B;
} segment2d_t;

typedef struct {
    dot2d_t     O;
    geonum_t    R;
} circle2d_t;

typedef struct {
    dot2d_t     LB;
    dot2d_t     RT;
} rectangle2d_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern geonum_t dot_distance(dot2d_t *A, dot2d_t *B);
extern void     get_set_rectangle(dot2d_t *pSet, uint32_t n, rectangle2d_t *pRectangle);
extern bool     is_line_horizontal(segment2d_t *l1);
extern bool     is_line_vertical(segment2d_t *l1);
extern bool     is_line_parallel(segment2d_t *l1, segment2d_t *l2);
extern int32_t  dot_position_of_line(dot2d_t *d, segment2d_t *l);
extern bool     is_dot_on_segment(dot2d_t *d, segment2d_t *l);
extern uint32_t is_segment_intersection(segment2d_t *l1, segment2d_t *l2);
extern bool     is_dot_in_polygon(dot2d_t *pDot, dot2d_t *pVertex, uint32_t n);

extern float    distance_on_earth(float longitude1, float latitude1, float longitude2, float latitude2);

#endif  //! #ifndef __GEOMETRY_H__
/* EOF */
