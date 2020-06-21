

#include "./app_cfg.h"
#include "./at_api.h"
#include "./at_helper.h"
#include "../deadline/deadline.h"

THIS_FILE_NAME("at_api");

#define SOCKET_STATUS_UNUSE		    0
#define SOCKET_STATUS_INUSE 		1
#define SOCKET_STATUS_SHUTDOWN 		2

enum {
    SOCKET_TYPE_NONE = 0,
    SOCKET_TYPE_TCP,
    SOCKET_TYPE_UDP,
};

#define AT_DEVICE_UNINIT    0
#define AT_DEVICE_OK 		1

#define AT_API_SOCKET_NUM       (2u)
#define AT_API_MAX_IP_LEN       (40u)



#define AT_API_MAX_FRAME_SIZE   AT_API_RECV_FIFO_SIZE


static const at_adaptor_api_t *at_adaptor_api = NULL;
static OS_HANDLE         at_adaptor_api_mutex = NULL;
static uint32_t          at_device_status = AT_DEVICE_UNINIT;
static socket_t          link_array[AT_API_SOCKET_NUM] = {0};

at_apn_info   apn_info;


bool at_api_init(at_option_t *opt)
{
    if (at_adaptor_api_mutex != NULL) {
        // we only init it once.
        return true;
    }
    
    memset(&apn_info, 0, sizeof(apn_info));
    if (NULL != opt) {
        if (NULL != opt->apn) {
            if (strlen(opt->apn) < sizeof(apn_info.name)) {
                strcpy(apn_info.name, opt->apn);
            } else {
                RTT_LOG("APN too long.");
                return false;
            }
        }
        if (NULL != opt->apn_user) {
            if (strlen(opt->apn_user) < sizeof(apn_info.usr)) {
                strcpy(apn_info.usr, opt->apn_user);
            } else {
                RTT_LOG("APN user too long.");
                return false;
            }
        }
        if (NULL != opt->apn_pswd) {
            if (strlen(opt->apn_pswd) < sizeof(apn_info.psw)) {
                strcpy(apn_info.psw, opt->apn_pswd);
            } else {
                RTT_LOG("APN pswd too long.");
                return false;
            }
        }
    }
    
    at_adaptor_api   = NULL;
    at_device_status = AT_DEVICE_UNINIT;
    memset(link_array, 0, sizeof(link_array));
    for (uint32_t i = 0; i < AT_API_SOCKET_NUM; i++) {
        link_array[i].so  = i;
        
        if (OS_FLAG_CREATE(link_array[i].RecvFlag, false, false) != OS_ERR_NONE) {
            RUNTIME_FAULT(-3, "init flag failed!");
        }
        
        if (OS_MUTEX_CREATE(link_array[i].RecvMutex) != OS_ERR_NONE) {
            RUNTIME_FAULT(-3, "init mutex failed!");
        }
        
    }
    
    OS_ERR err = OS_MUTEX_CREATE(at_adaptor_api_mutex);
    if (err != OS_ERR_NONE) {
        DBG_LOG("init at_adaptor_api_mutex failed!");
        return false;
    }
    
    return true;
}

bool at_api_deinit(void)
{
    at_api_deregister_adaptor();
    OS_MUTEX_DELETE(at_adaptor_api_mutex);
    at_adaptor_api_mutex = NULL;

    for (uint32_t i = 0; i < AT_API_SOCKET_NUM; i++) {
        OS_FLAG_DELETE(link_array[i].RecvFlag);
        OS_MUTEX_DELETE(link_array[i].RecvMutex);
    }
    
    return true;
}

/*
 *  \brief  refister the api which implemented by a particular AT device.
 *  \param  api     the api to be resistered.
 *  \return AT_OK   none error.
 *  \note   1) This is in Singleton pattern(This function will only action when it is called the first time).
 *          2) This should be called before calling at_api_init().
 */
bool at_api_register_adaptor(const at_adaptor_api_t *api)
{
    bool ret = false;
    
    if (api->link_num < AT_API_SOCKET_NUM) {
        DBG_LOG("too few links!");
        return false;
    }
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (NULL == at_adaptor_api) {
        at_adaptor_api = api;
        ret = true;
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);

    return ret;
}

bool at_api_deregister_adaptor(void)
{
    at_api_deinit_adaptor();
    at_adaptor_api = NULL;

    return true;
}

/*
 *  \brief  Open and init AT device.
 *  \return AT_OK       device is ok to used.
 *  \return AT_FAILED
 */
bool at_api_init_adaptor(void)
{
    bool ret = false;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->init) {
        if (AT_OK == at_adaptor_api->init()) {
            at_device_status = AT_DEVICE_OK;
            ret = true;
        }
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return ret;
}

/*
 *  \brief  close and deinit AT device.
 *  \return AT_OK       device is closed.
 *  \return AT_FAILED
 */
