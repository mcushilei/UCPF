/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\i_io_dma.h"
#include "..\device.h"
#include "..\pm\pm.h"

/*============================ MACROS ========================================*/
#ifndef DMA0_ENABLE_CHANNEL_COUNT
#define DMA0_ENABLE_CHANNEL_COUNT 16
#endif

//!< DMA Channel 
#define __DMA_CHANNEL(__N, __VALUE)     DMA_CHANNEL##__N = __N,
#define __DMA_CHANNEL_M(__N, __VALUE)   DMA_CHANNEL##__N##_MSK = (1ul<<(__N)),

//!< Memory/Periphral Configuration
#define DMA_TSF_MODE_MEMORY_TO_MEMORY       (DMA_TSF_SOURCE_ADDR_INC_1WIDTH      |      \
                                            DMA_TSF_DESTINATION_ADDR_INC_1WIDTH)

#define DMA_TSF_MODE_MEMORY_TO_PERIPHRAL    (DMA_TSF_SOURCE_ADDR_INC_1WIDTH      |      \
                                            DMA_TSF_DESTINATION_ADDR_NO_INCREMENT)

#define DMA_TSF_MODE_PERIPHRAL_TO_MEMORY    (DMA_TSF_SOURCE_ADDR_NO_INCREMENT    |      \
                                            DMA_TSF_DESTINATION_ADDR_INC_1WIDTH)

#define DMA_TSF_MODE_PERIPHRAL_TO_PERIPHRAL (DMA_TSF_SOURCE_ADDR_NO_INCREMENT    |      \
                                            DMA_TSF_SOURCE_ADDR_NO_INCREMENT)

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

#define SAFE_CLK_CODE(...)                                                      \
    {                                                                           \
        ahbclk_status_t tAHBclk =                                               \
            PM_GET_AHB_CLK_STATUS((em_ahb_clk_t)ptThis->tAHBClk);               \
        PM_AHB_CLK_ENABLE((em_ahb_clk_t)ptThis->tAHBClk);                       \
        __VA_ARGS__;                                                            \
        PM_AHB_CLK_RESUME((em_ahb_clk_t)ptThis->tAHBClk, tAHBclk);              \
    }

