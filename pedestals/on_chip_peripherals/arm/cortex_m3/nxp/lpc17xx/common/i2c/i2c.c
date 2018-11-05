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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_LPC17XX_I2C_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_i2c.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
//! \brief The reference for current object
#define this        (*ptThis)

#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus = PM_POWER_GET_STATUS(ptThis->tPOWER);              \
        PM_POWER_ENABLE(ptThis->tPOWER);                                     \
        __VA_ARGS__;                                                            \
        PM_POWER_RESUME(ptThis->tPOWER, wStatus);                            \
    }

#ifndef I2C_MIN_CLOCKRATE
#define I2C_MIN_CLOCKRATE               1000
#endif


//! \brief Macro for i2c init
#define __I2C_OBJ(__N,__DATA)\
    {\
        ((i2c_reg_t *)(I2C##__N##_BASE_ADDRESS)),\
        PCONP_I2C##__N,    \
    },

//! \brief Macro for spi interface init
#define __I2C_INTERFACE(__N,__A)\
    {\
        &i2c##__N##_enable,\
        &i2c##__N##_disable,\
        &i2c##__N##_master_init,\
        &i2c##__N##_slave_init,\
        &i2c##__N##_set_address,\
        &i2c##__N##_get_address,\
        &i2c##__N##_slave_enable,\
        &i2c##__N##_slave_disable,\
        &i2c##__N##_bus_send_start,\
        &i2c##__N##_bus_send_start_after_stop,\
        &i2c##__N##_bus_send_stop,\
        &i2c##__N##_bus_feed_data,\
        &i2c##__N##_bus_get_data,\
        &i2c##__N##_bus_send_ack,\
        &i2c##__N##_bus_send_nack,\
        &i2c##__N##_bus_get_status,\
    },
    
//! \brief Macro for i2c interface function prototypes
#define __I2C_INTERFACE_PROTOTYPES(__N,__DATA)\
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
    extern bool i2c##__N##_bus_get_status(uint8_t *pchStatus);\

//! \brief Macro of i2c modules interface function body
#define __I2C_INTERFACE_DEFINE(__N,__DATA)                          \
    bool i2c##__N##_enable(void)                \
    {                                                               \
        return i2c_enable((__i2c_t *)&__I2C[__N]); \
    }                                                               \
    bool i2c##__N##_disable(void)                \
    {                                                               \
        return i2c_disable((__i2c_t *)&__I2C[__N]); \
    }                                                               \
    bool i2c##__N##_master_init(uint32_t wClockRate)                \
    {                                                               \
        return i2c_master_init((__i2c_t *)&__I2C[__N], wClockRate); \
    }                                                               \
    bool i2c##__N##_slave_init(uint8_t chAddress)                   \
    {                                                               \
        return i2c_slave_init((__i2c_t *)&__I2C[__N],chAddress);    \
    }                                                               \
    bool i2c##__N##_set_address(i2c_addr_cfg_t *ptAddrCfg)          \
    {                                                               \
        return i2c_slave_set_address((__i2c_t *)&__I2C[__N], ptAddrCfg);   \
    }                                                               \
    bool i2c##__N##_get_address(i2c_addr_cfg_t *ptAddrCfg)          \
    {                                                               \
        return i2c_slave_get_address((__i2c_t *)&__I2C[__N], ptAddrCfg);     \
    }                                                               \
    bool i2c##__N##_slave_enable(void)                              \
    {                                                               \
        return i2c_slave_enable((__i2c_t *)&__I2C[__N]);            \
    }                                                               \
    bool i2c##__N##_slave_disable(void)                             \
    {                                                               \
        return i2c_slave_disable((__i2c_t *)&__I2C[__N]);           \
    }                                                               \
    void i2c##__N##_bus_send_start(void)                            \
    {                                                               \
        i2c_bus_send_start((__i2c_t *)&__I2C[__N]);                 \
    }                                                               \
    void i2c##__N##_bus_send_start_after_stop(void)                 \
    {                                                               \
        i2c_bus_send_start_after_stop((__i2c_t *)&__I2C[__N]);      \
    }                                                               \
    void i2c##__N##_bus_send_stop(void)                             \
    {                                                               \
        i2c_bus_send_stop((__i2c_t *)&__I2C[__N]);                  \
    }                                                               \
    void i2c##__N##_bus_feed_data(uint8_t chData)                   \
    {                                                               \
        i2c_bus_feed_data((__i2c_t *)&__I2C[__N], chData);          \
    }                                                               \
    uint8_t i2c##__N##_bus_get_data(void)                           \
    {                                                               \
        return i2c_bus_get_data((__i2c_t *)&__I2C[__N]);            \
    }                                                               \
    void i2c##__N##_bus_send_ack(void)                              \
    {                                                               \
        i2c_bus_send_ack((__i2c_t *)&__I2C[__N]);                   \
    }                                                               \
    void i2c##__N##_bus_send_nack(void)                             \
    {                                                               \
        i2c_bus_send_nack((__i2c_t *)&__I2C[__N]);                  \
    }                                                               \
    bool i2c##__N##_bus_get_status(uint8_t *pchStatus)                        \
    {                                                               \
        return i2c_bus_get_status((__i2c_t *)&__I2C[__N], pchStatus);          \
    }                                                               \

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
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

//! \name i2c internal class
//! @{
DEF_STRUCTURE(__i2c_t)
    i2c_reg_t*  ptREG;         //!< reference to register page
    uint32_t    tPOWER;
END_DEF_STRUCTURE(__i2c_t)
//! @}

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


/*============================ PROTOTYPES ====================================*/
MREPEAT(I2C_COUNT, __I2C_INTERFACE_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief I2C object
extern const i_i2c_t I2C[] = {
    MREPEAT(I2C_COUNT, __I2C_INTERFACE, 0)
};
/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal i2c boject
static const __i2c_t __I2C[] = {
    MREPEAT(I2C_COUNT, __I2C_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/
bool i2c_enable(__i2c_t *ptThis)
{
    PM_POWER_ENABLE(this.tPOWER);
    
    return true;
}

bool i2c_disable(__i2c_t *ptThis)
{
    PM_POWER_DISABLE(this.tPOWER);
    
    return true;
}

/*! \brief initialize i2c
 *! \param *ptThis i2c class pointer
 *! \param wClockRate i2c clock freq in Hz
 *! \retval true initialize finished
 *! \retval false error
 */
static bool i2c_master_init(__i2c_t *ptThis, uint32_t wClockRate)
{
    if (wClockRate < I2C_MIN_CLOCKRATE) {
        return false;
    }
    
    SAFE_CLK_CODE(
        uint32_t wTempPeriod = PM_CORE_CLK_GET() / wClockRate;
        uint32_t wTempHigh = wTempPeriod >> 1;
        uint32_t wTempLow  = wTempPeriod - wTempHigh;
        
        if (wTempHigh < 4) {
            wTempHigh = 4;
        }
        if (wTempLow < 4) {
            wTempLow = 4;
        }
        this.ptREG->SCLH = wTempHigh;
        this.ptREG->SCLL = wTempLow;
        this.ptREG->CONCLR = I2C_CONCLR_STAC_MSK |
                                   I2C_CONCLR_AAC_MSK  |    //!< slave is not enabled.
                                   I2C_CONCLR_SIC_MSK;
        this.ptREG->CONSET = I2C_CONSET_I2EN_MSK | I2C_CONSET_AA_MSK;
    )
        
    return true;
}

static bool i2c_slave_init(__i2c_t *ptThis, uint8_t chAddress)
{
    SAFE_CLK_CODE(
        uint32_t wTempADR = this.ptREG->ADR0;

        wTempADR &= ~I2C_ADR_ADDRESS_MSK;
        wTempADR |= chAddress << I2C_ADR_ADDRESS0; //!< 7 bits address
        
        this.ptREG->ADR0 = wTempADR;
        this.ptREG->CONCLR = I2C_CONCLR_STAC_MSK | I2C_CONCLR_SIC_MSK;
        this.ptREG->CONSET = I2C_CONSET_I2EN_MSK | I2C_CONSET_AA_MSK;
    )
        
    return true;
}

static bool i2c_slave_set_address(__i2c_t *ptThis, i2c_addr_cfg_t *ptAddrCfg)
{
    uint32_t wTempADR  = 0;
    uint32_t wTempMASK = 0;
    
    //! validate input parameter
    if (ptAddrCfg == NULL) {
        return false;
    }
    
    if (ptAddrCfg->chAddrIndex >= 4) {
        return false;
    }
    
    //! read register
    switch (ptAddrCfg->chAddrIndex) {
        case 0:
            wTempADR    = this.ptREG->ADR0;
            wTempMASK   = this.ptREG->MASK0;
            break;
        case 1:
            wTempADR    = this.ptREG->ADR1;
            wTempMASK   = this.ptREG->MASK1;
            break;
        case 2:
            wTempADR    = this.ptREG->ADR2;
            wTempMASK   = this.ptREG->MASK2;
            break;
        case 3:
            wTempADR    = this.ptREG->ADR3;
            wTempMASK   = this.ptREG->MASK3;
            break;
        default:
            break;
    }
        
    //! modify register
    if (ptAddrCfg->bGeneralCall) {
        wTempADR |= I2C_ADR_GC_MSK;
    } else {
        wTempADR &= ~I2C_ADR_GC_MSK;
    }
    wTempADR &= ~I2C_ADR_ADDRESS_MSK;
    wTempADR |= ptAddrCfg->chAddress << I2C_ADR_ADDRESS0; //!< 7 bits address
    
    wTempMASK &= ~I2C_ADR_MASK_MSK;
    wTempMASK |= ptAddrCfg->chMask << I2C_ADR_MASK0;     //!< 7 bits address mask
    
    //! write back
    switch (ptAddrCfg->chAddrIndex == 0) {
        case 0:
            this.ptREG->ADR0  = wTempADR;
            this.ptREG->MASK0 = wTempMASK;
            break;
        case 1:
            this.ptREG->ADR1  = wTempADR;
            this.ptREG->MASK1 = wTempMASK;
            break;
        case 2:
            this.ptREG->ADR2  = wTempADR;
            this.ptREG->MASK2 = wTempMASK;
            break;
        case 3:
            this.ptREG->ADR3  = wTempADR;
            this.ptREG->MASK3 = wTempMASK;
            break;
        default:
            return false;
    }
    
    return true;
}

static bool i2c_slave_get_address(__i2c_t *ptThis, i2c_addr_cfg_t *ptAddrCfg)
{
    //! validate input parameter
    if (ptAddrCfg == NULL) {
        return false;
    }
    
    if (ptAddrCfg->chAddrIndex >= 4) {
        return false;
    }
    
    uint32_t wTempADR;
    uint32_t wTempMSK;
    
    switch (ptAddrCfg->chAddrIndex) {
        case 0:
        default:
            wTempADR = this.ptREG->ADR0;
            wTempMSK = this.ptREG->MASK0;
            break;
        case 1:
            wTempADR = this.ptREG->ADR1;
            wTempMSK = this.ptREG->MASK1;
            break;
        case 2:
            wTempADR = this.ptREG->ADR2;
            wTempMSK = this.ptREG->MASK2;
            break;
        case 3:
            wTempADR = this.ptREG->ADR3;
            wTempMSK = this.ptREG->MASK3;
            break;
    }
    
    ptAddrCfg->chAddress      = ((wTempADR & I2C_ADR_ADDRESS_MSK) >> I2C_ADR_ADDRESS0);
    ptAddrCfg->chMask         = ((wTempMSK & I2C_ADR_MASK_MSK) >> I2C_ADR_MASK0);
    if (wTempADR & I2C_ADR_GC_MSK) {
        ptAddrCfg->bGeneralCall = true;
    } else {
        ptAddrCfg->bGeneralCall = false;
    }
    
    return true;
}

static bool i2c_slave_enable(__i2c_t *ptThis)
{
    this.ptREG->CONSET = I2C_CONSET_AA_MSK;
    
    return true;
}

static bool i2c_slave_disable(__i2c_t *ptThis)
{
    this.ptREG->CONCLR = I2C_CONCLR_AAC_MSK;
    
    return true;
}

static void i2c_bus_send_start(__i2c_t *ptThis)
{
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
    this.ptREG->CONSET = I2C_CONSET_STA_MSK;
}

static void i2c_bus_send_start_after_stop(__i2c_t *ptThis)
{
    this.ptREG->CONSET = I2C_CONSET_STA_MSK | I2C_CONSET_STO_MSK;
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
}

static void i2c_bus_send_stop(__i2c_t *ptThis)
{
    this.ptREG->CONSET = I2C_CONSET_STO_MSK;
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
}

static void i2c_bus_feed_data(__i2c_t *ptThis, uint8_t chData)
{
    this.ptREG->DAT = chData;
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
}

static uint8_t i2c_bus_get_data(__i2c_t *ptThis)
{
    uint8_t chData = this.ptREG->DAT;
    return chData;
}

static void i2c_bus_send_ack(__i2c_t *ptThis)
{
    this.ptREG->CONSET = I2C_CONSET_AA_MSK;
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
}

static void i2c_bus_send_nack(__i2c_t *ptThis)
{
    this.ptREG->CONCLR = I2C_CONCLR_AAC_MSK;
    this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
}

static bool i2c_bus_get_status(__i2c_t *ptThis, uint8_t *pchStatus)
{
    uint32_t wStatus;
    
    if (this.ptREG->CONSET & I2C_CONSET_SI_MSK) {
        wStatus = this.ptREG->STAT;
        if ((I2C_BUS_M_START == wStatus)
        ||  (I2C_BUS_M_RESTART == wStatus)) {
            //! clear start bit
            this.ptREG->CONCLR = I2C_CONCLR_STAC_MSK;
        }
        
        if (I2C_BUS_INVALID_STATUS == wStatus) {
            this.ptREG->CONSET = I2C_CONSET_STO_MSK;
            //! clear SI bit
            this.ptREG->CONCLR = I2C_CONCLR_SIC_MSK;
        }
        
        if (pchStatus != NULL) {
            *pchStatus = wStatus;
        }
        return true;
    }
    
    return false;
}

//    switch (this.ptREG->STAT) {
///*=========================== Master start ===================================*/
//        //! start condition has been transimitted
//        case I2C_BUS_M_START:
//        case I2C_BUS_M_RESTART:
//            break;
//            
///*=========================== Master transmitter start =======================*/
//        //! SLA + W has been transmitted, ACK received
//        case I2C_BUS_M_T_SLA_W_ACK:
//            break;
//            
//        //! SLA + W has been transmitted, NACK received
//        case I2C_BUS_M_T_SLA_W_NACK:
//            break;
//            
//        //! data has been transmitted, ACK received
//        case I2C_BUS_M_T_DAT_ACK:
//            break;
//            
//        //! data has been transmitted, NACK received
//        case I2C_BUS_M_T_DAT_NACK:
//            break;
//        
//        //! arbitration lost in sla+w/r or data bytes
//        case I2C_BUS_M_T_ARB_LOST:
//            break;
///*=========================== Master transmitter end =========================*/
///*=========================== Master receive start ===========================*/
//        //! SLA + R has been transmitted, ACK received
//        case I2C_BUS_M_R_SLA_R_ACK:
//            break;
//            
//        //! SLA + R has been transmitted, NACK received
//        case I2C_BUS_M_R_SLA_R_NACK:
//            break;
//            
//        //! data has been received, ACK returned
//        case I2C_BUS_M_R_DAT_ACK:
//            break;
//            
//        //! data has been received, NACK returned
//        case I2C_BUS_M_R_DAT_NACK:
//            break;
///*=========================== Master receive end =============================*/
///*============================= Slave receiver begain ========================*/
//        case I2C_BUS_S_R_SLA_W_ACK:
//            break;
//            
//        case I2C_BUS_S_R_ARB_IN_SLA_RW:
//            break;
//            
//        case I2C_BUS_S_R_GC_ACK:
//            break;
//            
//        case I2C_BUS_S_R_ARB_IN_GC:
//            break;
//            
//        case I2C_BUS_S_R_DAT_ADDR_ACK:
//            break;
//            
//        case I2C_BUS_S_R_DAT_ADDR_NACK:
//            break;
//            
//        case I2C_BUS_S_R_DAT_GC_ACK:
//            break;
//            
//        case I2C_BUS_S_R_DAT_GC_NACK:
//            break;
//            
//        case I2C_BUS_S_R_STOP:
//            break;
///*============================== Slave receiver end ==========================*/
///*============================ Slave transmitter start =======================*/
//        case I2C_BUS_S_T_SLA_R_ACK:
//            break;
//            
//        case I2C_BUS_S_T_ARB_IN_SLA_RW:
//            break;
//            
//        case I2C_BUS_S_T_DAT_ACK:
//            break;
//            
//        case I2C_BUS_S_T_DAT_NACK:
//            break;
//            
//        case I2C_BUS_S_T_LAST_DAT_ACK:
//            break;
//
///*============================= Slave transmitter end ========================*/
//        case I2C_BUS_ERROR:
//        case I2C_BUS_INVALID_STATUS:
//        default:
//            break;
//    }
    

//#define I2C_BUS_CHECK_FSM_RST()  do{\
//            s_tStatus = I2C_BUS_CHECK_FLAG;\
//        } while (false)
//        
//static bool i2c_task(__i2c_t *ptThis)
//{
//    static enum {
//        I2C_BUS_CHECK_FLAG   = 0,
//        I2C_BUS_CHECK_STATUS,
//    } s_tStatus;
//    
//    switch (s_tStatus) {
//        //! check status
//        case I2C_BUS_CHECK_FLAG:
//            if (this.ptREG->CONSET & I2C_CONSET_SI_MSK) {
//                s_tStatus = I2C_BUS_CHECK_STATUS;
//            } else {
//                break;
//            }
//            
//        //! handle status
//        case I2C_BUS_CHECK_STATUS:
//            i2c_handle_status(&this);
//            I2C_BUS_CHECK_FSM_RST();
//            return fsm_rt_cpl;
//    }
//    
//    return fsm_rt_ongoing;
//}


//! \brief i2c modules interface function body
MREPEAT(I2C_COUNT, __I2C_INTERFACE_DEFINE, NULL)
