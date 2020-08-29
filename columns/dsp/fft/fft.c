/*******************************************************************************
 *  Copyright(C)2017-2018 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include <math.h>
#include ".\fft.h"

/*============================ MACROS ========================================*/
#ifndef
#define PI                  (3.14159265358979)
#endif

#define FFT_SAMPLE_NUM      (1u << FFT_BIT_LEN)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


float    FFT_SIN_TBL[FFT_BIT_LEN];
float    FFT_COS_TBL[FFT_BIT_LEN];
uint16_t FFT_BIT_TBL[FFT_SAMPLE_NUM];

void fft_init(void)
{
    uint32_t i, x;

    for (i = 0; i < FFT_SAMPLE_NUM; i++) {
        x = i;
        x = ((x & 0x55555555u) << 1) | (((0x55555555u << 1) & x) >> 1);
        x = ((x & 0x33333333u) << 2) | (((0x33333333u << 2) & x) >> 2);
        x = ((x & 0x0F0F0F0Fu) << 4) | (((0x0F0F0F0Fu << 4) & x) >> 4);
        x = ((x & 0x00FF00FFu) << 8) | (((0x00FF00FFu << 8) & x) >> 8);
        x = ((x & 0x0000FFFFu) << 16) | (((0x0000FFFFu << 16) & x) >> 16);
        FFT_BIT_TBL[i] = x >> (32u - FFT_BIT_LEN);
    }

    for (i = 0; i < FFT_BIT_LEN; i++) {
        FFT_SIN_TBL[i] = sin(PI / (1u << i));
        FFT_COS_TBL[i] = cos(PI / (1u << i));
    }
}

void fft(float_complex_t xin[FFT_SAMPLE_NUM])
{
    uint32_t    i, j, le, li, ip, in;
    float_complex_t   c, w, t;

    //! 变址运算，即把自然顺序变成倒位序
    for (i = 0; i < FFT_SAMPLE_NUM; i++) {
        j = FFT_BIT_TBL[i];
        if (i < j) {
            t.Real = xin[j].Real;
            xin[j].Real = xin[i].Real;
            xin[i].Real = t.Real;
        }
        xin[j].Imag = 0.0f;
    }

    //! 蝶形级数: FFT_BIT_LEN = log(2)FFT_SAMPLE_NUM
    for (i = 0; i < FFT_BIT_LEN; i++) {
        le = 1u << (i + 1);                 //!< le表示蝶形大小: 2^(i+1)个点
        li = le >> 1;                       //!< li表示同一蝶形中参加运算的两点的距离(蝶形结个数): le/2 = 2^(i)个点

        //!< c为蝶形结运算系数，初始值为1.0
        c.Real = 1.0f;
        c.Imag = 0.0f;
        //!< w为旋转因子
        w.Real =  FFT_COS_TBL[i];           //!< cos(PI/li) = cos(PI/(2^(i)));
        w.Imag = -FFT_SIN_TBL[i];           //!< sin(PI/li) = sin(PI/(2^(i)));

        for (j = 0; j < li; j++) {                          //!< 同一蝶形中不同位置的蝶形结（系数c不同）
            for (ip = j; ip < FFT_SAMPLE_NUM; ip += le) {   //!< 不同蝶形中相同位置的蝶形结
                in = ip + li;                               //!< ip，in分别表示蝶形结中参加运算的的两个点
                //!< 蝶形运算
                COMPLEX_MULTIPLY(t,       xin[in], c);      //!  t       = xin[in] * c;
                COMPLEX_SUB     (xin[in], xin[ip], t);      //!  xin[in] = xin[ip] - t;
                COMPLEX_ADD     (xin[ip], xin[ip], t);      //!  xin[ip] = xin[ip] + t;
            }
            COMPLEX_MULTIPLY(c, c, w);                      //!< c = c * w;
        }
    }
}





// This gets used with the Kaiser window.
static float bessel(float x)
{
    float sum, XtoIpower;
    uint32_t i, j, Factorial;

    sum = 0.0f;
    for (i = 1; i < 10; i++) {
        XtoIpower = pow(x / 2.0f, i);
        Factorial = 1;
        for (j = 1; j <= i; j++) {
            Factorial *= j;
        }
        sum += pow(XtoIpower / Factorial, 2);
    }

    return 1.0f + sum;
}

// This gets used with the Sinc window and various places in the BasicFIR function.
static float sinc(float x)
{
    if (x > -1.0E-5 && x < 1.0E-5) {
        return 1.0f;
    } else {
        return sin(x) / x;
    }
}


//---------------------------------------------------------------------------
// These are the various windows definitions. These windows can be used for either
// FIR filter design or with an FFT for spectral analysis.
// Sourced verbatim from: ~MyDocs\Code\Common\FFTFunctions.cpp
// For definitions, see this article:  http://en.wikipedia.org/wiki/Window_function

