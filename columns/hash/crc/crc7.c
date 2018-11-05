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
/*! \brief CRC7
 *! \param hwPoly       CRC polynomial
 *! \param pchCRCValue  CRC init value
 *! \param chData       target byte
 *! \return CRC7 result, MSB 7 bits are valid.
 */
uint8_t crc7_calculator(uint8_t chPoly, uint8_t chCRCValue, uint8_t chData)
{
    uint_fast8_t i;

    chCRCValue ^= chData;
    for (i = 8; i; i--) {
        if (chCRCValue & 0x80) {
            chCRCValue <<= 1;
            chCRCValue ^= chPoly << 1;
        } else {
            chCRCValue <<= 1;
        }
    }

    return chCRCValue;
}

void crc7_table_generator(uint8_t chPoly, uint8_t *pchTable)
{
    uint_fast16_t i;

    for (i = 0; i < 256; i++) {
        pchTable[i] = crc7_calculator(chPoly, 0, i);
    }
}

/*! \brief crc7
 *! \param phwCRCValue  CRC Variable
 *! \param chData       target byte
 *! \param pchTable     CRC table
 *! \return CRC7 result
 */
uint8_t crc7_check(const uint8_t *pchTable, uint8_t chCRCValue, uint8_t chData)
{
    chCRCValue = pchTable[chCRCValue ^ chData];

    return chCRCValue;
}

