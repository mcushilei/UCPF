/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_sys.h"
#include ".\reg_pm.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum {
    PLL_CLKSRC_IRC  = 0,
    PLL_CLKSRC_OSC,
} em_pll_clk_src_t;

typedef enum {
    MAIN_CLKSRC_PLL      = 0x0,
    MAIN_CLKSRC_EXT,
    MAIN_CLKSRC_CURRENT,
} em_main_clk_src_t;

typedef enum {
    PERIPHERAL_CLOCK_I2C    = 0,
    PERIPHERAL_CLOCK_ADC,
    PERIPHERAL_CLOCK_DAC,
    PERIPHERAL_CLOCK_UART,
    PERIPHERAL_CLOCK_SPI,
    PERIPHERAL_CLOCK_TIMER,
    PERIPHERAL_CLOCK_INVALID_VALUE,
} em_peripheral_clk_t;

typedef enum {
    PERIPHERAL_POWER_INVALID_VALUE = 0,
    PERIPHERAL_POWER_I2C_SPI    = 1,
    PERIPHERAL_POWER_UART0,
    PERIPHERAL_POWER_PWM,
    PERIPHERAL_POWER_MAX_VALUE,
} em_peripheral_pw_t;

//! \name the lowpower mode
//! @{
typedef enum {
    SLEEP           = 0,
    DEEP_SLEEP,
    POWER_DOWN,
    DEEP_POWER_DOWN,
} em_lowpower_mode_t;
//! @}


/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern bool     pll_enable(void);
extern bool     pll_disable(void);
extern bool     pll_clock_select(uint32_t wSrc);
extern uint32_t pll_get_in_clock(void);
extern uint32_t pll_get_out_clock(void);
extern uint32_t main_clock_get(void);
extern bool     core_clock_config(uint32_t tSrc, uint32_t wDiv);
extern uint32_t core_clock_get(void);
extern bool     enter_lowpower_mode(uint32_t wMode);
extern bool     peripheral_clock_config(uint8_t chIndex, uint8_t chDiv);
extern uint32_t peripheral_clock_get_div(uint8_t chIndex);
extern uint32_t peripheral_clock_get(uint8_t chIndex);
extern bool     peripheral_power_enable(uint32_t wIndex);
extern bool     peripheral_power_disable(uint32_t wIndex);
extern uint32_t peripheral_power_status_get(uint32_t wIndex);
extern bool     peripheral_power_status_resume(uint32_t wIndex, uint32_t tStatus);

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/
bool pll_enable(void)
{
    return true;
}

bool pll_disable(void)
{
    return true;
}

bool pll_clock_select(uint32_t wSrc)
{
    uint32_t wReg = 0;
    
    switch (wSrc) {
        case PLL_CLKSRC_IRC:
            SAFE_ATOM_CODE(
                wReg = PLL_REG.POWCON0 & (~(1u << 6));  //!< disable external crystal
                PLL_REG.POWKEY1 = 0x01;
                PLL_REG.POWCON0 = wReg;
                PLL_REG.POWKEY2 = 0xF4; 
            )

            SAFE_ATOM_CODE(
                PLL_REG.PLLKEY1 = 0xAA;
                PLL_REG.PLLCON  =   (0u << 2) |
                                    (0u << 0); //!< select external crystal
                PLL_REG.PLLKEY2 = 0x55; 
            )
        break;
        
        case PLL_CLKSRC_OSC:
            SAFE_ATOM_CODE(
                wReg = PLL_REG.POWCON0 | (1u << 6);  //!< enable external crystal
                PLL_REG.POWKEY1 = 0x01;
                PLL_REG.POWCON0 = wReg;
                PLL_REG.POWKEY2 = 0xF4; 
            )

            SAFE_ATOM_CODE(
                PLL_REG.PLLKEY1 = 0xAA;
                PLL_REG.PLLCON  =   (0u << 2) |
                                    (2u << 0); //!< select external crystal
                PLL_REG.PLLKEY2 = 0x55; 
            )
        break;
        
        default:
            return false;
    }
    
    return true;
}

uint32_t pll_get_in_clock(void)
{
    uint32_t wReg = PLL_REG.PLLCON;
    
    if (wReg & (1u << 2)) {
        //! external clock.
        return EXT_CLOCK_FREQ;
    } else if (2 == (wReg & (3u << 0))) {
        //! external crystal.
        return CRYSTAL_FREQ;
    } else {
        //! internal oscillator.
        return INT_OSC_FREQ;
    }
}

uint32_t pll_get_out_clock(void)
{
    return PLL_OUT_FREQ;
}

uint32_t main_clock_get(void)
{
    return PLL_OUT_FREQ;
}

bool core_clock_config(uint32_t wSrc, uint32_t wDiv)
{
    uint32_t wReg = 0;
    
    //!< select source
    switch (wSrc) {
        case MAIN_CLKSRC_PLL:
            SAFE_ATOM_CODE(
                wReg = PLL_REG.PLLCON;
                wReg &= ~(1u << 2);     //!< deselect external clock.
                PLL_REG.PLLKEY1 = 0xAA;
                PLL_REG.PLLCON  = wReg;
                PLL_REG.PLLKEY2 = 0x55;
            )
        break;
        
        case MAIN_CLKSRC_EXT:
            SAFE_ATOM_CODE(
                wReg = PLL_REG.PLLCON;
                wReg |= 1u << 2;        //!< select external clock.
                PLL_REG.PLLKEY1 = 0xAA;
                PLL_REG.PLLCON  = wReg;
                PLL_REG.PLLKEY2 = 0x55; 
            )
        break;
        
        default:
            return false;
    }
    
    //!< select div
    SAFE_ATOM_CODE(
        wReg  = PLL_REG.POWCON0;
        wReg &= ~(0x07u << 0);
        wReg |= (wDiv & 0x07u) << 0;
        PLL_REG.POWKEY1 = 0x01;
        PLL_REG.POWCON0 = wReg;
        PLL_REG.POWKEY2 = 0xF4; 
    )

    return true;
}

uint32_t core_clock_get(void)
{
    uint32_t wReg = 0;
    uint32_t wCLK;
    
    if ((1u << 2) &PLL_REG.PLLCON) {    //!< external ckock.
        wCLK = EXT_CLOCK_FREQ;
    } else {
        wCLK = pll_get_out_clock();
    }
    
    wReg = (PLL_REG.POWCON0 >> 0) & 0x07;
    return wCLK / (1u << wReg);
}

bool peripheral_clock_config(uint8_t chIndex , uint8_t chDiv)
{
    return true;
}

uint32_t peripheral_clock_get_div(uint8_t chIndex)
{
    return 0;
}

uint32_t peripheral_clock_get(uint8_t chIndex)
{
    switch (chIndex) {
        case PERIPHERAL_CLOCK_I2C:
        case PERIPHERAL_CLOCK_ADC:
        case PERIPHERAL_CLOCK_DAC:
            return main_clock_get();
        default:
            return core_clock_get();
    }
}

bool peripheral_power_enable(uint32_t wIndex)
{
    uint32_t wReg;
    
    if ((PERIPHERAL_POWER_MAX_VALUE <= wIndex)
    ||  (PERIPHERAL_POWER_INVALID_VALUE == wIndex)) {
        return false;
    }
    SAFE_ATOM_CODE(
        wReg = PLL_REG.POWCON1 | (0x01u << (wIndex * 3 - 1));
        PLL_REG.POWKEY3 = 0x76;
        PLL_REG.POWCON1 = wReg;
        PLL_REG.POWKEY4 = 0xB1;
    )
    
    return true;
}

bool peripheral_power_disable(uint32_t wIndex)
{
    uint32_t wReg = 0;
    
    if ((PERIPHERAL_POWER_MAX_VALUE <= wIndex)
    ||  (PERIPHERAL_POWER_INVALID_VALUE == wIndex)) {
        return false;
    }
    SAFE_ATOM_CODE(
        wReg = PLL_REG.POWCON1 & (~(0x01u << (wIndex * 3 - 1)));
        PLL_REG.POWKEY3 = 0x76;
        PLL_REG.POWCON1 = wReg;
        PLL_REG.POWKEY4 = 0xB1;
    )
    
    return true;
}

uint32_t peripheral_power_status_get(uint32_t wIndex)
{
    if ((PERIPHERAL_POWER_MAX_VALUE <= wIndex)
    ||  (PERIPHERAL_POWER_INVALID_VALUE == wIndex)) {
        return false;
    }
    
    return PLL_REG.POWCON1 & (~(0x01u << (wIndex * 3 - 1)));
}

bool peripheral_power_status_resume(uint32_t wIndex , uint32_t tStatus)
{
    uint32_t wReg = 0, wMSK = 0;
    
    if ((PERIPHERAL_POWER_MAX_VALUE <= wIndex)
    ||  (PERIPHERAL_POWER_INVALID_VALUE == wIndex)) {
        return false;
    }
    
    wMSK = ~(0x01u << (wIndex * 3 - 1));
    SAFE_ATOM_CODE(
        wReg = PLL_REG.POWCON1
             & wMSK
             | (tStatus & wMSK);
        PLL_REG.POWKEY3 = 0x76;
        PLL_REG.POWCON1 = wReg;
        PLL_REG.POWKEY4 = 0xB1;
    )
    
    return true;
}

bool enter_lowpower_mode(uint32_t wMode)
{
    uint32_t wReg;
    
    switch (wMode) {
        case SLEEP:
            wReg = PLL_REG.POWCON0 & (~(1u << 3));
            break;
        case DEEP_SLEEP:
        case POWER_DOWN:
            wReg = PLL_REG.POWCON0 & (~((1u << 3) | (1u << 4) | (1u << 5)));
            break;
        default:
            wReg = PLL_REG.POWCON0;
            break;
    }
    PLL_REG.POWKEY1 = 0x01;
    PLL_REG.POWCON0 = wReg;
    PLL_REG.POWKEY2 = 0xF4; 

    return true;
}
