#ifndef __I_IO_SCON_H__
#define __I_IO_SCON_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define SYSCON_REG              (*(volatile syscon_reg_t *)SYSCON_BASE_ADDRESS)
#define FLASH_REG               (*(volatile flash_reg_t *)FLASH_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
#define SSC_SYSPLLCTRL_MSEL_BIT0        0
#define SSC_SYSPLLCTRL_MSEL_BIT1        1
#define SSC_SYSPLLCTRL_MSEL_BIT2        2
#define SSC_SYSPLLCTRL_MSEL_BIT3        3
#define SSC_SYSPLLCTRL_MSEL_BIT4        4
#define SSC_SYSPLLCTRL_MSEL_MSK         (   (1ul << SSC_SYSPLLCTRL_MSEL_BIT0)         \
                                        |   (1ul << SSC_SYSPLLCTRL_MSEL_BIT1)         \
                                        |   (1ul << SSC_SYSPLLCTRL_MSEL_BIT2)         \
                                        |   (1ul << SSC_SYSPLLCTRL_MSEL_BIT3)         \
                                        |   (1ul << SSC_SYSPLLCTRL_MSEL_BIT4)         )
#define SSC_SYSPLLCTRL_MSEL(__SEL)  \
            (((__SEL) << SSC_SYSPLLCTRL_MSEL_BIT0) & SSC_SYSPLLCTRL_MSEL_MSK)

#define SSC_SYSPLLCTRL_PSEL_BIT0        5
#define SSC_SYSPLLCTRL_PSEL_BIT1        6
#define SSC_SYSPLLCTRL_PSEL_MSK         (   (1ul << SSC_SYSPLLCTRL_PSEL_BIT0)         \
                                        |   (1ul << SSC_SYSPLLCTRL_PSEL_BIT1))
#define SSC_SYSPLLCTRL_PSEL(__SEL)  \
            (((__SEL) << SSC_SYSPLLCTRL_PSEL_BIT0) & SSC_SYSPLLCTRL_PSEL_MSK)

#define SSC_SYSPLLSTAT_LOCK_BIT0        0
#define SSC_SYSPLLSTAT_LOCK_MSK         (1ul << SSC_SYSPLLSTAT_LOCK_BIT0)

#define SSC_SYSPLLCLKSEL_SEL_BIT0       0
#define SSC_SYSPLLCLKSEL_SEL_BIT1       1
#define SSC_SYSPLLCLKSEL_SEL_MSK        ((1ul << SSC_SYSPLLCLKSEL_SEL_BIT0) \
                                        |(1ul << SSC_SYSPLLCLKSEL_SEL_BIT1)) 

#define SSC_SYSPLLCLKUEN_ENA_BIT0            0
#define SSC_SYSPLLCLKUEN_ENA_MSK        (1ul<<SSC_SYSPLLCLKUEN_ENA_BIT0)

#define SSC_MAINCLKSEL_SEL_BIT0     0
#define SSC_MAINCLKSEL_SEL_BIT1     1
#define SSC_MAINCLKSEL_SEL_BIT2     2
#define SSC_MAINCLKSEL_SEL_MSK      ((1ul << SSC_MAINCLKSEL_SEL_BIT0)    \
                                    |(1ul << SSC_MAINCLKSEL_SEL_BIT1)    \
                                    |(1ul << SSC_MAINCLKSEL_SEL_BIT2))
#define SSC_MAINCLKSEL_SEL(__SEL)   \
            (((__SEL) << SSC_MAINCLKSEL_SEL_BIT0) & SSC_MAINCLKSEL_SEL_MSK)

#define SSC_MAINCLKUEN_ENA_BIT0     0
#define SSC_MAINCLKUEN_ENA_MSK      (1ul<<SSC_MAINCLKUEN_ENA_BIT0)

