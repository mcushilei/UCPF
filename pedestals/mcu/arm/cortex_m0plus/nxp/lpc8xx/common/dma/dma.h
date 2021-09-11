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

/**
================================================================================
                        ##### How to use it #####
================================================================================

## Brief

quick start

1. initialise the DMA by calling Chip_DMA_Init()
2. provid the descriptor table by calling Chip_DMA_SetSRAMBase(). This tabel is 
   a array of DMA_CHDESC_T of which each item works as the start descriptor for 
   a DMA channel.
3. init the other descriptors if needed. (depend on the usage)
4. setup the transfer of a channel by calling Chip_DMA_SetupTranChannel() with 
   the start descriptor initial values.
5. setup the channel configration by calling Chip_DMA_SetupChannelConfig() and
   Chip_DMA_SetupChannelTransfer().
6. enabled the interrupt of the channel if needed by calling 
   Chip_DMA_EnableIntChannel().
7. if the channel is triggered by a event, calling Chip_DMATRIGMUX_SetInputTrig()
   to chosse the event.
8. enable the channel by calling Chip_DMA_EnableChannel(). if the DMA has been 
   enabled the channel starts work after that call.
9. enable DMA by calling Chip_DMA_Enable().
10. enable interrupt of DMA by calling NVIC_EnableIRQ(DMA_IRQn).

note

1. one dma descriptor can take up to 1024 transfes.

## usage

there are 3 different usages:

1. one-shot
2. ping-pang
3. linked-list

### one-shot

### ping-pang

a example:

```C
    ALIGN(16) DMA_CHDESC_T Descriptor_Start;
    ALIGN(16) DMA_CHDESC_T Descriptor_A;
    ALIGN(16) DMA_CHDESC_T Descriptor_B;
    uint32_t xfercount = length - 1;

	Chip_DMA_Init( &DMA0_REG );

    Chip_DMA_Disable( &DMA0_REG );
	Chip_DMA_SetSRAMBase( &DMA0_REG , (uint32_t)(Chip_DMA_Table));
    
    Descriptor_Start.source = (uint32_t)(&ADC0_REG.SEQ_GDAT[ADC_SEQA_IDX]);
	Descriptor_Start.dest   = (uint32_t)(&buf[xfercount]);
	Descriptor_Start.next   = (uint32_t)&Descriptor_B;
    
    Descriptor_A.xfercfg = REG_DMA_XFERCFG_CFGVALID_M
                        | REG_DMA_XFERCFG_RELOAD_M
                        | REG_DMA_XFERCFG_CLRTRIG_M
                        | REG_DMA_XFERCFG_SETINTA_M
                        | REG_DMA_XFERCFG_WIDTH_32
                        | REG_DMA_XFERCFG_DSTINC_1 
                        | REG_DMA_XFERCFG_SRCINC_0
                        | REG_DMA_XFERCFG_XFERCOUNT_SET( xfercount );
    Descriptor_A.source = (uint32_t)(&ADC0_REG.SEQ_GDAT[ADC_SEQA_IDX]);
    Descriptor_A.dest   = (uint32_t)(&buf[xfercount]);
    Descriptor_A.next   = (uint32_t)&Descriptor_B;   <---------------------------+
                                                                                 |
    Descriptor_B.xfercfg = REG_DMA_XFERCFG_CFGVALID_M                            |
                        | REG_DMA_XFERCFG_RELOAD_M                               |
                        | REG_DMA_XFERCFG_CLRTRIG_M                              |
                        | REG_DMA_XFERCFG_SETINTB_M     //!< we use two interrupt flags as a distiction between these two descriptors.
                        | REG_DMA_XFERCFG_WIDTH_32                               |
                        | REG_DMA_XFERCFG_DSTINC_1                               |
                        | REG_DMA_XFERCFG_SRCINC_0                               |
                        | REG_DMA_XFERCFG_XFERCOUNT_SET( xfercount );            |
    Descriptor_B.source = (uint32_t)(&ADC0_REG.SEQ_GDAT[ADC_SEQA_IDX]);          |
    Descriptor_B.dest   = (uint32_t)(&buf[xfercount + length]);                  |
    Descriptor_B.next   = (uint32_t)&Descriptor_A;   <---------------------------+  cross referencing between Descriptor_B and Descriptor_A
    
    if( !Chip_DMA_SetupTranChannel( &DMA0_REG, 1, &Descriptor_Start) ) {
        return;
    }

	Chip_DMA_SetupChannelConfig( &DMA0_REG, 1,
              DMA_CFG_HWTRIGEN
            | DMA_CFG_TRIGPOL_HIGH
            | DMA_CFG_TRIGTYPE_EDGE
            | DMA_CFG_TRIGBURST_BURST
            | DMA_CFG_BURSTPOWER_1 );
    
    Chip_DMA_SetupChannelTransfer( &DMA0_REG, 1,
              REG_DMA_XFERCFG_CFGVALID_M
            | REG_DMA_XFERCFG_RELOAD_M
            | REG_DMA_XFERCFG_CLRTRIG_M
            | REG_DMA_XFERCFG_SETINTA_M
            | REG_DMA_XFERCFG_WIDTH_32
            | REG_DMA_XFERCFG_DSTINC_1
            | REG_DMA_XFERCFG_SRCINC_0
            | REG_DMA_XFERCFG_XFERCOUNT_SET(xfercount) );
    
    Chip_DMA_EnableIntChannel(  &DMA0_REG, 1 );
    Chip_DMATRIGMUX_SetInputTrig( &DMATRIGMUX_REG, 1, DMATRIG_ADC_SEQA_IRQ );
    Chip_DMA_EnableChannel(  &DMA0_REG, 1 );
    Chip_DMA_Enable(  &DMA0_REG  );
    
	NVIC_EnableIRQ(DMA_IRQn);
```
    
  */
//! Do not move this pre-processor statement to other places
#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_DMA_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_DMA_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_dma.h"

/*============================ MACROS ========================================*/
/* DMA interrupt status bits (common) */
#define DMA_INTSTAT_ACTIVEINT       0x2		/*!< Summarizes whether any enabled interrupts are pending */
#define DMA_INTSTAT_ACTIVEERRINT    0x4		/*!< Summarizes whether any error interrupts are pending */

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

/* DMA channel source/address/next descriptor */
typedef struct {
	uint32_t  xfercfg;		/*!< Transfer configuration (only used in linked lists and ping-pong configs) */
	uint32_t  source;		/*!< DMA transfer source end address */
	uint32_t  dest;			/*!< DMA transfer desintation end address */
	uint32_t  next;			/*!< Link to next DMA descriptor, must be 16 byte aligned */
} DMA_CHDESC_T;

/*============================ GLOBAL VARIABLES ==============================*/


/* DMA SRAM table - this can be optionally used with the Chip_DMA_SetSRAMBase()
   function if a DMA SRAM table is needed. */
extern DMA_CHDESC_T Chip_DMA_Table[18];

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/**
 * @brief	Initialize DMA controller
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 */
static INLINE void Chip_DMA_Init(dma_reg_t *pDMA)
{
	(void) pDMA;
    
	clock_enable(CLOCK_DMA);
    sysctrl_periph_reset(DMA_RST);
}

/**
 * @brief	De-Initialize DMA controller
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 */
static INLINE void Chip_DMA_DeInit(dma_reg_t *pDMA)
{
	(void) pDMA;
	clock_disable(CLOCK_DMA);
}

/**
 * @brief	Enable DMA controller
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 */
static INLINE void Chip_DMA_Enable(dma_reg_t *pDMA)
{
	pDMA->CTRL = 1;
}

/**
 * @brief	Disable DMA controller
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 */
static INLINE void Chip_DMA_Disable(dma_reg_t *pDMA)
{
	pDMA->CTRL = 0;
}

/**
 * @brief	Get pending interrupt or error interrupts
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	An Or'ed value of DMA_INTSTAT_* types
 * @note	If any DMA channels have an active interrupt or error interrupt
 *			pending, this functional will a common status that applies to all
 *			channels.
 */
static INLINE uint32_t Chip_DMA_GetIntStatus(dma_reg_t *pDMA)
{
	return (pDMA->INTSTAT & ~REG_DMA_INTSTAT_RESERVED);
}

/**
 * @brief	Set DMA controller SRAM base address
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	base	: The base address where the DMA descriptors will be stored
 * @return	Nothing
 * @note	A 256 byte block of memory aligned on a 256 byte boundary must be
 *			provided for this function. It sets the base address used for
 *			DMA descriptor table (16 descriptors total that use 16 bytes each).<br>
 *
 *			A pre-defined table with correct alignment can be used for this
 *			function by calling Chip_DMA_SetSRAMBase(LPC_DMA, (uint32_t)(Chip_DMA_Table));
 */
static INLINE void Chip_DMA_SetSRAMBase(dma_reg_t *pDMA, uint32_t base)
{
	pDMA->SRAMBASE = base & ~REG_DMA_SRAMBASE_RESERVED;
}

/**
 * @brief	Returns DMA controller SRAM base address
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	The base address where the DMA descriptors are stored
 */
static INLINE uint32_t Chip_DMA_GetSRAMBase(dma_reg_t *pDMA)
{
	return (pDMA->SRAMBASE & ~REG_DMA_SRAMBASE_RESERVED);
}

/** @defgroup DMA_COMMON_8XX CHIP: LPC8xx DMA Controller driver common channel functions
 * @{
 */

/**
 * @brief	Enables a single DMA channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_EnableChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].ENABLESET = (1 << ch);
}

/**
 * @brief	Disables a single DMA channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_DisableChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].ENABLECLR = (1 << ch);
}

/**
 * @brief	Returns all enabled DMA channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	An Or'ed value of all enabled DMA channels (0 - 15)
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) is enabled. A low state is disabled.
 */
static INLINE uint32_t Chip_DMA_GetEnabledChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].ENABLESET & ~REG_DMA_ENABLESET_RESERVED);
}

/**
 * @brief	Returns all active DMA channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	An Or'ed value of all active DMA channels (0 - 15)
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) is active. A low state is inactive. A active
 *			channel indicates that a DMA operation has been started but
 *			not yet fully completed.
 */
static INLINE uint32_t Chip_DMA_GetActiveChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].ACTIVE & ~REG_DMA_ACTIVE_RESERVED);
}

/**
 * @brief	Returns all busy DMA channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	An Or'ed value of all busy DMA channels (0 - 15)
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) is busy. A low state is not busy. A DMA
 *			channel is considered busy when there is any operation
 *			related to that channel in the DMA controller�s internal
 *			pipeline.
 */
static INLINE uint32_t Chip_DMA_GetBusyChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].BUSY & ~REG_DMA_BUSY_RESERVED);
}

/**
 * @brief	Returns pending error interrupt status for all DMA channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	An Or'ed value of all channels (0 - 15) error interrupt status
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has a pending error interrupt. A low state
 *			indicates no error interrupt.
 */
static INLINE uint32_t Chip_DMA_GetErrorIntChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].ERRINT & ~REG_DMA_ERRINT_RESERVED);
}

/**
 * @brief	Clears a pending error interrupt status for a single DMA channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_ClearErrorIntChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].ERRINT = (1 << ch);
}

/**
 * @brief	Enables a single DMA channel's interrupt used in common DMA interrupt
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_EnableIntChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].INTENSET = (1 << ch);
}

/**
 * @brief	Disables a single DMA channel's interrupt used in common DMA interrupt
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_DisableIntChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].INTENCLR = (1 << ch);
}

/**
 * @brief	Returns all enabled interrupt channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an enabled interrupt for the channel.
 *			A low state indicates that the DMA channel will not contribute
 *			to the common DMA interrupt status.
 */
static INLINE uint32_t Chip_DMA_GetEnableIntChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].INTENSET & ~REG_DMA_INTENSET_RESERVED);
}

/**
 * @brief	Returns active A interrupt status for all channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active A interrupt for the channel.
 *			A low state indicates that the A interrupt is not active.
 */
static INLINE uint32_t Chip_DMA_GetActiveIntAChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].INTA & ~REG_DMA_INTA_RESERVED);
}

/**
 * @brief	Clears active A interrupt status for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_ClearActiveIntAChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].INTA = (1 << ch);
}

/**
 * @brief	Returns active B interrupt status for all channels
 * @param	pDMA	: The base of DMA controller on the chip
 * @return	Nothing
 * @note	A high values in bits 0 .. 15 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active B interrupt for the channel.
 *			A low state indicates that the B interrupt is not active.
 */
static INLINE uint32_t Chip_DMA_GetActiveIntBChannels(dma_reg_t *pDMA)
{
	return (pDMA->DMACOMMON[0].INTB & ~REG_DMA_INTB_RESERVED);
}

/**
 * @brief	Clears active B interrupt status for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_ClearActiveIntBChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].INTB = (1 << ch);
}

/**
 * @brief	Sets the VALIDPENDING control bit for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 * @note	See the User Manual for more information for what this bit does.
 *
 */
static INLINE void Chip_DMA_SetValidChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].SETVALID = (1 << ch);
}

/**
 * @brief	Sets the TRIG bit for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 * @note	See the User Manual for more information for what this bit does.
 */
static INLINE void Chip_DMA_SetTrigChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].SETTRIG = (1 << ch);
}

/**
 * @brief	Aborts a DMA operation for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 * @note	To abort a channel, the channel should first be disabled. Then wait
 *			until the channel is no longer busy by checking the corresponding
 *			bit in BUSY. Finally, abort the channel operation. This prevents the
 *			channel from restarting an incomplete operation when it is enabled
 *			again.
 */
static INLINE void Chip_DMA_AbortChannel(dma_reg_t *pDMA, uint32_t ch)
{
	pDMA->DMACOMMON[0].ABORT = (1 << ch);
}

/**
 * @}
 */

/** @defgroup DMA_CHANNEL_8XX CHIP: LPC8xx DMA Controller driver channel specific functions
 * @{
 */

/* Support definitions for setting the configuration of a DMA channel. You
   will need to get more information on these options from the User manual. */
#define DMA_CFG_PERIPHREQEN     (1 << 0)	/*!< Enables Peripheral DMA requests */
#define DMA_CFG_HWTRIGEN        (1 << 1)	/*!< Use hardware triggering via imput mux */
#define DMA_CFG_TRIGPOL_LOW     (0 << 4)	/*!< Hardware trigger is active low or falling edge */
#define DMA_CFG_TRIGPOL_HIGH    (1 << 4)	/*!< Hardware trigger is active high or rising edge */
#define DMA_CFG_TRIGTYPE_EDGE   (0 << 5)	/*!< Hardware trigger is edge triggered */
#define DMA_CFG_TRIGTYPE_LEVEL  (1 << 5)	/*!< Hardware trigger is level triggered */
#define DMA_CFG_TRIGBURST_SNGL  (0 << 6)	/*!< Single transfer. Hardware trigger causes a single transfer */
#define DMA_CFG_TRIGBURST_BURST (1 << 6)	/*!< Burst transfer (see UM) */
#define DMA_CFG_BURSTPOWER_1    (0 << 8)	/*!< Set DMA burst size to 1 transfer */
#define DMA_CFG_BURSTPOWER_2    (1 << 8)	/*!< Set DMA burst size to 2 transfers */
#define DMA_CFG_BURSTPOWER_4    (2 << 8)	/*!< Set DMA burst size to 4 transfers */
#define DMA_CFG_BURSTPOWER_8    (3 << 8)	/*!< Set DMA burst size to 8 transfers */
#define DMA_CFG_BURSTPOWER_16   (4 << 8)	/*!< Set DMA burst size to 16 transfers */
#define DMA_CFG_BURSTPOWER_32   (5 << 8)	/*!< Set DMA burst size to 32 transfers */
#define DMA_CFG_BURSTPOWER_64   (6 << 8)	/*!< Set DMA burst size to 64 transfers */
#define DMA_CFG_BURSTPOWER_128  (7 << 8)	/*!< Set DMA burst size to 128 transfers */
#define DMA_CFG_BURSTPOWER_256  (8 << 8)	/*!< Set DMA burst size to 256 transfers */
#define DMA_CFG_BURSTPOWER_512  (9 << 8)	/*!< Set DMA burst size to 512 transfers */
#define DMA_CFG_BURSTPOWER_1024 (10 << 8)	/*!< Set DMA burst size to 1024 transfers */
#define DMA_CFG_BURSTPOWER(n)   ((n) << 8)	/*!< Set DMA burst size to 2^n transfers, max n=10 */
#define DMA_CFG_SRCBURSTWRAP    (1 << 14)	/*!< Source burst wrapping is enabled for this DMA channel */
#define DMA_CFG_DSTBURSTWRAP    (1 << 15)	/*!< Destination burst wrapping is enabled for this DMA channel */
#define DMA_CFG_CHPRIORITY(p)   ((p) << 16)	/*!< Sets DMA channel priority, min 0 (highest), max 3 (lowest) */

