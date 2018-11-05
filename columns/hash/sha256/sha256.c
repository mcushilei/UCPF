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

/*!  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *!
 *!  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

//! \note do not move this pre-processor statement to other places
#define __SHA256_C__

/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"

/*============================ MACROS ========================================*/
#define SHA256_PROCESS(__C, __D)    sha256_process(__C, __D)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*! \brief 32-bit integer big endian manipulation macros
 */
#define SHA_GET_UINT32_BE(n, b)         \
do {                                    \
    uint32_t t;                         \
    uint8_t  *p = (uint8_t *)(b);       \
    t = ( (uint32_t)(p)[0] << 24 )      \
      | ( (uint32_t)(p)[1] << 16 )      \
      | ( (uint32_t)(p)[2] <<  8 )      \
      | ( (uint32_t)(p)[3]       );     \
    (n) = t;                            \
} while (0)

#define SHA_PUT_UINT32_BE(n, b)         \
do {                                    \
    uint32_t t = (n);                   \
    uint8_t  *p = (uint8_t *)(b);       \
    (p)[0] = ( t >> 24 ) & 0xFFu;       \
    (p)[1] = ( t >> 16 ) & 0xFFu;       \
    (p)[2] = ( t >>  8 ) & 0xFFu;       \
    (p)[3] = ( t       ) & 0xFFu;       \
} while (0)

/*============================ TYPES =========================================*/
/*! \brief          SHA-256 context structure
 */
typedef struct {
    uint32_t wTotal[2];         //!< number of bytes processed
    uint32_t wState[8];         //!< intermediate digest state
    uint8_t  chBuffer[64];      //!< data block being processed
} sha256_context_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*! \brief        SHA-256 context setup
 *!
 *! \param ptCtx  context to be initialized
 */
void sha256_starts(sha256_context_t *ptCtx)
{
    ptCtx->wTotal[0] = 0;
    ptCtx->wTotal[1] = 0;

    ptCtx->wState[0] = 0x6A09E667;
    ptCtx->wState[1] = 0xBB67AE85;
    ptCtx->wState[2] = 0x3C6EF372;
    ptCtx->wState[3] = 0xA54FF53A;
    ptCtx->wState[4] = 0x510E527F;
    ptCtx->wState[5] = 0x9B05688C;
    ptCtx->wState[6] = 0x1F83D9AB;
    ptCtx->wState[7] = 0x5BE0CD19;
}

#define SHFR(x, n) (x >> n)
#define ROTR(x, n) (SHFR(x, n) | (x << (32 - n)))

#define S0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))
#define S2(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) ((x & y) | (z & (x | y)))
#define F1(x, y, z) (z ^ (x & (y ^ z)))

#if defined(SHA256_OPT_SMALLER)
#   define R(t) (S1(W[(t - 2) & 0xFul]) + W[(t - 7) & 0xFul] + S0(W[(t - 15) & 0xFul]) + W[(t - 16) & 0xFul])
#else
#   define R(t) (S1(W[t - 2]) + W[t - 7] + S0(W[t - 15]) + W[t - 16])
#endif

#define P(a, b, c, d, e, f, g, h, x, k)         \
do {                                            \
    uint32_t t;                                 \
    t = h + S3(e) + F1(e, f, g) + x + k;        \
    d += t;                                     \
    h = t + S2(a) + F0(a, b, c);                \
} while (0)

/*! \brief          SHA-256 process
 *!
 *! \param ptCtx    SHA-256 context
 *! \param pchData  buffer holding the data, data mast be 64 bytes(512 bits) length
 */
#if defined(SHA256_OPT_SMALLER)
static void sha256_process(sha256_context_t *ptCtx, uint8_t *pchData)
#else
static void sha256_process(sha256_context_t *ptCtx, const uint8_t *pchData)
#endif
{
    static const uint32_t K256[64] = {
        0x428A2F98ul, 0x71374491ul, 0xB5C0FBCFul, 0xE9B5DBA5ul,
        0x3956C25Bul, 0x59F111F1ul, 0x923F82A4ul, 0xAB1C5ED5ul,
        0xD807AA98ul, 0x12835B01ul, 0x243185BEul, 0x550C7DC3ul,
        0x72BE5D74ul, 0x80DEB1FEul, 0x9BDC06A7ul, 0xC19BF174ul,
        0xE49B69C1ul, 0xEFBE4786ul, 0x0FC19DC6ul, 0x240CA1CCul,
        0x2DE92C6Ful, 0x4A7484AAul, 0x5CB0A9DCul, 0x76F988DAul,
        0x983E5152ul, 0xA831C66Dul, 0xB00327C8ul, 0xBF597FC7ul,
        0xC6E00BF3ul, 0xD5A79147ul, 0x06CA6351ul, 0x14292967ul,
        0x27B70A85ul, 0x2E1B2138ul, 0x4D2C6DFCul, 0x53380D13ul,
        0x650A7354ul, 0x766A0ABBul, 0x81C2C92Eul, 0x92722C85ul,
        0xA2BFE8A1ul, 0xA81A664Bul, 0xC24B8B70ul, 0xC76C51A3ul,
        0xD192E819ul, 0xD6990624ul, 0xF40E3585ul, 0x106AA070ul,
        0x19A4C116ul, 0x1E376C08ul, 0x2748774Cul, 0x34B0BCB5ul,
        0x391C0CB3ul, 0x4ED8AA4Aul, 0x5B9CCA4Ful, 0x682E6FF3ul,
        0x748F82EEul, 0x78A5636Ful, 0x84C87814ul, 0x8CC70208ul,
        0x90BEFFFAul, 0xA4506CEBul, 0xBEF9A3F7ul, 0xC67178F2ul,
    };
#if defined(SHA256_OPT_SMALLER)
    uint32_t *W = (uint32_t *)pchData;
#else
    uint32_t W[64];
#endif
    uint32_t A[8];
    uint32_t i;

    A[0] = ptCtx->wState[0];
    A[1] = ptCtx->wState[1];
    A[2] = ptCtx->wState[2];
    A[3] = ptCtx->wState[3];
    A[4] = ptCtx->wState[4];
    A[5] = ptCtx->wState[5];
    A[6] = ptCtx->wState[6];
    A[7] = ptCtx->wState[7];

#if defined(SHA256_OPT_SMALLER)
    for (i = 0; i < 16; i++) {
        uint32_t tmp;
        SHA_GET_UINT32_BE(W[i], pchData + 4 * i);
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i], K256[i]);

        tmp  = A[7]; 
        A[7] = A[6];
        A[6] = A[5];
        A[5] = A[4];
        A[4] = A[3];
        A[3] = A[2];
        A[2] = A[1];
        A[1] = A[0];
        A[0] = tmp;
    }
    for (; i < 64; i++) {
        uint32_t tmp;
        W[i & 0xFul] = R(i);
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i & 0xFul], K256[i]);

        tmp  = A[7]; 
        A[7] = A[6];
        A[6] = A[5];
        A[5] = A[4];
        A[4] = A[3];
        A[3] = A[2];
        A[2] = A[1];
        A[1] = A[0];
        A[0] = tmp;
    }
#else /* SHA256_OPT_SMALLER */
    for (i = 0; i < 16; i += 8) {
        SHA_GET_UINT32_BE(W[i + 0], pchData + 4 * (i + 0));
        SHA_GET_UINT32_BE(W[i + 1], pchData + 4 * (i + 1));
        SHA_GET_UINT32_BE(W[i + 2], pchData + 4 * (i + 2));
        SHA_GET_UINT32_BE(W[i + 3], pchData + 4 * (i + 3));
        SHA_GET_UINT32_BE(W[i + 4], pchData + 4 * (i + 4));
        SHA_GET_UINT32_BE(W[i + 5], pchData + 4 * (i + 5));
        SHA_GET_UINT32_BE(W[i + 6], pchData + 4 * (i + 6));
        SHA_GET_UINT32_BE(W[i + 7], pchData + 4 * (i + 7));
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i + 0], K256[i + 0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i + 1], K256[i + 1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i + 2], K256[i + 2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i + 3], K256[i + 3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i + 4], K256[i + 4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i + 5], K256[i + 5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i + 6], K256[i + 6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i + 7], K256[i + 7]);
    }
    for (; i < 64; i += 8) {
        W[i + 0] = R(i + 0);
        W[i + 1] = R(i + 1);
        W[i + 2] = R(i + 2);
        W[i + 3] = R(i + 3);
        W[i + 4] = R(i + 4);
        W[i + 5] = R(i + 5);
        W[i + 6] = R(i + 6);
        W[i + 7] = R(i + 7);
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i + 0], K256[i + 0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i + 1], K256[i + 1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i + 2], K256[i + 2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i + 3], K256[i + 3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i + 4], K256[i + 4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i + 5], K256[i + 5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i + 6], K256[i + 6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i + 7], K256[i + 7]);
    }
