

#include "./app_cfg.h"
#include "./at_api.h"
#include "./at_helper.h"

#define SOCKET_STATUS_UNUSE		    0
#define SOCKET_STATUS_INUSE 		1
#define SOCKET_STATUS_CLOSE 		2

#define AT_DEVICE_UNINIT    0
#define AT_DEVICE_OK 		1

#define AT_API_MAX_LINK_USED    4
#define AT_API_MAX_IP_LEN       40



typedef struct {
    uint32_t    len;                //!< length of buffer's memory.
    char        *addr;              //!< buffer's memory.
    char        ipaddr[AT_API_MAX_IP_LEN];
    int         port;
} at_data_buf_t;

static const at_adaptor_api_t *at_adaptor_api = NULL;
static OS_HANDLE         at_adaptor_api_mutex = NULL;
static uint32_t          at_device_status = AT_DEVICE_UNINIT;
static socket_t          link_array[AT_API_MAX_LINK_USED] = {0};

at_apn_info   apn_info;

bool at_api_set_apn(char *name, char *usr, char *psw)
{
    if ( (strlen(name) >= sizeof(apn_info.name)) || (strlen(usr) >= sizeof(apn_info.usr)) || (strlen(psw) >= sizeof(apn_info.psw)) ) {
        return false;
    }
    if (0 == strlen(name)) {
        apn_info.name[0] = '\0';
    } else {
        string_copy(apn_info.name, name, sizeof(apn_info.name));
    }
    if (0 == strlen(usr)) {
        apn_info.usr[0] = '\0';
    } else {
        string_copy(apn_info.usr, usr, sizeof(apn_info.usr));
    }
    if (0 == strlen(psw)) {
        apn_info.psw[0] = '\0';
    } else {
        string_copy(apn_info.psw, psw, sizeof(apn_info.psw));
    }
    return true;
}


