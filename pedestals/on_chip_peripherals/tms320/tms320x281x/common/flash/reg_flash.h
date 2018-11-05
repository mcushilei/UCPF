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


#ifndef __REG_SCI_H__
#define __REG_SCI_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PLASH_REG       (*(flash_reg_t *)   FLASH_BASE_ADDRESS)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/* CSM Register File */
typedef volatile struct {
    union {
        struct {
            reg16_t     KEY0;    // KEY reg bits 15-0
            reg16_t     KEY1;    // KEY reg bits 31-16
            reg16_t     KEY2;    // KEY reg bits 47-32
            reg16_t     KEY3;    // KEY reg bits 63-48
            reg16_t     KEY4;    // KEY reg bits 79-64
            reg16_t     KEY5;    // KEY reg bits 95-80
            reg16_t     KEY6;    // KEY reg bits 111-96
            reg16_t     KEY7;    // KEY reg bits 127-112
        };
        reg16_t         KEY[8];
    };

    REG16_RSVD(7)               // reserved

    // CSM Status & Control register
    DEF_REG16
        reg16_t     SECURE  :1;     // 0     Secure flag
        reg16_t             :14;    // 14-1  reserved
        reg16_t     FORCESEC:1;     // 15    Force Secure control bit
    END_DEF_REG16(CSMSCR)
} flash_reg_t;

/*============================ PROTOTYPES ====================================*/


#endif
