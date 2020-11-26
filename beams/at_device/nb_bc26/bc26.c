#include "./app_cfg.h"
#include "./bc26.h"
#include "./bc26_plug.h"
#include "../../at_helper/at_api.h"
#include "../../at_helper/at_helper.h"

THIS_FILE_NAME("bc26");


#define AT_SOCKET_UDP       (17u)
#define AT_SOCKET_TCP       (6u)

#define AT_DATA_LEN         (2048)
#define AT_MAX_PAYLOADLEN   (1024 + 512)
#define MAX_SOCK_NUM        (4)


static char *wbuf = NULL;
static char *rbuf = NULL;

static int32_t  nb_init    (uint32_t opt);
static int32_t  nb_deinit  (void);
static int32_t  nb_get_imsi(char *buf, uint32_t len);
static int32_t  nb_get_imei(char *buf, uint32_t len);
static int32_t  nb_bind    (int32_t *id, const char *host, uint32_t port);
static int32_t  nb_connect (int32_t *id, const char *host, uint32_t port);
static int32_t  nb_sendto  (int32_t id, const char *buf, uint32_t len, const char* ip, int port);
static int32_t  nb_send    (int32_t id, const char *buf, uint32_t len);
static int32_t  nb_close   (int32_t id);

const at_adaptor_api_t bc26_interface =
{
    .init       = nb_init,
    .deinit     = nb_deinit,
    
    .get_imsi   = nb_get_imsi,
    .get_imei   = nb_get_imei,

    .bind       = nb_bind,
    .connect    = nb_connect,

    .send       = nb_send,
    .sendto     = nb_sendto,

    .close      = nb_close,
    
    .resolve_domain_name = NULL,

    .link_num   = MAX_SOCK_NUM,
};

static char imsi[16];
static char imei[16];



#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR */
static char *strnstr(const char *s1, const char *s2, size_t len)
{
    size_t l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    while (len >= l2) {
        len--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}
#endif




static int32_t bc26_check_connection(void)
{
    char *cmd = "AT";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t bc26_echo_off(void)
{
    char *cmd = "ATE0";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t bc26_get_version(void)
{
    char *cmd = "ATI";
    char buf[64] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "OK", buf, &buf_len) < 0)
        return AT_FAILED;

    //DBG_LOG("%s", buf);
    return AT_OK;
}

static int32_t bc26_check_ue(void)
{
    const char *cmd = "AT+CFUN?";
    return at_cmd(cmd, strlen(cmd), "+CFUN: 1", NULL,NULL);
}

static int32_t bc26_enable_rf(void)
{
    char *cmd = "AT+CFUN=1";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc26_shutdown(void)
{
    char *cmd = "AT+CFUN=0";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static char *bc26_get_imei(void)
{
    char *cmd = "AT+CGSN=1";
    char buf[32] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "+CGSN:", buf, &buf_len) < 0)
        return NULL;
    
    strncpy(imei, &buf[strlen("\r\n+CGSN:")], UBOUND(imei));
    imei[15] = '\0';
    return imei;
}

static char *bc26_get_imsi(void)
{
    char *cmd = "AT+CIMI";
    char buf[32] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "OK", buf, &buf_len) < 0)
        return NULL;
    
    strncpy(imsi, &buf[strlen("\r\n")], UBOUND(imsi));
    imsi[15] = '\0';
    return imsi;
}

static int32_t bc26_disable_edrx(void)
{
    char *cmd = "AT+CEDRXS=0";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc26_disable_psm(void)
{
    char *cmd = "AT+CPSMS=0";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc26_set_apn(void)
{
    int cmd_len = 0;
    char cmd[128 + 64] = {0};
    uint32_t buf_len = UBOUND(cmd);
    int match;
    
    if (strlen(apn_info.name) && strlen(apn_info.usr) && strlen(apn_info.psw)) {
        cmd_len = snprintf(cmd, buf_len, "AT+QCGDEFCONT=\"IP\",\"%s\",\"%s\",\"%s\"", apn_info.name, apn_info.usr, apn_info.psw);
    } else if (strlen(apn_info.name)) {
        cmd_len = snprintf(cmd, buf_len, "AT+QCGDEFCONT=\"IP\",\"%s\"", apn_info.name);
    } else {
        cmd_len = snprintf(cmd, buf_len, "AT+QCGDEFCONT=\"IP\"");
    }
	match = at_cmd_with_2_suffix(cmd, cmd_len, "OK", "ERROR", cmd, &buf_len, 1000);
    if (match != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t bc26_check_csq(void)
{
    char *cmd = "AT+CSQ";
    return at_cmd(cmd, strlen(cmd), "+CSQ:", NULL,NULL);
}

static int32_t bc26_get_csq(void)
{
    char *cmd = "AT+CSQ";
    int rssi = -1;
    char buf[32] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "+CSQ:", buf, &buf_len) < 0) {
        return -1;
    }
    sscanf(buf, "%*s%d,", &rssi);
    return rssi;
}

static int32_t bc26_get_regstat(void)
{
	char *cmd = "AT+CEREG?";
    char buf[32] = {0};
    uint32_t buf_len = UBOUND(buf);
    uint32_t n = 0, stat = 0;
    
    if (at_cmd(cmd, strlen(cmd), "+CEREG:", buf, &buf_len) != AT_OK) {
        return AT_FAILED;
    }
    if (sscanf(buf, "\r\n+CEREG:%u,%u", &n, &stat) < 2) {
        return AT_FAILED;
    }
    if (stat != 1 && stat != 5) {
        return AT_FAILED;
    }
    
    return AT_OK;
}

static int32_t bc26_get_netstat(void)
{
	char *cmd = "AT+CGATT?";
    return at_cmd(cmd, strlen(cmd), "+CGATT: 1", NULL,NULL);
}

static int32_t bc26_query_ip(char *ip)
{
	char *cmd = "AT+CGPADDR=1";
    char buf[40] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "+CGPADDR: 1,", buf, &buf_len) < 0) {
        return AT_FAILED;
    }
    
    if (sscanf(buf, "%*[^+]+CGPADDR: 1,%s\r\n", ip) == 1) {
        return AT_OK;
    }
    return AT_FAILED;
}

static int32_t bc26_cfg_data_format(void)
{
    int32_t rc = 0;
    char *cmd = NULL;
    
    cmd = "AT+QICFG=\"dataformat\",1,1";
    rc |= at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
    cmd = "AT+QICFG=\"viewmode\",1";
    rc |= at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
    cmd = "AT+QICFG=\"echomode\",0";
    rc |= at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
    
    return rc;
}

static int32_t bc26_create_sock_and_connect(int socket, int local_port, const char *remote_ip, int remote_port)
{
    int cmd_len;
	int ret;
    char tmpbuf[184];

    cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+QIOPEN=1,%d,\"TCP\",\"%s\",%d,%d,1,0", socket, remote_ip, remote_port, local_port);
	ret = at_cmd_with_2_suffix(tmpbuf, cmd_len, "+QIOPEN:", "ERROR", NULL, NULL, 30 * 1000);
    if (ret != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t bc26_at_create_sock_and_bind(int socket, int local_port, const char *remote_ip, int remote_port)
{
    int cmd_len;
	int ret;
    char tmpbuf[184];

    cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+QIOPEN=1,%d,\"UDP\",\"%s\",%d,%d,1,0", socket, remote_ip, remote_port, local_port);
	ret = at_cmd_with_2_suffix(tmpbuf, cmd_len, "+QIOPEN:", "ERROR", NULL, NULL, 30 * 1000);
    if (ret != 0) {
        return AT_FAILED;
    }
    return AT_OK;
}

static void bc26_close_sock(int sock)
{
    char tmpbuf[24];
    int cmd_len;

	cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+QICLOSE=%d", sock);
	at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", NULL,NULL, 1000);
}

