/*******************************************************************************
 *  Copyright(C)2015 by Dreistein<mcu_shilei@hotmail.com>                     *
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

/*============================ MACROS ========================================*/

#define MAX_VOLT_SCALE (4)
#define MAX_TIME_SCALE (9)

#define SIZE_OF_GRADE  (12)     //!< in pixel

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct{
    uint8_t runState;
    uint8_t runStop;        //0:run 1:stop
    uint8_t channelSelect;

    uint8_t     YScaleIdx;
    uint32_t    ADCValuePerGrad[MAX_VOLT_SCALE];
    uint32_t    ADCValuePerPixel[MAX_VOLT_SCALE];
    
    uint8_t     XScaleIdx;
    uint32_t    timeScale[MAX_TIME_SCALE];
    char       *pTimeName[MAX_TIME_SCALE];
    uint32_t    timeDivCoef[MAX_TIME_SCALE]; 
    uint32_t    sampleRate;
    
    char        pFFTSampRate[8];
    char        pFFTDiv[8];
    char        pFFTMax[8];
} SCOPE_CTRL, *PSCOPE_CTRL;


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
THIS_FILE_NAME("template");


SCOPE_CTRL g_scopeCtrl;

float g_pVolScale[MAX_VOLT_SCALE]      = {0.1, 0.4, 0.8, 1.0};
char *g_pVolScaleName[MAX_VOLT_SCALE]  = {"0.1v", "0.4v", "0.8v", "1.0v"};

uint32_t pTime[MAX_TIME_SCALE]         = {10, 20, 50, 100, 200, 500, 1000, 2000, 2500}; //单位:us
char *g_pTimeScaleName[MAX_TIME_SCALE] = {"10us", "20us", "50us", "100u", "200u", "500u", "1.0m", "2.0m", "2.5m"};

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
extern volatile uint32_t sysTickCounter;
uint8_t gram[128 * 8];

uint32_t ADC_Buf[256];


void OLED_DrawGraduate(uint8_t x, uint8_t y)
{
    GRAM_DrawPoint( gram, x, y,   1 );
    
    GRAM_DrawPoint( gram, x - 1, y + 1, 1 );
    GRAM_DrawPoint( gram, x,     y + 1, 1 );
    GRAM_DrawPoint( gram, x + 1, y + 1, 1 );
}

void OLED_ShowGrid(uint32_t waveOrFFt)
{
    uint8_t i,j;    

    //清除所有数据
    GRAM_Clear(gram);

    //wave
    if( waveOrFFt == 0 ) {
        //画竖刻度线, 12 pixel per grade.
        for(i = 0; i <= 108; i += SIZE_OF_GRADE) {
            for(j = 0; j <= 48; j += 3) {   // 0, 3, 6, ... 48
                GRAM_DrawPoint( gram, i, j, 1 );
            }
        }

        //画横刻度线, 12 pixel per grade.
        for(j = 0; j <= 48; j += SIZE_OF_GRADE) {
            for(i = 0; i <= 108; i += 3) {   // 0, 3, 6, ... 108
                GRAM_DrawPoint( gram, i, j, 1 );
            }
        }
    }
    //fft
    else {
        //画竖刻度线
        for(i = 2; i <= 66; i += 64) {
            for(j = 0; j <= 50; j += 2) {
                GRAM_DrawPoint( gram, i, j, 1 );
            }
        }

        //画横刻度线
        for(j = 0; j <= 50; j += 50) {
            for(i = 2; i <= 66; i += 2) {
                GRAM_DrawPoint( gram, i, j, 1);
            }
        }

        //画小三角形作为刻度标注
        for(i = 2; i <= 66; i += 16) {
            OLED_DrawGraduate(i, 52);
        }
    }
}

void SCOPE_InitVoltParam(void)
{
    uint32_t i;
    
    for(i = 0; i < MAX_VOLT_SCALE; i++) {
        //计算转换系数
        g_scopeCtrl.ADCValuePerGrad[i]  = (uint32_t)(g_pVolScale[i] / 3.3 * (1u << 16));    // 1986, 7945, 15888, 19859
        g_scopeCtrl.ADCValuePerPixel[i] = g_scopeCtrl.ADCValuePerGrad[i] / SIZE_OF_GRADE;
        
        DBG_LOG("g_scopeCtrl.ADCValuePerPixel[%u] = %u", i, g_scopeCtrl.ADCValuePerPixel[i]);
        DBG_LOG("g_scopeCtrl.ADCValuePerGrad[%u] = %u", i, g_scopeCtrl.ADCValuePerGrad[i]);
    }

    g_scopeCtrl.YScaleIdx = 0;
}

void SCOPE_InitTimeParam(void)
{
    uint32_t i;
    
    for(i = 0; i < MAX_TIME_SCALE; i++) {
        g_scopeCtrl.timeDivCoef[i] =  pTime[i] / 10; //ADC采样频率设置的分频系数
    }
    g_scopeCtrl.XScaleIdx = 2;
}

