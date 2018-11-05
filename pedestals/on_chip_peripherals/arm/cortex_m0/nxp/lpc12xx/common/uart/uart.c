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
#include ".\reg_uart.h"
#include "..\device.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define MODE_LENGTH_MSK         0x0300
#define MODE_LENGTH_SHIFT       8

#define __SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus = PM_AHB_CLK_GET_STATUS(ptThis->tAHBClk);          \
        uint32_t tPCLK   = PM_PCLK_GET_STATUS(ptThis->tPClk);               \
        PM_AHB_CLK_ENABLE(ptThis->tAHBClk);                                 \
        PM_PCLK_CFG(ptThis->tPClk, uart_calculate_pclk_prescaler());       \
        __VA_ARGS__                                                         \
        PM_AHB_CLK_RESUME(ptThis->tAHBClk, tStatus);                        \
        PM_PCLK_RESUME(ptThis->tPClk, tPCLK);                               \
    }

#   define __UART_INTERFACE(__N, __A)                                      \
    {                                                                       \
        &uart##__N##_init,                                                  \
        &uart##__N##_idle,                                                  \
        &uart##__N##_enable,                                                \
        &uart##__N##_disable,                                               \
        &uart##__N##_write_byte,                                            \
        &uart##__N##_read_byte,                                             \
        {                                                                   \
            &uart##__N##_baudrate_set,                                               \
            &uart##__N##_baudrate_get,                                               \
        },                                                                  \
    },


#   define __UART_INTERFACE_PREPARE(__N, __A)             

#define __UART_INTERFACE_DEFINE(__N, __A)                                  \
    bool uart##__N##_init(uart_cfg_t *ptUsartCFG)                          \
    {                                                                       \
        return uart_init(__N, ptUsartCFG);                                 \
    }                                                                       \
                                                                            \
    bool uart##__N##_idle(void)                                             \
    {                                                                       \
        return uart_idle(__N);                                             \
    }                                                                       \
                                                                            \
    bool uart##__N##_enable(void)                                           \
    {                                                                       \
        return uart_enable(__N);                                           \
    }                                                                       \
                                                                            \
    bool uart##__N##_disable(void)                                          \
    {                                                                       \
        return uart_disable(__N);                                          \
    }                                                                       \
                                                                            \
    bool uart##__N##_write_byte(uint8_t chByte)                             \
    {                                                                       \
        return uart_write_byte(__N, chByte);                               \
    }                                                                       \
                                                                            \
    bool uart##__N##_read_byte(uint8_t *pchByte)                            \
    {                                                                       \
        return uart_read_byte(__N, pchByte);                               \
    }                                                                       \
                                                                            \
    bool uart##__N##_baudrate_set(uint32_t wBaudrate)                      \
    {                                                                       \
        return uart_baudrate_set(__N, wBaudrate);                          \
    }                                                                       \
                                                                            \
    uint32_t uart##__N##_baudrate_get(void)                                \
    {                                                                       \
        return uart_baudrate_get(__N);                                     \
    }                                                                       \
                                                                            \
    bool uart##__N##_int_enable(uint32_t wMask)                             \
    {                                                                       \
        return uart_int_enable(__N, wMask);          \
    }                                                                       \
                                                                            \
    bool uart##__N##_int_disable(uint32_t wMask)                            \
    {                                                                       \
        return uart_int_disable(__N, wMask);         \
    }                                                                       \
                                                                            \
    __UART_INTERFACE_PREPARE(__N, __A)                                     \


#define __UART_INTERFACE_PROTOTYPES(__N, __A)                              \
    extern bool uart##__N##_init(uart_cfg_t *ptUsartCFG);                  \
    extern bool uart##__N##_idle(void);                                     \
    extern bool     uart##__N##_baudrate_set(uint32_t wBaudrate);          \
    extern uint32_t uart##__N##_baudrate_get(void);                        \
    extern bool uart##__N##_enable(void);                                   \
    extern bool uart##__N##_disable(void);                                  \
    extern bool uart##__N##_write_byte(uint8_t chByte);                     \
    extern bool uart##__N##_read_byte(uint8_t *pchByte);                    \
    extern bool uart##__N##_int_enable(uint32_t wMask);                     \
    extern bool uart##__N##_int_disable(uint32_t wMask);                    \
    extern bool uart##__N##_irda_init(uart_IrDA_cfg_t *ptUsartCFG);        \
    extern bool uart##__N##_485_init(uart_485_cfg_t *pt485CFG);            \
    extern bool uart##__N##_485_send_address(uint8_t chAddress);            \

