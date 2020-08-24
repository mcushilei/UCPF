/*
 *  Platform abstraction layer
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
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

#if !defined(MBEDTLS_CONFIG_FILE)
#error "mbedtls_config.h path must be defined by MBEDTLS_CONFIG_FILE"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)

#include "./mbedtls_platform.h"

#if !defined(MBEDTLS_PLATFORM_ZEROIZE_ALT)
/*
 * This implementation should never be optimized out by the compiler
 *
 * This implementation for mbedtls_platform_zeroize() was inspired from Colin
 * Percival's blog article at:
 *
 * http://www.daemonology.net/blog/2014-09-04-how-to-zero-a-buffer.html
 *
 * It uses a volatile function pointer to the standard memset(). Because the
 * pointer is volatile the compiler expects it to change at
 * any time and will not optimize out the call that could potentially perform
 * other operations on the input buffer instead of just setting it to 0.
 * Nevertheless, as pointed out by davidtgoldblatt on Hacker News
 * (refer to http://www.daemonology.net/blog/2014-09-05-erratum.html for
 * details), optimizations of the following form are still possible:
 *
 * if( memset_func != memset )
 *     memset_func( buf, 0, len );
 *
 * Note that it is extremely difficult to guarantee that
 * mbedtls_platform_zeroize() will not be optimized out by aggressive compilers
 * in a portable way. For this reason, Mbed TLS also provides the configuration
 * option MBEDTLS_PLATFORM_ZEROIZE_ALT, which allows users to configure
 * mbedtls_platform_zeroize() to use a suitable implementation for their
 * platform and needs.
 */
static void * (* const volatile memset_func)( void *, int, size_t ) = memset;

void mbedtls_platform_zeroize( void *buf, size_t len )
{
    MBEDTLS_INTERNAL_VALIDATE( len == 0 || buf != NULL );

    if( len > 0 )
        memset_func( buf, 0, len );
}
#endif /* MBEDTLS_PLATFORM_ZEROIZE_ALT */


#if defined(MBEDTLS_HAVE_TIME_DATE) && !defined(MBEDTLS_PLATFORM_GMTIME_R_ALT)
#include <time.h>
#if !defined(_WIN32) && (defined(unix) || \
    defined(__unix) || defined(__unix__) || (defined(__APPLE__) && \
    defined(__MACH__)))
#include <unistd.h>
#endif /* !_WIN32 && (unix || __unix || __unix__ ||
        * (__APPLE__ && __MACH__)) */

#if !( ( defined(_POSIX_VERSION) && _POSIX_VERSION >= 200809L ) ||     \
       ( defined(_POSIX_THREAD_SAFE_FUNCTIONS ) &&                     \
         _POSIX_THREAD_SAFE_FUNCTIONS >= 20112L ) )
/*
 * This is a convenience shorthand macro to avoid checking the long
 * preprocessor conditions above. Ideally, we could expose this macro in
 * platform_util.h and simply use it in platform_util.c, threading.c and
 * threading.h. However, this macro is not part of the Mbed TLS public API, so
 * we keep it private by only defining it in this file
 */
#if ! ( defined(_WIN32) && !defined(EFIX64) && !defined(EFI32) )
#define PLATFORM_UTIL_USE_GMTIME
#endif /* ! ( defined(_WIN32) && !defined(EFIX64) && !defined(EFI32) ) */

#endif /* !( ( defined(_POSIX_VERSION) && _POSIX_VERSION >= 200809L ) ||     \
             ( defined(_POSIX_THREAD_SAFE_FUNCTIONS ) &&                     \
                _POSIX_THREAD_SAFE_FUNCTIONS >= 20112L ) ) */

struct tm *mbedtls_platform_gmtime_r( const mbedtls_time_t *tt,
                                      struct tm *tm_buf )
{
#if defined(_WIN32) && !defined(EFIX64) && !defined(EFI32)
    return( ( gmtime_s( tm_buf, tt ) == 0 ) ? tm_buf : NULL );
#elif !defined(PLATFORM_UTIL_USE_GMTIME)
    return( gmtime_r( tt, tm_buf ) );
#else
    struct tm *lt;

#if defined(MBEDTLS_THREADING_C)
    if( mbedtls_mutex_lock( &mbedtls_threading_gmtime_mutex ) != 0 )
        return( NULL );
#endif /* MBEDTLS_THREADING_C */

