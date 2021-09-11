/*******************************************************************************
 *  Copyright(C)2020 by Dreistein<mcu_shilei@hotmail.com>                     *
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


#ifndef __DRIVER_ARM_M0PLUS_NXP_LPC8XX_REG_REG_DMA_H__
#define __DRIVER_ARM_M0PLUS_NXP_LPC8XX_REG_REG_DMA_H__

/*============================ INCLUDES ======================================*/
#include "../device.h"

/*============================ MACROS ========================================*/
#define DMA0_REG          (*(dma_reg_t *)    DMA0_BASE_ADDRESS)

/**
    PRINT_REG_DEFINR_B( DMA, CTRL, ENABLE, 0, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, CTRL, ENABLE );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_CTRL_ENABLE_S           (0)
#define REG_DMA_CTRL_ENABLE_M           (0x00000001u)
#define REG_DMA_CTRL_ENABLE_SET(v)      (((uint32_t)(v) << 0) & 0x00000001)
#define REG_DMA_CTRL_ENABLE_GET(v)      (((uint32_t)(v) & 0x00000001) >> 0)

#define REG_DMA_CTRL_RESERVED (~(\
      REG_DMA_CTRL_ENABLE_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, INTSTAT, ACTIVEINT, 1, 1 );
    PRINT_REG_DEFINR_B( DMA, INTSTAT, ACTIVEERRINT, 2, 2 );

    PRINT_REG_RESERVED_BEGIN(   DMA, INTSTAT, ACTIVEINT );
    PRINT_REG_RESERVED(         DMA, INTSTAT, ACTIVEERRINT );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_INTSTAT_ACTIVEINT_S     (1)
#define REG_DMA_INTSTAT_ACTIVEINT_M     (0x00000002u)
#define REG_DMA_INTSTAT_ACTIVEINT_SET(v)  (((uint32_t)(v) << 1) & 0x00000002)
#define REG_DMA_INTSTAT_ACTIVEINT_GET(v)  (((uint32_t)(v) & 0x00000002) >> 1)
#define REG_DMA_INTSTAT_ACTIVEERRINT_S  (2)
#define REG_DMA_INTSTAT_ACTIVEERRINT_M  (0x00000004u)
#define REG_DMA_INTSTAT_ACTIVEERRINT_SET(v)  (((uint32_t)(v) << 2) & 0x00000004)
#define REG_DMA_INTSTAT_ACTIVEERRINT_GET(v)  (((uint32_t)(v) & 0x00000004) >> 2)

#define REG_DMA_INTSTAT_RESERVED (~(\
      REG_DMA_INTSTAT_ACTIVEINT_M  \
    | REG_DMA_INTSTAT_ACTIVEERRINT_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, SRAMBASE, OFFSET, 31, 9 );

    PRINT_REG_RESERVED_BEGIN(   DMA, SRAMBASE, OFFSET );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_SRAMBASE_OFFSET_S       (9)
#define REG_DMA_SRAMBASE_OFFSET_M       (0xFFFFFE00u)
#define REG_DMA_SRAMBASE_OFFSET_SET(v)  (((uint32_t)(v) << 9) & 0xFFFFFE00)
#define REG_DMA_SRAMBASE_OFFSET_GET(v)  (((uint32_t)(v) & 0xFFFFFE00) >> 9)

#define REG_DMA_SRAMBASE_RESERVED (~(\
      REG_DMA_SRAMBASE_OFFSET_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, ENABLESET, ENA, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, ENABLESET, ENA );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_ENABLESET_ENA_S         (0)
#define REG_DMA_ENABLESET_ENA_M         (0x0003FFFFu)
#define REG_DMA_ENABLESET_ENA_SET(v)    (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_ENABLESET_ENA_GET(v)    (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_ENABLESET_RESERVED (~(\
      REG_DMA_ENABLESET_ENA_M ) \
)


/**
    PRINT_REG_DEFINR_B( DMA, ENABLECLR, CLR, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, ENABLECLR, CLR );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_ENABLECLR_CLR_S         (0)
#define REG_DMA_ENABLECLR_CLR_M         (0x0003FFFFu)
#define REG_DMA_ENABLECLR_CLR_SET(v)    (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_ENABLECLR_CLR_GET(v)    (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_ENABLECLR_RESERVED (~(\
      REG_DMA_ENABLECLR_CLR_M ) \
)


/**
    PRINT_REG_DEFINR_B( DMA, ACTIVE, ACT, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, ACTIVE, ACT );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_ACTIVE_ACT_S            (0)
#define REG_DMA_ACTIVE_ACT_M            (0x0003FFFFu)
#define REG_DMA_ACTIVE_ACT_SET(v)       (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_ACTIVE_ACT_GET(v)       (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_ACTIVE_RESERVED (~(\
      REG_DMA_ACTIVE_ACT_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, BUSY, BSY, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, BUSY, BSY );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_BUSY_BSY_S              (0)
#define REG_DMA_BUSY_BSY_M              (0x0003FFFFu)
#define REG_DMA_BUSY_BSY_SET(v)         (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_BUSY_BSY_GET(v)         (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_BUSY_RESERVED (~(\
      REG_DMA_BUSY_BSY_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, ERRINT, ERR, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, ERRINT, ERR );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_ERRINT_ERR_S            (0)
#define REG_DMA_ERRINT_ERR_M            (0x0003FFFFu)
#define REG_DMA_ERRINT_ERR_SET(v)       (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_ERRINT_ERR_GET(v)       (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_ERRINT_RESERVED (~(\
      REG_DMA_ERRINT_ERR_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, INTENSET, INTEN, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, INTENSET, INTEN );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_INTENSET_INTEN_S        (0)
#define REG_DMA_INTENSET_INTEN_M        (0x0003FFFFu)
#define REG_DMA_INTENSET_INTEN_SET(v)   (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_INTENSET_INTEN_GET(v)   (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_INTENSET_RESERVED (~(\
      REG_DMA_INTENSET_INTEN_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, INTENCLR, CLR, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, INTENCLR, CLR );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_INTENCLR_CLR_S          (0)
#define REG_DMA_INTENCLR_CLR_M          (0x0003FFFFu)
#define REG_DMA_INTENCLR_CLR_SET(v)     (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_INTENCLR_CLR_GET(v)     (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_INTENCLR_RESERVED (~(\
      REG_DMA_INTENCLR_CLR_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, INTA, IA, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, INTA, IA );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_INTA_IA_S               (0)
#define REG_DMA_INTA_IA_M               (0x0003FFFFu)
#define REG_DMA_INTA_IA_SET(v)          (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_INTA_IA_GET(v)          (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_INTA_RESERVED (~(\
      REG_DMA_INTA_IA_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, INTB, IB, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, INTB, IB );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_INTB_IB_S               (0)
#define REG_DMA_INTB_IB_M               (0x0003FFFFu)
#define REG_DMA_INTB_IB_SET(v)          (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_INTB_IB_GET(v)          (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_INTB_RESERVED (~(\
      REG_DMA_INTB_IB_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, SETVALID, SV, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, SETVALID, SV );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_SETVALID_SV_S           (0)
#define REG_DMA_SETVALID_SV_M           (0x0003FFFFu)
#define REG_DMA_SETVALID_SV_SET(v)      (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_SETVALID_SV_GET(v)      (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_SETVALID_RESERVED (~(\
      REG_DMA_SETVALID_SV_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, SETTRIG, TRIG, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, SETTRIG, TRIG );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_SETTRIG_TRIG_S          (0)
#define REG_DMA_SETTRIG_TRIG_M          (0x0003FFFFu)
#define REG_DMA_SETTRIG_TRIG_SET(v)     (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_SETTRIG_TRIG_GET(v)     (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_SETTRIG_RESERVED (~(\
      REG_DMA_SETTRIG_TRIG_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, ABORT, ABORTCTRL, 17, 0 );

    PRINT_REG_RESERVED_BEGIN(   DMA, ABORT, ABORTCTRL );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_ABORT_ABORTCTRL_S       (0)
#define REG_DMA_ABORT_ABORTCTRL_M       (0x0003FFFFu)
#define REG_DMA_ABORT_ABORTCTRL_SET(v)  (((uint32_t)(v) << 0) & 0x0003FFFF)
#define REG_DMA_ABORT_ABORTCTRL_GET(v)  (((uint32_t)(v) & 0x0003FFFF) >> 0)

#define REG_DMA_ABORT_RESERVED (~(\
      REG_DMA_ABORT_ABORTCTRL_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, CFG, PERIPHREQEN, 0, 0 );
    PRINT_REG_DEFINR_B( DMA, CFG, HWTRIGEN, 1, 1 );
    PRINT_REG_DEFINR_B( DMA, CFG, TRIGPOL, 4, 4 );
    PRINT_REG_DEFINR_B( DMA, CFG, TRIGTYPE, 5, 5 );
    PRINT_REG_DEFINR_B( DMA, CFG, TRIGBURST, 6, 6 );
    PRINT_REG_DEFINR_B( DMA, CFG, BURSTPOWER, 11, 8 );
    PRINT_REG_DEFINR_B( DMA, CFG, SRCBURSTWRAP, 14, 14 );
    PRINT_REG_DEFINR_B( DMA, CFG, DSTBURSTWRAP, 15, 15 );
    PRINT_REG_DEFINR_B( DMA, CFG, CHPRIORITY, 18, 16 );

    PRINT_REG_RESERVED_BEGIN(   DMA, CFG, PERIPHREQEN );
    PRINT_REG_RESERVED(         DMA, CFG, HWTRIGEN );
    PRINT_REG_RESERVED(         DMA, CFG, TRIGPOL );
    PRINT_REG_RESERVED(         DMA, CFG, TRIGTYPE );
    PRINT_REG_RESERVED(         DMA, CFG, TRIGBURST );
    PRINT_REG_RESERVED(         DMA, CFG, BURSTPOWER );
    PRINT_REG_RESERVED(         DMA, CFG, SRCBURSTWRAP );
    PRINT_REG_RESERVED(         DMA, CFG, DSTBURSTWRAP );
    PRINT_REG_RESERVED(         DMA, CFG, CHPRIORITY );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_CFG_PERIPHREQEN_S       (0)
#define REG_DMA_CFG_PERIPHREQEN_M       (0x00000001u)
#define REG_DMA_CFG_PERIPHREQEN_SET(v)  (((uint32_t)(v) << 0) & 0x00000001)
#define REG_DMA_CFG_PERIPHREQEN_GET(v)  (((uint32_t)(v) & 0x00000001) >> 0)
#define REG_DMA_CFG_HWTRIGEN_S          (1)
#define REG_DMA_CFG_HWTRIGEN_M          (0x00000002u)
#define REG_DMA_CFG_HWTRIGEN_SET(v)     (((uint32_t)(v) << 1) & 0x00000002)
#define REG_DMA_CFG_HWTRIGEN_GET(v)     (((uint32_t)(v) & 0x00000002) >> 1)
#define REG_DMA_CFG_TRIGPOL_S           (4)
#define REG_DMA_CFG_TRIGPOL_M           (0x00000010u)
#define REG_DMA_CFG_TRIGPOL_SET(v)      (((uint32_t)(v) << 4) & 0x00000010)
#define REG_DMA_CFG_TRIGPOL_GET(v)      (((uint32_t)(v) & 0x00000010) >> 4)
#define REG_DMA_CFG_TRIGTYPE_S          (5)
#define REG_DMA_CFG_TRIGTYPE_M          (0x00000020u)
#define REG_DMA_CFG_TRIGTYPE_SET(v)     (((uint32_t)(v) << 5) & 0x00000020)
#define REG_DMA_CFG_TRIGTYPE_GET(v)     (((uint32_t)(v) & 0x00000020) >> 5)
#define REG_DMA_CFG_TRIGBURST_S         (6)
#define REG_DMA_CFG_TRIGBURST_M         (0x00000040u)
#define REG_DMA_CFG_TRIGBURST_SET(v)    (((uint32_t)(v) << 6) & 0x00000040)
#define REG_DMA_CFG_TRIGBURST_GET(v)    (((uint32_t)(v) & 0x00000040) >> 6)
#define REG_DMA_CFG_BURSTPOWER_S        (8)
#define REG_DMA_CFG_BURSTPOWER_M        (0x00000F00u)
#define REG_DMA_CFG_BURSTPOWER_SET(v)   (((uint32_t)(v) << 8) & 0x00000F00)
#define REG_DMA_CFG_BURSTPOWER_GET(v)   (((uint32_t)(v) & 0x00000F00) >> 8)
#define REG_DMA_CFG_SRCBURSTWRAP_S      (14)
#define REG_DMA_CFG_SRCBURSTWRAP_M      (0x00004000u)
#define REG_DMA_CFG_SRCBURSTWRAP_SET(v)  (((uint32_t)(v) << 14) & 0x00004000)
#define REG_DMA_CFG_SRCBURSTWRAP_GET(v)  (((uint32_t)(v) & 0x00004000) >> 14)
#define REG_DMA_CFG_DSTBURSTWRAP_S      (15)
#define REG_DMA_CFG_DSTBURSTWRAP_M      (0x00008000u)
#define REG_DMA_CFG_DSTBURSTWRAP_SET(v)  (((uint32_t)(v) << 15) & 0x00008000)
#define REG_DMA_CFG_DSTBURSTWRAP_GET(v)  (((uint32_t)(v) & 0x00008000) >> 15)
#define REG_DMA_CFG_CHPRIORITY_S        (16)
#define REG_DMA_CFG_CHPRIORITY_M        (0x00070000u)
#define REG_DMA_CFG_CHPRIORITY_SET(v)   (((uint32_t)(v) << 16) & 0x00070000)
#define REG_DMA_CFG_CHPRIORITY_GET(v)   (((uint32_t)(v) & 0x00070000) >> 16)

#define REG_DMA_CFG_RESERVED (~(\
      REG_DMA_CFG_PERIPHREQEN_M  \
    | REG_DMA_CFG_HWTRIGEN_M  \
    | REG_DMA_CFG_TRIGPOL_M  \
    | REG_DMA_CFG_TRIGTYPE_M  \
    | REG_DMA_CFG_TRIGBURST_M  \
    | REG_DMA_CFG_BURSTPOWER_M  \
    | REG_DMA_CFG_SRCBURSTWRAP_M  \
    | REG_DMA_CFG_DSTBURSTWRAP_M  \
    | REG_DMA_CFG_CHPRIORITY_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, CTLSTAT, VALIDPENDING, 0, 0 );
    PRINT_REG_DEFINR_B( DMA, CTLSTAT, TRIG, 2, 2 );

    PRINT_REG_RESERVED_BEGIN(   DMA, CTLSTAT, VALIDPENDING );
    PRINT_REG_RESERVED(         DMA, CTLSTAT, TRIG );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_CTLSTAT_VALIDPENDING_S  (0)
#define REG_DMA_CTLSTAT_VALIDPENDING_M  (0x00000001u)
#define REG_DMA_CTLSTAT_VALIDPENDING_SET(v)  (((uint32_t)(v) << 0) & 0x00000001)
#define REG_DMA_CTLSTAT_VALIDPENDING_GET(v)  (((uint32_t)(v) & 0x00000001) >> 0)
#define REG_DMA_CTLSTAT_TRIG_S          (2)
#define REG_DMA_CTLSTAT_TRIG_M          (0x00000004u)
#define REG_DMA_CTLSTAT_TRIG_SET(v)     (((uint32_t)(v) << 2) & 0x00000004)
#define REG_DMA_CTLSTAT_TRIG_GET(v)     (((uint32_t)(v) & 0x00000004) >> 2)

#define REG_DMA_CTLSTAT_RESERVED (~(\
      REG_DMA_CTLSTAT_VALIDPENDING_M  \
    | REG_DMA_CTLSTAT_TRIG_M ) \
)

/**
    PRINT_REG_DEFINR_B( DMA, XFERCFG, CFGVALID, 0, 0 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, RELOAD, 1, 1 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, SWTRIG, 2, 2 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, CLRTRIG, 3, 3 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, SETINTA, 4, 4 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, SETINTB, 5, 5 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, WIDTH, 9, 8 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, SRCINC, 13, 12 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, DSTINC, 15, 14 );
    PRINT_REG_DEFINR_B( DMA, XFERCFG, XFERCOUNT, 25, 16 );

    PRINT_REG_RESERVED_BEGIN(   DMA, XFERCFG, CFGVALID );
    PRINT_REG_RESERVED(         DMA, XFERCFG, RELOAD );
    PRINT_REG_RESERVED(         DMA, XFERCFG, SWTRIG );
    PRINT_REG_RESERVED(         DMA, XFERCFG, CLRTRIG );
    PRINT_REG_RESERVED(         DMA, XFERCFG, SETINTA );
    PRINT_REG_RESERVED(         DMA, XFERCFG, SETINTB );
    PRINT_REG_RESERVED(         DMA, XFERCFG, WIDTH );
    PRINT_REG_RESERVED(         DMA, XFERCFG, SRCINC );
    PRINT_REG_RESERVED(         DMA, XFERCFG, DSTINC );
    PRINT_REG_RESERVED(         DMA, XFERCFG, XFERCOUNT );
    PRINT_REG_RESERVED_END();
 */
#define REG_DMA_XFERCFG_CFGVALID_S      (0)
#define REG_DMA_XFERCFG_CFGVALID_M      (0x00000001u)
#define REG_DMA_XFERCFG_CFGVALID_SET(v)  (((uint32_t)(v) << 0) & 0x00000001)
#define REG_DMA_XFERCFG_CFGVALID_GET(v)  (((uint32_t)(v) & 0x00000001) >> 0)
#define REG_DMA_XFERCFG_RELOAD_S        (1)
#define REG_DMA_XFERCFG_RELOAD_M        (0x00000002u)
#define REG_DMA_XFERCFG_RELOAD_SET(v)   (((uint32_t)(v) << 1) & 0x00000002)
#define REG_DMA_XFERCFG_RELOAD_GET(v)   (((uint32_t)(v) & 0x00000002) >> 1)
#define REG_DMA_XFERCFG_SWTRIG_S        (2)
#define REG_DMA_XFERCFG_SWTRIG_M        (0x00000004u)
#define REG_DMA_XFERCFG_SWTRIG_SET(v)   (((uint32_t)(v) << 2) & 0x00000004)
#define REG_DMA_XFERCFG_SWTRIG_GET(v)   (((uint32_t)(v) & 0x00000004) >> 2)
#define REG_DMA_XFERCFG_CLRTRIG_S       (3)
#define REG_DMA_XFERCFG_CLRTRIG_M       (0x00000008u)
#define REG_DMA_XFERCFG_CLRTRIG_SET(v)  (((uint32_t)(v) << 3) & 0x00000008)
#define REG_DMA_XFERCFG_CLRTRIG_GET(v)  (((uint32_t)(v) & 0x00000008) >> 3)
#define REG_DMA_XFERCFG_SETINTA_S       (4)
#define REG_DMA_XFERCFG_SETINTA_M       (0x00000010u)
#define REG_DMA_XFERCFG_SETINTA_SET(v)  (((uint32_t)(v) << 4) & 0x00000010)
#define REG_DMA_XFERCFG_SETINTA_GET(v)  (((uint32_t)(v) & 0x00000010) >> 4)
#define REG_DMA_XFERCFG_SETINTB_S       (5)
#define REG_DMA_XFERCFG_SETINTB_M       (0x00000020u)
#define REG_DMA_XFERCFG_SETINTB_SET(v)  (((uint32_t)(v) << 5) & 0x00000020)
#define REG_DMA_XFERCFG_SETINTB_GET(v)  (((uint32_t)(v) & 0x00000020) >> 5)
#define REG_DMA_XFERCFG_WIDTH_S         (8)
#define REG_DMA_XFERCFG_WIDTH_M         (0x00000300u)
#define REG_DMA_XFERCFG_WIDTH_SET(v)    (((uint32_t)(v) << 8) & 0x00000300)
#define REG_DMA_XFERCFG_WIDTH_GET(v)    (((uint32_t)(v) & 0x00000300) >> 8)
#define REG_DMA_XFERCFG_SRCINC_S        (12)
#define REG_DMA_XFERCFG_SRCINC_M        (0x00003000u)
#define REG_DMA_XFERCFG_SRCINC_SET(v)   (((uint32_t)(v) << 12) & 0x00003000)
#define REG_DMA_XFERCFG_SRCINC_GET(v)   (((uint32_t)(v) & 0x00003000) >> 12)
#define REG_DMA_XFERCFG_DSTINC_S        (14)
#define REG_DMA_XFERCFG_DSTINC_M        (0x0000C000u)
#define REG_DMA_XFERCFG_DSTINC_SET(v)   (((uint32_t)(v) << 14) & 0x0000C000)
#define REG_DMA_XFERCFG_DSTINC_GET(v)   (((uint32_t)(v) & 0x0000C000) >> 14)
#define REG_DMA_XFERCFG_XFERCOUNT_S     (16)
#define REG_DMA_XFERCFG_XFERCOUNT_M     (0x03FF0000u)
#define REG_DMA_XFERCFG_XFERCOUNT_SET(v)  (((uint32_t)(v) << 16) & 0x03FF0000)
#define REG_DMA_XFERCFG_XFERCOUNT_GET(v)  (((uint32_t)(v) & 0x03FF0000) >> 16)

