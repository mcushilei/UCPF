/*******************************************************************************
 *  Copyright(C)2015-2019 by Dreistein<mcu_shilei@hotmail.com>                *
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
#include "./app_cfg.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
THIS_FILE_NAME("template");

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

static void my_debug( void *ctx, int level, const char *file, int line, const char *str )
{
	printf(" \r\n %s:%04d: %s", file, line, str );
}

void mbedtls_param_failed ( const char *failure_condition,
							const char *file,
							int line )
{
	printf(" \r\n %s:%04d: %s", file, line, failure_condition );
}

static const char *ca_crt = {
    "-----BEGIN CERTIFICATE-----\r\n"
	"MIIDmDCCAoCgAwIBAgIUH7tF45N1YhwRPciN1KuztHo86UgwDQYJKoZIhvcNAQEL\r\n"
	"BQAwVTELMAkGA1UEBhMCQ04xEDAOBgNVBAgMB0JlaWppbmcxDDAKBgNVBAcMA1NK\r\n"
	"UzEOMAwGA1UECgwFWmhlbmcxFjAUBgNVBAMMDU1RVFQtVExTLVRFU1QwHhcNMjAw\r\n"
	"ODIxMDc0NTE4WhcNMzAwODE5MDc0NTE4WjBVMQswCQYDVQQGEwJDTjEQMA4GA1UE\r\n"
	"CAwHQmVpamluZzEMMAoGA1UEBwwDU0pTMQ4wDAYDVQQKDAVaaGVuZzEWMBQGA1UE\r\n"
	"AwwNTVFUVC1UTFMtVEVTVDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\r\n"
	"ALaSWX4XJr3Tm23ClIXLbxuRXL8ajtY5g/JM2JDiEzHIoZO6ybFvfCODmlyf1hV5\r\n"
	"Dlhz/DDOcjLg/mZsJw+QyVVUamYHvQZPec0cviLYlq2y+TKISPeqFX8UrhOdBtXw\r\n"
	"OxNVkUKSTFM7KeUrDzbXmoiAah4qJ3UZCa1uVNcM/qLne1HhrZxQZCmgd3tqVfuI\r\n"
	"LEtA1mX1o37smgPv0LAoB3Goti3/G1Cz1gGGBc/4vMe+mgKGMjT5H12JcvVTtEgG\r\n"
	"t0HAILQ/QtyfUoeWTaL1DPNGUsnAZ58UUzdmxBzhtSMKLzgprPwJG/Qo44cEigkf\r\n"
	"EYc+yG/FiAQ8/GzienGvvm8CAwEAAaNgMF4wHQYDVR0OBBYEFAaq/V0NYvg3fzeG\r\n"
	"PRodcsgXeaxeMB8GA1UdIwQYMBaAFAaq/V0NYvg3fzeGPRodcsgXeaxeMAwGA1Ud\r\n"
	"EwQFMAMBAf8wDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3DQEBCwUAA4IBAQCVbfF/\r\n"
	"Pi6bZh0krUo65XOwhkXPsNFEsePLRH94RMVjRNtT8qN6LlHdY3OnK0yxlJ3UD/Ou\r\n"
	"pIPHK1nre+0KgzsbMY71OALFcIDZQribhV0/Su27qt32scStAxljfBW48pJ/0AWG\r\n"
	"KM4KxmrO8HwlNZbk4SqzuclesHQk+zav02yWxLek6fnfWp2+Fth4wNyiBi6P/G7K\r\n"
	"AgcOknTQQsGxR7mJ2C9sKFhFo/bxM95EAd9Jsp+RIOBXLJv9NWJOT5WuHXYsalFl\r\n"
	"Uci1av0TRvJn6MKYNc1pNcVsgWb8vGyF3iYxU443thlQ5u5xEzP1td7sTxlBddGo\r\n"
	"nQV4NYQIcu6SxrDq\r\n"
	"-----END CERTIFICATE-----"
};

static int server_certificate_verify(void *hostname, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    if (0 != *flags)
        printf(" \r\n ! server_certificate_verify failed returned 0x%04x\n", *flags);
    return *flags;
}

static int tls_entropy_source(void *data, uint8_t *output, size_t len, size_t *out_len)
{
    unsigned int seed = GetTickCount();
	static unsigned int last_seed;
    (void) data;

    last_seed += (seed >> ((seed ^ last_seed) % 3));
	srand(seed);
    seed = rand();

    if (len > sizeof(seed)) {
        len = sizeof(seed);
    }

    memcpy(output, &seed, len);
    *out_len = len;

    return 0;
}

mbedtls_net_context server_fd;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config ssl_conf;
mbedtls_x509_crt ca_cert;
unsigned char memory_buf[100 * 1024];

static int my_ssl_init(void)
{
	int rc;

	mbedtls_memory_buffer_alloc_init( memory_buf, sizeof(memory_buf) );
	mbedtls_debug_set_threshold(3);
	mbedtls_net_init( &server_fd );
	mbedtls_ssl_init( &ssl );
	mbedtls_ssl_config_init( &ssl_conf );
	mbedtls_x509_crt_init( &ca_cert );
	mbedtls_ctr_drbg_init( &ctr_drbg );
	mbedtls_entropy_init( &entropy );

	mbedtls_entropy_add_source(&entropy, tls_entropy_source, NULL, MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);

	if( ( rc = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0 ) ) != 0 )
	{
		printf( " \r\n ! mbedtls_ctr_drbg_seed returned 0x%04x",  (rc < 0 )? -rc : rc );
		goto exit;
	}

	if( (rc = mbedtls_ssl_config_defaults( &ssl_conf,
										   MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0 ) {
        printf(" \r\n ! mbedtls_ssl_config_defaults failed returned 0x%04x", (rc < 0 )? -rc : rc);
        goto exit;
    }

	mbedtls_ssl_conf_dbg(&ssl_conf, my_debug, NULL);

	mbedtls_ssl_conf_rng( &ssl_conf, mbedtls_ctr_drbg_random, &ctr_drbg );

	mbedtls_ssl_conf_authmode( &ssl_conf, MBEDTLS_SSL_VERIFY_REQUIRED );

	if( 0 != (rc = mbedtls_x509_crt_parse(&ca_cert, ca_crt, strlen(ca_crt) + 1)) ) {
        printf(" \r\n ! parse ca crt failed returned 0x%04x", (rc < 0 )? -rc : rc);
        goto exit;
    }

    mbedtls_ssl_conf_ca_chain(&ssl_conf, &ca_cert, NULL);

	mbedtls_ssl_conf_verify(&ssl_conf, server_certificate_verify, (void *)"59.110.243.144");

    //if ((rc = mbedtls_ssl_conf_own_cert(&ssl_conf, &client_cert, &private_key)) != 0) {
    //    printf(" \r\n ! mbedtls_ssl_conf_own_cert failed returned 0x%04x", (rc < 0 )? -rc : rc);
    //    goto exit;
    //}


    mbedtls_ssl_conf_read_timeout(&ssl_conf, 5000);

    if( 0 != (rc = mbedtls_ssl_setup(&ssl, &ssl_conf)) ) {
        printf(" \r\n ! mbedtls_ssl_setup failed returned 0x%04x", (rc < 0)? -rc : rc);
        goto exit;
    }

    if( 0 != (rc = mbedtls_ssl_set_hostname(&ssl, "59.110.243.144")) ) {
        printf(" \r\n ! mbedtls_ssl_set_hostname failed returned 0x%04x", (rc < 0)? -rc : rc);
        goto exit;
    }

    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

exit:

    return rc;
}

/*! \note user's application
 *  \param none
 *  \retval true hal deinitialization succeeded.
 */
