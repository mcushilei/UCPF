#ifndef __I_IO_PSC_H__
#define __I_IO_PSC_H__

//! Register Type definition
#ifndef __REG_TYPE__
#define __REG_TYPE__

typedef volatile unsigned char     reg8_t;
typedef volatile unsigned short    reg16_t;
typedef volatile unsigned long     reg32_t;

#endif
        
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



//! Allow FTM 
#define PSC_FSECURE_ALWFTM                  0
//! Allow FTM Mask
#define PSC_FSECURE_ALWFTM_MSK              (1 << PSC_FSECURE_ALWFTM)
//! Set Value to Allow FTM
#define PSC_FSECURE_ALWFTM_SET(__VAL)       ((__VAL) << PSC_FSECURE_ALWFTM)

//! Allow SWD 
#define PSC_FSECURE_ALWSWD                  1
//! Allow SWD Mask
#define PSC_FSECURE_ALWSWD_MSK              (1 << PSC_FSECURE_ALWSWD) 
//! Set Value to Allow SWD
#define PSC_FSECURE_ALWSWD_SET(__VAL)       ((__VAL) << PSC_FSECURE_ALWSWD)

//! Allow ISP 
#define PSC_FSECURE_ALWISP                  2
//! Allow ISP Mask
#define PSC_FSECURE_ALWISP_MSK              (1 << PSC_FSECURE_ALWISP) 
//! Set Value to Allow ISP 
#define PSC_FSECURE_ALWISP_SET(__VAL)       ((__VAL) << PSC_FSECURE_ALWISP)


//! Peripheral Enable
#define PSC_PRPHEN0                                     0 
#define PSC_PRPHEN1                                     1 
#define PSC_PRPHEN2                                     2 
#define PSC_PRPHEN3                                     3 
#define PSC_PRPHEN4                                     4 
#define PSC_PRPHEN5                                     5 
#define PSC_PRPHEN6                                     6 
#define PSC_PRPHEN7                                     7 
#define PSC_PRPHEN8                                     8 
#define PSC_PRPHEN9                                     9 
#define PSC_PRPHEN10                                    10
#define PSC_PRPHEN11                                    11
#define PSC_PRPHEN12                                    12
#define PSC_PRPHEN13                                    13
#define PSC_PRPHEN14                                    14
#define PSC_PRPHEN15                                    15
//! Peripheral Enable Mask
#define PSC_PRPHEN_MSK                                 (   (1 << PSC_PRPHEN0)      |   \
                                                            (1 << PSC_PRPHEN1)     |   \
                                                            (1 << PSC_PRPHEN2)     |   \
                                                            (1 << PSC_PRPHEN3)     |   \
                                                            (1 << PSC_PRPHEN4)     |   \
                                                            (1 << PSC_PRPHEN5)     |   \
                                                            (1 << PSC_PRPHEN6)     |   \
                                                            (1 << PSC_PRPHEN7)     |   \
                                                            (1 << PSC_PRPHEN8)     |   \
                                                            (1 << PSC_PRPHEN9)     |   \
                                                            (1 << PSC_PRPHEN10)    |   \
                                                            (1 << PSC_PRPHEN11)    |   \
                                                            (1 << PSC_PRPHEN12)    |   \
                                                            (1 << PSC_PRPHEN13)    |   \
                                                            (1 << PSC_PRPHEN14)    |   \
                                                            (1 << PSC_PRPHEN15))
// Set Value to Peripheral Enable
#define PSC_PRPHEN_SET(__VAL)                           ((__VAL) << PSC_PRPHEN0)


//! Flash Size
#define PSC_FLASH_SZ0                                   0
#define PSC_FLASH_SZ1                                   1
#define PSC_FLASH_SZ2                                   2
#define PSC_FLASH_SZ3                                   3
#define PSC_FLASH_SZ4                                   4
#define PSC_FLASH_SZ5                                   5
#define PSC_FLASH_SZ6                                   6
//! Flash Size Mask
#define PSC_FLASH_SZ_MSK                               (   (1 << PSC_FLASH_SZ0) |   \
                                                            (1 << PSC_FLASH_SZ1) |   \
                                                            (1 << PSC_FLASH_SZ2) |   \
                                                            (1 << PSC_FLASH_SZ3) |   \
                                                            (1 << PSC_FLASH_SZ4) |   \
                                                            (1 << PSC_FLASH_SZ5) |   \
                                                            (1 << PSC_FLASH_SZ6))

