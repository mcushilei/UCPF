/*******************************************************************************
 *  Copyright(C)2018 by Dreistein<mcu_shilei@hotmail.com>                     *
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

//! Do not move this pre-processor statement to other places
#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_UART_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\uart_public.h"
#include "..\sys\pm.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

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
    

#define __SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus = clock_get_status(this.AHBClk);           \
        clock_enable(this.AHBClk);                                  \
        do {__VA_ARGS__} while (0);                                 \
        clock_resume_status(this.AHBClk, tStatus);                  \
    }

#define __UART_INTERFACE(__N, __A)                                      \
    {                                                                       \
        &uart##__N##_config,                                                  \
        &uart##__N##_idle,                                                  \
        &uart##__N##_enable,                                                \
        &uart##__N##_disable,                                               \
        &uart##__N##_write_byte,                                            \
        &uart##__N##_read_byte,                                             \
    },


#define __UART_INTERFACE_PREPARE(__N, __A)             

#define __UART_INTERFACE_DEFINE(__N, __A)                                   \
    bool uart##__N##_config(const uart_cfg_t *pConfig)                      \
    {                                                                       \
        return uart_config(__N, pConfig);                                   \
    }                                                                       \
                                                                            \
    bool uart##__N##_idle(void)                                             \
    {                                                                       \
        return uart_is_idle(__N);                                             \
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


#define __UART_INTERFACE_PROTOTYPES(__N, __A)                               \
    extern bool uart##__N##_config(const uart_cfg_t *pConfig);              \
    extern bool uart##__N##_idle(void);                                     \
    extern bool uart##__N##_enable(void);                                   \
    extern bool uart##__N##_disable(void);                                  \
    extern bool uart##__N##_write_byte(uint8_t chByte);                     \
    extern bool uart##__N##_read_byte(uint8_t *pchByte);                    \
    extern bool uart##__N##_int_enable(uint32_t wMask);                     \
    extern bool uart##__N##_int_disable(uint32_t wMask);                    \

#define __UART_OBJ(__N, __A)                                               \
    {                                                                       \
        ((void *)(USART##__N##_BASE_ADDRESS)),                        \
        CLOCK_UART##__N,                                                   \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

DEF_STRUCTURE(__uart_t)
    uart_reg_t     *pReg;
    uint32_t        AHBClk;
END_DEF_STRUCTURE(__uart_t)

DEF_STRUCTURE(__uart_obj_t)
    void           *pReg;
    uint32_t        AHBClk;
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
    __uart_t *ptThis = (__uart_t *)&__UART[uart];

    //! Enable Peripheral Clock
    clock_enable(this.AHBClk);
    //! reset peripheral
    SYSCON_REG.PRESETCTRL &= ~BIT(3 + uart);
    SYSCON_REG.PRESETCTRL |=  BIT(3 + uart);
    //! Transmit enable
    this.pReg->CFG |= 1u << 0;

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

    //! Disable Peripheral Clock
    clock_disable(this.AHBClk);

    return true;
}    

/*! \brief idle uart
 *! \param void
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool uart_is_idle(uint32_t uart)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    uint32_t regSTAT;
    bool bResult = false;
    
    __SAFE_CLK_CODE(
        regSTAT = this.pReg->STAT & ((1u << 1)
                                   | (1u << 3));
        if (regSTAT == ((1u << 1) | (1u << 3))) {
            bResult = true;
        }
    )
  
    return bResult;
}    

/*! \brief setup baudrate
 *! \param  reading object
 *! \retval false setup baudrate false
 *! \retval true setup baudrate true
 */
static bool uart_baudrate_set(uint32_t uart, uint32_t baudrate)
{    
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    uint32_t div;

//    if (!uart_is_idle(uart)) {
//        return false;
//    }
    
    if (baudrate == 0u) {
        return false;
    }
    div = ((uart_clock_get() >> 4) + (baudrate >> 1)) / baudrate;
    if (div > 65536u) {
        return false;
    }
    if (div != 0) {
        div--;
    }

           
    do {
        uart_reg_t *pReg = this.pReg;     
        __SAFE_CLK_CODE(
            uint32_t cfg = pReg->CFG    //!< store register value.
                        & ( (1u << 0)
                          | (3u << 2)
                          | (3u << 4)
                          | (1u << 6)
                          | (1u << 9)
                          | (1u << 11)
                          | (1u << 12)
                          | (1u << 14)
                          | (1u << 15) );
            pReg->CFG &= ~(1u << 0);        //!< disable transceiver.
            pReg->BRG = div;
            pReg->CFG = cfg;            //!< restore register value.
        )
    } while (0);
    
    return true;
}

/*! \brief config uart
 *! \param pConfig uart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool uart_config(uint32_t uart, const uart_cfg_t *pConfig)
{
    __uart_t *ptThis = (__uart_t *)&__UART[uart];
    bool bResault = false;

    if (NULL == pConfig) {
        return false;
    }

    __SAFE_CLK_CODE (
        do { 
            uint16_t mode = pConfig->Mode;
            uart_reg_t *pReg = this.pReg; 
            uint32_t cfg = pReg->CFG & (1u << 0);

            //! parity configuration
            switch (mode & UART_MODE_PARITY_MASK) {
                case UART_NO_PARITY:
                    break;
                case UART_EVEN_PARITY:
                    cfg |= 2u << 4;
                    break;
                case UART_ODD_PARITY:
                    cfg |= 3u << 4;
                    break;
                case UART_FORCE_1_PARITY:
                case UART_FORCE_0_PARITY:
                default:
                    goto __ERROR_EXIT;
            }

            //! stop bits
            switch (mode & UART_MODE_STOPBIT_MASK) {
                case UART_1_STOPBIT:
                    break;
                case UART_2_STOPBIT:
                    cfg |= 1u << 6;
                    break;
                default:
                    goto __ERROR_EXIT;
            }

            //! bit length
            switch (mode & UART_MODE_BITLENGTH_MASK) {
                case UART_7_BIT_LENGTH:
                    break;
                case UART_8_BIT_LENGTH:
                    cfg |= 1u << 2;
                    break;
                default:
                    goto __ERROR_EXIT;
            }
            
            //! update CFG register
            pReg->CFG = cfg;
            
            //! autobaud configuration 
            if (mode & UART_AUTO_BAUD_ENABLE) {
                goto __ERROR_EXIT;
            } else {
                if (!uart_baudrate_set(uart, pConfig->Baudrate)) {
                    break;
                }
            }

            bResault = true;
__ERROR_EXIT:
        } while (0);
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
    
    if (this.pReg->STAT & (1u << 2)) {
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
 
    if (this.pReg->STAT & (1u << 0)) {
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

MREPEAT(UART_COUNT, __UART_INTERFACE_DEFINE, 0)
