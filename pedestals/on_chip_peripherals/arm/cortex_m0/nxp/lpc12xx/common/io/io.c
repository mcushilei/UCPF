
#define __LPC_IO_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_ioctrl.h"
#include ".\reg_gpio.h"


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define __GPIO_INTERFACE(__N, __VALUE)      i_gpio_t GPIO##__N;

#define __GPIO_FUNC_PROTOTYPE(__N,__VALUE)                                            \
static void     gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask);  \
static uint32_t gpio##__N##_get_direction(uint32_t wPinMask);                \
static void     gpio##__N##_set_input(uint32_t wPinMask);                    \
static void     gpio##__N##_set_output(uint32_t wPinMask);                   \
static void     gpio##__N##_switch_direction(uint32_t wPinMask);             \
static uint32_t gpio##__N##_read(void);                                      \
static void     gpio##__N##_write(uint32_t wValue, uint32_t wPinMask);       \
static void     gpio##__N##_set(uint32_t wPinMask);                          \
static void     gpio##__N##_clear(uint32_t wPinMask);                        \
static void     gpio##__N##_toggle(uint32_t wPinMask);                       

#define __GPIO_INTERFACE_DEF(__N, __VALUE)                                      \
            {                                                                   \
                &gpio##__N##_set_direction,                                     \
                &gpio##__N##_get_direction,                                     \
                &gpio##__N##_set_input,                                         \
                &gpio##__N##_set_output,                                        \
                &gpio##__N##_switch_direction,                                  \
                &gpio##__N##_read,                                              \
                &gpio##__N##_write,                                             \
                &gpio##__N##_set,                                               \
                &gpio##__N##_clear,                                             \
                &gpio##__N##_toggle,                                            \
            },

#define __GPIO_FUNC_DEF(__N, __VALUE)                                          \
static void gpio##__N##_set_direction(uint32_t wDirection, uint32_t wPinMask)   \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
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
    GPIO##__N##_REG.DIR &= ~wPinMask;                                             \
}                                                                               \
static void gpio##__N##_set_output(uint32_t wPinMask)                           \
{                                                                               \
    GPIO##__N##_REG.DIR |= wPinMask;                                              \
}                                                                               \
static void gpio##__N##_switch_direction(uint32_t wPinMask)                     \
{                                                                               \
    GPIO##__N##_REG.DIR ^= wPinMask;                                              \
}                                                                               \
static uint32_t gpio##__N##_read(void)                                          \
{                                                                               \
    return GPIO##__N##_REG.IN;                                                    \
}                                                                               \
static void gpio##__N##_write(uint32_t wValue, uint32_t wPinMask)               \
{                                                                               \
    SAFE_ATOM_CODE (                                                            \
        uint32_t wTemp = GPIO##__N##_REG.OUT & ~wPinMask;                         \
        wTemp |= (wValue & wPinMask);                                           \
        GPIO##__N##_REG.OUT = wTemp;                                              \
    )                                                                           \
                                                                                \
}                                                                               \
static void gpio##__N##_set(uint32_t wPinMask)                                  \
{                                                                               \
    GPIO##__N##_REG.OUTSET = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_clear(uint32_t wPinMask)                                \
{                                                                               \
    GPIO##__N##_REG.OUTCLR = wPinMask;                                            \
}                                                                               \
static void gpio##__N##_toggle(uint32_t wPinMask)                               \
{                                                                               \
    GPIO##__N##_REG.OUTTOG = wPinMask;                                            \
}                                                                                                                                             
                                                

/*============================ TYPES =========================================*/
//! \name io configuration structure
//! @{
typedef struct {
    em_io_pin_t     tPIN;           //!< pin number
    uint32_t        wFunction;      //!< io Funcitons
    uint32_t        wMode;          //!< io mode
} io_cfg_t;
//! @}

//! \name gpio control interface
//! @{
DEF_INTERFACE(i_gpio_t)
    void        (*SetDirection)(uint32_t wDirection, uint32_t wPinMask);
    uint32_t    (*GetDirection)(uint32_t wPinMask);
    void        (*SetInput)(uint32_t wPinMask);
    void        (*SetOutput)(uint32_t wPinMask);
    void        (*SwitchDirection)(uint32_t wPinMask);
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
    bool            (*Config)(io_cfg_t const *ptCFG, uint32_t wSize);//!< io configuration
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
static bool io_configuration(io_cfg_t const *ptCFG, uint32_t wSize);

/*============================ LOCAL VARIABLES ===============================*/
/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
const io_t IO = {
        &io_configuration,                  //!< general io configuration
        {
            {MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE_DEF, NULL)}
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
static bool io_configuration(io_cfg_t const *ptCFG, uint32_t wSize)
{
    bool tResult = true;

    if (NULL == ptCFG) {
        return false;
    }

    //! io configure
    for (; wSize; --wSize) {
        uint32_t wPinIndex  = ptCFG->tPIN;
        uint32_t wIOCTRL    = ptCFG->wMode ^ IOCTRL_PIN_ADMOD_MSK 
                            | IOCTRL_PIN_FUNSEL(ptCFG->wFunction);
        if (wPinIndex >= IO_PIN_COUNT) {
            return false;
        }
        IOCTRL_REG.PIN[wPinIndex].Value = wIOCTRL;
        ptCFG++;
    }

    return tResult;
}


  
/* EOF */