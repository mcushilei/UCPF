/*******************************************************************************
 *  Copyright(C)2018-2020 by Dreistein<mcu_shilei@hotmail.com>                *
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
    

#define __UART_INTERFACE(__N, __A)                                      \
    {                                                                       \
        &uart##__N##_init,                                                  \
        &uart##__N##_deinit,                                               \
        &uart##__N##_write_byte,                                            \
        &uart##__N##_read_byte,                                             \
    },


#define __UART_INTERFACE_PREPARE(__N, __A)             

#define __UART_INTERFACE_DEFINE(__N, __A)                                   \
    static bool uart##__N##_init(const uart_cfg_t *pConfig)                      \
    {                                                                       \
        return uart_init(__N, pConfig);                                   \
    }                                                                       \
                                                                            \
    static bool uart##__N##_deinit(void)                                          \
    {                                                                       \
        return uart_deinit(__N);                                          \
    }                                                                       \
                                                                            \
    static bool uart##__N##_write_byte(uint8_t chByte)                             \
    {                                                                       \
        return uart_write_byte(__N, chByte);                               \
    }                                                                       \
                                                                            \
    static bool uart##__N##_read_byte(uint8_t *pchByte)                            \
    {                                                                       \
        return uart_read_byte(__N, pchByte);                               \
    }                                                                       \
                                                                            \
    __UART_INTERFACE_PREPARE(__N, __A)                                     \


#define __UART_INTERFACE_PROTOTYPES(__N, __A)                               \
    static bool uart##__N##_init(const uart_cfg_t *pConfig);              \
    static bool uart##__N##_deinit(void);                                  \
    static bool uart##__N##_write_byte(uint8_t chByte);                     \
    static bool uart##__N##_read_byte(uint8_t *pchByte);                    \

#define __UART_INSTANCE_DEF(__N, __A)                                               \
    {                                                                       \
        ((void *)(USART##__N##_BASE_ADDRESS)),                        \
        CLOCK_UART##__N,                                                   \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct {
    uart_reg_t     *Instance;
    uint32_t        AHBClk;
} uart_instance_t;


/*============================ PROTOTYPES ====================================*/
MREPEAT(UART_COUNT, __UART_INTERFACE_PROTOTYPES, 0)

/*============================ PRIVATE VARIABLES =============================*/
//! \brief internal uart object
static const uart_instance_t __UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INSTANCE_DEF, 0)
};

/*============================ PUBLIC VARIABLES ==============================*/
//! \brief uart interface
const i_uart_t UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/


/**
  * \brief Initialize the UART MSP.
  * \param uart UART instance number.
  * \retval None
  */
WEAK void csp_uart_init(uint32_t uart)
{
  /* Prevent unused argument(s) compilation warning  */
  (void)uart;

  /* NOTE : This function should not be modified, when the callback is needed,
            this function should be implemented in the user file.
   */
}

/**
  * \brief DeInitialize the UART MSP.
  * \param uart UART instance number.
  * \retval None
  */
WEAK void csp_uart_deinit(uint32_t uart)
{
  /* Prevent unused argument(s) compilation warning  */
  (void)uart;

  /* NOTE : This function should not be modified, when the callback is needed,
            this function should be implemented in the user file.
   */
}

/** \brief disable uart
  * \param void
  * \retval true disable succeed
  * \retval false disable failed
  */
static bool uart_deinit(uint32_t uart)
{
    uart_instance_t *ptThis = (uart_instance_t *)&__UART[uart];

    //! Disable Peripheral Clock
    clock_disable(this.AHBClk);
    
    csp_uart_deinit(uart);

    return true;
}    
  

/** \brief setup baudrate
  * \param  reading object
  * \retval false setup baudrate false
  * \retval true setup baudrate true
  */
static bool set_baudrate(uint32_t uart, uint32_t baudrate)
{    
    uart_instance_t *ptThis = (uart_instance_t *)&__UART[uart];
    uint32_t div;

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
        uint32_t cfg = this.Instance->CFG    //!< store register value.
                    & ( (1u << 0)
                      | (3u << 2)
                      | (3u << 4)
                      | (1u << 6)
                      | (1u << 9)
                      | (1u << 11)
                      | (1u << 12)
                      | (1u << 14)
                      | (1u << 15) );
        this.Instance->CFG &= ~(1u << 0);        //!< disable transceiver.
        this.Instance->BRG = div;
        this.Instance->CFG = cfg;            //!< restore register value.
    } while (0);
    
    return true;
}

/** \brief config uart
  * \param pConfig uart configuration object
  * \retval true initialization succeed
  * \retval false initialization failed
  */
static bool uart_init(uint32_t uart, const uart_cfg_t *pConfig)
{
    uart_instance_t *ptThis = (uart_instance_t *)&__UART[uart];
    bool bResault = false;

    if (NULL == pConfig) {
        return false;
    }

    csp_uart_init(uart);
    
    //! Enable Peripheral Clock
    clock_enable(this.AHBClk);
    //! reset peripheral
    sysctrl_periph_reset(BIT(3 + uart));
    
    do { 
        uint16_t mode = pConfig->Mode;
        uint32_t cfg = this.Instance->CFG & (1u << 0);

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
        this.Instance->CFG = cfg;
        
        //! autobaud configuration 
        if (mode & UART_AUTO_BAUD_ENABLE) {
            goto __ERROR_EXIT;
        } else {
            if (!set_baudrate(uart, pConfig->Baudrate)) {
                break;
            }
        }

        //! Transmit enable
        this.Instance->CFG |= 1u << 0;
        bResault = true;
        break;

__ERROR_EXIT:
        //! Disable Peripheral Clock
        clock_disable(this.AHBClk);
        csp_uart_deinit(uart);
    } while (0);
    
    return bResault;
}

/** \brief write one byte to uart
  * \param chByte byte of writing
  * \retval true writing succeed
  * \retval false writing failed
  */
static bool uart_write_byte(uint32_t uart, uint8_t chByte)
{
    uart_instance_t *ptThis = (uart_instance_t *)&__UART[uart];
    
    if (this.Instance->STAT & (1u << 2)) {
	    this.Instance->TXDAT = chByte;
        return true;
	}

    return false; 
}    

/** \brief read byte from uart
  * \param pchByte reading object
  * \retval true reading succeed
  * \retval false reading failed
  */
static bool uart_read_byte(uint32_t uart, uint8_t *pchByte)
{
    uart_instance_t *ptThis = (uart_instance_t *)&__UART[uart];
    
    if (NULL == pchByte) {
        return false;
    }      
 
    if (this.Instance->STAT & (1u << 0)) {
        *pchByte = this.Instance->RXDAT;
        return true;
    }

    return false;
}


MREPEAT(UART_COUNT, __UART_INTERFACE_DEFINE, 0)
