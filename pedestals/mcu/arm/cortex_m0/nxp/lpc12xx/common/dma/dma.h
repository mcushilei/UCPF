#ifndef __DRIVER_ARM_M0_ISTARCHIP_GSP_SDMA_H__
#define __DRIVER_ARM_M0_ISTARCHIP_GSP_SDMA_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\i_io_dma.h"

/*============================ MACROS ========================================*/
//!< DMA Channel 
#define __DMA_CHANNEL(__N, __VALUE)     DMA_CHANNEL##__N = __N,
#define __DMA_CHANNEL_M(__N, __VALUE)   DMA_CHANNEL##__N##_MSK = (1ul<<(__N)),

//!< Memory/Periphral Configuration
#define DMA_TSF_MODE_MEMORY_TO_MEMORY       (\
                DMA_TSF_SOURCE_ADDR_INC_1WIDTH          |   \
                DMA_TSF_DESTINATION_ADDR_INC_1WIDTH         )

#define DMA_TSF_MODE_MEMORY_TO_PERIPHRAL    (\
                DMA_TSF_SOURCE_ADDR_INC_1WIDTH          |   \
                DMA_TSF_DESTINATION_ADDR_NO_INCREMENT       )

#define DMA_TSF_MODE_PERIPHRAL_TO_MEMORY    (\
                DMA_TSF_SOURCE_ADDR_NO_INCREMENT        |   \
                DMA_TSF_DESTINATION_ADDR_INC_1WIDTH         )

#define DMA_TSF_MODE_PERIPHRAL_TO_PERIPHRAL (\
                DMA_TSF_SOURCE_ADDR_NO_INCREMENT        |   \
                DMA_TSF_SOURCE_ADDR_NO_INCREMENT            )

//!< Auto/Manual Configuration     
#define DMA_TSF_AUTO_TRIGGER_NEXT           (DMA_TSF_TRIGGER_SOFTWARE_SET_DISABLE|      \
                                            DMA_TSF_TRIGGER_AUTO_CLEAR_DISABLE)

#define DMA_TSF_MANUAL_TRIGGER_NEXT         (DMA_TSF_TRIGGER_SOFTWARE_SET_DISABLE|      \
                                            DMA_TSF_TRIGGER_AUTO_CLEAR_ENABLE)

//!< DMA Transfer Size Configuration
#define DMA_TSF_DATA_BITS_8                 DMA_TSF_WIDTH_8BIT
#define DMA_TSF_DATA_BITS_16                DMA_TSF_WIDTH_16BIT
#define DMA_TSF_DATA_BITS_32                DMA_TSF_WIDTH_32BIT  
#define DMA_TSF_DATA_BITS_BYTE              DMA_TSF_WIDTH_8BIT
#define DMA_TSF_DATA_BITS_HALFWORD          DMA_TSF_WIDTH_16BIT
#define DMA_TSF_DATA_BITS_WORD              DMA_TSF_WIDTH_32BIT 

//!< DMA Transfer finish node
#define DMA_TSF_FINAL_NODE                  DMA_TSF_RELOAD_ENABLE

/*============================ MACROFIED FUNCTIONS ===========================*/

//!< transfer size DMA Channel Configuration Macro
#define TRANSFER_SIZE(__N)              (((__N-1) & 0x3FF) << 16ul) 

//!< srambase address offest
#define SRAMBASE_ADDR_OFFEST(__SRAM_BASE_ADDR, __DMA_CHANNEL)                   \
        (__SRAM_BASE_ADDR + __DMA_CHANNEL * sizeof(dma_channel_desciptor_t))

