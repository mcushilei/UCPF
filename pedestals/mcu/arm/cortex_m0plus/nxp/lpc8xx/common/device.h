/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_COMMON_DEVICE_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_COMMON_DEVICE_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"

/*============================ MACROS ========================================*/
#define WDT_BASE_ADDRESS                (0x40000000ul)
#define MRT_BASE_ADDRESS                (0x40004000ul)
#define SWM_BASE_ADDRESS                (0x4000C000ul)
#define PMU_BASE_ADDRESS                (0x40020000ul)
#define FLASHCRTL_BASE_ADDRESS          (0x40040000ul)
#define IOCON_BASE_ADDRESS              (0x40044000ul)
#define SYSCON_BASE_ADDRESS             (0x40048000ul)
#define USART0_BASE_ADDRESS             (0x40064000ul)
#define USART1_BASE_ADDRESS             (0x40068000ul)
#define USART2_BASE_ADDRESS             (0x4006C000ul)
#define GPIO_BASE_ADDRESS               (0xA0000000ul)
#define PININT_BASE_ADDRESS             (0xA0004000ul)
#define SPI0_BASE_ADDRESS               (0x40058000ul)
#define SPI1_BASE_ADDRESS               (0x4005C000ul)
#define ADC0_BASE_ADDRESS               (0x4001C000ul)
#define DMA0_BASE_ADDRESS               (0x50008000ul)
#define DMATRIGMUX_BASE_ADDRESS         (0x40028000ul)
#define INMUX_BASE_ADDRESS              (0x4002C000ul)

/*============================ MACROFIED FUNCTIONS ===========================*/

//! \brief these macros are used to generate the macros operating on register values in the reg_xxx.h files.
//! PRINT_REG_DEFINE_W    a field of a register is defined by width and shift.
//! PRINT_REG_DEFINE_B    a field of a register is defined by MSB and LSB.
//! part: is the peripheral name eg. ADC, UART etc.
//! reg: is the register name.
//! field: is the name of bit fild in a register.
//!
#define PRINT_REG_DEFINE_B(part, reg, field, msb, lsb)    \
do {    \
    char buf[200];\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_S", #part, #reg, #field);\
    printf("%-40s  (%u)", buf, lsb);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_M", #part, #reg, #field);\
    printf("%-40s  (0x%08Xu)", buf, ((1u << ((msb) - (lsb) + 1u)) - 1) << lsb);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_SET(v)", #part, #reg, #field);\
    printf("%-40s  (((uint32_t)(v) << %u) & 0x%08X)", buf, lsb, ((1u << ((msb) - (lsb) + 1u)) - 1) << lsb);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_GET(v)", #part, #reg, #field);\
    printf("%-40s  (((uint32_t)(v) & 0x%08X) >> %u)", buf, ((1u << ((msb) - (lsb) + 1u)) - 1) << lsb, lsb);\
} while (0)

#define PRINT_REG_DEFINE_W(part, reg, field, width, shift)    \
do {    \
    char buf[200];\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_S", #part, #reg, #field);\
    printf("%-40s  (%u)", buf, shift);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_M", #part, #reg, #field);\
    printf("%-40s  (0x%08Xu)", buf, ((1u << (width)) - 1) << shift);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_SET(v)", #part, #reg, #field);\
    printf("%-40s  (((uint32_t)(v) << %u) & 0x%08X)", buf, shift, ((1u << (width)) - 1) << shift);\
    snprintf(buf, sizeof(buf), "\r\n#define REG_%s_%s_%s_GET(v)", #part, #reg, #field);\
    printf("%-40s  (((uint32_t)(v) & 0x%08X) >> %u)", buf, ((1u << (width)) - 1) << shift, shift);\
} while (0)

#define PRINT_REG_RESERVED_BEGIN(part, reg, field)\
do {    \
    printf("\r\n\r\n#define REG_%s_%s_RESERVED (~(\\\r\n      REG_%s_%s_%s_M", #part, #reg, #part, #reg, #field);\
} while (0)

#define PRINT_REG_RESERVED(part, reg, field)\
do {    \
    printf("  \\\r\n    | REG_%s_%s_%s_M", #part, #reg, #field);\
} while (0)

#define PRINT_REG_RESERVED_END()\
do {    \
    printf(" ) \\\r\n)");\
} while (0)


#define PRINT_OFFSET_OF(type, mem) \
do {\
    printf("\r\n %s: %08X", #mem, (uint32_t)&(((type *)0)->mem));\
} while (0)


//! \brief      bitfield value operations.
//! REG_BFVS    convert the value of a bitfield to a uint32_t value.
//! REG_BFVG    return the value of a bitfield from a uint32_t value(eg. from a register)
//! REG_BFVM    move a value of a bitfield to a variable.
//! \param 
//! part: the peripheral name eg. ADC, UART etc.
//! reg: the register name.
//! field: the field name in a register.
//! value: a varialbe or a value
//!
#define REG_BFVS(part, reg, field, value)                               \
    ( ((uint32_t)(value) << REG_##part##_##reg##_##field##_S) &         \
      REG_##part##_##reg##_##field##_M )

#define REG_BFVG(part, reg, field, value)                               \
    ( ((uint32_t)(value) & REG_##part##_##reg##_##field##_M) >>         \
      REG_##part##_##reg##_##field##_S )

#define REG_BFVM(part, reg, field, var, value)      \
    ( var = (uint32_t)(var)                         \
       & (~REG_##part##_##reg##_##field##_M)        \
       | REG_BFVS(part, reg, field, value) )

#define REG_BFM(part, reg, field)                   \
    ( REG_##part##_##reg##_##field##_M )

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
