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

#ifndef __DRIVER_SPI_H__
#define __DRIVER_SPI_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_spi.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __SPI_CLK_DIV(__N,__Value)     \
            SPI_PCLK_DIV_##__N = (2 * (__N)),

//! \brief Macro for extern spi interface function prototypes
#define __EXTERN_SPI_INTERFACE(__N, __DATA)                                 \
    extern bool spi##__N##_cfg(spi_cfg_t *ptSspCfg);                       \
    extern bool spi##__N##_open(void);                                      \
    extern bool spi##__N##_close(void);                                     \
    extern bool spi##__N##_idle(void);                                      \
    extern bool spi##__N##_write(uint16_t hwOut);                           \
    extern bool spi##__N##_read(uint16_t *phwIn);

/*============================ TYPES =========================================*/
//! \name spi working mode
//! @{
enum {
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
};
//! @}

//! \name spi datasize
//! @{
enum {
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
};

//! \name enum spi clk div
//! @{
enum{
    MREPEAT(127,__SPI_CLK_DIV ,NULL)
};
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
    bool (*Config)(spi_cfg_t *ptCfg);           //!< Config spi
    bool (*Open)(void);                         //!< open spi
    bool (*Close)(void);                        //!< close spi
    bool (*Idle)(void);                         //!< get busy flag status
    bool (*Write)(uint16_t Data);              //!< write data to spi buffer
    bool (*Read)(uint16_t *pData);              //!< read data from spi buffer
END_DEF_INTERFACE(i_spi_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief SPI objects
extern const i_spi_t SPI[SSP_COUNT];

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
//! \brief spi interface function prototypes
MREPEAT(SSP_COUNT, __EXTERN_SPI_INTERFACE, NULL)

extern bool spi_config(spi_cfg_t *ptSpiCfg);
extern bool spi_write(uint16_t Data);
extern bool spi_read(uint16_t *pData);
extern bool spi_enable(void);
extern bool spi_disable(void);

#endif
