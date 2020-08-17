
#ifndef __MQTT_PACKET_H__
#define __MQTT_PACKET_H__


#include "./app_cfg.h"


/* Size of a data length elements in protocol */
#define MQTT_SIZE_OF_LENGTH   2


enum {
    MQTT_CODE_SUCCESS = 0,
    MQTT_CODE_ERROR_BAD_ARG = -1,
    MQTT_CODE_ERROR_OUT_OF_BUFFER = -2,
    MQTT_CODE_ERROR_MALFORMED_DATA = -3, /* Error (Malformed Remaining Len) */
    MQTT_CODE_ERROR_PACKET_TYPE = -4,
    MQTT_CODE_ERROR_PACKET_ID = -5,
    MQTT_CODE_ERROR_TLS_CONNECT = -6,
    MQTT_CODE_ERROR_TIMEOUT = -7,
    MQTT_CODE_ERROR_NETWORK = -8,
    MQTT_CODE_ERROR_MEMORY = -9,
    MQTT_CODE_ERROR_STAT = -10,
    MQTT_CODE_ERROR_PROPERTY = -11,
    MQTT_CODE_ERROR_SERVER_PROP = -12,
    MQTT_CODE_ERROR_CALLBACK = -13,

    MQTT_CODE_CONTINUE = -101,
    MQTT_CODE_STDIN_WAKE = -102,
};


typedef struct _MqttTopic           MqttTopic;
typedef struct _MqttHeader          MqttHeader;
typedef struct _MqttConnectVariableHeader MqttConnectVariableHeader;
typedef struct _MqttConnectAck      MqttConnectAck;
typedef struct _MqttConnect         MqttConnect;
typedef struct _MqttPublishResp     MqttPublishResp;
typedef struct _MqttPublish         MqttPublish;
typedef struct _MqttPublish         MqttMessage; /* message is Publish */
typedef struct _MqttSubscribeAck    MqttSubscribeAck;
typedef struct _MqttSubscribe       MqttSubscribe;
typedef struct _MqttUnsubscribeAck  MqttUnsubscribeAck;
typedef struct _MqttUnsubscribe     MqttUnsubscribe;

/* QoS */
enum {
    MQTT_QOS_0 = 0, /* At most once delivery */
    MQTT_QOS_1 = 1, /* At least once delivery */
    MQTT_QOS_2 = 2, /* Exactly once delivery */
    MQTT_QOS_3 = 3, /* Reserved - must not be used */
};

/* Topic */
struct _MqttTopic {
    const char *topic_filter;

    /* These are only on subscribe */
    uint8_t     qos;
    uint8_t     return_code;
};


/* PACKET HEADER */
enum {
    MQTT_PACKET_TYPE_RESERVED = 0,
    MQTT_PACKET_TYPE_CONNECT = 1,
    MQTT_PACKET_TYPE_CONNECT_ACK = 2,       /* Acknowledgment */
    MQTT_PACKET_TYPE_PUBLISH = 3,
    MQTT_PACKET_TYPE_PUBLISH_ACK = 4,       /* Acknowledgment */
    MQTT_PACKET_TYPE_PUBLISH_REC = 5,       /* Received */
    MQTT_PACKET_TYPE_PUBLISH_REL= 6,        /* Release */
    MQTT_PACKET_TYPE_PUBLISH_COMP = 7,      /* Complete */
    MQTT_PACKET_TYPE_SUBSCRIBE = 8,
    MQTT_PACKET_TYPE_SUBSCRIBE_ACK = 9,     /* Acknowledgment */
    MQTT_PACKET_TYPE_UNSUBSCRIBE = 10,
    MQTT_PACKET_TYPE_UNSUBSCRIBE_ACK = 11,  /* Acknowledgment */
    MQTT_PACKET_TYPE_PING_REQ = 12,         /* Request */
    MQTT_PACKET_TYPE_PING_RESP = 13,        /* Response */
    MQTT_PACKET_TYPE_DISCONNECT = 14,
    MQTT_PACKET_TYPE_AUTH = 15,             /* Authentication (MQTT 5) */
    MQTT_PACKET_TYPE_ANY = 16,
};

/* Packet types: Located in first byte of packet in bits 4-7 */
#define MQTT_PACKET_TYPE_GET(x)  (((x) >> 4) & 0xF)
#define MQTT_PACKET_TYPE_SET(x)  (((x) & 0xF) << 4)

