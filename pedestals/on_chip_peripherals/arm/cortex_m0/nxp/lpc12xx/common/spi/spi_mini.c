/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\i_io_spi.h"
#include "..\device.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
#ifndef SYSTEM_MAIN_CLK
#error "SYSTEM_MAIN_CLK is not defined!"
#endif

#ifndef MINI_SPI_USE_SPI
#error "MINI_SPI_USE_SPI is not defined!"
#endif

//! \brief The reference for current object
#define this                (*ptSPIREG)

//! \brief caculate spi prescaler
#define SPI_PRESCALER       (SYSTEM_MAIN_CLK / MAX_SYSTEM_CLK_FREQ)

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \brief Macro for spi init
#define SPI_T_INIT(__N)     __SPI_T_INIT(__N)
#define __SPI_T_INIT(__N)                                                       \
    ((spi_reg_t *)(SPI##__N##_BASE_ADDRESS))                                    \

//! \brief Macro for spi clock division
#define __SPI_CLK_DIV(__N, __Value)                                             \
    SPI_PCLK_DIV_##__N = (2 * (__N)),

/*============================ TYPES =========================================*/
//! \name enum spi clk div
//! @{
typedef enum{
    MREPEAT(127, __SPI_CLK_DIV, NULL)
} em_spiclk_div_t;
//! @}

//! \name spi working mode
//! @{
typedef enum {
    SPI_MODE_MASTER             = 0x00,             //!< select master mode
    SPI_MODE_SLAVE              = BIT(0),           //!< select slave mode

    SPI_MODE_FORMAT_SPI         = 0x00,             //!< use standard spi fram
    SPI_MODE_FORMAT_TI          = BIT(1),
    SPI_MODE_FORMAT_MICROWIRE   = BIT(2),

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
//! @}

//! \name spi configuration structure
//! @{
DEF_STRUCTURE(spi_cfg_t)
    uint16_t            hwMode;                     //!< spi working mode
    uint8_t             chDataSize;                 //!< frame size
    uint8_t             chClockDiv;                 //!< P_CLK prescaler
END_DEF_STRUCTURE(spi_cfg_t)
//! @}

/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal spi object
static spi_reg_t *const ptSPIREG = SPI_T_INIT(MINI_SPI_USE_SPI);

/*============================ IMPLEMENTATION ================================*/
/*! \brief enable spi
 *! \param void
 *! \retval fsm_rt_cpl spi enabled
 *! \retval fsm_rt_err illegal input pointer
 */
bool spi_open(void)
{
    // enable AHBCLK
    PM_AHB_CLK_ENABLE((em_ahb_clk_t)(AHBCLK_SPI0 + MINI_SPI_USE_SPI));
    PM_PCLK_CFG(PCLK_SPI0 + MINI_SPI_USE_SPI, 1 + SPI_PRESCALER);

    //! set spi enable bit
    this.CR1.Value |= SPI_CR1_SSE_MSK;

    return true;
}

/*! \brief disable spi
 *! \param void
 *! \retval fsm_rt_cpl close prosess is complete
 *! \retval fsm_rt_on_going close prosess is on going
 *! \retval fsm_rt_err illegal input pointer
 */
bool spi_close(void)
{
    //! clear spi enable bit
    this.CR1.Value &= ~SPI_CR1_SSE_MSK;

    // Disable AHBCLK
    PM_AHB_CLK_DISABLE((em_ahb_clk_t)(AHBCLK_SPI0 + MINI_SPI_USE_SPI));
    PM_PCLK_CFG(PCLK_SPI0 + MINI_SPI_USE_SPI, 0);

    return true;
}

/*! \brief initialize spi
 *! \param ptSpiCfg spi configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
bool spi_init(spi_cfg_t *ptSpiCfg)
{
    //! validate input parameter
    if (ptSpiCfg == NULL) {
        return false;
    }

    if ((ptSpiCfg->chClockDiv < 2ul) || (ptSpiCfg->chClockDiv > 254ul)) {
        return false;
    }

    spi_open();

    {
        //!  read CR0 register
        uint32_t *ptREG = (uint32_t *)&(this.CR0.Value);
        uint32_t wTempCR0 = (*ptREG) & ~(   SPI_CR0_DSS_MSK     |
                                           SPI_CR0_FRF_MSK     |
                                           SPI_CR0_CPOL_MSK    |
                                           SPI_CR0_CPHA_MSK    |
                                           SPI_CR0_SCR_MSK);

        //! set data size
        wTempCR0 |= SPI_DATASIZE_SET(SPI_MODE_DATASIZE_8);

        (*ptREG) = wTempCR0;        //! update CR0
    }

    {
        //! read CR1 register
        uint32_t wTempCR1 = this.CR1.Value & ~(
                                           SPI_CR1_LBM_MSK     |
                                           SPI_CR1_MS_MSK      |
                                           SPI_CR1_SOD_MSK
                                           );

        this.CR1.Value = wTempCR1;   //! update CR1
    }

    {
        uint32_t wTempCPSR = this.CPSR.Value & ~(SPI_CPSR_CPSDVSR_MSK);

        //! set clock prescaler
        wTempCPSR |= SPI_CPSR_SET(ptSpiCfg->chClockDiv & 0xFFFFFFFEul);

        this.CPSR.Value = wTempCPSR;  //! update CPSR
    }

    return true;
}

/*! \brief exchange data once
 *! \param hwOut the out-data
 *! \param *hwIn the addr of in-data
 *! \retval fsm_rt_on_going exchange is on going
 *! \retval fsm_rt_cpl exchange is complete
 *! \retval fsm_rt_err illegal input pointer
 */
bool spi_data_exchange(uint16_t hwOut, uint16_t *phwIn)
{
    uint16_t hwTemp;

    while (!(this.SR.Value & SPI_SR_TNF_MSK));   //!< tx fifo is full
    this.DR.Value = hwOut;

    while (!(this.SR.Value & SPI_SR_RNE_MSK));   //!< rx fifo is empty
    hwTemp = this.DR.Value;
    if (phwIn != NULL) {
        *phwIn = hwTemp;
    }

    return true;
}

//! end of file