//! Set Value to Flash Size
#define PSC_FLASH_SZ_SET(__VAL)                         ((__VAL) << PSC_FLASH_SZ0)


//! SRAM SIZE
#define PSC_SRAM_SZ0                                    0
#define PSC_SRAM_SZ1                                    1
#define PSC_SRAM_SZ2                                    2
//! SRAM SIZE Mask
#define PSC_SRAM_SZ_MSK                                (   (1 << PSC_SRAM_SZ0)    |   \
                                                            (1 << PSC_SRAM_SZ1)    |   \
                                                            (1 << PSC_SRAM_SZ2))
//! Set Value to SRAM SIZE
#define PSC_SRAM_SZ_SET(__VAL)                          ((__VAL) << PSC_SRAM_SZ0)


//! ISPA PORTSLICE
#define PSC_ISPAPIN0                                    0
#define PSC_ISPAPIN1                                    1
#define PSC_ISPAPIN2                                    2
#define PSC_ISPAPIN3                                    3
#define PSC_ISPAPIN4                                    4
#define PSC_ISPAPIN5                                    5
//! ISPA PORTSLICE Mask
#define PSC_ISPAPIN_MSK                                (   (1 << PSC_ISPAPIN0) |   \
                                                            (1 << PSC_ISPAPIN1) |   \
                                                            (1 << PSC_ISPAPIN2) |   \
                                                            (1 << PSC_ISPAPIN3) |   \
                                                            (1 << PSC_ISPAPIN4) |   \
                                                            (1 << PSC_ISPAPIN5))
//! Set Value to ISPA PORTSLICE
#define PSC_ISPAPIN_SET(__VAL)                          ((__VAL) << PSC_ISPAPIN0)

//! ISPA PORTSLICE
#define PSC_ISPAPORT0                                   6
#define PSC_ISPAPORT1                                   7
//! ISPA PORT Mask
#define PSC_ISPAPORT_MSK                               (   (1 << PSC_ISPAPORT0) |   \
                                                            (1 << PSC_ISPAPORT1))
//! Set Value to ISPA PORT
#define PSC_ISPAPORT_SET(__VAL)                         ((__VAL) << PSC_ISPAPORT0)

//! ISPB PORTSLICE
#define PSC_ISPBPIN0                                    8
#define PSC_ISPBPIN1                                    9
#define PSC_ISPBPIN2                                    10
#define PSC_ISPBPIN3                                    11
#define PSC_ISPBPIN4                                    12
#define PSC_ISPBPIN5                                    13
//! ISPB PORTSLICE Mask
#define PSC_ISPBPIN_MSK                                (   (1 << PSC_ISPBPIN0) |   \
                                                            (1 << PSC_ISPBPIN1) |   \
                                                            (1 << PSC_ISPBPIN2) |   \
                                                            (1 << PSC_ISPBPIN3) |   \
                                                            (1 << PSC_ISPBPIN4) |   \
                                                            (1 << PSC_ISPBPIN5))
//! Set Value to ISPB PORTSLICE
#define PSC_ISPBPIN_SET(__VAL)                          ((__VAL) << PSC_ISPBPIN0)

//! ISPB PORTSLICE
#define PSC_ISPBPORT0                                   14 
#define PSC_ISPBPORT1                                   15
//! ISPB PORTSLICE Mask
#define PSC_ISPBPORT_MSK                               ((1 << PSC_ISPBPORT0) |   \
                                                          (1 << PSC_ISPBPORT1))
//! Set Value to ISPB PORT
#define PSC_ISPBPORT_SET(__VAL)                         ((__VAL) << PSC_ISPBPORT0)

//! ISPC PORTSLICE
#define PSC_ISPCPIN0                                    16
#define PSC_ISPCPIN1                                    17 
#define PSC_ISPCPIN2                                    18
#define PSC_ISPCPIN3                                    19
#define PSC_ISPCPIN4                                    20
#define PSC_ISPCPIN5                                    21 
//! ISPC PORTSLICE Mask
#define PSC_ISPCPIN_MSK                                (   (1 << PSC_ISPCPIN0) |   \
                                                            (1 << PSC_ISPCPIN1) |   \
                                                            (1 << PSC_ISPCPIN2) |   \
                                                            (1 << PSC_ISPCPIN3) |   \
                                                            (1 << PSC_ISPCPIN4) |   \
                                                            (1 << PSC_ISPCPIN5))
//! Set Value to ISPC PORTSLICE
#define PSC_ISPCPIN_SET(__VAL)                          ((__VAL) << PSC_ISPCPIN0)

