#ifndef __REG_WDT_H__
#define __REG_WDT_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
#define WWDT_REG                         (*(wdt_reg_t *)WDT_BASE_ADDRESS)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//! \name register page type
//! @{
typedef volatile struct {
	reg32_t         MOD;			/*!< Watchdog mode register. This register contains the basic mode and status of the Watchdog Timer. */
	reg32_t         TC;			/*!< Watchdog timer constant register. This register determines the time-out value. */
	reg32_t         FEED;		/*!< Watchdog feed sequence register. Writing 0xAA followed by 0x55 to this register reloads the Watchdog timer with the value contained in WDTC. */
	const  reg32_t  TV;			/*!< Watchdog timer value register. This register reads out the current value of the Watchdog timer. */
	reg32_t         CLKSEL;		/*!< Watchdog clock select register. */
	reg32_t         WARNINT;		/*!< Watchdog warning interrupt register. This register contains the Watchdog warning interrupt compare value. */
	reg32_t         WINDOW;		/*!< Watchdog timer window register. This register contains the Watchdog window value. */
} wdt_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
