
#include "./app_cfg.h"
#include "./n700.h"
#include "./n700_plug.h"
#include "../../at_helper/at_api.h"
#include "../../at_helper/at_helper.h"

THIS_FILE_NAME("n700");


static const char * const AT_RCV_TCP_PREFIX = "\r\n+MIPRTCP:";
static const char * const AT_RCV_UDP_PREFIX = "\r\n+MIPRUDP:";



#define IP_LEN              16
#define MAX_SOCK_NUM        (4)

#define N700_TCP_PROTO      (0)
#define N700_UDP_PROTO      (1)
#define N700_SSL_PROTO      (2)


static int32_t n700_init(uint32_t opt);
static int32_t n700_deinit(void);
static int     n700_get_imsi(char *buf, uint32_t len);
static int     n700_get_imei(char *buf, uint32_t len);
static int32_t n700_bind    (int32_t *id, const char *host, uint32_t port);
static int32_t n700_connect (int32_t *id, const char *host, uint32_t port);
static int32_t n700_sendto  (int32_t id, const char *buf, uint32_t len, const char *ip, int port);
static int32_t n700_send    (int32_t id, const char *buf, uint32_t len);
static int32_t n700_close   (int32_t id);

at_adaptor_api_t n700_interface =
{
    .init           = n700_init,
    .deinit         = n700_deinit,
    
    .get_imsi       = n700_get_imsi,
    .get_imei       = n700_get_imei,
    
    .bind           = n700_bind,
    .connect        = n700_connect,

    .send           = n700_send,
    .sendto         = n700_sendto,

    .close          = n700_close,
    
    .resolve_domain_name = NULL,

    .link_num       = MAX_SOCK_NUM,
};

static const char *apn = "cmnet";
static char imsi[16];
static char imei[16];



static int32_t n700_at_echo_off(void)
{
    char *cmd = "ATE0\r";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t n700_at_check_connection(void)
{
    char *cmd = "AT\r";
    return at_cmd(cmd, strlen(cmd), "OK\r\n", NULL,NULL);
}

static int32_t n700_at_check_sim(void)
{
    char *cmd = "AT+CPIN?\r";
    return at_cmd(cmd, strlen(cmd), "+CPIN:", NULL,NULL);
}

static char *n700_at_get_imsi(void)
{
    char *cmd = "AT+CIMI?\r";
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CIMI:", tmpbuf, &buf_len) < 0)
        return NULL;
    
    strncpy(imsi, &tmpbuf[strlen("\r\n+CIMI:")], UBOUND(imsi));
    imsi[15] = '\0';
    return imsi;
}

static char *n700_at_get_imei(void)
{
    char *cmd = "AT+CGSN=1\r";
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CGSN:", tmpbuf, &buf_len) < 0)
        return NULL;
    
    strncpy(imei, &tmpbuf[strlen("\r\n+CGSN: ")], UBOUND(imei));
    imei[15] = '\0';
    return imei;
}

static int32_t n700_at_check_csq(void)
{
    char *cmd = "AT+CSQ?\r";
    return at_cmd(cmd, strlen(cmd), "+CSQ:", NULL,NULL);
}

static int32_t n700_at_get_csq(void)
{
    char *cmd = "AT+CSQ?\r";
    int csq = 0;
    char tmpbuf[32] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    
    if(at_cmd(cmd, strlen(cmd), "+CSQ:", tmpbuf, &buf_len) < 0)
        return -1;
    
    sscanf(tmpbuf,"+CSQ:%d,99",&csq);
    return csq;
}

static int32_t n700_at_cgatt_attach(void)
{
	char *cmd = "AT+CGATT=1\r";
    return at_cmd(cmd, strlen(cmd), "OK", NULL, NULL);
}

static int32_t n700_at_get_netstat(void)
{
	char *cmd = "AT+CGATT?\r";
    return at_cmd(cmd, strlen(cmd), "+CGATT: 1", NULL, NULL);
}

static int32_t n700_at_ppp(void)
{
    int cmd_len = 0;
    char tmpbuf[24] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);
    int match;
    
    if (NULL != apn) {
        cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPCALL=1,\"%s\"", apn);
    } else {
        cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPCALL=1");
    }
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", tmpbuf, &buf_len, 30000);
    if (match != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t n700_at_query_ip(int *pstatus, char *ip)
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

static int32_t n700_at_create_sock_and_connect(int socket, const char *remote_ip, int remote_port)
{
    int cmd_len;
    int match;
    char tmpbuf[64] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);

    cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPOPEN=%d,,\"%s\",%d,%d", socket, remote_ip, remote_port, N700_TCP_PROTO);
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "+MIPOPEN:", "ERROR", tmpbuf, &buf_len, 10000);
    if (match != 0) {
        return AT_FAILED;
    }

    return AT_OK;
}

