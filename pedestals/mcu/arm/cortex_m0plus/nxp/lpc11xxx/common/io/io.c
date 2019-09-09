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

#define __IO_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_iocon.h"
#include ".\reg_gpio.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      gpio_reg_page_t GPIO##__N;

#define SAFE_CLK_CODE(...)  \
    {\
        uint32_t Status   = ahb_clock_get_status(AHBCLK_IOCON);       \
        ahb_clock_enable(AHBCLK_IOCON);                                \
        __VA_ARGS__;                                                    \
        ahb_clock_resume_status(AHBCLK_IOCON, Status);                \
    }

#define __GPIO_FUNC_PROTOTYPE(__N,__VALUE)                                            \
static void     gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask);  \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask);                \
static void     gpio##__N##_set_input(uint32_t wPinMask);                    \
static void     gpio##__N##_set_output(uint32_t wPinMask);                   \
static uint32_t gpio##__N##_read(void);                                      \
static void     gpio##__N##_write(uint32_t wValue, uint32_t wPinMask);       \
static void     gpio##__N##_set(uint32_t wPinMask);                          \
static void     gpio##__N##_clear(uint32_t wPinMask);                        \
static void     gpio##__N##_toggle(uint32_t wPinMask);                       

#define __GPIO_INTERFACE_INIT(__N, __VALUE)                                     \
            {                                                                   \
                &gpio##__N##_set_direction,                                     \
                &gpio##__N##_get_direction,                                     \
                &gpio##__N##_set_input,                                         \
                &gpio##__N##_set_output,                                        \
                &gpio##__N##_read,                                              \
                &gpio##__N##_write,                                             \
                &gpio##__N##_set,                                               \
                &gpio##__N##_clear,                                             \
                &gpio##__N##_toggle,                                            \
            },

#define __GPIO_FUNC_DEF(__N, __VALUE)                                          \
static void gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask)   \
{                                                                               \
    SAFE_ATOM_CODE(                                                            \
        uint32_t wTemp = GPIO##__N##_REG.DIR & ~wPinMask;                         \
        wTemp |= (wDirection & wPinMask);                                       \
        GPIO##__N##_REG.DIR = wTemp;                                              \
    )                                                                           \
}                                                                               \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask)                    \
{                                                                               \
    return GPIO##__N##_REG.DIR;                                                   \
}                                                                               \
static void gpio##__N##_set_input(uint32_t wPinMask)                            \
{                                                                               \
    SAFE_ATOM_CODE(                                                            \
        GPIO##__N##_REG.DIR &= ~wPinMask;                                             \
    )                                                                           \
}                                                                               \
static void gpio##__N##_set_output(uint32_t wPinMask)                           \
{                                                                               \
    SAFE_ATOM_CODE(                                                            \
        GPIO##__N##_REG.DIR |= wPinMask;                                              \
    )                                                                           \
}                                                                               \
static uint32_t gpio##__N##_read(void)                                          \
{                                                                               \
    return GPIO##__N##_REG.PIN;                                                    \
}                                                                               \
static void gpio##__N##_write(uint32_t wValue, uint32_t wPinMask)               \
{                                                                               \
    SAFE_ATOM_CODE(                                                            \
        uint32_t wTemp = GPIO##__N##_REG.PIN & (~wPinMask);                         \
        wTemp |= wValue & wPinMask;                                           \
        GPIO##__N##_REG.PIN = wTemp;                                              \
    )                                                                           \
}                                                                               \
static void gpio##__N##_set(uint32_t wPinMask)                                  \
{                                                                               \
    GPIO##__N##_REG.SET = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_clear(uint32_t wPinMask)                                \
{                                                                               \
    GPIO##__N##_REG.CLR = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_toggle(uint32_t wPinMask)                               \
{                                                                               \
    GPIO##__N##_REG.NOT = wPinMask;                                            \
}                                                                                                                                             
                                                

/*============================ TYPES =========================================*/
//! \name io configuration structure
//! @{
typedef struct {
    uint32_t        Port;
    uint32_t        PIN;           //!< pin number
    uint32_t        Function;      //!< io Funcitons
    uint32_t        Mode;          //!< io mode
} io_cfg_t;
//! @}

//! \name gpio control interface
//! @{
DEF_INTERFACE(i_gpio_t)
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
    uint32_t    (*Read)(void);
    void        (*Write)(uint32_t wValue, uint32_t wPinMask);
    void        (*Set)(uint32_t wPinMask);
    void        (*Clear)(uint32_t wPinMask);
    void        (*Toggle)(uint32_t wPinMask);
END_DEF_INTERFACE(i_gpio_t)
//! @}


//! \name csc user interface
//! @{
DEF_INTERFACE(io_t)
    bool            (*Config)(io_cfg_t const *pConfig, uint32_t Size);//!< io configuration
    union {
        i_gpio_t  GPIO[IO_PORT_COUNT];               //!< dedicated gpio control interface
        struct {
            MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE, NULL)
        };
    };
END_DEF_INTERFACE(io_t)
//! @}


/*============================ PROTOTYPES ====================================*/
MREPEAT(IO_PORT_COUNT, __GPIO_FUNC_PROTOTYPE, NULL)
static bool io_configurate(io_cfg_t const *pConfig, uint32_t Size);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
const io_t IO = {
        &io_configurate,                  //!< general io configuration
        {
            {MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE_INIT, NULL)}
        }
    };

/*============================ IMPLEMENTATION ================================*/
MREPEAT(IO_PORT_COUNT, __GPIO_FUNC_DEF, NULL)

/*! \name io general configuration
 *! \param io configuration structure array
 *! \param chSize array size
 *! \retval true configuration succeed
 *! \retval false configuration fail
 */
static bool io_configurate(io_cfg_t const *pConfig, uint32_t Size)
{
    if (NULL == pConfig) {
        return false;
    }

    SAFE_CLK_CODE(
        //! io configure
        for (; Size; --Size) {
            uint32_t cfg = pConfig->Mode | pConfig->Function;
            switch (pConfig->Port) {
                case 0:
                    if (pConfig->PIN >= 24) {
                        break;
                    }
                    IOCON_REG.PIO0[pConfig->PIN] = cfg;
                    break;
                    
                case 1:
                    if (pConfig->PIN >= 32) {
                        break;
                    }
                    IOCON_REG.PIO1[pConfig->PIN] = cfg;
                    break;
                    
                case 2:
                    if (pConfig->PIN < 2) {
                        IOCON_REG.PIO2A[pConfig->PIN] = cfg;
                    } else if (pConfig->PIN < 24) {
                        IOCON_REG.PIO2B[pConfig->PIN - 2] = cfg;
                    }
                    break;
                    
                default:
                    break;
            }
            pConfig++;
        }
    )

    return true;
}


  
/* EOF */