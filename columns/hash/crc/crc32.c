/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief calculate CRC32
 *! \param hwPoly       CRC polynomial
 *! \param pchCRCValue  CRC initialize value
 *! \return CRC32 result
 */
uint32_t crc32_calculator(uint32_t wPoly, uint32_t wCRCValue, uint8_t chData)
{
    uint_fast8_t i;

    wCRCValue ^= (uint32_t)chData << 24;
    for (i = 8; i; i--) {
        if (wCRCValue & 0x80000000) {
            wCRCValue <<= 1;
            wCRCValue ^= wPoly;
        } else {
            wCRCValue <<= 1;
        }
    }

    return wCRCValue;
}

void crc32_table_generator(uint32_t wPoly, uint32_t *pwTable)
{
    uint_fast16_t i;

    for (i = 0; i < 256; i++) {
        pwTable[i] = crc32_calculator(wPoly, 0, i);
    }
}

/*! \brief calculate CRC32 checksum by looking up table
 *! \param wCRCValue    CRC initialize value
 *! \param chData       target byte
 *! \param pwTable      CRC table
 *! \return CRC32 result
 */
uint32_t crc32_check(const uint32_t *pwTable, uint32_t wCRCValue, uint8_t chData)
{
    wCRCValue = (wCRCValue << 8) ^ pwTable[(wCRCValue >> 24) ^ chData];
        
    return wCRCValue;
}