#define SSC_AHBCLK_CORE_MSK         (1ul<< AHBCLK_CORE)
#define SSC_AHBCLK_ROM_MSK          (1ul<< AHBCLK_ROM)
#define SSC_AHBCLK_SRAM_MSK         (1ul<< AHBCLK_SRAM)
#define SSC_AHBCLK_FLASHC_MSK       (1ul<< AHBCLK_FLASHREG)
#define SSC_AHBCLK_FLASH_MSK        (1ul<< AHBCLK_FLASH)
#define SSC_AHBCLK_I2C_MSK          (1ul<< AHBCLK_I2C)
#define SSC_AHBCLK_CRC_MSK          (1ul<< AHBCLK_CRC)
#define SSC_AHBCLK_TIMER0_MSK       (1ul<< AHBCLK_TIMER0)
#define SSC_AHBCLK_TIMER1_MSK       (1ul<< AHBCLK_TIMER1)
#define SSC_AHBCLK_TIMER2_MSK       (1ul<< AHBCLK_TIMER2)
#define SSC_AHBCLK_TIMER3_MSK       (1ul<< AHBCLK_TIMER3)
#define SSC_AHBCLK_SPI_MSK          (1ul<< AHBCLK_SPI)
#define SSC_AHBCLK_UART0_MSK        (1ul<< AHBCLK_UART0)
#define SSC_AHBCLK_UART1_MSK        (1ul<< AHBCLK_UART1)
#define SSC_AHBCLK_ADC_MSK          (1ul<< AHBCLK_ADC)
#define SSC_AHBCLK_WDT_MSK          (1ul<< AHBCLK_WDT)
#define SSC_AHBCLK_IOCTRL_MSK       (1ul<< AHBCLK_IOCTRL)
#define SSC_AHBCLK_DMA_MSK          (1ul<< AHBCLK_DMA)
#define SSC_AHBCLK_RTC_MSK          (1ul<< AHBCLK_RTC)
#define SSC_AHBCLK_CMP_MSK          (1ul<< AHBCLK_CMP)
#define SSC_AHBCLK_GPIOAHB2_MSK     (1ul<< AHBCLK_GPIOAHB2)
#define SSC_AHBCLK_GPIOAHB1_MSK     (1ul<< AHBCLK_GPIOAHB1)
#define SSC_AHBCLK_GPIOAHB0_MSK     (1ul<< AHBCLK_GPIOAHB0)

#define SSC_POWER_IRCOUT_MSK        (1ul<<POWER_IRCOUT)
#define SSC_POWER_IRC_MSK           (1ul<<POWER_IRC)
#define SSC_POWER_FLASH_MSK         (1ul<<POWER_FLASH)
#define SSC_POWER_BOD_MSK           (1ul<<POWER_BOD)
#define SSC_POWER_ADC_MSK           (1ul<<POWER_ADC)
#define SSC_POWER_SYSOSC_MSK        (1ul<<POWER_SYSOSC)
#define SSC_POWER_WDTOSC_MSK        (1ul<<POWER_WDTOSC)
#define SSC_POWER_SYSPLL_MSK        (1ul<<POWER_SYSPLL)
#define SSC_POWER_COMP_MSK          (1ul<<POWER_COMP)

/*============================ TYPES =========================================*/
//! \name Peripheral AHB Clock Macros
//! @{
typedef enum {
    AHBCLK_CORE             = 0,
    AHBCLK_ROM              = 1,
    AHBCLK_SRAM             = 2,
    AHBCLK_FLASHREG         = 3,
    AHBCLK_FLASH            = 4,
    AHBCLK_I2C0             = 5,
    AHBCLK_CRC              = 6,
    AHBCLK_TIMER0           = 7,
    AHBCLK_TIMER1           = 8,
    AHBCLK_TIMER2           = 9,
    AHBCLK_TIMER3           = 10,
    AHBCLK_SPI0             = 11,
    AHBCLK_UART0           = 12,
    AHBCLK_UART1           = 13,
    AHBCLK_ADC              = 14,
    AHBCLK_WDT              = 15,
    AHBCLK_IOCTRL           = 16,
    AHBCLK_DMA              = 17,
    AHBCLK_RTC              = 19,
    AHBCLK_CMP              = 20,
    AHBCLK_GPIOAHB2         = 29,
    AHBCLK_GPIOAHB1         = 30,
    AHBCLK_GPIOAHB0         = 31,
} em_ahb_clk_t;
//! @}

//! \name peripheral clock index
//! @{
typedef enum {
    PCLK_SPI0               = 0,    //!< SPI
    PCLK_UART0             = 1,    //!< UART0
    PCLK_UART1             = 2,    //!< UART1
    PCLK_RTC                = 3,    //!< RTC
    PCLK_MAX_INDEX          = 3,
} em_pclk_t;
//! @}

//! \name analog circuit power set index
//! @{
typedef enum {
    POWER_IRCOUT               = 0,
    POWER_IRC                  = 1,
    POWER_FLASH                = 2,
    POWER_BOD                  = 3,
    POWER_ADC                  = 4,
    POWER_SYSOSC               = 5,
    POWER_WDTOSC               = 6,
    POWER_SYSPLL               = 7,
    POWER_COMP                 = 15,
} em_power_t;
//! @}

//! \brief system control block register page
//! @{
typedef struct {
    DEF_REG32
        reg32_t MAP             :2;
        reg32_t                 :30;
    END_DEF_REG32(SYSMEMREMAP)            //!< R/W 0x000 System memory remap 

    DEF_REG32
        reg32_t SSP             :1;
        reg32_t I2C             :1;
        reg32_t UART0           :1;
        reg32_t UART1           :1;
        reg32_t CT16B0          :1;
        reg32_t CT16B1          :1;
        reg32_t CT32B0          :1;
        reg32_t CT32B1          :1;
        reg32_t CMP             :1;
        reg32_t CRC             :1;
        reg32_t DMA             :1;
        reg32_t                 :1;
        reg32_t                 :1;
        reg32_t                 :1;
        reg32_t                 :1;
        reg32_t FLASH_OVERRIDE  :1;
        reg32_t                 :16;
    END_DEF_REG32(PRESETCTRL)             //!< R/W 0x004 Peripheral reset 

    DEF_REG32
        reg32_t MSEL            :5;    
        reg32_t PSEL            :2;
        reg32_t                 :25;
    END_DEF_REG32(SYSPLLCTRL)             //!< R/W 0x008 System PLL control

    DEF_REG32
        reg32_t LOCK            :1;
        reg32_t                 :31;
    END_DEF_REG32(SYSPLLSTAT)             //!< R 0x00C System PLL status

    REG32_RSVD(4)    //!< Reserved

    DEF_REG32
        reg32_t BYPASS          :1;
        reg32_t FREQRANGE       :1;
        reg32_t                 :30;
    END_DEF_REG32(SYSOSCCTRL)               //!< R/W 0x020 System oscillator control

    DEF_REG32
        reg32_t DIVSEL          :5;
        reg32_t FREQSEL         :4;
        reg32_t                 :23;
    END_DEF_REG32(WDTOSCCTRL)             //!< R/W 0x024 Watchdog oscillator control

    DEF_REG32
        reg32_t TRIM            :8;
        reg32_t                 :24;
    END_DEF_REG32(IRCCTRL)                  //!< R/W 0x028 Internal resonant crystal control 

    REG32_RSVD(1)    //!< Reserved

    DEF_REG32
        reg32_t POR             :1;
        reg32_t EXTRST          :1;
        reg32_t WDT             :1;
        reg32_t BOD             :1;
        reg32_t SYSRST          :1;
        reg32_t                 :27;
    END_DEF_REG32(SYSRSTSTAT)             //!< R/W 0x030 System reset status register 

    REG32_RSVD(3)    //!< Reserved

    DEF_REG32
        reg32_t SEL             :2;
        reg32_t                 :30;
    END_DEF_REG32(SYSPLLCLKSEL)           //!< R/W 0x040 System PLL  clock source select 

    DEF_REG32
        reg32_t ENA             :1;
        reg32_t                 :31;
    END_DEF_REG32(SYSPLLCLKUEN)           //!< R/W 0x044 System  PLL clock source update enable 

    REG32_RSVD(10)    //!< Reserved

    DEF_REG32
        reg32_t SEL             :2;
        reg32_t                 :30;
    END_DEF_REG32(MAINCLKSEL)             //!< R/W 0x070 Main clock source 

    DEF_REG32
        reg32_t ENA             :1;
        reg32_t                 :31;
    END_DEF_REG32(MAINCLKUEN)             //!< R/W 0x074 Main clock source update enable 

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(SYSAHBCLKDIV)           //!< R/W 0x078 System clock divider 

    REG32_RSVD(1)    //!< Reserved

    reg32_t     SYSAHBCLKCTRL;              //!< R/W 0x080 AHB clock control

    REG32_RSVD(4)    //!< Reserved

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(SSPCLKDIV)             //!< R/W 0x094 SSP0  clock divider 

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(UART0CLKDIV)            //!< R/W 0x098 UART0 clock 

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(UART1CLKDIV)            //!< R/W 0x09C UART1  clock 

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(RTCCLKDIV)             //!< R/W 0x0A0 RTC0  clock 

    REG32_RSVD(15)    //!< Reserved

    DEF_REG32
        reg32_t SEL             :2;
        reg32_t                 :30;
    END_DEF_REG32(CLKOUTSEL)              //!< R/W 0x0E0 CLKOUT clock source 

    DEF_REG32
        reg32_t ENA             :1;
        reg32_t                 :31;
    END_DEF_REG32(CLKOUTUEN)              //!< R/W 0x0E4 CLKOUT clock source update 

    DEF_REG32
        reg32_t DIV             :8;
        reg32_t                 :24;
    END_DEF_REG32(CLKOUTDIV)              //!< R/W 0x0E8 CLKOUT clock 

    REG32_RSVD(5)    //!< Reserved

    DEF_REG32
        reg32_t PIOSTAT         :24;
        reg32_t                 :8;
    END_DEF_REG32(PIOPORCAP0)             //!< R 0x100 POR captured PIO status 0 user dependent 

    reg32_t     PIOPORCAP1;                 //!< R 0x104 POR captured PIO status 1 user dependent 

    REG32_RSVD(11)    //!< Reserved

    reg32_t     IOCONFIGCLKDIV6;
    reg32_t     IOCONFIGCLKDIV5;
    reg32_t     IOCONFIGCLKDIV4;
    reg32_t     IOCONFIGCLKDIV3;
    reg32_t     IOCONFIGCLKDIV2;
    reg32_t     IOCONFIGCLKDIV1;
    reg32_t     IOCONFIGCLKDIV0;

    DEF_REG32
        reg32_t BODRSTLEV       :2;
        reg32_t BODINTVAL       :2;
        reg32_t BODRSTENA       :1;
        reg32_t                 :27;
    END_DEF_REG32(BODCTRL)                //!< R/W 0x150 Brown-Out 

    DEF_REG32
        reg32_t CALIB           :26;
        reg32_t                 :6;
    END_DEF_REG32(STCALIB)                //!< R/W 0x154 System tick counter calibration 

    reg32_t AHBPRIO;                 /*!< Offset: 0x158 AHB priority setting (R/W) */

    REG32_RSVD(5)    //!< Reserved

    DEF_REG32
        reg32_t LATENCY         :8;
        reg32_t                 :24;
    END_DEF_REG32(IRQL)                   //!< R/W 0x170 IRQ delay. Allows trade-off between interrupt latency and determinism. 

    DEF_REG32
        reg32_t IRQN            :5;
        reg32_t                 :26;
        reg32_t NMIEN           :1;
    END_DEF_REG32(NMISRC)                 //!< R/W 0x174 NMI Source 

    REG32_RSVD(34)    //!< Reserved

    reg32_t STARTAPRP0;             /*!< Offset: 0x200 Start logic edge control Register 0 (R/W) */   
    
    DEF_REG32
        reg32_t PINT0           :1;
        reg32_t PINT1           :1;
        reg32_t PINT2           :1;
        reg32_t PINT3           :1;
        reg32_t PINT4           :1;
        reg32_t PINT5           :1;
        reg32_t PINT6           :1;
        reg32_t PINT7           :1;
        reg32_t                 :24;
    END_DEF_REG32(STARTERP0)              //!< R/W 0x204 Start logic 0 interrupt wake-up enable 

    reg32_t STARTRSRP0CLR;          /*!< Offset: 0x208 Start logic reset Register 0  ( /W) */
    reg32_t STARTSRP0;              /*!< Offset: 0x20C Start logic status Register 0 (R/) */
    reg32_t STARTAPRP1;             /*!< Offset: 0x210 Start logic edge control Register 1 (R/W) */  

    DEF_REG32
        reg32_t                 :12;
        reg32_t WWDTINT         :1;
        reg32_t BODINT          :1;
        reg32_t                 :5;
        reg32_t USB_WAKEUP      :1;
        reg32_t GPIOINT0        :1;
        reg32_t GPIOINT1        :1;
        reg32_t                 :1;
    END_DEF_REG32(STARTERP1)              //!< R/W 0x214 Start logic 1 interrupt wake-up enable register
    reg32_t STARTRSRP1CLR;          /*!< Offset: 0x218 Start logic reset Register 1  ( /W) */
    reg32_t STARTSRP1;              /*!< Offset: 0x21C Start logic status Register 1 (R/) */  

    REG32_RSVD(4)    //!< Reserved

    reg32_t PDSLEEPCFG;
    reg32_t PDAWAKECFG;
    DEF_REG32
        reg32_t IRCOUT          :1;     //!< 0
        reg32_t IRC             :1;     //!< 1
        reg32_t FLASH           :1;     //!< 2
        reg32_t BOD             :1;     //!< 3
        reg32_t ADC             :1;     //!< 4
        reg32_t SYSOSC          :1;     //!< 5
        reg32_t WDTOSC          :1;     //!< 6
        reg32_t SYSPLL          :1;     //!< 7
        reg32_t                 :7;     //!< 8~14 Reserved
        reg32_t COMP            :1;     //!< 15
        reg32_t                 :16;    //!< 16~31 Reserved
    END_DEF_REG32(PDRUNCFG)              //!< R/W 0x238 Power-down configuration

    REG32_RSVD(110) //!< Reserved

    reg32_t DEVICE_ID;                  //!< R 0x3F4 Device ID
} syscon_reg_t;
//! @}

typedef struct {
    REG32_RSVD(7) //!< Reserved
        
    DEF_REG32
        reg32_t RDCFG           :2;
        reg32_t                 :30;
    END_DEF_REG32(FLASHCFG)              
} flash_reg_t; 

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#endif