#endif /* SHA256_OPT_SMALLER */

    ptCtx->wState[0] += A[0];
    ptCtx->wState[1] += A[1];
    ptCtx->wState[2] += A[2];
    ptCtx->wState[3] += A[3];
    ptCtx->wState[4] += A[4];
    ptCtx->wState[5] += A[5];
    ptCtx->wState[6] += A[6];
    ptCtx->wState[7] += A[7];
}

static void sha256_mem_copy(uint8_t *pD, const uint8_t *pS, uint32_t wN)
{
    for (; wN; --wN) {
        *pD = *pS;
        pD++;
        pS++;
    }
}

/*! \brief          SHA-256 buffer data and process
 *!
 *! \param ptCtx    SHA-256 context
 *! \param pchData  buffer holding the data
 *! \param wLen     length of the input data
 */
void sha256_update( sha256_context_t    *ptCtx,
                    const uint8_t       *pchData,
                    uint32_t            wLen)
{
    uint32_t gapn;
    uint32_t last;

    if (wLen == 0) {
        return;
    }

    //! try aligning to 64(512 bits)
    last = ptCtx->wTotal[0] & 0x3F;
    gapn = 64 - last;

    //! add data length
    ptCtx->wTotal[0] += (uint32_t) wLen;
    if (ptCtx->wTotal[0] < (uint32_t)wLen) {    //!< overflow
        ptCtx->wTotal[1]++;
    }

    //! fill gap if present
    if (last && wLen >= gapn) {
        sha256_mem_copy(ptCtx->chBuffer + last, pchData, gapn);
        SHA256_PROCESS(ptCtx, ptCtx->chBuffer);
        pchData += gapn;
        wLen  -= gapn;
        last = 0;
    }

    //! process aligned data
    for (; wLen >= 64; wLen -= 64) {
#if defined(SHA256_OPT_SMALLER)
        sha256_mem_copy(ptCtx->chBuffer, pchData, 64);
        SHA256_PROCESS(ptCtx, ptCtx->chBuffer);
#else
        SHA256_PROCESS(ptCtx, pchData);
#endif
        pchData += 64;
    }

    //! buffer unaligned data
    if (wLen) {
        sha256_mem_copy(ptCtx->chBuffer + last, pchData, wLen);
    }
}

/*! \brief                SHA-256 final digest
 *!
 *! \param ptCtx          SHA-256 context
 *! \param pchChecksum    256 checksum result
 */
void sha256_finish(sha256_context_t *ptCtx, uint8_t *pchChecksum)
{
#if defined(SHA256_OPT_SMALLER)
    uint8_t chPadding;
#else
    static const uint8_t chPadding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
#endif
    uint32_t last, padn;
    uint32_t high, low;
    uint8_t  msglen[8];
    uint32_t i;

    //! amount data length in bit
    high = (ptCtx->wTotal[1] <<  3)
         | (ptCtx->wTotal[0] >> 29);
    low  = (ptCtx->wTotal[0] <<  3);

    SHA_PUT_UINT32_BE(high, msglen + 0);
    SHA_PUT_UINT32_BE(low,  msglen + 4);

    //! check how many pad should be appended(in byte) to align 64(512 bits)
    last = ptCtx->wTotal[0] & 0x3F;
    padn = (last < (64 - 8))? ((64 - 8) - last) : ((128 - 8) - last);

    //!< process appended data
#if defined(SHA256_OPT_SMALLER)
    chPadding = 0x80;
    sha256_update(ptCtx, &chPadding, 1);
    chPadding = 0;
    padn--;
    for (i = padn; i; i--) {
        sha256_update(ptCtx, &chPadding, 1);
    }
#else
    sha256_update(ptCtx, chPadding, padn);
#endif

    //!< process data length
    sha256_update(ptCtx, msglen, 8);

    for (i = 0; i < 8; i++) {
        SHA_PUT_UINT32_BE(ptCtx->wState[i], pchChecksum + 4 * i);
    }
}


/*! \brief              Output = SHA-256( input buffer )
 *!
 *! \param pchData      buffer holding the data
 *! \param wLen         length of the input data
 *! \param pchChecksum  SHA-256 checksum result
 */
void sha256_checksum(uint8_t *pchChecksum, const uint8_t *pchData, uint32_t wLen)
{
    sha256_context_t tCtx;

    sha256_starts(&tCtx);
    sha256_update(&tCtx, pchData, wLen);
    sha256_finish(&tCtx, pchChecksum);
}