/* Packet flag bit-mask: Located in first uint8_t of packet in bits 0-3 */
#define MQTT_PACKET_FLAGS_GET(x) ((x) & 0xF)
#define MQTT_PACKET_FLAGS_SET(x) (x)

#define MQTT_PACKET_FLAGS_GET_QOS(type_flags)   \
    ((MQTT_PACKET_FLAGS_GET((type_flags)) & \
        MQTT_PACKET_FLAG_QOS_MASK) >> MQTT_PACKET_FLAG_QOS_SHIFT)
#define MQTT_PACKET_FLAGS_SET_QOS(qos)   \
    (MQTT_PACKET_FLAGS_SET(((qos) << MQTT_PACKET_FLAG_QOS_SHIFT) & \
        MQTT_PACKET_FLAG_QOS_MASK))

enum {
    MQTT_PACKET_FLAG_RETAIN = 0x1,
    MQTT_PACKET_FLAG_QOS_SHIFT = 0x1,
    MQTT_PACKET_FLAG_QOS_MASK = 0x6,
    MQTT_PACKET_FLAG_DUPLICATE = 0x8,
};

/* Packet Header: Size is variable 2 - 5 bytes */
#define MQTT_PACKET_MAX_LEN_BYTES   4
#define MQTT_PACKET_LEN_ENCODE_MASK 0x80
struct _MqttHeader {
    /* Type = bits 4-7, bits 0-3 are flags */
    uint8_t type_flags; /* MqttPacketType and MqttPacketFlags */

    /* Remaining Length: variable 1-4 bytes, encoded using scheme
       where bit 7 = continuation bit */
    uint8_t len[MQTT_PACKET_MAX_LEN_BYTES];
};


/* CONNECT PACKET */
/* Connect flag bit-mask: Located in uint8_t 8 of the MqttConnect packet */
#define MQTT_CONNECT_FLAG_GET_QOS(flags) \
    (((flags) MQTT_CONNECT_FLAG_WILL_QOS_MASK) >> \
        MQTT_CONNECT_FLAG_WILL_QOS_SHIFT)
#define MQTT_CONNECT_FLAG_SET_QOS(qos) \
    (((qos) << MQTT_CONNECT_FLAG_WILL_QOS_SHIFT) & \
        MQTT_CONNECT_FLAG_WILL_QOS_MASK)
enum {
    MQTT_CONNECT_FLAG_RESERVED = 0x01,
    MQTT_CONNECT_FLAG_CLEAN_SESSION = 0x02, /* Old v3.1.1 name */
    MQTT_CONNECT_FLAG_CLEAN_START   = 0x02,
    MQTT_CONNECT_FLAG_WILL_FLAG = 0x04,
    MQTT_CONNECT_FLAG_WILL_QOS_SHIFT = 3,
    MQTT_CONNECT_FLAG_WILL_QOS_MASK = 0x18,
    MQTT_CONNECT_FLAG_WILL_RETAIN = 0x20,
    MQTT_CONNECT_FLAG_PASSWORD = 0x40,
    MQTT_CONNECT_FLAG_USERNAME = 0x80,
};

/* Connect Protocol */


/* CONNECT ACKNOWLEDGE */
/* Connect Ack flags */
enum {
    MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT = 0x01,
};

/* Connect Ack return codes */
enum {
    MQTT_CONNECT_ACK_CODE_ACCEPTED = 0,
    MQTT_CONNECT_ACK_CODE_REFUSED_PROTO = 1,
    MQTT_CONNECT_ACK_CODE_REFUSED_ID = 2,
    MQTT_CONNECT_ACK_CODE_REFUSED_UNAVAIL = 3,
    MQTT_CONNECT_ACK_CODE_REFUSED_BAD_USER_PWD = 4,
    MQTT_CONNECT_ACK_CODE_REFUSED_NOT_AUTH = 5,
};

/* Connect Ack packet structure */
struct _MqttConnectAck {
    uint8_t     flags;
    uint8_t     return_code;
};
/* Connect Ack has no payload */

/* CONNECT */
/* Constant values for the protocol name */
#define MQTT_CONNECT_PROTOCOL_NAME_LEN  4

/* version level */
#define MQTT_CONNECT_PROTOCOL_LEVEL_4   4 /* v3.1.1 */
#define MQTT_CONNECT_PROTOCOL_LEVEL_5   5 /* v5.0 */
#define MQTT_CONNECT_PROTOCOL_LEVEL     MQTT_CONNECT_PROTOCOL_LEVEL_4