bool at_api_deinit_adaptor(void)
{
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->deinit) {
        at_adaptor_api->deinit();
        at_device_status = AT_DEVICE_UNINIT;
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return true;
}

int at_api_get_imsi(char *buf, uint32_t len)
{
    int rc = 0;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->get_imsi) {
        rc = at_adaptor_api->get_imsi(buf, len);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return rc;
}

int at_api_get_imei(char *buf, uint32_t len)
{
    int rc = 0;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->get_imei) {
        rc = at_adaptor_api->get_imei(buf, len);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return rc;
}

socket_t *socket_api_create(void)
{
    uint32_t i = 0;

    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    for (i = 0; i < AT_API_SOCKET_NUM; i++) {
        if (SOCKET_STATUS_UNUSE == link_array[i].status) {
            break;
        }
    }
    if (i >= AT_API_SOCKET_NUM) {
        OS_MUTEX_RELEASE(at_adaptor_api_mutex);
        return NULL;
    }

    link_array[i].status = SOCKET_STATUS_INUSE;
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);

    if (!fifo_init(&link_array[i].RecvFIFO, link_array[i].RecvBuffer, AT_API_RECV_FIFO_SIZE)) {
        DBG_LOG("init FIFO failed!");
        goto __INIT_FIFO_FAIL;
    }
    
    return &link_array[i];
    
__INIT_FIFO_FAIL:
    link_array[i].status = SOCKET_STATUS_UNUSE;
    
    return NULL;
}

/* the closing may not be complete if the socket is in use. the socket will be
   freed automaticly when it is out of use. */
int socket_api_delete(socket_t *pSocket)
{
    socket_api_shutdown(pSocket);
    
    pSocket->status = SOCKET_STATUS_UNUSE;
    
    return SOCKET_ERR_NONE;
}

int socket_api_shutdown(socket_t *pSocket)
{
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->close && (at_device_status == AT_DEVICE_OK)) {
        if (AT_OK != at_adaptor_api->close(pSocket->so)) {
            //! nothing to do.
        }
    }
    pSocket->type = SOCKET_TYPE_NONE;
    pSocket->status = SOCKET_STATUS_SHUTDOWN;
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return SOCKET_ERR_NONE;
}

/*
 *  \brief  create and bind a socket.
 *  \return <0       cannot bind a socket.
 *  \return >=0      The socket ID to be used.
 *  \note   This function is only used to create a UDP socket.
 */
int socket_api_bind(socket_t *pSocket, const char *host, uint32_t port)
{
    int32_t ret = AT_FAILED;
    int rc = SOCKET_ERR_NONE;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->bind && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->bind(&pSocket->so, host, port);
    }
    if (AT_OK != ret) {
        //DBG_LOG("bind failed!");
        rc = SOCKET_ERR_FAIL;
    } else {
        pSocket->type = SOCKET_TYPE_UDP;
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return rc;
}

/*
 *  \brief  create a socket AND connect to specified destination port.
 *  \return >=0      The socket ID to be used.
 */
int socket_api_connect(socket_t *pSocket, const char *host, uint32_t port)
{
    int32_t ret = AT_FAILED;
    int rc = SOCKET_ERR_NONE;

    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->connect && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->connect(&pSocket->so, host, port);
        if (AT_OK != ret) {
        }
    }
    if (AT_OK != ret) {
        at_adaptor_api->close(pSocket->so);
        //DBG_LOG("connect failed!");
        rc = SOCKET_ERR_FAIL;
    } else {
        pSocket->type = SOCKET_TYPE_TCP;
        pSocket->status = SOCKET_STATUS_INUSE;
        fifo_flush(&pSocket->RecvFIFO);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    return rc;
}

/*
 *  \brief  send data on a TCP socket.
 *  \return AT_OK
 *  \return AT_FAILED
 */
int socket_api_send(socket_t *pSocket, const char *buf, uint32_t *len)
{
    int32_t ret = AT_FAILED;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->send && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->send(pSocket->so, buf, *len);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (AT_OK != ret) {
        return SOCKET_ERR_FAIL;
    }
    
    return SOCKET_ERR_NONE;
}

/*
 *  \brief  send data to dest UDP port.
 *  \return AT_OK
 *  \return AT_FAILED
 */
int socket_api_sendto(socket_t *pSocket, const char *buf, uint32_t *len, const char *ipaddr, int port)
{
    int32_t ret = AT_FAILED;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->sendto && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->sendto(pSocket->so, buf, *len, ipaddr, port);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (AT_OK != ret) {
        return SOCKET_ERR_FAIL;
    }
    
    return SOCKET_ERR_NONE;
}

