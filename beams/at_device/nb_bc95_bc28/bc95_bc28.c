#include "./app_cfg.h"
#include "./bc95_bc28.h"
#include "./bc95_bc28_plug.h"
#include "../../at_helper/at_api.h"
#include "../../at_helper/at_helper.h"

THIS_FILE_NAME("bc95");


#define AT_SOCKET_UDP       (17u)
#define AT_SOCKET_TCP       (6u)

#define AT_DATA_LEN         (2048)
#define AT_MAX_PAYLOADLEN   (1024 + 512)
#define MAX_SOCK_NUM        (4)


static char *wbuf = NULL;
static char *rbuf = NULL;

static int32_t  nb_bc28_init    (uint32_t opt);
static int32_t  nb_bc28_deinit  (void);
static int32_t  nb_get_imsi(char *buf, uint32_t len);
static int32_t  nb_get_imei(char *buf, uint32_t len);
static int32_t  nb_bind    (int32_t *id, const char *host, uint32_t port);
static int32_t  nb_connect (int32_t *id, const char *host, uint32_t port);
static int32_t  nb_sendto  (int32_t id, const char *buf, uint32_t len, const char* ip, int port);
static int32_t  nb_send    (int32_t id, const char *buf, uint32_t len);
static int32_t  nb_close   (int32_t id);
static int32_t  bc95_resolve_domain_name(const char *name, char *ip);

const at_adaptor_api_t bc95_bc28_interface =
{
    .init       = nb_bc28_init,
    .deinit     = nb_bc28_deinit,
    
    .get_imsi   = nb_get_imsi,
    .get_imei   = nb_get_imei,

    .bind       = nb_bind,
    .connect    = nb_connect,

    .send       = nb_send,
    .sendto     = nb_sendto,

    .close      = nb_close,
    
    .resolve_domain_name = bc95_resolve_domain_name,

    .link_num   = MAX_SOCK_NUM,
};

static char imsi[16];
static char imei[16];


static int32_t bc95_check_connection(void)
{
    char *cmd = "AT";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t bc95_echo_off(void)
{
    char *cmd = "ATE0";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t bc95_get_version(void)
{
    char *cmd = "ATI";
    char buf[64] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "OK", buf, &buf_len) < 0)
        return AT_FAILED;

    //DBG_LOG("%s", buf);
    return AT_OK;
}