bool at_api_init(void)
{
    if (at_adaptor_api_mutex != NULL) {
        return true;
    }
    
    at_adaptor_api   = NULL;
    at_device_status = AT_DEVICE_UNINIT;
    
    for (uint32_t i = 0; i < AT_API_MAX_LINK_USED; i++) {
        link_array[i].status = SOCKET_STATUS_UNUSE;
        link_array[i].qid    = NULL;
        link_array[i].index  = i;
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
    
    if (api->link_num < AT_API_MAX_LINK_USED) {
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

socket_t *socket_api_create(void)
{
    uint32_t i = 0;

    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    for (i = 0; i < AT_API_MAX_LINK_USED; i++) {
        if (SOCKET_STATUS_UNUSE == link_array[i].status) {
            break;
        }
    }
    if (i >= AT_API_MAX_LINK_USED) {
        OS_MUTEX_RELEASE(at_adaptor_api_mutex);
        return NULL;
    }

    link_array[i].status = SOCKET_STATUS_INUSE;
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (OS_QUEUE_CREATE(link_array[i].qid, 16, sizeof(at_data_buf_t)) != OS_ERR_NONE) {
        DBG_LOG("init qid failed!");
        link_array[i].status = SOCKET_STATUS_UNUSE;
        return NULL;
    }
    
    return &link_array[i];
}

/*
 *  \brief  create and bind a socket.
 *  \return <0       cannot bind a socket.
 *  \return >=0      The socket ID to be used.
 *  \note   This function is only used to create a UDP socket.
 */
int socket_api_bind(socket_t *pSocket, const char *host, const char *port)
{
    int32_t ret = AT_FAILED;
    
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->bind && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->bind(pSocket->index, host, port, SOCKET_PROTO_UDP);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (AT_OK != ret) {
        DBG_LOG("bind failed!");
        return SOCKET_ERR_FAIL;
    }
    
    return SOCKET_ERR_NONE;
}

/*
 *  \brief  create a socket AND connect to specified destination port.
 *  \return >=0      The socket ID to be used.
 */
int socket_api_connect(socket_t *pSocket, const char *host, const char *port)
{
    int32_t ret = AT_FAILED;

    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->connect && (at_device_status == AT_DEVICE_OK)) {
        ret = at_adaptor_api->connect(pSocket->index, host, port, SOCKET_PROTO_TCP);
        if (AT_OK != ret) {
            at_adaptor_api->close(pSocket->index);
        }
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (AT_OK != ret) {
        DBG_LOG("connect failed!");
        return SOCKET_ERR_FAIL;
    }
    
    return SOCKET_ERR_NONE;
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
        ret = at_adaptor_api->send(pSocket->index, buf, *len);
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
        ret = at_adaptor_api->sendto(pSocket->index, buf, *len, ipaddr, port);
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    if (AT_OK != ret) {
        return SOCKET_ERR_FAIL;
    }
    
    return SOCKET_ERR_NONE;
}

static int __socket_api_recv(socket_t *pSocket, char *buf, uint32_t *len, char *ipaddr, int *port, uint32_t timeout)
{
    uint32_t rxlen = 0;
    uint32_t buflen = *len;
    OS_ERR ret;

    (void)ipaddr; //gprs not need remote ip,
    (void)port;   //gprs not need remote port,
    *len = 0;
    
    at_data_buf_t  qbuf = {0};
    timeout = timeout > 20000 ? 20000 : timeout;        //!< max 20s to wait.
    ret = OS_QUEUE_READ(pSocket->qid, &qbuf, timeout);
    if (OS_ERR_TIMEOUT == ret) {
        return SOCKET_ERR_TIMEOUT;
    } else if (OS_ERR_NONE != ret) {
        DBG_LOG("recv queue error: %u", ret);
        return SOCKET_ERR_FAIL;
    }

    if (qbuf.len && (qbuf.addr != NULL)) {
        rxlen = (buflen < qbuf.len) ? buflen : qbuf.len;
        memcpy(buf, qbuf.addr, rxlen);
        *len = rxlen;
    }
    osHeapFree(qbuf.addr);
    
    return SOCKET_ERR_NONE;    
}

int socket_api_recv(socket_t *pSocket, char *buf, uint32_t *len, uint32_t timeout)
{
    return __socket_api_recv(pSocket, buf, len, NULL, NULL, timeout);
}

int socket_api_recvfrom(socket_t *pSocket, char *buf, uint32_t *len, char *ipaddr, int *port, uint32_t timeout)
{
    return __socket_api_recv(pSocket, buf, len, ipaddr, port, timeout);
}

/* the closing may not be complete if the socket is in use. the socket will be
   freed automaticly when it is out of use. */
int socket_api_delete(socket_t *pSocket)
{
    OS_MUTEX_WAIT(at_adaptor_api_mutex, OS_INFINITE);
    if (at_adaptor_api && at_adaptor_api->close && (at_device_status == AT_DEVICE_OK)) {
        if (AT_OK != at_adaptor_api->close(pSocket->index)) {
            //! nothing to do.
        }
    }
    
    if (pSocket->status == SOCKET_STATUS_INUSE) {
        pSocket->status = SOCKET_STATUS_CLOSE;
    }
    OS_MUTEX_RELEASE(at_adaptor_api_mutex);
    
    osQueueStopEnqueue(pSocket->qid);
        
    //! free all buffers left in the queue.
    at_data_buf_t	qbuf = {0};
    while (OS_QUEUE_READ(pSocket->qid, &qbuf, 0) == OS_ERR_NONE) {
        if (qbuf.addr != NULL) {
            osHeapFree(qbuf.addr);
        }
    }
    
    if (OS_ERR_NONE == OS_QUEUE_DELETE(pSocket->qid)) {
        pSocket->status   = SOCKET_STATUS_UNUSE;
        pSocket->qid      = NULL;
        DBG_LOG("queue delete.");
    } else {
        DBG_LOG("queue delete fail!");
    }
    
    return SOCKET_ERR_NONE;
}




void at_api_handle_data(uint32_t id, char *buf, uint32_t len)
{
    at_data_buf_t qbuf = {0};
    
    if (0 == len || (NULL == buf)) {
        return;
    }
    
    if (AT_API_MAX_LINK_USED <= id) {
        return;
    }
    
    DBG_LOG("from id %u", id);
    
    qbuf.addr = osHeapMalloc(len);
    if (NULL == qbuf.addr) {
        DBG_LOG("out of memory!");
        return;
    }

    qbuf.len = len;
    memcpy(qbuf.addr, buf, len);
    
    if (link_array[id].status == SOCKET_STATUS_INUSE) {
        if (OS_ERR_NONE != OS_QUEUE_WRITE(link_array[id].qid, &qbuf, 0)) {
            DBG_LOG("write queue fail!");
            osHeapFree(qbuf.addr);
        }
    } else {
        DBG_LOG("socket is not open!");
    }
}