// This function has 6 inputs
// Data is the array, of length N, containing the data to to be windowed. 
// This data is either a FIR filter sinc pulse, or the data to be analyzed by an fft.

// windowType is an enum defined in the header file, which is at the bottom of this file.
// e.g. WINDOW_TYPE_KAISER, WINDOW_TYPE_SINC, WINDOW_TYPE_HANNING, WINDOW_TYPE_HAMMING, WINDOW_TYPE_BLACKMAN, ...

// Alpha sets the width of the flat top.
// Windows such as the Tukey and Trapezoid are defined to have a variably wide flat top.
// As can be seen by its definition, the Tukey is just a Hanning window with a flat top.
// Alpha can be used to give any of these windows a partial flat top, except the Flattop and Kaiser.
// Alpha = 0 gives the original window. (i.e. no flat top)
// To generate a Tukey window, use a Hanning with 0 < Alpha < 1
// To generate a Bartlett window (triangular), use a Trapezoid window with Alpha = 0.
// Alpha = 1 generates a rectangular window in all cases. (except the Flattop and Kaiser)

// Beta is used with the Kaiser, Sinc, and Sine windows only.
// These three windows are primarily used for FIR filter design, not spectral analysis.
// In FIR filter design, Beta controls the filter's transition bandwidth and the sidelobe levels.
// The code ignores Beta except in the Kaiser, Sinc, and Sine window cases.

// unityGain controls whether the gain of these windows is set to unity.
// Only the Flattop window has unity gain by design. The Hanning window, for example, has a gain
// of 1/2.  unityGain = true will set the gain of all these windows to 1.
// Then, when the window is applied to a signal, the signal's energy content is preserved.
// Don't use this with FIR filter design however. Since most of the enegy in an FIR sinc pulse
// is in the middle of the window, the window needs a peak amplitude of one, not unity gain.
// Setting unityGain = true will simply cause the resulting FIR filter to have excess gain.

// If using these windows for FIR filters, start with the Kaiser, Sinc, or Sine windows and
// adjust Beta for the desired transition BW and sidelobe levels (set Alpha = 0).
// While the FlatTop is an excellent window for spectral analysis, don't use it for FIR filter design.
// It has a peak amplitude of ~ 4.7 which causes the resulting FIR filter to have about this much gain.
// It works poorly for FIR filters even if you adjust its peak amplitude.
// The Trapezoid also works poorly for FIR filter design.

// If using these windows with an fft for spectral analysis, start with the Hanning, Gauss, or Flattop.
// When choosing a window for spectral analysis, you must trade off between resolution and amplitude accuracy.
// The Hanning has the best resolution while the Flatop has the best amplitude accuracy.
// The Gauss is midway between these two for both accuracy and resolution.
// These three were the only windows available in the HP 89410A Vector Signal Analyzer. Which is to say,
// unless you have specific windowing requirements, use one of these 3 for general purpose signal analysis.
// Set unityGain = true when using any of these windows for spectral analysis to preserve the signal's enegy level.

