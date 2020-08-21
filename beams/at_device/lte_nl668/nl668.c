

#include "./app_cfg.h"
#include "./nl668.h"
#include "./nl668_plug.h"
#include "../../at_helper/at_api.h"
#include "../../at_helper/at_helper.h"

THIS_FILE_NAME("nl668");

static const char * const AT_RCV_TCP_PREFIX = "\r\n+MIPRTCP:";
static const char * const AT_RCV_UDP_PREFIX = "\r\n+MIPRUDP:";



#define IP_LEN              16
#define MAX_SOCK_NUM        (4)

#define NL668_TCP_PROTO     (0)
#define NL668_UDP_PROTO     (1)
#define NL668_SSL_PROTO     (2)


static int32_t nl668_init   (void);
static int32_t nl668_deinit (void);
static int     nl668_get_imsi(char *buf, uint32_t len);
static int     nl668_get_imei(char *buf, uint32_t len);
static int32_t nl668_bind   (int32_t *id, const char *host, uint32_t port);
static int32_t nl668_connect(int32_t *id, const char *host, uint32_t port);
static int32_t nl668_sendto (int32_t id, const char *buf, uint32_t len, const char *ip, int port);
static int32_t nl668_send   (int32_t id, const char *buf, uint32_t len);
static int32_t nl668_close  (int32_t id);

const at_adaptor_api_t nl668_interface =
{
    .init           = nl668_init,
    .deinit         = nl668_deinit,
    
    .get_imsi       = nl668_get_imsi,
    .get_imei       = nl668_get_imei,

    .bind           = nl668_bind,
    .connect        = nl668_connect,

    .send           = nl668_send,
    .sendto         = nl668_sendto,

    .close          = nl668_close,
    
    .link_num       = MAX_SOCK_NUM,
};

static const char *apn = "cmnet";
static char imsi[16];
static char imei[16];


static int32_t nl668_at_echo_off(void)
{
    char *cmd = "ATE0\r";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t nl668_at_check_connection(void)
{
    char *cmd = "AT\r";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t nl668_at_check_sim(void)
{
    char *cmd = "AT+CPIN?\r";
    return at_cmd(cmd, strlen(cmd), "+CPIN:", NULL,NULL);
}

static char *nl668_at_get_imsi(void)
{
    char *cmd = "AT+CIMI?\r";
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CIMI:", tmpbuf, &buf_len) < 0)
        return NULL;
    
    strncpy(imsi, &tmpbuf[strlen("\r\n+CIMI: ")], UBOUND(imsi));
    imsi[15] = '\0';
    
    if (15 != int_str_len(imsi)) {
        return NULL;
    }
    
    return imsi;
}

static char *nl668_at_get_imei(void)
{
    char *cmd = "AT+CGSN=1\r";
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CGSN:", tmpbuf, &buf_len) < 0)
        return NULL;
    
    strncpy(imei, &tmpbuf[strlen("\r\n+CGSN: ")], UBOUND(imei));
    imei[15] = '\0';
    
    if (15 != int_str_len(imei)) {
        return NULL;
    }
    
    return imei;
}

static int32_t nl668_at_check_csq(void)
{
    char *cmd = "AT+CSQ?\r";
    return at_cmd(cmd, strlen(cmd), "+CSQ:", NULL,NULL);
}

static int32_t nl668_at_get_csq(void)
{
    char *cmd = "AT+CSQ?\r";
    int rssi = 0;
    int ber = 0;
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CSQ:", tmpbuf, &buf_len) < 0)
        return -1;
    
    sscanf(tmpbuf,"\r\n+CSQ:%u,%u",&rssi, &ber);
    return rssi;
}

static int32_t nl668_at_cgatt_attach(void)
{
	char *cmd = "AT+CGATT=1\r";
    return at_cmd(cmd, strlen(cmd), "OK", NULL, NULL);
}

static int32_t nl668_at_get_netstat(void)
{
	char *cmd = "AT+CGATT?\r";
    return at_cmd(cmd, strlen(cmd), "+CGATT: 1", NULL, NULL);
}

static int32_t nl668_at_ppp(void)
{
    int cmd_len = 0;
    char tmpbuf[128 + 64] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    int match;
    
    if (strlen(apn_info.name) && strlen(apn_info.usr) && strlen(apn_info.psw)) {
        cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPCALL=1,\"%s\",\"%s\",\"%s\"", apn_info.name, apn_info.usr, apn_info.psw);
    } else if (strlen(apn_info.name)) {
        cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPCALL=1,\"%s\"", apn_info.name);
    } else {
        cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPCALL=1,\"%s\"", apn);
    }
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "+MIPCALL:", "ERROR", tmpbuf, &buf_len, 35000);
    if (match != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t nl668_at_query_ip(int *pstatus, char *ip)
{
	char *cmd = "AT+MIPCALL?\r";
    char tmpbuf[40] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    int status = 0;
    char *str = NULL;
    
    if(at_cmd(cmd, strlen(cmd), "+MIPCALL:", tmpbuf, &buf_len) < 0) {
        return AT_FAILED;
    }
    
    str = find_string_by_n(tmpbuf, "+MIPCALL:", buf_len);
    sscanf(str,"+MIPCALL:%d",&status);
    *pstatus = status;
    if (status == 1) {
        sscanf(str, "+MIPCALL:%d,%s\r\n", &status, ip);
    } else {
        ip[0] = '\0';
    }
    return AT_OK;
}

static int32_t nl668_at_create_sock_and_connect(int socket, int proto, char *remote_ip, int remote_port)
{
    int cmd_len;
    int match;
    char tmpbuf[64] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);

    cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPOPEN=%d,,\"%s\",%d,%d", socket, remote_ip, remote_port, proto);
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "+MIPOPEN:", "ERROR", tmpbuf, &buf_len, 10000);
    if (match != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t nl668_at_create_sock_and_bind(int socket, int proto, int local_port, const char *remote_ip, int remote_port)
{
    int cmd_len;
    int match;
    char tmpbuf[64] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);

    cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPODM=%d,%d,%s,%d,%d", socket, local_port, remote_ip, remote_port, proto);
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", tmpbuf, &buf_len, 0);
    if (match != 0) {
        return AT_FAILED;
    }
    if (NULL != find_string_by_n(tmpbuf, "+MIPODM:", buf_len)) {
        return AT_OK;
    }

    return AT_FAILED;
}

static void nl668_at_close(int32_t id)
{
    char cmd[64] = {0};
    int cmd_len;

	cmd_len = snprintf(cmd, UBOUND(cmd), "AT+MIPCLOSE=%d,1", id);
	at_cmd_with_2_suffix(cmd, cmd_len, "+MIPCLOSE:", "ERROR", NULL, NULL, 0);
}

static int32_t nl668_at_data_format_cfg(void)
{
    char cmd[64] = {0};
    int cmd_len;

	cmd_len = snprintf(cmd, UBOUND(cmd), "AT+GTSET=\"SENDTIME\",1");
	if (0u != at_cmd_with_2_suffix(cmd, cmd_len, "OK", "ERROR", NULL, NULL, 0)) {
        return AT_FAILED;
    }

	cmd_len = snprintf(cmd, UBOUND(cmd), "AT+GTSET=\"IPRFMT\",2");
	if (0u != at_cmd_with_2_suffix(cmd, cmd_len, "OK", "ERROR", NULL, NULL, 0)) {
        return AT_FAILED;
    }
    
    return AT_OK;
}

