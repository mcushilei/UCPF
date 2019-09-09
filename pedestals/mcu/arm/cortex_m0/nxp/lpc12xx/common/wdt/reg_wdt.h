#ifndef __I_IO_WDT_H__
#define __I_IO_WDT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define WDT_REG                         (*(volatile wdt_reg_t *)WDT_BASE_ADDRESS)

//! \name REGISTER DEFIEN
//! @{
/* ---------------------------REGISTER DEFINE BEGIN ------------------------- */

//! \brief Watchdog Mode register 
#define WDT_MOD_REG                     GSP_WDT.MOD
#define WDT_MOD                         GSP_WDT.MOD.Value
#define WDT_MOD_BIT                     GSP_WDT.MOD.Bits

//! \brief Watchdog Timer Constant register
#define WDT_TC_REG                      GSP_WDT.TC
#define WDT_TC                          WDT_TC_REG.Value
#define WDT_TC_BIT                      WDT_TC_REG
                 
//! \brief Watchdog Feed register 
#define WDT_FEED_REG                    GSP_WDT.FEED
#define WDT_FEED                        WDT_FEED_REG.Value
#define WDT_FEED_BIT                    WDT_FEED_REG

//! \brief  Watchdog Timer Value register
#define WDT_TV_REG                      GSP_WDT.TV
#define WDT_TV                          WDT_TV_REG.Value
#define WDT_TV_BIT                      WDT_TV_REG


//! \brief Watchdog Timer Warning Interrupt register  
#define WDT_WARNINT_REG                 GSP_WDT.WARNINT
#define WDT_WARNINT                     WDT_WARNINT_REG.Value
#define WDT_WARNINT_BIT                 WDT_WARNINT_REG

//! \brief Watchdog Timer Window register  
#define WDT_WINDOW_REG                  GSP_WDT.WINDOW
#define WDT_WINDOW                      WDT_WINDOW_REG.Value
#define WDT_WINDOW_BIT                  WDT_WINDOW_REG

/* ----------------------------REGISTER DEFINE  END-------------------------- */
//! @}

//! \name DEFINE REGISTER OP MAC 
//! @{
/* -----------------------DEFINE REGISTER OP MAC  BETIN---------------------- */

//! \brief Watchdog Mode register 
#define WDT_MOD_WDEN                    0
#define WDT_MOD_WDEN_MSK               (1 << WDT_MOD_WDEN)

#define WDT_MOD_WDRESET                 1
#define WDT_MOD_WDRESET_MSK            (1<<WDT_MOD_WDRESET)

#define WDT_MOD_WDTOF                   2
#define WDT_MOD_WDTOF_MSK              (1<<WDT_MOD_WDTOF)

#define WDT_MOD_WDINT                   3
#define WDT_MOD_WDINT_MSK              (1<<WDT_MOD_WDINT)

#define WDT_MOD_WDPROTECT               4
#define WDT_MOD_WDPROTECT_MSK          (1<<WDT_MOD_WDPROTECT)

#define WDT_MOD_LOCK                    5
#define WDT_MOD_LOCK_MSK               (1<<WDT_MOD_LOCK)

//! \brief Watchdog Timer Constant register
#define WDT_TC_WDTC                     0
#define WDT_TC_WDTC_MSK                (0xFFFFFF << WDT_TC_WDTC)

//! \brief Watchdog Feed register
#define WDT_FEED_WDTC                   0
#define WDT_FEED_WDTC_MSK              (0xFF << WDT_FEED_WDTC)

//! \brief Watchdog Timer Value register
#define WDT_TV_WDTC                     0
#define WDT_TV_WDTC_MSK                (0xFFFFFF << WDT_TV_WDTC)

//! \brief Watchdog Warning Interrupt register
#define WDT_WARNINT_WDTC                0
#define WDT_WARNINT_WDTC_MSK           (0x3FF << WDT_WARNINT_WDTC)

//! \brief Watchdog Timer Window register
#define WDT_WINDOW_WDTC                 0
#define WDT_WINDOW_WDTC_MSK            (0xFFFFFF << WDT_WINDOW_WDTC)

/* --------------------------DEFINE REGISTER OP MAC   END-------------------- */
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name register page type
//! @{
typedef struct {
    //! \brief Watchdog Mode register
    DEF_REG32
        reg32_t WDEN        : 1;
        reg32_t WDRESET     : 1;
        reg32_t WDTOF       : 1;
        reg32_t WDINT       : 1;
        reg32_t WDPROTECT   : 1;
        reg32_t WDLOCKCLK   : 1;
        reg32_t WDLOCKDP    : 1;
        reg32_t WDLOCKEN    : 1;
        reg32_t             :24;
    END_DEF_REG32(MOD) 

    //! \brief Watchdog Timer Constant register
    DEF_REG32
        reg32_t WDTC    :24;
        reg32_t         : 8;
    END_DEF_REG32(TC) 

    //! \brief Watchdog Feed register
    reg32_t FEED;

    //! \brief Watchdog Timer Value register
    DEF_REG32
        reg32_t COUNT   :24;
        reg32_t         :8;
    END_DEF_REG32(TV) 

    //! \brief Watchdog Timer Clock Source Selection register
    DEF_REG32
        reg32_t WDSEL   :2;
        reg32_t         :29;
        reg32_t WDLOCK  :1;
    END_DEF_REG32(CLKSEL)
    
    //! \brief Watchdog Warning Interrupt register
    DEF_REG32
        reg32_t WARNINT :10;
        reg32_t         :22;
    END_DEF_REG32(WARNINT)

    //! \brief Watchdog Timer Window register
    DEF_REG32
        reg32_t WINDOW  :24;
        reg32_t         : 8;
    END_DEF_REG32(WINDOW)
} wdt_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif