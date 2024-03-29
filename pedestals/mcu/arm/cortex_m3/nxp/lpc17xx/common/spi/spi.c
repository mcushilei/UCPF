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
#include ".\reg_spi.h"
#include "..\device.h"
#include "..\scon\pm.h"
#include "./spi.h"

/*============================ MACROS ========================================*/
//! \brief The reference for current object
#define this        (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define __SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus   = PM_POWER_GET_STATUS(ptThis->tPOWER);            \
        PM_POWER_ENABLE(ptThis->tPOWER);                                     \
        __VA_ARGS__;                                                            \
        PM_POWER_RESUME(ptThis->tPOWER, wStatus);                            \
    }

#define __SAFE_CLK_CODE_BEGIN()                                                      \
        uint32_t __wStatus   = PM_POWER_GET_STATUS(ptThis->tPOWER);            \
        PM_POWER_ENABLE(ptThis->tPOWER);                                     \

#define __SAFE_CLK_CODE_END()                                                      \
        PM_POWER_RESUME(ptThis->tPOWER, __wStatus);                            \

//! \brief Macro for spi init
#define __SPI_OBJ(__N,__DATA)                                                   \
    {                                                                           \
        ((ssp_reg_t *)(SSP##__N##_BASE_ADDRESS)),                               \
        PCONP_SSP##__N,                                                         \
        PCLK_SSP##__N,                                                          \
    },

//! \brief Macro for spi interface init
#define __SPI_INTERFACE(__N,__A)                                                \
    {                                                                           \
        &spi##__N##_init,                                                       \
        &spi##__N##_deinit,                                                       \
        &spi##__N##_idle,                                                       \
        &spi##__N##_write,                                                      \
        &spi##__N##_read,                                                       \
    },

//! \brief Macro for spi interface function prototypes
#define __SPI_INTERFACE_PROTOTYPES(__N,__DATA)                                  \
    extern bool spi##__N##_init(spi_cfg_t *ptSpiCfg);                            \
    extern bool spi##__N##_deinit(void);                                          \
    extern bool spi##__N##_idle(void);                                          \
    extern bool spi##__N##_write(uint16_t hwOut);                               \
    extern bool spi##__N##_read(uint16_t *phwIn);

//! \brief Macro of spi modules interface function body
#define __SPI_INTERFACE_DEFINE(__N, __DATA)                                     \
    bool spi##__N##_init(spi_cfg_t *ptSpiCfg)                                    \
    {                                                                           \
        return ssp_init(ptSpiCfg, __N);                       \
    }                                                                           \
    bool spi##__N##_deinit(void)                                                  \
    {                                                                           \
        return ssp_deinit(__N);                                \
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

/*============================ TYPES =========================================*/

//! \name internal class
//! @{
typedef struct {
    ssp_reg_t       *ptREG;                 //!< reference to register page
    uint32_t      tPOWER;
    uint32_t    tPClk;
} __spi_t;
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief spi interface function prototypes
MREPEAT(SSP_COUNT, __SPI_INTERFACE_PROTOTYPES, 0)

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal spi object
static const __spi_t __SPI[] = {
    MREPEAT(SSP_COUNT, __SPI_OBJ, 0)
};

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief SPI object
const i_spi_t SPI[] = {
    MREPEAT(SSP_COUNT, __SPI_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/


WEAK void ssp_init_callback(const i_spi_t *interface)
{
    
}


/*! \brief initialize spi
 *! \param ptSpiCfg spi configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool ssp_init(spi_cfg_t *ptSpiCfg, uint32_t index)
{
    const __spi_t *ptThis = &__SPI[index];
    
    if ((ptSpiCfg->chDataSize < SPI_MODE_DATASIZE_4)
    ||  (ptSpiCfg->chDataSize > SPI_MODE_DATASIZE_16)) {
        return false;
    }

    if ((ptSpiCfg->chClockDiv < 2) || (ptSpiCfg->chClockDiv > 254)) {
        return false;
    }
    
    ssp_init_callback(&SPI[index]);
    
    //! enable AHBCLK
    PM_POWER_ENABLE(ptThis->tPOWER);

    __SAFE_CLK_CODE_BEGIN();
        ssp_reg_t *ptREG = this.ptREG;

        //!  read CR0 register
        uint32_t wTempCR0 = 0;

        //! set frame type
        if (ptSpiCfg->hwMode & SPI_MODE_FORMAT_TI) {
            //!< use TI fram
            wTempCR0 |= SPI_FORMAT_SET(SPI_FORMAT_TI);
        } else if(ptSpiCfg->hwMode & SPI_MODE_FORMAT_MICROWIRE) {
            //! use Microwire
            wTempCR0 |= SPI_FORMAT_SET(SPI_FORMAT_MICROWIRE);
        } else {
            //! use standard type
        }

        //! set clock polarity and sample point
        if (ptSpiCfg->hwMode & SPI_MODE_CLK_IDLE_HIGH) {
            wTempCR0 |= SPI_CLK_POLARITY_SET(SPI_CLK_IDLE_HIGH);
        } else {
            //! use idle low type
        }
        
        if (ptSpiCfg->hwMode & SPI_MODE_SAMP_SECOND_EDGE) {
        } else {
            //! samp at first edge
            wTempCR0 |= 1u << SPI_CR0_CPHA;
        }

        //! set data size
        wTempCR0 |= SPI_DATASIZE_SET(ptSpiCfg->chDataSize);
        
        //! set Serial Clock Rate.
        wTempCR0 |= 1u << 8;
        
        //! update CR0
        ptREG->CR0 = wTempCR0;

        //! read CR1 register
        uint32_t wTempCR1 = 0;

        //! switch between master mode and slave mode
        if (ptSpiCfg->hwMode & SPI_MODE_SLAVE) {
            //! use slave mode
            wTempCR1 |= SPI_MODE_SET(SPI_SLAVE);
        } else {
            //! use master mode
        }

        //! set as loop-back mode
        if (ptSpiCfg->hwMode & SPI_MODE_LOOP_BACK) {
            wTempCR1 |= SPI_LOOP_BACK_SET(SPI_LOOP_BACK);
        } else {
            //! standard mode
        }

        //! if slave, data out disable
        if (ptSpiCfg->hwMode & SPI_MODE_SLAVE_OUT_DISABLE) {
            wTempCR1 |= SPI_SLAVE_OUT_SET(SPI_OUT_DISABLE);
        } else {
            //! standard mode
        }

        //! update CR1
        ptREG->CR1 = wTempCR1;
        
        uint32_t wTempCPSR = 0;

        //! set clock prescaler
        wTempCPSR = SPI_CPSR_SET(ptSpiCfg->chClockDiv & 0x000000FEu);
        
        //! update CPSR
        ptREG->CPSR = wTempCPSR;
        
        //! set spi enable bit
        this.ptREG->CR1 |= SPI_CR1_SSE_MSK;
    __SAFE_CLK_CODE_END();

    return true;
}

static uint8_t ssp_calculate_pclk_prescaler(void)
{
    uint32_t wPrescaler = 1;

    return wPrescaler;
}


WEAK void ssp_deinit_callback(const i_spi_t *interface)
{
    
}

/*! \brief disable spi
 *! \param void
 */
static bool ssp_deinit(uint32_t index)
{
    const __spi_t *ptThis = &__SPI[index];
    
    __SAFE_CLK_CODE_BEGIN();
    //! disable spi module
    this.ptREG->CR1 &=~ SPI_CR1_SSE_MSK;
    __SAFE_CLK_CODE_END();
    
    //! Disable AHBCLK
    PM_POWER_DISABLE(ptThis->tPOWER);
    
    ssp_deinit_callback(&SPI[index]);
    
    return true;
}

/*! \brief return state of spi
 *! \param void
 *! \retval true spi is idle
 *! \retval false spi is busy
 */
static bool ssp_is_idle(uint32_t index)
{
    const __spi_t *ptThis = &__SPI[index];
    bool bResult = true;

    //! spi is busy, return false
    if (this.ptREG->SR & SPI_SR_BSY_MSK) {
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
static bool ssp_write(uint16_t hwOut, uint32_t index)
{
    const __spi_t *ptThis = &__SPI[index];
    
    //! if tx fifo is not full, write data
    if (this.ptREG->SR & SPI_SR_TNF_MSK) {            //!< tx fifo is not full
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
static bool ssp_read(uint16_t *phwIn, uint32_t index)
{
    const __spi_t *ptThis = &__SPI[index];
    
    //! if rx fifo is not empty, read data
    if (this.ptREG->SR & SPI_SR_RNE_MSK) {            //!< rx fifo is not empty
        uint16_t hwRcv = this.ptREG->DR;
        if (phwIn != NULL) {
            *phwIn = hwRcv;
        }
        return true;
    }

    return false;
}

//! \brief Spi modules interface function body
MREPEAT(SSP_COUNT, __SPI_INTERFACE_DEFINE, NULL)








bool spi_config(spi_cfg_t *ptSpiCfg)
{
    if ((ptSpiCfg->chDataSize < SPI_MODE_DATASIZE_4)
    ||  (ptSpiCfg->chDataSize > SPI_MODE_DATASIZE_16)) {
        return false;
    }

    if ((ptSpiCfg->chClockDiv < 2) || (ptSpiCfg->chClockDiv > 254)) {
        return false;
    }

    uint32_t wStatus   = PM_POWER_GET_STATUS(PCONP_SPI);
    PM_POWER_ENABLE(PCONP_SPI);
    
    spi_reg_t *ptREG = &SPI_REG;

    //!  read SPCR register
    uint32_t wTemp = ptREG->SPCR &
                        ~(  SPI_SPCR_LSBF_MSK     |
                            SPI_SPCR_CPOL_MSK    |
                            SPI_SPCR_CPHA_MSK    |
                            SPI_SPCR_MSTR_MSK   |
                            SPI_SPCR_BITS_MSK);

    if (ptSpiCfg->hwMode & SPI_MODE_CLK_IDLE_HIGH) {
        wTemp |= SPI_SPCR_CPOL_MSK;
    } else {
        //! use idle low type
    }
    
    if (ptSpiCfg->hwMode & SPI_MODE_SAMP_SECOND_EDGE) {
        wTemp |= SPI_SPCR_CPHA_MSK;
    } else {
        //! samp at first edge
    }
    
    if (ptSpiCfg->hwMode & SPI_MODE_LSB_FIRST) {
        wTemp |= SPI_SPCR_LSBF_MSK;
    } else {
        //! MSB transive first.
    }

    wTemp |= (uint32_t)(ptSpiCfg->chDataSize & 0x0Fu) << SPI_SPCR_BITS_BIT0;

    if (ptSpiCfg->hwMode & SPI_MODE_SLAVE) {
        //! use slave mode
    } else {
        wTemp |= SPI_SPCR_MSTR_MSK;
        //! use master mode
    }
    
    ptREG->SPCR = wTemp;
    
    //! update CPSR
    wTemp = ptSpiCfg->chClockDiv;
    if (wTemp < 8) {
        wTemp = 8;
    }
    if (wTemp & 0x01u) {
        wTemp += 1;
    }
    ptREG->SPCCR = wTemp;
        
    PM_POWER_RESUME(PCONP_SPI, wStatus);

    return true;
}

bool spi_write(uint16_t hwOut)
{
    SPI_REG.SPDR = hwOut;
    if (SPI_REG.SPSR & SPI_SPSR_WCOL_MKS) {
        return false;
    }

    return true;
}

bool spi_read(uint16_t *phwIn)
{
    if (SPI_REG.SPSR & SPI_SPSR_SPIF_MSK) {
        uint16_t hwRcv = SPI_REG.SPDR;
        if (phwIn != NULL) {
            *phwIn = hwRcv;
        }
        return true;
    }

    return false;
}

bool spi_enable(void)
{
    //! enable AHBCLK
    PM_POWER_ENABLE(PCONP_SPI);

    return true;
}

bool spi_disable(void)
{
    //! Disable AHBCLK
    PM_POWER_DISABLE(PCONP_SPI);
    
    return true;
}

//! end of file
