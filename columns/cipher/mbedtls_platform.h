/**
 * \file platform.h
 *
 * \brief This file contains the definitions and functions of the
 *        Mbed TLS platform abstraction layer.
 *
 *        The platform abstraction layer removes the need for the library
 *        to directly link to standard C library functions or operating
 *        system services, making the library easier to port and embed.
 *        Application developers and users of the library can provide their own
 *        implementations of these functions, or implementations specific to
 *        their platform, which can be statically linked to the library or
 *        dynamically configured at runtime.
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
#ifndef __MBEDTLS_PLATFORM_H__
#define __MBEDTLS_PLATFORM_H__

#if !defined(MBEDTLS_CONFIG_FILE)
#error "mbedtls_config.h path must be defined by MBEDTLS_CONFIG_FILE"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "./app_cfg.h"
#include "./mbedtls_error.h"

#if defined(MBEDTLS_HAVE_TIME_DATE)
#include "./mbedtls_platform_time.h"
#include <time.h>
#endif /* MBEDTLS_HAVE_TIME_DATE */

#define MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED	-0x0072 /**< The requested feature is not supported by the platform */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_CHECK_PARAMS)

#if defined(MBEDTLS_CHECK_PARAMS_ASSERT)
/* Allow the user to define MBEDTLS_PARAM_FAILED to something like assert
 * (which is what our config.h suggests). */
#include <assert.h>
#endif /* MBEDTLS_CHECK_PARAMS_ASSERT */

#if defined(MBEDTLS_PARAM_FAILED)
/** An alternative definition of MBEDTLS_PARAM_FAILED has been set in config.h.
 *
 * This flag can be used to check whether it is safe to assume that
 * MBEDTLS_PARAM_FAILED() will expand to a call to mbedtls_param_failed().
 */
#define MBEDTLS_PARAM_FAILED_ALT
#elif defined(MBEDTLS_CHECK_PARAMS_ASSERT) /* MBEDTLS_PARAM_FAILED */
#define MBEDTLS_PARAM_FAILED( cond ) assert( cond )
#define MBEDTLS_PARAM_FAILED_ALT
#else /* MBEDTLS_PARAM_FAILED */
#define MBEDTLS_PARAM_FAILED( cond ) \
    mbedtls_param_failed( #cond, __FILE__, __LINE__ )

/**
 * \brief       User supplied callback function for parameter validation failure.
 *              See #MBEDTLS_CHECK_PARAMS for context.
 *
 *              This function will be called unless an alternative treatement
 *              is defined through the #MBEDTLS_PARAM_FAILED macro.
 *
 *              This function can return, and the operation will be aborted, or
 *              alternatively, through use of setjmp()/longjmp() can resume
 *              execution in the application code.
 *
 * \param failure_condition The assertion that didn't hold.
 * \param file  The file where the assertion failed.
 * \param line  The line in the file where the assertion failed.
 */
void mbedtls_param_failed( const char *failure_condition,
                           const char *file,
                           int line );
#endif /* MBEDTLS_PARAM_FAILED */

/* Internal macro meant to be called only from within the library. */
#define MBEDTLS_INTERNAL_VALIDATE_RET( cond, ret )  \
    do {                                            \
        if( !(cond) )                               \
        {                                           \
            MBEDTLS_PARAM_FAILED( cond );           \
            return( ret );                          \
        }                                           \
    } while( 0 )

/* Internal macro meant to be called only from within the library. */
#define MBEDTLS_INTERNAL_VALIDATE( cond )           \
    do {                                            \
        if( !(cond) )                               \
        {                                           \
            MBEDTLS_PARAM_FAILED( cond );           \
            return;                                 \
        }                                           \
    } while( 0 )

#else /* MBEDTLS_CHECK_PARAMS */

/* Internal macros meant to be called only from within the library. */
#define MBEDTLS_INTERNAL_VALIDATE_RET( cond, ret )  do { } while( 0 )
#define MBEDTLS_INTERNAL_VALIDATE( cond )           do { } while( 0 )

#endif /* MBEDTLS_CHECK_PARAMS */

/**
 * \name SECTION: Module settings
 *
 * The configuration options you can set for this module are in this section.
 * Either change them in config.h or define them on the compiler command line.
 * \{
 */

/* The older Microsoft Windows common runtime provides non-conforming
 * implementations of some standard library functions, including snprintf
 * and vsnprintf. This affects MSVC and MinGW builds.
 */
#if defined(__MINGW32__) || (defined(_MSC_VER) && _MSC_VER <= 1900)
#define MBEDTLS_PLATFORM_HAS_NON_CONFORMING_SNPRINTF
#define MBEDTLS_PLATFORM_HAS_NON_CONFORMING_VSNPRINTF
#endif

#if !defined(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if !defined(MBEDTLS_PLATFORM_STD_SNPRINTF)
#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_SNPRINTF)
#define MBEDTLS_PLATFORM_STD_SNPRINTF   mbedtls_platform_win32_snprintf /**< The default \c snprintf function to use.  */
#else
#define MBEDTLS_PLATFORM_STD_SNPRINTF   snprintf /**< The default \c snprintf function to use.  */
#endif
#endif

#if !defined(MBEDTLS_PLATFORM_STD_VSNPRINTF)
#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_VSNPRINTF)
#define MBEDTLS_PLATFORM_STD_VSNPRINTF   mbedtls_platform_win32_vsnprintf /**< The default \c vsnprintf function to use.  */
#else
#define MBEDTLS_PLATFORM_STD_VSNPRINTF		vsnprintf /**< The default \c vsnprintf function to use.  */
#endif
#endif

#if !defined(MBEDTLS_PLATFORM_STD_PRINTF)
#define MBEDTLS_PLATFORM_STD_PRINTF			printf /**< The default \c printf function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_FPRINTF)
#define MBEDTLS_PLATFORM_STD_FPRINTF		fprintf /**< The default \c fprintf function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_CALLOC)
#define MBEDTLS_PLATFORM_STD_CALLOC			calloc /**< The default \c calloc function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_FREE)
#define MBEDTLS_PLATFORM_STD_FREE			free /**< The default \c free function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_EXIT)
#define MBEDTLS_PLATFORM_STD_EXIT			exit /**< The default \c exit function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_TIME)
#define MBEDTLS_PLATFORM_STD_TIME			time    /**< The default \c time function to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_EXIT_SUCCESS)
#define MBEDTLS_PLATFORM_STD_EXIT_SUCCESS	EXIT_SUCCESS /**< The default exit value to use. */
#endif

#if !defined(MBEDTLS_PLATFORM_STD_EXIT_FAILURE)
#define MBEDTLS_PLATFORM_STD_EXIT_FAILURE	EXIT_FAILURE /**< The default exit value to use. */
#endif

#if defined(MBEDTLS_FS_IO)
#if !defined(MBEDTLS_PLATFORM_STD_NV_SEED_READ)
#define MBEDTLS_PLATFORM_STD_NV_SEED_READ   mbedtls_platform_std_nv_seed_read
#endif
#if !defined(MBEDTLS_PLATFORM_STD_NV_SEED_WRITE)
#define MBEDTLS_PLATFORM_STD_NV_SEED_WRITE  mbedtls_platform_std_nv_seed_write
#endif
#if !defined(MBEDTLS_PLATFORM_STD_NV_SEED_FILE)
#define MBEDTLS_PLATFORM_STD_NV_SEED_FILE   "seedfile"
#endif
#endif /* MBEDTLS_FS_IO */

#else /* MBEDTLS_PLATFORM_NO_STD_FUNCTIONS */

#if defined(MBEDTLS_PLATFORM_STD_MEM_HDR)
#include MBEDTLS_PLATFORM_STD_MEM_HDR
#endif

#endif /* MBEDTLS_PLATFORM_NO_STD_FUNCTIONS */


/* \} name SECTION: Module settings */

/*
 * The function pointers for calloc and free.
 */
#if defined(MBEDTLS_PLATFORM_MEMORY)

#if defined(MBEDTLS_PLATFORM_FREE_MACRO) && \
    defined(MBEDTLS_PLATFORM_CALLOC_MACRO)
#define mbedtls_free       MBEDTLS_PLATFORM_FREE_MACRO
#define mbedtls_calloc     MBEDTLS_PLATFORM_CALLOC_MACRO
#else
/* For size_t */
#include <stddef.h>
extern void *mbedtls_calloc( size_t n, size_t size );
extern void mbedtls_free( void *ptr );

/**
 * \brief               This function dynamically sets the memory-management
 *                      functions used by the library, during runtime.
 *
 * \param calloc_func   The \c calloc function implementation.
 * \param free_func     The \c free function implementation.
 *
 * \return              \c 0.
 */
int mbedtls_platform_set_calloc_free( void * (*calloc_func)( size_t, size_t ),
                              void (*free_func)( void * ) );
#endif /* MBEDTLS_PLATFORM_FREE_MACRO && MBEDTLS_PLATFORM_CALLOC_MACRO */

#else /* !MBEDTLS_PLATFORM_MEMORY */

#define mbedtls_free       free
#define mbedtls_calloc     calloc

#endif /* MBEDTLS_PLATFORM_MEMORY && !MBEDTLS_PLATFORM_{FREE,CALLOC}_MACRO */

/*
 * The function pointers for fprintf
 */
#if defined(MBEDTLS_PLATFORM_FPRINTF_ALT)
/* We need FILE * */
#include <stdio.h>
extern int (*mbedtls_fprintf)( FILE *stream, const char *format, ... );

/**
 * \brief                This function dynamically configures the fprintf
 *                       function that is called when the
 *                       mbedtls_fprintf() function is invoked by the library.
 *
 * \param fprintf_func   The \c fprintf function implementation.
 *
 * \return               \c 0.
 */
int mbedtls_platform_set_fprintf( int (*fprintf_func)( FILE *stream, const char *,
                                               ... ) );
#else /* !MBEDTLS_PLATFORM_FPRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_FPRINTF_MACRO)
#define mbedtls_fprintf    MBEDTLS_PLATFORM_FPRINTF_MACRO
#else
#define mbedtls_fprintf    fprintf
#endif /* MBEDTLS_PLATFORM_FPRINTF_MACRO */

#endif /* MBEDTLS_PLATFORM_FPRINTF_ALT */

/*
 * The function pointers for printf
 */
#if defined(MBEDTLS_PLATFORM_PRINTF_ALT)
extern int (*mbedtls_printf)( const char *format, ... );

/**
 * \brief               This function dynamically configures the snprintf
 *                      function that is called when the mbedtls_snprintf()
 *                      function is invoked by the library.
 *
 * \param printf_func   The \c printf function implementation.
 *
 * \return              \c 0 on success.
 */
int mbedtls_platform_set_printf( int (*printf_func)( const char *, ... ) );

#else /* !MBEDTLS_PLATFORM_PRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_PRINTF_MACRO)
#define mbedtls_printf     MBEDTLS_PLATFORM_PRINTF_MACRO
#else
#define mbedtls_printf     printf
#endif /* MBEDTLS_PLATFORM_PRINTF_MACRO */

#endif /* MBEDTLS_PLATFORM_PRINTF_ALT */

/*
 * The function pointers for snprintf
 *
 * The snprintf implementation should conform to C99:
 * - it *must* always correctly zero-terminate the buffer
 *   (except when n == 0, then it must leave the buffer untouched)
 * - however it is acceptable to return -1 instead of the required length when
 *   the destination buffer is too short.
 */
#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_SNPRINTF)
/* For Windows (inc. MSYS2), we provide our own fixed implementation */
int mbedtls_platform_win32_snprintf( char *s, size_t n, const char *fmt, ... );
#endif

#if defined(MBEDTLS_PLATFORM_SNPRINTF_ALT)
extern int (*mbedtls_snprintf)( char * s, size_t n, const char * format, ... );

/**
 * \brief                 This function allows configuring a custom
 *                        \c snprintf function pointer.
 *
 * \param snprintf_func   The \c snprintf function implementation.
 *
 * \return                \c 0 on success.
 */
int mbedtls_platform_set_snprintf( int (*snprintf_func)( char * s, size_t n,
                                                 const char * format, ... ) );
#else /* MBEDTLS_PLATFORM_SNPRINTF_ALT */
#if defined(MBEDTLS_PLATFORM_SNPRINTF_MACRO)
#define mbedtls_snprintf   MBEDTLS_PLATFORM_SNPRINTF_MACRO
#else
#define mbedtls_snprintf   MBEDTLS_PLATFORM_STD_SNPRINTF
#endif /* MBEDTLS_PLATFORM_SNPRINTF_MACRO */
#endif /* MBEDTLS_PLATFORM_SNPRINTF_ALT */

/*
 * The function pointers for vsnprintf
 *
 * The vsnprintf implementation should conform to C99:
 * - it *must* always correctly zero-terminate the buffer
 *   (except when n == 0, then it must leave the buffer untouched)
 * - however it is acceptable to return -1 instead of the required length when
 *   the destination buffer is too short.
 */
#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_VSNPRINTF)
#include <stdarg.h>
/* For Older Windows (inc. MSYS2), we provide our own fixed implementation */
int mbedtls_platform_win32_vsnprintf( char *s, size_t n, const char *fmt, va_list arg );
#endif

#if defined(MBEDTLS_PLATFORM_VSNPRINTF_ALT)
#include <stdarg.h>
extern int (*mbedtls_vsnprintf)( char * s, size_t n, const char * format, va_list arg );

/**
 * \brief   Set your own snprintf function pointer
 *
 * \param   vsnprintf_func   The \c vsnprintf function implementation
 *
 * \return  \c 0
 */
int mbedtls_platform_set_vsnprintf( int (*vsnprintf_func)( char * s, size_t n,
                                                 const char * format, va_list arg ) );
#else /* MBEDTLS_PLATFORM_VSNPRINTF_ALT */
#if defined(MBEDTLS_PLATFORM_VSNPRINTF_MACRO)
#define mbedtls_vsnprintf   MBEDTLS_PLATFORM_VSNPRINTF_MACRO
#else
#define mbedtls_vsnprintf   vsnprintf
#endif /* MBEDTLS_PLATFORM_VSNPRINTF_MACRO */
#endif /* MBEDTLS_PLATFORM_VSNPRINTF_ALT */

/*
 * The function pointers for exit
 */
#if defined(MBEDTLS_PLATFORM_EXIT_ALT)
extern void (*mbedtls_exit)( int status );

/**
 * \brief             This function dynamically configures the exit
 *                    function that is called when the mbedtls_exit()
 *                    function is invoked by the library.
 *
 * \param exit_func   The \c exit function implementation.
 *
 * \return            \c 0 on success.
 */
int mbedtls_platform_set_exit( void (*exit_func)( int status ) );
#else
#if defined(MBEDTLS_PLATFORM_EXIT_MACRO)
#define mbedtls_exit   MBEDTLS_PLATFORM_EXIT_MACRO
#else
#define mbedtls_exit   exit
#endif /* MBEDTLS_PLATFORM_EXIT_MACRO */
#endif /* MBEDTLS_PLATFORM_EXIT_ALT */

/*
 * The default exit values
 */
#if defined(MBEDTLS_PLATFORM_STD_EXIT_SUCCESS)
#define MBEDTLS_EXIT_SUCCESS MBEDTLS_PLATFORM_STD_EXIT_SUCCESS
#else
#define MBEDTLS_EXIT_SUCCESS 0
#endif
#if defined(MBEDTLS_PLATFORM_STD_EXIT_FAILURE)
#define MBEDTLS_EXIT_FAILURE MBEDTLS_PLATFORM_STD_EXIT_FAILURE
#else
#define MBEDTLS_EXIT_FAILURE 1
#endif

/*
 * The function pointers for reading from and writing a seed file to
 * Non-Volatile storage (NV) in a platform-independent way
 *
 * Only enabled when the NV seed entropy source is enabled
 */
#if defined(MBEDTLS_ENTROPY_NV_SEED)
#if !defined(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS) && defined(MBEDTLS_FS_IO)
/* Internal standard platform definitions */
int mbedtls_platform_std_nv_seed_read( unsigned char *buf, size_t buf_len );
int mbedtls_platform_std_nv_seed_write( unsigned char *buf, size_t buf_len );
#endif

#if defined(MBEDTLS_PLATFORM_NV_SEED_ALT)
extern int (*mbedtls_nv_seed_read)( unsigned char *buf, size_t buf_len );
extern int (*mbedtls_nv_seed_write)( unsigned char *buf, size_t buf_len );

/**
 * \brief   This function allows configuring custom seed file writing and
 *          reading functions.
 *
 * \param   nv_seed_read_func   The seed reading function implementation.
 * \param   nv_seed_write_func  The seed writing function implementation.
 *
 * \return  \c 0 on success.
 */
int mbedtls_platform_set_nv_seed(
            int (*nv_seed_read_func)( unsigned char *buf, size_t buf_len ),
            int (*nv_seed_write_func)( unsigned char *buf, size_t buf_len )
            );
#else
#if defined(MBEDTLS_PLATFORM_NV_SEED_READ_MACRO) && \
    defined(MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO)
#define mbedtls_nv_seed_read    MBEDTLS_PLATFORM_NV_SEED_READ_MACRO
#define mbedtls_nv_seed_write   MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO
#else
#define mbedtls_nv_seed_read    mbedtls_platform_std_nv_seed_read
#define mbedtls_nv_seed_write   mbedtls_platform_std_nv_seed_write
#endif
#endif /* MBEDTLS_PLATFORM_NV_SEED_ALT */
#endif /* MBEDTLS_ENTROPY_NV_SEED */

#if !defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)

/**
 * \brief   The platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 */
typedef struct mbedtls_platform_context
{
    char dummy; /**< A placeholder member, as empty structs are not portable. */
}
mbedtls_platform_context;

#else
#include "platform_alt.h"
#endif /* !MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */

/**
 * \brief   This function performs any platform-specific initialization
 *          operations.
 *
 * \note    This function should be called before any other library functions.
 *
 *          Its implementation is platform-specific, and unless
 *          platform-specific code is provided, it does nothing.
 *
 * \note    The usage and necessity of this function is dependent on the platform.
 *
 * \param   ctx     The platform context.
 *
 * \return  \c 0 on success.
 */
int mbedtls_platform_setup( mbedtls_platform_context *ctx );
/**
 * \brief   This function performs any platform teardown operations.
 *
 * \note    This function should be called after every other Mbed TLS module
 *          has been correctly freed using the appropriate free function.
 *
 *          Its implementation is platform-specific, and unless
 *          platform-specific code is provided, it does nothing.
 *
 * \note    The usage and necessity of this function is dependent on the platform.
 *
 * \param   ctx     The platform context.
 *
 */
void mbedtls_platform_teardown( mbedtls_platform_context *ctx );

/**
 * \brief       Securely zeroize a buffer
 *
 *              The function is meant to wipe the data contained in a buffer so
 *              that it can no longer be recovered even if the program memory
 *              is later compromised. Call this function on sensitive data
 *              stored on the stack before returning from a function, and on
 *              sensitive data stored on the heap before freeing the heap
 *              object.
 *
 *              It is extremely difficult to guarantee that calls to
 *              mbedtls_platform_zeroize() are not removed by aggressive
 *              compiler optimizations in a portable way. For this reason, Mbed
 *              TLS provides the configuration option
 *              MBEDTLS_PLATFORM_ZEROIZE_ALT, which allows users to configure
 *              mbedtls_platform_zeroize() to use a suitable implementation for
 *              their platform and needs
 *
 * \param buf   Buffer to be zeroized
 * \param len   Length of the buffer in bytes
 *
 */
void mbedtls_platform_zeroize( void *buf, size_t len );


#if defined(MBEDTLS_HAVE_TIME_DATE)
/**
 * \brief      Platform-specific implementation of gmtime_r()
 *
 *             The function is a thread-safe abstraction that behaves
 *             similarly to the gmtime_r() function from Unix/POSIX.
 *
 *             Mbed TLS will try to identify the underlying platform and
 *             make use of an appropriate underlying implementation (e.g.
 *             gmtime_r() for POSIX and gmtime_s() for Windows). If this is
 *             not possible, then gmtime() will be used. In this case, calls
 *             from the library to gmtime() will be guarded by the mutex
 *             mbedtls_threading_gmtime_mutex if MBEDTLS_THREADING_C is
 *             enabled. It is recommended that calls from outside the library
 *             are also guarded by this mutex.
 *
 *             If MBEDTLS_PLATFORM_GMTIME_R_ALT is defined, then Mbed TLS will
 *             unconditionally use the alternative implementation for
 *             mbedtls_platform_gmtime_r() supplied by the user at compile time.
 *
 * \param tt     Pointer to an object containing time (in seconds) since the
 *               epoch to be converted
 * \param tm_buf Pointer to an object where the results will be stored
 *
 * \return      Pointer to an object of type struct tm on success, otherwise
 *              NULL
 */
struct tm *mbedtls_platform_gmtime_r( const mbedtls_time_t *tt,
                                      struct tm *tm_buf );
#endif /* MBEDTLS_HAVE_TIME_DATE */



#ifdef __cplusplus
}
#endif

#endif /* platform.h */
