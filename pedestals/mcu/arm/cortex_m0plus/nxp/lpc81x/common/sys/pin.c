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

#define __DRIVER_ARM_M0PLUS_NXP_LPC81X_PIN_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\pin_public.h"
#include ".\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define SAFE_CLK_CODE(...)  \
    {\
        uint32_t __clockStatusIOCON = clock_get_status(CLOCK_IOCON);    \
        uint32_t __clockStatusSWM   = clock_get_status(CLOCK_SWM);      \
        clock_enable(CLOCK_IOCON);                                      \
        clock_enable(CLOCK_SWM);                                        \
        __VA_ARGS__                                                     \
        clock_resume_status(CLOCK_IOCON, __clockStatusIOCON);           \
        clock_resume_status(CLOCK_SWM,   __clockStatusSWM);             \
    }

/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
static bool io_configurate(io_cfg_t const *pConfig, uint32_t size);

/*============================ PRIVATE VARIABLES =============================*/
static const uint8_t port0PinRegIndexTable[18] = {
    17, 11,  6,  5,
     4,  3, 16, 15,
    14, 13,  8,  7,
     2,  1, 18, 10,
     9,  0,
};

/*============================ PUBLIC VARIABLES ==============================*/
//! \brief io interface
const io_t IO = {
    &io_configurate,                  //!< general io configuration
};

/*============================ IMPLEMENTATION ================================*/

/*! \param  io configuration structure array
 *! \param  chSize array size
 *! \retval true configuration succeed
 *! \retval false configuration fail
 */
static bool io_configurate(io_cfg_t const *pConfig, uint32_t size)
{
    if (NULL == pConfig) {
        return false;
    }

    //SAFE_CLK_CODE(
    clock_enable(CLOCK_IOCON);                                      \
    clock_enable(CLOCK_SWM); 
        for (; size; --size) {
            switch (pConfig->Port) {
                case 0:
                    if (pConfig->PIN >= UBOUND(port0PinRegIndexTable)) {
                        break;
                    } else {
                        uint8_t index;
                        uint32_t fun = pConfig->Function;
                        
                        index = port0PinRegIndexTable[pConfig->PIN];
                        IOCON_REG.PIO0[index] = pConfig->Mode;
                        if (fun >= PIN_FN_ACMP_I1) {
                            fun -= PIN_FN_ACMP_I1;
                            if (fun > 8u) {
                                //! invalid input, just ignore it.
                            } else {
                                SWM_REG.PINENABLE0 &= ~BIT(fun);
                            }
                        } else {
                            uint8_t index = pConfig->Function >> 2;
                            uint32_t shift = 8 * (pConfig->Function & 0x03u);
                            SWM_REG.PINASSIGN[index] &= ~(0xFFu << shift);
                            SWM_REG.PINASSIGN[index] |=  (0xFFu & pConfig->PIN) << shift;
                        }
                    }
                    break;
                    
                default:
                    break;
            }
            
            pConfig++;
        }
    //)

    return true;
}


  
/* EOF */