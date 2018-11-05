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


#ifndef __CRC_H__
#define __CRC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define CRC7_POLY_TELECOM       (0x09)

#define CRC8_POLY_STAND         (0xD5)
#define CRC8_POLY_CCITT         (0x07)
#define CRC8_POLY_DALLAS_MAXIM  (0x31)
#define CRC8_POLY_SAE_J1850     (0x1D)
#define CRC8_POLY_WCDMA         (0x9B)

#define CRC16_POLY_ARINC        (0xA02B)
#define CRC16_POLY_CCITT        (0x1021)
#define CRC16_POLY_CDMA2000     (0xC867)
#define CRC16_POLY_DECT         (0x0589)
#define CRC16_POLY_T10_DIF      (0x8BB7)
#define CRC16_POLY_DNP          (0x3D65)
#define CRC16_POLY_IBM          (0x8005)

#define CRC32_POLY_IEEE802_3    (0x04C11DB7u)


/*============================ MACROFIED FUNCTIONS ===========================*/
#define CRC7(__CRCVAL, __NEWCHAR)       crc7_calculator(CRC7_POLY, (__CRCVAL), (__NEWCHAR))
#define CRC8(__CRCVAL, __NEWCHAR)       crc8_calculator(CRC8_POLY, (__CRCVAL), (__NEWCHAR))
#define CRC16(__CRCVAL, __NEWCHAR)      crc16_calculator(CRC16_POLY, (__CRCVAL), (__NEWCHAR))
#define CRC32(__CRCVAL, __NEWCHAR)      crc32_calculator(CRC32_POLY, (__CRCVAL), (__NEWCHAR))

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern uint8_t  crc7_calculator(uint8_t chPoly, uint8_t chCRCValue, uint8_t chData);
extern void     crc7_table_generator(uint8_t chPoly, uint8_t pchTable[256]);
extern uint8_t  crc7_check(const uint8_t *pchTable, uint8_t chCRCValue, uint8_t chData);
extern uint8_t  crc8_calculator(uint8_t chPoly, uint8_t chCRCValue, uint8_t chData);
extern void     crc8_table_generator(uint8_t chPoly, uint8_t pchTable[256]);
extern uint8_t  crc8_check(const uint8_t *pchTable, uint8_t chCRCValue, uint8_t chData);
extern uint16_t crc16_calculator(uint16_t hwPoly, uint16_t hwCRCValue, uint8_t chData);
extern void     crc16_table_generator(uint16_t hwPoly, uint16_t phwTable[256]);
extern uint16_t crc16_check(const uint16_t *phwTable, uint16_t hwCRCValue, uint8_t chData);
extern uint32_t crc32_calculator(uint32_t wPoly, uint32_t wCRCValue, uint8_t chData);
extern void     crc32_table_generator(uint32_t wPoly, uint32_t pwTable[256]);
extern uint32_t crc32_check(const uint32_t *pwTable, uint32_t wCRCValue, uint8_t chData);

#endif
