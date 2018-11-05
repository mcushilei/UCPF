#ifndef __GSP_SPI_MINI_DRIVER_H__
#define __GSP_SPI_MINI_DRIVER_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\i_io_spi.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SPI_INIT(__SPI,...)                                                     \
    do {                                                                        \
        spi_cfg_t tCFG = {                                                      \
            __VA_ARGS__                                                         \
        };                                                                      \
        spi_init(&tCFG);                                                        \
    } while (false)

#define SPI_OPEN(__SPI)                                                         \
        spi_open()                                                              \

#define SPI_CLOSE(__SPI)                                                        \
        spi_close()                                                             \

#define SPI_DATA_EXCHANGE(__SPI, __OUT, __PIN)                                  \
        spi_data_exchange(__OUT, __PIN)                                         \

#define __SPI_CLK_DIV(__N, __Value)                                             \
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
   MREPEAT(127, __SPI_CLK_DIV, NULL)
} em_spiclk_div_t;
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
//! \brief spi interface function prototypes
extern bool spi_init(spi_cfg_t *ptSpiCfg);
extern bool spi_open(void);
extern bool spi_close(void);
extern bool spi_data_exchange(uint16_t hwOut, uint16_t *phwIn);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

#endif