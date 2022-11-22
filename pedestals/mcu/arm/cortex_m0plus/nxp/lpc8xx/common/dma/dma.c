/*******************************************************************************
 *  Copyright(C)2018-2021 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "..\sys\pm.h"
#include ".\dma.h"

/*============================ MACROS ========================================*/
#ifndef CHIP_DMA_MAX_CHANNEL_USED
#define CHIP_DMA_MAX_CHANNEL_USED    (18u)
#endif

#if CHIP_DMA_MAX_CHANNEL_USED > 18u
#error CHIP_DMA_MAX_CHANNEL_USED cannot great than 18!
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ PRIVATE VARIABLES =============================*/

/* Alignment of 512 bytes */
ALIGN(512) static DMA_CHDESC_T Chip_DMA_Table[CHIP_DMA_MAX_CHANNEL_USED];


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

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

void Chip_DMA_Init(dma_reg_t *pDMA)
{
	clock_enable(CLOCK_DMA);
    sysctrl_periph_reset(DMA_RST);
    Chip_DMA_SetSRAMBase( pDMA , (uint32_t)(Chip_DMA_Table));
}

void Chip_DMA_DeInit(dma_reg_t *pDMA)
{
	(void) pDMA;
	clock_disable(CLOCK_DMA);
}

