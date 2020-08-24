/**
 * \file rsa.h
 *
 * \brief This file provides an API for the RSA public-key cryptosystem.
 *
 * The RSA public-key cryptosystem is defined in <em>Public-Key
 * Cryptography Standards (PKCS) #1 v1.5: RSA Encryption</em>
 * and <em>Public-Key Cryptography Standards (PKCS) #1 v2.1:
 * RSA Cryptography Specifications</em>.
 *
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_RSA_WRAPPER_H
#define MBEDTLS_RSA_WRAPPER_H

#if !defined(MBEDTLS_CONFIG_FILE)
#error "mbedtls_config.h path must be defined by MBEDTLS_CONFIG_FILE"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "./app_cfg.h"
#include "./md.h"
#include "cipher/rsa.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * \brief          This function adds the message padding, then performs an RSA
 *                 operation.
 *
 *                 It is the generic wrapper for performing a PKCS#1 encryption
 *                 operation using the \p mode from the context.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG to use. It is mandatory for PKCS#1 v2.1 padding
 *                 encoding, and for PKCS#1 v1.5 padding encoding when used
 *                 with \p mode set to #MBEDTLS_RSA_PUBLIC. For PKCS#1 v1.5
 *                 padding encoding and \p mode set to #MBEDTLS_RSA_PRIVATE,
 *                 it is used for blinding and should be provided in this
 *                 case; see mbedtls_rsa_private() for more.
 * \param p_rng    The RNG context to be passed to \p f_rng. May be
 *                 \c NULL if \p f_rng is \c NULL or if \p f_rng doesn't
 *                 need a context argument.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param ilen     The length of the plaintext in Bytes.
 * \param input    The input data to encrypt. This must be a readable
 *                 buffer of size \p ilen Bytes. It may be \c NULL if
 *                 `ilen == 0`.
 * \param output   The output buffer. This must be a writable buffer
 *                 of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_pkcs1_encrypt( mbedtls_rsa_context *ctx,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng,
                       int mode, size_t ilen,
                       const unsigned char *input,
                       unsigned char *output );

/**
 * \brief          This function performs a PKCS#1 v1.5 encryption operation
 *                 (RSAES-PKCS1-v1_5-ENCRYPT).
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function to use. It is needed for padding generation
 *                 if \p mode is #MBEDTLS_RSA_PUBLIC. If \p mode is
 *                 #MBEDTLS_RSA_PRIVATE (discouraged), it is used for
 *                 blinding and should be provided; see mbedtls_rsa_private().
 * \param p_rng    The RNG context to be passed to \p f_rng. This may
 *                 be \c NULL if \p f_rng is \c NULL or if \p f_rng
 *                 doesn't need a context argument.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param ilen     The length of the plaintext in Bytes.
 * \param input    The input data to encrypt. This must be a readable
 *                 buffer of size \p ilen Bytes. It may be \c NULL if
 *                 `ilen == 0`.
 * \param output   The output buffer. This must be a writable buffer
 *                 of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsaes_pkcs1_v15_encrypt( mbedtls_rsa_context *ctx,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng,
                                 int mode, size_t ilen,
                                 const unsigned char *input,
                                 unsigned char *output );

/**
 * \brief            This function performs a PKCS#1 v2.1 OAEP encryption
 *                   operation (RSAES-OAEP-ENCRYPT).
 *
 * \note             The output buffer must be as large as the size
 *                   of ctx->N. For example, 128 Bytes if RSA-1024 is used.
 *
 * \deprecated       It is deprecated and discouraged to call this function
 *                   in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                   are likely to remove the \p mode argument and have it
 *                   implicitly set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note             Alternative implementations of RSA need not support
 *                   mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                   return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx        The initnialized RSA context to use.
 * \param f_rng      The RNG function to use. This is needed for padding
 *                   generation and must be provided.
 * \param p_rng      The RNG context to be passed to \p f_rng. This may
 *                   be \c NULL if \p f_rng doesn't need a context argument.
 * \param mode       The mode of operation. This must be either
 *                   #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param label      The buffer holding the custom label to use.
 *                   This must be a readable buffer of length \p label_len
 *                   Bytes. It may be \c NULL if \p label_len is \c 0.
 * \param label_len  The length of the label in Bytes.
 * \param ilen       The length of the plaintext buffer \p input in Bytes.
 * \param input      The input data to encrypt. This must be a readable
 *                   buffer of size \p ilen Bytes. It may be \c NULL if
 *                   `ilen == 0`.
 * \param output     The output buffer. This must be a writable buffer
 *                   of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                   for an 2048-bit RSA modulus.
 *
 * \return           \c 0 on success.
 * \return           An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsaes_oaep_encrypt( mbedtls_rsa_context *ctx,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng,
                            int mode,
                            const unsigned char *label, size_t label_len,
                            size_t ilen,
                            const unsigned char *input,
                            unsigned char *output );

/**
 * \brief          This function performs an RSA operation, then removes the
 *                 message padding.
 *
 *                 It is the generic wrapper for performing a PKCS#1 decryption
 *                 operation using the \p mode from the context.
 *
 * \note           The output buffer length \c output_max_len should be
 *                 as large as the size \p ctx->len of \p ctx->N (for example,
 *                 128 Bytes if RSA-1024 is used) to be able to hold an
 *                 arbitrary decrypted message. If it is not large enough to
 *                 hold the decryption of the particular ciphertext provided,
 *                 the function returns \c MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. If \p mode is
 *                 #MBEDTLS_RSA_PUBLIC, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param olen     The address at which to store the length of
 *                 the plaintext. This must not be \c NULL.
 * \param input    The ciphertext buffer. This must be a readable buffer
 *                 of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 * \param output   The buffer used to hold the plaintext. This must
 *                 be a writable buffer of length \p output_max_len Bytes.
 * \param output_max_len The length in Bytes of the output buffer \p output.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_pkcs1_decrypt( mbedtls_rsa_context *ctx,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng,
                       int mode, size_t *olen,
                       const unsigned char *input,
                       unsigned char *output,
                       size_t output_max_len );

/**
 * \brief          This function performs a PKCS#1 v1.5 decryption
 *                 operation (RSAES-PKCS1-v1_5-DECRYPT).
 *
 * \note           The output buffer length \c output_max_len should be
 *                 as large as the size \p ctx->len of \p ctx->N, for example,
 *                 128 Bytes if RSA-1024 is used, to be able to hold an
 *                 arbitrary decrypted message. If it is not large enough to
 *                 hold the decryption of the particular ciphertext provided,
 *                 the function returns #MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. If \p mode is
 *                 #MBEDTLS_RSA_PUBLIC, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param olen     The address at which to store the length of
 *                 the plaintext. This must not be \c NULL.
 * \param input    The ciphertext buffer. This must be a readable buffer
 *                 of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 * \param output   The buffer used to hold the plaintext. This must
 *                 be a writable buffer of length \p output_max_len Bytes.
 * \param output_max_len The length in Bytes of the output buffer \p output.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 *
 */
int mbedtls_rsa_rsaes_pkcs1_v15_decrypt( mbedtls_rsa_context *ctx,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng,
                                 int mode, size_t *olen,
                                 const unsigned char *input,
                                 unsigned char *output,
                                 size_t output_max_len );

/**
 * \brief            This function performs a PKCS#1 v2.1 OAEP decryption
 *                   operation (RSAES-OAEP-DECRYPT).
 *
 * \note             The output buffer length \c output_max_len should be
 *                   as large as the size \p ctx->len of \p ctx->N, for
 *                   example, 128 Bytes if RSA-1024 is used, to be able to
 *                   hold an arbitrary decrypted message. If it is not
 *                   large enough to hold the decryption of the particular
 *                   ciphertext provided, the function returns
 *                   #MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE.
 *
 * \deprecated       It is deprecated and discouraged to call this function
 *                   in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                   are likely to remove the \p mode argument and have it
 *                   implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note             Alternative implementations of RSA need not support
 *                   mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                   return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx        The initialized RSA context to use.
 * \param f_rng      The RNG function. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                   this is used for blinding and should be provided; see
 *                   mbedtls_rsa_private() for more. If \p mode is
 *                   #MBEDTLS_RSA_PUBLIC, it is ignored.
 * \param p_rng      The RNG context to be passed to \p f_rng. This may be
 *                   \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode       The mode of operation. This must be either
 *                   #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param label      The buffer holding the custom label to use.
 *                   This must be a readable buffer of length \p label_len
 *                   Bytes. It may be \c NULL if \p label_len is \c 0.
 * \param label_len  The length of the label in Bytes.
 * \param olen       The address at which to store the length of
 *                   the plaintext. This must not be \c NULL.
 * \param input      The ciphertext buffer. This must be a readable buffer
 *                   of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                   for an 2048-bit RSA modulus.
 * \param output     The buffer used to hold the plaintext. This must
 *                   be a writable buffer of length \p output_max_len Bytes.
 * \param output_max_len The length in Bytes of the output buffer \p output.
 *
 * \return         \c 0 on success.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsaes_oaep_decrypt( mbedtls_rsa_context *ctx,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng,
                            int mode,
                            const unsigned char *label, size_t label_len,
                            size_t *olen,
                            const unsigned char *input,
                            unsigned char *output,
                            size_t output_max_len );

/**
 * \brief          This function performs a private RSA operation to sign
 *                 a message digest using PKCS#1.
 *
 *                 It is the generic wrapper for performing a PKCS#1
 *                 signature using the \p mode from the context.
 *
 * \note           The \p sig buffer must be as large as the size
 *                 of \p ctx->N. For example, 128 Bytes if RSA-1024 is used.
 *
 * \note           For PKCS#1 v2.1 encoding, see comments on
 *                 mbedtls_rsa_rsassa_pss_sign() for details on
 *                 \p md_alg and \p hash_id.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function to use. If the padding mode is PKCS#1 v2.1,
 *                 this must be provided. If the padding mode is PKCS#1 v1.5 and
 *                 \p mode is #MBEDTLS_RSA_PRIVATE, it is used for blinding
 *                 and should be provided; see mbedtls_rsa_private() for more
 *                 more. It is ignored otherwise.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be \c NULL
 *                 if \p f_rng is \c NULL or doesn't need a context argument.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 Ths is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer to hold the signature. This must be a writable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus. A buffer length of
 *                 #MBEDTLS_MPI_MAX_SIZE is always safe.
 *
 * \return         \c 0 if the signing operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_pkcs1_sign( mbedtls_rsa_context *ctx,
                    int (*f_rng)(void *, unsigned char *, size_t),
                    void *p_rng,
                    int mode,
                    mbedtls_md_type_t md_alg,
                    unsigned int hashlen,
                    const unsigned char *hash,
                    unsigned char *sig );

/**
 * \brief          This function performs a PKCS#1 v1.5 signature
 *                 operation (RSASSA-PKCS1-v1_5-SIGN).
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. If \p mode is
 *                 #MBEDTLS_RSA_PUBLIC, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be \c NULL
 *                 if \p f_rng is \c NULL or doesn't need a context argument.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 Ths is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer to hold the signature. This must be a writable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus. A buffer length of
 *                 #MBEDTLS_MPI_MAX_SIZE is always safe.
 *
 * \return         \c 0 if the signing operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsassa_pkcs1_v15_sign( mbedtls_rsa_context *ctx,
                               int (*f_rng)(void *, unsigned char *, size_t),
                               void *p_rng,
                               int mode,
                               mbedtls_md_type_t md_alg,
                               unsigned int hashlen,
                               const unsigned char *hash,
                               unsigned char *sig );

/**
 * \brief          This function performs a PKCS#1 v2.1 PSS signature
 *                 operation (RSASSA-PSS-SIGN).
 *
 * \note           The \p hash_id in the RSA context is the one used for the
 *                 encoding. \p md_alg in the function call is the type of hash
 *                 that is encoded. According to <em>RFC-3447: Public-Key
 *                 Cryptography Standards (PKCS) #1 v2.1: RSA Cryptography
 *                 Specifications</em> it is advised to keep both hashes the
 *                 same.
 *
 * \note           This function always uses the maximum possible salt size,
 *                 up to the length of the payload hash. This choice of salt
 *                 size complies with FIPS 186-4 ยง5.5 (e) and RFC 8017 (PKCS#1
 *                 v2.2) ยง9.1.1 step 3. Furthermore this function enforces a
 *                 minimum salt size which is the hash size minus 2 bytes. If
 *                 this minimum size is too large given the key size (the salt
 *                 size, plus the hash size, plus 2 bytes must be no more than
 *                 the key size in bytes), this function returns
 *                 #MBEDTLS_ERR_RSA_BAD_INPUT_DATA.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PUBLIC mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PRIVATE.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PUBLIC and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA context to use.
 * \param f_rng    The RNG function. It must not be \c NULL.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be \c NULL
 *                 if \p f_rng doesn't need a context argument.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PRIVATE or #MBEDTLS_RSA_PUBLIC (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 Ths is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer to hold the signature. This must be a writable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus. A buffer length of
 *                 #MBEDTLS_MPI_MAX_SIZE is always safe.
 *
 * \return         \c 0 if the signing operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsassa_pss_sign( mbedtls_rsa_context *ctx,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng,
                         int mode,
                         mbedtls_md_type_t md_alg,
                         unsigned int hashlen,
                         const unsigned char *hash,
                         unsigned char *sig );

/**
 * \brief          This function performs a public RSA operation and checks
 *                 the message digest.
 *
 *                 This is the generic wrapper for performing a PKCS#1
 *                 verification using the mode from the context.
 *
 * \note           For PKCS#1 v2.1 encoding, see comments on
 *                 mbedtls_rsa_rsassa_pss_verify() about \p md_alg and
 *                 \p hash_id.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA public key context to use.
 * \param f_rng    The RNG function to use. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. Otherwise, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 This is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer holding the signature. This must be a readable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 if the verify operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_pkcs1_verify( mbedtls_rsa_context *ctx,
                      int (*f_rng)(void *, unsigned char *, size_t),
                      void *p_rng,
                      int mode,
                      mbedtls_md_type_t md_alg,
                      unsigned int hashlen,
                      const unsigned char *hash,
                      const unsigned char *sig );

/**
 * \brief          This function performs a PKCS#1 v1.5 verification
 *                 operation (RSASSA-PKCS1-v1_5-VERIFY).
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA public key context to use.
 * \param f_rng    The RNG function to use. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. Otherwise, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 This is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer holding the signature. This must be a readable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 if the verify operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsassa_pkcs1_v15_verify( mbedtls_rsa_context *ctx,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng,
                                 int mode,
                                 mbedtls_md_type_t md_alg,
                                 unsigned int hashlen,
                                 const unsigned char *hash,
                                 const unsigned char *sig );

/**
 * \brief          This function performs a PKCS#1 v2.1 PSS verification
 *                 operation (RSASSA-PSS-VERIFY).
 *
 *                 The hash function for the MGF mask generating function
 *                 is that specified in the RSA context.
 *
 * \note           The \p hash_id in the RSA context is the one used for the
 *                 verification. \p md_alg in the function call is the type of
 *                 hash that is verified. According to <em>RFC-3447: Public-Key
 *                 Cryptography Standards (PKCS) #1 v2.1: RSA Cryptography
 *                 Specifications</em> it is advised to keep both hashes the
 *                 same. If \p hash_id in the RSA context is unset,
 *                 the \p md_alg from the function call is used.
 *
 * \deprecated     It is deprecated and discouraged to call this function
 *                 in #MBEDTLS_RSA_PRIVATE mode. Future versions of the library
 *                 are likely to remove the \p mode argument and have it
 *                 implicitly set to #MBEDTLS_RSA_PUBLIC.
 *
 * \note           Alternative implementations of RSA need not support
 *                 mode being set to #MBEDTLS_RSA_PRIVATE and might instead
 *                 return #MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED.
 *
 * \param ctx      The initialized RSA public key context to use.
 * \param f_rng    The RNG function to use. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. Otherwise, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE (deprecated).
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 This is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param sig      The buffer holding the signature. This must be a readable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 if the verify operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsassa_pss_verify( mbedtls_rsa_context *ctx,
                           int (*f_rng)(void *, unsigned char *, size_t),
                           void *p_rng,
                           int mode,
                           mbedtls_md_type_t md_alg,
                           unsigned int hashlen,
                           const unsigned char *hash,
                           const unsigned char *sig );

/**
 * \brief          This function performs a PKCS#1 v2.1 PSS verification
 *                 operation (RSASSA-PSS-VERIFY).
 *
 *                 The hash function for the MGF mask generating function
 *                 is that specified in \p mgf1_hash_id.
 *
 * \note           The \p sig buffer must be as large as the size
 *                 of \p ctx->N. For example, 128 Bytes if RSA-1024 is used.
 *
 * \note           The \p hash_id in the RSA context is ignored.
 *
 * \param ctx      The initialized RSA public key context to use.
 * \param f_rng    The RNG function to use. If \p mode is #MBEDTLS_RSA_PRIVATE,
 *                 this is used for blinding and should be provided; see
 *                 mbedtls_rsa_private() for more. Otherwise, it is ignored.
 * \param p_rng    The RNG context to be passed to \p f_rng. This may be
 *                 \c NULL if \p f_rng is \c NULL or doesn't need a context.
 * \param mode     The mode of operation. This must be either
 *                 #MBEDTLS_RSA_PUBLIC or #MBEDTLS_RSA_PRIVATE.
 * \param md_alg   The message-digest algorithm used to hash the original data.
 *                 Use #MBEDTLS_MD_NONE for signing raw data.
 * \param hashlen  The length of the message digest.
 *                 This is only used if \p md_alg is #MBEDTLS_MD_NONE.
 * \param hash     The buffer holding the message digest or raw data.
 *                 If \p md_alg is #MBEDTLS_MD_NONE, this must be a readable
 *                 buffer of length \p hashlen Bytes. If \p md_alg is not
 *                 #MBEDTLS_MD_NONE, it must be a readable buffer of length
 *                 the size of the hash corresponding to \p md_alg.
 * \param mgf1_hash_id      The message digest used for mask generation.
 * \param expected_salt_len The length of the salt used in padding. Use
 *                          #MBEDTLS_RSA_SALT_LEN_ANY to accept any salt length.
 * \param sig      The buffer holding the signature. This must be a readable
 *                 buffer of length \c ctx->len Bytes. For example, \c 256 Bytes
 *                 for an 2048-bit RSA modulus.
 *
 * \return         \c 0 if the verify operation was successful.
 * \return         An \c MBEDTLS_ERR_RSA_XXX error code on failure.
 */
int mbedtls_rsa_rsassa_pss_verify_ext( mbedtls_rsa_context *ctx,
                               int (*f_rng)(void *, unsigned char *, size_t),
                               void *p_rng,
                               int mode,
                               mbedtls_md_type_t md_alg,
                               unsigned int hashlen,
                               const unsigned char *hash,
                               mbedtls_md_type_t mgf1_hash_id,
                               int expected_salt_len,
                               const unsigned char *sig );

/**
 * \brief          This function copies the components of an RSA context.
 *
 * \param dst      The destination context. This must be initialized.
 * \param src      The source context. This must be initialized.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory allocation failure.
 */
int mbedtls_rsa_copy( mbedtls_rsa_context *dst, const mbedtls_rsa_context *src );

/**
 * \brief          This function frees the components of an RSA key.
 *
 * \param ctx      The RSA context to free. May be \c NULL, in which case
 *                 this function is a no-op. If it is not \c NULL, it must
 *                 point to an initialized RSA context.
 */
void mbedtls_rsa_free( mbedtls_rsa_context *ctx );

#ifdef __cplusplus
}
#endif

#endif /* rsa.h */
