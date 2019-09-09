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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_usart.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define MODE_LENGTH_MSK         0x0300
#define MODE_LENGTH_SHIFT       8

#define __SAFE_CLK_CODE(...)                          \
    {                                               \
        uint32_t wStatus = PM.Power.GetStatus(ptThis->tPCON);   \
        PM.Power.Enable(ptThis->tPCON);             \
        __VA_ARGS__;                               \
        PM.Power.Resume(ptThis->tPCON, wStatus);    \
    }

#define __USART_INTERFACE(__N, __A)                                           \
    {                                                                       \
        &usart##__N##_init,                                                  \
        &usart##__N##_idle,                                                  \
        &usart##__N##_enable,                                                \
        &usart##__N##_disable,                                               \
        &usart##__N##_write_byte,                                            \
        &usart##__N##_read_byte,                                             \
        {                                                                   \
            &usart##__N##_baudrate_set,                                     \
            &usart##__N##_baudrate_get,                                     \
        },                                                                  \
    },


#define __USART_FUNCTION(__N, __A)                                             \
    bool usart##__N##_init(usart_cfg_t *ptUsartCFG)                          \
    {                                                                       \
        return usart_init(__N, ptUsartCFG);           \
    }                                                                       \
                                                                            \
    bool usart##__N##_idle(void)                                             \
    {                                                                       \
        return usart_idle(__N);                       \
    }                                                                       \
                                                                            \
    bool usart##__N##_enable(void)                                           \
    {                                                                       \
        return usart_enable(__N);                     \
    }                                                                       \
                                                                            \
    bool usart##__N##_disable(void)                                          \
    {                                                                       \
        return usart_disable(__N);                    \
    }                                                                       \
                                                                            \
    bool usart##__N##_write_byte(uint8_t chByte)                             \
    {                                                                       \
        return usart_write_byte(__N, chByte);         \
    }                                                                       \
                                                                            \
    bool usart##__N##_read_byte(uint8_t *pchByte)                            \
    {                                                                       \
        return usart_read_byte(__N, pchByte);         \
    }                                                                       \
                                                                            \
    bool usart##__N##_baudrate_set(uint32_t wBaudrate)                                \
    {                                                                       \
        return usart_baudrate_set(__N, wBaudrate);             \
    }                                                                       \
                                                                            \
    uint32_t usart##__N##_baudrate_get(void)                                          \
    {                                                                       \
        return usart_baudrate_get(__N);                        \
    }                                                                       \
                                                                            \
    bool usart##__N##_int_enable(uint32_t wMask)                             \
    {                                                                       \
        return usart_int_enable(__N, wMask);          \
    }                                                                       \
                                                                            \
    bool usart##__N##_int_disable(uint32_t wMask)                            \
    {                                                                       \
        return usart_int_disable(__N, wMask);         \
    }


#define __USART_FUNCTION_PROTOTYPES(__N, __A)                                             \
    extern bool usart##__N##_init(usart_cfg_t *ptUsartCFG);                  \
    extern bool usart##__N##_idle(void);                                     \
    extern bool usart##__N##_enable(void);                                   \
    extern bool usart##__N##_disable(void);                                  \
    extern bool usart##__N##_write_byte(uint8_t chByte);                     \
    extern bool usart##__N##_read_byte(uint8_t *pchByte);                    \
    extern bool usart##__N##_baudrate_set(uint32_t wBaudrate);              \
    extern uint32_t usart##__N##_baudrate_get(void);                        \
    extern bool usart##__N##_int_enable(uint32_t wMask);                     \
    extern bool usart##__N##_int_disable(uint32_t wMask);                    \
    
#define __USART_OBJ(__N, __A)                                            \
    {                                                                       \
        ((uart_reg_t *)(UART##__N##_BASE)),                        \
        PCONP_UART##__N,                                                   \
        PCLK_UART##__N,                                                   \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name usart working mode 
//! @{
typedef enum {
    USART_NO__AUTO_BAUD     = 0x00,
    USART_AUTO_BAUD_MODE0   = 0x01,
    USART_AUTO_BAUD_MODE1   = 0x03,
    USART_AUTO_RESTART      = 0x04,
    USART_NO__AUTO_RESTART  = 0x00,

    USART_NO_PARITY         = 0x00,
    USART_EVEN_PARITY       = 0x18,
    USART_ODD_PARITY        = 0x08,
    USART_FORCE_1_PARITY    = 0x28,
    USART_FORCE_0_PARITY    = 0x38,

    USART_1_STOPBIT         = 0x00,
    USART_2_STOPBIT         = 0x40,

    USART_ENABLE_FIFO       = 0x00,
    USART_DISABLE_FIFO      = 0x80,

    USART_5_BIT_LENGTH      = 0x0000,
    USART_6_BIT_LENGTH      = 0x0100,
    USART_7_BIT_LENGTH      = 0x0200,
    USART_8_BIT_LENGTH      = 0x0300,
       
    USART_SYNC_MODE         = 0x0400,
}em_usart_mode_t;
//! @}

//! \name usart configuration
//! @{
typedef struct {
    uint16_t        hwMode;
    uint32_t        wBaudrate;
} usart_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name i_usart_t
//! @{
DEF_INTERFACE(i_usart_t)
    bool            (*Init)(usart_cfg_t *ptCFG);
    bool            (*Idle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;
END_DEF_INTERFACE(i_usart_t)
//! @}

//! \name internal class
//! @{
DEF_CLASS(__usart_t)
    uart_reg_t *const   ptREG;
    const em_pconp_t    tPCON;
    const em_pclksel_t  tPCLK;
    uint32_t            wBaundRate;
END_DEF_CLASS(__usart_t)
//! @}

/*============================ PROTOTYPES ====================================*/
static bool usart_idle(uint32_t wUsart);
static bool usart_init(uint32_t wUsart, usart_cfg_t *ptUsartCFG);
static bool     usart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate);
static uint32_t usart_baudrate_get(uint32_t wUsart);
static bool usart_enable(uint32_t wUsart);
static bool usart_disable(uint32_t wUsart);
static bool usart_write_byte(uint32_t wUsart, uint8_t chByte);
static bool usart_read_byte(uint32_t wUsart, uint8_t *pchByte);
static bool usart_int_enable(uint32_t wUsart, uint32_t wMask);
static bool usart_int_disable(uint32_t wUsart, uint32_t wMask);

MREPEAT(USART_COUNT, __USART_FUNCTION_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/                                               
//! \brief usart object
const i_usart_t USART[] = {
    MREPEAT(USART_COUNT, __USART_INTERFACE, 0)
};

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal usart object
static CLASS(__usart_t) __USART[] = {
    MREPEAT(USART_COUNT, __USART_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief to check whether usart is idle.
 *! \param wUsart wich usart to check
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool usart_idle(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    uint32_t wTempLCR;
    bool bResult = false;
    
    if (wUsart >= USART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        wTempLCR = ptThis->ptREG->LSR & (USART_LSR_RDR_MSK | USART_LSR_TEMT_MSK);
        if (USART_LSR_TEMT_MSK & wTempLCR) {
            bResult = true;
        }    
    )
  
    return bResult;
}    

static bool usart_int_enable(uint32_t wUsart, uint32_t wMask)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    
    if (wUsart >= USART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER |= wMask 
                            & (USART_IER_RBRIE_MSK 
                            | USART_IER_THREIE_MSK 
                            | USART_IER_RXIE_MSK 
                            | USART_IER_ABEOINTEN_MSK 
                            | USART_IER_ABTOINTEN_MSK);
    )

    return true;
}

static bool usart_int_disable(uint32_t wUsart, uint32_t wMask)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    
    if (wUsart >= USART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER &= ~(wMask 
                            & (USART_IER_RBRIE_MSK 
                            | USART_IER_THREIE_MSK 
                            | USART_IER_RXIE_MSK 
                            | USART_IER_ABEOINTEN_MSK 
                            | USART_IER_ABTOINTEN_MSK));
    )

    return true;
}


/*! \brief setup Baudrate
 *! \param  reading object
 *! \retval false setup baudrate false
 *! \retval true setup baudrate true
 */
static bool usart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate)
{    
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if (wUsart >= USART_COUNT) {
        return false;
    }

    if (!usart_idle(wUsart)) {
        return false;
    }
    
    if (!wBaudrate) {
        wBaudrate++;
    }
           
    do {
        uart_reg_t *ptREG = ptThis->ptREG;     
        uint32_t wPclkFrequency = PM.Clock.Peripheral.Get(ptThis->tPCLK);

        wPclkFrequency = (wPclkFrequency >> 4) / wBaudrate;
        __SAFE_CLK_CODE(
            ptREG->LCR |= USART_LCR_DLAB_MSK;
            ptREG->DLM  = wPclkFrequency >> 8;
            ptREG->DLL  = wPclkFrequency & 0xFF;
            ptREG->LCR &= ~USART_LCR_DLAB_MSK;
            ptREG->FDR  = 0x10;   
        )

    } while (0);
    ptThis->wBaundRate = wBaudrate;    

    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t usart_baudrate_get(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
     
    if (wUsart >= USART_COUNT) {
        return 0;
    }

    return ptThis->wBaundRate;
}

/*! \brief initialize usart
 *! \param ptUsartCFG usart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool usart_init(uint32_t wUsart, usart_cfg_t *ptUsartCFG)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if ((wUsart >= USART_COUNT) || (NULL == ptUsartCFG)) {
        return false;
    }

    __SAFE_CLK_CODE (
        do { 
            uint16_t hwMode = ptUsartCFG->hwMode;
            uart_reg_t *ptREG = ptThis->ptREG; 

            //! read LCR
            uint32_t wTempLCR = ptREG->LCR & ~ 
                        (   USART_LCR_WLS_MSK              |
                            USART_LCR_SBS_MSK              |
                            USART_LCR_PE_MSK               |
                            USART_LCR_PS_MSK               |
                            USART_LCR_BC_MSK               |
                            USART_LCR_DLAB_MSK);
            //! read ACR
            uint32_t wTempACR = ptREG->ACR & ~ 
                        (   USART_ACR_AUTO_BAUD_START_MSK  |
                            USART_ACR_MODE_MSK             |
                            USART_ACR_AUTO_RESTART_MSK     |
                            USART_ACR_ABEOINTCLR_MSK       |
                            USART_ACR_ABTOINTCLR_MSK);

            //! FIFO configuration
            if (!(hwMode & USART_DISABLE_FIFO)) {
                ptREG->FCR =  USART_FCR_FIFO_EN_MSK      | 
                                            USART_FCR_RX_FIFO_RS_MSK   |
                                            USART_FCR_TX_FIFO_RS_MSK;
            }

            //! parity configuration
            wTempLCR |= hwMode & (USART_LCR_PE_MSK | USART_LCR_PS_MSK);

            //! configure stop bits
            if (hwMode & USART_2_STOPBIT) {
                wTempLCR |= USART_LCR_SBS_MSK;
            }

            //! bit length configuration
            wTempLCR |= USART_LCR_WLS_SET(
                ((hwMode & MODE_LENGTH_MSK) >> MODE_LENGTH_SHIFT));

            //! autobaud configuration 
            if (hwMode & USART_AUTO_BAUD_MODE1) {
                wTempACR |= hwMode & 0x0007;
                ptREG->ACR = wTempACR;
            } else {
                if (!usart_baudrate_set(wUsart, ptUsartCFG->wBaudrate)) {
                    return false;
                }
            }

            //! update LCR register
            ptREG->LCR = wTempLCR;      
        } while(0);
    )
    
    return true;
}

/*! \brief enable usart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool usart_enable(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if (wUsart >= USART_COUNT) {
        return false;
    }
    
    //! Enable Peripheral Clock
    PM.Power.Enable(ptThis->tPCON);
    //! Transmit enable
    ptThis->ptREG->TER |= USART_TER_TXEN_MSK; 

    return true;
}    

/*! \brief disable usart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool usart_disable(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if (wUsart >= USART_COUNT) {
        return false;
    }

    //! Transmit disable
    ptThis->ptREG->TER &= ~USART_TER_TXEN_MSK; 

    //! Disable Peripheral Clock
    PM.Power.Disable(ptThis->tPCON);
    
    return true;
}    

/*! \brief write one byte to usart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool usart_write_byte(uint32_t wUsart, uint8_t chByte)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart]; 
    
    if (wUsart >= USART_COUNT) {
        return false;
    }

    if (ptThis->ptREG->FCR & USART_FCR_FIFO_EN_MSK) {
        if(((ptThis->ptREG->FIFOLVL & USART_FIFOLVL_TX_MSK) >> USART_FIFOLVL_TX0) != 0x0f) {
            ptThis->ptREG->THR = chByte;
            return true;
        }
    } else {
        if((ptThis->ptREG->LSR & USART_LSR_THRE_MSK)) {
            ptThis->ptREG->THR = chByte;
            return true;
        }
	}

    return false; 
}    

/*! \brief read byte from usart
 *! \param pchByte reading object
 *! \retval true reading succeed
 *! \retval false reading failed
 */
static bool usart_read_byte(uint32_t wUsart, uint8_t *pchByte)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    
    if ((wUsart >= USART_COUNT) || (NULL == pchByte)) {
        return false;
    }
 
    if (ptThis->ptREG->LSR & USART_LSR_RDR_MSK) {
        *pchByte = ptThis->ptREG->RBR;
        return true;
    }

    return false;
}

MREPEAT(USART_COUNT, __USART_FUNCTION, 0)
