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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_i2c.h"
#include ".\i2c.h"

/*============================ MACROS ========================================*/
#ifndef I2C_MIN_CLOCKRATE
#define I2C_MIN_CLOCKRATE               1000
#endif


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

WEAK void __driver_i2c_initpd(i2c_reg_t *instance)
{
}

WEAK void __driver_i2c_deinitpd(i2c_reg_t *instance)
{
}

WEAK uint32_t __driver_i2c_get_clockrate(i2c_reg_t *instance)
{
    return PM_CORE_CLK_GET();
}

/*! \brief initialize i2c
 *! \param *instance i2c class pointer
 *! \param wClockRate i2c clock freq in Hz
 *! \retval true initialize finished
 *! \retval false error
 */
bool i2c_master_init(i2c_reg_t *instance, uint32_t wClockRate)
{
    if (wClockRate < I2C_MIN_CLOCKRATE) {
        return false;
    }
    
    __driver_i2c_initpd(instance);
    
    uint32_t wTempPeriod = __driver_i2c_get_clockrate(instance) / wClockRate;
    uint32_t wTempHigh = (wTempPeriod + 1u) >> 1;
    uint32_t wTempLow  = wTempHigh;
    
    if (wTempHigh < 4) {
        wTempHigh = 4;
    }
    if (wTempLow < 4) {
        wTempLow = 4;
    }
    instance->SCLH = wTempHigh;
    instance->SCLL = wTempLow;
    instance->CONCLR = I2C_CONCLR_STAC_MSK |
                       I2C_CONCLR_AAC_MSK  |    //!< slave is not enabled.
                       I2C_CONCLR_SIC_MSK;
    instance->CONSET = I2C_CONSET_I2EN_MSK | I2C_CONSET_AA_MSK;
        
    return true;
}

bool i2c_slave_init(i2c_reg_t *instance, uint8_t chAddress)
{
    __driver_i2c_initpd(instance);

    uint32_t wTempADR = instance->ADR0;

    wTempADR &= ~I2C_ADR_ADDRESS_MSK;
    wTempADR |= chAddress << I2C_ADR_ADDRESS0; //!< 7 bits address
    
    instance->ADR0 = wTempADR;
    instance->CONCLR = I2C_CONCLR_STAC_MSK | I2C_CONCLR_SIC_MSK;
    instance->CONSET = I2C_CONSET_I2EN_MSK | I2C_CONSET_AA_MSK;
    
    return true;
}

bool i2c_slave_set_address(i2c_reg_t *instance, i2c_addr_cfg_t *ptAddrCfg)
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
            wTempADR    = instance->ADR0;
            wTempMASK   = instance->MASK0;
            break;
        case 1:
            wTempADR    = instance->ADR1;
            wTempMASK   = instance->MASK1;
            break;
        case 2:
            wTempADR    = instance->ADR2;
            wTempMASK   = instance->MASK2;
            break;
        case 3:
            wTempADR    = instance->ADR3;
            wTempMASK   = instance->MASK3;
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
            instance->ADR0  = wTempADR;
            instance->MASK0 = wTempMASK;
            break;
        case 1:
            instance->ADR1  = wTempADR;
            instance->MASK1 = wTempMASK;
            break;
        case 2:
            instance->ADR2  = wTempADR;
            instance->MASK2 = wTempMASK;
            break;
        case 3:
            instance->ADR3  = wTempADR;
            instance->MASK3 = wTempMASK;
            break;
        default:
            return false;
    }
    
    return true;
}

bool i2c_slave_get_address(i2c_reg_t *instance, i2c_addr_cfg_t *ptAddrCfg)
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
            wTempADR = instance->ADR0;
            wTempMSK = instance->MASK0;
            break;
        case 1:
            wTempADR = instance->ADR1;
            wTempMSK = instance->MASK1;
            break;
        case 2:
            wTempADR = instance->ADR2;
            wTempMSK = instance->MASK2;
            break;
        case 3:
            wTempADR = instance->ADR3;
            wTempMSK = instance->MASK3;
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

bool i2c_slave_enable(i2c_reg_t *instance)
{
    instance->CONSET = I2C_CONSET_AA_MSK;
    
    return true;
}

bool i2c_slave_disable(i2c_reg_t *instance)
{
    instance->CONCLR = I2C_CONCLR_AAC_MSK;
    
    return true;
}

void i2c_bus_send_start(i2c_reg_t *instance)
{
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
    instance->CONSET = I2C_CONSET_STA_MSK;
}

void i2c_bus_send_start_after_stop(i2c_reg_t *instance)
{
    instance->CONSET = I2C_CONSET_STA_MSK | I2C_CONSET_STO_MSK;
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
}

void i2c_bus_send_stop(i2c_reg_t *instance)
{
    instance->CONSET = I2C_CONSET_STO_MSK;
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
}

void i2c_bus_feed_data(i2c_reg_t *instance, uint8_t chData)
{
    instance->DAT = chData;
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
}

uint8_t i2c_bus_get_data(i2c_reg_t *instance)
{
    uint8_t chData = instance->DAT;
    return chData;
}

void i2c_bus_send_ack(i2c_reg_t *instance)
{
    instance->CONSET = I2C_CONSET_AA_MSK;
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
}

void i2c_bus_send_nack(i2c_reg_t *instance)
{
    instance->CONCLR = I2C_CONCLR_AAC_MSK;
    instance->CONCLR = I2C_CONCLR_SIC_MSK;
}

bool i2c_bus_get_status(i2c_reg_t *instance, uint8_t *pchStatus)
{
    uint32_t wStatus;
    
    wStatus = instance->STAT;
    if ((I2C_BUS_M_START == wStatus) ||  (I2C_BUS_M_RESTART == wStatus)) {
        //! clear start bit
        instance->CONCLR = I2C_CONCLR_STAC_MSK;
    }
    
    if (I2C_BUS_INVALID_STATUS == wStatus) {
        instance->CONSET = I2C_CONSET_STO_MSK;
        //! clear SI bit
        instance->CONCLR = I2C_CONCLR_SIC_MSK;
    }
    
    if (pchStatus != NULL) {
        *pchStatus = wStatus;
    }
    
    return true;
}

bool i2c_bus_poll_status(i2c_reg_t *instance, uint8_t *pchStatus)
{
    if (instance->CONSET & I2C_CONSET_SI_MSK) {
        i2c_bus_get_status(instance, pchStatus);
        return true;
    }
    
    return false;
}

//    switch (instance->STAT) {
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
//bool i2c_task(i2c_reg_t *instance)
//{
//    static enum {
//        I2C_BUS_CHECK_FLAG   = 0,
//        I2C_BUS_CHECK_STATUS,
//    } s_tStatus;
//    
//    switch (s_tStatus) {
//        //! check status
//        case I2C_BUS_CHECK_FLAG:
//            if (instance->CONSET & I2C_CONSET_SI_MSK) {
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