//! ISPC PORTSLICE
#define PSC_ISPCPORT0                                   22
#define PSC_ISPCPORT1                                   23
//! ISPC PORT Mask
#define PSC_ISPCPORT_MSK                               (   (1 << PSC_ISPCPORT0) |   \
                                                             (1 << PSC_ISPCPORT1))
//! Set Value to ISPC PORT
#define PSC_ISPCPORT_SET(__VAL)                         ((__VAL) << PSC_ISPCPORT0)


//! ISPD PORTSLICE
#define PSC_ISPDPIN0                                    24
#define PSC_ISPDPIN1                                    25
#define PSC_ISPDPIN2                                    26
#define PSC_ISPDPIN3                                    27
#define PSC_ISPDPIN4                                    28
#define PSC_ISPDPIN5                                    29

//! ISPD PORTSLICE Mask
#define PSC_ISPDPIN_MSK                                (   (1 << PSC_ISPDPORTSLICE0) |   \
                                                            (1 << PSC_ISPDPORTSLICE1) |   \
                                                            (1 << PSC_ISPDPORTSLICE2) |   \
                                                            (1 << PSC_ISPDPORTSLICE3) |   \
                                                            (1 << PSC_ISPDPORTSLICE4) |   \
                                                            (1 << PSC_ISPDPORTSLICE5))
//! Set Value to ISPD PORTSLICE
#define PSC_ISPDPIN_SET(__VAL)                          ((__VAL) << PSC_ISPDPIN0)

//! ISPD PORTSLICE
#define PSC_ISPDPORT0                                   30
#define PSC_ISPDPORT1                                   31
//! ISPD PORT Mask
#define PSC_ISPDPORT_MSK                               (   (1 << PSC_ISPDPORT0) |   \
                                                             (1 << PSC_ISPDPORT1))
//! Set Value to ISPD PORT
#define PSC_ISPDPORT_SET(__VAL)                         ((__VAL) << PSC_ISPDPORT0)


//! Irc Trim
#define PSC_IRCTRIM0                                   0
#define PSC_IRCTRIM1                                   1
#define PSC_IRCTRIM2                                   2
#define PSC_IRCTRIM3                                   3
#define PSC_IRCTRIM4                                   4
#define PSC_IRCTRIM5                                   5
#define PSC_IRCTRIM6                                   6
#define PSC_IRCTRIM7                                   7
//! Irc Trim Mask
#define PSC_IRCTRIM_MSK                               (   (1 << PSC_IRCTRIM0)  |   \
                                                            (1 << PSC_IRCTRIM1)  |   \
                                                            (1 << PSC_IRCTRIM2)  |   \
                                                            (1 << PSC_IRCTRIM3)  |   \
                                                            (1 << PSC_IRCTRIM4)  |   \
                                                            (1 << PSC_IRCTRIM5)  |   \
                                                            (1 << PSC_IRCTRIM6)  |   \
                                                            (1 << PSC_IRCTRIM7))
//! Set Value to Irc Trim
#define PSC_IRCTRIM_SET(__VAL)                         ((__VAL) << PSC_IRCTRIM0)


//! Watchdog Oscillator Trim
#define PSC_WDOSCTRIM0                                  0
#define PSC_WDOSCTRIM1                                  1
#define PSC_WDOSCTRIM2                                  2
#define PSC_WDOSCTRIM3                                  3
#define PSC_WDOSCTRIM4                                  4
#define PSC_WDOSCTRIM5                                  5
#define PSC_WDOSCTRIM6                                  6
#define PSC_WDOSCTRIM7                                  7
#define PSC_WDOSCTRIM8                                  8
//! Watchdog Oscillator Trim Mask
#define PSC_LFLA_WOSC_RIM_MSK                          (   (1 << PSC_WDOSCTRIM0) |   \
                                                            (1 << PSC_WDOSCTRIM1) |   \
                                                            (1 << PSC_WDOSCTRIM2) |   \
                                                            (1 << PSC_WDOSCTRIM3) |   \
                                                            (1 << PSC_WDOSCTRIM4) |   \
                                                            (1 << PSC_WDOSCTRIM5) |   \
                                                            (1 << PSC_WDOSCTRIM6) |   \
                                                            (1 << PSC_WDOSCTRIM7) |   \
                                                            (1 << PSC_WDOSCTRIM8))
//! Set Value to Watchdog Oscillator Trim
#define PSC_LFLA_WOSC_RIM(__VAL)                        ((__VAL) << PSC_WDOSCTRIM0)


