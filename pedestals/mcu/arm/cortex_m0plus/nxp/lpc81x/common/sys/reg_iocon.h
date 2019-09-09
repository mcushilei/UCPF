/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_IOCON_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_REG_IOCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define IOCON_REG               (*(iocon_reg_t *)   IOCON_BASE_ADDRESS)
#define SWM_REG                 (*(swm_reg_t *)     SWM_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef volatile struct {
    union {
        reg32_t PIO0[31];
        struct {
            reg32_t PIO0_17;                    /*!< (@ 0x40044000) I/O configuration for pin PIO0_17 */
            reg32_t PIO0_13;                    /*!< (@ 0x40044004) I/O configuration for pin PIO0_13 */
            reg32_t PIO0_12;                    /*!< (@ 0x40044008) I/O configuration for pin PIO0_12 */
            reg32_t PIO0_5;                     /*!< (@ 0x4004400C) I/O configuration for pin PIO0_5 */
            reg32_t PIO0_4;                     /*!< (@ 0x40044010) I/O configuration for pin PIO0_4 */
            reg32_t PIO0_3;                     /*!< (@ 0x40044014) I/O configuration for pin PIO0_3 */
            reg32_t PIO0_2;                     /*!< (@ 0x40044018) I/O configuration for pin PIO0_2 */
            reg32_t PIO0_11;                    /*!< (@ 0x4004401C) I/O configuration for pin PIO0_11 */
            reg32_t PIO0_10;                    /*!< (@ 0x40044020) I/O configuration for pin PIO0_10 */
            reg32_t PIO0_16;                    /*!< (@ 0x40044024) I/O configuration for pin PIO0_16 */
            reg32_t PIO0_15;                    /*!< (@ 0x40044028) I/O configuration for pin PIO0_15 */
            reg32_t PIO0_1;                     /*!< (@ 0x4004402C) I/O configuration for pin PIO0_1 */
            REG32_RSVD(1)
            reg32_t PIO0_9;                     /*!< (@ 0x40044034) I/O configuration for pin PIO0_9 */
            reg32_t PIO0_8;                     /*!< (@ 0x40044038) I/O configuration for pin PIO0_8 */
            reg32_t PIO0_7;                     /*!< (@ 0x4004403C) I/O configuration for pin PIO0_7 */
            reg32_t PIO0_6;                     /*!< (@ 0x40044040) I/O configuration for pin PIO0_6 */
            reg32_t PIO0_0;                     /*!< (@ 0x40044044) I/O configuration for pin PIO0_0 */
            reg32_t PIO0_14;                    /*!< (@ 0x40044048) I/O configuration for pin PIO0_14 */
            REG32_RSVD(1)
            reg32_t PIO0_28;                    // 0x50
            reg32_t PIO0_27;                    // 0x54
            reg32_t PIO0_26;                    // 0x58
            reg32_t PIO0_25;                    // 0x5c
            reg32_t PIO0_24;                    // 0x60
            reg32_t PIO0_23;                    // 0x64
            reg32_t PIO0_22;                    // 0x68
            reg32_t PIO0_21;                    // 0x6c
            reg32_t PIO0_20;                    // 0x70
            reg32_t PIO0_19;                    // 0x74
            reg32_t PIO0_18;                    // 0x78
        };
    };
} iocon_reg_t;

typedef volatile struct {
	reg32_t  PINASSIGN[12];
    REG32_RSVD(100)
    reg32_t  PINENABLE0;
} swm_reg_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */