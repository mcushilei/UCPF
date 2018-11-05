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
#ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_PM_PUBLIC_H__
#define __DRIVER_ARM_M4_AMBIQ_APOLLO2_PM_PUBLIC_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_pwrctrl.h"
#include ".\reg_mcuctrl.h"
#include ".\reg_clkgen.h"

/*============================ MACROS ========================================*/

//*****************************************************************************
//
// Peripheral enable bits for driver_pwrctrl_periph_enable/disable()
//
//*****************************************************************************
#define DRIVER_PWRCTRL_ADC      REG_PWRCTRL_DEVICEEN_PWRADC_M
#define DRIVER_PWRCTRL_IOM0     REG_PWRCTRL_DEVICEEN_IO_MASTER0_M
#define DRIVER_PWRCTRL_IOM1     REG_PWRCTRL_DEVICEEN_IO_MASTER1_M
#define DRIVER_PWRCTRL_IOM2     REG_PWRCTRL_DEVICEEN_IO_MASTER2_M
#define DRIVER_PWRCTRL_IOM3     REG_PWRCTRL_DEVICEEN_IO_MASTER3_M
#define DRIVER_PWRCTRL_IOM4     REG_PWRCTRL_DEVICEEN_IO_MASTER4_M
#define DRIVER_PWRCTRL_IOM5     REG_PWRCTRL_DEVICEEN_IO_MASTER5_M
#define DRIVER_PWRCTRL_IOS      REG_PWRCTRL_DEVICEEN_IO_SLAVE_M
#define DRIVER_PWRCTRL_PDM      REG_PWRCTRL_DEVICEEN_PWRPDM_M
#define DRIVER_PWRCTRL_UART0    REG_PWRCTRL_DEVICEEN_UART0_M
#define DRIVER_PWRCTRL_UART1    REG_PWRCTRL_DEVICEEN_UART1_M

//*****************************************************************************
//
// Memory enable values for driver_pwrctrl_memory_enable()
//
//*****************************************************************************
#define DRIVER_PWRCTRL_MEMEN_SRAM8K     REG_PWRCTRL_MEMEN_SRAMEN_SRAM8K
#define DRIVER_PWRCTRL_MEMEN_SRAM16K    REG_PWRCTRL_MEMEN_SRAMEN_SRAM16K
#define DRIVER_PWRCTRL_MEMEN_SRAM24K                    \
            (REG_PWRCTRL_MEMEN_SRAMEN_SRAM16K       |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP0_SRAM2)
#define DRIVER_PWRCTRL_MEMEN_SRAM32K    REG_PWRCTRL_MEMEN_SRAMEN_SRAM32K
#define DRIVER_PWRCTRL_MEMEN_SRAM64K    REG_PWRCTRL_MEMEN_SRAMEN_SRAM64K
#define DRIVER_PWRCTRL_MEMEN_SRAM96K                    \
            (REG_PWRCTRL_MEMEN_SRAMEN_SRAM64K       |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP2)
#define DRIVER_PWRCTRL_MEMEN_SRAM128K   REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K
#define DRIVER_PWRCTRL_MEMEN_SRAM160K                   \
            (REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K      |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP4)
#define DRIVER_PWRCTRL_MEMEN_SRAM192K                   \
            (REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K      |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP4        |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP5)
#define DRIVER_PWRCTRL_MEMEN_SRAM224K                   \
            (REG_PWRCTRL_MEMEN_SRAMEN_SRAM128K      |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP4        |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP5        |   \
             REG_PWRCTRL_MEMEN_SRAMEN_GROUP6)
#define DRIVER_PWRCTRL_MEMEN_SRAM256K   REG_PWRCTRL_MEMEN_SRAMEN_SRAM256K

#define DRIVER_PWRCTRL_MEMEN_FLASH512K  REG_PWRCTRL_MEMEN_FLASH0_M
#define DRIVER_PWRCTRL_MEMEN_FLASH1M                \
            (REG_PWRCTRL_MEMEN_FLASH0_M     |   \
             REG_PWRCTRL_MEMEN_FLASH1_M)
#define DRIVER_PWRCTRL_MEMEN_CACHE                  \
            (REG_PWRCTRL_MEMEN_CACHEB0_M    |   \
             REG_PWRCTRL_MEMEN_CACHEB2_M)
#define DRIVER_PWRCTRL_MEMEN_CACHE_DIS              \
            ~(REG_PWRCTRL_MEMEN_CACHEB0_M   |   \
              REG_PWRCTRL_MEMEN_CACHEB2_M)

//
// Power up all available memory devices (this is the default power up state)
//
#define DRIVER_PWRCTRL_MEMEN_ALL                    \
            (REG_PWRCTRL_MEMEN_SRAMEN_ALL    |   \
             REG_PWRCTRL_MEMEN_FLASH0_M     |   \
             REG_PWRCTRL_MEMEN_FLASH1_M     |   \
             REG_PWRCTRL_MEMEN_CACHEB0_M    |   \
             REG_PWRCTRL_MEMEN_CACHEB2_M)

#define DRIVER_CLKGEN_SYSCLK_MAX        0x00000000
#define DRIVER_CLKGEN_SYSCLK_48MHZ      REG_CLKGEN_CCTRL_CORESEL_HFRC
#define DRIVER_CLKGEN_SYSCLK_24MHZ      REG_CLKGEN_CCTRL_CORESEL_HFRC_DIV2

//*****************************************************************************
//
//! @name OSC Start and Stop
//! @brief OSC Start and Stop defines.
//!
//! OSC Start and Stop defines to be used with \e am_hal_clkgen_osc_x().
//! @{
//
//*****************************************************************************
#define DRIVER_CLKGEN_OSC_LFRC          REG_CLKGEN_OCTRL_STOPRC_M
#define DRIVER_CLKGEN_OSC_XT            REG_CLKGEN_OCTRL_STOPXT_M
//! @}

                
//*****************************************************************************
//
// OSC Start, Stop, Select defines
//
//*****************************************************************************
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC         REG_CLKGEN_CLKOUT_CKSEL_LFRC
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV2      REG_CLKGEN_CLKOUT_CKSEL_XT_DIV2
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV4      REG_CLKGEN_CLKOUT_CKSEL_XT_DIV4
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV8      REG_CLKGEN_CLKOUT_CKSEL_XT_DIV8
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV16     REG_CLKGEN_CLKOUT_CKSEL_XT_DIV16
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV32     REG_CLKGEN_CLKOUT_CKSEL_XT_DIV32
#define DRIVER_CLKGEN_CLKOUT_CKSEL_RTC_100Hz    REG_CLKGEN_CLKOUT_CKSEL_RTC_100Hz
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV2M     REG_CLKGEN_CLKOUT_CKSEL_XT_DIV2M
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT           REG_CLKGEN_CLKOUT_CKSEL_XT
#define DRIVER_CLKGEN_CLKOUT_CKSEL_CG_100Hz     REG_CLKGEN_CLKOUT_CKSEL_CG_100Hz
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC         REG_CLKGEN_CLKOUT_CKSEL_HFRC
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV4    REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV4
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV8    REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV8
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV32   REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV32
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64   REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV128  REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV128
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV256  REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV256
#define DRIVER_CLKGEN_CLKOUT_CKSEL_CORE_CLK     REG_CLKGEN_CLKOUT_CKSEL_CORE_CLK
#define DRIVER_CLKGEN_CLKOUT_CKSEL_FLASH_CLK    REG_CLKGEN_CLKOUT_CKSEL_FLASH_CLK
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2    REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32   REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC_DIV512  REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV512
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32K  REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV32K
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV256    REG_CLKGEN_CLKOUT_CKSEL_XT_DIV256
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV8K     REG_CLKGEN_CLKOUT_CKSEL_XT_DIV8K
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XT_DIV64K    REG_CLKGEN_CLKOUT_CKSEL_XT_DIV64K
#define DRIVER_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV16  REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV16
#define DRIVER_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV128 REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV128
#define DRIVER_CLKGEN_CLKOUT_CKSEL_ULFRC_1Hz    REG_CLKGEN_CLKOUT_CKSEL_ULFRC_1Hz
#define DRIVER_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV4K  REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV4K
#define DRIVER_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV1M  REG_CLKGEN_CLKOUT_CKSEL_ULFRC_DIV1M
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64K  REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV64K
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRC_DIV16M  REG_CLKGEN_CLKOUT_CKSEL_HFRC_DIV16M
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2M   REG_CLKGEN_CLKOUT_CKSEL_LFRC_DIV2M
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRCNE       REG_CLKGEN_CLKOUT_CKSEL_HFRCNE
#define DRIVER_CLKGEN_CLKOUT_CKSEL_HFRCNE_DIV8  REG_CLKGEN_CLKOUT_CKSEL_HFRCNE_DIV8
#define DRIVER_CLKGEN_CLKOUT_CKSEL_CORE_CLKNE   REG_CLKGEN_CLKOUT_CKSEL_CORE_CLKNE
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XTNE         REG_CLKGEN_CLKOUT_CKSEL_XTNE
#define DRIVER_CLKGEN_CLKOUT_CKSEL_XTNE_DIV16   REG_CLKGEN_CLKOUT_CKSEL_XTNE_DIV16
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRCNE_DIV32 REG_CLKGEN_CLKOUT_CKSEL_LFRCNE_DIV32
#define DRIVER_CLKGEN_CLKOUT_CKSEL_FCLKNE       REG_CLKGEN_CLKOUT_CKSEL_FCLKNE
#define DRIVER_CLKGEN_CLKOUT_CKSEL_LFRCNE       REG_CLKGEN_CLKOUT_CKSEL_LFRCNE

//*****************************************************************************
//
// UARTEN
//
//*****************************************************************************
#define DRIVER_CLKGEN_UARTEN_DIS          REG_CLKGEN_UARTEN_UART0EN_DIS
#define DRIVER_CLKGEN_UARTEN_EN           REG_CLKGEN_UARTEN_UART0EN_EN
#define DRIVER_CLKGEN_UARTEN_REDUCE_FREQ  REG_CLKGEN_UARTEN_UART0EN_REDUCE_FREQ
#define DRIVER_CLKGEN_UARTEN_EN_POWER_SAV REG_CLKGEN_UARTEN_UART0EN_EN_POWER_SAV

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool driver_pwrctrl_periph_enable(uint32_t ui32Peripheral);
extern bool driver_pwrctrl_periph_disable(uint32_t ui32Peripheral);
extern bool driver_pwrctrl_memory_enable(uint32_t ui32MemEn);
extern void driver_pwrctrl_bucks_init(void);
extern void driver_pwrctrl_bucks_enable(void);
extern void driver_pwrctrl_bucks_disable(void);
extern void driver_pwrctrl_low_power_init(void);

extern void     driver_clkgen_sysclk_select(uint32_t ui32ClockSetting);
extern uint32_t driver_clkgen_sysclk_get(void);
extern void     driver_clkgen_uarten_set(uint32_t ui32Module, uint32_t ui32UartEn);
extern void     driver_clkgen_osc_start(uint32_t ui32OscFlags);
extern void     driver_clkgen_osc_stop(uint32_t ui32OscFlags);
extern void     driver_clkgen_hfrc_adjust_enable(uint32_t ui32Warmup, uint32_t ui32Frequency);
extern void     driver_clkgen_hfrc_adjust_disable(void);

#endif  //!< #ifndef __DRIVER_ARM_M4_AMBIQ_APOLLO2_PM_PUBLIC_H__
/* EOF */
