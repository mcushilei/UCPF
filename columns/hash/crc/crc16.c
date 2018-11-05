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
/*! \brief calculate CRC16
 *! \param hwPoly       CRC polynomial
 *! \param pchCRCValue  CRC init value
 *! \param chData       target byte
 *! \return CRC16 result
 */
uint16_t crc16_calculator(uint16_t hwPoly, uint16_t hwCRCValue, uint8_t chData)
{
    uint_fast8_t i;

    hwCRCValue ^= (uint16_t)chData << 8;
    for (i = 8; i; i--) {
        if (hwCRCValue & 0x8000) {
            hwCRCValue <<= 1;
            hwCRCValue ^= hwPoly;
        } else {
            hwCRCValue <<= 1;
        }
    }

    return hwCRCValue;
}

void crc16_table_generator(uint16_t hwPoly, uint16_t *phwTable)
{
    uint_fast16_t i;

    for (i = 0; i < 256; i++) {
        phwTable[i] = crc16_calculator(hwPoly, 0, i);
    }
}

/*! \brief calculate CRC16 checksum by looking up table
 *! \param phwCRCValue  CRC init value
 *! \param chData       target byte
 *! \param phwTable     CRC table
 *! \return CRC16 result
 */
uint16_t crc16_check(const uint16_t *phwTable, uint16_t hwCRCValue, uint8_t chData)
{
    hwCRCValue = (hwCRCValue << 8) ^ phwTable[(hwCRCValue >> 8) ^ chData];

    return hwCRCValue;
}