//! Core Voltage Trim Setting
#define PSC_VCORETRIM0                                  0
#define PSC_VCORETRIM1                                  1
#define PSC_VCORETRIM2                                  2
#define PSC_VCORETRIM3                                  3
#define PSC_VCORETRIM4                                  4
//! Core Voltage Trim Setting Mask
#define PSC_VCORETRIM_MSK                              (   (1 << PSC_VCORETRIM0) |   \
                                                            (1 << PSC_VCORETRIM1) |   \
                                                            (1 << PSC_VCORETRIM2) |   \
                                                            (1 << PSC_VCORETRIM3) |   \
                                                            (1 << PSC_VCORETRIM4))
//! Set Value to Core Voltage Trim
#define PSC_VCORETRIM_SET(__VAL)                        ((__VAL) << PSC_VCORETRIM0)


//! Analog Voltage Trim Setting
#define PSC_VDDATRIM0                                   0
#define PSC_VDDATRIM1                                   1
#define PSC_VDDATRIM2                                   2
#define PSC_VDDATRIM3                                   3
#define PSC_VDDATRIM4                                   4
//! Analog Voltage Trim Setting Mask
#define PSC_VDDATRIM_MSK                               (   (1 << PSC_VDDATRIM0) |   \
                                                            (1 << PSC_VDDATRIM1) |   \
                                                            (1 << PSC_VDDATRIM2) |   \
                                                            (1 << PSC_VDDATRIM3) |   \
                                                            (1 << PSC_VDDATRIM4))
//! Set Value to Analog Voltage Trim
#define PSC_VDDATRIM_SET(__VAL)                         ((__VAL) << PSC_VDDATRIM0)


//! Auto Clock Enable Setting
#define PSC_AUTOCLKEN0                                  0
#define PSC_AUTOCLKEN1                                  1
#define PSC_AUTOCLKEN2                                  2
#define PSC_AUTOCLKEN3                                  3
#define PSC_AUTOCLKEN4                                  4
#define PSC_AUTOCLKEN5                                  5
#define PSC_AUTOCLKEN6                                  6
#define PSC_AUTOCLKEN7                                  7
#define PSC_AUTOCLKEN8                                  8
#define PSC_AUTOCLKEN9                                  9
#define PSC_AUTOCLKEN10                                 0
#define PSC_AUTOCLKEN11                                 1
#define PSC_AUTOCLKEN12                                 2
#define PSC_AUTOCLKEN13                                 3
#define PSC_AUTOCLKEN14                                 4
#define PSC_AUTOCLKEN15                                 5
//! Auto Clock Enable Setting Mask
#define PSC_AUTOCLKEN_MSK                              (   (1 << PSC_AUTOCLKEN0)     |   \
                                                            (1 << PSC_AUTOCLKEN1)     |   \
                                                            (1 << PSC_AUTOCLKEN2)     |   \
                                                            (1 << PSC_AUTOCLKEN3)     |   \
                                                            (1 << PSC_AUTOCLKEN4)     |   \
                                                            (1 << PSC_AUTOCLKEN5)     |   \
                                                            (1 << PSC_AUTOCLKEN6)     |   \
                                                            (1 << PSC_AUTOCLKEN7)     |   \
                                                            (1 << PSC_AUTOCLKEN8)     |   \
                                                            (1 << PSC_AUTOCLKEN9)     |   \
                                                            (1 << PSC_AUTOCLKEN10)    |   \
                                                            (1 << PSC_AUTOCLKEN11)    |   \
                                                            (1 << PSC_AUTOCLKEN12)    |   \
                                                            (1 << PSC_AUTOCLKEN13)    |   \
                                                            (1 << PSC_AUTOCLKEN14)    |   \
                                                            (1 << PSC_AUTOCLKEN15))
//! Set Value to Auto Clock Enable Setting
#define PSC_AUTOCLKEN_SET(__VAL)                        ((__VAL) << PSC_AUTOCLKEN0)


//!< MASTER0 PRIORITY 
#define PSC_AHBPRI_MASTER0_PRI0                         0
#define PSC_AHBPRI_MASTER0_PRI1                         1
#define PSC_AHBPRI_MASTER0_PRI2                         2
//! MASTER0 PRIORITY Mask
#define PSC_AHBPRI_MASTER0_PRI_MSK                     (   (1 << PSC_AHBPRI_MASTER0_PRI0)  |   \
                                                            (1 << PSC_AHBPRI_MASTER0_PRI1)  |   \
                                                            (1 << PSC_AHBPRI_MASTER0_PRI2))
