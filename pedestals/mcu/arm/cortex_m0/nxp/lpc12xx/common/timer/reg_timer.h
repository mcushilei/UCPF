
/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
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

#ifndef __I_IO_TIMER_H__
#define __I_IO_TIMER_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/

//! \note IR Register Macro
//! @{
#define TIMER0_IR_MR0INT                (0)
#define TIMER0_IR_MR0INT_MSK            (1ul << TIMER0_IR_MR0INT)

#define TIMER0_IR_MR1INT                (1)
#define TIMER0_IR_MR1INT_MSK            (1ul << TIMER0_IR_MR1INT)

#define TIMER0_IR_MR2INT                (2)
#define TIMER0_IR_MR2INT_MSK            (1ul << TIMER0_IR_MR2INT)

#define TIMER0_IR_MR3INT                (3)
#define TIMER0_IR_MR3INT_MSK            (1ul << TIMER0_IR_MR3INT)

#define TIMER0_IR_CR0INT                (4)
#define TIMER0_IR_CR0INT_MSK            (1ul << TIMER0_IR_CR0INT)

#define TIMER_TCR_CEN                   (0)
#define TIMER_TCR_CEN_MSK               (1ul << TIMER_TCR_CEN)

#define TIMER_TCR_CRST                  (1)
#define TIMER_TCR_CRST_MSK              (1ul << TIMER_TCR_CRST)
//! @}

//! \note MCR Register Macro
//! @{
#define MCR_MR0I                        (0) 
#define MCR_MR0I_MSK                    (1ul << MCR_MR0I) 
#define MCR_MR0R                        (1) 
#define MCR_MR0R_MSK                    (1ul << MCR_MR0R) 
#define MCR_MR0S                        (2) 
#define MCR_MR0S_MSK                    (1ul << MCR_MR0S) 
#define MCR_MR1I                        (3) 
#define MCR_MR1I_MSK                    (1ul << MCR_MR1I) 
#define MCR_MR1R                        (4) 
#define MCR_MR1R_MSK                    (1ul << MCR_MR1R) 
#define MCR_MR1S                        (5) 
#define MCR_MR1S_MSK                    (1ul << MCR_MR1S) 
#define MCR_MR2I                        (6) 
#define MCR_MR2I_MSK                    (1ul << MCR_MR2I) 
#define MCR_MR2R                        (7) 
#define MCR_MR2R_MSK                    (1ul << MCR_MR2R) 
#define MCR_MR2S                        (8) 
#define MCR_MR2S_MSK                    (1ul << MCR_MR2S) 
#define MCR_MR3I                        (9) 
#define MCR_MR3I_MSK                    (1ul << MCR_MR3I) 
#define MCR_MR3R                        (10) 
#define MCR_MR3R_MSK                    (1ul << MCR_MR3R) 
#define MCR_MR3S                        (11) 
#define MCR_MR3S_MSK                    (1ul << MCR_MR3S) 
//! @}

//! \note EMR Register Macro
//! @{
#define EMR_EM0                         (0)
#define EMR_EM0_MSK                     (1ul << EMR_EM0) 
#define EMR_EM1                         (1)
#define EMR_EM1_MSK                     (1ul << EMR_EM1) 
#define EMR_EM2                          (2)
#define EMR_EM2_MSK                     (1ul << EMR_EM2) 
#define EMR_EM3                          (3)
#define EMR_EM3_MSK                     (1ul << EMR_EM3) 

#define EMR_EMC0_0                      (4)
#define EMR_EMC0_1                      (5)
#define EMR_EMC0_MSK                    ((1ul << EMR_EMC0_0)    |\
                                         (1ul << EMR_EMC0_1))
#define EMR_EMC0_SET(__VALUE)           (((__VALUE) << EMR_EMC3_0) & EMR_EMC0_MSK)
#define EMR_EMC1_0                      (6)
#define EMR_EMC1_1                      (7)
#define EMR_EMC1_MSK                    ((1ul << EMR_EMC1_0)    |\
                                         (1ul << EMR_EMC1_1))
