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


#ifndef __REG_PIE_H__
#define __REG_PIE_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define PIE_REG                 (*(pie_reg_t *)PIE_BASE_ADDRESS)
#define XINT_REG                (*(xint_reg_t *)XINTRUPT_BASE_ADDRESS)

#define PIEACK_GROUP1   0x0001;
#define PIEACK_GROUP2   0x0002;
#define PIEACK_GROUP3   0x0004;
#define PIEACK_GROUP4   0x0008;
#define PIEACK_GROUP5   0x0010;
#define PIEACK_GROUP6   0x0020;
#define PIEACK_GROUP7   0x0040;
#define PIEACK_GROUP8   0x0080;
#define PIEACK_GROUP9   0x0100;
#define PIEACK_GROUP10  0x0200;
#define PIEACK_GROUP11  0x0400;
#define PIEACK_GROUP12  0x0800;

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
DEF_STRUCTURE(PIE_GROUP)
    DEF_REG16
        reg16_t INTx1:1;         // 0    INTx.1
        reg16_t INTx2:1;         // 1    INTx.2
        reg16_t INTx3:1;         // 2    INTx.3
        reg16_t INTx4:1;         // 3    INTx.4
        reg16_t INTx5:1;         // 4    INTx.5
        reg16_t INTx6:1;         // 5    INTx.6
        reg16_t INTx7:1;         // 6    INTx.7
        reg16_t INTx8:1;         // 7    INTx.8
        reg16_t      :8;         // 15:8 reserved
    END_DEF_REG16(PIEIER)

    DEF_REG16
        reg16_t INTx1:1;         // 0    INTx.1
        reg16_t INTx2:1;         // 1    INTx.2
        reg16_t INTx3:1;         // 2    INTx.3
        reg16_t INTx4:1;         // 3    INTx.4
        reg16_t INTx5:1;         // 4    INTx.5
        reg16_t INTx6:1;         // 5    INTx.6
        reg16_t INTx7:1;         // 6    INTx.7
        reg16_t INTx8:1;         // 7    INTx.8
        reg16_t      :8;         // 15:8 reserved
    END_DEF_REG16(PIEIFR)
END_DEF_STRUCTURE(PIE_GROUP)

typedef volatile struct {
    DEF_REG16
        reg16_t  ENPIE  :1;     // 0    Enable PIE block
        reg16_t  PIEVECT:15;    // 15:1 Fetched vector address
    END_DEF_REG16(PIECTRL)

    DEF_REG16
        reg16_t ACK1:1;         // 0    Acknowledge PIE interrupt group 1
        reg16_t ACK2:1;         // 1    Acknowledge PIE interrupt group 2
        reg16_t ACK3:1;         // 2    Acknowledge PIE interrupt group 3
        reg16_t ACK4:1;         // 3    Acknowledge PIE interrupt group 4
        reg16_t ACK5:1;         // 4    Acknowledge PIE interrupt group 5
        reg16_t ACK6:1;         // 5    Acknowledge PIE interrupt group 6
        reg16_t ACK7:1;         // 6    Acknowledge PIE interrupt group 7
        reg16_t ACK8:1;         // 7    Acknowledge PIE interrupt group 8
        reg16_t ACK9:1;         // 8    Acknowledge PIE interrupt group 9
        reg16_t ACK10:1;        // 9    Acknowledge PIE interrupt group 10
        reg16_t ACK11:1;        // 10   Acknowledge PIE interrupt group 11
        reg16_t ACK12:1;        // 11   Acknowledge PIE interrupt group 12
        reg16_t     :4;         // 15:12 reserved
    END_DEF_REG16(PIEACK)

    PIE_GROUP   GROUP[12];
} pie_reg_t;

typedef volatile struct {
    DEF_REG16
        reg16_t  ENABLE :1;     // 0    Enable interrupt
        reg16_t         :1;
        reg16_t  POLARITY:1;    // 2    Polarity select
        reg16_t         :13;
    END_DEF_REG16(XINT1CR)

    DEF_REG16
        reg16_t  ENABLE :1;     // 0    Enable interrupt
        reg16_t         :1;
        reg16_t  POLARITY:1;    // 2    Polarity select
        reg16_t         :13;
    END_DEF_REG16(XINT2CR)

    REG16_RSVD(5)      // reserved

    DEF_REG16
        reg16_t  ENABLE :1;     // 0    Enable interrupt
        reg16_t  SELECT :1;     // 1    Select the source for INT13
        reg16_t  POLARITY:1;    // 2    Polarity select
        reg16_t         :13;
    END_DEF_REG16(XNMIC)

    reg16_t XINT1CTR;
    reg16_t XINT2CTR;

    REG16_RSVD(5)      // reserved

    reg16_t XNMICTR;
} xint_reg_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