//! Set Value to MASTER0 PRIORITY
#define PSC_AHBPRI_MASTER0_PRI_SET(__VAL)               ((__VAL) << PSC_AHBPRI_MASTER0_PRI0)

//! MASTER1 PRIORITY 
#define PSC_AHBPRI_MASTER1_PRI0                         3
#define PSC_AHBPRI_MASTER1_PRI1                         4
#define PSC_AHBPRI_MASTER1_PRI2                         5
//! MASTER1 PRIORITY Mask
#define PSC_AHBPRI_MASTER1_PRI_MSK                     (   (1 << PSC_AHBPRI_MASTER1_PRI0)  |   \
                                                            (1 << PSC_AHBPRI_MASTER1_PRI1)  |   \
                                                            (1 << PSC_AHBPRI_MASTER1_PRI2))
//! Set Value to MASTER1 PRIORITY
#define PSC_AHBPRI_MASTER1_PRI_SET(__VAL)               ((__VAL) << PSC_AHBPRI_MASTER1_PRI0)

//! MASTER2 PRIORITY 
#define PSC_AHBPRI_MASTER2_PRI0                         6
#define PSC_AHBPRI_MASTER2_PRI1                         7
#define PSC_AHBPRI_MASTER2_PRI2                         8
//! MASTER2 PRIORITY Mask
#define PSC_AHBPRI_MASTER2_PRI_MSK                     (   (1 << PSC_AHBPRI_MASTER2_PRI0)  |   \
                                                            (1 << PSC_AHBPRI_MASTER2_PRI1)  |   \
                                                            (1 << PSC_AHBPRI_MASTER2_PRI2))
//! Set Value to MASTER2 PRIORITY
#define PSC_AHBPRI_MASTER2_PRI_SET(__VAL)               ((__VAL) << PSC_AHBPRI_MASTER2_PRI0)

//! MASTER3 PRIORITY 
#define PSC_AHBPRI_MASTER3_PRI0                         9
#define PSC_AHBPRI_MASTER3_PRI1                         10
#define PSC_AHBPRI_MASTER3_PRI2                         11
//! MASTER3 PRIORITY Mask
#define PSC_AHBPRI_MASTER3_PRI_MSK                     (   (1 << PSC_AHBPRI_MASTER3_PRI0)  |   \
                                                            (1 << PSC_AHBPRI_MASTER3_PRI1)  |   \
                                                            (1 << PSC_AHBPRI_MASTER3_PRI2))
//! Set Value to MASTER3 PRIORITY
#define PSC_AHBPRI_MASTER3_PRI_SET(__VAL)               ((__VAL) << PSC_AHBPRI_MASTER3_PRI0)

//! MASTER4 PRIORITY 
#define PSC_AHBPRI_MASTER4_PRI0                         12
#define PSC_AHBPRI_MASTER4_PRI1                         13
#define PSC_AHBPRI_MASTER4_PRI2                         14
//! MASTER4 PRIORITY Mask
#define PSC_AHBPRI_MASTER4_PRI_MSK                     (   (1 << PSC_AHBPRI_MASTER4_PRI0)  |   \
                                                            (1 << PSC_AHBPRI_MASTER4_PRI1)  |   \
                                                            (1 << PSC_AHBPRI_MASTER4_PRI2))
//! Set Value to MASTER4 PRIORITY
#define PSC_AHBPRI_MASTER4_PRI_SET(__VAL)               ((__VAL) << PSC_AHBPRI_MASTER4_PRI0)


//! iStarChip Manufacturer ID
#define PSC_ISCMID0                                     0 
#define PSC_ISCMID1                                     1 
#define PSC_ISCMID2                                     2 
#define PSC_ISCMID3                                     3 
#define PSC_ISCMID4                                     4 
#define PSC_ISCMID5                                     5 
#define PSC_ISCMID6                                     6 
#define PSC_ISCMID7                                     7 
#define PSC_ISCMID8                                     8 
#define PSC_ISCMID9                                     9 
#define PSC_ISCMID10                                    10
//! iStarChip Manufacturer ID Mask
#define PSC_ISCMID_MSK                                 (   (1 << PSC_ISCMID0)    |   \
                                                            (1 << PSC_ISCMID1)    |   \
                                                            (1 << PSC_ISCMID2)    |   \
                                                            (1 << PSC_ISCMID3)    |   \
                                                            (1 << PSC_ISCMID4)    |   \
                                                            (1 << PSC_ISCMID5)    |   \
                                                            (1 << PSC_ISCMID6)    |   \
                                                            (1 << PSC_ISCMID7)    |   \
                                                            (1 << PSC_ISCMID8)    |   \
                                                            (1 << PSC_ISCMID9)    |   \
                                                            (1 << PSC_ISCMID10))
