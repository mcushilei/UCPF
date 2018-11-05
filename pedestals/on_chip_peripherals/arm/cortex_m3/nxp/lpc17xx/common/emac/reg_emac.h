/*******************************************************************************
 *  Copyright(C)2017 by Dreistein<mcu_shilei@hotmail.com>                     *
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
#ifndef __DRIVER_REG_EMAC_H__
#define __DRIVER_REG_EMAC_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define EMAC_REG         (*(emac_reg_t *)EMAC_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! System Control Register File
typedef volatile struct {
      reg32_t MAC1;                   /* MAC Registers                      */
      reg32_t MAC2;
      reg32_t IPGT;
      reg32_t IPGR;
      reg32_t CLRT;
      reg32_t MAXF;
      reg32_t SUPP;
      reg32_t TEST;
      reg32_t MCFG;
      reg32_t MCMD;
      reg32_t MADR;
      reg32_t MWTD;
      reg32_t MRDD;
      reg32_t MIND;
      REG32_RSVD(2)
      reg32_t SA0;
      reg32_t SA1;
      reg32_t SA2;
      REG32_RSVD(45)
      reg32_t Command;                /* Control Registers                  */
      reg32_t Status;
      reg32_t RxDescriptor;
      reg32_t RxStatus;
      reg32_t RxDescriptorNumber;
      reg32_t RxProduceIndex;
      reg32_t RxConsumeIndex;
      reg32_t TxDescriptor;
      reg32_t TxStatus;
      reg32_t TxDescriptorNumber;
      reg32_t TxProduceIndex;
      reg32_t TxConsumeIndex;
      REG32_RSVD(10)
      reg32_t TSV0;
      reg32_t TSV1;
      reg32_t RSV;
      REG32_RSVD(3)
      reg32_t FlowControlCounter;
      reg32_t FlowControlStatus;
      REG32_RSVD(34)
      reg32_t RxFilterCtrl;           /* Rx Filter Registers                */
      reg32_t RxFilterWoLStatus;
      reg32_t RxFilterWoLClear;
      REG32_RSVD(1)
      reg32_t HashFilterL;
      reg32_t HashFilterH;
      reg32_t __RSVD[882];
      reg32_t IntStatus;              /* Module Control Registers           */
      reg32_t IntEnable;
      reg32_t IntClear;
      reg32_t IntSet;
      REG32_RSVD(1)
      reg32_t PowerDown;
      REG32_RSVD(1)
      reg32_t Module_ID;
} emac_reg_t;

typedef volatile struct {
      reg32_t Buffer;
      reg32_t Control;
} emac_rx_desc_t;

typedef volatile struct {
      reg32_t Status;
      reg32_t HashCRC;
} emac_rx_status_t;

typedef volatile struct {
      reg32_t Buffer;
      reg32_t Control;
} emac_tx_desc_t;

typedef volatile struct {
      reg32_t Status;
} emac_tx_status_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif  //! #ifndef __DRIVER_REG_EMAC_H__
/* EOF */
