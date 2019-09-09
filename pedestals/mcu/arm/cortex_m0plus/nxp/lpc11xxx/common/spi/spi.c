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

//! \note do not move this pre-processor statement to other places
#define __DRIVER_SSP_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_spi.h"
#include "..\device.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
//! \brief The reference for current object
#define this        (*ptThis)

#if (SSP_COUNT == 0 || SSP_COUNT > 2)
#error "Invalid SSP_COUNT!"
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#define __SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus   = PM_AHB_CLK_GET_STATUS(ptThis->AHBClock);            \
        PM_AHB_CLK_ENABLE(ptThis->AHBClock);                                     \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME(ptThis->AHBClock, wStatus);                            \
    }

//! \brief Macro for spi init
#define __SSP_OBJ(__N,__DATA)                                                   \
    {                                                                           \
        ((ssp_reg_t *)(SSP##__N##_BASE_ADDRESS)),                               \
        AHBCLK_SSP##__N,                                                         \
        __N,                                                          \
    },

//! \brief Macro for spi interface init
#define __SSP_INTERFACE(__N,__A)                                                \
    {                                                                           \
        &spi##__N##_config,                                                       \
        &spi##__N##_open,                                                       \
        &spi##__N##_close,                                                      \
        &spi##__N##_idle,                                                       \
        &spi##__N##_write,                                                      \
        &spi##__N##_read,                                                       \
    },

//! \brief Macro for spi interface function prototypes
#define __SSP_INTERFACE_PROTOTYPES(__N,__DATA)                                  \
    extern bool spi##__N##_config(spi_cfg_t *ptSpiCfg);                            \
    extern bool spi##__N##_open(void);                                          \
    extern bool spi##__N##_close(void);                                         \
    extern bool spi##__N##_idle(void);                                          \
    extern bool spi##__N##_write(uint16_t hwOut);                               \
    extern bool spi##__N##_read(uint16_t *phwIn);

//! \brief Macro of spi modules interface function body
#define __SSP_INTERFACE_DEFINE(__N, __DATA)                                     \
    bool spi##__N##_config(spi_cfg_t *ptSpiCfg)                                    \
    {                                                                           \
        return ssp_config(ptSpiCfg, __N);                       \
    }                                                                           \
    bool spi##__N##_open(void)                                                  \
    {                                                                           \
        return ssp_open(__N);                                \
    }                                                                           \
    bool spi##__N##_close(void)                                                 \
    {                                                                           \
        return ssp_close(__N);                               \
    }                                                                           \
    bool spi##__N##_idle(void)                                                  \
    {                                                                           \
        return ssp_is_idle(__N);                             \
    }                                                                           \
    bool spi##__N##_write(uint16_t hwOut)                                       \
    {                                                                           \
        return ssp_write(hwOut, __N);                        \
    }                                                                           \
    bool spi##__N##_read(uint16_t *phwIn)                                       \
    {                                                                           \
        return ssp_read(phwIn, __N);                         \
    }

#define __SPI_CLK_DIV(__N,__Value)     \
            SPI_PCLK_DIV_##__N = (2 * (__N)),

/*============================ TYPES =========================================*/
//! \name spi working mode
//! @{
typedef enum {
    SPI_MODE_MASTER             = 0x00,             //!< select master mode
    SPI_MODE_SLAVE              = BIT(0),           //!< select slave mode

    SPI_MODE_FORMAT_SPI         = 0x00,             //!< use standard spi fram
    SPI_MODE_FORMAT_TI          = BIT(1),
    SPI_MODE_FORMAT_MICROWIRE   = BIT(2),
    
    SPI_MODE_LSB_FIRST          = BIT(7),
    SPI_MODE_MSB_FIRST          = 0x00,

    SPI_MODE_CLK_IDLE_HIGH      = BIT(3),           //!< SCK is high in idle
    SPI_MODE_CLK_IDLE_LOW       = 0x00,             //!< SCK is low in idle

    SPI_MODE_SAMP_FIRST_EDGE    = 0x00,             //!< sample at first edge of sck
    SPI_MODE_SAMP_SECOND_EDGE   = BIT(4),           //!< sample at second edge of sck

    SPI_MODE_LOOP_BACK          = BIT(5),           //!< enable loop back
    SPI_MODE_NOT_LOOP_BACK      = 0x00,             //!< default disable loop back

    SPI_MODE_SLAVE_OUT_ENABLE   = 0x00,             //!< default enable slave output
    SPI_MODE_SLAVE_OUT_DISABLE  = BIT(6)            //!< disable slave output
} em_spi_mode_t;
//! @}

//! \name spi datasize
//! @{
typedef enum {
    SPI_MODE_DATASIZE_4         = 0x3,              //!< datasize is 4 bits
    SPI_MODE_DATASIZE_5         = 0x4,              //!< datasize is 5 bits
    SPI_MODE_DATASIZE_6         = 0x5,              //!< datasize is 6 bits
    SPI_MODE_DATASIZE_7         = 0x6,              //!< datasize is 7 bits
    SPI_MODE_DATASIZE_8         = 0x7,              //!< datasize is 8 bits
    SPI_MODE_DATASIZE_9         = 0x8,              //!< datasize is 9 bits
    SPI_MODE_DATASIZE_10        = 0x9,              //!< datasize is 10 bits
    SPI_MODE_DATASIZE_11        = 0xA,              //!< datasize is 11 bits
    SPI_MODE_DATASIZE_12        = 0xB,              //!< datasize is 12 bits
    SPI_MODE_DATASIZE_13        = 0xC,              //!< datasize is 13 bits
    SPI_MODE_DATASIZE_14        = 0xD,              //!< datasize is 14 bits
    SPI_MODE_DATASIZE_15        = 0xE,              //!< datasize is 15 bits
    SPI_MODE_DATASIZE_16        = 0xF               //!< datasize is 16 bits
} em_spi_datasize_t;

//! \name enum spi clk div
//! @{
typedef enum{
    MREPEAT(127,__SPI_CLK_DIV ,NULL)
} em_spiclk_div_t;
//! @}

//! \name spi configuration structure
//! @{
typedef struct{
    uint16_t hwMode;                     //!< spi working mode
    uint8_t  chDataSize;                 //!< frame size
    uint8_t  chClockDiv;                 //!< P_CLK prescaler
} spi_cfg_t;
//! @}

//! \name class: i_spi_t
//! @{
DEF_INTERFACE(i_spi_t)
    bool (*Config)(spi_cfg_t *ptCfg);        //!< Config spi
    bool (*Open)(void);                      //!< open spi
    bool (*Close)(void);                     //!< close spi
    bool (*Idle)(void);                      //!< get busy flag status
    bool (*Write)(uint16_t hwOut);           //!< write data to spi buffer
    bool (*Read)(uint16_t *phwIn);           //!< read data from spi buffer
END_DEF_INTERFACE(i_spi_t)
//! @}

//! \name internal class
//! @{
DEF_STRUCTURE(__spi_t)
    ssp_reg_t       *ptREG;                 //!< reference to register page
    uint32_t        AHBClock;
    uint32_t        PClock;
END_DEF_STRUCTURE(__spi_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief spi interface function prototypes
MREPEAT(SSP_COUNT, __SSP_INTERFACE_PROTOTYPES, 0)

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal spi object
static const __spi_t __SPI[SSP_COUNT] = {
    MREPEAT(SSP_COUNT, __SSP_OBJ, 0)
};

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief SPI object
const i_spi_t SPI[SSP_COUNT] = {
    MREPEAT(SSP_COUNT, __SSP_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief initialize spi
 *! \param ptSpiCfg spi configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool ssp_config(spi_cfg_t *ptSpiCfg, uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    if ((ptSpiCfg->chDataSize < SPI_MODE_DATASIZE_4)
    ||  (ptSpiCfg->chDataSize > SPI_MODE_DATASIZE_16)) {
        return false;
    }

    if ((ptSpiCfg->chClockDiv < 2) || (ptSpiCfg->chClockDiv > 254)) {
        return false;
    }

    __SAFE_CLK_CODE (
        ssp_reg_t *ptREG = this.ptREG;

        //!  read CR0 register
        uint32_t wTempCR0 = ptREG->CR0 &
                            ~(  SSP_CR0_DSS_MSK     |
                                SSP_CR0_FRF_MSK     |
                                SSP_CR0_CPOL_MSK    |
                                SSP_CR0_CPHA_MSK    |
                                SSP_CR0_SCR_MSK);

        //! set frame type
        if (ptSpiCfg->hwMode & SPI_MODE_FORMAT_TI) {
            //!< use TI fram
            wTempCR0 |= SSP_FORMAT_SET(SSP_FORMAT_TI);
        } else if(ptSpiCfg->hwMode & SPI_MODE_FORMAT_MICROWIRE) {
            //! use Microwire
            wTempCR0 |= SSP_FORMAT_SET(SSP_FORMAT_MICROWIRE);
        } else {
            //! use standard type
        }

        //! set clock polarity and sample point
        if (ptSpiCfg->hwMode & SPI_MODE_CLK_IDLE_HIGH) {
            wTempCR0 |= SSP_CLK_POLARITY_SET(SSP_CLK_IDLE_HIGH);
        } else {
            //! use idle low type
        }
        if (ptSpiCfg->hwMode & SPI_MODE_SAMP_SECOND_EDGE) {
        } else {
            //! samp at first edge
            wTempCR0 |= SSP_CLK_SAMP_SET(SSP_SAMP_SECOND_EDGE);
        }

        //! set data size
        wTempCR0 |= SSP_DATASIZE_SET(ptSpiCfg->chDataSize - 1);
        
        //! update CR0
        ptREG->CR0 = wTempCR0;

        //! read CR1 register
        uint32_t wTempCR1 = ptREG->CR1 &
                            ~(  SSP_CR1_LBM_MSK     |
                                SSP_CR1_MS_MSK      |
                                SSP_CR1_SOD_MSK);

        //! switch between master mode and slave mode
        if (ptSpiCfg->hwMode & SPI_MODE_SLAVE) {
            //! use slave mode
            wTempCR1 |= SSP_MODE_SET(SSP_SLAVE);
        } else {
            //! use master mode
        }

        //! set as loop-back mode
        if (ptSpiCfg->hwMode & SPI_MODE_LOOP_BACK) {
            wTempCR1 |= SSP_LOOP_BACK_SET(SSP_LOOP_BACK);
        } else {
            //! standard mode
        }

        //! if slave, data out disable
        if (ptSpiCfg->hwMode & SPI_MODE_SLAVE_OUT_DISABLE) {
            wTempCR1 |= SSP_SLAVE_OUT_SET(SSP_OUT_DISABLE);
        } else {
            //! standard mode
        }

        //! update CR1
        ptREG->CR1 = wTempCR1;
        
        uint32_t wTempCPSR = ptREG->CPSR & ~(SSP_CPSR_CPSDVSR_MSK);

        //! set clock prescaler
        wTempCPSR |= SSP_CPSR_SET(ptSpiCfg->chClockDiv & 0xFFFFFFFEul);
        
        //! update CPSR
        ptREG->CPSR = wTempCPSR;
    )

    return true;
}

/*! \brief enable spi
 *! \param void
 */
static bool ssp_open(uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    //! enable AHBCLK
    PM_AHB_CLK_ENABLE(ptThis->AHBClock);
    //! enable peripheral clock and reset peripheral.
    switch (ssp) {
        case 0:
            SYSCON_REG.SSP0CLKDIV = 2u ;
            SYSCON_REG.PRESETCTRL &= ~(1u << SCON_PRESETCTRL_SSP0_BIAS);
            SYSCON_REG.PRESETCTRL |= 1u << SCON_PRESETCTRL_SSP0_BIAS;
            break;
        case 1:
            SYSCON_REG.SSP1CLKDIV = 1u;
            SYSCON_REG.PRESETCTRL &= ~(1u << SCON_PRESETCTRL_SSP1_BIAS);
            SYSCON_REG.PRESETCTRL |= 1u << SCON_PRESETCTRL_SSP1_BIAS;
            break;
    }
    //! set spi enable bit
    this.ptREG->CR1 |= SSP_CR1_SSE_MSK;

    return true;
}

/*! \brief disable spi
 *! \param void
 */
static bool ssp_close(uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    //! disable spi module
    this.ptREG->CR1 &=~ SSP_CR1_SSE_MSK;
    //! disable peripheral clock
    switch (ssp) {
        case 0:
            SYSCON_REG.SSP0CLKDIV = 0u;
            break;
        case 1:
            SYSCON_REG.SSP1CLKDIV = 0u;
            break;
    }
    //! Disable AHBCLK
    PM_AHB_CLK_DISABLE(ptThis->AHBClock);
    
    return true;
}

/*! \brief return state of spi
 *! \param void
 *! \retval true spi is idle
 *! \retval false spi is busy
 */
static bool ssp_is_idle(uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    bool bResult = true;

    //! spi is busy, return false
    if (this.ptREG->SR & SSP_SR_BSY_MSK) {
        bResult = false;
    }

    //! spi is idle, return true
    return bResult;
}

/*! \brief write a data with out recieving
 *! \param hwOut the out-data
 *! \retval true write access is success
 *! \retval false write access is failed or illegal input parameter
 */
static bool ssp_write(uint16_t hwOut, uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    //! if tx fifo is not full, write data
    if (this.ptREG->SR & SSP_SR_TNF_MSK) {            //!< tx fifo is not full
        this.ptREG->DR = hwOut;
        return true;
    }

    return false;
}

/*! \brief read a data with out sending
 *! \param phwIn the addr of in-data
 *! \retval true read access is success
 *! \retval false read access is failed or illegal input parameter
 */
static bool ssp_read(uint16_t *phwIn, uint32_t ssp)
{
    __spi_t *ptThis = (__spi_t *)&__SPI[ssp];

    //! if rx fifo is not empty, read data
    if (this.ptREG->SR & SSP_SR_RNE_MSK) {            //!< rx fifo is not empty
        uint16_t hwRcv = this.ptREG->DR;
        if (phwIn != NULL) {
            *phwIn = hwRcv;
        }
        return true;
    }

    return false;
}

//! \brief Spi modules interface function body
MREPEAT(SSP_COUNT, __SSP_INTERFACE_DEFINE, NULL)


//! end of file
