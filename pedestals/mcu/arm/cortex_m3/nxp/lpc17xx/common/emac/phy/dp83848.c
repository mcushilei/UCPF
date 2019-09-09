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
#define __DRIVER_PHY_DP83848_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\emac.h"

/*============================ MACROS ========================================*/
#define DP83848_ADR	            0x0001		/* Default PHY device address        */
#define DP83848C_ID     	    0x00080017  /* PHY Identifier                    */
     
/* DP83848 Standard Registers */
#define DP83848_REG_BMCR		0x00        /* Basic Mode Control Register       */
#define DP83848_REG_BMSR        0x01        /* Basic Mode Status Register        */
#define DP83848_REG_IDR1      	0x02        /* PHY Identifier 1                  */
#define DP83848_REG_IDR2     	0x03        /* PHY Identifier 2                  */
#define DP83848_REG_PHYSTS   	0x10        /* PHY Identifier 2                  */

/* DP83848 Extended Registers */
#define DP83848_REG_STS         0x10        /* Status Register                   */
#define DP83848_REG_MICR        0x11        /* MII Interrupt Control Register    */
#define DP83848_REG_MISR        0x12        /* MII Interrupt Status Register     */
#define DP83848_REG_FCSCR       0x14        /* False Carrier Sense Counter       */
#define DP83848_REG_RECR        0x15        /* Receive Error Counter             */
#define DP83848_REG_PCSR        0x16        /* PCS Sublayer Config. and Status   */
#define DP83848_REG_RBR         0x17        /* RMII and Bypass Register          */
#define DP83848_REG_LEDCR       0x18        /* LED Direct Control Register       */
#define DP83848_REG_PHYCR       0x19        /* PHY Control Register              */
#define DP83848_REG_10BTSCR     0x1A        /* 10Base-T Status/Control Register  */
#define DP83848_REG_CDCTRL1     0x1B        /* CD Test Control and BIST Extens.  */
#define DP83848_REG_EDCR        0x1D        /* Energy Detect Control Register    */



#define DP83848_BMCR_COLLISION_TEST               ((uint16_t)0x0080)
#define DP83848_BMCR_DUPLEX_MODE                  ((uint16_t)0x0100)
#define DP83848_BMCR_RESTART_AUTO_NEGOTIATION     ((uint16_t)0x0200)
#define DP83848_BMCR_ISOLATE                      ((uint16_t)0x0400)
#define DP83848_BMCR_POWER_DOWN                   ((uint16_t)0x0800)
#define DP83848_BMCR_AUTO_NEGOTIATION_ENABLE      ((uint16_t)0x1000)
#define DP83848_BMCR_SPEED_SELECTION              ((uint16_t)0x2000)
#define DP83848_BMCR_LOOPBACK                     ((uint16_t)0x4000)
#define DP83848_BMCR_RESET                        ((uint16_t)0x8000)

#define DP83848_BMSR_EXTENDED_CAPABLITIY          ((uint16_t)0x0001)
#define DP83848_BMSR_JABBER_DETECT                ((uint16_t)0x0002)
#define DP83848_BMSR_LINK_STATUS                  ((uint16_t)0x0004)
#define DP83848_BMSR_AUTO_NEGOTIATION_ABILITY     ((uint16_t)0x0008)
#define DP83848_BMSR_REMOTE_FAULT                 ((uint16_t)0x0010)
#define DP83848_BMSR_AUTO_NEGOTIATION_COMPLETE    ((uint16_t)0x0020)
#define DP83848_BMSR_MF_PREAMBLE_SUPPRESSION      ((uint16_t)0x0040)
#define DP83848_BMSR_10_HALF_DUPLES               ((uint16_t)0x0800)
#define DP83848_BMSR_10_FULL_DUPLES               ((uint16_t)0x1000)
#define DP83848_BMSR_100_HALF_DUPLES              ((uint16_t)0x2000)
#define DP83848_BMSR_100_FULL_DUPLES              ((uint16_t)0x4000)
#define DP83848_BMSR_100_T4                       ((uint16_t)0x8000)

