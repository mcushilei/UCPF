/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\reg_spi.h"
#include "..\device.h"
#include "..\scon\pm.h"

/*============================ MACROS ========================================*/
//! \brief The reference for current object
#define this        (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        uint32_t wStatus   = PM_AHB_CLK_GET_STATUS(ptThis->tAHBClk);            \
        PM_AHB_CLK_ENABLE(ptThis->tAHBClk);                                     \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME(ptThis->tAHBClk, wStatus);                            \
    }

//! \brief Macro for spi init
#define __SPI_OBJ(__N,__DATA)                                                   \
    {                                                                           \
        ((spi_reg_t *)(SPI##__N##_BASE_ADDRESS)),                               \
        AHBCLK_SPI##__N,                                                        \
        PCLK_SPI##__N,                                                          \
    },

//! \brief Macro for spi interface init
#define __SPI_INTERFACE(__N,__A)                                                \
    {                                                                           \
        &spi##__N##_cfg,                                                       \
        &spi##__N##_open,                                                       \
        &spi##__N##_close,                                                      \
        &spi##__N##_idle,                                                       \
        &spi##__N##_write,                                                      \
        &spi##__N##_read,                                                       \
    },

//! \brief Macro for spi interface function prototypes
#define __SPI_INTERFACE_PROTOTYPES(__N,__DATA)                                  \
    extern bool spi##__N##_cfg(spi_cfg_t *ptSpiCfg);                            \
    extern bool spi##__N##_open(void);                                          \
    extern bool spi##__N##_close(void);                                         \
    extern bool spi##__N##_idle(void);                                          \
    extern bool spi##__N##_write(uint16_t hwOut);                               \
    extern bool spi##__N##_read(uint16_t *phwIn);

//! \brief Macro of spi modules interface function body
#define __SPI_INTERFACE_DEFINE(__N, __DATA)                                     \
    bool spi##__N##_cfg(spi_cfg_t *ptSpiCfg)                                    \
    {                                                                           \
        return spi_cfg(ptSpiCfg, (__spi_t *)&__SPI[__N]);                       \
    }                                                                           \
    bool spi##__N##_open(void)                                                  \
    {                                                                           \
        return spi_open((__spi_t *)&__SPI[__N]);                                \
    }                                                                           \
    bool spi##__N##_close(void)                                                 \
    {                                                                           \
        return spi_close((__spi_t *)&__SPI[__N]);                               \
    }                                                                           \
    bool spi##__N##_idle(void)                                                  \
    {                                                                           \
        return spi_is_idle((__spi_t *)&__SPI[__N]);                             \
    }                                                                           \
    bool spi##__N##_write(uint16_t hwOut)                                       \
    {                                                                           \
        return spi_write(hwOut, (__spi_t *)&__SPI[__N]);                        \
    }                                                                           \
    bool spi##__N##_read(uint16_t *phwIn)                                       \
    {                                                                           \
        return spi_read(phwIn, (__spi_t *)&__SPI[__N]);                         \
    }

#define __SPI_CLK_DIV(__N, __Value)                                             \
            SPI_PCLK_DIV_##__N = (2 * (__N)),

/*============================ TYPES =========================================*/
//! \name enum spi clk div
//! @{
typedef enum {
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
    spi_reg_t       *ptREG;                 //!< reference to register page
    em_ahb_clk_t    tAHBClk;
    em_pclk_t       tPClk;
END_DEF_STRUCTURE(__spi_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief spi interface function prototypes
MREPEAT(SPI_COUNT, __SPI_INTERFACE_PROTOTYPES, 0)

/*============================ LOCAL VARIABLES ===============================*/
//! \brief internal spi object
static const __spi_t __SPI[] = {
    MREPEAT(SPI_COUNT, __SPI_OBJ, 0)
};

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief SPI object
const i_spi_t SPI[] = {
    MREPEAT(SPI_COUNT, __SPI_INTERFACE, 0)
};

/*============================ IMPLEMENTATION ================================*/
/*! \brief initialize spi
 *! \param ptSpiCfg spi configuration object
 *! \retval true initialization succeed
 *! \retval false initialization failed
 */
static bool spi_cfg(spi_cfg_t *ptSpiCfg, __spi_t *ptThis)
{
    if ((ptSpiCfg->chDataSize < SPI_MODE_DATASIZE_4)
    ||  (ptSpiCfg->chDataSize > SPI_MODE_DATASIZE_16)) {
        return false;
    }

    if ((ptSpiCfg->chClockDiv < 2) || (ptSpiCfg->chClockDiv > 254)) {
        return false;
    }

    SAFE_CLK_CODE (
        spi_reg_t *ptREG = this.ptREG;

        //!  read CR0 register
        uint32_t wTempCR0 = ptREG->CR0.Value &
                            ~(  SPI_CR0_DSS_MSK     |
                                SPI_CR0_FRF_MSK     |
                                SPI_CR0_CPOL_MSK    |
                                SPI_CR0_CPHA_MSK    |
                                SPI_CR0_SCR_MSK);

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
            wTempCR0 |= SPI_CLK_SAMP_SET(SPI_SAMP_SECOND_EDGE);
        } else {
            //! samp at first edge
        }

        //! set data size
        wTempCR0 |= SPI_DATASIZE_SET(ptSpiCfg->chDataSize);
        
        //! update CR0
        ptREG->CR0.Value = wTempCR0;

        //! read CR1 register
        uint32_t wTempCR1 = ptREG->CR1.Value &
                            ~(  SPI_CR1_LBM_MSK     |
                                SPI_CR1_MS_MSK      |
                                SPI_CR1_SOD_MSK);

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
        ptREG->CR1.Value = wTempCR1;
        
        uint32_t wTempCPSR = ptREG->CPSR.Value & ~(SPI_CPSR_CPSDVSR_MSK);

        //! set clock prescaler
        wTempCPSR |= SPI_CPSR_SET(ptSpiCfg->chClockDiv & 0xFFFFFFFEul);
        
        //! update CPSR
        ptREG->CPSR.Value = wTempCPSR;
    )

    return true;
}

static uint8_t spi_calculate_pclk_prescaler(void)
{
    uint32_t wPrescaler = 1;
    uint32_t wMainClock = PM_MAIN_CLK_GET();

    if (wMainClock > MAX_SYSTEM_CLK_FREQ) {
        wPrescaler += (wMainClock - 1) / MAX_SYSTEM_CLK_FREQ;
    }

    return 1;//wPrescaler
}

/*! \brief enable spi
 *! \param void
 */
static bool spi_open(__spi_t *ptThis)
{
    //! enable AHBCLK
    PM_AHB_CLK_ENABLE(ptThis->tAHBClk);
    //! enable PCLK
    PM_PCLK_CFG(ptThis->tPClk, spi_calculate_pclk_prescaler());
    //! set spi enable bit
    this.ptREG->CR1.Value |= SPI_CR1_SSE_MSK;

    return true;
}

/*! \brief disable spi
 *! \param void
 */
static bool spi_close(__spi_t *ptThis)
{
    //! disable spi module
    this.ptREG->CR1.Value &=~ SPI_CR1_SSE_MSK;
    //! Disable AHBCLK
    PM_AHB_CLK_DISABLE(ptThis->tAHBClk);
    //! Disable PCLK
    PM_PCLK_CFG(ptThis->tPClk, 0);
    
    return true;
}

/*! \brief return state of spi
 *! \param void
 *! \retval true spi is idle
 *! \retval false spi is busy
 */
static bool spi_is_idle(__spi_t *ptThis)
{
    bool bResult = true;

    //! spi is busy, return false
    if (this.ptREG->SR.Value & SPI_SR_BSY_MSK) {
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
static bool spi_write(uint16_t hwOut, __spi_t *ptThis)
{
    //! if tx fifo is not full, write data
    if (this.ptREG->SR.Value & SPI_SR_TNF_MSK) {            //!< tx fifo is not full
        this.ptREG->DR.Value = hwOut;
        return true;
    }

    return false;
}

/*! \brief read a data with out sending
 *! \param phwIn the addr of in-data
 *! \retval true read access is success
 *! \retval false read access is failed or illegal input parameter
 */
static bool spi_read(uint16_t *phwIn, __spi_t *ptThis)
{
    //! if rx fifo is not empty, read data
    if (this.ptREG->SR.Value & SPI_SR_RNE_MSK) {            //!< rx fifo is not empty
        uint16_t hwRcv = this.ptREG->DR.Value;
        if (phwIn != NULL) {
            *phwIn = hwRcv;
        }
        return true;
    }

    return false;
}

//#define SPI_DATA_EXCHANGE_START        0
//#define SPI_DATA_EXCHANGE_SEND         1
//#define SPI_DATA_EXCHANGE_RECEIVE      2
//
//#define SPI_DATA_EXCHANGE_FSM_RST()\
//    do {\
//        ptThis->chStateExchange = SPI_DATA_EXCHANGE_START;\
//    } while(0)
//
//static bool spi_data_exchange(uint16_t hwOut, uint16_t *phwIn, __spi_t *ptThis)
//{
//    switch(this.chStateExchange) {
//        //! start
//        case SPI_DATA_EXCHANGE_START:
//            this.chStateExchange = SPI_DATA_EXCHANGE_SEND;
//            //break;
//        //! send data out
//        case SPI_DATA_EXCHANGE_SEND:
//            if (this.ptREG->SR.Value & SPI_SR_TNF_MSK) {    //!< tx fifo is not full
//                this.ptREG->DR.Value = hwOut;
//                this.chStateExchange = SPI_DATA_EXCHANGE_RECEIVE;
//            }
//            break;
//        //! read data in
//        case SPI_DATA_EXCHANGE_RECEIVE:
//            if (this.ptREG->SR.Value & SPI_SR_RNE_MSK) {    //!< rx fifo is not empty
//                uint16_t hwTemp;
//                hwTemp = this.ptREG->DR.Value;
//                if (phwIn != NULL) {
//                    *phwIn = hwTemp;
//                }
//                SPI_DATA_EXCHANGE_FSM_RST();
//                return fsm_rt_cpl;
//            }
//            break;
//    }
//
//    return fsm_rt_on_going;
//}

//! \brief Spi modules interface function body
MREPEAT(SPI_COUNT, __SPI_INTERFACE_DEFINE, NULL)

//! end of file