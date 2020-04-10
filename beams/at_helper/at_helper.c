

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "./at_plug.h"
#include "./at_helper.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
//#define AT_INTO
#ifdef AT_INTO
#define AT_LOG      DBG_LOG
#else
#define AT_LOG(...)
#endif

/*============================ TYPES =========================================*/
/*============================ PRIVATE PROTOTYPES ============================*/
/*============================ PRIVATE VARIABLES =============================*/
at_t at;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void at_write_task_msg(uint32_t type)
{
    at_msg_t msg = {0};

    msg.msg_type = type;
    OS_QUEUE_WRITE(at.RecvQueue, &msg, 0);
}

static void at_add_listener(at_listener_t *p)
{
    at_listener_t **cur = &at.ListenerList;

    while (*cur != NULL) {
        cur = &((*cur)->next);
    }
    *cur = p;
}

static void at_remove_listener(at_listener_t *p)
{
    at_listener_t **cur = &at.ListenerList;

    for (; *cur != NULL; cur = &(*cur)->next) {
        if (*cur == p) {
            *cur = p->next;
            break;
        }
    }
}

static void at_destroy_listener(void)
{
    at_listener_t *cur = at.ListenerList;
    at_listener_t *tmp;

    while (cur != NULL) {
        if (cur->listener_callback != NULL) {
            tmp = cur;
            cur = cur->next;
            at_free(tmp);
        } else {
            cur = cur->next;
        }
    }
}



int32_t at_cmd_multi_suffix(const char *cmd, uint32_t cmd_len, at_cmd_info_t *cmd_info, uint32_t timeout)
{
    OS_ERR ret;
    at_listener_t listener = {0};
    uint32_t print_len;

    if ((cmd_info == NULL) || (cmd == NULL)) {
        return AT_FAILED;
    }

    listener.cmd_info = *cmd_info;
    print_len = ((cmd_info->resp_buf != NULL && cmd_info->resp_len != NULL) ? *(cmd_info->resp_len) : -1);
    
    if (timeout < at.timeout) {
        timeout = at.timeout;
    }
    
    AT_LOG("cmd:%s resp_len: %d resp_buf: 0x%08p timeout: %u", cmd, print_len, cmd_info->resp_buf, timeout);

    OS_MUTEX_WAIT(at.trx_mux, OS_INFINITE);
    {
        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        at_add_listener(&listener);
        at_transmit(cmd, cmd_len, true);
        OS_MUTEX_RELEASE(at.cmd_mux);

        ret = OS_SEMAPHORE_WAIT(at.resp_sem, timeout);

        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        at_remove_listener(&listener);     //<! in case we reached a timeout.
        OS_MUTEX_RELEASE(at.cmd_mux);
    }
    OS_MUTEX_RELEASE(at.trx_mux);

    if (ret != OS_ERR_NONE) {
        AT_LOG("at cmd resp timeout!(cmd = %s)!", cmd);
        return AT_FAILED;
    }

    *cmd_info = listener.cmd_info;
    return AT_OK;
}

/*
 *  \brief  to send TCP or UDP data.
 *  \return AT_OK       if write successfully
 */
int32_t at_write(const char *cmd, const char *suffix, const char *buf, uint32_t len)
{
    at_listener_t   listener = {0};
    const char     *suffix_array[1];
    OS_ERR          ret;

    listener.cmd_info.match_num     = UBOUND(suffix_array);
    listener.cmd_info.suffix_array  = suffix_array;

    OS_MUTEX_WAIT(at.trx_mux, OS_INFINITE);
    do {
        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        suffix_array[0] = ">";
        at_add_listener(&listener);
        at_transmit(cmd, strlen(cmd), true);
        OS_MUTEX_RELEASE(at.cmd_mux);

        ret = OS_SEMAPHORE_WAIT(at.resp_sem, at.timeout);
        
        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        at_remove_listener(&listener);
        OS_MUTEX_RELEASE(at.cmd_mux);
        
        if (ret != OS_ERR_NONE) {
            break;
        }

        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        suffix_array[0] = suffix;
        at_add_listener(&listener);
        at_transmit(buf, len, false);
        OS_MUTEX_RELEASE(at.cmd_mux);

        ret = OS_SEMAPHORE_WAIT(at.resp_sem, at.timeout);

        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        at_remove_listener(&listener);
        OS_MUTEX_RELEASE(at.cmd_mux);
    } while (0);
    OS_MUTEX_RELEASE(at.trx_mux);

    if (ret != OS_ERR_NONE) {
        AT_LOG("OS_SEMAPHORE_WAIT for listener.resp_sem failed(ret = %x)!", ret);
        return AT_FAILED;
    }
    return AT_OK;
}

