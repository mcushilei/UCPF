/*******************************************************************************
 *  Copyright(C)2018-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "..\sys\pm.h"
#include ".\uart.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define __UART_INTERFACE(__N, __A)                                      \
    {                                                                       \
        &uart##__N##_config,                                                  \
        &uart##__N##_enable,                                                \
        &uart##__N##_disable,                                               \
        &uart##__N##_open,                                                \
        &uart##__N##_close,                                               \
        &uart##__N##_write_byte,                                            \
        &uart##__N##_read_byte,                                             \
    },


#define __UART_INTERFACE_DEFINE(__N, __A)                                   \
    bool uart##__N##_config(const uart_cfg_t *pConfig)                      \
    {                                                                       \
        return uart_config(__N, pConfig);                                   \
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
    bool uart##__N##_open(void)                                           \
    {                                                                       \
        return uart_open(__N);                                           \
    }                                                                       \
                                                                            \
    bool uart##__N##_close(void)                                          \
    {                                                                       \
        return uart_close(__N);                                          \
    }                                                                       \
                                                                            \
    bool uart##__N##_write_byte(uint8_t data)                             \
    {                                                                       \
        return uart_write_byte(__N, data);                               \
    }                                                                       \
                                                                            \
    bool uart##__N##_read_byte(uint8_t *pData)                            \
    {                                                                       \
        return uart_read_byte(__N, pData);                               \
    }                                                                       \
                                                                            \
    bool uart##__N##_int_enable(uint32_t wMask)                             \
    {                                                                       \
        return uart_int_enable(__N, wMask);          \
    }                                                                       \
    bool uart##__N##_int_disable(uint32_t wMask)                            \
    {                                                                       \
        return uart_int_disable(__N, wMask);         \
    }                                                                       \


#define __UART_INTERFACE_PROTOTYPES(__N, __A)                               \
    extern bool uart##__N##_config(const uart_cfg_t *pConfig);              \
    extern bool uart##__N##_enable(void);                                   \
    extern bool uart##__N##_disable(void);                                  \
    extern bool uart##__N##_open(void);                                   \
    extern bool uart##__N##_close(void);                                  \
    extern bool uart##__N##_write_byte(uint8_t data);                     \
    extern bool uart##__N##_read_byte(uint8_t *pData);                    \
    extern bool uart##__N##_int_enable(uint32_t wMask);                     \
    extern bool uart##__N##_int_disable(uint32_t wMask);                    \

#define __UART_OBJ(__N, __A)                                               \
    {                                                                       \
        ((uart_reg_t *)(REG_UART##__N##_BASEADDR)),                        \
        DRIVER_PWRCTRL_UART##__N,                                           \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

DEF_STRUCTURE(__uart_obj_t)
    uart_reg_t     *pReg;
    uint32_t        ClkEN;
END_DEF_STRUCTURE(__uart_obj_t)


/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, __UART_INTERFACE_PROTOTYPES, 0)

/*============================ PRIVATE VARIABLES =============================*/
//! \brief internal uart object
static __uart_obj_t __UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_OBJ, 0)
};

/*============================ PUBLIC VARIABLES ==============================*/
//! \brief uart interface
const i_uart_t UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief enable uart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool uart_enable(uint32_t uart)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];

    driver_pwrctrl_periph_enable(this.ClkEN);

    //
    // Set CLKGEN.UARTEN, clear the field then write the desired enable value
    // Valid enable values are DIS, EN, REDUCE_FREQ, EN_POWER_SAV.
    //
    driver_clkgen_uarten_set(uart, DRIVER_CLKGEN_UARTEN_EN);

    //
    // Enable the UART clock.
    //
    this.pReg->CR |= REG_UART_CR_CLKEN_M;

    //
    // Select default UART clock source
    //
    this.pReg->CR |= REG_UART_CR_CLKSEL_24MHZ;

    return true;
}    

/*! \brief disable uart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool uart_disable(uint32_t uart)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];
    
    //
    // Disable the UART clock.
    //
    this.pReg->CR &= ~REG_UART_CR_CLKEN_M;

    //
    // Disable the UART clock in the CLKGEN module.
    //
    driver_clkgen_uarten_set(uart, DRIVER_CLKGEN_UARTEN_DIS);

    driver_pwrctrl_periph_disable(this.ClkEN);

    return true;
}

static bool uart_open(uint32_t uart)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];

    this.pReg->CR |= REG_UART_CR_UARTEN_M   |
                     REG_UART_CR_RXE_M      |
                     REG_UART_CR_TXE_M;
    return true;
}

static bool uart_close(uint32_t uart)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];

    this.pReg->CR &= ~(REG_UART_CR_UARTEN_M   |
                       REG_UART_CR_RXE_M      |
                       REG_UART_CR_TXE_M);
    return true;
}

//*****************************************************************************
//
// Set Baud Rate based on the UART clock frequency.
//
//*****************************************************************************
static bool uart_baudrate_set(uint32_t uart, uint32_t baudrate)
{    
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];
    uint32_t ui32Intermediate;
    uint32_t ui32IntegerDivisor;
    uint32_t ui32FractionDivisor;
    uint32_t ui32BaudClk;
    uint32_t ui32UartClkFreq;
    
    
    ui32UartClkFreq = driver_clkgen_sysclk_get();
    
    //
    // Calculate register values.
    //
    ui32BaudClk         = 32u * baudrate;
    ui32IntegerDivisor  = ui32UartClkFreq / ui32BaudClk;
    ui32Intermediate    = ui32UartClkFreq * 64u / ui32BaudClk;
    ui32FractionDivisor = ui32Intermediate - (ui32IntegerDivisor * 64u);

    //
    // Write the UART regs.
    //
    this.pReg->IBRD = ui32IntegerDivisor;
    this.pReg->FBRD = ui32FractionDivisor;
    
    return true;
}


//*****************************************************************************
//
//! @name UART FIFO Levels
//! @brief Macro definitions for RTV interrupt status bits.
//!
//! These macros correspond to the bits in the UART interrupt status register.
//! They may be used with any of the \e am_hal_uart_int_x() functions.
//!
//! @{
//
//*****************************************************************************
//TX
#define DRIVER_UART_TX_FIFO_1_8             REG_UART_IFLS_TXIFLSEL(0)
#define DRIVER_UART_TX_FIFO_1_4             REG_UART_IFLS_TXIFLSEL(1)
#define DRIVER_UART_TX_FIFO_1_2             REG_UART_IFLS_TXIFLSEL(2)
#define DRIVER_UART_TX_FIFO_3_4             REG_UART_IFLS_TXIFLSEL(3)
#define DRIVER_UART_TX_FIFO_7_8             REG_UART_IFLS_TXIFLSEL(4)
// RX
#define DRIVER_UART_RX_FIFO_1_8             REG_UART_IFLS_RXIFLSEL(0)
#define DRIVER_UART_RX_FIFO_1_4             REG_UART_IFLS_RXIFLSEL(1)
#define DRIVER_UART_RX_FIFO_1_2             REG_UART_IFLS_RXIFLSEL(2)
#define DRIVER_UART_RX_FIFO_3_4             REG_UART_IFLS_RXIFLSEL(3)
#define DRIVER_UART_RX_FIFO_7_8             REG_UART_IFLS_RXIFLSEL(4)
//! @}


/*! \brief config uart
 *! \param pConfig uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_config(uint32_t uart, const uart_cfg_t *pConfig)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];
    bool bResault = false;

    if (NULL == pConfig) {
        return false;
    }

    do { 
        uint32_t cfg = REG_UART_LCRH_FEN_M;//

        //! autobaud configuration 
        if (!uart_baudrate_set(uart, pConfig->Baudrate)) {
            break;
        }

        //! parity
        cfg |= pConfig->Parity;

        //! stop bits
        cfg |= pConfig->StopBits;

        //! bit length
        cfg |= pConfig->DataBits;
        
        //! update LCRH register
        this.pReg->LCRH = cfg;
        
        //
        // Set the clock select field for 24MHz from the HFRC
        //
        this.pReg->CR |= REG_UART_CR_CLKSEL_24MHZ;
        
        //
        // Write the FIFO level register.
        //
        this.pReg->IFLS = DRIVER_UART_TX_FIFO_1_2 | DRIVER_UART_RX_FIFO_1_2;
        
        bResault = true;
    } while (0);
    
    return bResault;
}

/*! \brief write one byte to uart
 *! \param data byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool uart_write_byte(uint32_t uart, uint8_t data)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];
    
    if ( this.pReg->FR & REG_BFVM(UART, FR, TXFF) ) {
        return false; 
	}

    this.pReg->DR = data;
    return true;
}    

/*! \brief read byte from uart
 *! \param pData reading object
 *! \retval true reading succeed
 *! \retval false reading failed
 */
static bool uart_read_byte(uint32_t uart, uint8_t *pData)
{
    __uart_obj_t *ptThis = (__uart_obj_t *)&__UART[uart];
    
    if (NULL == pData) {
        return false;
    }      
 
    if ( this.pReg->FR & REG_BFVM(UART, FR, RXFE) ) {
        return false;
    }

    *pData = this.pReg->DR;
    return true;
}

static bool uart_int_enable(uint32_t uart, uint32_t wMask)
{
    return true;
}

static bool uart_int_disable(uint32_t uart, uint32_t wMask)
{
    return true;
}

MREPEAT(UART_COUNT, __UART_INTERFACE_DEFINE, 0)