/* Connect structure */
struct _MqttConnect {
    uint8_t     clean_session;
    uint8_t     protocol_level;
    uint16_t    keep_alive_sec;

    const char *client_id;

    /* (Optional) Last will and testament */
    MqttMessage *lwt_msg;

    /* (Optional) login */
    const char *username;
    const char *password;
};


/* PUBLISH RESPONSE */
/* This is the response struct for PUBLISH_ACK, PUBLISH_REC and PUBLISH_COMP */
/* If QoS = 0: No response */
/* If QoS = 1: Expect response packet with type =
    MQTT_PACKET_TYPE_PUBLISH_ACK */
/* If QoS = 2: Expect response packet with type =
    MQTT_PACKET_TYPE_PUBLISH_REC */
/* Packet Id required if QoS is 1 or 2 */
/* If Qos = 2: Send MQTT_PACKET_TYPE_PUBLISH_REL with PacketId to complete
    QoS2 protocol exchange */
/* Expect response packet with type = MQTT_PACKET_TYPE_PUBLISH_COMP */
struct _MqttPublishResp {
    uint16_t    packet_id;
};

/* PUBLISH */
/* PacketId sent only if QoS > 0 */
struct _MqttPublish {
    uint8_t     duplicate;
    uint8_t     qos;
    uint8_t     retain;
    uint16_t    packet_id;

    uint16_t    topic_name_len;
    const char *topic_name;
    char       *payload;        /* Payload buffer */
    uint32_t    payload_len;    /* Payload length */
};


/* SUBSCRIBE ACK */
/* Packet Id followed by list of return codes corresponding to subscription
    topic list sent. */
enum {
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS0 = 0,
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS1 = 1,
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS2 = 2,
    MQTT_SUBSCRIBE_ACK_CODE_FAILURE = 0x80,
};

struct _MqttSubscribeAck {
    uint16_t    packet_id;
    uint16_t    return_codes_count;
    uint8_t    *return_codes; /* List of return codes. */
};

/* SUBSCRIBE */
/* Packet Id followed by contiguous list of topics w/Qos to subscribe to. */
struct _MqttSubscribe {
    uint16_t    packet_id;
    uint16_t    topic_count;
    MqttTopic  *topics;
};


/* UNSUBSCRIBE ACK */
struct _MqttUnsubscribeAck {
    uint16_t    packet_id;
};
/* No response payload (besides packet Id) */

/* UNSUBSCRIBE */
/* Packet Id followed by contiguous list of topics to unsubscribe from. */
struct _MqttUnsubscribe {
    uint16_t    packet_id;
    uint16_t    topic_count;
    MqttTopic  *topics;
};






/* Returns positive number of buffer bytes decoded or negative error. "value"
   is the decoded variable byte integer */
extern int mqtt_decode_remaining_length(uint8_t *buf, uint32_t *value);


/* Packet Encoders/Decoders */
extern int mqtt_serialize_connect(char *tx_buf, int tx_buf_len,
    MqttConnect *connect);
extern int mqtt_deserialize_connect_ack(char *rx_buf, int rx_buf_len,
    MqttConnectAck *connect_ack);

extern int mqtt_serialize_publish(char *tx_buf, int tx_buf_len,
    MqttPublish *publish);
extern int mqtt_deserialize_publish(char *rx_buf, int rx_buf_len,
    MqttPublish *publish);
extern int mqtt_serialize_publish_resp(char *tx_buf, int tx_buf_len,
    uint8_t type, MqttPublishResp *publish_resp);
extern int mqtt_deserialize_publish_resp(char *rx_buf, int rx_buf_len,
    uint8_t type, MqttPublishResp *publish_resp);

extern int mqtt_serialize_subscribe(char *tx_buf, int tx_buf_len,
    MqttSubscribe *subscribe);
extern int mqtt_deserialize_subscribe_ack(char *rx_buf, int rx_buf_len,
    MqttSubscribeAck *subscribe_ack);

extern int mqtt_serialize_unsubscribe(char *tx_buf, int tx_buf_len,
    MqttUnsubscribe *unsubscribe);
extern int mqtt_deserialize_unsubscribe_ack(char *rx_buf, int rx_buf_len,
    MqttUnsubscribeAck *unsubscribe_ack);

extern int mqtt_serialize_ping(char *tx_buf, int tx_buf_len);
extern int mqtt_deserialize_ping(char *rx_buf, int rx_buf_len);

extern int mqtt_serialize_disconnect(char *tx_buf, int tx_buf_len);




#endif /* __MQTT_PACKET_H__ */