#define DP83848_PHYSTS_LINK_STATUS                ((uint16_t)1 << 0)
#define DP83848_PHYSTS_SPEED_STATUS               ((uint16_t)1 << 1)
#define DP83848_PHYSTS_DUPLEX_STATUS              ((uint16_t)1 << 2)
#define DP83848_PHYSTS_LOOPBACK_STATUS            ((uint16_t)1 << 3)
#define DP83848_PHYSTS_AUTO_NEG_COMPLETE          ((uint16_t)1 << 4)
#define DP83848_PHYSTS_JABBER_DETECT              ((uint16_t)1 << 5)
#define DP83848_PHYSTS_REMOTE_FAULT               ((uint16_t)1 << 6)
#define DP83848_PHYSTS_MII_INTERRUPT              ((uint16_t)1 << 7)
#define DP83848_PHYSTS_PAGE_RECEIVED              ((uint16_t)1 << 8)
#define DP83848_PHYSTS_DESCRAMBLER_LOCK           ((uint16_t)1 << 9)
#define DP83848_PHYSTS_SIGNAL_DETECT              ((uint16_t)1 << 10)
#define DP83848_PHYSTS_FALSE_CARRIER_SENSE_LATCH  ((uint16_t)1 << 11)
#define DP83848_PHYSTS_POLARITY_STATUS            ((uint16_t)1 << 12)
#define DP83848_PHYSTS_RECEIVE_ERROR_LATCH        ((uint16_t)1 << 13)
#define DP83848_PHYSTS_MDIX_MODE                  ((uint16_t)1 << 14)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


bool ethernet_phy_is_net_connected(void)
{
    uint16_t regValue;

    regValue = 0;
    mii_phy_reg_read(DP83848_ADR, DP83848_REG_BMSR, &regValue);
    if (regValue & DP83848_BMSR_LINK_STATUS) {
        return true;
	}
    return false;
}

bool ethernet_phy_auto_negotiation(void)
{
    uint32_t tout;
    uint16_t regValue;

    if (ethernet_phy_is_net_connected()) {
        return true;
    }

    //Check if AUTO_NEGOTIATION is enable.
    regValue = 0;
    mii_phy_reg_read(DP83848_ADR, DP83848_REG_BMCR, &regValue);
    if (!(DP83848_BMCR_AUTO_NEGOTIATION_ENABLE & regValue)) {
        return false;
    }

    //Restart a AUTO_NEGOTIATION process.
    regValue |= DP83848_BMCR_RESTART_AUTO_NEGOTIATION;  /* Use autonegotiation and 100M speed. */
    if (!mii_phy_reg_write(DP83848_ADR, DP83848_REG_BMCR, &regValue)) {
        return false;
    }
    for (tout = 0x100000; tout != 0; tout--) {          /* Wait to complete Auto_Negotiation. 2~3s!!! */
        regValue = 0;
        mii_phy_reg_read(DP83848_ADR, DP83848_REG_BMSR, &regValue);
        if (regValue & DP83848_BMSR_AUTO_NEGOTIATION_COMPLETE) {
            break;
        }
    }
    if (0 == tout) {
        return false;
    }

    return true;
}

bool ethernet_phy_init(void)
{
    uint32_t tout, id1 = 0, id2 = 0;
    uint16_t regValue;
      
    /* reset DP83848 */
    regValue = DP83848_BMCR_RESET;
    if (!mii_phy_reg_write(DP83848_ADR, DP83848_REG_BMCR, &regValue)) {
        false; 
    }
    
    /* Wait for reset to complete. */
    for (tout = 0x50000; tout != 0; tout--) {
        regValue = 0;
        mii_phy_reg_read(DP83848_ADR, DP83848_REG_BMCR, &regValue);
        if (!(regValue & DP83848_BMCR_RESET)) {
            break; 
        }
    }
    if (0 == tout) {
        return false;
    }

    /* Use Auto_Negotiation */
#ifdef NET_DEBUG
    regValue = DP83848_BMCR_SPEED_SELECTION | DP83848_BMCR_DUPLEX_MODE | DP83848_BMCR_LOOPBACK;
#else
    regValue = DP83848_BMCR_AUTO_NEGOTIATION_ENABLE;
#endif
    if (!mii_phy_reg_write(DP83848_ADR, DP83848_REG_BMCR, &regValue)) {
        return false;
    }

#ifdef NET_DEBUG
    /* Check if this is a DP83848C DP83848. */
    mii_phy_reg_read(DP83848_ADR, DP83848_REG_IDR1, (uint16_t *)&id1);
    mii_phy_reg_read(DP83848_ADR, DP83848_REG_IDR2, (uint16_t *)&id2);
    id1 <<= 6;
    id2 >>= 10;
    if ((id1 | id2) != DP83848C_ID) {
        return false;
    }
#endif

    return true;
}

uint8_t ethernet_phy_get_net_mode(void)
{
    uint16_t regValue = 0;

    if (!mii_phy_reg_read(DP83848_ADR, DP83848_REG_BMSR, &regValue)) {
        return 0;
    }
    
    if (DP83848_BMSR_100_FULL_DUPLES & regValue) {
        return (FULL_DUPLES_100M);
    } else if (DP83848_BMSR_100_HALF_DUPLES & regValue) {
        return (HALF_DUPLES_100M);
    } else if (DP83848_BMSR_10_FULL_DUPLES & regValue) {
        return (FULL_DUPLES_10M);
    } else if (DP83848_BMSR_10_HALF_DUPLES & regValue) {
        return (HALF_DUPLES_10M);
    }

    return 0;
}

/* EOF */