static int __socket_api_recv(socket_t *pSocket, char *buf, uint32_t buflen, uint32_t timeout)
{
    uint32_t rl = 0;
    uint32_t cnt = 0;
    OS_ERR ret;
    deadline_t timer;
    
    deadline_init(&timer);
    deadline_set_ms(&timer, timeout);

    while (cnt < buflen) {
        OS_MUTEX_WAIT(pSocket->RecvMutex, OS_INFINITE);
        rl = fifo_burst_out(&pSocket->RecvFIFO, buf + cnt, buflen - cnt);
        OS_MUTEX_RELEASE(pSocket->RecvMutex);
        if (rl != 0u) {
            cnt += rl;
            continue;
        }
        ret = OS_FLAG_WAIT(pSocket->RecvFlag, deadline_left_ms(&timer));
        if (OS_ERR_TIMEOUT == ret) {
            break;
        } else if (OS_ERR_NONE != ret) {
            DBG_LOG("recv flag error: %u", ret);
            break;
        }
    }
    
    return cnt;
}

int socket_api_recv(socket_t *pSocket, char *buf, uint32_t *len, uint32_t timeout)
{
    uint32_t rl = 0;
    uint32_t buflen = *len;

    if (NULL == pSocket) {
        return SOCKET_ERR_INVALID_SOCKET;
    }
    
    if (NULL == buf) {
        return SOCKET_ERR_NULL_BUFFER;
    }

    if (buflen == 0u) {
        return SOCKET_ERR_NONE;
    }
    
    if (buflen > AT_API_RECV_FIFO_SIZE) {
        return SOCKET_ERR_BUFFER_SIZE;
    }
    
    *len = 0;
    timeout = timeout > 20000 ? 20000 : timeout;        //!< max 20s to wait.
    
    rl = __socket_api_recv(pSocket, buf, buflen, timeout);
    *len = rl;
    
    if (0u == rl) {
        return SOCKET_ERR_TIMEOUT;
    }
    
    return SOCKET_ERR_NONE;    
}

/*  \note   if the len is too small, the data received will be lost and SOCKET_ERR_BUFFER_SIZE will be return.
 */
int socket_api_recvfrom(socket_t *pSocket, char *buf, uint32_t *len, char *ipaddr, int *port, uint32_t timeout)
{
    uint32_t buflen = *len;
    int rc = SOCKET_ERR_NONE;
    OS_ERR ret;
    
    if (NULL == pSocket) {
        return SOCKET_ERR_INVALID_SOCKET;
    }
    
    if (NULL == buf) {
        return SOCKET_ERR_NULL_BUFFER;
    }

    if (buflen == 0u) {
        return SOCKET_ERR_NONE;
    }

    (void)ipaddr; //gprs not need remote ip,
    (void)port;   //gprs not need remote port,
    
    *len = 0;
    timeout = timeout > 20000 ? 20000 : timeout;        //!< max 20s to wait.

    ret = OS_FLAG_WAIT(pSocket->RecvFlag, timeout);
    if (OS_ERR_TIMEOUT == ret) {
        rc = SOCKET_ERR_TIMEOUT;
        goto __ERROR_EXIT;
    } else if (OS_ERR_NONE != ret) {
        DBG_LOG("recv flag error: %u", ret);
        rc = SOCKET_ERR_FAIL;
        goto __ERROR_EXIT;
    }

    OS_MUTEX_WAIT(pSocket->RecvMutex, OS_INFINITE);
    do {
        if (pSocket->RecvLength > buflen) {
            rc = SOCKET_ERR_BUFFER_SIZE;
            break;
        } else {
            memcpy(buf, pSocket->RecvBuffer, pSocket->RecvLength);
            *len = pSocket->RecvLength;
        }
    } while (0);
    OS_MUTEX_RELEASE(pSocket->RecvMutex);
    
__ERROR_EXIT:
    return rc;
}




void at_api_handle_data(uint32_t so, char *buf, uint32_t len)
{
    uint32_t id;
    
    if (0 == len || (NULL == buf)) {
        return;
    }

    
    if (len > AT_API_MAX_FRAME_SIZE) {
        return;
    }
    
    for (id = 0; id < AT_API_SOCKET_NUM; id++) {
        if (link_array[id].so == so) {
            break;
        }
    }
    if (id >= AT_API_SOCKET_NUM) {
        return;
    }
    
    if (SOCKET_STATUS_INUSE != link_array[id].status) {
        return;
    }
    
    OS_MUTEX_WAIT(link_array[id].RecvMutex, OS_INFINITE);
    if (link_array[id].status == SOCKET_STATUS_INUSE) {
        if (link_array[id].type == SOCKET_TYPE_TCP) {
            if (0 == fifo_burst_in(&link_array[id].RecvFIFO, buf, len)) {
                DBG_LOG("write fifo fail!");
            }
        } else if (link_array[id].type == SOCKET_TYPE_UDP) {
            memcpy(link_array[id].RecvBuffer, buf, len);
            link_array[id].RecvLength = len;
        }
        OS_FLAG_SET(link_array[id].RecvFlag);
    }
    OS_MUTEX_RELEASE(link_array[id].RecvMutex);
}
