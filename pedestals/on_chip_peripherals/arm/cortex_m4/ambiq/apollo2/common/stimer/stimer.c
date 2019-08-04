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




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include "..\device.h"
#include ".\reg_stimer.h"
#include "..\gpio\gpio.h"
#include "..\sys\pm.h"
#include ".\stimer.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

//*****************************************************************************
//
//! @brief Set up the stimer.
//!
//! @param ui32STimerConfig is the value to load into the configuration reg.
//!
//! This function should be used to perform the initial set-up of the
//! stimer.
//!
//! @return The 32-bit current config of the STimer Config register
//
//*****************************************************************************
uint32_t driver_stimer_config(uint32_t ui32STimerConfig)
{
    uint32_t ui32CurrVal;

    //
    // Read the current config
    //
    ui32CurrVal = REG_STIMER.STCFG;

    //
    // Write our configuration value.
    //
    REG_STIMER.STCFG = ui32STimerConfig;

    //
    // If all of the clock sources are not HFRC, disable LDO when sleeping if timers are enabled.
    //
    if ( (REG_BFR(STIMER, STCFG, CLKSEL) == REG_STIMER_STCFG_CLKSEL_HFRC_DIV16)   ||
         (REG_BFR(STIMER, STCFG, CLKSEL) == REG_STIMER_STCFG_CLKSEL_HFRC_DIV256) ) {
             
        REG_BFW(PWRCTRL, MISCOPT, DIS_LDOLPMODE_TIMERS, 0);
        
    } else {
        
        REG_BFW(PWRCTRL, MISCOPT, DIS_LDOLPMODE_TIMERS, 1);
        
    }
    return ui32CurrVal;
}

//*****************************************************************************
//
//! @brief Get the current stimer value.
//!
//! This function can be used to read, uninvasively, the value in the stimer.
//!
//! @return The 32-bit value from the STimer counter register.
//
//*****************************************************************************
uint32_t driver_stimer_counter_get(void)
{
    return REG_STIMER.STTMR;
}

//*****************************************************************************
//
//! @brief Clear the stimer counter.
//!
//! This function clears the STimer Counter and leaves the stimer running.
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_counter_clear(void)
{
    //
    // Set the clear bit
    //
    REG_STIMER.STCFG |= REG_STIMER_STCFG_CLEAR_M;

    //
    // Reset the clear bit
    //
    REG_STIMER.STCFG &= ~REG_STIMER_STCFG_CLEAR_M;
}

//*****************************************************************************
//
//! @brief Set the compare value.
//!
//! @param ui32CmprInstance is the compare register instance number (0-7).
//! @param ui32Delta        is the value to add to the STimer counter and load into
//!                         the comparator register. It should be > 1
//!
//! NOTE: There is no way to set an absolute value into a comparator register.
//!       Only deltas added to the STimer counter can be written to the compare
//!       registers.
//! CAUTION: The HAL implementation requires temporarily disabling the
//!       comparison. To avoid the remote possibility of losing an interrupt
//!       during that time, delta should always be set to a value greater than 1
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_compare_delta_set(uint32_t ui32CmprInstance, uint32_t ui32Delta)
{
    uint32_t cfgVal;
    
    if ( ui32CmprInstance > 7 ) {
        return;        
    }

    cfgVal = REG_STIMER.STCFG;
    // We need to disable the compare temporarily while setting the delta value
    // That leaves a corner case where we could miss the trigger if setting a very
    // small delta. To avoid this, we take critical section, and we should ensure
    // that delta value is at least > 1

    // Disable the compare if already enabled, when setting the new value
    REG_STIMER.STCFG &= ~((REG_STIMER_STCFG_COMPARE_A_EN_ENABLE << ui32CmprInstance));
    //
    // Start a critical section.
    //
    REG_STIMER.SCMPR[ui32CmprInstance] = ui32Delta;
    // Restore Compare Enable bit
    REG_STIMER.STCFG |= cfgVal & (REG_STIMER_STCFG_COMPARE_A_EN_ENABLE << ui32CmprInstance);
    //
    // End the critical section.
    //
}

//*****************************************************************************
//
//! @brief Get the current stimer compare register value.
//!
//! @param ui32CmprInstance is the compare register instance number (0-7).
//!
//! This function can be used to read the value in an stimer compare register.
//!
//!
//! @return None.
//
//*****************************************************************************
uint32_t driver_stimer_compare_get(uint32_t ui32CmprInstance)
{
    if ( ui32CmprInstance > 7 ) {
        
        return 0;
        
    }

    return REG_STIMER.SCMPR[ui32CmprInstance];
}

//*****************************************************************************
//
//! @brief Start capturing data with the specified capture register.
//!
//! @param ui32CaptureNum is the Capture Register Number to read (0-3).
//!        ui32GPIONumber is the pin number.
//!        bPolarity: false (0) = Capture on low to high transition.
//!                   true  (1) = Capture on high to low transition.
//!
//! Use this function to start capturing.
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_capture_start(uint32_t ui32CaptureNum,
                            uint32_t ui32GPIONumber,
                            bool bPolarity)
{
    uint32_t ui32CapCtrl;

    if ( ui32GPIONumber > (PAD_COUNT - 1) ) {
        
        return;
        
    }

    //
    // Set the polarity and pin selection in the GPIO block.
    //
    switch (ui32CaptureNum) {
        
         case 0:
            REG_BFW(GPIO, STMRCAP, STPOL0, bPolarity);
            REG_BFW(GPIO, STMRCAP, STSEL0, ui32GPIONumber);
            ui32CapCtrl = REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_M;
            break;
            
         case 1:
            REG_BFW(GPIO, STMRCAP, STPOL1, bPolarity);
            REG_BFW(GPIO, STMRCAP, STSEL1, ui32GPIONumber);
            ui32CapCtrl = REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_M;
            break;
            
         case 2:
            REG_BFW(GPIO, STMRCAP, STPOL2, bPolarity);
            REG_BFW(GPIO, STMRCAP, STSEL2, ui32GPIONumber);
            ui32CapCtrl = REG_STIMER_CAPTURE_CONTROL_CAPTURE_C_M;
            break;
            
         case 3:
            REG_BFW(GPIO, STMRCAP, STPOL3, bPolarity);
            REG_BFW(GPIO, STMRCAP, STSEL3, ui32GPIONumber);
            ui32CapCtrl = REG_STIMER_CAPTURE_CONTROL_CAPTURE_D_M;
            break;
            
         default:
            return;     // error concealment.
            
    }

    //
    // Enable it in the STIMER Block
    //
    REG_STIMER.CAPTURE_CONTROL |= ui32CapCtrl;
}

//*****************************************************************************
//
//! @brief Start capturing data with the specified capture register.
//!
//! @param ui32CaptureNum is the Capture Register Number to read.
//!
//! Use this function to start capturing.
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_capture_stop(uint32_t ui32CaptureNum)
{
    //
    // Disable it in the STIMER block.
    //
    REG_STIMER.CAPTURE_CONTROL &=
        ~(REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_M <<
            ((REG_STIMER_CAPTURE_CONTROL_CAPTURE_B_S -
              REG_STIMER_CAPTURE_CONTROL_CAPTURE_A_S) * ui32CaptureNum));
}

//*****************************************************************************
//
//! @brief Get the current stimer capture register value.
//!
//! @param ui32CaptureNum is the Capture Register Number to read.
//!
//! This function can be used to read the value in an stimer capture register.
//!
//!
//! @return None.
//
//*****************************************************************************
uint32_t driver_stimer_capture_get(uint32_t ui32CaptureNum)
{
    if ( ui32CaptureNum > 7 ) {
        
        return 0;
        
    }

    return REG_STIMER.SCAPT[ui32CaptureNum];
}

//*****************************************************************************
//
//! @brief Enables the selected system timer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will enable the selected interrupts in the STIMER interrupt
//! enable register. In order to receive an interrupt from an stimer component,
//! you will need to enable the interrupt for that component in this main
//! register, as well as in the stimer configuration register (accessible though
//! driver_stimer_config()), and in the NVIC.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     DRIVER_STIMER_INT_COMPAREA
//!     DRIVER_STIMER_INT_COMPAREB
//!     DRIVER_STIMER_INT_COMPAREC
//!     DRIVER_STIMER_INT_COMPARED
//!     DRIVER_STIMER_INT_COMPAREE
//!     DRIVER_STIMER_INT_COMPAREF
//!     DRIVER_STIMER_INT_COMPAREG
//!     DRIVER_STIMER_INT_COMPAREH
//!
//!     DRIVER_STIMER_INT_OVERFLOW
//!
//!     DRIVER_STIMER_INT_CAPTUREA
//!     DRIVER_STIMER_INT_CAPTUREB
//!     DRIVER_STIMER_INT_CAPTUREC
//!     DRIVER_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_int_enable(uint32_t ui32Interrupt)
{
    //
    // Enable the interrupt at the module level.
    //
    REG_STIMER.STMINTEN |= ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Return the enabled stimer interrupts.
//!
//! This function will return all enabled interrupts in the STIMER
//! interrupt enable register.
//!
//! @return return enabled interrupts. This will be a logical or of:
//!
//!     DRIVER_STIMER_INT_COMPAREA
//!     DRIVER_STIMER_INT_COMPAREB
//!     DRIVER_STIMER_INT_COMPAREC
//!     DRIVER_STIMER_INT_COMPARED
//!     DRIVER_STIMER_INT_COMPAREE
//!     DRIVER_STIMER_INT_COMPAREF
//!     DRIVER_STIMER_INT_COMPAREG
//!     DRIVER_STIMER_INT_COMPAREH
//!
//!     DRIVER_STIMER_INT_OVERFLOW
//!
//!     DRIVER_STIMER_INT_CAPTUREA
//!     DRIVER_STIMER_INT_CAPTUREB
//!     DRIVER_STIMER_INT_CAPTUREC
//!     DRIVER_STIMER_INT_CAPTURED
//!
//! @return Return the enabled timer interrupts.
//
//*****************************************************************************
uint32_t driver_stimer_int_enable_get(void)
{
    //
    // Return enabled interrupts.
    //
    return REG_STIMER.STMINTEN;
}

//*****************************************************************************
//
//! @brief Disables the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will disable the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     DRIVER_STIMER_INT_COMPAREA
//!     DRIVER_STIMER_INT_COMPAREB
//!     DRIVER_STIMER_INT_COMPAREC
//!     DRIVER_STIMER_INT_COMPARED
//!     DRIVER_STIMER_INT_COMPAREE
//!     DRIVER_STIMER_INT_COMPAREF
//!     DRIVER_STIMER_INT_COMPAREG
//!     DRIVER_STIMER_INT_COMPAREH
//!
//!     DRIVER_STIMER_INT_OVERFLOW
//!
//!     DRIVER_STIMER_INT_CAPTUREA
//!     DRIVER_STIMER_INT_CAPTUREB
//!     DRIVER_STIMER_INT_CAPTUREC
//!     DRIVER_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_int_disable(uint32_t ui32Interrupt)
{
    //
    // Disable the interrupt at the module level.
    //
    REG_STIMER.STMINTEN &= ~ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Sets the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will set the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     DRIVER_STIMER_INT_COMPAREA
//!     DRIVER_STIMER_INT_COMPAREB
//!     DRIVER_STIMER_INT_COMPAREC
//!     DRIVER_STIMER_INT_COMPARED
//!     DRIVER_STIMER_INT_COMPAREE
//!     DRIVER_STIMER_INT_COMPAREF
//!     DRIVER_STIMER_INT_COMPAREG
//!     DRIVER_STIMER_INT_COMPAREH
//!
//!     DRIVER_STIMER_INT_OVERFLOW
//!
//!     DRIVER_STIMER_INT_CAPTUREA
//!     DRIVER_STIMER_INT_CAPTUREB
//!     DRIVER_STIMER_INT_CAPTUREC
//!     DRIVER_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_int_set(uint32_t ui32Interrupt)
{
    //
    // Set the interrupts.
    //
    REG_STIMER.STMINTSET = ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Clears the selected stimer interrupt.
//!
//! @param ui32Interrupt is the interrupt to be used.
//!
//! This function will clear the selected interrupts in the STIMER
//! interrupt register.
//!
//! ui32Interrupt should be the logical OR of one or more of the following
//! values:
//!
//!     DRIVER_STIMER_INT_COMPAREA
//!     DRIVER_STIMER_INT_COMPAREB
//!     DRIVER_STIMER_INT_COMPAREC
//!     DRIVER_STIMER_INT_COMPARED
//!     DRIVER_STIMER_INT_COMPAREE
//!     DRIVER_STIMER_INT_COMPAREF
//!     DRIVER_STIMER_INT_COMPAREG
//!     DRIVER_STIMER_INT_COMPAREH
//!
//!     DRIVER_STIMER_INT_OVERFLOW
//!
//!     DRIVER_STIMER_INT_CAPTUREA
//!     DRIVER_STIMER_INT_CAPTUREB
//!     DRIVER_STIMER_INT_CAPTUREC
//!     DRIVER_STIMER_INT_CAPTURED
//!
//! @return None.
//
//*****************************************************************************
void driver_stimer_int_clear(uint32_t ui32Interrupt)
{
    //
    // Disable the interrupt at the module level.
    //
    REG_STIMER.STMINTCLR = ui32Interrupt;
}


//*****************************************************************************
//
//! @brief Returns either the enabled or raw stimer interrupt status.
//!
//! This function will return the stimer interrupt status.
//!
//! @bEnabledOnly if true returns the status of the enabled interrupts
//! only.
//!
//! The return value will be the logical OR of one or more of the following
//! values:
//!
//!
//! @return Returns the stimer interrupt status.
//
//*****************************************************************************
uint32_t driver_stimer_int_status_get(bool bEnabledOnly)
{
    //
    // Return the desired status.
    //
    uint32_t ui32RetVal = REG_STIMER.STMINTSTAT;

    if ( bEnabledOnly ) {
        
        ui32RetVal &= REG_STIMER.STMINTEN;
        
    }

    return ui32RetVal;
}

/* EOF */