//! Set Value to iStarChip Manufacturer ID
#define PSC_ISCMID_SET(__VAL)                           ((__VAL) << PSC_ISCMID0)
              

//! Unique ID Word 0
#define PSC_UID0_REG                        GSP_PSC.UID[0]
#define PSC_UID0                            PSC_UID0_REG

//! Unique ID Word 1
#define PSC_UID1_REG                        GSP_PSC.UID[1]
#define PSC_UID1                            PSC_UID1_REG

//! Unique ID Word 2
#define PSC_UID2_REG                        GSP_PSC.UID[2]
#define PSC_UID2                            PSC_UID2_REG

//! Unique ID Word 3
#define PSC_UID3_REG                        GSP_PSC.UID[3]
#define PSC_UID3                            PSC_UID3_REG


//! PACKAGE TYPE ID           
#define PSC_PACKID                          GSP_PSC.PACKID
					
//! FACTORY SECURE
#define PSC_FSECURE_REG                     GSP_PSC.FSECURE
#define PSC_FSECURE                         PSC_FSECURE_REG.Value
#define PSC_FSECURE_BIT                     PSC_FSECURE_REG
					
//! SIGNATURE
#define PSC_SIGN_REG                        GSP_PSC.SIGN
#define PSC_SIGN                            PSC_SIGN_REG
						
//! PRODUCT ID
#define PSC_PRODID_REG                      GSP_PSC.PRODID
#define PSC_PRODID                          PSC_PRODID_REG.Value
#define PSC_PRODID_BIT                      PSC_PRODID_REG
						
//! CONFIGURATION ID
#define PSC_CFGID_REG                       GSP_PSC.CFGID
#define PSC_CFGID                           PSC_CFGID_REG
				
//! PERIPHERAL ENABLE
#define PSC_PRPHEN_REG                      GSP_PSC.PRPHEN
#define PSC_PRPHEN                          PSC_PRPHEN.Value
#define PSC_PRPHEN_BIT                      PSC_PRPHEN.Bit
				
//! FLASH SIZE
#define PSC_FLASHSZ_REG                     GSP_PSC.FLASHSZ
#define PSC_FLASHSZ                         PSC_FLASHSZ_REG.Value
#define PSC_FLASHSZ_BIT                     PSC_FLASHSZ_REG
						
//! SRAM0 SIZE
#define PSC_SRAM0SZ_REG                     GSP_PSC.SRAM0SZ
#define PSC_SRAM0SZ                         PSC_SRAM0SZ_REG.Value
#define PSC_SRAM0SZ_BIT                     PSC_SRAM0SZ_REG
						
//! SRAM1 SIZE
#define PSC_SRAM1SZ_REG                     GSP_PSC.SRAM1SZ
#define PSC_SRAM1SZ                         PSC_SRAM1SZ.Value
#define PSC_SRAM1SZ_BIT                     PSC_SRAM1SZ.Bit
						
//! SRAM2 SIZE
#define PSC_SRAM2SZ_REG                     GSP_PSC.SRAM2SZ
#define PSC_SRAM2SZ                         PSC_SRAM2SZ.Value
#define PSC_SRAM2SZ_BIT                     PSC_SRAM2SZ.Bit

//! ISP ID
#define PSC_ISPID_REG                       GSP_PSC.ISPID
#define PSC_ISPID                           PSC_ISPID_REG.Value
#define PSC_ISPID_BIT                       PSC_ISPID_REG
							
//! IRC TRIM
#define PSC_IRCTRIM_REG                     GSP_PSC.IRCTRIM
#define PSC_IRCTRIM                         PSC_IRCTRIM_REG.Value
#define PSC_IRCTRIM_BIT                     PSC_IRCTRIM_REG
						
//! LFLA WDT OSC TRIM
#define PSC_WDOSCTRIM_REG                   GSP_PSC.WDOSCTRIM
#define PSC_WDOSCTRIM                       PSC_WDOSCTRIM_REG.Value
#define PSC_WDOSCTRIM_BIT                   PSC_WDOSCTRIM_REG
				
