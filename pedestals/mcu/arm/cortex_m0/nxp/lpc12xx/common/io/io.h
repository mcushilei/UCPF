
#ifndef __LPC_IO_H__
#define __LPC_IO_H__
#ifndef __LPC_IO_C__


/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_gpio.h"
#include ".\reg_ioctrl.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define __GPIO_INTERFACE(__N, __VALUE)      i_gpio_t GPIO##__N;

#define __IO_PORT_PIN_MSK(__N, __OFFSET)    PIN##__N##_MSK = (1ul << (__N)),

//! \brief enable pull-up
#define IO_ENABLE_PULL_UP(__PIN)        do {                                \
            IOCTRL_REG.PIN[(__PIN)].PUE = 1;                                \
        } while(0)

//! \brief disable pull-up
#define IO_DISABLE_PULL_UP(__PIN)       do {                                \
            IOCTRL_REG.PIN[(__PIN)].PUE = 0;                                \
        } while(0)

//! \brief io function selection
#define IO_FUNCTION_SELECT(__PIN, __FUNC)   do {                            \
            IOCTRL_REG.PIN[(__PIN)].FUNSEL = (__FUNC);                      \
        } while(0)

#define IO_CFG(...)                     do {                            \
            io_cfg_t tCFG[] = {__VA_ARGS__};                            \
            uint32_t i = 0;                                         \
            for (; i != ARRAY_LENGTH(tCFG); ++i) {                    \
                uint32_t wPinIndex  = tCFG[i].tPIN;                 \
                uint32_t wIOCTRL    = tCFG[i].wMode ^ IOCTRL_PIN_ADMOD_MSK \
                                    | IOCTRL_PIN_FUNSEL(tCFG[i].wFunction);\
                if (wPinIndex >= IO_PIN_COUNT) {                        \
                    break;                                              \
                }                                                       \
                IOCTRL_REG.PIN[wPinIndex].Value = wIOCTRL;              \
            }                                                           \
        } while(0)

/*============================ TYPES =========================================*/
//! \name pin mask
//! @{
typedef enum {
    MREPEAT(IO_PORT_PIN_COUNT, __IO_PORT_PIN_MSK, 0)
} em_io_pin_msk_t;
//! @}

//! \name Port No.
//! @{
typedef enum {
#if defined(IO_PORTA)
    PA,
#endif
#if defined(IO_PORTB)
    PB,
#endif
#if defined(IO_PORTC)
    PC,
#endif
#if defined(IO_PORTD)
    PD,
#endif
} em_io_port_no_t;
//! @}

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
            MREPEAT(IO_PORT_COUNT, __GPIO_INTERFACE, 0)
        };
    };
END_DEF_INTERFACE(io_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief io interface
extern const io_t IO;

/*============================ PROTOTYPES ====================================*/

#endif
#endif
/* EOF */