/**
 * @brief	Setup a DMA channel configuration
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	cfg		: An Or'ed value of DMA_CFG_* values that define the channel's configuration
 * @return	Nothing
 * @note	This function sets up all configurable options for the DMA channel.
 *			These options are usually set once for a channel and then unchanged.<br>
 *
 *			The following example show how to configure the channel for peripheral
 *			DMA requests, burst transfer size of 1 (in 'transfers', not bytes),
 *			continuous reading of the same source address, incrementing destination
 *			address, and highest channel priority.<br>
 *			Example: Chip_DMA_SetupChannelConfig(pDMA, SSP0_RX_DMA,
 *				(DMA_CFG_PERIPHREQEN | DMA_CFG_TRIGBURST_BURST | DMA_CFG_BURSTPOWER_1 |
 *				DMA_CFG_SRCBURSTWRAP | DMA_CFG_CHPRIORITY(0)));<br>
 *
 *			The following example show how to configure the channel for an external
 *			trigger from the imput mux with low edge polarity, a burst transfer size of 8,
 *			incrementing source and destination addresses, and lowest channel
 *			priority.<br>
 *			Example: Chip_DMA_SetupChannelConfig(pDMA, DMA_CH14,
 *				(DMA_CFG_HWTRIGEN | DMA_CFG_TRIGPOL_LOW | DMA_CFG_TRIGTYPE_EDGE |
 *				DMA_CFG_TRIGBURST_BURST | DMA_CFG_BURSTPOWER_8 |
 *				DMA_CFG_CHPRIORITY(3)));<br>
 *
 *			For non-peripheral DMA triggering (DMA_CFG_HWTRIGEN definition), use the
 *			DMA input mux functions to configure the DMA trigger source for a DMA channel.
 */
static INLINE void Chip_DMA_SetupChannelConfig(dma_reg_t *pDMA, uint32_t ch, uint32_t cfg)
{
	pDMA->DMACH[ch].CFG = cfg;
}

/* DMA channel control and status register definitions */
#define DMA_CTLSTAT_VALIDPENDING    (1 << 0)	/*!< Valid pending flag for this channel */
#define DMA_CTLSTAT_TRIG            (1 << 2)	/*!< Trigger flag. Indicates that the trigger for this channel is currently set */

/**
 * @brief	Returns channel specific status flags
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	AN Or'ed value of DMA_CTLSTAT_VALIDPENDING and DMA_CTLSTAT_TRIG
 */
static INLINE uint32_t Chip_DMA_GetChannelStatus(dma_reg_t *pDMA, uint32_t ch)
{
	return (pDMA->DMACH[ch].CTLSTAT & ~REG_DMA_CTLSTAT_RESERVED);
}

/* DMA channel transfer configuration registers definitions */
#define DMA_XFERCFG_CFGVALID        (1 << 0)	/*!< Configuration Valid flag */
#define DMA_XFERCFG_RELOAD          (1 << 1)	/*!< Indicates whether the channels control structure will be reloaded when the current descriptor is exhausted */
#define DMA_XFERCFG_SWTRIG          (1 << 2)	/*!< Software Trigger */
#define DMA_XFERCFG_CLRTRIG         (1 << 3)	/*!< Clear Trigger */
#define DMA_XFERCFG_SETINTA         (1 << 4)	/*!< Set Interrupt flag A for this channel to fire when descriptor is complete */
#define DMA_XFERCFG_SETINTB         (1 << 5)	/*!< Set Interrupt flag B for this channel to fire when descriptor is complete */