    lt = gmtime( tt );

    if( lt != NULL )
    {
        memcpy( tm_buf, lt, sizeof( struct tm ) );
    }

#if defined(MBEDTLS_THREADING_C)
    if( mbedtls_mutex_unlock( &mbedtls_threading_gmtime_mutex ) != 0 )
        return( NULL );
#endif /* MBEDTLS_THREADING_C */

    return( ( lt == NULL ) ? NULL : tm_buf );
#endif /* _WIN32 && !EFIX64 && !EFI32 */
}
#endif /* MBEDTLS_HAVE_TIME_DATE && MBEDTLS_PLATFORM_GMTIME_R_ALT */


/* The compile time configuration of memory allocation via the macros
 * MBEDTLS_PLATFORM_{FREE/CALLOC}_MACRO takes precedence over the runtime
 * configuration via mbedtls_platform_set_calloc_free(). So, omit everything
 * related to the latter if MBEDTLS_PLATFORM_{FREE/CALLOC}_MACRO are defined. */
#if defined(MBEDTLS_PLATFORM_MEMORY) &&                 \
    !( defined(MBEDTLS_PLATFORM_CALLOC_MACRO) &&        \
       defined(MBEDTLS_PLATFORM_FREE_MACRO) )

#if !defined(MBEDTLS_PLATFORM_STD_CALLOC)
static void *platform_calloc_uninit( size_t n, size_t size )
{
    ((void) n);
    ((void) size);
    return( NULL );
}

#define MBEDTLS_PLATFORM_STD_CALLOC   platform_calloc_uninit
#endif /* !MBEDTLS_PLATFORM_STD_CALLOC */

#if !defined(MBEDTLS_PLATFORM_STD_FREE)
static void platform_free_uninit( void *ptr )
{
    ((void) ptr);
}

#define MBEDTLS_PLATFORM_STD_FREE     platform_free_uninit
#endif /* !MBEDTLS_PLATFORM_STD_FREE */

static void * (*mbedtls_calloc_func)( size_t, size_t ) = MBEDTLS_PLATFORM_STD_CALLOC;
static void (*mbedtls_free_func)( void * ) = MBEDTLS_PLATFORM_STD_FREE;

void * mbedtls_calloc( size_t nmemb, size_t size )
{
    return (*mbedtls_calloc_func)( nmemb, size );
}

void mbedtls_free( void * ptr )
{
    (*mbedtls_free_func)( ptr );
}

int mbedtls_platform_set_calloc_free( void * (*calloc_func)( size_t, size_t ),
                              void (*free_func)( void * ) )
{
    mbedtls_calloc_func = calloc_func;
    mbedtls_free_func = free_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_MEMORY &&
          !( defined(MBEDTLS_PLATFORM_CALLOC_MACRO) &&
             defined(MBEDTLS_PLATFORM_FREE_MACRO) ) */

#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_SNPRINTF)
#include <stdarg.h>
int mbedtls_platform_win32_snprintf( char *s, size_t n, const char *fmt, ... )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    va_list argp;

    va_start( argp, fmt );
    ret = mbedtls_vsnprintf( s, n, fmt, argp );
    va_end( argp );

    return( ret );
}
#endif

#if defined(MBEDTLS_PLATFORM_SNPRINTF_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_SNPRINTF)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static int platform_snprintf_uninit( char * s, size_t n,
                                     const char * format, ... )
{
    ((void) s);
    ((void) n);
    ((void) format);
    return( 0 );
}

#define MBEDTLS_PLATFORM_STD_SNPRINTF    platform_snprintf_uninit
#endif /* !MBEDTLS_PLATFORM_STD_SNPRINTF */

int (*mbedtls_snprintf)( char * s, size_t n,
                          const char * format,
                          ... ) = MBEDTLS_PLATFORM_STD_SNPRINTF;

int mbedtls_platform_set_snprintf( int (*snprintf_func)( char * s, size_t n,
                                                 const char * format,
                                                 ... ) )
{
    mbedtls_snprintf = snprintf_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_SNPRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_HAS_NON_CONFORMING_VSNPRINTF)
