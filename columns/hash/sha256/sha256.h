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


//! \note do not move this pre-processor statement to other places
#ifndef __SHA256_C__
#ifndef __SHA256_H__
#define __SHA256_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/**
 * \brief   SHA-256 context structure
 */
typedef struct {
    uint32_t wTotal[2];         /*!< number of bytes processed  */
    uint32_t wState[8];         /*!< intermediate digest state  */
    uint8_t  chBuffer[64];      /*!< data block being processed */
} sha256_context_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void sha256_starts(sha256_context_t *ptCtx);
extern void sha256_update(sha256_context_t *ptCtx, const uint8_t *pchData, uint32_t wLen);
extern void sha256_finish(sha256_context_t *ptCtx, uint8_t *pchChecksum);
extern void sha256_checksum(uint8_t *pchChecksum, const uint8_t *pchData, uint32_t wLen);

#endif  //!< #ifndef __SHA256_H__
#endif  //!< #ifndef __SHA256_C__
/* EOF */
