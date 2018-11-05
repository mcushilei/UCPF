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

#ifndef __SWSPI_TEMPLATE_H__
#define __SWSPI_TEMPLATE_H__


typedef enum {
    SW_SPI_MODE_MASTER             = 0x00,             //!< select master mode
    SW_SPI_MODE_SLAVE              = _BV(0),           //!< select slave mode

    SW_SPI_MODE_FORMAT_SPI         = 0x00,             //!< use standard spi fram
    SW_SPI_MODE_FORMAT_TI          = _BV(1),
    SW_SPI_MODE_FORMAT_MICROWIRE   = _BV(2),

    SW_SPI_MODE_CLK_IDLE_HIGH      = _BV(3),           //!< SCK is high in idle
    SW_SPI_MODE_CLK_IDLE_LOW       = 0x00,             //!< SCK is low in idle

    SW_SPI_MODE_SAMP_FIRST_EDGE    = 0x00,             //!< sample at first edge of sck
    SW_SPI_MODE_SAMP_SECOND_EDGE   = _BV(4),           //!< sample at second edge of sck

    SW_SPI_MODE_LOOP_BACK          = _BV(5),           //!< enable loop back
    SW_SPI_MODE_NOT_LOOP_BACK      = 0x00,             //!< default disable loop back

    SW_SPI_MODE_SLAVE_OUT_ENABLE   = 0x00,             //!< default enable slave output
    SW_SPI_MODE_SLAVE_OUT_DISABLE  = _BV(6)            //!< disable slave output
} em_sw_spi_mode_t;

typedef enum {
    SW_SPI_MODE_DATASIZE_4         = 0x3,              //!< datasize is 4 bits
    SW_SPI_MODE_DATASIZE_5         = 0x4,              //!< datasize is 5 bits
    SW_SPI_MODE_DATASIZE_6         = 0x5,              //!< datasize is 6 bits
    SW_SPI_MODE_DATASIZE_7         = 0x6,              //!< datasize is 7 bits
    SW_SPI_MODE_DATASIZE_8         = 0x7,              //!< datasize is 8 bits
    SW_SPI_MODE_DATASIZE_9         = 0x8,              //!< datasize is 9 bits
    SW_SPI_MODE_DATASIZE_10        = 0x9,              //!< datasize is 10 bits
    SW_SPI_MODE_DATASIZE_11        = 0xA,              //!< datasize is 11 bits
    SW_SPI_MODE_DATASIZE_12        = 0xB,              //!< datasize is 12 bits
    SW_SPI_MODE_DATASIZE_13        = 0xC,              //!< datasize is 13 bits
    SW_SPI_MODE_DATASIZE_14        = 0xD,              //!< datasize is 14 bits
    SW_SPI_MODE_DATASIZE_15        = 0xE,              //!< datasize is 15 bits
    SW_SPI_MODE_DATASIZE_16        = 0xF               //!< datasize is 16 bits
} em_sw_spi_datasize_t;

#define __SW_SPI_CLK_DIV(__N,__Value)           \
            SW_SPI_PCLK_DIV_##__N = (2*(__N)),

typedef enum{
   MREPEAT(127,__SW_SPI_CLK_DIV ,NULL)
} em_sw_spiclk_div_t;

DEF_STRUCTURE(sw_spi_cfg_t)
    union {
        uint32_t        hwMode;
        struct {
            uint32_t                :24;
            uint32_t chDataSize     :8;
        };
    };
    em_sw_spiclk_div_t  chClockDiv;
END_DEF_STRUCTURE(sw_spi_cfg_t)

#define END_EXTERN_SWSPI
#define END_DEFINE_SWSPI

#define SW_SPI_INIT(__NAME, __CFG)         __NAME##_sw_spi_init(__CFG)
#define SW_SPI_WRITE(__NAME, __D)          __NAME##_sw_spi_write(__D)
#define SW_SPI_READ(__NAME, __PD)          __NAME##_sw_spi_read(__PD)
#define SW_SPI_EXCHANGE(__NAME, __W, __PR) __NAME##_sw_spi_exchange(__W, __PR)
#define SW_SPI_TASK(__NAME)                __NAME##_sw_spi_task()


#define EXTERN_SWSPI(__NAME)                                                    \
extern void __NAME##_sw_spi_init(sw_spi_cfg_t *ptCFG);                          \
extern bool __NAME##_sw_spi_write(uint16_t hwData);                             \
extern bool __NAME##_sw_spi_read(uint16_t *phwData);                            \
extern fsm_rt_t __NAME##_sw_spi_exchange(uint16_t hwWrite, uint16_t *phwRead);  \
extern void __NAME##_sw_spi_task(void);


#define SW_SPI_SPIF                 (0x00000001)
#define SW_SPI_NEW                  (0x00010000)
#define SW_SPI_BUSY                 (0x00020000)

#define SW_SPI_LSBF                 (0x00000100)
#define SW_SPI_MSTR                 (0x00000200)
#define SW_SPI_CPHA                 (0x00000400)
#define SW_SPI_CPOL                 (0x00000800)


#define SW_SPI_EXCHANGE_TRANS_STATE_FSM(__S)  do {\
                                                this.tExchangeFSM = __S;\
                                            } while (false)

#define SW_SPI_EXCHANGE_RESET_FSM()     do {\
                                            SW_SPI_EXCHANGE_TRANS_STATE_FSM(EXCHANGE_START);\
                                        } while (false)

#define SW_SPI_M_SHIFT_TRANS_STATE_FSM(__S)  do {\
                                                this.tMasterShiftFSM = __S;\
                                            } while (false)

#define SW_SPI_M_SHIFT_RESET_FSM()  do {\
                                        SW_SPI_M_SHIFT_TRANS_STATE_FSM(M_SHIFT_START);\
                                    } while (false)

#define SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(__S)  do {\
                                                this.tMasterTransFSM = __S;\
                                            } while (false)

#define SW_SPI_M_TRANSCEIVER_RESET_FSM()    do {\
                                                SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(M_TRANS_START);\
                                            } while (false)

#define SW_SPI_M_TASK_TRANS_STATE_FSM(__S)  do {\
                                                this.tMasterTaskFSM = __S;\
                                            } while (false)

#define SW_SPI_M_TASK_RESET_FSM()   do {\
                                        SW_SPI_M_TASK_TRANS_STATE_FSM(M_TASK_START);\
                                    } while (false)

#define SW_SPI_S_SHIFT_TRANS_STATE_FSM(__S)  do {\
                                                this.tSlaveShiftFSM = __S;\
                                            } while (false)

#define SW_SPI_S_SHIFT_RESET_FSM()  do {\
                                        SW_SPI_S_SHIFT_TRANS_STATE_FSM(S_SHIFT_START);\
                                    } while (false)

#define SW_SPI_S_TRANSCEIVER_TRANS_STATE_FSM(__S)  do {\
                                                this.tSlaveTransFSM = __S;\
                                            } while (false)

#define SW_SPI_S_TRANSCEIVER_RESET_FSM()    do {\
                                                SW_SPI_S_SHIFT_RESET_FSM();\
                                                SW_SPI_S_TRANSCEIVER_TRANS_STATE_FSM(S_TRANS_START);\
                                            } while (false)

#define SW_SPI_S_TASK_TRANS_STATE_FSM(__S)  do {\
                                                this.tSlaveTaskFSM = __S;\
                                            } while (false)

#define SW_SPI_S_TASK_RESET_FSM()   do {\
                                        SW_SPI_S_TASK_TRANS_STATE_FSM(S_TASK_START);\
                                    } while (false)

#define SW_SPI_OUT_BIT(__BIT)                                                   \
    do {                                                                        \
        if (BIT((__BIT)) & this.hwSnd) {                                        \
            SW_SPI_MOSI_H();                                                    \
        } else {                                                                \
            SW_SPI_MOSI_L();                                                    \
        }                                                                       \
    } while (0)

#define SW_SPI_IN_BIT(__BIT)                                                    \
    do {                                                                        \
        if (SW_SPI_MISO()) {                                                    \
            this.hwRcv |= BIT((__BIT));                                         \
        }                                                                       \
    } while (0)

#define GET_CLK_LEVEL(__PL)         (SW_SPI_CLK()? (*(__PL) = 1) : (*(__PL) = 0))
#define GET_SS_LEVEL(__PL)          (SW_SPI_SS()? (*(__PL) = 1) : (*(__PL) = 0))

#define this                        (*s_ptThis)


#define DEFINE_SWSPI(__NAME)                                                    \
DEF_CLASS(sw_spi_t)                                                             \
    union {                                                                     \
        uint8_t chBITS;                                                         \
        uint32_t wCTRL;                                                         \
    };                                                                          \
    uint32_t wSPDW;                                                             \
    uint32_t wSPSW;                                                             \
    uint32_t wSPCCW;                                                            \
    enum {                                                                      \
        M_SHIFT_START = 0,                                                      \
        TIME_PRE_EDGE,                                                          \
        TIME_LAT_EDGE,                                                          \
    } tMasterShiftFSM;                                                          \
    enum {                                                                      \
        M_TRANS_START = 0,                                                      \
        DELAY_PRE_FRAME,                                                        \
        M_SHIFT,                                                                \
        DELAY_POST_FRAME,                                                       \
        DELAY_SS_HIGH,                                                          \
    } tMasterTransFSM;                                                          \
    enum {                                                                      \
        M_TASK_START = 0,                                                       \
        CHECK_BOX,                                                              \
        M_TRANSCEIVER,                                                          \
    } tMasterTaskFSM;                                                           \
    enum {                                                                      \
        S_SHIFT_START = 0,                                                      \
        SHIFT_CHECK_PRE_EDGE,                                                   \
        SHIFT_CHECK_LAT_EDGE,                                                   \
    } tSlaveShiftFSM;                                                           \
    enum {                                                                      \
        S_TRANS_START = 0,                                                      \
        WAIT_SS_LOW,                                                            \
        S_SHIFT,                                                                \
    } tSlaveTransFSM;                                                           \
    enum {                                                                      \
        S_TASK_START = 0,                                                       \
        S_TRANSCEIVER,                                                          \
    } tSlaveTaskFSM;                                                            \
    enum {                                                                      \
        EXCHANGE_START = 0,                                                     \
        WRITE,                                                                  \
        READ,                                                                   \
    } tExchangeFSM;                                                             \
    uint32_t wTimer;                                                            \
    uint32_t wBitCnt;                                                           \
    uint16_t hwSnd, hwRcv;                                                      \
    int32_t  iLevel;                                                            \
END_DEF_CLASS(sw_spi_t)                                                         \
                                                                                \
static sw_spi_t s_tSPI;                                                         \
static CLASS(sw_spi_t) * const s_ptThis = (CLASS(sw_spi_t)*)&s_tSPI;            \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_m_shift(void)                                   \
{                                                                               \
fsm_run:                                                                        \
    switch (this.tMasterShiftFSM) {                                             \
        case M_SHIFT_START:                                                     \
            this.wBitCnt = this.chBITS;                                         \
            this.hwSnd   = this.wSPDW;                                          \
            this.hwRcv   = 0;                                                   \
            this.wTimer  = 0;                                                   \
            if (!(this.wCTRL & SW_SPI_CPHA)) {                                  \
                this.wTimer  = SW_SPI_P_CLK * this.wSPCCW;                      \
                if (this.wCTRL & SW_SPI_LSBF) {                                 \
                    SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);                 \
                } else {                                                        \
                    SW_SPI_OUT_BIT(this.wBitCnt - 1);                           \
                }                                                               \
                SW_SPI_M_SHIFT_TRANS_STATE_FSM(TIME_PRE_EDGE);                  \
                if (this.wTimer) {                                              \
                    this.wTimer--;                                              \
                    break;                                                      \
                } else {                                                        \
                    goto fsm_run;                                               \
                }                                                               \
            }                                                                   \
            SW_SPI_M_SHIFT_TRANS_STATE_FSM(TIME_PRE_EDGE);                      \
        case TIME_PRE_EDGE:                                                     \
            if (0 == this.wTimer) {                                             \
                if (0 == this.wBitCnt) {                                        \
                    this.wSPDW = this.hwRcv;                                    \
                    SW_SPI_M_SHIFT_RESET_FSM();                                 \
                    return fsm_rt_cpl;                                          \
                }                                                               \
                                                                                \
                if (this.wCTRL & SW_SPI_CPOL) {                                 \
                    SW_SPI_CLK_L();                                             \
                } else {                                                        \
                    SW_SPI_CLK_H();                                             \
                }                                                               \
                                                                                \
                if (this.wCTRL & SW_SPI_CPHA) {                                 \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);             \
                    } else {                                                    \
                        SW_SPI_OUT_BIT(this.wBitCnt - 1);                       \
                    }                                                           \
                } else {                                                        \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_IN_BIT(this.chBITS - this.wBitCnt);              \
                    } else {                                                    \
                        SW_SPI_IN_BIT(this.wBitCnt - 1);                        \
                    }                                                           \
                    this.wBitCnt--;                                             \
                }                                                               \
                                                                                \
                this.wTimer = SW_SPI_P_CLK * this.wSPCCW;                       \
                SW_SPI_M_SHIFT_TRANS_STATE_FSM(TIME_LAT_EDGE);                  \
                if (this.wTimer) {                                              \
                    this.wTimer--;                                              \
                } else {                                                        \
                    goto fsm_run;                                               \
                }                                                               \
            } else {                                                            \
                this.wTimer--;                                                  \
            }                                                                   \
            break;                                                              \
        case TIME_LAT_EDGE:                                                     \
            if (0 == this.wTimer) {                                             \
                                                                                \
                if (this.wCTRL & SW_SPI_CPOL) {                                 \
                    SW_SPI_CLK_H();                                             \
                } else {                                                        \
                    SW_SPI_CLK_L();                                             \
                }                                                               \
                                                                                \
                if (this.wCTRL & SW_SPI_CPHA) {                                 \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_IN_BIT(this.chBITS - this.wBitCnt);              \
                    } else {                                                    \
                        SW_SPI_IN_BIT(this.wBitCnt - 1);                        \
                    }                                                           \
                    this.wBitCnt--;                                             \
                } else {                                                        \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);             \
                    } else {                                                    \
                        SW_SPI_OUT_BIT(this.wBitCnt - 1);                       \
                    }                                                           \
                }                                                               \
                                                                                \
                this.wTimer = SW_SPI_P_CLK * this.wSPCCW;                       \
                SW_SPI_M_SHIFT_TRANS_STATE_FSM(TIME_PRE_EDGE);                  \
                if (this.wTimer) {                                              \
                    this.wTimer--;                                              \
                } else {                                                        \
                    goto fsm_run;                                               \
                }                                                               \
            } else {                                                            \
                this.wTimer--;                                                  \
            }                                                                   \
            break;                                                              \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_m_transceiver(void)                             \
{                                                                               \
    switch (this.tMasterTransFSM) {                                             \
        case M_TRANS_START:                                                     \
                                                                                \
            SW_SPI_SS_L();                                                      \
                                                                                \
            this.wTimer  = SW_SPI_PRE_FRAM_DELAY * SW_SPI_P_CLK * this.wSPCCW;  \
            SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(DELAY_PRE_FRAME);              \
        case DELAY_PRE_FRAME:                                                   \
            if (this.wTimer) {                                                  \
                this.wTimer--;                                                  \
                break;                                                          \
            }                                                                   \
            SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(M_SHIFT);                      \
        case M_SHIFT:                                                           \
            if (fsm_rt_on_going == __NAME##_sw_spi_m_shift()) {                 \
                break;                                                          \
            }                                                                   \
                                                                                \
            this.wTimer = SW_SPI_POST_FRAM_DELAY * SW_SPI_P_CLK * this.wSPCCW;  \
            SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(DELAY_POST_FRAME);             \
        case DELAY_POST_FRAME:                                                  \
            if (this.wTimer) {                                                  \
                this.wTimer--;                                                  \
                break;                                                          \
            }                                                                   \
                                                                                \
            SW_SPI_SS_H();                                                      \
                                                                                \
            this.wTimer = SW_SPI_INTER_FRAM_DELAY * SW_SPI_P_CLK * this.wSPCCW; \
            SW_SPI_M_TRANSCEIVER_TRANS_STATE_FSM(DELAY_SS_HIGH);                \
        case DELAY_SS_HIGH:                                                     \
            if (this.wTimer) {                                                  \
                this.wTimer--;                                                  \
                break;                                                          \
            }                                                                   \
            this.wSPSW |= SW_SPI_SPIF;                                          \
            this.wSPSW &= ~SW_SPI_BUSY;                                         \
            SW_SPI_M_TRANSCEIVER_RESET_FSM();                                   \
            return fsm_rt_cpl;                                                  \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_m_task(void)                                    \
{                                                                               \
    switch (this.tMasterTaskFSM) {                                              \
        case M_TASK_START:                                                      \
            SW_SPI_M_TASK_TRANS_STATE_FSM(CHECK_BOX);                           \
        case CHECK_BOX:                                                         \
            if (!(this.wSPSW & SW_SPI_NEW)) {                                   \
                break;                                                          \
            }                                                                   \
            this.wSPSW &= ~SW_SPI_NEW;                                          \
            this.wSPSW |= SW_SPI_BUSY;                                          \
            SW_SPI_M_TASK_TRANS_STATE_FSM(M_TRANSCEIVER);                       \
        case M_TRANSCEIVER:                                                     \
            if (fsm_rt_on_going == __NAME##_sw_spi_m_transceiver()) {           \
                break;                                                          \
            }                                                                   \
            SW_SPI_M_TASK_RESET_FSM();                                          \
            return fsm_rt_cpl;                                                  \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_s_shift(void)                                   \
{                                                                               \
    int32_t iLevel = 0;                                                         \
                                                                                \
    switch (this.tSlaveShiftFSM) {                                              \
        case S_SHIFT_START:                                                     \
            this.wBitCnt = this.chBITS;                                         \
            this.hwSnd   = this.wSPDW;                                          \
            this.hwRcv   = 0;                                                   \
            if (this.wCTRL & SW_SPI_CPOL) {                                     \
                this.iLevel = 1;                                                \
            } else {                                                            \
                this.iLevel = 0;                                                \
            }                                                                   \
            if (!(this.wCTRL & SW_SPI_CPHA)) {                                  \
                if (this.wCTRL & SW_SPI_LSBF) {                                 \
                    SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);                 \
                } else {                                                        \
                    SW_SPI_OUT_BIT(this.wBitCnt - 1);                           \
                }                                                               \
            }                                                                   \
            SW_SPI_S_SHIFT_TRANS_STATE_FSM(SHIFT_CHECK_PRE_EDGE);               \
        case SHIFT_CHECK_PRE_EDGE:                                              \
            iLevel = this.iLevel;                                               \
            GET_CLK_LEVEL(&iLevel);                                             \
            if (iLevel != this.iLevel) {                                        \
                this.iLevel = iLevel;                                           \
                                                                                \
                if (this.wCTRL & SW_SPI_CPHA) {                                 \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);             \
                    } else {                                                    \
                        SW_SPI_OUT_BIT(this.wBitCnt - 1);                       \
                    }                                                           \
                } else {                                                        \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_IN_BIT(this.chBITS - this.wBitCnt);              \
                    } else {                                                    \
                        SW_SPI_IN_BIT(this.wBitCnt - 1);                        \
                    }                                                           \
                    this.wBitCnt--;                                             \
                    if (0 == this.wBitCnt) {                                    \
                        if (!(this.wSPSW & SW_SPI_SPIF)) {                      \
                            this.wSPDW = this.hwRcv;                            \
                            this.wSPSW |= SW_SPI_SPIF;                          \
                        }                                                       \
                        SW_SPI_S_SHIFT_RESET_FSM();                             \
                        return fsm_rt_cpl;                                      \
                    }                                                           \
                }                                                               \
                                                                                \
                SW_SPI_S_SHIFT_TRANS_STATE_FSM(SHIFT_CHECK_LAT_EDGE);           \
            }                                                                   \
            break;                                                              \
        case SHIFT_CHECK_LAT_EDGE:                                              \
            iLevel = this.iLevel;                                               \
            GET_CLK_LEVEL(&iLevel);                                             \
            if (iLevel != this.iLevel) {                                        \
                this.iLevel = iLevel;                                           \
                                                                                \
                if (this.wCTRL & SW_SPI_CPHA) {                                 \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_IN_BIT(this.chBITS - this.wBitCnt);              \
                    } else {                                                    \
                        SW_SPI_IN_BIT(this.wBitCnt - 1);                        \
                    }                                                           \
                    this.wBitCnt--;                                             \
                    if (0 == this.wBitCnt) {                                    \
                        if (!(this.wSPSW & SW_SPI_SPIF)) {                      \
                            this.wSPDW = this.hwRcv;                            \
                            this.wSPSW |= SW_SPI_SPIF;                          \
                        }                                                       \
                        SW_SPI_S_SHIFT_RESET_FSM();                             \
                        return fsm_rt_cpl;                                      \
                    }                                                           \
                } else {                                                        \
                    if (this.wCTRL & SW_SPI_LSBF) {                             \
                        SW_SPI_OUT_BIT(this.chBITS - this.wBitCnt);             \
                    } else {                                                    \
                        SW_SPI_OUT_BIT(this.wBitCnt - 1);                       \
                    }                                                           \
                }                                                               \
                                                                                \
                SW_SPI_S_SHIFT_TRANS_STATE_FSM(SHIFT_CHECK_PRE_EDGE);           \
            }                                                                   \
            break;                                                              \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_s_transceiver(void)                             \
{                                                                               \
    int32_t iLevel;                                                             \
                                                                                \
    switch (this.tSlaveTransFSM) {                                              \
        case S_TRANS_START:                                                     \
            this.iLevel = 1;                                                    \
            SW_SPI_S_TRANSCEIVER_TRANS_STATE_FSM(WAIT_SS_LOW);                  \
        case WAIT_SS_LOW:                                                       \
            GET_SS_LEVEL(&this.iLevel);                                         \
            if (this.iLevel) {                                                  \
                break;                                                          \
            } else {                                                            \
                this.wSPSW |= SW_SPI_BUSY;                                      \
                SW_SPI_S_TRANSCEIVER_TRANS_STATE_FSM(S_SHIFT);                  \
            }                                                                   \
        case S_SHIFT:                                                           \
            if (fsm_rt_on_going == __NAME##_sw_spi_s_shift()) {                 \
                GET_SS_LEVEL(&iLevel);                                          \
                if (iLevel) {                                                   \
                    this.wSPSW &= ~SW_SPI_BUSY;                                 \
                    SW_SPI_S_TRANSCEIVER_RESET_FSM();                           \
                    return fsm_rt_cpl;                                          \
                }                                                               \
                break;                                                          \
            }                                                                   \
            this.wSPSW |= SW_SPI_SPIF;                                          \
            this.wSPSW &= ~SW_SPI_BUSY;                                         \
            SW_SPI_S_TRANSCEIVER_RESET_FSM();                                   \
            return fsm_rt_cpl;                                                  \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
static fsm_rt_t __NAME##_sw_spi_s_task(void)                                    \
{                                                                               \
    switch (this.tSlaveTaskFSM) {                                               \
        case S_TASK_START:                                                      \
            SW_SPI_S_TASK_TRANS_STATE_FSM(S_TRANSCEIVER);                       \
        case S_TRANSCEIVER:                                                     \
            if (fsm_rt_cpl == __NAME##_sw_spi_s_transceiver()) {                \
                SW_SPI_S_TASK_RESET_FSM();                                      \
                return fsm_rt_cpl;                                              \
            }                                                                   \
            break;                                                              \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \
                                                                                \
void __NAME##_sw_spi_init(sw_spi_cfg_t *ptCFG)                                  \
{                                                                               \
    if (!(ptCFG->hwMode & SW_SPI_MODE_SLAVE)) {                                 \
        this.wCTRL |= SW_SPI_MSTR;                                              \
    }                                                                           \
                                                                                \
    if (ptCFG->hwMode & SW_SPI_MODE_CLK_IDLE_HIGH) {                            \
        this.wCTRL |= SW_SPI_CPOL;                                              \
    }                                                                           \
                                                                                \
    if (ptCFG->hwMode & SW_SPI_MODE_SAMP_SECOND_EDGE) {                         \
        this.wCTRL |= SW_SPI_CPHA;                                              \
    }                                                                           \
                                                                                \
    this.wCTRL |= SW_SPI_LSBF;                                                  \
    this.chBITS = ptCFG->chDataSize + 1;                                        \
    this.wSPDW = 0;                                                             \
    this.wSPSW = 0;                                                             \
    this.wSPCCW = ptCFG->chClockDiv;                                            \
    this.tSlaveShiftFSM = S_SHIFT_START;                                        \
                                                                                \
    if (this.wCTRL & SW_SPI_MSTR) {                                             \
        SW_SPI_CLK_MASTER_INIT();                                               \
        SW_SPI_MOSI_MASTER_INIT();                                              \
        SW_SPI_MISO_MASTER_INIT();                                              \
        SW_SPI_SS_MASTER_INIT();                                                \
                                                                                \
        SW_SPI_SS_H();                                                          \
        SW_SPI_MOSI_H();                                                        \
        if (this.wCTRL & SW_SPI_CPOL) {                                         \
            SW_SPI_CLK_H();                                                     \
        } else {                                                                \
            SW_SPI_CLK_L();                                                     \
        }                                                                       \
    } else {                                                                    \
        SW_SPI_CLK_SLAVE_INIT();                                                \
        SW_SPI_MOSI_SLAVE_INIT();                                               \
        SW_SPI_MISO_SLAVE_INIT();                                               \
        SW_SPI_SS_SLAVE_INIT();                                                 \
                                                                                \
        SW_SPI_MISO_H();                                                        \
    }                                                                           \
}                                                                               \
                                                                                \
void __NAME##_sw_spi_task(void)                                                 \
{                                                                               \
    if (this.wCTRL & SW_SPI_MSTR) {                                             \
        __NAME##_sw_spi_m_task();                                               \
    } else {                                                                    \
        __NAME##_sw_spi_s_task();                                               \
    }                                                                           \
}                                                                               \
                                                                                \
bool __NAME##_sw_spi_write(uint16_t hwData)                                     \
{                                                                               \
    if (this.wSPSW & SW_SPI_BUSY) {                                             \
        return false;                                                           \
    }                                                                           \
    this.wSPSW |= SW_SPI_NEW;                                                   \
    this.wSPSW &= ~SW_SPI_SPIF;                                                 \
    this.wSPDW = hwData;                                                        \
                                                                                \
    return true;                                                                \
}                                                                               \
                                                                                \
bool __NAME##_sw_spi_read(uint16_t *phwData)                                    \
{                                                                               \
    if (!(this.wSPSW & SW_SPI_SPIF)) {                                          \
        return false;                                                           \
    }                                                                           \
    this.wSPSW &= ~SW_SPI_SPIF;                                                 \
    *phwData = this.wSPDW;                                                      \
                                                                                \
    return true;                                                                \
}                                                                               \
                                                                                \
fsm_rt_t __NAME##_sw_spi_exchange(uint16_t hwWrite, uint16_t *phwRead)          \
{                                                                               \
    uint16_t hwTmp = 0;                                                         \
                                                                                \
    if (NULL == phwRead) {                                                      \
        return fsm_rt_err;                                                      \
    }                                                                           \
                                                                                \
    switch (this.tExchangeFSM) {                                                \
        case EXCHANGE_START:                                                    \
            SW_SPI_EXCHANGE_TRANS_STATE_FSM(WRITE);                             \
        case WRITE:                                                             \
            if (false == __NAME##_sw_spi_write(hwWrite)) {                      \
                break;                                                          \
            }                                                                   \
            SW_SPI_EXCHANGE_TRANS_STATE_FSM(READ);                              \
        case READ:                                                              \
            if (false != __NAME##_sw_spi_read(&hwTmp)) {                        \
                if (NULL != phwRead) {                                          \
                    *phwRead = hwTmp;                                           \
                }                                                               \
                SW_SPI_EXCHANGE_RESET_FSM();                                    \
                return fsm_rt_cpl;                                              \
            }                                                                   \
            break;                                                              \
    }                                                                           \
                                                                                \
    return fsm_rt_on_going;                                                     \
}                                                                               \

#endif
