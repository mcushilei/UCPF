/*******************************************************************************
 *  Copyright(C)2016-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include ".\reg_uart.h"
#include "..\scon\pm.h"
#include "./uart.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define MODE_LENGTH_MSK         0x0300
#define MODE_LENGTH_SHIFT       8

#define __SAFE_CLK_CODE_BEGIN()                                         \
    {                                                                   \
        uint32_t wStatus = peripheral_clock_get_status(ptThis->tPCON);  \
        peripheral_clock_enable(ptThis->tPCON);                         \

#define __SAFE_CLK_CODE_END()                                           \
        peripheral_clock_resume_status(ptThis->tPCON, wStatus);         \
    }

#define __SAFE_CLK_CODE(...)    \
    __SAFE_CLK_CODE_BEGIN()     \
    __VA_ARGS__;                \
    __SAFE_CLK_CODE_END()       \

#define __UART_INTERFACE(__N, __A)                                          \
    {                                                                       \
        &uart##__N##_config,                                                \
        &uart##__N##_idle,                                                  \
        &uart##__N##_enable,                                                \
        &uart##__N##_disable,                                               \
        &uart##__N##_write_byte,                                            \
        &uart##__N##_read_byte,                                             \
        {                                                                   \
            &uart##__N##_baudrate_set,                                      \
            &uart##__N##_baudrate_get,                                      \
        },                                                                  \
    },


#define __UART_FUNCTION(__N, __A)                                           \
    bool uart##__N##_config(const uart_cfg_t *ptUsartCFG)                   \
    {                                                                       \
        return uart_config(__N, ptUsartCFG);                                \
    }                                                                       \
                                                                            \
    bool uart##__N##_idle(void)                                             \
    {                                                                       \
        return uart_idle(__N);                                              \
    }                                                                       \
                                                                            \
    bool uart##__N##_enable(void)                                           \
    {                                                                       \
        return uart_enable(__N);                                            \
    }                                                                       \
                                                                            \
    bool uart##__N##_disable(void)                                          \
    {                                                                       \
        return uart_disable(__N);                                           \
    }                                                                       \
                                                                            \
    bool uart##__N##_write_byte(uint8_t chByte)                             \
    {                                                                       \
        return uart_write_byte(__N, chByte);                                \
    }                                                                       \
                                                                            \
    bool uart##__N##_read_byte(uint8_t *pchByte)                            \
    {                                                                       \
        return uart_read_byte(__N, pchByte);                                \
    }                                                                       \
                                                                            \
    bool uart##__N##_baudrate_set(uint32_t wBaudrate)                       \
    {                                                                       \
        return uart_baudrate_set(__N, wBaudrate);                           \
    }                                                                       \
                                                                            \
    uint32_t uart##__N##_baudrate_get(void)                                 \
    {                                                                       \
        return uart_baudrate_get(__N);                                      \
    }                                                                       \
                                                                            \
    bool uart##__N##_int_enable(uint32_t wMask)                             \
    {                                                                       \
        return uart_int_enable(__N, wMask);                                 \
    }                                                                       \
                                                                            \
    bool uart##__N##_int_disable(uint32_t wMask)                            \
    {                                                                       \
        return uart_int_disable(__N, wMask);                                \
    }


#define __UART_FUNCTION_PROTOTYPES(__N, __A)                                \
    static bool uart##__N##_config(const uart_cfg_t *ptUsartCFG);           \
    static bool uart##__N##_idle(void);                                     \
    static bool uart##__N##_enable(void);                                   \
    static bool uart##__N##_disable(void);                                  \
    static bool uart##__N##_write_byte(uint8_t chByte);                     \
    static bool uart##__N##_read_byte(uint8_t *pchByte);                    \
    static bool uart##__N##_baudrate_set(uint32_t wBaudrate);               \
    static uint32_t uart##__N##_baudrate_get(void);                         \
    static bool uart##__N##_int_enable(uint32_t wMask);                     \
    static bool uart##__N##_int_disable(uint32_t wMask);                    \
    
#define __UART_OBJ(__N, __A)                                                \
    {                                                                       \
        ((uart_reg_t *)(UART##__N##_BASE_ADDRESS)),                         \
        PCONP_UART##__N,                                                    \
        PCLK_UART##__N,                                                     \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name internal class
//! @{
DEF_CLASS(__uart_t)
    uart_reg_t *const   ptREG;
    const uint32_t      tPCON;
    const uint32_t      tPCLK;
    uint32_t            wBaundRate;
END_DEF_CLASS(__uart_t)
//! @}

/*============================ PROTOTYPES ====================================*/
static bool uart_idle(uint32_t wUsart);
static bool uart_config(uint32_t wUsart, const uart_cfg_t *ptUsartCFG);
static bool     uart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate);
static uint32_t uart_baudrate_get(uint32_t wUsart);
static bool uart_enable(uint32_t wUsart);
static bool uart_disable(uint32_t wUsart);
static bool uart_write_byte(uint32_t wUsart, uint8_t chByte);
static bool uart_read_byte(uint32_t wUsart, uint8_t *pchByte);
static bool uart_int_enable(uint32_t wUsart, uint32_t wMask);
static bool uart_int_disable(uint32_t wUsart, uint32_t wMask);

