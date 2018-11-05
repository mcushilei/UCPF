/*******************************************************************************
 *  Copyright(C)2016 by Dreistein<mcu_shilei@hotmail.com>                     *
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

#ifndef __DRIVER_LPC17XX_I2C_C__
#ifndef __DRIVER_LPC17XX_I2C_H__
#define __DRIVER_LPC17XX_I2C_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_i2c.h"

/*============================ MACROS ========================================*/
//! \brief Macro for i2c interface function
#define __EXTERN_I2C_INTERFACE(__N,__DATA)\
    extern bool i2c##__N##_enable(void);\
    extern bool i2c##__N##_disable(void);\
    extern bool i2c##__N##_master_init(uint32_t wClockRate);\
    extern bool i2c##__N##_slave_init(uint8_t chAddress);\
    extern bool i2c##__N##_set_address(i2c_addr_cfg_t *ptAddrCfg);\
    extern bool i2c##__N##_get_address(i2c_addr_cfg_t *ptAddrCfg);\
    extern bool i2c##__N##_slave_enable(void);\
    extern bool i2c##__N##_slave_disable(void);\
    extern void i2c##__N##_bus_send_start(void);\
    extern void i2c##__N##_bus_send_start_after_stop(void);\
    extern void i2c##__N##_bus_send_stop(void);\
    extern void i2c##__N##_bus_feed_data(uint8_t chData);\
    extern uint8_t i2c##__N##_bus_get_data(void);\
    extern void i2c##__N##_bus_send_ack(void);\
    extern void i2c##__N##_bus_send_nack(void);\
    extern uint32_t i2c##__N##_bus_get_status(void);\
    
#define I2C0       I2C[0]
#define I2C1       I2C[1]
#define I2C2       I2C[2]
#define I2C3       I2C[3]

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name i2c bus status
//! @{
typedef enum {
    //! i2c works as master transmitter
    I2C_BUS_ERROR                    =0x00,
    I2C_BUS_M_START                  =0x08,
    I2C_BUS_M_RESTART                =0x10,
    I2C_BUS_M_T_SLA_W_ACK            =0x18,
    I2C_BUS_M_T_SLA_W_NACK           =0x20,
    I2C_BUS_M_T_DAT_ACK              =0x28,
    I2C_BUS_M_T_DAT_NACK             =0x30,
    I2C_BUS_M_T_ARB_LOST             =0x38,
        
    //! i2c works as master receiver
    I2C_BUS_M_R_SLA_R_ACK            =0x40,
    I2C_BUS_M_R_SLA_R_NACK           =0x48,
    I2C_BUS_M_R_DAT_ACK              =0x50,
    I2C_BUS_M_R_DAT_NACK             =0x58,
    
    //! i2c works as slave transmitter
    I2C_BUS_S_R_SLA_W_ACK            =0x60,
    I2C_BUS_S_R_ARB_IN_SLA_RW        =0x68,
    I2C_BUS_S_R_GC_ACK               =0x70,
    I2C_BUS_S_R_ARB_IN_GC            =0x78,
    I2C_BUS_S_R_DAT_ADDR_ACK         =0x80,
    I2C_BUS_S_R_DAT_ADDR_NACK        =0x88,
    I2C_BUS_S_R_DAT_GC_ACK           =0x90,
    I2C_BUS_S_R_DAT_GC_NACK          =0x98,
    I2C_BUS_S_R_STOP                 =0xA0,
    
    //! i2c works as slave reveiver
    I2C_BUS_S_T_SLA_R_ACK            =0xA8,
    I2C_BUS_S_T_ARB_IN_SLA_RW        =0xB0,
    I2C_BUS_S_T_DAT_ACK              =0xB8,
    I2C_BUS_S_T_DAT_NACK             =0xC0,
    I2C_BUS_S_T_LAST_DAT_ACK         =0xC8,
    I2C_BUS_INVALID_STATUS           =0xF8,
} em_i2c_bus_status_t;
//! @}

//! \brief i2c address configuration structure
typedef struct {
    uint8_t     chAddrIndex;    //!< index of four address register
    uint8_t     chAddress;      //!< 7bit own address, RIGHT align
    uint8_t     chMask;         //!< address mask
    bool        bGeneralCall;   //!< general call bit,true for enable "0x00"general call address.
} i2c_addr_cfg_t;

//! \brief class: i_i2c_t
DEF_INTERFACE(i_i2c_t)
    bool    (*Enable)(void);
    bool    (*Disable)(void);
    bool    (*MasterInit)(uint32_t wClockRate);                     //!< i2c master interface
    bool    (*SlaveInit)(uint8_t chAddress);                      //!< i2c slace interface
    bool    (*SetAddress)(i2c_addr_cfg_t *ptAddrCfg);
    bool    (*GetAddress)(i2c_addr_cfg_t *ptAddrCfg);
    bool    (*SlaveEnable)(void);
    bool    (*SlaveDisable)(void);
    void    (*BusSendStart)(void);
    void    (*BusSendStartAfterStop)(void);
    void    (*BusSendStop)(void);
    void    (*FeedData)(uint8_t chData);//!< load new data
    uint8_t (*GetData)(void);
    void    (*BusSendAck)(void);
    void    (*BusSendNack)(void);
    bool    (*BusGetStatus)(uint8_t *pchStatus);
END_DEF_INTERFACE(i_i2c_t)

/*============================ PROTOTYPES ====================================*/
//! \brief i2c interface function
MREPEAT(I2C_COUNT, __EXTERN_I2C_INTERFACE, NULL)

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief I2C object
extern const i_i2c_t I2C[I2C_COUNT];



#endif
#endif