/*
 *  \brief  send a AT command and receive it's response
 *  \param  cmd         the comannd string.
 *  \param  cmd_len     the length of the command string.
 *  \param  suffix      the desired response string.
 *  \param  resp_buf    the buffer to store the response string.
 *  \param  resp_len    the varable to store the length of the response string.
 *  \return AT_OK
 *  \return AT_FAILED
 *  \note   resp_buf and resp_len is optional and they should be present at the same time.
 *          The match condition is when the respon string HAS string "suffix".
 */
int32_t at_cmd(const char *cmd, uint32_t cmd_len, const char *resp_match, char *resp_buf, uint32_t *resp_len)
{
    const char     *suffix_array[1] = {resp_match};
    at_cmd_info_t   cmd_info = {0};

    cmd_info.suffix_array   = suffix_array;
    cmd_info.match_num      = UBOUND(suffix_array);
    cmd_info.resp_buf       = resp_buf;
    cmd_info.resp_len       = resp_len;

    return at_cmd_multi_suffix(cmd, cmd_len, &cmd_info, 0);
}

int32_t at_cmd_with_2_suffix(   const char      *cmd,
                                uint32_t         cmd_len,
                                const char      *resp_match_1,
                                const char      *resp_match_2,
                                char            *resp_buf,
                                uint32_t        *resp_len,
                                uint32_t         timeout)
{
    const char     *suffix_array[2] = {0};
    at_cmd_info_t   cmd_info = {0};

    suffix_array[0] = resp_match_1;
    suffix_array[1] = resp_match_2;

    cmd_info.suffix_array   = suffix_array;
    cmd_info.match_num      = UBOUND(suffix_array);
    cmd_info.resp_buf       = resp_buf;
    cmd_info.resp_len       = resp_len;

	if (at_cmd_multi_suffix(cmd, cmd_len, &cmd_info, timeout) != AT_OK) {
        return AT_FAILED;
    }

    return cmd_info.match_idx;
}

static int handle_listener_callback(at_listener_t *listener, char *resp_buf, uint32_t resp_len)
{
    if (listener->listener_callback == NULL) {
        return AT_FAILED;
    }

    if (listener->listener_callback(resp_buf, resp_len) == AT_OK) {
        at_remove_listener(listener);
        return AT_OK;
    }

    return AT_FAILED;
}

static void store_resp_buf(char *resp_buf, uint32_t *resp_len, const char *src, uint32_t src_len)
{
    uint32_t copy_len;

    copy_len = MIN(*resp_len, src_len);
    memcpy(resp_buf, src, copy_len);
    *resp_len = copy_len;
}

static void resp_cmd_matching(char *resp_buf, uint32_t resp_len)
{
    at_listener_t *listener = at.ListenerList;

    if (NULL == listener) {
        return;
    }

    //! if the listener's callback funtion has handle it, then we skip the folowing.
    if (handle_listener_callback(listener, resp_buf, resp_len) == AT_OK) {
        return;
    }

    listener->cmd_info.match_idx = -1;
    
    if (listener->cmd_info.suffix_array == NULL) {
        at_remove_listener(listener);
        OS_SEMAPHORE_RELEASE(at.resp_sem);
        return;
    }
    
    //! see if the received string matches the listener's desire.
    for (uint32_t i = 0; i < listener->cmd_info.match_num; i++) {
        char *suffix;

        if (listener->cmd_info.suffix_array[i] == NULL) {
            continue;
        }

        suffix = strstr(resp_buf, listener->cmd_info.suffix_array[i]);
        if (suffix != NULL) {
            if ((NULL != listener->cmd_info.resp_buf) && (NULL != listener->cmd_info.resp_len)) {
                //! to store resp string into listener's buffer.
                store_resp_buf(listener->cmd_info.resp_buf, listener->cmd_info.resp_len, resp_buf, resp_len);
            }
            listener->cmd_info.match_idx = i;
            at_remove_listener(listener);
            OS_SEMAPHORE_RELEASE(at.resp_sem);
            break;
        }
    }
}