#define REG_DMA_XFERCFG_WIDTH_8     REG_DMA_XFERCFG_WIDTH_SET(0)	/*!< 8-bit transfers are performed */
#define REG_DMA_XFERCFG_WIDTH_16    REG_DMA_XFERCFG_WIDTH_SET(1)	/*!< 16-bit transfers are performed */
#define REG_DMA_XFERCFG_WIDTH_32    REG_DMA_XFERCFG_WIDTH_SET(2)	/*!< 32-bit transfers are performed */

#define REG_DMA_XFERCFG_SRCINC_0    REG_DMA_XFERCFG_SRCINC_SET(0)	/*!< DMA source address is not incremented after a transfer */
#define REG_DMA_XFERCFG_SRCINC_1    REG_DMA_XFERCFG_SRCINC_SET(1)	/*!< DMA source address is incremented by 1 (width) after a transfer */
#define REG_DMA_XFERCFG_SRCINC_2    REG_DMA_XFERCFG_SRCINC_SET(2)	/*!< DMA source address is incremented by 2 (width) after a transfer */
#define REG_DMA_XFERCFG_SRCINC_4    REG_DMA_XFERCFG_SRCINC_SET(3)	/*!< DMA source address is incremented by 4 (width) after a transfer */

#define REG_DMA_XFERCFG_DSTINC_0    REG_DMA_XFERCFG_DSTINC_SET(0)	/*!< DMA destination address is not incremented after a transfer */
#define REG_DMA_XFERCFG_DSTINC_1    REG_DMA_XFERCFG_DSTINC_SET(1)	/*!< DMA destination address is incremented by 1 (width) after a transfer */
#define REG_DMA_XFERCFG_DSTINC_2    REG_DMA_XFERCFG_DSTINC_SET(2)	/*!< DMA destination address is incremented by 2 (width) after a transfer */
#define REG_DMA_XFERCFG_DSTINC_4    REG_DMA_XFERCFG_DSTINC_SET(3)	/*!< DMA destination address is incremented by 4 (width) after a transfer */

#define DMA_XFERCFG_XFERCOUNT(n)    ((n - 1) << 16)	/*!< DMA transfer count in 'transfers', between (0)1 and (1023)1024 */

/**
 * @brief	Setup a DMA channel transfer configuration
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	cfg		: An Or'ed value of DMA_XFERCFG_* values that define the channel's transfer configuration
 * @return	Nothing
 * @note	This function sets up the transfer configuration for the DMA channel.<br>
 *
 *			The following example show how to configure the channel's transfer for
 *			multiple transfer descriptors (ie, ping-pong), interrupt 'A' trigger on
 *			transfer descriptor completion, 128 byte size transfers, and source and
 *			destination address increment.<br>
 *			Example: Chip_DMA_SetupChannelTransfer(pDMA, SSP0_RX_DMA,
 *				(DMA_XFERCFG_CFGVALID | DMA_XFERCFG_RELOAD | DMA_XFERCFG_SETINTA |
 *				DMA_XFERCFG_WIDTH_8 | DMA_XFERCFG_SRCINC_1 | DMA_XFERCFG_DSTINC_1 |
 *				DMA_XFERCFG_XFERCOUNT(128)));<br>
 */
static INLINE void Chip_DMA_SetupChannelTransfer(dma_reg_t *pDMA, uint32_t ch, uint32_t cfg)
{
	pDMA->DMACH[ch].XFERCFG = cfg & ~REG_DMA_XFERCFG_RESERVED;
}

/**
 * @brief	Set DMA transfer register interrupt bits (safe)
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	mask	: Bits to set
 * @return	Nothing
 * @note	This function safely sets bits in the DMA channel specific XFERCFG
 *			register.
 */
static INLINE void Chip_DMA_SetChannelTransferBits(dma_reg_t *pDMA, uint32_t ch, uint32_t mask)
{
	/* Read and write values may not be the same, write 0 to
	   undefined bits */
	pDMA->DMACH[ch].XFERCFG = (pDMA->DMACH[ch].XFERCFG | mask) & ~REG_DMA_XFERCFG_RESERVED;
}

/**
 * @brief	Clear DMA transfer register interrupt bits (safe)
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	mask	: Bits to clear
 * @return	Nothing
 * @note	This function safely clears bits in the DMA channel specific XFERCFG
 *			register.
 */
static INLINE void Chip_DMA_ClearChannelTransferBits(dma_reg_t *pDMA, uint32_t ch, uint32_t mask)
{
	/* Read and write values may not be the same, write 0 to
	   undefined bits */
	pDMA->DMACH[ch].XFERCFG &= ~(REG_DMA_XFERCFG_RESERVED | mask);
}

/**
 * @brief	Update the transfer size in an existing DMA channel transfer configuration
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	trans	: Number of transfers to update the transfer configuration to (1 - 1023)
 * @return	Nothing
 */
static INLINE void Chip_DMA_SetupChannelTransferSize(dma_reg_t *pDMA, uint32_t ch, uint32_t trans)
{
	pDMA->DMACH[ch].XFERCFG = (pDMA->DMACH[ch].XFERCFG & ~(REG_DMA_XFERCFG_RESERVED | REG_DMA_XFERCFG_XFERCOUNT_M)) | REG_DMA_XFERCFG_XFERCOUNT_SET(trans);
}

/**
 * @brief	Sets a DMA channel configuration as valid
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_SetChannelValid(dma_reg_t *pDMA, uint32_t ch)
{
	Chip_DMA_SetChannelTransferBits(pDMA, ch, REG_DMA_XFERCFG_CFGVALID_M);
}

/**
 * @brief	Sets a DMA channel configuration as invalid
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_SetChannelInValid(dma_reg_t *pDMA, uint32_t ch)
{
	Chip_DMA_ClearChannelTransferBits(pDMA, ch, REG_DMA_XFERCFG_CFGVALID_M);
}

/**
 * @brief	Performs a software trigger of the DMA channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 */
static INLINE void Chip_DMA_SWTriggerChannel(dma_reg_t *pDMA, uint32_t ch)
{
	Chip_DMA_SetChannelTransferBits(pDMA, ch, REG_DMA_XFERCFG_SWTRIG_M);
}

/**
 * @brief	Checks if the given channel is active or not
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	1 if channel @a ch is active; 0 if channel @a ch is not active
 */
static INLINE bool Chip_DMA_IsChannelActive(dma_reg_t *pDMA, uint32_t ch)
{
	return (pDMA->DMACOMMON[0].ACTIVE & (1 << ch)) != 0;
}

/**
 * @brief	Sets up a DMA channel with the passed DMA transfer descriptor
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @param	desc	: Pointer to DMA transfer descriptor
 * @return	false if the DMA channel was active, otherwise true
 * @note	This function will set the DMA descriptor in the SRAM table to the
 *			the passed descriptor. This function is only meant to be used when
 *			the DMA channel is not active and can be used to setup the
 *			initial transfer for a linked list or ping-pong buffer or just a
 *			single transfer without a next descriptor.<br>
 *
 *			If using this function to write the initial transfer descriptor in
 *			a linked list or ping-pong buffer configuration, it should contain a
 *			non-NULL 'next' field pointer.
 */
static INLINE bool Chip_DMA_SetupTranChannel(dma_reg_t *pDMA, uint32_t ch, const DMA_CHDESC_T *desc)
{
	/* If channel is active return false */
	if( Chip_DMA_IsChannelActive( pDMA, ch ) )
		return false;

	/* Assign the descriptor to descriptor table */
	( (DMA_CHDESC_T *)(pDMA->SRAMBASE & ~REG_DMA_SRAMBASE_RESERVED) )[ch] = *desc;
	return true;
}


#endif  // #ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_DMA_H__