//! VDD CORE TRIM
#define PSC_VCORETRIM_REG                   GSP_PSC.VCORETRIM
#define PSC_VCORETRIM                       PSC_VCORETRIM_REG.Value
#define PSC_VCORETRIM_BIT                   PSC_VCORETRIM_REG
					
//! VDDA 1V8 TRIM
#define PSC_VDDATRIM_REG                    GSP_PSC.VDDATRIM
#define PSC_VDDATRIM                        PSC_VDDATRIM_REG.Value
#define PSC_VDDATRIM_BIT                    PSC_VDDATRIM_REG
					
//! AUTO CLK ENABLE
#define PSC_AUTOCLKEN_REG                   GSP_PSC.AUTOCLKEN
#define PSC_AUTOCLKEN                       PSC_AUTOCLKEN_REG.Value
#define PSC_AUTOCLKEN_BIT                   PSC_AUTOCLKEN_REG
					
//! AHB PRIORITY
#define PSC_AHBPRI_REG                      GSP_PSC.AHBPRI
#define PSC_AHBPRI                          PSC_AHBPRI_REG.Value
#define PSC_AHBPRI_BIT                      PSC_AHBPRI_REG
					
//! MICRO JTAG DEVICE ID
#define PSC_JTGDEVID_REG                    GSP_PSC.JTGDEVID
#define PSC_JTGDEVID                        PSC_JTGDEVID
			
//! MICRO JTAG DP ID
#define PSC_JTGDAPID_REG                    GSP_PSC.JTGDAPID
#define PSC_JTGDAPID                        PSC_JTGDAPID_REG
			 
//! MICRO SW DP ID
#define PSC_SWDID_REG                       GSP_PSC.SWDID
#define PSC_SWDID                           PSC_SWDID_REG
					
//! ISC MANUFACTURER ID
#define PSC_ISCMID_REG                      GSP_PSC.ISCMID
#define PSC_ISCMID                          PSC_ISCMID_REG.Value
#define PSC_ISCMID_BIT                      PSC_ISCMID_REG
				
//! JTAG DEVICE ID
#define PSC_JTAGID_REG                      GSP_PSC.JTAGID
#define PSC_JTAGID                          PSC_JTAGID_REG
					
//! ALLOW FTM ENTRY
#define PSC_ALWFTM_REG                      GSP_PSC.ALWFTM
#define PSC_ALWFTM                          PSC_ALWFTM_REG
					
//! ALLOW_SWD ENTRY
#define PSC_ALWSWD_REG                      GSP_PSC.ALWSWD
#define PSC_ALWSWD                          PSC_ALWSWD_REG
					
//! ALLOW ISP ENTRY
#define PSC_ALWISP_REG                      GSP_PSC.ALWISP
#define PSC_ALWISP                          PSC_ALWISP_REG
					