#define REG_DMA_XFERCFG_RESERVED (~(\
      REG_DMA_XFERCFG_CFGVALID_M  \
    | REG_DMA_XFERCFG_RELOAD_M  \
    | REG_DMA_XFERCFG_SWTRIG_M  \
    | REG_DMA_XFERCFG_CLRTRIG_M  \
    | REG_DMA_XFERCFG_SETINTA_M  \
    | REG_DMA_XFERCFG_SETINTB_M  \
    | REG_DMA_XFERCFG_WIDTH_M  \
    | REG_DMA_XFERCFG_SRCINC_M  \
    | REG_DMA_XFERCFG_DSTINC_M  \
    | REG_DMA_XFERCFG_XFERCOUNT_M ) \
)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

/**
 * @brief DMA Controller channel registers structure
 */
typedef struct {
    reg32_t  CFG;				/*!< DMA Configuration register */
    reg32_t  CTLSTAT;			/*!< DMA Control and status register */
    reg32_t  XFERCFG;			/*!< DMA Transfer configuration register */
    REG32_RSVD(1)
} LPC_DMA_CHANNEL_T;

/**
 * @brief DMA Controller shared registers structure
 */
typedef struct {
    reg32_t  ENABLESET;		/*!< DMA Channel Enable read and Set for all DMA channels */
    REG32_RSVD(1)
    reg32_t  ENABLECLR;		/*!< DMA Channel Enable Clear for all DMA channels */
    REG32_RSVD(1)
    reg32_t  ACTIVE;        /*!< DMA Channel Active status for all DMA channels */
    REG32_RSVD(1)
    reg32_t  BUSY;			/*!< DMA Channel Busy status for all DMA channels */
    REG32_RSVD(1)
    reg32_t  ERRINT;        /*!< DMA Error Interrupt status for all DMA channels */
    REG32_RSVD(1)
    reg32_t  INTENSET;		/*!< DMA Interrupt Enable read and Set for all DMA channels */
    REG32_RSVD(1)
    reg32_t  INTENCLR;		/*!< DMA Interrupt Enable Clear for all DMA channels */
    REG32_RSVD(1)
    reg32_t  INTA;			/*!< DMA Interrupt A status for all DMA channels */
    REG32_RSVD(1)
    reg32_t  INTB;			/*!< DMA Interrupt B status for all DMA channels */
    REG32_RSVD(1)
    reg32_t  SETVALID;		/*!< DMA Set ValidPending control bits for all DMA channels */
    REG32_RSVD(1)
    reg32_t  SETTRIG;       /*!< DMA Set Trigger control bits for all DMA channels */
    REG32_RSVD(1)
    reg32_t  ABORT;			/*!< DMA Channel Abort control for all DMA channels */
} LPC_DMA_COMMON_T;


/** verified by:
    PRINT_OFFSET_OF( dma_reg_t, CTRL );
    PRINT_OFFSET_OF( dma_reg_t, INTSTAT );
    PRINT_OFFSET_OF( dma_reg_t, SRAMBASE );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].ENABLESET );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].ENABLECLR );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].ACTIVE );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].BUSY );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].ERRINT );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].INTENSET );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].INTENCLR );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].INTA );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].INTB );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].SETVALID );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].SETTRIG );
    PRINT_OFFSET_OF( dma_reg_t, DMACOMMON[0].ABORT );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[0].CFG );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[0].CTLSTAT );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[0].XFERCFG );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[1].CFG );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[1].CTLSTAT );
    PRINT_OFFSET_OF( dma_reg_t, DMACH[1].XFERCFG );
 */

/**
 * @brief DMA Controller register block structure
 */
typedef struct {
	reg32_t             CTRL;			/*!< DMA control register */
	reg32_t             INTSTAT;	    /*!< DMA Interrupt status register */
	reg32_t             SRAMBASE;		/*!< DMA SRAM address of the channel configuration table */
    REG32_RSVD(5)
	LPC_DMA_COMMON_T    DMACOMMON[1];	/*!< DMA shared channel (common) registers */
    REG32_RSVD(225)
	LPC_DMA_CHANNEL_T   DMACH[18];	/*!< DMA channel registers */
} dma_reg_t;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif 
