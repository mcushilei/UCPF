/**
 * \file pkcs11.h
 *
 * \brief Wrapper for PKCS#11 library libpkcs11-helper
 *
 * \author Adriaan de Jong <dejong@fox-it.com>
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_PKCS11_H
#define MBEDTLS_PKCS11_H

#if !defined(MBEDTLS_CONFIG_FILE)
#error "mbedtls_config.h path must be defined by MBEDTLS_CONFIG_FILE"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PKCS11_C)

#include "./x509_crt.h"

#include <pkcs11-helper-1.0/pkcs11h-certificate.h>

#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_PKCS11_C */

#endif /* MBEDTLS_PKCS11_H */