#include <stdarg.h>
int mbedtls_platform_win32_vsnprintf( char *s, size_t n, const char *fmt, va_list arg )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    /* Avoid calling the invalid parameter handler by checking ourselves */
    if( s == NULL || n == 0 || fmt == NULL )
        return( -1 );

#if defined(_TRUNCATE)
    ret = vsnprintf_s( s, n, _TRUNCATE, fmt, arg );
#else
    ret = vsnprintf( s, n, fmt, arg );
    if( ret < 0 || (size_t) ret == n )
    {
        s[n-1] = '\0';
        ret = -1;
    }
#endif

    return( ret );
}
#endif

#if defined(MBEDTLS_PLATFORM_VSNPRINTF_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_VSNPRINTF)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static int platform_vsnprintf_uninit( char * s, size_t n,
                                     const char * format, va_list arg )
{
    ((void) s);
    ((void) n);
    ((void) format);
    ((void) arg);
    return( -1 );
}

#define MBEDTLS_PLATFORM_STD_VSNPRINTF    platform_vsnprintf_uninit
#endif /* !MBEDTLS_PLATFORM_STD_VSNPRINTF */

int (*mbedtls_vsnprintf)( char * s, size_t n,
                          const char * format,
                          va_list arg ) = MBEDTLS_PLATFORM_STD_VSNPRINTF;

int mbedtls_platform_set_vsnprintf( int (*vsnprintf_func)( char * s, size_t n,
                                                 const char * format,
                                                 va_list arg ) )
{
    mbedtls_vsnprintf = vsnprintf_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_VSNPRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_PRINTF_ALT)

#if !defined(MBEDTLS_PLATFORM_STD_PRINTF)
/*
 * Make dummy function to prevent NULL pointer dereferences before
 * mbedtls_printf has been set by calling mbedtls_platform_set_printf().
 */
static int platform_printf_uninit( const char *format, ... )
{
    ((void) format);
    return( 0 );
}
#define MBEDTLS_PLATFORM_STD_PRINTF    platform_printf_uninit
#endif /* !MBEDTLS_PLATFORM_STD_PRINTF */

int (*mbedtls_printf)( const char *, ... ) = MBEDTLS_PLATFORM_STD_PRINTF;

int mbedtls_platform_set_printf( int (*printf_func)( const char *, ... ) )
{
    mbedtls_printf = printf_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_PRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_FPRINTF_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_FPRINTF)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static int platform_fprintf_uninit( FILE *stream, const char *format, ... )
{
    ((void) stream);
    ((void) format);
    return( 0 );
}

#define MBEDTLS_PLATFORM_STD_FPRINTF   platform_fprintf_uninit
#endif /* !MBEDTLS_PLATFORM_STD_FPRINTF */

int (*mbedtls_fprintf)( FILE *, const char *, ... ) =
                                        MBEDTLS_PLATFORM_STD_FPRINTF;

int mbedtls_platform_set_fprintf( int (*fprintf_func)( FILE *, const char *, ... ) )
{
    mbedtls_fprintf = fprintf_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_FPRINTF_ALT */

#if defined(MBEDTLS_PLATFORM_EXIT_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_EXIT)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static void platform_exit_uninit( int status )
{
    ((void) status);
}

#define MBEDTLS_PLATFORM_STD_EXIT   platform_exit_uninit
#endif /* !MBEDTLS_PLATFORM_STD_EXIT */

void (*mbedtls_exit)( int status ) = MBEDTLS_PLATFORM_STD_EXIT;

int mbedtls_platform_set_exit( void (*exit_func)( int status ) )
{
    mbedtls_exit = exit_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_EXIT_ALT */

#if defined(MBEDTLS_HAVE_TIME)

#if defined(MBEDTLS_PLATFORM_TIME_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_TIME)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static mbedtls_time_t platform_time_uninit( mbedtls_time_t* timer )
{
    ((void) timer);
    return( 0 );
}

#define MBEDTLS_PLATFORM_STD_TIME   platform_time_uninit
#endif /* !MBEDTLS_PLATFORM_STD_TIME */

mbedtls_time_t (*mbedtls_time)( mbedtls_time_t* timer ) = MBEDTLS_PLATFORM_STD_TIME;

int mbedtls_platform_set_time( mbedtls_time_t (*time_func)( mbedtls_time_t* timer ) )
{
    mbedtls_time = time_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_TIME_ALT */

#endif /* MBEDTLS_HAVE_TIME */

#if defined(MBEDTLS_ENTROPY_NV_SEED)
#if !defined(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS) && defined(MBEDTLS_FS_IO)
/* Default implementations for the platform independent seed functions use
 * standard libc file functions to read from and write to a pre-defined filename
 */
int mbedtls_platform_std_nv_seed_read( unsigned char *buf, size_t buf_len )
{
    FILE *file;
    size_t n;

    if( ( file = fopen( MBEDTLS_PLATFORM_STD_NV_SEED_FILE, "rb" ) ) == NULL )
        return( -1 );

    if( ( n = fread( buf, 1, buf_len, file ) ) != buf_len )
    {
        fclose( file );
        mbedtls_platform_zeroize( buf, buf_len );
        return( -1 );
    }

    fclose( file );
    return( (int)n );
}

int mbedtls_platform_std_nv_seed_write( unsigned char *buf, size_t buf_len )
{
    FILE *file;
    size_t n;

    if( ( file = fopen( MBEDTLS_PLATFORM_STD_NV_SEED_FILE, "w" ) ) == NULL )
        return -1;

    if( ( n = fwrite( buf, 1, buf_len, file ) ) != buf_len )
    {
        fclose( file );
        return -1;
    }

    fclose( file );
    return( (int)n );
}
#endif /* MBEDTLS_PLATFORM_NO_STD_FUNCTIONS */

#if defined(MBEDTLS_PLATFORM_NV_SEED_ALT)
#if !defined(MBEDTLS_PLATFORM_STD_NV_SEED_READ)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static int platform_nv_seed_read_uninit( unsigned char *buf, size_t buf_len )
{
    ((void) buf);
    ((void) buf_len);
    return( -1 );
}

#define MBEDTLS_PLATFORM_STD_NV_SEED_READ   platform_nv_seed_read_uninit
#endif /* !MBEDTLS_PLATFORM_STD_NV_SEED_READ */

#if !defined(MBEDTLS_PLATFORM_STD_NV_SEED_WRITE)
/*
 * Make dummy function to prevent NULL pointer dereferences
 */
static int platform_nv_seed_write_uninit( unsigned char *buf, size_t buf_len )
{
    ((void) buf);
    ((void) buf_len);
    return( -1 );
}

#define MBEDTLS_PLATFORM_STD_NV_SEED_WRITE   platform_nv_seed_write_uninit
#endif /* !MBEDTLS_PLATFORM_STD_NV_SEED_WRITE */

int (*mbedtls_nv_seed_read)( unsigned char *buf, size_t buf_len ) =
            MBEDTLS_PLATFORM_STD_NV_SEED_READ;
int (*mbedtls_nv_seed_write)( unsigned char *buf, size_t buf_len ) =
            MBEDTLS_PLATFORM_STD_NV_SEED_WRITE;

int mbedtls_platform_set_nv_seed(
        int (*nv_seed_read_func)( unsigned char *buf, size_t buf_len ),
        int (*nv_seed_write_func)( unsigned char *buf, size_t buf_len ) )
{
    mbedtls_nv_seed_read = nv_seed_read_func;
    mbedtls_nv_seed_write = nv_seed_write_func;
    return( 0 );
}
#endif /* MBEDTLS_PLATFORM_NV_SEED_ALT */
#endif /* MBEDTLS_ENTROPY_NV_SEED */

#if !defined(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)
/*
 * Placeholder platform setup that does nothing by default
 */
int mbedtls_platform_setup( mbedtls_platform_context *ctx )
{
    (void)ctx;

    return( 0 );
}

/*
 * Placeholder platform teardown that does nothing by default
 */
void mbedtls_platform_teardown( mbedtls_platform_context *ctx )
{
    (void)ctx;
}
#endif /* MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT */

#endif /* MBEDTLS_PLATFORM_C */
