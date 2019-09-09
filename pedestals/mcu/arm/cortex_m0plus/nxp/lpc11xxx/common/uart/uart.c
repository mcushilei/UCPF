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
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define USART0_PCLK_GET()       (main_clock_get())

#define __SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus = PM_AHB_CLK_GET_STATUS(this.AHBClk);          \
        PM_AHB_CLK_ENABLE(this.AHBClk);                                 \
        __VA_ARGS__                                                         \
        PM_AHB_CLK_RESUME(this.AHBClk, tStatus);                        \
    }

#define __UART_INTERFACE(__N, __A)                                      \
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


#define __UART_INTERFACE_PREPARE(__N, __A)             

#define __UART_INTERFACE_DEFINE(__N, __A)                                  \
    bool uart##__N##_init(uart_cfg_t *pConfig)                          \
    {                                                                       \
        return uart_init(__N, pConfig);                                 \
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
    bool uart##__N##_baudrate_set(uint32_t Baudrate)                      \
    {                                                                       \
        return uart_baudrate_set(__N, Baudrate);                          \
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
    extern bool uart##__N##_init(uart_cfg_t *pConfig);                  \
    extern bool uart##__N##_idle(void);                                     \
    extern bool     uart##__N##_baudrate_set(uint32_t Baudrate);          \
    extern uint32_t uart##__N##_baudrate_get(void);                        \
    extern bool uart##__N##_enable(void);                                   \
    extern bool uart##__N##_disable(void);                                  \
    extern bool uart##__N##_write_byte(uint8_t chByte);                     \
    extern bool uart##__N##_read_byte(uint8_t *pchByte);                    \
    extern bool uart##__N##_int_enable(uint32_t wMask);                     \
    extern bool uart##__N##_int_disable(uint32_t wMask);                    \

#define __UART_OBJ(__N, __A)                                               \
    {                                                                       \
        ((void *)(USART##__N##_BASE_ADDRESS)),                        \
        AHBCLK_USART##__N,                                                   \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
#define UART_MODE_BITLENGTH_BIAS        0
#define UART_MODE_BITLENGTH_MASK        BITMASK(3, 0)
#define UART_MODE_PARITY_BIAS           4
#define UART_MODE_PARITY_MASK           BITMASK(7, 4)
#define UART_MODE_STOPBIT_BIAS          8
#define UART_MODE_STOPBIT_MASK          BITMASK(9, 8)
#define UART_MODE_FIFOEN_BIAS           10
#define UART_MODE_FIFOEN_MASK           BIT(10)
#define UART_MODE_AUTOBAUD_BIAS         11
#define UART_MODE_AUTOBAUD_MASK         BIT(11)
    
//! \name uart working mode 
//! @{
typedef enum {
    //! bit 0-3
    UART_5_BIT_LENGTH      = 0u << 0,
    UART_6_BIT_LENGTH      = 1u << 0,
    UART_7_BIT_LENGTH      = 2u << 0,
    UART_8_BIT_LENGTH      = 3u << 0,

    //! bit 4-7
    UART_NO_PARITY         = 0u << 4,
    UART_EVEN_PARITY       = 1u << 4,
    UART_ODD_PARITY        = 2u << 4,
    UART_FORCE_1_PARITY    = 3u << 4,
    UART_FORCE_0_PARITY    = 4u << 4,

    //! bit 8-9
    UART_1_STOPBIT         = 0u << 8,
    UART_2_STOPBIT         = 1u << 8,

    //! bit 10
    UART_ENABLE_FIFO       = 1u << 10,

    //! bit 11
    UART_AUTO_BAUD_ENABLE  = 1u << 11,
} em_uart_mode_t;
//! @}

//! \name uart configuration
//! @{
typedef struct {
    uint32_t            Baudrate;
    uint16_t            Mode;
} uart_cfg_t;
//! @}

//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t Baudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
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

END_DEF_INTERFACE(i_uart_t)
//! @}

//! \name internal class
//! @{
DEF_STRUCTURE(__uart0_t)
    usart0_reg_t *  pReg;
    uint32_t        AHBClk;
END_DEF_STRUCTURE(__uart0_t)

DEF_STRUCTURE(__uart_t)
    usart_reg_t *   pReg;
    uint32_t        AHBClk;
END_DEF_STRUCTURE(__uart_t)

DEF_STRUCTURE(__uart_obj_t)
    void *          pReg;
    uint32_t        AHBClk;
END_DEF_STRUCTURE(__uart_obj_t)
//! @}

/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, __UART_INTERFACE_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/                                               
//! \brief uart interface
const i_uart_t UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal uart object
static __uart_obj_t __UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/

/*! \brief enable uart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool uart0_enable(void)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];

    //! Enable AHB Clock
    PM_AHB_CLK_ENABLE(this.AHBClk);
    //! Enable Peripheral Clock
    SYSCON_REG.USART0CLKDIV = 1u << SCON_USART0CLKDIV_BIAS;
    //! Transmit enable
    this.pReg->TER |= USART_TER_TXEN_MASK; 

    return true;
}    

/*! \brief disable uart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool uart0_disable(void)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];

    //! Transmit disable
    this.pReg->TER &= ~USART_TER_TXEN_MASK; 
    //! Disable AHB Clock
    PM_AHB_CLK_DISABLE(this.AHBClk);
    //! Disable Peripheral Clock
    SYSCON_REG.USART0CLKDIV = 0;
    
    return true;
}    

/*! \brief idle uart
 *! \param void
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool uart0_idle(void)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    uint32_t TempLCR;
    bool bResult = false;
    
    __SAFE_CLK_CODE(
        TempLCR = this.pReg->LSR & (USART_LSR_RDR_MASK | USART_LSR_TEMT_MASK);
        if (USART_LSR_TEMT_MASK & TempLCR) {
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
static bool uart0_baudrate_set(uint32_t Baudrate)
{    
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];

    if (!uart0_idle()) {
        return false;
    }
    
    if (!Baudrate) {
        Baudrate++;
    }
           
    do {
        usart0_reg_t *pReg = this.pReg;     

        __SAFE_CLK_CODE(
            uint32_t wPclkFrequency;
            
            wPclkFrequency = ((USART0_PCLK_GET() >> 4) + (Baudrate >> 1)) / Baudrate;
            pReg->LCR |= USART_LCR_DLAB_MASK;
            pReg->DLM  = wPclkFrequency >> 8;
            pReg->DLL  = wPclkFrequency & 0xFF;
            pReg->LCR &= ~USART_LCR_DLAB_MASK;
            pReg->FDR  = 0x10;
        )
    } while (0);
    
    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t uart0_baudrate_get(void)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    usart0_reg_t *pReg = this.pReg;     
    uint32_t wPclkFrequency;

    __SAFE_CLK_CODE(
        pReg->LCR |= USART_LCR_DLAB_MASK;
        wPclkFrequency = (pReg->DLM << 8) | pReg->DLL;
        pReg->LCR &= ~USART_LCR_DLAB_MASK;
    )

    return (USART0_PCLK_GET() >> 4) / wPclkFrequency;
}

/*! \brief initialize uart
 *! \param pConfig uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart0_init(uart_cfg_t *pConfig)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    bool bResault = false;

    if (NULL == pConfig) {
        return false;
    }

    __SAFE_CLK_CODE (
        do { 
            uint16_t Mode = pConfig->Mode;
            usart0_reg_t *pReg = this.pReg; 
            uint32_t TempLCR = 0;

            //! FIFO configuration
            if (Mode & UART_MODE_FIFOEN_MASK) {
                pReg->FCR = USART_FCR_FIFO_EN_MASK
                          | USART_FCR_RX_FIFO_RS_MASK
                          | USART_FCR_TX_FIFO_RS_MASK;
            }

            //! parity configuration
            switch (Mode & UART_MODE_PARITY_MASK) {
                case UART_NO_PARITY:
                    break;
                case UART_EVEN_PARITY:
                    TempLCR |= USART_LCR_PE_MASK | (1u << USART_LCR_PS_BIAS);
                    break;
                case UART_ODD_PARITY:
                    TempLCR |= USART_LCR_PE_MASK | (0u << USART_LCR_PS_BIAS);
                    break;
                case UART_FORCE_1_PARITY:
                    TempLCR |= USART_LCR_PE_MASK | (2u << USART_LCR_PS_BIAS);
                    break;
                case UART_FORCE_0_PARITY:
                    TempLCR |= USART_LCR_PE_MASK | (3u << USART_LCR_PS_BIAS);
                    break;
                default:
                    goto __ERROR_EXIT;
            }

            //! stop bits
            switch (Mode & UART_MODE_STOPBIT_MASK) {
                case UART_1_STOPBIT:
                    break;
                case UART_2_STOPBIT:
                    TempLCR |= 1u << USART_LCR_SBS_BIAS;
                    break;
                default:
                    goto __ERROR_EXIT;
            }

            //! bit length
            switch (Mode & UART_MODE_BITLENGTH_MASK) {
                case UART_5_BIT_LENGTH:
                    TempLCR |= 0u << USART_LCR_WLS_BIAS;
                    break;
                case UART_6_BIT_LENGTH:
                    TempLCR |= 1u << USART_LCR_WLS_BIAS;
                    break;
                case UART_7_BIT_LENGTH:
                    TempLCR |= 2u << USART_LCR_WLS_BIAS;
                    break;
                case UART_8_BIT_LENGTH:
                    TempLCR |= 3u << USART_LCR_WLS_BIAS;
                    break;
                default:
                    goto __ERROR_EXIT;
            }
            
            //! update LCR register
            pReg->LCR = TempLCR;
            
            //! autobaud configuration 
            if (Mode & UART_AUTO_BAUD_ENABLE) {
                pReg->ACR = 1u << USART_ACR_AUTO_BAUD_START_BIAS;
            } else {
                if (!uart0_baudrate_set(pConfig->Baudrate)) {
                    break;
                }
            }

            bResault = true;
__ERROR_EXIT:
        } while(0);
    )
    
    return bResault;
}

/*! \brief write one byte to uart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool uart0_write_byte(uint8_t chByte)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0]; 
    
    if ((this.pReg->LSR & USART_LSR_THRE_MASK)) {
	    this.pReg->THR = chByte;
        return true;
	}

    return false; 
}    

/*! \brief read byte from uart
 *! \param pchByte reading object
 *! \retval true reading succeed
 *! \retval false reading failed
 */
static bool uart0_read_byte(uint8_t *pchByte)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    
    if (NULL == pchByte) {
        return false;
    }      
 
    if (this.pReg->LSR & USART_LSR_RDR_MASK) {
        *pchByte = this.pReg->RBR;
        return true;
    }

    return false;
}

static bool uart0_int_enable(uint32_t wMask)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    
    __SAFE_CLK_CODE(
        this.pReg->IER |= wMask 
                            & (USART_IER_RBRIE_MASK 
                            | USART_IER_THREIE_MASK 
                            | USART_IER_RXIE_MASK 
                            | USART_IER_ABEOINTEN_MASK 
                            | USART_IER_ABTOINTEN_MASK);
    )

    return true;
}

static bool uart0_int_disable(uint32_t wMask)
{
    __uart0_t *ptThis = (__uart0_t *)&__UART[0];
    
    __SAFE_CLK_CODE(
        this.pReg->IER &= ~(wMask 
                            & (USART_IER_RBRIE_MASK 
                            | USART_IER_THREIE_MASK 
                            | USART_IER_RXIE_MASK 
                            | USART_IER_ABEOINTEN_MASK 
                            | USART_IER_ABTOINTEN_MASK));
    )

    return true;
}






/*! \brief enable uart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool uart_enable(uint32_t uart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];

    //! Enable AHB Clock
    PM_AHB_CLK_ENABLE(this.AHBClk);
    SYSCON_REG.PRESETCTRL &= ~(1u << (SCON_PRESETCTRL_USART1_BIAS + uart - 1));
    SYSCON_REG.PRESETCTRL |= 1u << (SCON_PRESETCTRL_USART1_BIAS + uart - 1);
    //! Enable Peripheral Clock
    scon_usart_clock_enable();
    //! Transmit enable
    this.pReg->CFG |= 1u << USART_CFG_ENABLE_BIAS;

    return true;
}    

/*! \brief disable uart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool uart_disable(uint32_t uart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];

    //! Transmit disable
    this.pReg->CFG &= ~(1u << USART_CFG_ENABLE_BIAS);
    SYSCON_REG.PRESETCTRL &= ~(1u << (SCON_PRESETCTRL_USART1_BIAS + uart - 1));
    //! Disable AHB Clock
    PM_AHB_CLK_DISABLE(this.AHBClk);

    return true;
}    

/*! \brief idle uart
 *! \param void
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool uart_idle(uint32_t uart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    uint32_t RegSTAT;
    bool bResult = false;
    
    __SAFE_CLK_CODE(
        RegSTAT = this.pReg->STAT & (USART_STAT_RXIDLE_MASK | USART_STAT_TXIDLE_MASK);
        if (RegSTAT == (USART_STAT_RXIDLE_MASK | USART_STAT_TXIDLE_MASK)) {
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
static bool uart_baudrate_set(uint32_t uart, uint32_t Baudrate)
{    
    __uart_t *ptThis = (__uart_t *)&__UART[uart];

    if (!uart_idle(uart)) {
        return false;
    }
    
    if (!Baudrate) {
        Baudrate++;
    }
           
    do {
        usart_reg_t *pReg = this.pReg;     
        uint32_t Div;

        __SAFE_CLK_CODE(
            uint32_t RegCFG = pReg->CFG;
            pReg->CFG &= ~(1u << USART_CFG_ENABLE_BIAS);
            Div = ((scon_usart_clock_get() >> 4) + (Baudrate >> 1)) / Baudrate;
            if (Div != 0) {
                Div--;
            }
            pReg->BRG = Div;
            pReg->CFG = RegCFG;
        )
    } while (0);
    
    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t uart_baudrate_get(uint32_t uart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    usart_reg_t *pReg = this.pReg;
    uint32_t Div;

    __SAFE_CLK_CODE(
        Div = pReg->BRG;
    )
    Div++;

    return (scon_usart_clock_get() >> 4) / Div;
}

/*! \brief initialize uart
 *! \param pConfig uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_init(uint32_t uart, uart_cfg_t *pConfig)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    bool bResault = false;

    if (NULL == pConfig) {
        return false;
    }

    __SAFE_CLK_CODE (
        do { 
            uint16_t Mode = pConfig->Mode;
            usart_reg_t *pReg = this.pReg; 
            uint32_t RegCFG = pReg->CFG
                            & (~(USART_CFG_DATALEN_MASK
                               | USART_CFG_PARITYSEL_MASK
                               | USART_CFG_STOPLEN_MASK));

            //! parity configuration
            switch (Mode & UART_MODE_PARITY_MASK) {
                case UART_NO_PARITY:
                    break;
                case UART_EVEN_PARITY:
                    RegCFG |= 2u << USART_CFG_PARITYSEL_BIAS;
                    break;
                case UART_ODD_PARITY:
                    RegCFG |= 3u << USART_CFG_PARITYSEL_BIAS;
                    break;
                case UART_FORCE_1_PARITY:
                case UART_FORCE_0_PARITY:
                default:
                    goto __ERROR_EXIT;
            }

            //! stop bits
            switch (Mode & UART_MODE_STOPBIT_MASK) {
                case UART_1_STOPBIT:
                    break;
                case UART_2_STOPBIT:
                    RegCFG |= 1u << USART_CFG_STOPLEN_BIAS;
                    break;
                default:
                    goto __ERROR_EXIT;
            }

            //! bit length
            switch (Mode & UART_MODE_BITLENGTH_MASK) {
                case UART_7_BIT_LENGTH:
                    break;
                case UART_8_BIT_LENGTH:
                    RegCFG |= 1u << USART_CFG_DATALEN_BIAS;
                    break;
                default:
                    goto __ERROR_EXIT;
            }
            
            //! update LCR register
            pReg->CFG = RegCFG;
            
            //! autobaud configuration 
            if (Mode & UART_AUTO_BAUD_ENABLE) {
                pReg->CTL = 1u << USART_CTL_AUTOBAUD_BIAS;
            } else {
                if (!uart_baudrate_set(uart, pConfig->Baudrate)) {
                    break;
                }
            }

            bResault = true;
__ERROR_EXIT:
        } while(0);
    )
    
    return bResault;
}

/*! \brief write one byte to uart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool uart_write_byte(uint32_t uart, uint8_t chByte)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    
    if (this.pReg->STAT & USART_STAT_TXRDY_MASK) {
	    this.pReg->TXDAT = chByte;
        return true;
	}

    return false; 
}    

/*! \brief read byte from uart
 *! \param pchByte reading object
 *! \retval true reading succeed
 *! \retval false reading failed
 */
static bool uart_read_byte(uint32_t uart, uint8_t *pchByte)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    
    if (NULL == pchByte) {
        return false;
    }      
 
    if (this.pReg->STAT & USART_STAT_RXRDY_MASK) {
        *pchByte = this.pReg->RXDAT;
        return true;
    }

    return false;
}

static bool uart_int_enable(uint32_t uart, uint32_t wMask)
{
    return true;
}

static bool uart_int_disable(uint32_t uart, uint32_t wMask)
{
    return true;
}

__UART_INTERFACE_DEFINE(1, 0)
__UART_INTERFACE_DEFINE(2, 0)
__UART_INTERFACE_DEFINE(3, 0)

