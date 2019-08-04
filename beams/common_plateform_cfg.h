/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
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

#ifndef __COMMON_PLATEFORM_CFG_H__
#define __COMMON_PLATEFORM_CFG_H__

/*============================ INCLUDES ======================================*/
#if     defined(__PRJ_TEMPLATE__)
#   include ".\frameworks\template\config\plateform_cfg.h"
#elif   defined(__PRJ_WINDOWS__)
#   include ".\frameworks\windows\config\plateform_cfg.h"
#elif   defined(__PRJ_SENSOR_VIBRATION__)
#   include ".\frameworks\sensor_vibration\config\plateform_cfg.h"
#elif   defined(__PRJ_BATTERY_MONITOR__)
#   include ".\frameworks\battery_monitor\config\plateform_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL__)
#   include ".\frameworks\beidou_ship_terminal\config\plateform_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL_TEST__)
#   include ".\frameworks\beidou_ship_terminal_test\config\plateform_cfg.h"
#elif   defined(__PRJ_LINE_VIBRATION__)
#   include ".\frameworks\line_vibration\config\plateform_cfg.h"
#elif   defined(__PRJ_BEIDOU_SHIP_TERMINAL_USER_SIMULATOR__)
#   include ".\frameworks\beidou_ship_terminal_user_simulator\config\plateform_cfg.h"
#elif   defined(__PRJ_SHIP_GPS__)
#   include ".\frameworks\ship_gps\config\plateform_cfg.h"
#elif   defined(__PRJ_BEIDOU_HARBOUR__)
#   include ".\frameworks\ship_gps_g2\config\plateform_cfg.h"
#elif   defined(__PRJ_LED_PANNEL__)
#   include ".\frameworks\led_panel\config\plateform_cfg.h"
#elif   defined(__PRJ_BEIJING_NENGYUAN__)
#   include ".\frameworks\beijing_alternative_energy\config\plateform_cfg.h"
#elif   defined(__PRJ_MSP432_TEMPLATE__)
#   include ".\frameworks\ti_msp432_template\config\plateform_cfg.h"
#else
#   error "Unknow Project"
#endif

/*============================ MACROS ========================================*/
#if     defined(__CPU_X86__)                //!< x86 series
#elif   defined(__NXP_K60__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4__                    //!< cortex-m4 core
#   define __NXP__                          //!< NXP
#   define __KINETIS_K__                    //!< Kinetis K series
#   define __K60__                          //!< K60
#elif   defined(__ST_STM32__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M3__                    //!< cortex-m3 core
#   define __ST__                           //!< ST
#   define __STM32F2__                      //!< STM32F2xx series
#   define __STM32F205__                    //!< MCU Part Number 
#elif   defined(__TI_TM4C129__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4F__                   //!< cortex-m4 core with FPU
#   define __TI__                           //!< TI
#   define __TM4C__                         //!< TM4C series
#   define __TM4C129__                      //!< TM4C129
#elif   defined(__TI_MSP432P401R__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4F__                   //!< cortex-m4 core with FPU
#   define __TI__                           //!< TI
#   define __MSP432__                       //!< TM4C series
#   define __MSP432P401R__                  //!< TM4C129
#elif   defined(__TI_TMS320X2812__)
#	define __CPU_TMS320__                 	//!< TMS320 series
#	define __C28X__                         //!< C28x core
#	define __TI__                          	//!< TI
#	define __TMS320X281X__                	//!< TMS320X281X series
#	define __TMS320X2812__                	//!< Part Number
#elif   defined(__NXP_LPC1227__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M0__                    //!< cortex-m0 core
#   define __NXP__                          //!< NXP
#   define __LPC12XX__                      //!< LPC12XX series
#   define __LPC1227__                      //!< MCU Part Number 
#elif   defined(__NXP_LPC1754__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M3__                    //!< cortex-m3 core
#   define __NXP__                          //!< NXP
#   define __LPC17XX__                      //!< LPC17XX series
#   define __LPC1754__                      //!< MCU Part Number 
#elif   defined(__NXP_LPC1768__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M3__                    //!< cortex-m3 core
#   define __NXP__                          //!< NXP
#   define __LPC17XX__                      //!< LPC17XX series
#   define __LPC1768__                      //!< MCU Part Number 
#elif   defined(__ADI_ADUC7061__)
#   define __CPU_ARM__                      //!< arm series
#   define __ARM7__                         //!< ARM7 core
#   define __ADI__                          //!< NXP
#   define __ADUC706X__                     //!< ADUC706X series
#   define __ADUC7061__                     //!< ADUC7061
#elif   defined(__NXP_LPC11E68__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M0P__                   //!< cortex-m0+ core
#   define __NXP__                          //!< NXP
#   define __LPC11XXX__                     //!< LPC11xxx series
#   define __LPC11E68__                     //!< LPC11E68
#elif   defined(__NXP_LPC812__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M0P__                   //!< cortex-m0+ core
#   define __NXP__                          //!< NXP
#   define __LPC81X__                       //!< LPC81x series
#   define __LPC812__                       //!< LPC812
#elif   defined(__AMBIQ_APOLLO2__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4__                    //!< cortex-m4 core
#   define __AMBIQ__                        //!< AMBIQ
#   define __APOLLO2__                      //!< APOLLO2 series
#else
#   error "CPU must be specified!"
#endif


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

#endif
/* EOF */