//!< dma transfer configuration
#define DMA_TSF_CFG(\
        __DMA_MDOE, __SIZE,__SOURCE_HEAD_ADDR, __DESTINATION_HEAD_ADDR)         \
    {                                                                           \
        ((((__DMA_MDOE)                  |                                      \
        DMA_TSF_CFG_VALID_FLAG_ENABLE)   ^                                      \
        DMA_TSF_RELOAD_ENABLE)           |                                      \
        TRANSFER_SIZE(__SIZE))  ,                                               \
        (void*)(((unsigned int)__SOURCE_HEAD_ADDR)                              \
                + (((((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_SRCINC_MSK)          \
                    >> DMA_CHANNEL_XFERCOUNT_SRCINC0) < 3ul)                    \
                ? (((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_SRCINC_MSK)            \
                    >> DMA_CHANNEL_XFERCOUNT_SRCINC0):4ul)                      \
                * (1 << (((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_WIDTH_MSK )      \
                    >> DMA_CHANNEL_XFERCOUNT_WIDTH0))                           \
                * (__SIZE -1)),                                                 \
        (void*)(((unsigned int)__DESTINATION_HEAD_ADDR)                         \
                + (((((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_DSTINC_MSK)          \
                    >> DMA_CHANNEL_XFERCOUNT_DSTINC0) < 3ul)                    \
                ? (((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_DSTINC_MSK)            \
                    >> DMA_CHANNEL_XFERCOUNT_DSTINC0):4ul)                      \
                * (1 << (((__DMA_MDOE) & DMA_CHANNEL_XFERCOUNT_WIDTH_MSK )      \
                    >> DMA_CHANNEL_XFERCOUNT_WIDTH0))                           \
                *(__SIZE -1)),                                                  \
    }                
   
//!< dma channel configuration
#define DMA_CHN_CFG(__CHANNEL, __CHANNEL_MODE, ...)                             \
    do {                                                                        \
        ALIGN(0x10)                                                             \
        static dma_channel_desciptor_t tCFG[] = {__VA_ARGS__};                  \
        dma_cfg_t tDMA_CFG = {                                                  \
            __CHANNEL_MODE,                                                     \
            tCFG[0].wXFERCFG,                                                   \
            &tSRAMBASE[__CHANNEL],                                              \
        };                                                                      \
        dma_link_loop_back(tCFG, UBOUND(tCFG));                                 \
        tSRAMBASE[__CHANNEL] = tCFG[0];                                         \
        DMA0.Init(__CHANNEL, &tDMA_CFG);                                        \
        DMA0.Enable(1ul << __CHANNEL);                                          \
    } while(0) 

#define AUTO_LINK_CFG(\
        __CHANNEL, __CHANNEL_MODE, __TRANSFER_MODE, __SIZE, __W_SRC, __P_DST)   \
    do {                                                                        \
        ALIGN(0x10)                                                             \
        static dma_channel_desciptor_t tCFG[__SIZE];                            \
        dma_auto_link(tCFG, __W_SRC, __P_DST, __SIZE, __TRANSFER_MODE);         \
                                                                                \
        dma_cfg_t tDMA_CFG = {                                                  \
            __CHANNEL_MODE,                                                     \
            tCFG[0].wXFERCFG,                                                   \
            &tSRAMBASE[__CHANNEL],                                              \
        };                                                                      \
        tSRAMBASE[__CHANNEL] = tCFG[0];                                         \
        DMA0.Init(__CHANNEL, &tDMA_CFG);                                        \
        DMA0.Enable(1ul << __CHANNEL);                                          \
    } while (false);

/*============================ TYPES =========================================*/

//! \name DMA index number
//! @{
typedef enum {
    MREPEAT(16, __DMA_CHANNEL, NULL)
    MREPEAT(16, __DMA_CHANNEL_M, NULL)
}em_dma_index_t;
//! @}

//! \name DMA Channel Base configuration structure
//! @{
typedef enum {
    DMA_CHN_PERIPHERAL_REQUEST_DISABLE      = 0x00000000,
    DMA_CHN_PERIPHERAL_REQUEST_ENABLE       = 0x00000001,
    
    DMA_CHN_HARDWARE_TRIGGERING_DISABLE     = 0x00000002,
    DMA_CHN_HARDWARE_TRIGGERING_ENABLE      = 0x00000000,
    
    DMA_CHN_TRIG_RISING_EDGE_SENSITIVE      = 0x00000000,
    DMA_CHN_TRIG_FALLING_EDGE_SENSITIVE     = 0x00000010,
    DMA_CHN_TRIG_HIGH_LEVEL_SENSITIVE       = 0x00000020,
    DMA_CHN_TRIG_LOW_LEVEL_SENSITIVE        = 0x00000030,
    
    DMA_CHN_SINGLE_TRANSFER                 = 0x00000000,
    DMA_CHN_BURST_TRANSFER                  = 0x00000040,
    
    DMA_CHN_BURST_SIZE_1                    = 0x00000000,
    DMA_CHN_BURST_SIZE_2                    = 0x00000100,
    DMA_CHN_BURST_SIZE_4                    = 0x00000200,
    DMA_CHN_BURST_SIZE_8                    = 0x00000300,
    DMA_CHN_BURST_SIZE_16                   = 0x00000400,
    DMA_CHN_BURST_SIZE_32                   = 0x00000500,
    DMA_CHN_BURST_SIZE_64                   = 0x00000600,
    DMA_CHN_BURST_SIZE_128                  = 0x00000700,
    DMA_CHN_BURST_SIZE_256                  = 0x00000800,
    DMA_CHN_BURST_SIZE_512                  = 0x00000900,
    DMA_CHN_BURST_SIZE_1024                 = 0x00000A00,
    
    DMA_CHN_SOURCE_BURST_WRAP_DISABLE       = 0x00000000,
    DMA_CHN_SOURCE_BURST_WRAP_ENABLE        = 0x00004000,
    
    DMA_CHN_DESTINATION_BURST_WRAP_DISABLE  = 0x00000000,
    DMA_CHN_DESTINATION_BURST_WRAP_ENABLE   = 0x00008000,    
    
    DMA_CHN_CHANNEL_PRIORITY_0              = 0x00030000,
    DMA_CHN_CHANNEL_PRIORITY_1              = 0x00020000,
    DMA_CHN_CHANNEL_PRIORITY_2              = 0x00010000,
    DMA_CHN_CHANNEL_PRIORITY_3              = 0x00000000,    
}dma_channel_cfg_t;
//! @}

//! \name DMA Channel Transfer configuration structure
//! @{
typedef enum {
    DMA_TSF_CFG_VALID_FLAG_ENABLE           = 0x00000001,
    DMA_TSF_CFG_VALID_FLAG_DISABLE          = 0x00000000,
    
    DMA_TSF_RELOAD_DISABLE                  = 0x00000000,
    DMA_TSF_RELOAD_ENABLE                   = 0x00000002,
    
    DMA_TSF_TRIGGER_SOFTWARE_SET_DISABLE    = 0x00000000,
    DMA_TSF_TRIGGER_SOFTWARE_SET_ENABLE     = 0x00000004,
    
    DMA_TSF_TRIGGER_AUTO_CLEAR_DISABLE      = 0x00000000,
    DMA_TSF_TRIGGER_AUTO_CLEAR_ENABLE       = 0x00000008,
    
    DMA_TSF_WIDTH_8BIT                      = 0x00000000,
    DMA_TSF_WIDTH_16BIT                     = 0x00000100,
    DMA_TSF_WIDTH_32BIT                     = 0x00000200,

    DMA_TSF_SOURCE_ADDR_NO_INCREMENT        = 0x00000000,
    DMA_TSF_SOURCE_ADDR_INC_1WIDTH          = 0x00001000,
    DMA_TSF_SOURCE_ADDR_INC_2WIDTH          = 0x00002000,
    DMA_TSF_SOURCE_ADDR_INC_4WIDTH          = 0x00003000,
    
    DMA_TSF_DESTINATION_ADDR_NO_INCREMENT   = 0x00000000,
    DMA_TSF_DESTINATION_ADDR_INC_1WIDTH     = 0x00004000,
    DMA_TSF_DESTINATION_ADDR_INC_2WIDTH     = 0x00008000,
    DMA_TSF_DESTINATION_ADDR_INC_4WIDTH     = 0x0000C000,
}em_transfer_cfg_t;
//! @}

//! \name DMA configuration structure
//! @{
typedef struct {
    uint32_t                        wChannelCFG;
    uint32_t                        wChannelXFERCFG;    
    dma_channel_desciptor_t*        ptSRAMBASE;
}dma_cfg_t;
//! @}

//! \name DMA Channel State enumeration
//! @{
typedef enum {
    STATE_ACTIVE                    = 0x00000000,
    STATE_BUSY                      = 0x00000001,
    STATE_TIRG                      = 0x00000002,
    STATE_VAIL                      = 0x00000003,
}em_state_t;
//! @}

//! \name SDMA user interface
//! @{
DEF_INTERFACE(dma_t)
    //! Initialize specified DMA channel
    bool        (*Init)             (uint_fast8_t chIndex, dma_cfg_t* ptCFG); 
    //! Get DMA channel State
    bool        (*GetChannelState)  (uint_fast8_t chIndex, em_state_t tState);
    //! Abort for specified DMA channel
    bool        (*Abort)            (uint16_t hwMSK);
    //! Enable specified DMA channel
    void        (*Enable)           (uint16_t hwMSK);
    //! Disable specified DMA channel
    void        (*Disable)          (uint16_t hwMSK);    
    //! Set Trigger for specified DMA channel
    void        (*Trigger)          (uint16_t hwMSK);  
    //! Reload for specified DMA channel
    void        (*Reload)           (uint_fast8_t chIndex);      
    //! Register Page
    dma_reg_t*  const RegPage;       
END_DEF_INTERFACE(dma_t)
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief DMA interface
extern const dma_t DMA[DMA_COUNT];

#ifdef DMA0_ENABLE_CHANNEL_COUNT  
#   if 0 != DMA0_ENABLE_CHANNEL_COUNT 
        extern dma_channel_desciptor_t tSRAMBASE[];
#   endif
#else  
        extern dma_channel_desciptor_t tSRAMBASE[];
#endif

/*============================ PROTOTYPES ====================================*/
/*! \brief Link DMA Transfer as LoopBack
 *! \param tCFG DMA Desciptor 
 *! \return None
 */
extern void dma_link_loop_back(dma_channel_desciptor_t * tCFG, uint_fast16_t hwSize);

/*! \brief Auto Link DMA Transfer as LoopBack
 *! \param tCFG DMA Desciptor 
 *! \return None
 */
extern void dma_auto_link(dma_channel_desciptor_t  *ptCFG, 
    uint32_t *pwSrc, uint32_t *pDstAddr[], uint32_t wSize, uint32_t wXFERCFG);

#endif
/* EOF */