static int32_t n700_at_create_sock_and_bind(int socket, int local_port, const char *remote_ip, int remote_port)
{
    int cmd_len;
    int match;
    char tmpbuf[64] = {0};
    uint32_t buf_len = UBOUND(tmpbuf);

    cmd_len = snprintf(tmpbuf, buf_len, "AT+MIPODM=%d,%d,%s,%d,%d", socket, local_port, remote_ip, remote_port, N700_UDP_PROTO);
	match = at_cmd_with_2_suffix(tmpbuf, cmd_len, "OK", "ERROR", tmpbuf, &buf_len, 0);
    if (match != 0) {
        return AT_FAILED;
    }
    if (NULL != find_string_by_n(tmpbuf, "+MIPODM:", buf_len)) {
        return AT_OK;
    }

    return AT_FAILED;
}

static void n700_at_close(int32_t id)
{
    char cmd[64] = {0};
    int cmd_len;

	cmd_len = snprintf(cmd, UBOUND(cmd), "AT+MIPCLOSE=%d,1", id);
	at_cmd_with_2_suffix(cmd, cmd_len, "+MIPCLOSE:", "ERROR", NULL, NULL, 0);
}







static int32_t n700_bind(int32_t *id, const char *host, uint32_t port)
{
    return n700_at_create_sock_and_bind(*id, 2000, host, port);
}

static int32_t n700_connect(int32_t *id, const char *host, uint32_t port)
{
    return n700_at_create_sock_and_connect((*id) + 1, host, port);
}

static int32_t n700_close(int32_t id)
{
    if (id >= MAX_SOCK_NUM) {
        DBG_LOG("link id %d invalid", id);
        return AT_FAILED;
    }

    n700_at_close(id + 1);

    return AT_OK;
}


static int32_t n700_send(int32_t id , const char *buf, uint32_t len)
{
    int32_t ret = AT_FAILED;
    int cmd_len;
    char *pbuf;
    
	if (id >= MAX_SOCK_NUM) {
        DBG_LOG("invalid args");
        return AT_FAILED;
    }
    id++;
    
    char *cmd = at_malloc(len * 2 + 64);
    if (cmd == NULL) {
        DBG_LOG("Not enough memory, need %u", len * 2 + 64);
        return AT_FAILED;
    }

    cmd_len = snprintf(cmd, len * 2 + 64, "AT+MIPSEND=%d,", id);
    pbuf = cmd + cmd_len;
    *(pbuf++) = '\"';
    for (uint32_t i = 0; i < len; i++) {
        snprintf(pbuf, 3, "%02X", buf[i]);
        pbuf += 2;
    }
    *(pbuf++) = '\"';
    *(pbuf++) = '\r';
    cmd_len += len * 2 + 3;
    cmd[cmd_len] = '\0';

    if (0 != at_cmd_with_2_suffix(cmd, cmd_len, "+MIPSEND:", "ERROR", NULL, NULL, 0)) {
        goto __end;
    }
    
    cmd_len = snprintf(cmd, 64, "AT+MIPPUSH=%d", id);
    if (0 != at_cmd_with_2_suffix(cmd, cmd_len, "+MIPPUSH:", "ERROR", NULL, NULL, 0)) {
        goto __end;
    }
    
    ret = AT_OK;
__end:
    at_free(cmd);
    return ret;
}

static int32_t n700_sendto(int32_t id, const char *buf, uint32_t len, const char* ipaddr, int port)
{
    return n700_send(id, buf, len);
}


static char dataBuf[1024 + 512];