#define EMR_EMC1_SET(__VALUE)           (((__VALUE) << EMR_EMC1_0) & EMR_EMC1_MSK)
#define EMR_EMC2_0                      (8)
#define EMR_EMC2_1                      (9)
#define EMR_EMC2_MSK                    ((1ul << EMR_EMC2_0)    |\
                                         (1ul << EMR_EMC2_1))
#define EMR_EMC2_SET(__VALUE)           (((__VALUE) << EMR_EMC2_0) & EMR_EMC2_MSK)
#define EMR_EMC3_0                      (10)
#define EMR_EMC3_1                      (11)
#define EMR_EMC3_MSK                    ((1ul << EMR_EMC3_0)    |\
                                         (1ul << EMR_EMC3_1))
#define EMR_EMC3_SET(__VALUE)           (((__VALUE) << EMR_EMC3_0) & EMR_EMC3_MSK)
//! @}

//! \note PWPMC Register Macro
//! @{
#define PWMC_PWMEN0                     (0)
#define PWMC_PWMEN0_MSK                 (1ul << PWMC_PWMEN0)
#define PWMC_PWMEN1                     (1)
#define PWMC_PWMEN1_MSK                 (1ul << PWMC_PWMEN1)
#define PWMC_PWMEN2                     (2)
#define PWMC_PWMEN2_MSK                 (1ul << PWMC_PWMEN2)
#define PWMC_PWMEN3                     (3)
#define PWMC_PWMEN3_MSK                 (1ul << PWMC_PWMEN3)
//! @}

//! \note CCR Register MASK
//! @{
#define CCR_CAP0RE                      (0ul)
#define CCR_CAP0RE_MSK                  (1ul << CCR_CAP0RE)
#define CCR_CAP0FE                      (1ul)
#define CCR_CAP0FE_MSK                  (1ul << CCR_CAP0FE)
#define CCR_CAP0I                       (2ul)
#define CCR_CAP0I_MSK                   (1ul << CCR_CAP0I)
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/     
/*============================ TYPES =========================================*/