MREPEAT(UART_COUNT, __UART_FUNCTION_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/                                               
//! \brief uart object
const i_uart_t UART[] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal uart object
static CLASS(__uart_t) __UART[] = {
    MREPEAT(UART_COUNT, __UART_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief to check whether uart is idle.
 *! \param wUsart wich uart to check
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool uart_idle(uint32_t wUsart)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];
    uint32_t wTempLCR;
    bool bResult = false;
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        wTempLCR = ptThis->ptREG->LSR & (UART_LSR_RDR_MSK | UART_LSR_TEMT_MSK);
        if (UART_LSR_TEMT_MSK & wTempLCR) {
            bResult = true;
        }    
    )
  
    return bResult;
}    

static bool uart_int_enable(uint32_t wUsart, uint32_t wMask)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER |= wMask 
                            & (UART_IER_RBRIE_MSK 
                            | UART_IER_THREIE_MSK 
                            | UART_IER_RXIE_MSK 
                            | UART_IER_ABEOINTEN_MSK 
                            | UART_IER_ABTOINTEN_MSK);
    )

    return true;
}

static bool uart_int_disable(uint32_t wUsart, uint32_t wMask)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER &= ~(wMask 
                            & (UART_IER_RBRIE_MSK 
                            | UART_IER_THREIE_MSK 
                            | UART_IER_RXIE_MSK 
                            | UART_IER_ABEOINTEN_MSK 
                            | UART_IER_ABTOINTEN_MSK));
    )

    return true;
}


/*! \brief setup Baudrate
 *! \param  reading object
 *! \retval false setup baudrate false
 *! \retval true setup baudrate true
 */
static bool uart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate)
{    
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }

//    if (!uart_idle(wUsart)) {
//        return false;
//    }
    
    if (wBaudrate == 0) {
        wBaudrate++;
    }
           
    do {
        uart_reg_t *ptREG = ptThis->ptREG;     
        uint32_t wPclkFrequency = PM.Clock.Peripheral.Get(ptThis->tPCLK);

        wPclkFrequency = (wPclkFrequency >> 4) / wBaudrate;
        __SAFE_CLK_CODE_BEGIN()
            ptREG->LCR |= UART_LCR_DLAB_MSK;
            ptREG->DLM  = wPclkFrequency >> 8;
            ptREG->DLL  = wPclkFrequency & 0xFF;
            ptREG->LCR &= ~UART_LCR_DLAB_MSK;
            ptREG->FDR  = 0x10;   
        __SAFE_CLK_CODE_END()

    } while (0);
    ptThis->wBaundRate = wBaudrate;    

    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t uart_baudrate_get(uint32_t wUsart)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];
     
    if (wUsart >= UART_COUNT) {
        return 0;
    }

    return ptThis->wBaundRate;
}

/*! \brief initialize uart
 *! \param ptUsartCFG uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_config(uint32_t wUsart, const uart_cfg_t *ptUsartCFG)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];

    if ((wUsart >= UART_COUNT) || (NULL == ptUsartCFG)) {
        return false;
    }

    __SAFE_CLK_CODE_BEGIN()
    do { 
        uint16_t hwMode = ptUsartCFG->DataBits | ptUsartCFG->FlowCtrl | ptUsartCFG->Parity | ptUsartCFG->StopBits;
        uart_reg_t *ptREG = ptThis->ptREG; 

        //! read LCR
        uint32_t wTempLCR = ptREG->LCR & ~ 
                    (   UART_LCR_WLS_MSK              |
                        UART_LCR_SBS_MSK              |
                        UART_LCR_PE_MSK               |
                        UART_LCR_PS_MSK               |
                        UART_LCR_BC_MSK               |
                        UART_LCR_DLAB_MSK);
        //! read ACR
        uint32_t wTempACR = ptREG->ACR & ~ 
                    (   UART_ACR_AUTO_BAUD_START_MSK  |
                        UART_ACR_MODE_MSK             |
                        UART_ACR_AUTO_RESTART_MSK     |
                        UART_ACR_ABEOINTCLR_MSK       |
                        UART_ACR_ABTOINTCLR_MSK);

        //! FIFO configuration
        if (!(hwMode & DRIVER_UART_DISABLE_FIFO)) {
            ptREG->FCR =  UART_FCR_FIFO_EN_MSK | UART_FCR_RX_FIFO_RS_MSK | UART_FCR_TX_FIFO_RS_MSK |
                          (0x01u << 6);
        }

        //! parity configuration
        wTempLCR |= hwMode & (UART_LCR_PE_MSK | UART_LCR_PS_MSK);

        //! configure stop bits
        if (hwMode & DRIVER_UART_2_STOPBITS) {
            wTempLCR |= UART_LCR_SBS_MSK;
        }

        //! bit length configuration
        wTempLCR |= UART_LCR_WLS_SET(
            ((hwMode & MODE_LENGTH_MSK) >> MODE_LENGTH_SHIFT));

        //! autobaud configuration 
        if (hwMode & DRIVER_UART_AUTO_BAUD_MODE1) {
            wTempACR |= hwMode & 0x0007;
            ptREG->ACR = wTempACR;
        } else {
            if (!uart_baudrate_set(wUsart, ptUsartCFG->Baudrate)) {
                return false;
            }
        }

        //! update LCR register
        ptREG->LCR = wTempLCR;      
    } while(0);
    __SAFE_CLK_CODE_END()
    
    return true;
}

/*! \brief enable uart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool uart_enable(uint32_t wUsart)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }
    
    //! Enable Peripheral Clock
    peripheral_clock_enable(ptThis->tPCON);
    
    //! Transmit enable
    ptThis->ptREG->TER = (1u << 7); 

    return true;
}    

/*! \brief disable uart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool uart_disable(uint32_t wUsart)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }

    //! Transmit disable
    ptThis->ptREG->TER = (0u << 7); 

    //! Disable Peripheral Clock
    peripheral_clock_disable(ptThis->tPCON);
    
    return true;
}    

/*! \brief write one byte to uart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool uart_write_byte(uint32_t wUsart, uint8_t chByte)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart]; 
    
    if (wUsart >= UART_COUNT) {
        return false;
    }

    if (ptThis->ptREG->FCR & UART_FCR_FIFO_EN_MSK) {
        if(((ptThis->ptREG->FIFOLVL & UART_FIFOLVL_TX_MSK) >> UART_FIFOLVL_TX0) != 0x0f) {
            ptThis->ptREG->THR = chByte;
            return true;
        }
    } else {
        if((ptThis->ptREG->LSR & UART_LSR_THRE_MSK)) {
            ptThis->ptREG->THR = chByte;
            return true;
        }
	}

    return false; 
}    

/*! \brief read byte from uart
 *! \param pchByte reading object
 *! \retval true reading succeed
 *! \retval false reading failed
 */
static bool uart_read_byte(uint32_t wUsart, uint8_t *pchByte)
{
    CLASS(__uart_t) *ptThis = (CLASS(__uart_t) *)&__UART[wUsart];
    
    if ((wUsart >= UART_COUNT) || (NULL == pchByte)) {
        return false;
    }
 
    if (ptThis->ptREG->LSR & UART_LSR_RDR_MSK) {
        *pchByte = ptThis->ptREG->RBR;
        return true;
    }

    return false;
}

MREPEAT(UART_COUNT, __UART_FUNCTION, 0)