static int32_t n700_tcp_data_handler(void *arg, const char *buf, uint32_t len)
{
    int32_t linkid = 0, data_len = 0;
    char *p1;
    p1 = (char *)buf;
    

    //DBG_LOG("entry!");

    //! parser data frame ,\r\n+MIPRTCP,linkid,left,data
    if (sscanf((char *)buf, "\r\n+MIPRTCP:%d,", &linkid) < 1) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    if ((linkid == 0u) || (linkid > 4u)) {
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
    data_len = len - ((size_t)p1 - (size_t)buf) - 1u;
    data_len &= ~(uint32_t)0x01;

    if ((data_len / 2u) > sizeof(dataBuf)) {
        DBG_LOG("data too large!");
        goto END;
    }
    decode_hex_str(p1, data_len, dataBuf);
    at_api_handle_data(linkid, dataBuf, data_len / 2u);
    
    return data_len;
    
END:
    return 0;
}

static int32_t n700_udp_data_handler(void *arg, const char *buf, uint32_t len)
{
    char sourceIP[40];
    int sourcePort;
    int linkid = 0;
    int data_len = 0;
    char *p1 = (char *)buf;

    DBG_LOG("entry!");

    //! parser data frame ,\r\n+MIPRTCP,linkid,len,data
    if (sscanf((char *)buf, "\r\n+MIPRUDP:%[^,],%d,%d,%d", sourceIP, &sourcePort, &linkid, &data_len) < 4) {
        DBG_LOG("got data prefix invailed!");
        goto END;
    }
    if ((linkid == 0u) || (linkid > 4u)) {
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
    data_len = len - ((size_t)p1 - (size_t)buf) - 1u;
    data_len &= ~(uint32_t)0x01;

    if ((data_len / 2u) > sizeof(dataBuf)) {
        DBG_LOG("data too large!");
        goto END;
    }
    decode_hex_str(p1, data_len, dataBuf);
    at_api_handle_data(linkid, dataBuf, data_len / 2u);

    return data_len;
    
END:
    return 0;
}

static int32_t n700_cmd_match(const char *buf, const char *featurestr, uint32_t len)
{
    return memcmp(buf, featurestr, len);
}



static int32_t n700_init(uint32_t opt)
{
    int ret;
    int timecnt = 0;
    
    /* config AT engine. */
    static const at_config_t atCfg = {
        .name           = "lte_n700",
        .usart_port     = 2,
        .buardrate      = 115200,
        .linkid_num     = MAX_SOCK_NUM,
        .user_buf_len   = 1024 * 2,
        .cmd_begin      = "\r\n",
        .line_end       = "\r\n",
        .timeout        = 1000,    //<ms
    };
    at_init(&atCfg);

    
    // refer to <FIBOCOM N 700 应用指南_TCP_V1.0.0.pdf>
    
    n700_power_on();
    OS_TASK_SLEEP(8000);
    
	for (timecnt = 20; timecnt != 0u; timecnt--) {
        ret = n700_at_check_connection();
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
        ret = n700_at_echo_off();
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
        pstring = n700_at_get_imei();
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
        pstring = n700_at_get_imsi();
		if(pstring != NULL) {
			break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("\r\n cannot read SIM card!");
        goto __err_exit;
	} else {
        RTT_LOG("imsi:%s", pstring);
        
        int8_t mnc = 0;
        mnc = (imsi[3] - '0') * 10 + (imsi[4] - '0');
        switch (mnc) {
            case 11:
                apn = "CTLTE";
                break;
                
            case 1:
            case 6:
            case 9:
                apn = "UNINET";
                break;
                
            case 0:
            case 2:
            case 4:
            case 7:
                apn = "CMNET";
                break;
                
            default:
                apn = NULL;
                break;
        }
    }
    
    n700_at_check_csq();
    OS_TASK_SLEEP(3000);
    int32_t rssi = n700_at_get_csq();
    RTT_LOG("rssi:%d", rssi);
    
	for (timecnt = 20; timecnt != 0u; timecnt--) {
        ret = n700_at_cgatt_attach();
		if(ret != AT_FAILED) {
			break;
		}
        OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: net error!");
        goto __err_exit;
	}
    DBG_LOG("cgatt ok");

    //! wait for net to be ok.
	for (timecnt = 40; timecnt != 0u; timecnt--) {
        ret = n700_at_get_netstat();
		if(ret != AT_FAILED) {
			break;
		}
		OS_TASK_SLEEP(1000);
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: connect net timeout!");
        goto __err_exit;
	}
    DBG_LOG("netstat ok");
    
	for (timecnt = 3; timecnt != 0u; timecnt--) {
        ret = n700_at_ppp();
		if(ret != AT_FAILED) {
			break;
		}
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: PPP timeout!");
        goto __err_exit;
	}
    
	for (timecnt = 50; timecnt != 0u; timecnt--) {
        int status = 0;
        char ip[20] = {'\0'};
        ret = n700_at_query_ip(&status, ip);
		if(ret == AT_FAILED) {
			continue;
		}
        if (status == 1) {
            RTT_LOG("ip:%s", ip);
            break;
        } else {
            //DBG_LOG("ppp ...");
            OS_TASK_SLEEP(4000);
        }
	}
	if(timecnt == 0u) {
        RTT_LOG("ERROR: cannot get IP!");
        goto __err_exit;
	}
    
    at_oob_register(AT_RCV_TCP_PREFIX, n700_tcp_data_handler, n700_cmd_match);
    at_oob_register(AT_RCV_UDP_PREFIX, n700_udp_data_handler, n700_cmd_match);

    DBG_LOG("\r\n n700 init ok.");
    return AT_OK;

__err_exit:
    DBG_LOG("\r\n n700 init err.");
    at_deinit();
    n700_power_off();
    DBG_LOG("\r\n n700 power off.");
    return AT_FAILED;
}

static int32_t n700_deinit(void)
{
    at_deinit();
    n700_power_off();
    DBG_LOG("\r\n n700 power off.");
    
    return AT_OK;
}

static int n700_get_imsi(char *buf, uint32_t len)
{
    memcpy(buf, imsi, 15);
    buf[15] = '\0';
    return 15;
}

static int n700_get_imei(char *buf, uint32_t len)
{
    memcpy(buf, imei, 15);
    buf[15] = '\0';
    return 15;
}
