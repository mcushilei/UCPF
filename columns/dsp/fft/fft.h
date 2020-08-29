/*******************************************************************************
 *  Copyright(C)2017-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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


#ifndef __DSP_FFT_H__
#define __DSP_FFT_H__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
    float Real;
    float Imag;
} float_complex_t;

enum {
    WINDOW_TYPE_NONE,
    WINDOW_TYPE_KAISER,
    WINDOW_TYPE_SINC,
    WINDOW_TYPE_SINE,
    WINDOW_TYPE_HANNING,
    WINDOW_TYPE_HAMMING,
    WINDOW_TYPE_BLACKMAN,
    WINDOW_TYPE_FLATTOP,
    WINDOW_TYPE_BLACKMAN_HARRIS,
    WINDOW_TYPE_BLACKMAN_NUTTALL,
    WINDOW_TYPE_NUTTALL,
    WINDOW_TYPE_KAISER_BESSEL,
    WINDOW_TYPE_TRAPEZOID,
    WINDOW_TYPE_GAUSS,
    WINDOW_TYPE_TEST
};

#define COMPLEX_MULTIPLY(__c, __a, __b) do {                    \
        float_complex_t __d;                                    \
        __d.Real = __a.Real * __b.Real - __a.Imag * __b.Imag;   \
        __d.Imag = __a.Real * __b.Imag + __a.Imag * __b.Real;   \
        __c = __d;                                              \
    } while (0)

#define COMPLEX_ADD(__c, __a, __b) do {     \
        __c.Real = __a.Real + __b.Real;     \
        __c.Imag = __a.Imag + __b.Imag;     \
    } while (0)

#define COMPLEX_SUB(__c, __a, __b) do {     \
        __c.Real = __a.Real - __b.Real;     \
        __c.Imag = __a.Imag - __b.Imag;     \
    } while (0)

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
extern void fft_init(void);
extern void fft(float_complex_t xin[FFT_SAMPLE_NUM]);
extern void window_coeff(float *pBuff, uint32_t buffSize, uint8_t windowType, float Alpha, float Beta, bool unityGain);

#endif  //! #ifndef __DSP_FFT_H__
/* EOF */
