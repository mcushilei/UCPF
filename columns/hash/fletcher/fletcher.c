/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! \note do not move this pre-processor statement to other places
#define __CHECKSUM_FLETCHER_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
uint16_t fletcher16_checksum(const uint8_t *pchDate, uint32_t wLength)
{
    uint32_t a = 0, b = 0;
    uint32_t len;
 
    while (wLength) {
        len = (wLength >= 20u)? 20u : wLength;
        wLength -= len;
        do {
            a += *pchDate;
            b += a;
            pchDate++;
        } while (--len);
        /* First reduction step (partially)to reduce sums to 8 bits */
        a = (a & 0xffu) + (a >> 8);
        b = (b & 0xffu) + (b >> 8);
    }
    /* Second reduction step (completely)to reduce sums to 8 bits */
    a = (a & 0xffu) + (a >> 8);
    b = (b & 0xffu) + (b >> 8);
    return (b << 8) | a;
}

uint32_t fletcher32_checksum(const uint16_t *phwDate, uint32_t wLength)
{
    uint32_t a = 0, b = 0;
    uint32_t len;
 
    while (wLength) {
        len = (wLength >= 359u)? 359u : wLength;
        wLength -= len;
        do {
            a += *phwDate;
            b += a;
            phwDate++;
        } while (--len);
        /* First reduction step (partially)to reduce sums to 16 bits */
        a = (a & 0xffffu) + (a >> 16);
        b = (b & 0xffffu) + (b >> 16);
    }
    /* Second reduction step (completely)to reduce sums to 16 bits */
    a = (a & 0xffffu) + (a >> 16);
    b = (b & 0xffffu) + (b >> 16);
    return (b << 16) | a;
}
/* EOF */