void draw_wave(void)
{
    int32_t trigStartPos = 8;
    int32_t trigEndPos = trigStartPos + 108;
    uint16_t i, ii, j;
    int32_t yPos, yPosLast;
    uint32_t autoVMRoffset = 0;
    uint32_t autoVMRoffsetIndex = 0;
    static uint16_t lastIndex = 0;
    static uint16_t ADC_TransBuf[128];
    uint32_t vMax = 0;
    uint32_t vMin = 0;
    
    vMax = 0x00000000;
    vMin = 0x7FFFFFFF;
    for( i = 0; i < 128; i++ ) {
        ADC_TransBuf[i] = ADC_Buf[i];
        
        if( ADC_TransBuf[i] > vMax ) {
            vMax = ADC_TransBuf[i];
        }

        if( ADC_TransBuf[i] < vMin ) {
            vMin = ADC_TransBuf[i];
        }
    }
    
    DBG_LOG("vMax = %u", vMax);
    DBG_LOG("vMin = %u", vMin);

    //自动量程
    autoVMRoffsetIndex = vMin / g_scopeCtrl.ADCValuePerGrad[g_scopeCtrl.YScaleIdx];

    //消除量程抖动
    if( (autoVMRoffsetIndex < lastIndex) && (lastIndex - autoVMRoffsetIndex <= 1) ) {
        autoVMRoffsetIndex = lastIndex;
    }
    
    autoVMRoffset = autoVMRoffsetIndex *  g_scopeCtrl.ADCValuePerGrad[g_scopeCtrl.YScaleIdx];
    
    DBG_LOG("autoVMRoffset = %u", autoVMRoffset);
    
    OLED_ShowGrid(0);
    for( i = 0, ii = trigStartPos; ii < trigEndPos; i++, ii++ ) {
        
        uint16_t tempVal = (ADC_TransBuf[ii] - autoVMRoffset) / g_scopeCtrl.ADCValuePerPixel[g_scopeCtrl.YScaleIdx];

        if( tempVal > 48 ) {
            tempVal = 48;
        }

        yPos = 48 - tempVal;

        //tempVal为48时（即yPos为0）不显示，表示实际值超出显示范围
        if( yPos > 0 ) {
            GRAM_DrawPoint( gram, i, yPos, 1);
        }

        //y轴插值，使波形看起来是连续的(尤其当相邻两个点在Y方向上距离很远时)
        if( i == 0 ) {
            yPosLast = yPos;
        }
        else {
            if(yPos > (yPosLast + 1)) {
                for(j = yPosLast + 1; j < yPos; j++) {
                    GRAM_DrawPoint( gram, i, j, 1 );
                }
            }
            else if(yPosLast > (yPos + 1)) {
                for(j = yPosLast - 1; j > yPos; j--) {
                    GRAM_DrawPoint( gram, i, j, 1 );
                }
            }
            yPosLast = yPos;
        }
    }
}

/*! \note deinitialize application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 *  \retval false hal deinitialization failed
 */
int app_main(void)
{
    deadline_t delay;
    
    deadline_init(&delay);
    SSD1306_Init();
    SSD1306_DisplayInit();
    
    DBG_LOG("Core Clock: %u", core_clock_get());

    memset(gram, 0xF0F0F0F0, sizeof(gram) / 2);
    memset(gram + sizeof(gram) / 2, 0x0F0F0F0F, sizeof(gram) / 2);
    
    //while (1) {
        deadline_set_ms(&delay, 2000);
        SSD1306_Refresh(gram);
        while( !deadline_is_expired(&delay) );
    //}
    
        
    SCOPE_InitVoltParam();
    SCOPE_InitTimeParam();
    SSD1306_Refresh(gram);
    
    
    DMA_ADC_Setup(ADC_Buf, 128);
    App_ADC_Init();
    App_ADC_SetSampleRate(g_scopeCtrl.timeDivCoef[g_scopeCtrl.XScaleIdx]);

    while( 1 ) {
        if( DMA_IntA_Flag ) {
            DMA_IntA_Flag = 0;
            App_ADC_Start();
            DBG_LOG("ADC DMA done.");
            draw_wave();
            SSD1306_Refresh(gram);
        }
    }
    
    return 0;
}




/**
 * @brief	Handle interrupt from ADC sequencer A
 * @return	Nothing
 */
ISR(ADC_SEQB_IRQHandler)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags( &ADC0_REG );

	/* Sequence B completion interrupt */
	if (pending & REG_ADC_FLAGS_SEQB_INT_M) {
        Chip_ADC_ClearFlags( &ADC0_REG, REG_ADC_FLAGS_SEQB_INT_M );
		DBG_LOG( "ADC B done." );
	}
}

static volatile uint32_t wCnt = 0;


ISR(WDT_IRQHandler)
{
    BREATH_LED_OFF();
}

ISR(RTC_IRQHandler)
{
#ifdef __LPC17XX__
    uint32_t reg;
    static uint32_t i = 0;
    
    reg = RTC_REG.ILR;
    RTC_REG.ILR = reg & (
           (1u << 0) 
         | (1u << 1));
    
    if (reg & (1u << 0)) {      //!< counter increment interrupt.
    }
    
    if (reg & (1u << 1)) {      //!< alarm interrupt.
    }
    
    DBG_LOG("\r\n%u", i);
    i++;
#elif defined(__LPC12XX__)
#elif defined(__LPC11XXX__)
    rtc_alarm_clear_intflag();

#endif
}

/* EOF */