//! \brief system control block register page
//! @{
typedef volatile struct {
    DEF_REG32
        reg32_t MR0INT          :1;     //!< Interrupt flag for match channel 0. 
        reg32_t MR1INT          :1;     //!< Interrupt flag for match channel 1. 
        reg32_t MR2INT          :1;     //!< Interrupt flag for match channel 2. 
        reg32_t MR3INT          :1;     //!< Interrupt flag for match channel 3. 
        reg32_t CR0INT          :1;     //!< Interrupt flag for capture channel 0 event. 
        reg32_t                 :1;     //!< Reserved
        reg32_t                 :1;     //!< Reserved
        reg32_t                 :25;    //!< Reserved
    END_DEF_REG32(IR)                     //!< R/W 0x000 

    DEF_REG32
        reg32_t CEN             :1;     //!< Counter enable.
        reg32_t CRST            :1;     //!< Counter reset. 
        reg32_t                 :30;    //!< Reserved 
    END_DEF_REG32(TCR)                    //!< R/W 0x004 

    reg32_t TC;                         //!< R/W 0x008 Timer Counter
    reg32_t PR;                         //!< R/W 0x00C Prescale Register
    reg32_t PC;                         //!< R/W 0x010 Perscale Counter

    DEF_REG32
        reg32_t MR0I            :1;     //!< Interrupt on MR0
        reg32_t MR0R            :1;     //!< Reset on MR0
        reg32_t MR0S            :1;     //!< Stop on MR0
        reg32_t MR1I            :1;     //!< Interrupt on MR1
        reg32_t MR1R            :1;     //!< Reset on MR1
        reg32_t MR1S            :1;     //!< Stop on MR1
        reg32_t MR2I            :1;     //!< Interrupt on MR2
        reg32_t MR2R            :1;     //!< Reset on MR2
        reg32_t MR2S            :1;     //!< Stop on MR2
        reg32_t MR3I            :1;     //!< Interrupt on MR3
        reg32_t MR3R            :1;     //!< Reset on MR3
        reg32_t MR3S            :1;     //!< Stop on MR3
        reg32_t                 :20;    //!< Reserved
    END_DEF_REG32(MCR)                    //!< R/W 0x014 

    reg32_t MATCH[4];                   //!< Timer counter match value   

    DEF_REG32
        reg32_t CAP0RE          :1;     //!< Capture on CT16Bn_CAP0 rising edge
        reg32_t CAP0FE          :1;     //!< Capture on CT16Bn_CAP0 falling edge
        reg32_t CAP0I           :1;     //!< Interrupt on CT16Bn_CAP0 event
        reg32_t                 :29;    //!< Reserved
    END_DEF_REG32(CCR)                    //!< R/W 0x028 

    reg32_t CR0;                        //!< RO 0x02C Timer counter capture value  

    reg32_t                     :32;    //!< Reserved

    reg32_t                     :32;    //!< Reserved

    reg32_t                     :32;    //!< Reserved

    DEF_REG32
        reg32_t EM0             :1;     //!< External Match 0
        reg32_t EM1             :1;     //!< External Match 1
        reg32_t EM2             :1;     //!< External Match 2
        reg32_t EM3             :1;     //!< External Match 3
        reg32_t EMC0            :2;     //!< External Match Control 0
        reg32_t EMC1            :2;     //!< External Match Control 1
        reg32_t EMC2            :2;     //!< External Match Control 2
        reg32_t EMC3            :2;     //!< External Match Control 3
        reg32_t                 :20;    //!< Reserved
    END_DEF_REG32(EMR)                    //!< R/W 0x03C 


    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved

    DEF_REG32
        reg32_t CTM             :2;     //!< Counter/Timer Mode
        reg32_t CIS             :2;     //!< Count Input Select
        reg32_t ENCC            :1;     //!< Setting this bit to 1 enables clearing of the timer 
        reg32_t SELCC           :3;     //!< Edge select
        reg32_t                 :24;    //!< Reserved
    END_DEF_REG32(CTCR)                   //!< R/W 0x070 

    DEF_REG32
        reg32_t PWMEN0          :1;     //!< PWM channel0 enable 0
        reg32_t PWMEN1          :1;     //!< PWM channel1 enable 1
        reg32_t PWMEN2          :1;     //!< PWM channel2 enable 2
        reg32_t PWMEN3          :1;     //!< PWM channel3 enable 3
        reg32_t                 :28;    //!< Reserved
    END_DEF_REG32(PWMC)                   //!< R/W 0x074 
}tmr_reg_t;
//! @}

//! \brief system control block register page
//! @{
typedef volatile struct {
    DEF_REG32
        reg32_t CMPAF           :1;     //!< Interrupt flag for match channel 0. 
        reg32_t CMPBF           :1;     //!< Interrupt flag for match channel 1. 
        reg32_t CMPCF           :1;     //!< Interrupt flag for match channel 2. 
        reg32_t OVF             :1;     //!< Interrupt flag for match channel 3. 
        reg32_t CAPF            :1;     //!< Interrupt flag for capture channel 0 event. 
        reg32_t                 :1;     //!< Reserved
        reg32_t                 :1;     //!< Reserved
        reg32_t                 :25;    //!< Reserved
    END_DEF_REG32(STATUS)                 //!< R/W 0x000 

    reg32_t                     :32;    //!< Reserved

    reg32_t COUNTER;                    //!< R/W 0x008 Timer Counter

    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved

    reg32_t COMPAREA;                   //!< Timer counter match value   
    reg32_t COMPAREB;                   //!< Timer counter match value   
    reg32_t COMPAREC;                   //!< Timer counter match value   
    reg32_t TOP;                        //!< Timer counter top value   

    reg32_t                     :32;    //!< Reserved

    reg32_t CAPTURE;                    //!< RO 0x02C Timer counter capture value  

    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved
    reg32_t                     :32;    //!< Reserved 
    reg32_t                     :32;    //!< Reserved
}timer_reg_t;
//! @}
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif
