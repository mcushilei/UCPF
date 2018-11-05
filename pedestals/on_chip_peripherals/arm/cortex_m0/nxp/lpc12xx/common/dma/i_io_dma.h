#ifndef __I_IO_SDMA_H__
#define __I_IO_SDMA_H__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
//! \name DMA CONTRL register
//! @{
#define DMA_CONTRL_ENABLE                               0
#define DMA_CONTRL_ENABLE_MSK                          (1ul << DMA_CONTRL_ENABLE)
//! @}

//! \name DMA INTSTAT register
//! @{
#define DMA_INTSTAT_ACTIVEINT                           1
#define DMA_INTSTAT_ACTIVEINT_MSK                      (1ul << DMA_INTSTAT_ACTIVEINT)

#define DMA_INTSTAT_ACTIVEERRINT                        2
#define DMA_INTSTAT_ACTIVEERRINT_MSK                   (1ul << DMA_INTSTAT_ACTIVEERRINT)
//! @}

//! \name DMA SRAMBASE register
//! @{
#define DMA_SRAMBASE_OFFSET0                            10
#define DMA_SRAMBASE_OFFSET1                            11
#define DMA_SRAMBASE_OFFSET2                            12
#define DMA_SRAMBASE_OFFSET3                            13
#define DMA_SRAMBASE_OFFSET4                            14
#define DMA_SRAMBASE_OFFSET5                            15
#define DMA_SRAMBASE_OFFSET6                            16
#define DMA_SRAMBASE_OFFSET7                            17
#define DMA_SRAMBASE_OFFSET8                            18
#define DMA_SRAMBASE_OFFSET9                            19
#define DMA_SRAMBASE_OFFSET10                           20
#define DMA_SRAMBASE_OFFSET11                           21
#define DMA_SRAMBASE_OFFSET12                           22
#define DMA_SRAMBASE_OFFSET13                           23
#define DMA_SRAMBASE_OFFSET14                           24
#define DMA_SRAMBASE_OFFSET15                           25
#define DMA_SRAMBASE_OFFSET16                           26
#define DMA_SRAMBASE_OFFSET17                           27
#define DMA_SRAMBASE_OFFSET18                           28
#define DMA_SRAMBASE_OFFSET19                           29
#define DMA_SRAMBASE_OFFSET20                           30
#define DMA_SRAMBASE_OFFSET21                           31
#define DMA_SRAMBASE_OFFSET_MSK                        (   (1ul << DMA_SRAMBASE_OFFSET0)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET1)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET2)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET3)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET4)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET5)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET6)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET7)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET8)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET9)       |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET10)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET11)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET12)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET13)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET14)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET15)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET16)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET17)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET18)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET19)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET20)      |   \
                                                            (1ul << DMA_SRAMBASE_OFFSET21)          \
                                                        )
#define DMA_SRAMBASE_OFFSET(__SET)                      ((__SET) << DMA_SRAMBASE_OFFSET0)
//! @}


//! \name DMA ENABLESET register
//! @{
#define DMA_ENABLESET_ENA0                              0 
#define DMA_ENABLESET_ENA1                              1 
#define DMA_ENABLESET_ENA2                              2 
#define DMA_ENABLESET_ENA3                              3 
#define DMA_ENABLESET_ENA4                              4 
#define DMA_ENABLESET_ENA5                              5 
#define DMA_ENABLESET_ENA6                              6 
#define DMA_ENABLESET_ENA7                              7 
#define DMA_ENABLESET_ENA8                              8 
#define DMA_ENABLESET_ENA9                              9 
#define DMA_ENABLESET_ENA10                             10
#define DMA_ENABLESET_ENA11                             11
#define DMA_ENABLESET_ENA12                             12
#define DMA_ENABLESET_ENA13                             13
#define DMA_ENABLESET_ENA14                             14
#define DMA_ENABLESET_ENA15                             15
#define DMA_ENABLESET_ENA_MSK                          (   (1ul << DMA_ENABLESET_ENA0)       |   \
                                                            (1ul << DMA_ENABLESET_ENA1)       |   \
                                                            (1ul << DMA_ENABLESET_ENA2)       |   \
                                                            (1ul << DMA_ENABLESET_ENA3)       |   \
                                                            (1ul << DMA_ENABLESET_ENA4)       |   \
                                                            (1ul << DMA_ENABLESET_ENA5)       |   \
                                                            (1ul << DMA_ENABLESET_ENA6)       |   \
                                                            (1ul << DMA_ENABLESET_ENA7)       |   \
                                                            (1ul << DMA_ENABLESET_ENA8)       |   \
                                                            (1ul << DMA_ENABLESET_ENA9)       |   \
                                                            (1ul << DMA_ENABLESET_ENA10)      |   \
                                                            (1ul << DMA_ENABLESET_ENA11)      |   \
                                                            (1ul << DMA_ENABLESET_ENA12)      |   \
                                                            (1ul << DMA_ENABLESET_ENA13)      |   \
                                                            (1ul << DMA_ENABLESET_ENA14)      |   \
                                                            (1ul << DMA_ENABLESET_ENA15)          \
                                                        )
#define DMA_ENABLESET_ENA(__SET)                        ((__SET) << DMA_ENABLESET_ENA0)
//! @}

//! \name DMA ENABLECLR register
//! @{
#define DMA_ENABLECLR_CLR0                              0 
#define DMA_ENABLECLR_CLR1                              1 
#define DMA_ENABLECLR_CLR2                              2 
#define DMA_ENABLECLR_CLR3                              3 
#define DMA_ENABLECLR_CLR4                              4 
#define DMA_ENABLECLR_CLR5                              5 
#define DMA_ENABLECLR_CLR6                              6 
#define DMA_ENABLECLR_CLR7                              7 
#define DMA_ENABLECLR_CLR8                              8 
#define DMA_ENABLECLR_CLR9                              9 
#define DMA_ENABLECLR_CLR10                             10
#define DMA_ENABLECLR_CLR11                             11
#define DMA_ENABLECLR_CLR12                             12
#define DMA_ENABLECLR_CLR13                             13
#define DMA_ENABLECLR_CLR14                             14
#define DMA_ENABLECLR_CLR15                             15
#define DMA_ENABLECLR_CLR_MSK                          (   (1ul << DMA_ENABLECLR_CLR0)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR1)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR2)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR3)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR4)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR5)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR6)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR7)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR8)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR9)       |   \
                                                            (1ul << DMA_ENABLECLR_CLR10)      |   \
                                                            (1ul << DMA_ENABLECLR_CLR11)      |   \
                                                            (1ul << DMA_ENABLECLR_CLR12)      |   \
                                                            (1ul << DMA_ENABLECLR_CLR13)      |   \
                                                            (1ul << DMA_ENABLECLR_CLR14)      |   \
                                                            (1ul << DMA_ENABLECLR_CLR15)          \
                                                        )
#define DMA_ENABLECLR_CLR(__SET)                        ((__SET) << DMA_ENABLECLR_CLR0)
//! @}

//! \name DMA ACTIVE register
//! @{
#define DMA_ACTIVE_ACT0                                 0 
#define DMA_ACTIVE_ACT1                                 1 
#define DMA_ACTIVE_ACT2                                 2 
#define DMA_ACTIVE_ACT3                                 3 
#define DMA_ACTIVE_ACT4                                 4 
#define DMA_ACTIVE_ACT5                                 5 
#define DMA_ACTIVE_ACT6                                 6 
#define DMA_ACTIVE_ACT7                                 7 
#define DMA_ACTIVE_ACT8                                 8 
#define DMA_ACTIVE_ACT9                                 9 
#define DMA_ACTIVE_ACT10                                10
#define DMA_ACTIVE_ACT11                                11
#define DMA_ACTIVE_ACT12                                12
#define DMA_ACTIVE_ACT13                                13
#define DMA_ACTIVE_ACT14                                14
#define DMA_ACTIVE_ACT15                                15
#define DMA_ACTIVE_ACT_MSK                             (   (1ul << DMA_ACTIVE_ACT0)       |   \
                                                            (1ul << DMA_ACTIVE_ACT1)       |   \
                                                            (1ul << DMA_ACTIVE_ACT2)       |   \
                                                            (1ul << DMA_ACTIVE_ACT3)       |   \
                                                            (1ul << DMA_ACTIVE_ACT4)       |   \
                                                            (1ul << DMA_ACTIVE_ACT5)       |   \
                                                            (1ul << DMA_ACTIVE_ACT6)       |   \
                                                            (1ul << DMA_ACTIVE_ACT7)       |   \
                                                            (1ul << DMA_ACTIVE_ACT8)       |   \
                                                            (1ul << DMA_ACTIVE_ACT9)       |   \
                                                            (1ul << DMA_ACTIVE_ACT10)      |   \
                                                            (1ul << DMA_ACTIVE_ACT11)      |   \
                                                            (1ul << DMA_ACTIVE_ACT12)      |   \
                                                            (1ul << DMA_ACTIVE_ACT13)      |   \
                                                            (1ul << DMA_ACTIVE_ACT14)      |   \
                                                            (1ul << DMA_ACTIVE_ACT15)          \
                                                        )
#define DMA_ACTIVE_ACT(__SET)                           ((__SET) << DMA_ACTIVE_ACT0)
//! @}

//! \name DMA BUSY register
//! @{
#define DMA_BUSY_BSY0                                   0 
#define DMA_BUSY_BSY1                                   1 
#define DMA_BUSY_BSY2                                   2 
#define DMA_BUSY_BSY3                                   3 
#define DMA_BUSY_BSY4                                   4 
#define DMA_BUSY_BSY5                                   5 
#define DMA_BUSY_BSY6                                   6 
#define DMA_BUSY_BSY7                                   7 
#define DMA_BUSY_BSY8                                   8 
#define DMA_BUSY_BSY9                                   9 
#define DMA_BUSY_BSY10                                  10
#define DMA_BUSY_BSY11                                  11
#define DMA_BUSY_BSY12                                  12
#define DMA_BUSY_BSY13                                  13
#define DMA_BUSY_BSY14                                  14
#define DMA_BUSY_BSY15                                  15
#define DMA_BUSY_BSY_MSK                               (   (1ul << DMA_BUSY_BSY0)       |   \
                                                            (1ul << DMA_BUSY_BSY1)       |   \
                                                            (1ul << DMA_BUSY_BSY2)       |   \
                                                            (1ul << DMA_BUSY_BSY3)       |   \
                                                            (1ul << DMA_BUSY_BSY4)       |   \
                                                            (1ul << DMA_BUSY_BSY5)       |   \
                                                            (1ul << DMA_BUSY_BSY6)       |   \
                                                            (1ul << DMA_BUSY_BSY7)       |   \
                                                            (1ul << DMA_BUSY_BSY8)       |   \
                                                            (1ul << DMA_BUSY_BSY9)       |   \
                                                            (1ul << DMA_BUSY_BSY10)      |   \
                                                            (1ul << DMA_BUSY_BSY11)      |   \
                                                            (1ul << DMA_BUSY_BSY12)      |   \
                                                            (1ul << DMA_BUSY_BSY13)      |   \
                                                            (1ul << DMA_BUSY_BSY14)      |   \
                                                            (1ul << DMA_BUSY_BSY15)          \
                                                        )
#define DMA_BUSY_BSY(__SET)                             ((__SET) << DMA_BUSY_BSY0)
//! @}

//! \name DMA ERRINT register
//! @{
#define DMA_ERRINT_ERR0                                 0 
#define DMA_ERRINT_ERR1                                 1 
#define DMA_ERRINT_ERR2                                 2 
#define DMA_ERRINT_ERR3                                 3 
#define DMA_ERRINT_ERR4                                 4 
#define DMA_ERRINT_ERR5                                 5 
#define DMA_ERRINT_ERR6                                 6 
#define DMA_ERRINT_ERR7                                 7 
#define DMA_ERRINT_ERR8                                 8 
#define DMA_ERRINT_ERR9                                 9 
#define DMA_ERRINT_ERR10                                10
#define DMA_ERRINT_ERR11                                11
#define DMA_ERRINT_ERR12                                12
#define DMA_ERRINT_ERR13                                13
#define DMA_ERRINT_ERR14                                14
#define DMA_ERRINT_ERR15                                15
#define DMA_ERRINT_ERR_MSK                             (   (1ul << DMA_ERRINT_ERR0)       |   \
                                                            (1ul << DMA_ERRINT_ERR1)       |   \
                                                            (1ul << DMA_ERRINT_ERR2)       |   \
                                                            (1ul << DMA_ERRINT_ERR3)       |   \
                                                            (1ul << DMA_ERRINT_ERR4)       |   \
                                                            (1ul << DMA_ERRINT_ERR5)       |   \
                                                            (1ul << DMA_ERRINT_ERR6)       |   \
                                                            (1ul << DMA_ERRINT_ERR7)       |   \
                                                            (1ul << DMA_ERRINT_ERR8)       |   \
                                                            (1ul << DMA_ERRINT_ERR9)       |   \
                                                            (1ul << DMA_ERRINT_ERR10)      |   \
                                                            (1ul << DMA_ERRINT_ERR11)      |   \
                                                            (1ul << DMA_ERRINT_ERR12)      |   \
                                                            (1ul << DMA_ERRINT_ERR13)      |   \
                                                            (1ul << DMA_ERRINT_ERR14)      |   \
                                                            (1ul << DMA_ERRINT_ERR15)          \
                                                        )       
#define DMA_ERRINT_ERR(__SET)                           ((__SET) << DMA_ERRINT_ERR0)
//! @}


//! \name DMA INTENSET register
//! @{
#define DMA_INTENSET_INTEN0                              0 
#define DMA_INTENSET_INTEN1                              1 
#define DMA_INTENSET_INTEN2                              2 
#define DMA_INTENSET_INTEN3                              3 
#define DMA_INTENSET_INTEN4                              4 
#define DMA_INTENSET_INTEN5                              5 
#define DMA_INTENSET_INTEN6                              6 
#define DMA_INTENSET_INTEN7                              7 
#define DMA_INTENSET_INTEN8                              8 
#define DMA_INTENSET_INTEN9                              9 
#define DMA_INTENSET_INTEN10                             10
#define DMA_INTENSET_INTEN11                             11
#define DMA_INTENSET_INTEN12                             12
#define DMA_INTENSET_INTEN13                             13
#define DMA_INTENSET_INTEN14                             14
#define DMA_INTENSET_INTEN15                             15
#define DMA_INTENSET_INTEN_MSK                          (   (1ul << DMA_INTENSET_INTEN0)       |   \
                                                             (1ul << DMA_INTENSET_INTEN1)       |   \
                                                             (1ul << DMA_INTENSET_INTEN2)       |   \
                                                             (1ul << DMA_INTENSET_INTEN3)       |   \
                                                             (1ul << DMA_INTENSET_INTEN4)       |   \
                                                             (1ul << DMA_INTENSET_INTEN5)       |   \
                                                             (1ul << DMA_INTENSET_INTEN6)       |   \
                                                             (1ul << DMA_INTENSET_INTEN7)       |   \
                                                             (1ul << DMA_INTENSET_INTEN8)       |   \
                                                             (1ul << DMA_INTENSET_INTEN9)       |   \
                                                             (1ul << DMA_INTENSET_INTEN10)      |   \
                                                            (1ul << DMA_INTENSET_INTEN11)      |   \
                                                            (1ul << DMA_INTENSET_INTEN12)      |   \
                                                            (1ul << DMA_INTENSET_INTEN13)      |   \
                                                            (1ul << DMA_INTENSET_INTEN14)      |   \
                                                            (1ul << DMA_INTENSET_INTEN15)          \
                                                        )
#define DMA_INTENSET_INTEN(__SET)                       ((__SET) << DMA_INTENSET_INTEN0)
//! @}

//! \name DMA INTENCLR register
//! @{
#define DMA_INTENCLR_CLR0                               0        
#define DMA_INTENCLR_CLR1                               1        
#define DMA_INTENCLR_CLR2                               2        
#define DMA_INTENCLR_CLR3                               3        
#define DMA_INTENCLR_CLR4                               4        
#define DMA_INTENCLR_CLR5                               5        
#define DMA_INTENCLR_CLR6                               6        
#define DMA_INTENCLR_CLR7                               7        
#define DMA_INTENCLR_CLR8                               8        
#define DMA_INTENCLR_CLR9                               9        
#define DMA_INTENCLR_CLR10                              10       
#define DMA_INTENCLR_CLR11                              11       
#define DMA_INTENCLR_CLR12                              12       
#define DMA_INTENCLR_CLR13                              13       
#define DMA_INTENCLR_CLR14                              14       
#define DMA_INTENCLR_CLR15                              15       
#define DMA_INTENCLR_CLR_MSK                           (          (1ul << DMA_INTENCLR_CLR0)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR1)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR2)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR3)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR4)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR5)       |   \
                                                                   (1ul << DMA_INTENCLR_CLR6)       |   \
                                                            (1ul << DMA_INTENCLR_CLR7)       |   \
                                                            (1ul << DMA_INTENCLR_CLR8)       |   \
                                                            (1ul << DMA_INTENCLR_CLR9)       |   \
                                                            (1ul << DMA_INTENCLR_CLR10)      |   \
                                                            (1ul << DMA_INTENCLR_CLR11)      |   \
                                                            (1ul << DMA_INTENCLR_CLR12)      |   \
                                                            (1ul << DMA_INTENCLR_CLR13)      |   \
                                                            (1ul << DMA_INTENCLR_CLR14)      |   \
                                                            (1ul << DMA_INTENCLR_CLR15)          \
                                                        )
#define DMA_INTENCLR_CLR(__SET)                         ((__SET) << DMA_INTENCLR_CLR0)
//! @}

//! \name DMA INTA register
//! @{
#define DMA_INTA_IA0                                    0 
#define DMA_INTA_IA1                                    1 
#define DMA_INTA_IA2                                    2 
#define DMA_INTA_IA3                                    3 
#define DMA_INTA_IA4                                    4 
#define DMA_INTA_IA5                                    5 
#define DMA_INTA_IA6                                    6 
#define DMA_INTA_IA7                                    7 
#define DMA_INTA_IA8                                    8 
#define DMA_INTA_IA9                                    9 
#define DMA_INTA_IA10                                   10
#define DMA_INTA_IA11                                   11
#define DMA_INTA_IA12                                   12
#define DMA_INTA_IA13                                   13
#define DMA_INTA_IA14                                   14
#define DMA_INTA_IA15                                   15
#define DMA_INTA_IA_MSK                                (   (1ul << DMA_INTA_IA0)       |   \
                                                            (1ul << DMA_INTA_IA1)       |   \
                                                            (1ul << DMA_INTA_IA2)       |   \
                                                            (1ul << DMA_INTA_IA3)       |   \
                                                            (1ul << DMA_INTA_IA4)       |   \
                                                            (1ul << DMA_INTA_IA5)       |   \
                                                            (1ul << DMA_INTA_IA6)       |   \
                                                            (1ul << DMA_INTA_IA7)       |   \
                                                            (1ul << DMA_INTA_IA8)       |   \
                                                            (1ul << DMA_INTA_IA9)       |   \
                                                            (1ul << DMA_INTA_IA10)      |   \
                                                            (1ul << DMA_INTA_IA11)      |   \
                                                            (1ul << DMA_INTA_IA12)      |   \
                                                            (1ul << DMA_INTA_IA13)      |   \
                                                            (1ul << DMA_INTA_IA14)      |   \
                                                            (1ul << DMA_INTA_IA15)          \
                                                        )
#define DMA_INTA_IA(__SET)                              ((__SET) << DMA_INTA_IA0)
//! @}

//! \name DMA BUSY register
//! @{
#define DMA_INTB_IB0                                    0 
#define DMA_INTB_IB1                                    1 
#define DMA_INTB_IB2                                    2 
#define DMA_INTB_IB3                                    3 
#define DMA_INTB_IB4                                    4 
#define DMA_INTB_IB5                                    5 
#define DMA_INTB_IB6                                    6 
#define DMA_INTB_IB7                                    7 
#define DMA_INTB_IB8                                    8 
#define DMA_INTB_IB9                                    9 
#define DMA_INTB_IB10                                   10
#define DMA_INTB_IB11                                   11
#define DMA_INTB_IB12                                   12
#define DMA_INTB_IB13                                   13
#define DMA_INTB_IB14                                   14
#define DMA_INTB_IB15                                   15
#define DMA_INTB_IB_MSK                                (   (1ul << DMA_INTB_IB0)       |   \
                                                            (1ul << DMA_INTB_IB1)       |   \
                                                            (1ul << DMA_INTB_IB2)       |   \
                                                            (1ul << DMA_INTB_IB3)       |   \
                                                            (1ul << DMA_INTB_IB4)       |   \
                                                            (1ul << DMA_INTB_IB5)       |   \
                                                            (1ul << DMA_INTB_IB6)       |   \
                                                            (1ul << DMA_INTB_IB7)       |   \
                                                            (1ul << DMA_INTB_IB8)       |   \
                                                            (1ul << DMA_INTB_IB9)       |   \
                                                            (1ul << DMA_INTB_IB10)      |   \
                                                            (1ul << DMA_INTB_IB11)      |   \
                                                            (1ul << DMA_INTB_IB12)      |   \
                                                            (1ul << DMA_INTB_IB13)      |   \
                                                            (1ul << DMA_INTB_IB14)      |   \
                                                            (1ul << DMA_INTB_IB15)          \
                                                        )
#define DMA_INTB_IB(__SET)                              ((__SET) << DMA_INTB_IB0)
//! @}

//! \name DMA SETVALID register
//! @{
#define DMA_SETVALID_SV0                                0 
#define DMA_SETVALID_SV1                                1 
#define DMA_SETVALID_SV2                                2 
#define DMA_SETVALID_SV3                                3 
#define DMA_SETVALID_SV4                                4 
#define DMA_SETVALID_SV5                                5 
#define DMA_SETVALID_SV6                                6 
#define DMA_SETVALID_SV7                                7 
#define DMA_SETVALID_SV8                                8 
#define DMA_SETVALID_SV9                                9 
#define DMA_SETVALID_SV10                               10
#define DMA_SETVALID_SV11                               11
#define DMA_SETVALID_SV12                               12
#define DMA_SETVALID_SV13                               13
#define DMA_SETVALID_SV14                               14
#define DMA_SETVALID_SV15                               15
#define DMA_SETVALID_SV_MSK                            (   (1ul << DMA_SETVALID_SV0)       |   \
                                                            (1ul << DMA_SETVALID_SV1)       |   \
                                                            (1ul << DMA_SETVALID_SV2)       |   \
                                                            (1ul << DMA_SETVALID_SV3)       |   \
                                                            (1ul << DMA_SETVALID_SV4)       |   \
                                                            (1ul << DMA_SETVALID_SV5)       |   \
                                                            (1ul << DMA_SETVALID_SV6)       |   \
                                                            (1ul << DMA_SETVALID_SV7)       |   \
                                                            (1ul << DMA_SETVALID_SV8)       |   \
                                                            (1ul << DMA_SETVALID_SV9)       |   \
                                                            (1ul << DMA_SETVALID_SV10)      |   \
                                                            (1ul << DMA_SETVALID_SV11)      |   \
                                                            (1ul << DMA_SETVALID_SV12)      |   \
                                                            (1ul << DMA_SETVALID_SV13)      |   \
                                                            (1ul << DMA_SETVALID_SV14)      |   \
                                                            (1ul << DMA_SETVALID_SV15)          \
                                                        )
#define DMA_SETVALID_SV(__SET)                          ((__SET) << DMA_SETVALID_SV0)
//! @}

//! \name DMA SETTRIG register
//! @{
#define DMA_SETTRIG_TRIG0                               0 
#define DMA_SETTRIG_TRIG1                               1 
#define DMA_SETTRIG_TRIG2                               2 
#define DMA_SETTRIG_TRIG3                               3 
#define DMA_SETTRIG_TRIG4                               4 
#define DMA_SETTRIG_TRIG5                               5 
#define DMA_SETTRIG_TRIG6                               6 
#define DMA_SETTRIG_TRIG7                               7 
#define DMA_SETTRIG_TRIG8                               8 
#define DMA_SETTRIG_TRIG9                               9 
#define DMA_SETTRIG_TRIG10                              10
#define DMA_SETTRIG_TRIG11                              11
#define DMA_SETTRIG_TRIG12                              12
#define DMA_SETTRIG_TRIG13                              13
#define DMA_SETTRIG_TRIG14                              14
#define DMA_SETTRIG_TRIG15                              15
#define DMA_SETTRIG_TRIG_MSK                           (   (1ul << DMA_SETTRIG_TRIG0)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG1)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG2)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG3)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG4)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG5)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG6)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG7)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG8)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG9)       |   \
                                                            (1ul << DMA_SETTRIG_TRIG10)      |   \
                                                            (1ul << DMA_SETTRIG_TRIG11)      |   \
                                                            (1ul << DMA_SETTRIG_TRIG12)      |   \
                                                            (1ul << DMA_SETTRIG_TRIG13)      |   \
                                                            (1ul << DMA_SETTRIG_TRIG14)      |   \
                                                            (1ul << DMA_SETTRIG_TRIG15)          \
                                                        )
#define DMA_SETTRIG_TRIG(__SET)                         ((__SET) << DMA_SETTRIG_TRIG0)
//! @}

//! \name DMA ABORT register
//! @{
#define DMA_ABORT_AORTCTRL0                             0 
#define DMA_ABORT_AORTCTRL1                             1 
#define DMA_ABORT_AORTCTRL2                             2 
#define DMA_ABORT_AORTCTRL3                             3 
#define DMA_ABORT_AORTCTRL4                             4 
#define DMA_ABORT_AORTCTRL5                             5 
#define DMA_ABORT_AORTCTRL6                             6 
#define DMA_ABORT_AORTCTRL7                             7 
#define DMA_ABORT_AORTCTRL8                             8 
#define DMA_ABORT_AORTCTRL9                             9 
#define DMA_ABORT_AORTCTRL10                            10
#define DMA_ABORT_AORTCTRL11                            11
#define DMA_ABORT_AORTCTRL12                            12
#define DMA_ABORT_AORTCTRL13                            13
#define DMA_ABORT_AORTCTRL14                            14
#define DMA_ABORT_AORTCTRL15                            15
#define DMA_ABORT_AORTCTRL_MSK                         (   (1ul << DMA_ABORT_AORTCTRL0)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL1)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL2)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL3)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL4)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL5)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL6)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL7)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL8)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL9)       |   \
                                                            (1ul << DMA_ABORT_AORTCTRL10)      |   \
                                                            (1ul << DMA_ABORT_AORTCTRL11)      |   \
                                                            (1ul << DMA_ABORT_AORTCTRL12)      |   \
                                                            (1ul << DMA_ABORT_AORTCTRL13)      |   \
                                                            (1ul << DMA_ABORT_AORTCTRL14)      |   \
                                                            (1ul << DMA_ABORT_AORTCTRL15)          \
                                                        )
#define DMA_ABORT_AORTCTRL(__SET)                       ((__SET) << DMA_ABORT_AORTCTRL0)
//! @}


//! \name DMA Channel CONTRL register
//! @{
#define DMA_CHANNEL_CONTRL_PERIPHREQEN                  0
#define DMA_CHANNEL_CONTRL_PERIPHREQEN_MSK             (DMA_CHANNEL_CONTRL_PERIPHREQEN)

#define DMA_CHANNEL_CONTRL_HWTRIGEN                     1
#define DMA_CHANNEL_CONTRL_HWTRIGEN_MSK                (1ul << DMA_CHANNEL_CONTRL_HWTRIGEN)

#define DMA_CHANNEL_CONTRL_TRIGPOL                      4
#define DMA_CHANNEL_CONTRL_TRIGPOL_MSK                 (1ul << DMA_CHANNEL_CONTRL_TRIGPOL)

#define DMA_CHANNEL_CONTRL_TRIGTYPE                     5
#define DMA_CHANNEL_CONTRL_TRIGTYPE_MSK                (1ul << DMA_CHANNEL_CONTRL_TRIGTYPE)

#define DMA_CHANNEL_CONTRL_TRIGBURST                    6
#define DMA_CHANNEL_CONTRL_TRIGBURST_MSK               (1ul << DMA_CHANNEL_CONTRL_TRIGBURST)

#define DMA_CHANNEL_CONTRL_BURSTPOWER0                  8
#define DMA_CHANNEL_CONTRL_BURSTPOWER1                  9
#define DMA_CHANNEL_CONTRL_BURSTPOWER2                  10
#define DMA_CHANNEL_CONTRL_BURSTPOWER3                  11
#define DMA_CHANNEL_CONTRL_BURSTPOWER_MSK              (   (1ul << DMA_CHANNEL_CONTRL_BURSTPOWER0)     |   \
                                                            (1ul << DMA_CHANNEL_CONTRL_BURSTPOWER1)     |   \
                                                            (1ul << DMA_CHANNEL_CONTRL_BURSTPOWER2)     |   \
                                                            (1ul << DMA_CHANNEL_CONTRL_BURSTPOWER3)         \
                                                        )
#define DMA_CHANNEL_CONTRL_BURSTPOWER(__SET)            ((__SET) << DMA_CHANNEL_CONTRL_BURSTPOWER_SET0)

#define DMA_CHANNEL_CONTRL_SRCBURSTWRAP                 14
#define DMA_CHANNEL_CONTRL_SRCBURSTWRAP_MSK            (1ul << DMA_CHANNEL_CONTRL_SRCBURSTWRAP)             

#define DMA_CHANNEL_CONTRL_DSTBURSTWRAP                 15
#define DMA_CHANNEL_CONTRL_DSTBURSTWRAP_MSK            (1ul << DMA_CHANNEL_CONTRL_DSTBURSTWRAP)

#define DMA_CHANNEL_CONTRL_CHPRIORITY0                  16
#define DMA_CHANNEL_CONTRL_CHPRIORITY1                  17
#define DMA_CHANNEL_CONTRL_CHPRIORITY_MSK              (   (1ul << DMA_CHANNEL_CONTRL_CHPRIORITY0)     |   \
                                                            (1ul << DMA_CHANNEL_CONTRL_CHPRIORITY1)         \
                                                        )
#define DMA_CHANNEL_CONTRL_CHPRIORITY(__SET)            ((__SET) << DMA_CHANNEL_CONTRL_CHPRIORITY0)
//! @}


//! \name DMA Channel CTLSTAT register
//! @{
#define DMA_CHANNEL_CTLSTAT_VALIDPENDING                0
#define DMA_CHANNEL_CTLSTAT_VALIDPENDING_MSK           (1ul << DMA_CHANNEL_CTLSTAT_VALIDPENDING)

#define DMA_CHANNEL_CTLSTAT_TRIG                        2
#define DMA_CHANNEL_CTLSTAT_TRIG_MSK                   (1ul << DMA_CHANNEL_CTLSTAT_TRIG)        
//! @}

//! \name DMA Channel XFERCFG register
//! @{
#define DMA_CHANNEL_XFERCOUNT_CFGVALID                  0
#define DMA_CHANNEL_XFERCOUNT_CFGVALID_MSK             (1ul << DMA_CHANNEL_XFERCOUNT_CFGVALID)

#define DMA_CHANNEL_XFERCOUNT_RELOAD                    1
#define DMA_CHANNEL_XFERCOUNT_RELOAD_MSK               (1ul << DMA_CHANNEL_XFERCOUNT_RELOAD)  

#define DMA_CHANNEL_XFERCOUNT_SWTRIG                    2
#define DMA_CHANNEL_XFERCOUNT_SWTRIG_MSK               (1ul << DMA_CHANNEL_XFERCOUNT_SWTRIG)  

#define DMA_CHANNEL_XFERCOUNT_CLRTRLG                   3
#define DMA_CHANNEL_XFERCOUNT_CLRTRLG_MSK              (1ul << DMA_CHANNEL_XFERCOUNT_CLRTRLG) 

#define DMA_CHANNEL_XFERCOUNT_SETINTA                   4
#define DMA_CHANNEL_XFERCOUNT_SETINTA_MSK              (1ul << DMA_CHANNEL_XFERCOUNT_SETINTA) 

#define DMA_CHANNEL_XFERCOUNT_SETINTB                   5
#define DMA_CHANNEL_XFERCOUNT_SETINTB_MSK              (1ul << DMA_CHANNEL_XFERCOUNT_SETINTB) 

#define DMA_CHANNEL_XFERCOUNT_WIDTH0                    8
#define DMA_CHANNEL_XFERCOUNT_WIDTH1                    9
#define DMA_CHANNEL_XFERCOUNT_WIDTH_MSK                (   (1ul << DMA_CHANNEL_XFERCOUNT_WIDTH0)   |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_WIDTH1)       \
                                                        )

#define DMA_CHANNEL_XFERCOUNT_SRCINC0                   12
#define DMA_CHANNEL_XFERCOUNT_SRCINC1                   13
#define DMA_CHANNEL_XFERCOUNT_SRCINC_MSK               (   (1ul << DMA_CHANNEL_XFERCOUNT_SRCINC0)  |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_SRCINC1)      \
                                                        )
#define DMA_CHANNEL_XFERCOUNT_SRCINC(__SET)             ((__SET) << DMA_CHANNEL_XFERCOUNT_SRCINC0)

#define DMA_CHANNEL_XFERCOUNT_DSTINC0                   14  
#define DMA_CHANNEL_XFERCOUNT_DSTINC1                   15
#define DMA_CHANNEL_XFERCOUNT_DSTINC_MSK               (   (1ul << DMA_CHANNEL_XFERCOUNT_DSTINC0)  |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_DSTINC1)      \
                                                        )
#define DMA_CHANNEL_XFERCOUNT_DSTINC(__SET)             ((__SET) << DMA_CHANNEL_XFERCOUNT_DSTINC0)

#define DMA_CHANNEL_XFERCOUNT_XFERCOUT0                 16
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT1                 17
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT2                 18
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT3                 19
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT4                 20
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT5                 21
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT6                 22
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT7                 23
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT8                 24
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT9                 25
#define DMA_CHANNEL_XFERCOUNT_XFERCOUT_MSK             (   (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT0)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT1)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT2)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT3)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT4)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT5)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT6)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT7)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT8)      |   \
                                                            (1ul << DMA_CHANNEL_XFERCOUNT_XFERCOUT9)          \
                                                        )
#define DMA_CHANNEL_XFERCOUNT(__SET)           ((__SET) << DMA_CHANNEL_XFERCOUNT_XFERCOUT0)
//! @}
/*============================ MACROFIED FUNCTIONS ===========================*/

#ifndef __REG_MACRO__
#define __REG_MACRO__
#define DEF_REG                     \
        union {                     \
            struct {
    
#define END_DEF_REG(__NAME)         \
            };                      \
            reg32_t Value;          \
        }__NAME;
#endif
/*============================ TYPES =========================================*/

#ifndef __REG_TYPE__
#define __REG_TYPE__

typedef volatile unsigned char     reg8_t;
typedef volatile unsigned short    reg16_t;
typedef volatile unsigned int      reg32_t;

#endif

//! \name DMA Channel register
//! @{
typedef struct dma_channel_desciptor_t dma_channel_desciptor_t;

struct dma_channel_desciptor_t {
    uint32_t                        wXFERCFG;
    void*                           pSourceAddr;
    void*                           pDestinationAddr;
    dma_channel_desciptor_t*        pLinkToNext;
};
//! @}

//! \name DMA Channel register
//! @{
typedef struct {
    DEF_REG
        reg32_t     PERIPHREQEN : 1 ;               //!< Perpheral request Enable
        reg32_t     HWTRIGEN    : 1 ;               //!< Hardware Triggering Enable for ths channel
        reg32_t                 : 2 ;
        reg32_t     TRIGPOL     : 1 ;               //!< Trigger Polarity
        reg32_t     TRIGTYPE    : 1 ;               //!< Trigger Type
        reg32_t     TRIGBURST   : 1 ;               //!< Trigger Burst
        reg32_t                 : 1 ;
        reg32_t     BURSTPOWER  : 4 ;               //!< Selects how many transfers are performed for each DMA trigger
        reg32_t                 : 2 ;
        reg32_t     SRCBURSTWRAP: 1 ;               //!< Source Burst Wrap
        reg32_t     DSTBURSTWRAP: 1 ;               //!< Destination Burst Wrap
        reg32_t     CHPRIORITY  : 2 ;               //!< Priority of this channel when multiple DMA requests are pending
        reg32_t                 : 14 ;        
    END_DEF_REG(CFG) 
    
    DEF_REG
        reg32_t     VALIDPENDING: 1 ;               //!< Valid pending flag for this channel
        reg32_t                 : 1 ;
        reg32_t     TRIG        : 1 ;               //!< Trigger flag
        reg32_t                 : 29;
    END_DEF_REG(CTLSTAT)         
        
    DEF_REG
        reg32_t     CFGVALID    : 1 ;               //!< Configuration Valid flag
        reg32_t     RELOAD      : 1 ;               //!< Indicates whether the channel's control structure will be reloaded when the current discriptor is exhausted
        reg32_t     SWTRIG      : 1 ;               //!< Software Trigger
        reg32_t     CLRTRLG     : 1 ;               //!< Clear Trigger
        reg32_t     SETINTA     : 1 ;               //!< Set Interrupt flag A for this channel
        reg32_t     SETINTB     : 1 ;               //!< Set Interrupt flag B for this channel
        reg32_t                 : 2 ;
        reg32_t     WIDTH       : 2 ;               //!< Transfer width used for this DMA channel
        reg32_t                 : 2 ;
        reg32_t     SRCINC      : 2 ;               //!< Determines whether the source address is incremented for each DMA transfer
        reg32_t     DSTINC      : 2 ;               //!< Determines whether the destination address is incremented for each DMA transfer
        reg32_t     XFERCOUT    : 10;               //!< Total number of transfers to be performed,minus 1 encoded
        reg32_t                 : 6 ;        
    END_DEF_REG(XFERCFG) 
        
    reg32_t                     : 32;
} dma_channel_t;
//! @}

//! \name SDMA register page
//! @{
typedef volatile struct {
    DEF_REG
        reg32_t     ENABLE      : 1 ;               //!< DMA controller master enable
        reg32_t                 : 31;
    END_DEF_REG(CONTRL)                             //!< Control register
        
    DEF_REG
        reg32_t                 : 1 ;   
        reg32_t     ACTIVEINT   : 1 ;               //!< summarizes whether any enabled interrupts art pending
        reg32_t     ACTIVEERRINT: 1 ;               //!< summarizes whether any error interrupts art pending
        reg32_t                 : 29;               //!< Interrupt Status register
    END_DEF_REG(INTSTAT) 
        
    DEF_REG
        reg32_t                 : 10;   
        reg32_t     OFFSET      : 22;               //!< Address of the beginning of the DMA desciptor table
    END_DEF_REG(SRAMBASE)                           //!< SRAM Base address register
        
    reg32_t                     : 32;
    reg32_t                     : 32;
    reg32_t                     : 32;
    reg32_t                     : 32;
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     ENA         : 16;               //!< Enable for DMA channels 15:0
        reg32_t                 : 16;
    END_DEF_REG(ENABLESET)                         //!< Enable read and Set registers
        
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     CLR         : 16;               //!< Writing ones to this register clears the corresponing bits in ENABLESET0
        reg32_t                 : 16;
    END_DEF_REG(ENABLECLR)                         //!< Enable Clear register
    
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     ACT         : 16;               //!< Active flag for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(ACTIVE)                            //!< Active status register
        
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     BSY         : 16;               //!< Busy flag for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(BUSY)                              //!< Busy status register

    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     ERR         : 16;               //!< Error Interrupt flag for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(ERRINT)                            //!< Error Interrupt register

    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     INTEN       : 16;               //!< Interrupt Enable read and set for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(INTENSET)                          //!< Interrupt Enable read and Set register
    
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     CLR         : 16;               //!< Writing ones to this register clears the corresponing bits in INTENSET0
        reg32_t                 : 16;
    END_DEF_REG(INTENCLR)                          //!< Interrupt Enable Clear register
    
    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     IA          : 16;               //!< Interrupt A status for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(INTA)                              //!< Interrupt A register

    reg32_t                     : 32;
    
    DEF_REG
        reg32_t     IB          : 16;               //!< Interrupt B status for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(INTB)                              //!< Interrupt B register
        
    reg32_t                     : 32;        
        
    DEF_REG
        reg32_t     SV          : 16;               //!< SETVAILD control for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(SETVALID)                          //!< Set Valid register
        
    reg32_t                     : 32;        
        
    DEF_REG
        reg32_t     TRIG        : 16;               //!< Set Trigger control bit for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(SETTRIG)                           //!< Set Trigger register

    reg32_t                     : 32;        
        
    DEF_REG
        reg32_t     AORTCTRL    : 16;               //!< Abort control for DMA channel n
        reg32_t                 : 16;
    END_DEF_REG(ABORT)                             //!< Abort registers

    reg32_t                     RESERVED[225];
    
    dma_channel_t               Channel[16];    
} dma_reg_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */
