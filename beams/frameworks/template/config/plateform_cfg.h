/***************************************************************************
 *   Copyright(C)2009-2013 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __TEMPLATE_PLATEFORM_CFG_H__
#define __TEMPLATE_PLATEFORM_CFG_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/

#define TASK_SCHEDULER              ENABLED

#if     defined(__LPC17XX__)
#define SYS_OSC_FREQ                (12000000ul)            //!< 
#elif   defined(__LPC12XX__)
#define SYS_OSC_FREQ                (12000000ul)            //!< 
#elif   defined(__LPC11XXX__)
#define SYSOSC_BYPASS_EN            DISABLED
#define SYS_OSC_FREQ                (12000000ul)            //!< 
#elif   defined(__TMS320X281X__)
#define SYS_OSC_FREQ                (30000000ul)            //!<
#endif

#define __USER_FSM_CFG__
#define SAFE_TASK_THREAD_SYNC   ENABLED
#define FSM_MAX_TASKS           (10u)
#define FSM_MAX_FLAGS           (4u)
#define FSM_MAX_MUTEXES         (4u)
#define FSM_MAX_SEMS            (4u)
#define FSM_SAFE_ATOM_CODE(...) SAFE_ATOM_CODE(__VA_ARGS__)




#define IS_PLL_ENABLED()            ENABLED
#define EXT_CLOCK_FREQ              (0u)
#define CRYSTAL_FREQ                (32768u)

#ifdef __DEBUG__
#   define DEBUG_MSG_ENABLE             ENABLED
#   define DEBUG_ASSERT_ENABLE          ENABLED
#   define DEBUG_DISALLOW_FILE_INFO     DISABLED

#   define DEBUG_LED_FLASH(__N)    do {\
        volatile uint32_t j = __N;\
        for (; j; j--) {\
            GPIO0_REG.OUTTOG = PIN12_MSK;\
            for (volatile uint32_t i = 0x00040000; i; i--);\
            GPIO0_REG.OUTTOG = PIN12_MSK;\
            for (volatile uint32_t i = 0x00040000; i; i--);\
        }\
    } while (0)
#else
#   define DEBUG_MSG_ENABLE             ENABLED
#   define DEBUG_ASSERT_ENABLE          ENABLED

#   define DEBUG_LED_FLASH(__N)
#endif
#define DEBUG_FOMART_STRING         DISABLED
#define DEBUG_MIN_LEVEL             DEBUG_LEVEL_ALL // DEBUG_LEVEL_WARNING, DEBUG_LEVEL_SERIOUS, 
#define DEBUG_TYPES_ON              DEBUG_ANY       // DEBUG_STATE | DEBUG_TRACE | DEBUG_HALT |  | DEBUG_FRESH

#define TEMPLATE_DEBUG              DEBUG_ON
#define FIFO_DEBUG                  DEBUG_OFF
#define QUEUE_DEBUG                 DEBUG_OFF
#define POOL_DEBUG                  DEBUG_OFF
#define FRAME_DEBUG                 DEBUG_OFF

#define USE_MINI_SPI                        ENABLED
#define USE_SPI_FLASH_MINI                  ENABLED
#define MINI_SPI_USE_SPI                    0

#define DEVICE_WDT_TIME             (80u)   //!< in second, max 512sec.

#define FIFO_UINT_TYPE              uint16_t

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
/* EOF */