#define __UART_OBJ(__N, __A)                                               \
    {                                                                       \
        ((uart_reg_t *)(UART##__N##_BASE_ADDRESS)),                        \
        AHBCLK_UART##__N,                                                   \
        PCLK_UART##__N,                                                     \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name uart working mode 
//! @{
typedef enum {
    UART_NO_AUTO_BAUD      = 0x00,
    UART_AUTO_BAUD_MODE0   = 0x01,
    UART_AUTO_BAUD_MODE1   = 0x03,
    UART_AUTO_RESTART      = 0x04,
    UART_NO_AUTO_RESTART   = 0x00,

    UART_NO_PARITY         = 0x00,
    UART_EVEN_PARITY       = 0x18,
    UART_ODD_PARITY        = 0x08,
    UART_FORCE_1_PARITY    = 0x28,
    UART_FORCE_0_PARITY    = 0x38,

    UART_1_STOPBIT         = 0x00,
    UART_2_STOPBIT         = 0x40,

    UART_ENABLE_FIFO       = 0x00,
    UART_DISABLE_FIFO      = 0x80,

    UART_5_BIT_LENGTH      = 0x0000,
    UART_6_BIT_LENGTH      = 0x0100,
    UART_7_BIT_LENGTH      = 0x0200,
    UART_8_BIT_LENGTH      = 0x0300,
} em_uart_mode_t;
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} uart_cfg_t;
//! @}

//! \name uart_irda_mode_t
//! @{
typedef enum {
    UART_IRDA_DISABLE           = 0x00,         //!< disable irda
    UART_IRDA_ENABLE            = 0x01,         //!< enable irda
    
    UART_IRDA_NOT_INVERTED      = 0x00,         //!< not inverted the input
    UART_IRDA_INVERTED          = 0x02,         //!< inverted the input
    
    UART_IRDA_FIXPULSE_DISABLE  = 0x00,         //!< disable fixpulse
    UART_IRDA_FIXPULSE_ENABLE   = 0x04,         //!< enable fixpulse
    
    UART_IRDA_PULSEDIV_2        = 0x00,         //!< 2* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_4        = 0x08,         //!< 4* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_8        = 0x10,         //!< 8* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_16       = 0x18,         //!< 16* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_32       = 0x20,         //!< 32* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_64       = 0x28,         //!< 64* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_128      = 0x30,         //!< 128* T_pclk(fixpulse enable)
    UART_IRDA_PULSEDIV_256      = 0x38          //!< 256* T_pclk(fixpulse enable)
} em_uart_irda_mode_t;
//! @}

//! \name uart_IrDA_cfg_t
//! @{
typedef struct {
    uint16_t            hwIrDAMode;            //!< IrDA config word
} uart_IrDA_cfg_t;
//! @}

//! \name uart 485 working mode
//! @{
typedef enum {
    UART_485_DISABLE               = 0x00,
    UART_485_MULTI_STATION         = 0x01,
    UART_485_DISABLE_RX            = 0x02,
    UART_485_AUTO_ADDRESS_MATCH    = 0x04,
    UART_485_USE_DTR_AS_DIR_CTRL   = 0x08, 
    UART_485_ENABLE_AUTO_DIR_CTRL  = 0x10,
    UART_485_DIR_PIN_IDLE_LOW      = 0x20
}em_uart_485_mode_t;
//! @}

//! \name uart 485 configuration
//! @{
typedef struct {
    uint8_t chMode;
    uint8_t chAddress;
    uint8_t chDelay;
} uart_485_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name class: i_uart_485_t
//! @{
DEF_INTERFACE(i_uart_485_t)
    bool        (*Init)(uart_485_cfg_t *ptCFG);
    bool        (*SendAddress)(uint8_t chAddress);
END_DEF_INTERFACE(i_uart_485_t)
//! @}

//! \name class: i_uart_irda_t
//! @{
DEF_INTERFACE(i_uart_irda_t)
    bool        (*Init)(uart_IrDA_cfg_t *ptCFG);
END_DEF_INTERFACE(i_uart_irda_t)
//! @}

//! \name class: i_uart_t
//! @{
DEF_INTERFACE(i_uart_t)
    bool        (*Init)(uart_cfg_t *ptCFG);
    bool        (*Idle)(void);
    bool        (*Enable)(void);
    bool        (*Disable)(void);
    bool        (*WriteByte)(uint8_t chByte);
    bool        (*ReadByte)(uint8_t *pchByte);
    
    i_baudrate_t Baudrate;

#if UART_485_SUPPORT == ENABLED
    i_uart_485_t  RS485;
#endif

#if UART_IRDA_SUPPORT == ENABLED
    i_uart_irda_t IrDA;     
#endif 
END_DEF_INTERFACE(i_uart_t)
//! @}


//! \name internal class
//! @{
DEF_STRUCTURE(__uart_t)
    uart_reg_t *const   ptREG;
    const em_ahb_clk_t  tAHBClk;
    const em_pclk_t     tPClk;
    uint32_t        wBaundRate;
    uint8_t         chAddressState;
END_DEF_STRUCTURE(__uart_t)
//! @}

/*============================ PROTOTYPES ====================================*/
static bool uart_idle(uint32_t wUsart);
static bool uart_init(uint32_t wUsart, uart_cfg_t *ptUsartCFG);
static bool     uart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate);
static uint32_t uart_baudrate_get(uint32_t wUsart);
static bool uart_enable(uint32_t wUsart);
static bool uart_disable(uint32_t wUsart);
static bool uart_write_byte(uint32_t wUsart, uint8_t chByte);
static bool uart_read_byte(uint32_t wUsart, uint8_t *pchByte);
static bool uart_int_enable(uint32_t wUsart, uint32_t wMask);
static bool uart_int_disable(uint32_t wUsart, uint32_t wMask);
static bool uart_irda_init(uint32_t wUsart, uart_IrDA_cfg_t *ptUsartCFG);
static bool uart_485_init(uint32_t wUsart, uart_485_cfg_t *pt485CFG);
static bool uart_485_send_address(uint32_t wUsart, uint8_t chAddress);

MREPEAT(UART_COUNT, __UART_INTERFACE_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/                                               
//! \brief uart object
const i_uart_t UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};


/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal uart object
static __uart_t __UART[] = {
    MREPEAT(UART_COUNT, __UART_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/
static uint8_t uart_calculate_pclk_prescaler(void)
{
    uint32_t wPrescaler = 1;
    uint32_t wMainClock = PM_MAIN_CLK_GET();

    if (wMainClock > MAX_SYSTEM_CLK_FREQ) {
        wPrescaler += (wMainClock - 1) / MAX_SYSTEM_CLK_FREQ;
    }

    return 1;//wPrescaler
}

/*! \brief idle uart
 *! \param void
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool uart_idle(uint32_t wUsart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    uint32_t wTempLCR;
    bool bResult = false;
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        wTempLCR = ptThis->ptREG->LSR.Value & (UART_LSR_RDR_MSK | UART_LSR_TEMT_MSK);
        if (UART_LSR_TEMT_MSK & wTempLCR) {
            bResult = true;
        }    
    )
  
    return bResult;
}    

/*! \brief setup Baudrate
 *! \param  reading object
 *! \retval false setup baudrate false
 *! \retval true setup baudrate true
 */
static bool uart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate)
{    
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }

    if (!uart_idle(wUsart)) {
        return false;
    }
    
    if (!wBaudrate) {
        wBaudrate++;
    }
           
    do {
        uart_reg_t *ptREG = ptThis->ptREG;     

        __SAFE_CLK_CODE(
            uint32_t wPclkFrequency;
            
            wPclkFrequency = ((PM_PCLK_GET(ptThis->tPClk) >> 4) + (wBaudrate >> 1)) / wBaudrate;
            ptREG->LCR.Value |= UART_LCR_DLAB_MSK;
            ptREG->DLM.Value  = wPclkFrequency >> 8;
            ptREG->DLL.Value  = wPclkFrequency & 0xFF;
            ptREG->LCR.Value &= ~UART_LCR_DLAB_MSK;
            ptREG->FDR.Value  = 0x10;
        )
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
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    uart_reg_t *ptREG = ptThis->ptREG;     
    uint32_t wPclkFrequency;

    if (wUsart >= UART_COUNT) {
        return 0;
    }

    __SAFE_CLK_CODE(
        ptREG->LCR.Value |= UART_LCR_DLAB_MSK;
        wPclkFrequency = (ptREG->DLM.Value << 8) | ptREG->DLL.Value;
        ptREG->LCR.Value &= ~UART_LCR_DLAB_MSK;
    )

    return (PM_PCLK_GET(ptThis->tPClk) >> 4) / wPclkFrequency;
}

/*! \brief initialize uart
 *! \param ptUsartCFG uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_init(uint32_t wUsart, uart_cfg_t *ptUsartCFG)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    bool bResault = false;

    if ((wUsart >= UART_COUNT) || (NULL == ptUsartCFG)) {
        return false;
    }

    __SAFE_CLK_CODE (
        do { 
            uint16_t hwMode = ptUsartCFG->hwMode;
            uart_reg_t *ptREG = ptThis->ptREG; 

            //! read LCR
            uint32_t wTempLCR = ptREG->LCR.Value & ~ 
                        (   UART_LCR_WLS_MSK              |
                            UART_LCR_SBS_MSK              |
                            UART_LCR_PE_MSK               |
                            UART_LCR_PS_MSK               |
                            UART_LCR_BC_MSK               |
                            UART_LCR_DLAB_MSK);
            //! read ACR
            uint32_t wTempACR = ptREG->ACR.Value & ~ 
                        (   UART_ACR_AUTO_BAUD_START_MSK  |
                            UART_ACR_MODE_MSK             |
                            UART_ACR_AUTO_RESTART_MSK     |
                            UART_ACR_ABEOINTCLR_MSK       |
                            UART_ACR_ABTOINTCLR_MSK);

            //! FIFO configuration
            if (!(hwMode & UART_DISABLE_FIFO)) {
                ptREG->FCR.Value =  UART_FCR_FIFO_EN_MSK      | 
                                    UART_FCR_RX_FIFO_RS_MSK   |
                                    UART_FCR_TX_FIFO_RS_MSK;
            }

            //! parity configuration
            wTempLCR |= hwMode & (UART_LCR_PE_MSK | UART_LCR_PS_MSK);

            //! configure stop bits
            if (hwMode & UART_2_STOPBIT) {
                wTempLCR |= UART_LCR_SBS_MSK;
            }

            //! bit length configuration
            wTempLCR |= UART_LCR_WLS_SET(
                ((hwMode & MODE_LENGTH_MSK) >> MODE_LENGTH_SHIFT));
            
            //! autobaud configuration 
            if (hwMode & UART_AUTO_BAUD_MODE1) {
                wTempACR |= hwMode & 0x0007;
                ptREG->ACR.Value = wTempACR;
            } else {
                if (!uart_baudrate_set(wUsart, ptUsartCFG->wBaudrate)) {
                    break;
                }
            }

            //! update LCR register
            ptREG->LCR.Value = wTempLCR;
            
            bResault = true;
        } while(0);
    )
    
    return bResault;
}

/*! \brief enable uart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool uart_enable(uint32_t wUsart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }
    
    //! Enable AHB Clock
    PM_AHB_CLK_ENABLE(ptThis->tAHBClk);
    //! Enable Peripheral Clock
    PM_PCLK_CFG(ptThis->tPClk, uart_calculate_pclk_prescaler());
    //! Transmit enable
    ptThis->ptREG->TER.Value |= UART_TER_TXEN_MSK; 

    return true;
}    

/*! \brief disable uart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool uart_disable(uint32_t wUsart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];

    if (wUsart >= UART_COUNT) {
        return false;
    }

    //! Transmit disable
    ptThis->ptREG->TER.Value &= ~UART_TER_TXEN_MSK; 
    //! Disable AHB Clock
    PM_AHB_CLK_DISABLE(ptThis->tAHBClk);
    //! Disable Peripheral Clock
    PM_PCLK_CFG(ptThis->tPClk, 0);
    
    return true;
}    

/*! \brief write one byte to uart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool uart_write_byte(uint32_t wUsart, uint8_t chByte)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart]; 
    
    if (wUsart >= UART_COUNT) {
        return false;
    }    

//    if (ptThis->ptREG->FCR.Value & UART_FCR_FIFO_EN_MSK) {
//        if(((ptThis->ptREG->FIFOLVL.Value & UART_FIFOLVL_TX_MSK) >> UART_FIFOLVL_TX_BIT0) != 0x0fu) {
//            ptThis->ptREG->THR.Value = chByte;
//            return true;
//        }
    //} else 
    if ((ptThis->ptREG->LSR.Value & UART_LSR_THRE_MSK)) {
	    ptThis->ptREG->THR.Value = chByte;
        return true;
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
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT || (NULL == pchByte)) {
        return false;
    }      
 
    if (ptThis->ptREG->LSR.Value & UART_LSR_RDR_MSK) {
        *pchByte = this.ptREG->RBR.Value;
        return true;
    }

    return false;
}

static bool uart_int_enable(uint32_t wUsart, uint32_t wMask)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER.Value |= wMask 
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
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT) {
        return false;
    }
     
    __SAFE_CLK_CODE(
        ptThis->ptREG->IER.Value &= ~(wMask 
                            & (UART_IER_RBRIE_MSK 
                            | UART_IER_THREIE_MSK 
                            | UART_IER_RXIE_MSK 
                            | UART_IER_ABEOINTEN_MSK 
                            | UART_IER_ABTOINTEN_MSK));
    )

    return true;
}

#if UART_IRDA_SUPPORT == ENABLED
/*! \brief uart1_irda_init()
 *! \param irda_cfg_t
 *! \retval true : 
 *! \retval false: 
*/
static bool uart_irda_init(uint32_t wUsart, uart_IrDA_cfg_t *ptUsartCFG)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    
    if (wUsart >= UART_COUNT || (NULL == ptUsartCFG)) {
        return false;
    }

    __SAFE_CLK_CODE(
        ptThis->ptREG->ICR.Value = ptUsartCFG->hwIrDAMode;
    )
    
    return true;
}
#endif

#if UART_485_SUPPORT == ENABLED
/*! \brief initialize  uart_485 
 *! \param pt485CFG uart_485 configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_485_init(uint32_t wUsart, uart_485_cfg_t *pt485CFG)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];

    if (wUsart >= UART_COUNT || (NULL == pt485CFG)) {
        return false;
    }
      
    __SAFE_CLK_CODE(
        do {
            uart_reg_t *ptREG = this.ptREG;
             
            uint32_t wTempRs485CTRL = ptREG->RS485CTRL.Value & ~ 
                                (   UART_RS485CTRL_NMMEN_MSK      |
                                    UART_RS485CTRL_RXDIS_MSK      |
                                    UART_RS485CTRL_AADEN_MSK      |
                                    UART_RS485CTRL_DCTRL_MSK      |
                                    UART_RS485CTRL_OINV_MSK);


            /* UART RS485 Configure */
            wTempRs485CTRL |= pt485CFG->chMode;

            //! update 485 configuration
            ptThis->chAddressState = 0;

            ptREG->RS485CTRL.Value        = wTempRs485CTRL;
            ptREG->RS485ADRMATCH.Value   = pt485CFG->chAddress;
            ptREG->RS485DLY.Value         = pt485CFG->chDelay;

        } while(0);
    )

    return true;
}    
#endif