static int32_t nl668_at_send(int32_t id, const char *buf, uint32_t len)
{
    char cmd[64] = {0};
    int cmd_len;
    
    cmd_len = snprintf(cmd, 64, "AT+MIPSEND=%d,%d", id, len);
    if (AT_OK != at_write(cmd, "OK", buf, len)) {
        return AT_FAILED;
    }
    
    cmd_len = snprintf(cmd, 64, "AT+MIPPUSH=%d", id);
    if (0 != at_cmd_with_2_suffix(cmd, cmd_len, "+MIPPUSH:", "ERROR", NULL, NULL, 5000)) {
        return AT_FAILED;
    }
    
    return AT_OK;
}






static int32_t nl668_bind(int32_t *id, const char *host, uint32_t port)
{
    return nl668_at_create_sock_and_bind((*id) + 1, NL668_UDP_PROTO, 2000, (char *)host, port);
}

/*
 *  \return     AT_FAILED: failed
 *              others: the socket ID.
 */
static int32_t nl668_connect(int32_t *id, const char *host, uint32_t port)
{
    return nl668_at_create_sock_and_connect((*id) + 1, NL668_TCP_PROTO, (char *)host, port);
}


static int32_t nl668_close(int32_t id)
{
    if (id >= MAX_SOCK_NUM) {
        DBG_LOG("invalid link id: %d", id);
        return AT_FAILED;
    }

    nl668_at_close(id + 1);

    return AT_OK;
}


static int32_t nl668_send(int32_t id, const char *buf, uint32_t len)
{
    if (id >= MAX_SOCK_NUM) {
        DBG_LOG("invalid link id: %d", id);
        return AT_FAILED;
    }

    return nl668_at_send(id + 1, buf, len);
}

static int32_t nl668_sendto(int32_t id, const char *buf, uint32_t len, const char *ipaddr, int port)
{
    return nl668_send(id, buf, len);
}



static int32_t nl668_tcp_data_handler(void *arg, const char *buf, uint32_t len)
{
    uint32_t linkid = 0, data_len = 0;
    char *p1;
    p1 = (char *)buf;
    
    //DBG_LOG("TCP rcv entry!");

    if (sscanf((char *)buf, "\r\n+MIPRTCP:%d,%d,", &linkid, &data_len) < 2) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    if ((linkid == 0u) || (linkid > 6u)) {
        DBG_LOG("invalid socket ID!");
        goto END;
    } else {
        linkid--;
    }
    
    p1 = strstr(p1, ",");
    p1 = strstr(++p1, ",");
    if (NULL == p1) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    p1++;

    if (data_len > len) {
        DBG_LOG("error !! receive data not complete data_len:%d len:%d", data_len, len);
        goto END;
    }
    
    at_api_handle_data(linkid, p1, data_len);

    return data_len;
    
END:
    return 0;
}

static int32_t nl668_udp_data_handler(void *arg, const char *buf, uint32_t len)
{
    char sourceIP[40];
    uint32_t sourcePort;
    uint32_t linkid = 0;
    uint32_t data_len = 0;
    char *p1 = (char *)buf;

    //DBG_LOG("UDP rcv entry!");

    if (sscanf((char *)buf, "\r\n+MIPRUDP:%[^,],%u,%u,%u", sourceIP, &sourcePort, &linkid, &data_len) < 4) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    if ((linkid == 0u) || (linkid > 6u)) {
        DBG_LOG("invalid socket ID!");
        goto END;
    } else {
        linkid--;
    }
    
    p1 = strstr(p1, ",");
    p1 = strstr(++p1, ",");
    p1 = strstr(++p1, ",");
    p1 = strstr(++p1, ",");
    if (NULL == p1) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    p1++;

    if (data_len > len) {
        DBG_LOG("error !! receive data not complete data_len:%d len:%d", data_len, len);
        goto END;
    }

    at_api_handle_data(linkid, p1, data_len);
    
    return data_len;
    
END:
    return 0;
}

static int32_t nl668_cmd_match(const char *buf, const char *featurestr, uint32_t len)
{
    return memcmp(buf, featurestr, len);
}



static int32_t nl668_init(void)
{
    int ret;
    int timecnt = 0;
    
    /* config AT engine. */
    static const at_config_t at_nl668_conf = {
        .name           = "lte_nl668",
        .usart_port     = 2,
        .buardrate      = 115200,
        .linkid_num     = MAX_SOCK_NUM,
        .user_buf_len   = 1024*2,
        .cmd_begin      = "\r\n",
        .line_end       = "\r\n",
        .timeout        = 1000,    //<ms
    };
    at_init(&at_nl668_conf);

    nl668_power_on();
    OS_TASK_SLEEP(4000);
    
	for (timecnt = 10; timecnt != 0u; timecnt--) {
        ret = nl668_at_check_connection();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(100);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot connect to AT device!");
        goto __err_exit;
	}
    
	for (timecnt = 20; timecnt != 0u; timecnt--) {
        ret = nl668_at_echo_off();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(100);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = nl668_at_data_format_cfg();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(100);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    char *pstring = NULL;
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        pstring = nl668_at_get_imei();
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
        pstring = nl668_at_get_imsi();
		if(pstring != NULL) {
			break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot read SIM card!");
        goto __err_exit;
	} else {
        RTT_LOG("imsi:%s", pstring);
        
        int8_t mnc = 0;
        mnc = (imsi[3] - '0') * 10 + (imsi[4] - '0');
        switch (mnc) {
            case 1:
            case 6:
            case 9:
                apn = "UNINET";
                break;
                
            case 0:
            case 2:
            case 4:
            case 7:
            default:
                apn = "CMNET";
                break;
        }
    }
    
    
    nl668_at_check_csq();
    int32_t rssi = nl668_at_get_csq();
    RTT_LOG("rssi:%d", rssi);
    
	for (timecnt = 20; timecnt != 0u; timecnt--) {
        ret = nl668_at_cgatt_attach();
		if(ret != AT_FAILED) {
			break;
		}
        OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: net error!");
        goto __err_exit;
	}

    //! wait for net to be ok.
	for (timecnt = 40; timecnt != 0u; timecnt--) {
        ret = nl668_at_get_netstat();
		if(ret != AT_FAILED) {
			break;
		}
        OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: connect net timeout!");
        goto __err_exit;
	}
    
    
	for (timecnt = 3; timecnt != 0u; timecnt--) {
        ret = nl668_at_ppp();
		if(ret != AT_FAILED) {
			break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: PPP timeout!");
        goto __err_exit;
	}
    
	for (timecnt = 15; timecnt != 0u; timecnt--) {
        int status = 0;
        char ip[20] = {0};
        ret = nl668_at_query_ip(&status, ip);
		if(ret == AT_FAILED) {
			continue;
		}
        if (status == 1) {
            RTT_LOG("ip:%s", ip);
            break;
        } else {
            OS_TASK_SLEEP(2000);
        }
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot get IP!");
        goto __err_exit;
	}

    at_oob_register(AT_RCV_TCP_PREFIX, nl668_tcp_data_handler, nl668_cmd_match);
    at_oob_register(AT_RCV_UDP_PREFIX, nl668_udp_data_handler, nl668_cmd_match);

    DBG_LOG("nl668 init ok.");
    return AT_OK;
    
__err_exit:
    DBG_LOG("nl668 init fail!");
    nl668_power_off();
    at_deinit();
    DBG_LOG("nl668 power off.");
    return AT_FAILED;
}

static int32_t nl668_deinit(void)
{
    nl668_power_off();
    at_deinit();
    DBG_LOG("nl668 power off.");
    
    return AT_OK;
}

static int nl668_get_imsi(char *buf, uint32_t len)
{
    memcpy(buf, imsi, 15);
    buf[15] = '\0';
    return 15;
}

static int nl668_get_imei(char *buf, uint32_t len)
{
    memcpy(buf, imei, 15);
    buf[15] = '\0';
    return 15;
}