int app_main(void)
{
	int rc;

    rc = my_ssl_init();
    if (0 != rc)
        goto exit;

    if (0 != (rc = mbedtls_net_connect(&server_fd, "59.110.243.144", "8883", MBEDTLS_NET_PROTO_TCP)))
        goto exit;

    while( 0 != (rc = mbedtls_ssl_handshake(&ssl)) ) {
        if (rc != MBEDTLS_ERR_SSL_WANT_READ && rc != MBEDTLS_ERR_SSL_WANT_WRITE) {
            printf(" \r\n ! mbedtls handshake failed returned 0x%04x", (rc < 0 )? -rc : rc);
            if (rc == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
                printf(" \r\n ! unable to verify the server's certificate");
            }
            goto exit;
        }
    }

    if( 0 != (rc = mbedtls_ssl_get_verify_result(&ssl)) ) {
        printf(" \r\n ! mbedtls_ssl_get_verify_result returned 0x%04x", (rc < 0 )? -rc : rc);
        goto exit;
    }

	size_t maxUse = 0, maxBlocks = 0;
	mbedtls_memory_buffer_alloc_max_get(&maxUse, &maxBlocks);
	printf( "\r\n maxUse = %u, maxBlocks = %u", maxUse, maxBlocks );


exit:

    return 0;
}









/* EOF */