int32_t at_oob_register(const char *suffix, oob_callback_t *callback, oob_cmp_callback_t *oob_cmd_cmp)
{
    at_oob_t *oob;
    uint32_t len = strlen(suffix);
    
    if (suffix == NULL || oob_cmd_cmp == NULL || at.oob_num == AT_OOB_MAX_NUM || len >= (AT_OOB_SUFFIX_LEN - 1u)) {
        return AT_FAILED;
    }

    oob = &(at.oob[at.oob_num++]);
    strncpy(oob->suffix, suffix, AT_OOB_SUFFIX_LEN);
    oob->len            = len;
    oob->oob_callback   = callback;
    oob->oob_cmd_cmp    = oob_cmd_cmp;
    
    return AT_OK;
}

static uint32_t oob_cmd_matching(const char *buf, uint32_t len)
{
    int32_t ret = 0;
    uint32_t i;

    for (i = 0; i < at.oob_num; i++) {
        ret = at.oob[i].oob_cmd_cmp(buf, at.oob[i].suffix, at.oob[i].len);
        if (ret == 0) {
            if (at.oob[i].oob_callback != NULL) {
                at.oob[i].oob_callback(at.oob[i].oob_callback_arg, buf, len);
            }
            return len;
        }
    }
    
    return 0;
}

static OS_TASK_ENTRY(at_task)
{
    uint32_t    recv_len = 0;
    char       *tmp = at.data_buf;
    int         ret = 0;
    at_msg_t    msg;
    
    while(1) {
        if (OS_QUEUE_READ(at.RecvQueue, &msg, OS_INFINITE) != OS_ERR_NONE) {
            continue;
        }

        if (msg.msg_type == AT_TASK_QUIT) {
            AT_LOG("at recv task quit.");
            break;
        }

        if (msg.msg_type != AT_USART_RX) {
            continue;
        }

        memset(tmp, 0, at.Config.user_buf_len);
        recv_len = at_read_resp(tmp, msg.ori, msg.end);
        if (recv_len <= 0) {
            continue;
        }
        
        AT_LOG("recv %u:%s", recv_len, tmp);
        
        ret = oob_cmd_matching(tmp, recv_len);
        if (ret > 0) {
            continue;
        }

        OS_MUTEX_WAIT(at.cmd_mux, OS_INFINITE);
        resp_cmd_matching(tmp, recv_len);
        OS_MUTEX_RELEASE(at.cmd_mux);
    }
    
    at.TaskIsRunning = false;
    return 0;
}