//! System Controller Register Page 
//! \name psc_reg_t
//! @{
typedef struct {
    reg32_t                                 UID[4];         //!< Unique ID Word 0 Register
    
    reg32_t                                 PACKID;         //!< Package ID Indicator Register
    
    DEF_REG
        reg32_t     ALWFTM                  :1;             //!< Select the ability to enter into production test mode
        reg32_t     ALWSWD                  :1;             //!< Select the use of serial wire debug functionality
        reg32_t     ALWISP                  :1;             //!< Select use of ISP Functionality
        reg32_t                             :29;            //<! Reserved
    END_DEF_REG(FSECURE)
    
    reg32_t                                 SIGN;           //!< Signature Indicator Register

    reg32_t                                 PRODID;         //!< Product ID For CHIP JTAG Register

    reg32_t                                 CFGID;          //!< Configuration ID Register       

    //!< Peripheral Enable Security Setting
    DEF_REG
        reg32_t     ENABLE                  :16;            //!< Peripheral enable
        reg32_t                             :16;            //!< Reserved
    END_DEF_REG(PRPHEN)

    //!< Flash Size Security Setting      
    DEF_REG
        reg32_t     SIZE                    :7;             //!< Flash Size Security
        reg32_t                             :25;            //!< Reserved
    END_DEF_REG(FLASHSZ)

    //!< SRAM0 Size Security Setting     
    DEF_REG
        reg32_t     SIZE                    : 3;            //!< SRAM SIZE
        reg32_t                             :29;            //!< Reserved
    END_DEF_REG(SRAM0SZ)

    //!< SRAM1 Size Security Setting    
    DEF_REG
        reg32_t     SIZE                    : 3;            //!< SRAM SIZE
        reg32_t                             :29;            //!< Reserved        
    END_DEF_REG(SRAM1SZ)

    //!< SRAM2 Size Security Setting   
    DEF_REG
        reg32_t     SIZE                    : 3;            //!< SRAM SIZE
        reg32_t                             :29;            //!< Reserved        
    END_DEF_REG(SRAM2SZ)

    //!< ISP ID(Pin Locations)        
    DEF_REG
        reg32_t     ISPAPIN                 : 8;
        reg32_t     ISPBPIN                 : 8;
        reg32_t     ISPCPIN                 : 8;
        reg32_t     ISPDPIN                 : 8;
    END_DEF_REG(ISPID)

    //!< IRC Trim Setting (Secure Write)
    DEF_REG
        reg32_t     TRIM                    :8;             //!< Irc trim
        reg32_t                             :24;            //!< Reserved        
    END_DEF_REG(IRCTRIM)

    //!< Watchdog Oscillator Trim Setting   
    DEF_REG
        reg32_t FREQ                        : 4;
        reg32_t DIV                         : 5;
        reg32_t                             :23;       
    END_DEF_REG(WDOSCTRIM)



    //!< Auto Clock Enable Setting (Secure Write) 
    DEF_REG
        reg32_t     ENABLE                  :16;            //!< Auto Clock Enable Setting
        reg32_t                             :16;            //!< Reserved        
    END_DEF_REG(AUTOCLKEN)

    //!< AHB Matrix Priority Setting (Secure Write)
    DEF_REG
        reg32_t     MASTER0                 :3;             //!< MASTER0 PRIORITY
        reg32_t     MASTER1                 :3;             //!< MASTER1 PRIORITY
        reg32_t     MASTER2                 :3;             //!< MASTER2 PRIORITY
        reg32_t     MASTER3                 :3;             //!< MASTER3 PRIORITY
        reg32_t     MASTER4                 :3;             //!< MASTER4 PRIORITY
        reg32_t                             :17;            //!< Reserved        
    END_DEF_REG(AHBPRI)

    reg32_t                                 : 32;
    reg32_t                                 : 32;

    reg32_t                                 JTGDEVID;       //!< Micro JTAG Device ID                    
    reg32_t                                 JTGDAPID;       //!< Micro JTAG Device ID                    
    reg32_t                                 SWDID;          //!< Micro JTAG DAP ID                   

    //!< iStarChip Manufacturer DAP ID      
    DEF_REG
        reg32_t     ID                      :11;            //!< iStarChip Manufacturer ID
        reg32_t                             :21;            //!< Reserved            
    END_DEF_REG(ISCMID)

    reg32_t                                 RESERVED0[7]; //!< Reserved
    
    //!< Core Voltage Trim Setting (Secure Write)
    DEF_REG
        reg32_t     TRIM                    :5;             //!< Core Voltage Trim Setting 
        reg32_t                             :27;            //!< Receive        
    END_DEF_REG(VCORETRIM)

    //!< Analog Voltage Trim Setting (Secure Write)
    DEF_REG
        reg32_t     TRIM                    :5;             //!< Analog Voltage Trim Setting
        reg32_t                             :27;            //!< Receive        
    END_DEF_REG(VDDATRIM)

    reg32_t                                 RESERVED1[158]; //!< Reserved 
    reg32_t                                 JTAGID;         //!< Tiger¡¯s 32-Bit JTAG Device ID

    reg32_t                                 RESERVED2[55];  //!< Reserved 
    reg32_t                                 ALWFTM;         //!< Enable/Disable Factory Test Mode
    reg32_t                                 ALWSWD;         //!< Enable/Disable Serial Wire Debug
    reg32_t                                 ALWISP;         //!< Enable/Disable ISP Mode

    reg32_t                                 RESERVED3[4];
    reg32_t                                 CHIPVER;        //!< chip version
    
    reg32_t                                 RESERVED4[64];

    DEF_REG
        reg32_t     READDE                  : 1;            //!< Secure the low 1K Byte
        reg32_t                             : 31;
    END_DEF_REG(BOOTSEC)                                   //!< bootrom secure bit

    DEF_REG
        reg32_t     READDE                  : 1;            //!< disable read
        reg32_t     WRITEDE                 : 1;            //!< disable write
        reg32_t                             : 30;
    END_DEF_REG(INFOSEC)
    
    reg32_t                                 RESERVED5[188];
    
    reg32_t         RAWPIOA;
    reg32_t         RAWPIOB;

} psc_reg_t;                                                   
//! @}                                                             

//! psc.h 
#endif

/*EOF*/

