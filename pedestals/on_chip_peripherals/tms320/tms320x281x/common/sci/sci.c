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
#include "..\sysctrl\sc.h"
#include ".\reg_sci.h"

/*============================ MACROS ========================================*/
#define this                    (*ptThis)

#ifdef  SCIA_BASE_ADDRESS
#define USART0_BASE     SCIA_BASE_ADDRESS
#endif

#ifdef  SCIB_BASE_ADDRESS
#define USART1_BASE     SCIB_BASE_ADDRESS
#endif


#define MODE_LENGTH_MSK         0x0700
#define MODE_LENGTH_SHIFT       8

#define PCLKCR_USART0    PCLKCR_SCIAENCLK
#define PCLKCR_USART1    PCLKCR_SCIBENCLK

#define PCLK_USART0      LOW_SPEED_PCLK
#define PCLK_USART1      LOW_SPEED_PCLK

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                          \
    {                                                               \
        uint32_t wStatus = PM.Power.Peripheral.GetStatus(ptThis->tPCON);    \
        PM.Power.Peripheral.Enable(ptThis->tPCON);                  \
        __VA_ARGS__;                                                \
        PM.Power.Peripheral.ResumeStatus(ptThis->tPCON, wStatus);   \
    }

#define USART_CLASS(__N, __A)                                               \
    {                                                                       \
        ((sci_reg_t *)(USART##__N##_BASE)),                                 \
        PCLKCR_USART##__N,                                                  \
        PCLK_USART##__N,                                                    \
    },

#define USART_FUNCTION(__N, __A)                                            \
    bool usart##__N##_init(usart_cfg_t *ptUsartCFG)                         \
    {                                                                       \
        return usart_init(__N, ptUsartCFG);                                 \
    }                                                                       \
                                                                            \
    bool usart##__N##_check_idle(void)                                      \
    {                                                                       \
        return usart_is_idle(__N);                                          \
    }                                                                       \
                                                                            \
    bool usart##__N##_enable(void)                                          \
    {                                                                       \
        return usart_enable(__N);                                           \
    }                                                                       \
                                                                            \
    bool usart##__N##_disable(void)                                         \
    {                                                                       \
        return usart_disable(__N);                                          \
    }                                                                       \
                                                                            \
    bool usart##__N##_write_byte(uint8_t chByte)                            \
    {                                                                       \
        return usart_write_byte(__N, chByte);                               \
    }                                                                       \
                                                                            \
    bool usart##__N##_read_byte(uint8_t *pchByte)                           \
    {                                                                       \
        return usart_read_byte(__N, pchByte);                               \
    }                                                                       \
                                                                            \
    bool usart##__N##_baudrate_set(uint32_t wBaudrate)                      \
    {                                                                       \
        return usart_baudrate_set(__N, wBaudrate);                          \
    }                                                                       \
                                                                            \
    uint32_t usart##__N##_baudrate_get(void)                                \
    {                                                                       \
        return usart_baudrate_get(__N);                                     \
    }                                                                       \


#define USART_FUNCTION_PROTOTYPE(__N, __A)                                  \
    extern bool usart##__N##_init(usart_cfg_t *ptUsartCFG);                 \
    extern bool usart##__N##_check_idle(void);                              \
    extern bool usart##__N##_enable(void);                                  \
    extern bool usart##__N##_disable(void);                                 \
    extern bool usart##__N##_write_byte(uint8_t chByte);                    \
    extern bool usart##__N##_read_byte(uint8_t *pchByte);                   \
    extern bool usart##__N##_baudrate_set(uint32_t wBaudrate);              \
    extern uint32_t usart##__N##_baudrate_get(void);                        \


#define USART_INTERFACE(__N, __A)                                           \
    {                                                                       \
        &usart##__N##_init,                                                 \
        &usart##__N##_check_idle,                                           \
        &usart##__N##_enable,                                               \
        &usart##__N##_disable,                                              \
        &usart##__N##_write_byte,                                           \
        &usart##__N##_read_byte,                                            \
        {                                                                   \
            &usart##__N##_baudrate_set,                                     \
            &usart##__N##_baudrate_get,                                     \
        },                                                                  \
        ((sci_reg_t *)(USART##__N##_BASE)),                                 \
    },

/*============================ TYPES =========================================*/
//! \name usart working mode
//! @{
typedef enum {
    USART_NO_AUTO_BAUD      = 0x00,
    USART_AUTO_BAUD_MODE0   = 0x01,
    USART_AUTO_BAUD_MODE1   = 0x03,
    USART_AUTO_RESTART      = 0x04,
    USART_NO_AUTO_RESTART   = 0x00,

    USART_NO_PARITY         = 0x00,
    USART_EVEN_PARITY       = 0x18,
    USART_ODD_PARITY        = 0x08,
    USART_FORCE_1_PARITY    = 0x28,
    USART_FORCE_0_PARITY    = 0x38,

    USART_1_STOPBIT         = 0x00,
    USART_2_STOPBIT         = 0x40,

    USART_ENABLE_FIFO       = 0x00,
    USART_DISABLE_FIFO      = 0x80,

    USART_5_BIT_LENGTH      = 0x0400,
    USART_6_BIT_LENGTH      = 0x0500,
    USART_7_BIT_LENGTH      = 0x0600,
    USART_8_BIT_LENGTH      = 0x0700,

    USART_SYNC_MODE         = 0x0800,
} em_usart_mode_t;
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
    bool            (*CheckIdle)(void);
    bool            (*Enable)(void);
    bool            (*Disable)(void);
    bool            (*WriteByte)(uint8_t chByte);
    bool            (*ReadByte)(uint8_t *pchByte);
    i_baudrate_t    Baudrate;

    sci_reg_t *const RegPage;
END_DEF_INTERFACE(i_usart_t)
//! @}

//! \name internal class
//! @{
DEF_CLASS(__usart_t)
    sci_reg_t *const    ptREG;
    em_pclkcr_t         tPCON;
    em_pclksel_t        tPCLK;
END_DEF_CLASS(__usart_t)
//! @}

/*============================ PROTOTYPES ====================================*/
static bool usart_is_idle(uint32_t wUsart);
static bool usart_init(uint32_t wUsart, usart_cfg_t *ptUsartCFG);
static bool     usart_baudrate_set(uint32_t wUsart, uint32_t wBaudrate);
static uint32_t usart_baudrate_get(uint32_t wUsart);
static bool usart_enable(uint32_t wUsart);
static bool usart_disable(uint32_t wUsart);
static bool usart_write_byte(uint32_t wUsart, uint8_t chByte);
static bool usart_read_byte(uint32_t wUsart, uint8_t *pchByte);

MREPEAT(USART_COUNT, USART_FUNCTION_PROTOTYPE, 0)

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief usart object
const i_usart_t USART[USART_COUNT] = {
    MREPEAT(USART_COUNT, USART_INTERFACE, 0)
};

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal usart object
static const CLASS(__usart_t) __USART[USART_COUNT] = {
    MREPEAT(USART_COUNT, USART_CLASS, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief to check whether usart is idle.
 *! \param wUsart wich usart to check
 *! \retval true idle succeed
 *! \retval false idle failed
 */
static bool usart_is_idle(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    bool bResult = false;

    if (wUsart >= USART_COUNT) {
        return false;
    }

    SAFE_CLK_CODE(
        if (!(SCIRXST_RXRDY_MSK & this.ptREG->SCIRXST.Value)
        &&  (SCICTL2_TXEMPTY_MSK & this.ptREG->SCICTL2.Value)) {
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
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if (wUsart >= USART_COUNT) {
        return false;
    }

    //! Can not change baudrate when transiver are working.
    if (!usart_is_idle(wUsart)) {
        return false;
    }

    if (!wBaudrate) {
        wBaudrate++;
    }

    do {
        sci_reg_t *ptREG = ptThis->ptREG;
        uint32_t wValue = PM.Clock.Peripheral.Get(ptThis->tPCLK);

        wValue = (wValue >> 3) / wBaudrate;
        if ((wValue == 0) || (wValue > USHRT_MAX)) {
            return false;
        }

        wValue--;
        SAFE_CLK_CODE(
            ptREG->SCIHBAUD  = wValue >> 8;
            ptREG->SCILBAUD  = wValue & 0xFF;
        )
    } while (0);

    return true;
}

/*! \brief get Baudrate
 *! \param  none
 *! \retrun Baudrate
 */
static uint32_t usart_baudrate_get(uint32_t wUsart)
{
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];
    uint32_t wBaudrate = 0;

    if (wUsart >= USART_COUNT) {
        return 0;
    }

    do {
        sci_reg_t *ptREG = ptThis->ptREG;
        uint32_t  wPclkFrequency = PM.Clock.Peripheral.Get(ptThis->tPCLK);

        SAFE_CLK_CODE(
            wBaudrate = ptREG->SCIHBAUD;
            wBaudrate <<= 8;
            wBaudrate |= ptREG->SCILBAUD;
        )
        wBaudrate = (wPclkFrequency >> 3) / (wBaudrate + 1);
    } while (0);

    return wBaudrate;
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

    SAFE_CLK_CODE (
        do {
            uint16_t  hwMode = ptUsartCFG->hwMode;
            sci_reg_t *ptREG = ptThis->ptREG;

            //! reset all flag, disable TX and RX.
            ptREG->SCICTL1.Value = 0;
            ptREG->SCICTL1.Value = SCICTL1_SWRESET_MSK;

            //! reset to 0.
            ptREG->SCICCR.Value = 0;

            //! reset FIFO
            ptREG->SCIFFTX.Value = 0;
            ptREG->SCIFFTX.Value = SCIFFTX_SCIRST_MSK | SCIFFTX_TXFIFOXRESET_MSK | SCIFFTX_TXINTCLR_MSK;
            ptREG->SCIFFRX.Value = 0;
            ptREG->SCIFFRX.Value = SCIFFRX_RXFIFORESET_MSK | SCIFFRX_RXFFOVRCLR_MSK | SCIFFRX_RXFFINTCLR_MSK;

            //! FIFO configuration
            if (!(hwMode & USART_DISABLE_FIFO)) {
                ptREG->SCIFFTX.Value |= SCIFFTX_SCIFFENA_MSK;
            }

            //! parity configuration
            if (0x08 & hwMode) {
                if (0x10 & hwMode) {
                    ptREG->SCICCR.Value |= SCICCR_PARITY_MSK;
                }
                ptREG->SCICCR.Value |= SCICCR_PARITYENA_MSK;
            }

            //! configure stop bits
            if (hwMode & USART_2_STOPBIT) {
                ptREG->SCICCR.Value |= SCICCR_STOPBITS_MSK;
            }

            //! bit length configuration
            ptREG->SCICCR.Value |= (hwMode & MODE_LENGTH_MSK) >> MODE_LENGTH_SHIFT;

            //! baudrate configuration
            if (!usart_baudrate_set(wUsart, ptUsartCFG->wBaudrate)) {
                return false;
            }

            //! enable TX and RX.
            ptREG->SCICTL1.Value |= SCICTL1_RXENA_MSK | SCICTL1_TXENA_MSK;
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
    PM.Power.Peripheral.Enable(ptThis->tPCON);
    //! Transmit enable
    ptThis->ptREG->SCICTL1.Value |= SCICTL1_RXENA_MSK | SCICTL1_TXENA_MSK;

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

    //! Transmit enable
    ptThis->ptREG->SCICTL1.Value &= ~(SCICTL1_RXENA_MSK | SCICTL1_TXENA_MSK);

    //! Disable Peripheral Clock
    PM.Power.Peripheral.Disable(ptThis->tPCON);

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

    if (this.ptREG->SCICTL2.TXRDY) {
        this.ptREG->SCITXBUF = chByte;
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
    CLASS(__usart_t) *ptThis = (CLASS(__usart_t) *)&__USART[wUsart];

    if ((wUsart >= USART_COUNT) || (NULL == pchByte)) {
        return false;
    }

    if (this.ptREG->SCIRXST.RXRDY) {
        *pchByte = this.ptREG->SCIRXBUF.Value;
        return true;
    }

    return false;
}

MREPEAT(USART_COUNT, USART_FUNCTION, 0)



/* EOF */
