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


#ifndef __REG_SC_H__
#define __REG_SC_H__


/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SYS_REG         (*(sys_reg_t *)SYSTEM_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! System Control Register File
typedef volatile struct {
    // 0-9: reserved
    REG16_RSVD(10)
    
    // 10: High-speed peripheral clock pre-scaler
    DEF_REG16
       reg16_t HSPCLK   :3;     // 2:0   Rate relative to SYSCLKOUT
       reg16_t          :13;    // 15:3  reserved
    END_DEF_REG16(HISPCP)

#define HISPCP_BIT0     0u
#define HISPCP_BIT1     1u
#define HISPCP_BIT2     2u
#define HISPCP_MSK      ((1u << HISPCP_BIT0) \
                        |(1u << HISPCP_BIT1) \
                        |(1u << HISPCP_BIT2))

    // 11: Low-speed peripheral clock pre-scaler
    DEF_REG16
        reg16_t LSPCLK  :3;     // 2:0   Rate relative to SYSCLKOUT
        reg16_t         :13;    // 15:3  reserved
    END_DEF_REG16(LOSPCP)

#define LOSPCP_BIT0     0u
#define LOSPCP_BIT1     1u
#define LOSPCP_BIT2     2u
#define LOSPCP_MSK      ((1u << LOSPCP_BIT0) \
                        |(1u << LOSPCP_BIT1) \
                        |(1u << LOSPCP_BIT2))
      
    // 12: Peripheral clock control register
    DEF_REG16
        reg16_t EVAENCLK    :1; // 0     Enable high speed clk to EV-A
        reg16_t EVBENCLK    :1; // 1     Enable high speed clk to EV-B
        reg16_t             :1; // 2     reserved
        reg16_t ADCENCLK    :1; // 3     Enable high speed clk to ADC
        reg16_t             :4; // 7:4   reserved
        reg16_t SPIENCLK    :1; // 8     Enable low speed clk to SPI
        reg16_t             :1; // 9     reserved
        reg16_t SCIAENCLK   :1; // 10    Enable low speed clk to SCI-A
        reg16_t SCIBENCLK   :1; // 11    Enable low speed clk to SCI-B
        reg16_t MCBSPENCLK  :1; // 12    Enable low speed clk to McBSP
        reg16_t             :1; // 13    reserved
        reg16_t ECANENCLK   :1; // 14    Enable system clk to eCAN
    END_DEF_REG16(PCLKCR)

#define PCLKCR_EVAENCLK_MSK     (1u << PCLKCR_EVAENCLK)
#define PCLKCR_EVBENCLK_MSK     (1u << PCLKCR_EVBENCLK)
#define PCLKCR_ADCENCLK_MSK     (1u << PCLKCR_ADCENCLK)
#define PCLKCR_SPIENCLK_MSK     (1u << PCLKCR_SPIENCLK)
#define PCLKCR_SCIAENCLK_MSK    (1u << PCLKCR_SCIAENCLK)
#define PCLKCR_SCIBENCLK_MSK    (1u << PCLKCR_SCIBENCLK)
#define PCLKCR_MCBSPENCLK_MSK   (1u << PCLKCR_MCBSPENCLK)
#define PCLKCR_ECANENCLK_MSK    (1u << PCLKCR_ECANENCLK)

    // 13: reserved
    REG16_RSVD(1)

    // 14: Low-power mode control register 0
    DEF_REG16
        reg16_t LPM         :2; // 1:0   Set the low power mode
        reg16_t QUALSTDBY   :6; // 7:2   Qualification
        reg16_t             :8; // 15:8  reserved
    END_DEF_REG16(LPMCR0)

    // 15: Low-power mode control register 1
    DEF_REG16
        reg16_t XINT1   :1;     // 0     Enable XINT1 to wake the device from standby
        reg16_t XNMI    :1;     // 1     Enable XMNI to wake the device from standby
        reg16_t WDINT   :1;     // 2     Enable watchdog interrupt to wake the device from standby
        reg16_t T1CTRIP :1;     // 3     Enable T1CTRIP to wake the device from standby
        reg16_t T2CTRIP :1;     // 4     Enable T2CTRIP to wake the device from standby
        reg16_t T3CTRIP :1;     // 5     Enable T3CTRIP to wake the device from standby
        reg16_t T4CTRIP :1;     // 6     Enable T4CTRIP to wake the device from standby
        reg16_t C1TRIP  :1;     // 7     Enable C1TRIP to wake the device from standby
        reg16_t C2TRIP  :1;     // 8     Enable C2TRIP to wake the device from standby
        reg16_t C3TRIP  :1;     // 9     Enable C3TRIP to wake the device from standby
        reg16_t C4TRIP  :1;     // 10    Enable C4TRIP to wake the device from standby
        reg16_t C5TRIP  :1;     // 11    Enable C5TRIP to wake the device from standby
        reg16_t C6TRIP  :1;     // 12    Enable C6TRIP to wake the device from standby
        reg16_t SCIRXA  :1;     // 13    Enable SCIRXA to wake the device from standby
        reg16_t SCIRXB  :1;     // 14    Enable SCIRXB to wake the device from standby
        reg16_t CANRX   :1;     // 15    Enable CANRX to wake the device from standby
    END_DEF_REG16(LPMCR1)

    // 16: reserved
    REG16_RSVD(1)

    // 17: PLL control register
    DEF_REG16
        reg16_t DIV     :4;     // 3:0   Set clock ratio for the PLL
        reg16_t         :12;    // 15:4  reserved
    END_DEF_REG16(PLLCR)
} sys_reg_t;

typedef enum {
    PCLKCR_EVAENCLK    = 0,
    PCLKCR_EVBENCLK    = 1,
    PCLKCR_ADCENCLK    = 3,
    PCLKCR_SPIENCLK    = 8,
    PCLKCR_SCIAENCLK   = 10,
    PCLKCR_SCIBENCLK   = 11,
    PCLKCR_MCBSPENCLK  = 12,
    PCLKCR_ECANENCLK   = 14,
} em_pclkcr_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


#endif
