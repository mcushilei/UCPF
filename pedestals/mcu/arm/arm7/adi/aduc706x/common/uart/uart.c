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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_UART_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_uart.h"
#include "..\device.h"
#include "..\sys\pm.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)    

#define MODE_LENGTH_MSK         0x0300
#define MODE_LENGTH_SHIFT       8

#define __SAFE_CLK_CODE(...)  \
    {\
        uint32_t tStatus = peripheral_power_status_get(ptThis->tPower);          \
        peripheral_power_enable(ptThis->tPower);                                 \
        __VA_ARGS__;                                                        \
        peripheral_power_status_resume(ptThis->tPower, tStatus);                        \
    }

#   define __UART_INTERFACE(__N, __A)                                      \
    {                                                                       \
        &usart##__N##_config,                                                  \
        &usart##__N##_idle,                                                  \
        &usart##__N##_enable,                                                \
        &usart##__N##_disable,                                               \
        &usart##__N##_write_byte,                                            \
        &usart##__N##_read_byte,                                             \
        {                                                                   \
            &usart##__N##_baudrate_set,                                               \
            &usart##__N##_baudrate_get,                                               \
        },                                                                  \
    },  
    
#define __UART_FUNCTION(__N, __A)                                             \
    bool usart##__N##_config(usart_cfg_t *ptUsartCFG)                          \
    {                                                                       \
        return usart_config(__N, ptUsartCFG);           \
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

#define __UART_FUNCTION_PROTOTYPES(__N, __A)                                             \
    extern bool usart##__N##_config(usart_cfg_t *ptUsartCFG);                  \
    extern bool usart##__N##_idle(void);                                     \
    extern bool usart##__N##_enable(void);                                   \
    extern bool usart##__N##_disable(void);                                  \
    extern bool usart##__N##_write_byte(uint8_t chByte);                     \
    extern bool usart##__N##_read_byte(uint8_t *pchByte);                    \
    extern bool usart##__N##_baudrate_set(uint32_t wBaudrate);              \
    extern uint32_t usart##__N##_baudrate_get(void);                        \
    extern bool usart##__N##_int_enable(uint32_t wMask);                     \
    extern bool usart##__N##_int_disable(uint32_t wMask);                    \

#define __UART_OBJ(__N, __A)                                            \
    {                                                                       \
        ((uart_reg_t *)(UART##__N##_BASE_ADDRESS)),                        \
        PERIPHERAL_POWER_UART##__N,                                                   \
    },

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//! \name usart working mode 
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
} em_usart_mode_t;
//! @}

//! \name usart configuration
//! @{
typedef struct {
    uint32_t            wBaudrate;
    uint16_t            hwMode;
} usart_cfg_t;
//! @}


//! \name main baudrate interface
//! @{
DEF_INTERFACE(i_baudrate_t)
    bool            (*Set)(uint32_t wBaudrate);
    uint32_t        (*Get)(void);
END_DEF_INTERFACE(i_baudrate_t)
//! @}

//! \name class: i_uart_t
//! @{
DEF_INTERFACE(i_uart_t)
    bool        (*Config)(usart_cfg_t *ptCFG);
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
DEF_STRUCTURE(__usart_t)
    uart_reg_t *const           ptREG;
    const em_peripheral_pw_t    tPower;
END_DEF_STRUCTURE(__usart_t)
//! @}

/*============================ PROTOTYPES ====================================*/
static bool usart_idle(uint32_t wUsart);
static bool usart_config(uint32_t wUsart, usart_cfg_t *ptUsartCFG);
static bool usart_enable(uint32_t wUsart);
static bool usart_disable(uint32_t wUsart);
static bool usart_write_byte(uint32_t wUsart, uint8_t chByte);
static bool usart_read_byte(uint32_t wUsart, uint8_t *pchByte);
static bool     usart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate);
static uint32_t usart_baudrate_get(uint32_t wUsart);

MREPEAT(UART_COUNT, __UART_FUNCTION_PROTOTYPES, 0)

/*============================ GLOBAL VARIABLES ==============================*/                                               
//! \brief usart object
const i_uart_t UART[UART_COUNT] = {
    MREPEAT(UART_COUNT, __UART_INTERFACE, 0)
};


/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal usart object
static __usart_t __UART[] = {
    MREPEAT(UART_COUNT, __UART_OBJ, 0)
};

/*============================ IMPLEMENTATION ================================*/

/*! \brief idle usart
 *! \param void
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool usart_idle(uint32_t wUsart)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];
    bool bResult = false;
    
    __SAFE_CLK_CODE(
        if (this.ptREG->COMSTA0 & (1u << 5)) {
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
static bool usart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate)
{    
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];
    uart_reg_t *ptREG = this.ptREG;     

    if (0 == wBaudrate) {
        return false;
    }
           
    do {
        uint32_t wPclkFrequency;
        uint32_t wDL;
        uint32_t wN;
        uint32_t wM;
        
        wPclkFrequency = core_clock_get();
        wPclkFrequency >>= 5;       //!< / 32
        if (wPclkFrequency < wBaudrate) {
            return false;
        }
        wDL = wPclkFrequency / wBaudrate;
        wPclkFrequency *= 10000;
        wPclkFrequency = wPclkFrequency / (wDL * wBaudrate);
        
        if ((wPclkFrequency >= 10000) && (wPclkFrequency < 20000)) {
            wPclkFrequency -= 10000;
            wM = 1;
        } else if ((wPclkFrequency >= 20000) && (wPclkFrequency < 30000)) {
            wPclkFrequency -= 20000;
            wM = 2;
        } else if ((wPclkFrequency >= 30000) && (wPclkFrequency < 40000)) {
            wPclkFrequency -= 30000;
            wM = 3;
        } else if (wPclkFrequency >= 40000) {
            wPclkFrequency -= 40000;
            wM = 0;
        }
        wPclkFrequency <<= 11;
        wN = wPclkFrequency / 10000;
        if (wPclkFrequency % 10000 < 5000) {
        } else {
            wN++;
        }
        
        ptREG->COMCON0 |= (1u << 7);
        ptREG->COMDIV1  = wDL >> 8u;
        ptREG->COMDIV0  = wDL & 0xFFu;
        ptREG->COMCON0 &= ~(1u << 7);
        ptREG->COMDIV2  = (1u << 15)
                        | ((wM & 3) << 11)
                        | ((wN & BITMASK(10, 0)) << 0);
    } while (0);
    
    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t usart_baudrate_get(uint32_t wUsart)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];
    uart_reg_t *ptREG = this.ptREG;     
    uint32_t wPclkFrequency;
    uint32_t wN = ptREG->COMDIV2 & BITMASK(10, 0);
    uint32_t wM = (ptREG->COMDIV2 >> 11) & 3;

    __SAFE_CLK_CODE(
        ptREG->COMCON0 |= (1u << 7);
        wPclkFrequency = (ptREG->COMDIV1 << 8) | ptREG->COMDIV0;
        ptREG->COMCON0 &= ~(1u << 7);
    )

    return (core_clock_get() << 6) / wPclkFrequency / ((wM << 11) + wN);
}

/*! \brief initialize usart
 *! \param ptUsartCFG usart configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool usart_config(uint32_t wUsart, usart_cfg_t *ptUsartCFG)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];
    bool bResault = false;

    if (NULL == ptUsartCFG) {
        return false;
    }

        do { 
            uint16_t hwMode = ptUsartCFG->hwMode;
            uart_reg_t *ptREG = ptThis->ptREG; 

            //! read LCR
            uint32_t wReg = ptREG->COMCON0 & ~ 
                        (   (3u << 0)              |
                            (1u << 1)              |
                            (1u << 2)               |
                            (1u << 3)               |
                            (1u << 4)               |
                            (1u << 5)               |
                            (1u << 6)               |
                            (1u << 7)
                        );

            //! parity configuration
            if (hwMode & 0x08u) {
                wReg |= (1u << 3);
                if (hwMode & BIT(9)) {
                    wReg |= (1u << 3);
                } else {
                }
            } else {
            }

            //! configure stop bits
            if (hwMode & UART_2_STOPBIT) {
                wReg |= (1u << 2);
            }

            //! bit length configuration
            wReg |= ((hwMode & MODE_LENGTH_MSK) >> MODE_LENGTH_SHIFT) << 0;

            //! update LCR register
            ptREG->COMCON0 = wReg;
            
            //! baudrate configuration 
            if (!usart_baudrate_set(wUsart, ptUsartCFG->wBaudrate)) {
                break;
            }
            
            bResault = true;
        } while(0);
    
    return bResault;
}

/*! \brief enable usart
 *! \param void
 *! \retval true enable succeed
 *! \retval false enable failed
 */
static bool usart_enable(uint32_t wUsart)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];

    //! Enable peripheral clock
    peripheral_power_enable(ptThis->tPower);

    return true;
}    

/*! \brief disable usart
 *! \param void
 *! \retval true disable succeed
 *! \retval false disable failed
 */
static bool usart_disable(uint32_t wUsart)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];

    //! Disable peripheral clock
    peripheral_power_disable(ptThis->tPower);
    
    return true;
}    

/*! \brief write one byte to usart
 *! \param chByte byte of writing
 *! \retval true writing succeed
 *! \retval false writing failed
 */
static bool usart_write_byte(uint32_t wUsart, uint8_t chByte)
{
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart]; 
    
    if ((this.ptREG->COMSTA0 & (1u << 5))) {
	    this.ptREG->COMTX = chByte;
        return true;
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
    __usart_t *ptThis = (__usart_t *)&__UART[wUsart];
    
    if (NULL == pchByte) {
        return false;
    }      
 
    if (ptThis->ptREG->COMSTA0 & (1u << 0)) {
        *pchByte = this.ptREG->COMRX;
        return true;
    }

    return false;
}
 

MREPEAT(UART_COUNT, __UART_FUNCTION, 0)
