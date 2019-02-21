/*******************************************************************************
*  Copyright(C)2015-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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

#ifndef __USE_BITS_H__
#define __USE_BITS_H__

//! \brief types and macros for bit operations
//! \note  some bit operations may be supported by compiler or hardware.

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \brief bit mask operations
#define BIT(__N)                (1u << (__N))
#define __BITMASK(__WIDTH)      (BIT(__WIDTH) - 1u)
#define BITMASK(__MSB, __LSB)   ( (BIT(__MSB) | __BITMASK(__MSB)) ^ __BITMASK(__LSB) )

#define BIT64(__N)              ((uint64_t)1 << (__N))
#define __BITMASK64(__WIDTH)    (BIT64(__WIDTH) - 1u)
#define BITMASK64(__MSB, __LSB) ( (BIT64(__MSB) | __BITMASK64(__MSB)) ^ __BITMASK64(__LSB) )


//! \brief byte order operations
#define BYTE_ORDER_SWITCH_16(__UINT16) do {\
    __UINT16 = (((__UINT16) >> 8) & 0x00FF) | (((__UINT16) << 8) & 0xFF00);\
} while (0)

#define BYTE_ORDER_SWITCH_32(__UINT32) do {\
    __UINT32 = (((__UINT32) >>  8) & 0x00FF00FF) | (((__UINT32) <<  8) & 0xFF00FF00);\
    __UINT32 = (((__UINT32) >> 16) & 0x0000FFFF) | (((__UINT32) << 16) & 0xFFFF0000);\
} while (0)

#if defined(__BIG_ENDIAN__)
#define BYTE_ORDER_TO_MSB_32(__UINT32) 
#else
#define BYTE_ORDER_TO_MSB_32(__UINT32)  BYTE_ORDER_SWITCH_32(__UINT32)
#endif

#if defined(__BIG_ENDIAN__)
#define BYTE_ORDER_TO_LSB_32(__UINT32)  BYTE_ORDER_SWITCH_32(__UINT32)
#else
#define BYTE_ORDER_TO_LSB_32(__UINT32)
#endif


#if defined(__BIG_ENDIAN__)
#define NTHHW(__V)
#define HTNHW(__V)
#define NTHW(__V)
#define HTNW(__V)
#else
#define NTHHW(__V) \
do {\
    uint16_t __UINT16 = __V;\
    __V = (((__UINT16) >> 8) & 0x00FF) | (((__UINT16) << 8) & 0xFF00);\
} while (0)

#define HTNHW(__V) NTHHW(__V)

#define NTHW(__V) \
do {\
    uint32_t __UINT32 = __V;\
    __UINT32 = (((__UINT32) >>  8) & 0x00FF00FF) | (((__UINT32) <<  8) & 0xFF00FF00);\
    __V      = (((__UINT32) >> 16) & 0x0000FFFF) | (((__UINT32) << 16) & 0xFFFF0000);\
} while (0)

#define HTNW(__V) NTHW(__V)
#endif


/*============================ TYPES =========================================*/
typedef union {
    uint16_t    Value;

    uint8_t     Bytes[2];
    //! Byte0 is always the LSB.
    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t Byte1;
        uint8_t Byte0;
#else
        uint8_t Byte0;
        uint8_t Byte1;
#endif
    };

    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t B1;
        uint8_t B0;
#else
        uint8_t B0;
        uint8_t B1;
#endif
    } B0_LSB;

    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t B0;
        uint8_t B1;
#else
        uint8_t B1;
        uint8_t B0;
#endif
    } B0_MSB;
} hword_t;



typedef union {
    uint32_t    Value;

    uint16_t    HWords[2];
    struct {
#if defined(__BIG_ENDIAN__)
        uint16_t HWord1;
        uint16_t HWord0;
#else
        uint16_t HWord0;
        uint16_t HWord1;
#endif
    };

    uint8_t     Bytes[4];
    //! Byte0 is always the LSB.
    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t Byte3;
        uint8_t Byte2;
        uint8_t Byte1;
        uint8_t Byte0;
#else
        uint8_t Byte0;
        uint8_t Byte1;
        uint8_t Byte2;
        uint8_t Byte3;
#endif
    };

    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t B3;
        uint8_t B2;
        uint8_t B1;
        uint8_t B0;
#else
        uint8_t B0;
        uint8_t B1;
        uint8_t B2;
        uint8_t B3;
#endif
    } B0_LSB;

    struct {
#if defined(__BIG_ENDIAN__)
        uint8_t B0;
        uint8_t B1;
        uint8_t B2;
        uint8_t B3;
#else
        uint8_t B3;
        uint8_t B2;
        uint8_t B1;
        uint8_t B0;
#endif
    } B0_MSB;
} word_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
