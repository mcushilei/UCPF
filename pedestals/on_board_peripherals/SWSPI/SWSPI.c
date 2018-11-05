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

#include ".\app_cfg.h"

#define SW_SPI_SS_MASTER_INIT()     do{GSP_PORTA.DIR |= PA28_MSK;}while(0)
#define SW_SPI_SS_L()               do{GSP_PORTA.OUTCLR = PA28_MSK;}while(0)
#define SW_SPI_SS_H()               do{GSP_PORTA.OUTSET = PA28_MSK;}while(0)
#define SW_SPI_SS_SLAVE_INIT()      do{GSP_PORTA.DIR &= ~PA28_MSK;}while(0)
#define SW_SPI_SS()                 (GSP_PORTA.IN & PA28_MSK)

#define SW_SPI_CLK_MASTER_INIT()    do{GSP_PORTA.DIR |= PA18_MSK;}while(0)
#define SW_SPI_CLK_L()              do{GSP_PORTA.OUTCLR = PA18_MSK;}while(0)
#define SW_SPI_CLK_H()              do{GSP_PORTA.OUTSET = PA18_MSK;}while(0)
#define SW_SPI_CLK_SLAVE_INIT()     do{GSP_PORTA.DIR &= ~PA18_MSK;}while(0)
#define SW_SPI_CLK()                (GSP_PORTA.IN & PA18_MSK)

#define SW_SPI_MOSI_MASTER_INIT()   do{GSP_PORTA.DIR |= PA19_MSK;}while(0)
#define SW_SPI_MOSI_L()             do{GSP_PORTA.OUTCLR = PA19_MSK;}while(0)
#define SW_SPI_MOSI_H()             do{GSP_PORTA.OUTSET = PA19_MSK;}while(0)
#define SW_SPI_MOSI_SLAVE_INIT()    do{GSP_PORTA.DIR &= ~PA19_MSK;}while(0)
#define SW_SPI_MOSI()               (GSP_PORTA.IN & PA19_MSK)

#define SW_SPI_MISO_MASTER_INIT()   do{GSP_PORTA.DIR &= ~PA20_MSK;}while(0)
#define SW_SPI_MISO()               (GSP_PORTA.IN & PA20_MSK)
#define SW_SPI_MISO_SLAVE_INIT()    do{GSP_PORTA.DIR |= PA20_MSK;}while(0)
#define SW_SPI_MISO_L()             do{GSP_PORTA.OUTCLR = PA20_MSK;}while(0)
#define SW_SPI_MISO_H()             do{GSP_PORTA.OUTSET = PA20_MSK;}while(0)

#define SW_SPI_P_CLK                (1ul)
#define SW_SPI_PRE_FRAM_DELAY       (1ul)
#define SW_SPI_POST_FRAM_DELAY      (1ul)
#define SW_SPI_INTER_FRAM_DELAY     (1ul)


#if ((!defined(SW_SPI_P_CLK)) || (SW_SPI_P_CLK == 0))
#define SW_SPI_P_CLK                (1ul)
#warning "SW_SPI_P_CLK is unvalid, default 1 will be used."
#endif
#if ((!defined(SW_SPI_PRE_FRAM_DELAY)) || (SW_SPI_PRE_FRAM_DELAY == 0))
#define SW_SPI_PRE_FRAM_DELAY       (1ul)
#warning "SW_SPI_PRE_FRAM_DELAY is unvalid, default 1 will be used."
#endif
#if ((!defined(SW_SPI_POST_FRAM_DELAY)) || (SW_SPI_POST_FRAM_DELAY == 0))
#define SW_SPI_POST_FRAM_DELAY      (1ul)
#warning "SW_SPI_POST_FRAM_DELAY is unvalid, default 1 will be used."
#endif
#if ((!defined(SW_SPI_INTER_FRAM_DELAY)) || (SW_SPI_INTER_FRAM_DELAY == 0))
#define SW_SPI_INTER_FRAM_DELAY     (1ul)
#warning "SW_SPI_INTER_FRAM_DELAY is unvalid, default 1 will be used."
#endif

#include ".\SWSPI_TEMPLATE.h"


DEFINE_SWSPI(SWSPI0)
END_DEFINE_SWSPI

