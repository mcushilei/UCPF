
#ifndef __AT_API_H__
#define __AT_API_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"

/*============================ MACROS ========================================*/

enum {
    SOCKET_ERR_NONE         = 0,
    SOCKET_ERR_TIMEOUT,
    SOCKET_ERR_FAIL,
    SOCKET_ERR_BUFFER_SIZE,
    SOCKET_ERR_NULL_BUFFER,
    SOCKET_ERR_INVALID_SOCKET,
};

#define AT_API_RECV_FIFO_SIZE   (2u * 1024u)

#define SOCKET_PROTO_UDP     0
#define SOCKET_PROTO_TCP     1


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct {
	uint8_t     status;
    uint8_t     type;
    int32_t     so;
    
    OS_HANDLE   RecvMutex;
	OS_HANDLE   RecvFlag;
    char        RecvBuffer[AT_API_RECV_FIFO_SIZE];
    
    fifo_t      RecvFIFO;       //! this fifo is only used by TCP socket.
    uint32_t    RecvLength;     //! the packet size of received UDP.
} at_socket_t;

typedef struct at_option_t {
	const char     *apn;
	const char     *apn_user;
	const char     *apn_pswd;
} at_option_t;

typedef struct {
	int32_t  (*init)    (void);
	int32_t  (*deinit)  (void);
    
    int32_t  (*get_imsi)(char *buf, uint32_t len);
    int32_t  (*get_imei)(char *buf, uint32_t len);

    int32_t  (*bind)    (int32_t *id, const char *host, uint32_t port);
	int32_t  (*connect) (int32_t *id, const char *host, uint32_t port);

	int32_t  (*send)    (int32_t id, const char *buf, uint32_t len);
    int32_t  (*sendto)  (int32_t id, const char *buf, uint32_t len, const char *ipaddr, int port);

	int32_t  (*close)   (int32_t id);
    
    uint32_t link_num;
} at_adaptor_api_t;


/*============================ PUBLIC VARIABLES ==============================*/
/*============================ PUBLIC PROTOTYPES =============================*/
extern bool at_api_set_apn(char *name, char *usr, char *psw);

extern bool at_api_init(at_option_t *opt);
extern bool at_api_deinit(void);

extern bool at_api_register_adaptor(const at_adaptor_api_t *api);
extern bool at_api_deregister_adaptor(void);
extern bool at_api_init_adaptor(void);
extern bool at_api_deinit_adaptor(void);
extern int  at_api_get_imsi(char *buf, uint32_t len);
extern int  at_api_get_imei(char *buf, uint32_t len);

extern int  socket_api_create(void);
extern int  socket_api_delete(int so);
extern int  socket_api_connect(int so, const char* host, uint32_t port);
extern int  socket_api_bind(int so, const char *host, uint32_t port);
extern int  socket_api_send(int so, const char *buf, uint32_t *len);
extern int  socket_api_sendto(int so, const char *buf, uint32_t *len, const char *ipaddr, int port);
extern int  socket_api_recv(int so, char *buf, uint32_t *len, uint32_t timeout);
extern int  socket_api_recvfrom(int so, char *buf, uint32_t *len, char *senderIP, int *senderPort, uint32_t timeout);
extern int  socket_api_shutdown(int so);

extern void at_api_handle_data(uint32_t id, char *buf, uint32_t len);

#endif