static uint32_t create_at_task(void)
{
    return OS_TASK_CREATE(at.Task, "at_task", at_task, NULL, 0x200, 2, OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}

static int32_t at_struct_init(at_t *at)
{
    OS_ERR err;
    
    if (NULL == at) {
        AT_LOG("invaild param!");
        return AT_FAILED;
    }

    err = OS_QUEUE_CREATE(at->RecvQueue, 32, sizeof(at_msg_t));
    if (err != OS_ERR_NONE) {
        AT_LOG("init recvQueue failed!");
        return AT_FAILED;
    }

    err = OS_MUTEX_CREATE(at->cmd_mux);
    if (err != OS_ERR_NONE) {
        AT_LOG("init cmd_mux failed!");
        goto at_cmd_mux_failed;
    }

    err = OS_MUTEX_CREATE(at->trx_mux);
    if (err != OS_ERR_NONE) {
        AT_LOG("init trx_mux failed!");
        goto at_cmd_mux_failed;
    }

    err = OS_SEMAPHORE_CREATE(at->resp_sem, 0);
    if (err != OS_ERR_NONE) {
        AT_LOG("init resp_sem failed!");
        goto at_resp_sem_failed;
    }

    at->data_buf = at_malloc(at->Config.user_buf_len);
    if (NULL == at->data_buf) {
        AT_LOG("malloc data_buf failed!");
        goto malloc_recv_buf;
    }
    
    at->oob_num = 0;
    memset(at->oob, 0, AT_OOB_MAX_NUM * sizeof(at_oob_t));
    
    at->Task = NULL;
    at->ListenerList = NULL;
    at->timeout = at->Config.timeout;
    
    return AT_OK;

    at_free(at->data_buf);
malloc_recv_buf:
    (void)OS_SEMAPHORE_DELETE(at->resp_sem);
at_resp_sem_failed:
    (void)OS_MUTEX_DELETE(at->cmd_mux);
at_cmd_mux_failed:
    (void)OS_MUTEX_DELETE(at->trx_mux);
    (void)OS_QUEUE_DELETE(at->RecvQueue);

    return AT_FAILED;
}

static int32_t at_struct_deinit(at_t *at)
{
    if (at == NULL) {
        AT_LOG("invaild param!");
        return AT_FAILED;
    }

    OS_MUTEX_WAIT(at->cmd_mux, OS_INFINITE);
    at_destroy_listener();
    OS_MUTEX_RELEASE(at->cmd_mux);

    if (OS_MUTEX_DELETE(at->cmd_mux) != OS_ERR_NONE) {
        AT_LOG("delete at.cmd_mux failed!");
    }

    if (OS_SEMAPHORE_DELETE(at->resp_sem) != OS_ERR_NONE) {
        AT_LOG("delete at.resp_sem failed!");
    }

    if (OS_MUTEX_DELETE(at->trx_mux) != OS_ERR_NONE) {
        AT_LOG("delete at.trx_mux failed!");
    }

    if (OS_QUEUE_DELETE(at->RecvQueue) != OS_ERR_NONE) {
        AT_LOG("delete at.RecvQueue failed!");
    }

    if (NULL != at->data_buf) {
        at_free(at->data_buf);
        at->data_buf = NULL;
    }
    
    memset(at, 0, sizeof(at_t));

    return AT_OK;
}

void at_init(const at_config_t *config)
{

    if (NULL == config) {
        AT_LOG("Config is NULL, failed!!");
        return;
    }
    
    memcpy(&at.Config, config, sizeof(at_config_t));
    
    AT_LOG("Config %s(buffer total is %lu)......", at.Config.name, at.Config.user_buf_len);

    if (AT_OK != at_struct_init(&at)) {
        AT_LOG("at_struct_init failed!");
        return;
    }

    if (!at_usart_init()) {
        AT_LOG("at_usart_init failed!");
        at_struct_deinit(&at);
        return;
    }
    
    if (OS_ERR_NONE != create_at_task()) {
        AT_LOG("create_at_task failed!");
        at_usart_deinit();
        at_struct_deinit(&at);
        return;
    }
    at.TaskIsRunning = true;


    AT_LOG("Config complete.");
}

void at_deinit(void)
{
    int cnt = 0;
    const int max_try_num = 10;


    //! send a msg to the recv task to quit.
    while (at.TaskIsRunning && cnt < max_try_num) {
        at_write_task_msg(AT_TASK_QUIT);
        OS_TASK_SLEEP(1000);
        cnt++;
    }

    if (at.TaskIsRunning) {
        AT_LOG("at_task delete failed!");
    }

    at_usart_deinit();
    
    if (AT_OK != at_struct_deinit(&at)) {
        AT_LOG("at_struct_deinit failed!");
    }
}

at_config_t *at_get_config(void)
{
    return &at.Config;
}