static int32_t bc26_send(int32_t id , const char *buf, uint32_t len)
{
    int cmd_len;

    if ((len * 2) > AT_MAX_PAYLOADLEN) {
        DBG_LOG("data length too long: %u", len);
        return AT_FAILED;
    }

	cmd_len = snprintf(wbuf, AT_DATA_LEN, "AT+QISENDEX=%d,%d,", id, len);
	encode_hex_str(buf, len, wbuf + cmd_len);
    cmd_len += len * 2;
    wbuf[cmd_len]   = '\0';

	if (at_cmd_with_2_suffix(wbuf, cmd_len, "OK", "ERROR", NULL, NULL, 10000) != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t bc26_data_handler(void *arg, const char *buf, uint32_t len)
{
    int32_t socket = 0, data_len = 0;
    char *p1;
    p1 = (char *)buf;
    char str[16] = 0;
    
    //! parser data frame ,\r\n+QIURC: "recv",socket,length,data
    if (sscanf((char *)buf, "%*s%*[^\"]%16[^,]", str) < 1) {
        DBG_LOG("at cmd invailed!");
        goto END;
    }
    
    if( 0 != strncmp(str, "\"recv\"", 15) ) {
        goto END;
    }
    
    if (sscanf((char *)buf, "%*s%*[^,],%d,%d,", &socket, &data_len) < 2) {
        DBG_LOG("at cmd invailed!");
        goto END;
    }
    
    p1 = strstr(p1, ",");
    p1 = strstr(++p1, ",");
    p1 = strstr(++p1, ",");
    if (NULL == p1) {
        DBG_LOG("at cmd invailed!");
        goto END;
    }
    p1 += 1;
    if ((data_len * 2) > (len - ((size_t)p1 - (size_t)buf))) {
        DBG_LOG("at cmd incomplete!");
        goto END;
    }
    if ((data_len * 2) > AT_MAX_PAYLOADLEN) {
        DBG_LOG("data length too long: %u", data_len);
        goto END;
    }
    decode_hex_str(p1, data_len * 2, rbuf);
    at_api_handle_data(socket, rbuf, data_len);
    
    return data_len;
    
END:
    return 0;
}

static int32_t bc26_cmd_match(const char *buf, const char *featurestr, uint32_t len)
{
    return memcmp(buf, featurestr, len);
}





static int32_t nb_bind(int32_t *id, const char *host, uint32_t port)
{
    int rc;

    rc = bc26_at_create_sock_and_bind(*id, 0, host, port);
	if (rc < 0) {
        bc26_close_sock(*id);
		return AT_FAILED;
	}
    return AT_OK;
}

static int32_t nb_connect(int32_t *id, const char *host, uint32_t port)
{
    int rc;

    rc = bc26_create_sock_and_connect(*id, 0, host, port);
	if (rc < 0) {
        bc26_close_sock(*id);
		return AT_FAILED;
	}
    return AT_OK;
}

static int32_t nb_close(int32_t id)
{
    bc26_close_sock(id);

    return AT_OK;
}

static int32_t nb_send(int32_t id, const char *buf, uint32_t len)
{
    return bc26_send(id, buf, len);
}

static int32_t nb_sendto(int32_t id, const char *buf, uint32_t len, const char *ip, int port)
{
    return bc26_send(id, buf, len);
}

static int32_t nb_init(uint32_t opt)
{
    int ret;
    int timecnt = 0;
    
    /* config AT engine. */
    static const at_config_t atCfg = {
        .name           = "nb_bc26",
        .usart_port     = 2,
        .buardrate      = 57600,
        .linkid_num     = MAX_SOCK_NUM,
        .user_buf_len   = 1024 * 2,
        .cmd_begin      = "\r\n",
        .line_end       = "\r\n",
        .timeout        = 1000,    //<ms
    };
    char *pstring = NULL;
    int32_t rssi;
    char ip[20] = {'\0'};

    at_init(&atCfg);
    
    wbuf = at_malloc(AT_DATA_LEN);
    rbuf = at_malloc(AT_DATA_LEN);
    if (NULL == wbuf || NULL == rbuf) {
        goto __err_exit;
    }

    bc26_power_on();
    OS_TASK_SLEEP(6000);
    
	for (timecnt = 10; timecnt != 0u; timecnt--) {
        ret = bc26_check_connection();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(2000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot connect to AT device!");
        goto __err_exit;
	}
    
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc26_echo_off();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    

	for (timecnt = 5; timecnt != 0u; timecnt--) {
        pstring = bc26_get_imei();
		if(pstring != NULL) {
			break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot read IMEI!");
        goto __err_exit;
    } else {
        RTT_LOG("imei:%s", pstring);
    }
    
    pstring = NULL;
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        pstring = bc26_get_imsi();
		if(pstring != NULL) {
			break;
		}
        OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot read SIM card!");
        goto __err_exit;
	} else {
        RTT_LOG("imsi:%s", pstring);
    }
    
	for (timecnt = 10; timecnt != 0u; timecnt--) {
        ret = bc26_check_ue();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: AT device error!");
        goto __err_exit;
	}    
    
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc26_disable_edrx();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc26_disable_psm();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    

    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc26_cfg_data_format();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    
    
    bc26_check_csq();
    rssi = bc26_get_csq();
    RTT_LOG("rssi:%d", rssi);
    
    
	for (timecnt = 60; timecnt != 0u; timecnt--) {
        ret = bc26_get_regstat();
		if (ret == AT_OK) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: reg net timeout!");
        goto __err_exit;
	}

    //! wait for the net to be ok.
	for (timecnt = 20; timecnt != 0u; timecnt--) {
        ret = bc26_get_netstat();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: connect net timeout!");
        goto __err_exit;
	}
    
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc26_query_ip(ip);
		if(ret == AT_OK) {
            break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: get IP timeout!");
        goto __err_exit;
	}
    RTT_LOG("ip:%s", ip);
    
    at_oob_register("\r\n+QIURC: \"recv\"", bc26_data_handler, bc26_cmd_match);

    RTT_LOG("at device ok.");
    return AT_OK;

__err_exit:
    if (NULL != wbuf) {
        at_free(wbuf);
    }
    if (NULL != rbuf) {
        at_free(rbuf);
    }
    RTT_LOG("init at device error.");
    bc26_power_off();
    at_deinit();
    DBG_LOG("at device power off.");
    return AT_FAILED;
}

static int32_t nb_deinit(void)
{
    bc26_shutdown();
    OS_TASK_SLEEP(6000);
    bc26_enable_rf();
    bc26_power_off();
    at_deinit();
    DBG_LOG("at device power off.");
    
    return AT_OK;
}








static int nb_get_imsi(char *buf, uint32_t len)
{
    memcpy(buf, imsi, 15);
    buf[15] = '\0';
    return 15;
}

static int nb_get_imei(char *buf, uint32_t len)
{
    memcpy(buf, imei, 15);
    buf[15] = '\0';
    return 15;
}
