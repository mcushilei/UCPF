
#ifndef __AT_HELPER_H__
#define __AT_HELPER_H__

#include "./app_cfg.h"
#include "./at_plug.h"
#include "./at_api.h"


#define AT_OK    		 0
#define AT_FAILED 		-1
#define AT_TIMEOUT      -2

#define AT_OOB_MAX_NUM     (5u)
#define AT_OOB_SUFFIX_LEN     (40u)

typedef int32_t oob_callback_t          (void *arg, const char *buf, uint32_t len);
typedef int32_t oob_cmp_callback_t      (const char *buf, const char *suffix, uint32_t len);
typedef int32_t listener_callback_t     (const char *data, uint32_t len);

enum {
    AT_USART_RX,
    AT_TASK_QUIT,
};

typedef struct {
    uint32_t    ori;
    uint32_t    end;
    uint32_t    msg_type;
} at_msg_t;

typedef struct {
    const char    **suffix_array;
    char           *resp_buf;
    uint32_t       *resp_len;
    int             match_num;
    int             match_idx;
} at_cmd_info_t;

typedef struct at_listener_t at_listener_t;
struct at_listener_t {
	at_listener_t          *next;
    listener_callback_t    *listener_callback;      //!< only for asynchronous usage
    uint32_t                expire_time;            //!< only for asynchronous usage
    at_cmd_info_t           cmd_info;
};

/* Out Of Band data process*/
typedef struct oob_s {
	char                        suffix[AT_OOB_SUFFIX_LEN];  //!< the AT string to match.
	uint32_t                    len;                        //!< length of the AT string.
    oob_cmp_callback_t         *oob_cmd_cmp;                //!< used to compare string.
	oob_callback_t             *oob_callback;               //!< to be called when matching.
	void                       *oob_callback_arg;           //!< argument to callback function.
} at_oob_t;


typedef struct at_config_t {
	const char     *name;
	const char     *cmd_begin;
	const char     *line_end;
	uint32_t        usart_port;
	uint32_t        buardrate;
	uint32_t        linkid_num;
	uint32_t        user_buf_len;
	uint32_t        timeout;        //!< command respond timeout, in ms
} at_config_t;

typedef struct {
	OS_HANDLE       Task;
    OS_HANDLE       RecvQueue;          //!< msg queue
	OS_HANDLE       resp_sem;
	OS_HANDLE       cmd_mux;
    OS_HANDLE       trx_mux;
    
    at_config_t     Config;
	at_listener_t  *ListenerList;
	char           *data_buf;           /*buffer for inner use.*/
	uint32_t        timeout;            //!< command respond timeout, in ms
    bool            TaskIsRunning;
    
	at_oob_t        oob[AT_OOB_MAX_NUM];
	uint32_t        oob_num;
} at_t;

extern at_t  at;

extern void         at_init             (const at_config_t *config);
extern void         at_deinit           (void);
extern at_config_t *at_get_config       (void);
extern int32_t      at_oob_register     (const char *suffix, oob_callback_t *callback, oob_cmp_callback_t *oob_cmd_cmp);
extern int32_t      at_write            (const char *cmd, const char *suffix, const char *buf, uint32_t len);
extern int32_t      at_cmd_multi_suffix (const char *cmd, uint32_t cmd_len, at_cmd_info_t *cmd_info, uint32_t timeout);
extern int32_t      at_cmd              (const char *cmd, uint32_t cmd_len, const char *suffix, char *resp_buf, uint32_t *resp_len);
extern int32_t      at_cmd_with_2_suffix(const char *cmd, uint32_t cmd_len, const char *resp_match_1,
                                                                            const char *resp_match_2,
                                                                            char       *resp_buf,
                                                                            uint32_t   *resp_len,
                                                                            uint32_t    timeout);
                                

#endif
