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


//! \note do not move this pre-processor statement to other places
#define __DRIVER_LPC17XX_IO_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_ioctrl.h"
#include ".\reg_gpio.h"


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      const i_gpio_t GPIO##__N;

#define __IO_PORT_NAM(__N, __OFFSET)        PORT##__N = (__N),
#define __IO_PIN_NAM(__N, __OFFSET)         PIN##__N = (__N),
#define __IO_PIN_MSK(__N, __OFFSET)         PIN##__N##_MSK = (1ul << (__N)),

#define __GPIO_INTERFACE_DEF(__N, __VALUE)                              \
    {                                                                   \
        &gpio##__N##_set_direction,                                     \
        &gpio##__N##_get_direction,                                     \
        &gpio##__N##_set_input,                                         \
        &gpio##__N##_set_output,                                        \
        &gpio##__N##_reverse_direction,                                 \
        &gpio##__N##_read,                                              \
        &gpio##__N##_write,                                             \
        &gpio##__N##_set,                                               \
        &gpio##__N##_clear,                                             \
        &gpio##__N##_toggle,                                            \
    },

#define __GPIO_FUNC_PROTOTYPES(__N,__VALUE)                                     \
static void     gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask);  \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask);                   \
static void     gpio##__N##_set_input(uint32_t wPinMask);                       \
static void     gpio##__N##_set_output(uint32_t wPinMask);                      \
static void     gpio##__N##_reverse_direction(uint32_t wPinMask);               \
static uint32_t gpio##__N##_read(void);                                         \
static void     gpio##__N##_write(uint32_t wValue, uint32_t wPinMask);          \
static void     gpio##__N##_set(uint32_t wPinMask);                             \
static void     gpio##__N##_clear(uint32_t wPinMask);                           \
static void     gpio##__N##_toggle(uint32_t wPinMask);                       

#define __GPIO_FUNC_BODY(__N, __VALUE)                                          \
static void gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask)   \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        uint32_t wTemp = GPIO##__N##_REG.FIODIR & ~wPinMask;                      \
        wTemp |= (wDirection & wPinMask);                                       \
        GPIO##__N##_REG.FIODIR = wTemp;                                           \
    )                                                                           \
}                                                                               \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask)                    \
{                                                                               \
    return GPIO##__N##_REG.FIODIR;                                                \
}                                                                               \
static void gpio##__N##_set_input(uint32_t wPinMask)                            \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR &= ~wPinMask;                                      \
    )                                                                           \
}                                                                               \
static void gpio##__N##_set_output(uint32_t wPinMask)                           \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR |= wPinMask;                                       \
    )                                                                           \
}                                                                               \
static void gpio##__N##_reverse_direction(uint32_t wPinMask)                    \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIODIR ^= wPinMask;                                       \
    )                                                                           \
}                                                                               \
                                                                                \
static uint32_t gpio##__N##_read(void)                                          \
{                                                                               \
    return GPIO##__N##_REG.FIOPIN;                                                \
}                                                                               \
static void gpio##__N##_write(uint32_t wValue, uint32_t wPinMask)               \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        uint32_t wTemp = GPIO##__N##_REG.FIOPIN & ~wPinMask;                      \
        wTemp |= (wValue & wPinMask);                                           \
        GPIO##__N##_REG.FIOPIN = wTemp;                                           \
    )                                                                           \
}                                                                               \
                                                                                \
static void gpio##__N##_set(uint32_t wPinMask)                                  \
{                                                                               \
    GPIO##__N##_REG.FIOSET = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_clear(uint32_t wPinMask)                                \
{                                                                               \
    GPIO##__N##_REG.FIOCLR = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_toggle(uint32_t wPinMask)                               \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        GPIO##__N##_REG.FIOPIN ^= wPinMask;                                       \
    )                                                                           \
}                                                                                                                                             

/*============================ TYPES =========================================*/
enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_MSK, 0)
};

enum {
    MREPEAT(IO_PORT_COUNT, __IO_PORT_NAM, 0)
};

enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PIN_NAM, 0)
};

//! \name io configuration structure
//! @{
typedef struct {
    uint8_t         Port;                  //!< port number
    uint8_t         Pin;                   //!< pin number
    uint8_t         Function;              //!< io Funcitons
    uint8_t         Mode;                  //!< io mode
} io_cfg_t;
//! @}

//! \name gpio control interface
//! @{
DEF_INTERFACE(i_gpio_t)
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
    void        (*ReverseDirection)(uint32_t wPinMask);
    uint32_t    (*Read)(void);
    void        (*Write)(uint32_t wValue, uint32_t wPinMask);
    void        (*Set)(uint32_t wPinMask);
    void        (*Clear)(uint32_t wPinMask);
    void        (*Toggle)(uint32_t wPinMask);
END_DEF_INTERFACE(i_gpio_t)
//! @}

//! \name io interface
//! @{
DEF_INTERFACE(i_io_t)
    //! general io configuration
    bool            (*Config)(io_cfg_t *pConfig, uint32_t size);
    union {
        const i_gpio_t  GPIO[GPIO_COUNT];               //!< dedicated gpio control interface
        struct {
            MREPEAT(GPIO_COUNT, __GPIO_INTERFACE, NULL)
        };
    };
END_DEF_INTERFACE(i_io_t)
//! @}


/*============================ PROTOTYPES ====================================*/
static bool io_configuration( io_cfg_t *pConfig, uint32_t size );
MREPEAT(GPIO_COUNT, __GPIO_FUNC_PROTOTYPES, NULL)

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
const i_io_t IO = {
        &io_configuration,                  //!< general io configuration
        {
            {MREPEAT(GPIO_COUNT, __GPIO_INTERFACE_DEF, NULL)}
        }
    };

/*============================ IMPLEMENTATION ================================*/
MREPEAT(GPIO_COUNT, __GPIO_FUNC_BODY, NULL)

/*! \name io general configuration
 *! \param io configuration structure array
 *! \param size array size
 *! \retval true configuration succeed
 *! \retval false configuration fail
 */
static bool io_configuration(io_cfg_t *pConfig, uint32_t size)
{
    if (NULL == pConfig || 0u == size) {
        return false;
    }

    //! io configure
    for (; size != 0u; --size){
        uint32_t wFunction  = pConfig->Function;
        uint32_t wIOMODE    = pConfig->Mode;
        uint32_t wRegIndex, wBitIndex, wMask;

        wRegIndex = (pConfig->Port * 2u) + ((pConfig->Pin >> 4) & 0x01u);
        wBitIndex = (pConfig->Pin & 0x0Fu) * 2u;
        wMask = 0x03u << wBitIndex;
        PINCON_REG.PINSEL[wRegIndex]  = PINCON_REG.PINSEL[wRegIndex] & ~wMask | ((wFunction & 0x03u) << wBitIndex);
        PINCON_REG.PINMODE[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & ~wMask | ((wIOMODE & 0x03u) << wBitIndex);

        wRegIndex = pConfig->Port;
        wBitIndex = pConfig->Pin;
        wMask = 0x01u << wBitIndex;
        PINCON_REG.PINMODE_OD[wRegIndex] = PINCON_REG.PINMODE[wRegIndex] & ~wMask | (((wIOMODE >> 2) & 0x01u) << wBitIndex);
                
        pConfig++;
    }

    return true;
}


  
/* EOF */