void window_coeff(float *pBuff, uint32_t buffSize, uint8_t windowType, float Alpha, float Beta, bool unityGain)
{
    uint32_t j, M, hM, dM, topWidth;

    if (windowType == WINDOW_TYPE_KAISER || windowType == WINDOW_TYPE_FLATTOP) {
        Alpha = 0.0f;
    }

    if (Alpha < 0.0f) {
        Alpha = 0.0f;
    }
    if (Alpha > 1.0f) {
        Alpha = 1.0f;
    }

    if (Beta < 0.0f) {
        Beta = 0.0f;
    }
    if (Beta > 10.0f) {
        Beta = 10.0f;
    }

    topWidth = Alpha * buffSize;
    if (buffSize % 2u != 0u) {
        if (topWidth % 2u == 0u) {
            topWidth++;
        }
    } else {
        if (topWidth % 2u != 0u) {
            topWidth++;
        }
    }
    if (topWidth > buffSize) {
        topWidth = buffSize;
    }
    M = buffSize - topWidth;     //! note, M is even.
    hM = M / 2u;
    dM = M + 1u;


    // Calculate the window for buffSize / 2 points, then fold the window over (at the bottom).
    // topWidth points will be set to 1. (at the bottom)
    switch (windowType) {
        case WINDOW_TYPE_KAISER:
            for (j = 0; j < M; j++) {
                float Arg = Beta * (float)sqrt(1.0f - (float)pow((2 * j + 2) / (float)dM - 1.0f, 2));
                pBuff[j] = bessel(Arg) / bessel(Beta);
            }
            break;

        case WINDOW_TYPE_SINC:    // Lanczos
            for (j = 0; j < M; j++) {
                pBuff[j] = sinc((2 * j + 1 - (float)M) * PI / dM);
                pBuff[j] = pow(pBuff[j], Beta);
            }
            break;

        case WINDOW_TYPE_SINE:    // Hanning if Beta = 2
            for (j = 0; j < hM; j++) {
                pBuff[j] = sin((j + 1) * PI / dM);
                pBuff[j] = pow(pBuff[j], Beta);
            }
            break;

        case WINDOW_TYPE_HANNING:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.5f - 0.5f * (float)cos((j + 1) * 2 * PI / dM);
            }
            break;

        case WINDOW_TYPE_HAMMING:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.54f - 0.46f * (float)cos((j + 1) * 2 * PI / dM);
            }
            break;

        case WINDOW_TYPE_BLACKMAN:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.42f
                    - 0.50f * (float)cos((j + 1) * 2 * PI / dM)
                    + 0.08f * (float)cos((j + 1) * 2 * PI * 2.0f / dM);
            }
            break;

        case WINDOW_TYPE_FLATTOP:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 1.0f
                    - 1.93293488969227f * (float)cos((j + 1) * 2 * PI / dM)
                    + 1.28349769674027f * (float)cos((j + 1) * 2 * PI * 2.0f / dM)
                    - 0.38130801681619f * (float)cos((j + 1) * 2 * PI * 3.0f / dM)
                    + 0.02929730258511f * (float)cos((j + 1) * 2 * PI * 4.0f / dM);
            }
            break;

        case WINDOW_TYPE_BLACKMAN_HARRIS:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.35875f
                    - 0.48829f * (float)cos((j + 1) * 2 * PI / dM)
                    + 0.14128f * (float)cos((j + 1) * 2 * PI * 2.0f / dM)
                    - 0.01168f * (float)cos((j + 1) * 2 * PI * 3.0f / dM);
            }
            break;

        case WINDOW_TYPE_BLACKMAN_NUTTALL:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.3535819f
                    - 0.4891775f * (float)cos((j + 1) * 2 * PI / dM)
                    + 0.1365995f * (float)cos((j + 1) * 2 * PI * 2.0 / dM)
                    - 0.0106411f * (float)cos((j + 1) * 2 * PI * 3.0 / dM);
            }
            break;

        case WINDOW_TYPE_NUTTALL:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.355768f
                    - 0.487396f * (float)cos((j + 1) * 2 * PI / dM)
                    + 0.144232f * (float)cos((j + 1) * 2 * PI * 2.0f / dM)
                    - 0.012604f * (float)cos((j + 1) * 2 * PI * 3.0f / dM);
            }
            break;

        case WINDOW_TYPE_KAISER_BESSEL:
            for (j = 0; j < hM; j++) {
                pBuff[j] = 0.402f
                    - 0.498f * (float)cos(       2 * PI * (j + 1) / dM)
                    + 0.098f * (float)cos(2.0f * 2 * PI * (j + 1) / dM)
                    + 0.001f * (float)cos(3.0f * 2 * PI * (j + 1) / dM);
            }
            break;

        case WINDOW_TYPE_TRAPEZOID:   // Rectangle for Alpha = 1  Triangle for Alpha = 0
            for (j = 0; j < hM; j++) {
                pBuff[j] = (j + 1u) / (float)hM;
            }
            break;

        case WINDOW_TYPE_GAUSS:
            // This definition is from http://en.wikipedia.org/wiki/Window_function (Gauss Generalized normal window)
            // We set their p = 2, and use Alpha in the numerator, instead of Sigma in the denominator, as most others do.
            // Alpha = 2.718 puts the Gauss window response midway between the Hanning and the Flattop (basically what we want).
            // It also gives the same BW as the Gauss window used in the HP 89410A Vector Signal Analyzer.
            // Alpha = 1.8 puts it quite close to the Hanning.
            for (j = 0; j < hM; j++) {
                pBuff[j]  = ((j + 1) - dM / 2.0f) / (dM / 2.0f) * 2.7183f;
                pBuff[j] *= pBuff[j];
                pBuff[j]  = exp(-pBuff[j]);
            }
            break;

        default:      // Error.
            return;
    }

    // Fold the coefficients over.
    for (j = 0; j < hM; j++) {
        pBuff[buffSize - j - 1] = pBuff[j];
    }

    // This is the flat top if Alpha > 0. Cannot be applied to a Kaiser or Flat Top.
    if (windowType != WINDOW_TYPE_KAISER &&  windowType != WINDOW_TYPE_FLATTOP) {
        for (j = hM; j < topWidth; j++) {
            pBuff[j] = 1.0f;
        }
    }

    // This will set the gain of the window to 1.
    if (unityGain) {
        double Sum = 0.0;
        for (j = 0; j < buffSize; j++) {
            Sum += pBuff[j];
        }
        Sum /= buffSize;
        if (Sum != 0.0) {
            for (j = 0; j < buffSize; j++) {
                pBuff[j] /= Sum;
            }
        }
    }
}

/* EOF */
