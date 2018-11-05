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
#define __SERVICE_HASH_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
uint16_t rfc1145_checksum(const void *pData, uint32_t wLength)
{
    uint32_t sum = 0;
    uint16_t *phw = NULL;
    uint32_t swapped = 0;
    union {
        uint8_t     c[2];
        uint16_t    s;
    } cs;

    if (0 == wLength) {
        return 0;
    }

    if (1u & (uint32_t)pData) {
        cs.s = 0;
        cs.c[1] = *(uint8_t *)pData;
        sum += cs.s;
        pData = (uint8_t *)pData + 1;
        wLength--;
        swapped = ~swapped;
    }
    phw = (uint16_t *)pData;

    for (; wLength & (~1u); wLength -= 2) {
        sum += *phw;
        phw++;
    }

    if (wLength) {
        cs.s = 0;
        cs.c[0] = *(uint8_t *)phw;
        sum += cs.s;
    }

    //! reduce to 16 bits twice.
    sum = (sum & 0xFFFFu) + (sum >> 16);/* First reduction step (partially) */
    sum = (sum & 0xFFFFu) + (sum >> 16);/* Second reduction step (completely) */
    
    if (swapped) {
        sum = ((sum << 8) & (0x0000FF00u)) | ((sum >> 8) & (0x000000FFu));
    }

    return ~sum;
}


/* EOF */