static int32_t bc95_reboot(void)
{
    char *cmd = "AT+NRB";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_check_ue(void)
{
    const char *cmd = "AT+CFUN?";
    return at_cmd(cmd, strlen(cmd), "+CFUN:1", NULL,NULL);
}

static int32_t bc95_disable_rf(void)
{
    char *cmd = "AT+CFUN=4,1";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_enable_rf(void)
{
    char *cmd = "AT+CFUN=1";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_shutdown(void)
{
    char *cmd = "AT+CFUN=0";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_ncsearfcn(void)
{
    char *cmd = "AT+NCSEARFCN";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_disable_huawei_iot(void)
{
    char *cmd = "AT+QREGSWT=2";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static char *bc95_get_imsi(void)
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

static char *bc95_get_imei(void)
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

static int32_t bc95_disable_edrx(void)
{
    char *cmd = "AT+CEDRXS=0,5";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_disable_psm(void)
{
    char *cmd = "AT+CPSMS=0";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

// \note NB has no usr and psw.
static int32_t bc95_set_pdp(void)
{
    int cmd_len = 0;
    char cmd[128] = {0};
    uint32_t buf_len = UBOUND(cmd);
    
    if (strlen(apn_info.name)) {
        DBG_LOG("%s", apn_info.name);
        cmd_len = snprintf(cmd, buf_len, "AT+CGDCONT=0,\"IP\",\"%s\"", apn_info.name);
        return at_cmd(cmd, cmd_len, "OK", NULL,NULL);
    } else {
        return AT_OK;
    }
}

static int32_t bc95_cfg_data_rcv(void)
{
    char *cmd = "AT+NSONMI=3";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_check_csq(void)
{
    char *cmd = "AT+CSQ";
    return at_cmd(cmd, strlen(cmd), "+CSQ:", NULL,NULL);
}

static int32_t bc95_get_csq(void)
{
    char *cmd = "AT+CSQ";
    int rssi = -1;
    char buf[32] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "+CSQ:", buf, &buf_len) < 0) {
        return -1;
    }
    sscanf(buf, "%*[^+]+CSQ:%d,", &rssi);		
    return rssi;
}

static int32_t bc95_set_creg(void)
{
    char *cmd = "AT+CEREG=2";
    return at_cmd(cmd, strlen(cmd), "OK", NULL,NULL);
}

static int32_t bc95_get_regstat(void)
{
	char *cmd = "AT+CEREG?";
    char buf[64] = {0};
    uint32_t buf_len = UBOUND(buf);
    uint32_t n = 0, stat = 0;
    
    if (at_cmd(cmd, strlen(cmd), "+CEREG:", buf, &buf_len) != AT_OK) {
        return AT_FAILED;
    }
    //DBG_LOG("%s", buf);
    if (sscanf(buf, "\r\n+CEREG:%u,%u", &n, &stat) < 2) {
        return AT_FAILED;
    }
    if (stat != 1 && stat != 5) {
        return AT_FAILED;
    }
    
    return AT_OK;
}

static int32_t bc95_get_netstat(void)
{
	char *cmd = "AT+CGATT?";
    return at_cmd(cmd, strlen(cmd), "+CGATT:1", NULL,NULL);
}

static int32_t bc95_query_ip(char *ip)
{
	char *cmd = "AT+CGPADDR=0";
    char buf[40] = {0};
    uint32_t buf_len = UBOUND(buf);
    
    if(at_cmd(cmd, strlen(cmd), "+CGPADDR:0,", buf, &buf_len) < 0) {
        return AT_FAILED;
    }
    
    if (sscanf(buf, "\r\n+CGPADDR:0,%s\r\n", ip) == 1) {
        return AT_OK;
    }
    return AT_FAILED;
}

static int32_t bc95_resolve_domain_name(const char *name, char *ip)
{
    char cmd[164] = {0};
    int cmd_len;
    uint32_t buf_len = UBOUND(cmd);
    char *str = NULL;
    
	cmd_len = snprintf(cmd, UBOUND(cmd), "AT+QDNS=0,\"%s\"", name);
    if (0 != at_cmd_with_2_suffix(cmd, cmd_len, "+QDNS:", "ERROR", cmd, &buf_len, 5000)) {
        return AT_FAILED;
    }
    
    str = find_string_by_n(cmd, "+QDNS:", buf_len);
    sscanf(str, "+QDNS:%s", ip);

    return AT_OK;
}

static int32_t bc95_create_sock(int port,int proto)
{
	int socket;
    int cmd_len;
	int ret;
    char tmpbuf[64];
    char buf[16] = {0};
    uint32_t buf_len = sizeof(buf);


    if (proto == AT_SOCKET_UDP) {
        cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+NSOCR=DGRAM,17,%d,1,AF_INET", port);
    } else {
        cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+NSOCR=STREAM,6,%d,1,AF_INET", port);
    }

	ret = at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", buf, &buf_len, 1000);
    if (ret == 0) {
        ret = sscanf(buf, "%d", &socket);
        if ((ret == 1) && (socket >= 0)) {
            DBG_LOG("create socket ok: %u", socket);
            return socket;
        }
    }

    return AT_FAILED;
}

static int32_t bc95_close_sock(int socket)
{
    char tmpbuf[64];
    uint32_t cmd_len;
    uint32_t ret;

	cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+NSOCL=%d", socket);
    ret = at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", tmpbuf, &cmd_len, 10000);
	if (ret != 0) {
        return AT_FAILED;
    }
    return AT_OK;
}

static int32_t bc95_connect(int32_t id, const char *ip, int port)
{
    int cmd_len;
    char tmpbuf[64];

	cmd_len = snprintf(tmpbuf, sizeof(tmpbuf), "AT+NSOCO=%d,%s,%d", id, ip, port);

	if (at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", NULL, NULL, 10000) != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t bc95_sendto(int32_t id , const char *buf, uint32_t len, const char *ip, int port)
{
    int cmd_len;

    if ((len * 2) > AT_MAX_PAYLOADLEN) {
        DBG_LOG("data length too long: %u", len);
        return AT_FAILED;
    }

	cmd_len = snprintf(wbuf, AT_DATA_LEN, "AT+NSOST=%d,%s,%d,%d,", id,
        ip, port, len);
	encode_hex_str(buf, len, wbuf + cmd_len);
    cmd_len += len * 2;
    wbuf[cmd_len]   = '\0';

	if (at_cmd_with_2_suffix(wbuf, cmd_len, "OK", "ERROR", NULL, NULL, 10000) != 0) {
        return AT_FAILED;
    }

    return len;
}

static int32_t bc95_send(int32_t id , const char *buf, uint32_t len)
{
    int cmd_len;

    if ((len * 2) > AT_MAX_PAYLOADLEN) {
        DBG_LOG("data length too long: %u", len);
        return AT_FAILED;
    }

	cmd_len = snprintf(wbuf, AT_DATA_LEN, "AT+NSOSD=%d,%d,", id, len);
	encode_hex_str(buf, len, wbuf + cmd_len);
    cmd_len += len * 2;
    wbuf[cmd_len]   = '\0';

	if (at_cmd_with_2_suffix(wbuf, cmd_len, "OK", "ERROR", NULL, NULL, 10000) != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t bc95_data_handler(void *arg, const char *buf, uint32_t len)
{
    int32_t socket = 0, data_len = 0;
    char *p1;
    p1 = (char *)buf;
    
    //! parser data frame ,\r\n+NSONMI:socket,length,data
    if (sscanf((char *)buf, "\r\n+NSONMI:%d,%d", &socket, &data_len) < 2) {
        DBG_LOG("at cmd invailed!");
        goto END;
    }
    
    p1 = strstr(p1, ",");
    p1 = strstr(++p1, ",");
    if (NULL == p1) {
        DBG_LOG("at cmd invailed!");
        goto END;
    }
    p1++;
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

static int32_t bc95_cmd_match(const char *buf, const char *featurestr, uint32_t len)
{
    return memcmp(buf, featurestr, len);
}







static int32_t nb_bind(int32_t *id, const char *host, uint32_t port)
{
    int socket;

    (void)host;
    
    socket = bc95_create_sock(0, AT_SOCKET_UDP);
	if (socket < 0) {
		return AT_FAILED;
	}
    *id = socket;
    return AT_OK;
}

static int32_t nb_connect(int32_t *id, const char *host, uint32_t port)
{
    int socket = -1;

    socket = bc95_create_sock(0, AT_SOCKET_TCP);
	if (socket < 0) {
        DBG_LOG("create socket fail!");
		return AT_FAILED;
	}
    
    if (bc95_connect(socket, host, port) != AT_OK) {
        DBG_LOG("connect fail!");
        bc95_close_sock(socket);
        *id = -1;
        return AT_FAILED;
    }
    
    *id = socket;
    return AT_OK;
}

static int32_t nb_close(int32_t id)
{
    if (id < 0) {
        DBG_LOG("invalid link id: %d", id);
        return AT_FAILED;
    }

    if (bc95_close_sock(id) != AT_OK) {
        DBG_LOG("close socket %i fail!", id);
    }

    return AT_OK;
}

static int32_t nb_send(int32_t id, const char *buf, uint32_t len)
{
    return bc95_send(id, buf, len);
}

static int32_t nb_sendto(int32_t id, const char *buf, uint32_t len, const char *ip, int port)
{
    return bc95_sendto(id, buf, len, ip, port);
}

static int32_t nb_bc28_init(uint32_t opt)
{
    int ret;
    int timecnt = 0;
    
    /* config AT engine. */
    static const at_config_t atCfg = {
        .name           = "nb_bc95_bc28",
        .usart_port     = 2,
        .buardrate      = 9600,
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

    bc95_bc28_power_on();
    OS_TASK_SLEEP(6000);
    
	for (timecnt = 10; timecnt != 0u; timecnt--) {
        ret = bc95_check_connection();
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
        ret = bc95_echo_off();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    bc95_get_version();
    
    if (0 != opt) {
        
        for (timecnt = 5; timecnt != 0u; timecnt--) {
            ret = bc95_shutdown();
            if(ret != AT_FAILED) {
                break;
            }
            OS_TASK_SLEEP(1000);
        }
        if(timecnt == 0u) {
            goto __err_exit;
        }
        
        for (timecnt = 5; timecnt != 0u; timecnt--) {
            ret = bc95_ncsearfcn();
            if(ret != AT_FAILED) {
                break;
            }
            OS_TASK_SLEEP(1000);
        }
        if(timecnt == 0u) {
            goto __err_exit;
        }
        
        RTT_LOG("bc95 NB reset.");
    }
    
	for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_enable_rf();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("bc95 NB net poweron fail.");
        goto __err_exit;
	}
    

	for (timecnt = 5; timecnt != 0u; timecnt--) {
        pstring = bc95_get_imei();
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
        pstring = bc95_get_imsi();
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
        ret = bc95_check_ue();
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
        ret = bc95_disable_huawei_iot();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_disable_edrx();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_disable_psm();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    

    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_cfg_data_rcv();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_set_pdp();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    for (timecnt = 5; timecnt != 0u; timecnt--) {
        ret = bc95_set_creg();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(500);
	}
	if(timecnt == 0u) {
        goto __err_exit;
	}
    
    
    bc95_check_csq();
    OS_TASK_SLEEP(3000);
    rssi = bc95_get_csq();
    RTT_LOG("rssi:%d", rssi);
    
    
	for (timecnt = 100; timecnt != 0u; timecnt--) {
        ret = bc95_get_regstat();
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
        ret = bc95_get_netstat();
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
        ret = bc95_query_ip(ip);
		if(ret == AT_OK) {
            break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: get IP timeout!");
        goto __err_exit;
	}
    RTT_LOG("ip:%s", ip);
    
    at_oob_register("\r\n+NSONMI:", bc95_data_handler, bc95_cmd_match);

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
    bc95_bc28_power_off();
    at_deinit();
    DBG_LOG("at device power off.");
    return AT_FAILED;
}

static int32_t nb_bc28_deinit(void)
{
    bc95_shutdown();
    OS_TASK_SLEEP(6000);        //!< refer to Quectel_BC35-G&BC28&BC95 R2.0_应用设计指导_V1.2.pdf, 2.1.2
    bc95_enable_rf();
    bc95_bc28_power_off();
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