#define DMA_INTERFACE_MACRO(__N, __A)                                           \
    {                                                                           \
        &dma##__N##_init,                                                       \
        &dma##__N##_get_channel_state,                                          \
        &dma##__N##_abort,                                                      \
        &dma##__N##_enable,                                                     \
        &dma##__N##_disable,                                                    \
        &dma##__N##_trigger,                                                    \
        &dma##__N##_reload,                                                     \
        (dma_reg_t *)(DMA##__N##_BASE_ADDRESS)                                  \
    },
    
#define DMA_CLASS_MACRO(__N, __A)                                               \
    {                                                                           \
        (dma_reg_t *)(DMA##__N##_BASE_ADDRESS),                                 \
        AHBCLK_DMA                                                              \
    },    
    
#define DMA_FUNCTON_MACRO(__N, __A)                                             \
    bool dma##__N##_init(uint_fast8_t chIndex, dma_cfg_t* ptCFG)                \
    {                                                                           \
        return dma_init((__dma_t *)&__DMA[__N], chIndex, ptCFG);                \
    }                                                                           \
                                                                                \
    bool dma##__N##_get_channel_state(uint_fast8_t chIndex, em_state_t tState)  \
    {                                                                           \
        return dma_get_channel_state((__dma_t *)&__DMA[__N], chIndex, tState);  \
    }                                                                           \
                                                                                \
    bool dma##__N##_abort(uint16_t hwMSK)                                       \
    {                                                                           \
        return dma_abort((__dma_t *)&__DMA[__N], hwMSK);                        \
    }                                                                           \
                                                                                \
    void dma##__N##_enable(uint16_t hwMSK)                                      \
    {                                                                           \
        dma_enable((__dma_t *)&__DMA[__N], hwMSK);                              \
    }                                                                           \
                                                                                \
    void dma##__N##_disable(uint16_t hwMSK)                                     \
    {                                                                           \
        dma_disable((__dma_t *)&__DMA[__N], hwMSK);                             \
    }                                                                           \
                                                                                \
    void dma##__N##_trigger(uint16_t hwMSK)                                     \
    {                                                                           \
        dma_trigger((__dma_t *)&__DMA[__N], hwMSK);                             \
    }                                                                           \
    void dma##__N##_reload(uint_fast8_t chIndex)                                \
    {                                                                           \
        dma_reload((__dma_t *)&__DMA[__N], chIndex);                            \
    }               

#define DMA_EXTERN_FUNCTON_MACRO(__N, __A)                                      \
    extern bool dma##__N##_init(uint_fast8_t chIndex, dma_cfg_t* ptCFG);        \
    extern bool dma##__N##_get_channel_state(                                   \
        uint_fast8_t chIndex, em_state_t tState);                               \
    extern bool dma##__N##_abort(uint16_t hwMSK);                               \
    extern void dma##__N##_enable(uint16_t hwMSK);                              \
    extern void dma##__N##_disable(uint16_t hwMSK);                             \
    extern void dma##__N##_trigger(uint16_t hwMSK);                             \
    extern void dma##__N##_reload(uint_fast8_t chIndex);
/*============================ MACROFIED FUNCTIONS ===========================*/            

/*============================ TYPES =========================================*/

//! \name DMA Channel Base Configuration structure
//! @{
typedef enum {
    DMA_CHN_PERIPHERAL_REQUEST_DISABLE      = 0x00000000,
    DMA_CHN_PERIPHERAL_REQUEST_ENABLE       = 0x00000001,
    
    DMA_CHN_HARDWARE_TRIGGERING_ENABLE      = 0x00000000,
    DMA_CHN_HARDWARE_TRIGGERING_DISABLE     = 0x00000002,
    
    
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

//! \name DMA Configuration structure
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

//! \name internal class
//! @{
DEF_CLASS(__dma_t)
    dma_reg_t*  const ptREG;
    uint8_t     tAHBClk;
END_DEF_CLASS(__dma_t)
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

/*============================ PROTOTYPES ====================================*/

/*! \brief Initialize specified DMA channel
 *! \param 
 *! \retval true initialization succeed
 *! \retval false initialization fail
 */
static bool dma_init(__dma_t * ptDMA, uint_fast8_t chIndex, dma_cfg_t* ptCFG);

/*! \brief Abort for specified DMA channel
 *! \param hwMSK DMA channel MSK
 *! \retval true initialization succeed
 *! \retval false initialization fail
 */
static fsm_rt_t dma_abort(__dma_t * ptDMA, uint16_t hwMSK);

/*! \brief get dma channel state
 *! \param hwMSK access MSK
 *! \retval true dma
 */
static bool dma_get_channel_state(__dma_t * ptDMA, uint_fast8_t chIndex, em_state_t tState);

/*! \brief dma enable
 *! \param hwMSK access MSK
 *! \return none
 */
static void dma_enable(__dma_t * ptDMA, uint16_t hwMSK);

/*! \brief dma disable
 *! \param hwMSK access MSK
 *! \return none
 */
static void dma_disable(__dma_t * ptDMA, uint16_t hwMSK);

/*! \brief Set Trigger for specified DMA channel
 *! \param hwMSK DMA channel MSK
 *! \return none
 */
static void dma_trigger(__dma_t * ptDMA, uint16_t hwMSK);

/*! \brief dma reload
 *! \param none
 *! \return void
 */
static void dma_reload(__dma_t * ptDMA, uint_fast8_t chIndex);

MREPEAT(DMA_COUNT, DMA_EXTERN_FUNCTON_MACRO, 0)

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief DMA interface
const dma_t DMA[] = {   
    MREPEAT(DMA_COUNT, DMA_INTERFACE_MACRO, 0)
};

static CLASS(__dma_t) __DMA[] = {
    MREPEAT(DMA_COUNT, DMA_CLASS_MACRO, 0)
};

#if 0 != DMA0_ENABLE_CHANNEL_COUNT
#if __IS_COMPILER_IAR__
#pragma data_alignment=0x400
SECTION(".app_ram_noinit")
//#pragma location=".app_ram_noinit"
#endif
NO_INIT dma_channel_desciptor_t tSRAMBASE[DMA0_ENABLE_CHANNEL_COUNT];
#endif

/*============================ IMPLEMENTATION ================================*/
    
/*! \brief Initialize specified DMA channel
 *! \param 
 *! \retval true initialization succeed
 *! \retval false initialization fail
 */
static bool dma_init(__dma_t * ptDMA, uint_fast8_t chIndex, dma_cfg_t* ptCFG)
{  
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    if ((NULL == ptDMA) && (NULL == ptCFG)) {
        return false;
    }
    
    //ahbclk_status_t tAHBclk = PM_GET_AHB_CLK_STATUS((em_ahb_clk_t)ptThis->tAHBClk);
    //PM_AHB_CLK_ENABLE((em_ahb_clk_t)ptThis->tAHBClk);
    
    SAFE_CLK_CODE(
        ptThis->ptREG->Channel[chIndex].CFG.Value       
            = ptCFG->wChannelCFG ^ (DMA_CHN_HARDWARE_TRIGGERING_DISABLE | 
                                    DMA_CHN_TRIG_FALLING_EDGE_SENSITIVE |
                                    DMA_CHN_CHANNEL_PRIORITY_0);     
        ptThis->ptREG->Channel[chIndex].XFERCFG.Value   
            = ptCFG->wChannelXFERCFG;            
        ptThis->ptREG->SRAMBASE.Value                   
            = (uint32_t)ptCFG->ptSRAMBASE & DMA_SRAMBASE_OFFSET_MSK;
        
        ptThis->ptREG->ENABLESET.Value = 1ul << chIndex;     
        ptThis->ptREG->CONTRL.Value = DMA_CONTRL_ENABLE_MSK;
    )
 
    //PM_AHB_CLK_RESUME(AHBCLK_DMA,tAHBclk);
    return true;
}

/*! \brief Abort for specified DMA channel
 *! \param hwMSK DMA channel MSK
 *! \retval true abort succeed
 *! \retval false abort fail
 */
static fsm_rt_t dma_abort(__dma_t * ptDMA, uint16_t hwMSK)
{
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    fsm_rt_t tReturn = fsm_rt_on_going;
    
    if (NULL == ptDMA) {
        return fsm_rt_err;
    }
    
    SAFE_CLK_CODE(
        ptThis->ptREG->ENABLECLR.Value = hwMSK;

        if (0 == (ptThis->ptREG->BUSY.Value & hwMSK)) {
            ptThis->ptREG->ABORT.Value     = hwMSK;
            ptThis->ptREG->ENABLESET.Value = hwMSK;
            tReturn = fsm_rt_cpl;
        } 
    )
    
    return tReturn;
}

/*! \brief get dma channel state
 *! \param hwMSK access MSK
 *! \retval DMA Channel State
 */
static bool dma_get_channel_state(
    __dma_t * ptDMA, uint_fast8_t chIndex, em_state_t tState)
{
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    bool bRetVal = false;
    
    if (NULL == ptDMA) {
        return false;
    }    
  
    SAFE_CLK_CODE(
        //!< switch different flag
        switch (tState) {
            case STATE_ACTIVE:
                if (ptThis->ptREG->ACTIVE.Value & (1ul << chIndex)) {
                    bRetVal = true;
                }
                break;
                
            case STATE_BUSY:
                if (ptThis->ptREG->BUSY.Value & (1ul << chIndex)) {
                    bRetVal = true;
                }            
                break;
                
            case STATE_TIRG:
                if (    ptThis->ptREG->Channel[chIndex].CTLSTAT.Value 
                    &   DMA_CHANNEL_CTLSTAT_TRIG_MSK) {
                    bRetVal = true;
                }         
                break;

            case STATE_VAIL:
                if (    ptThis->ptREG->Channel[chIndex].CTLSTAT.Value 
                    &   DMA_CHANNEL_CTLSTAT_VALIDPENDING_MSK) {
                    bRetVal = true;
                }               
                break;            
        }
    )
    
    return bRetVal;
}

/*! \brief dma enable
 *! \param hwMSK access MSK
 *! \return none
 */
static void dma_enable(__dma_t * ptDMA, uint16_t hwMSK)
{        
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    PM_AHB_CLK_ENABLE((em_ahb_clk_t)ptThis->tAHBClk);
    ptThis->ptREG->ENABLESET.Value = hwMSK;
}

/*! \brief dma disable
 *! \param hwMSK access MSK
 *! \return none
 */
static void dma_disable(__dma_t * ptDMA, uint16_t hwMSK)
{
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    ptThis->ptREG->ENABLECLR.Value = hwMSK;
    PM_AHB_CLK_DISABLE((em_ahb_clk_t)ptThis->tAHBClk);
}

/*! \brief Set Trigger for specified DMA channel
 *! \param hwMSK DMA channel MSK
 *! \return None
 */
static void dma_trigger(__dma_t * ptDMA, uint16_t hwMSK)
{
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    
    SAFE_CLK_CODE(
        ptThis->ptREG->SETTRIG.Value = hwMSK;
    )
}

/*! \brief Reload for specified DMA channel
 *! \param chIndex DMA channel
 *! \return None
 */
static void dma_reload(__dma_t * ptDMA, uint_fast8_t chIndex)
{
    CLASS(__dma_t) *ptThis = (CLASS(__dma_t) *)ptDMA;
    
    SAFE_CLK_CODE(
        ptThis->ptREG->Channel[chIndex].XFERCFG.Value 
            = tSRAMBASE[chIndex].wXFERCFG;;
    )
}

MREPEAT(DMA_COUNT, DMA_FUNCTON_MACRO, 0)

/*! \brief Link DMA Transfer as LoopBack
 *! \param tCFG DMA Desciptor 
 *! \return None
 */
void dma_link_loop_back(dma_channel_desciptor_t * tCFG, uint_fast16_t hwSize)
{
    for (uint_fast16_t i = 0; i < hwSize; i++) {                                            
        tCFG[i].pLinkToNext = &tCFG[(i+1) % hwSize];                                
    }     
}

/*! \brief Auto Link DMA Transfer as LoopBack
 *! \param tCFG DMA Desciptor 
 *! \return None
 */
void dma_auto_link(dma_channel_desciptor_t  *ptCFG, 
                   uint32_t                 *pwSrc, 
                   uint32_t                 *pDstAddr[], 
                   uint32_t                 wSize,
                   uint32_t                 wXFERCFG)
{
    wXFERCFG &= DMA_TSF_TRIGGER_AUTO_CLEAR_ENABLE;          //!< check trigger clear mode
    wXFERCFG |= DMA_TSF_CFG_VALID_FLAG_ENABLE           |   //!< enable configuration mode
                DMA_TSF_RELOAD_ENABLE                   ;   //!< enable reload mode
    
    for (uint_fast16_t i = 0; i < wSize; i++) {                                            
        ptCFG[i].wXFERCFG         = wXFERCFG;
        ptCFG[i].pSourceAddr      = &pwSrc[i];
        ptCFG[i].pDestinationAddr = pDstAddr[i];
        ptCFG[i].pLinkToNext      = &ptCFG[(i+1) % wSize];     
    } 
    
    ptCFG[wSize-1].wXFERCFG = wXFERCFG | DMA_TSF_TRIGGER_AUTO_CLEAR_ENABLE;
}


/* EOF */