#define FSM_ADDRESS 0U
#define FSM_END     1U

#if UART_485_SUPPORT == ENABLED
/*! \brief send address to slave device(only uart 485 mode) 
 *! \param chAddress slave device address
 *! \retval fsm_on_going sending address is continuing
 *! \retval fsm_on_cpl sending address have completed
 *! \retval fsm_err sending address occur error
*/
static bool uart_485_send_address(uint32_t wUsart, uint8_t chAddress)
{
    __uart_t *ptThis = (__uart_t *)&__UART[wUsart];
    uart_reg_t *ptREG = this.ptREG;
    
    if (wUsart >= UART_COUNT) {
        return true;
    }
    
    switch (ptThis->chAddressState) {
        case FSM_ADDRESS:
            if((ptThis->ptREG->LSR.Value & UART_LSR_TEMT_MSK)) {
                ptREG->LCR.Value &= ~(UART_LCR_PE_MSK | UART_LCR_PS_MSK);
                ptREG->LCR.Value |=  (UART_LCR_PE_SET(0x01) | UART_LCR_PS_SET(0x02));                
                ptThis->ptREG->THR.Value = chAddress;
                ptThis->chAddressState = FSM_END;   
            }
            break;
            
        case FSM_END:
            if((ptThis->ptREG->LSR.Value & UART_LSR_TEMT_MSK)) {
                ptREG->LCR.Value &= ~(UART_LCR_PE_MSK | UART_LCR_PS_MSK);
                ptREG->LCR.Value |=  (UART_LCR_PE_SET(0x01) | UART_LCR_PS_SET(0x03));                
                ptThis->chAddressState = FSM_ADDRESS;
                return true;
            }
            break;
    }

    return false;
}    
#endif

MREPEAT(UART_COUNT, __UART_INTERFACE_DEFINE, 0)
