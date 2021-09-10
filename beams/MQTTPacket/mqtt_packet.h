
#ifndef __MQTT_PACKET_H__
#define __MQTT_PACKET_H__


#include "./app_cfg.h"

#ifndef MQTT_PACKEt_MAX_LENGtH
#define MQTT_PACKEt_MAX_LENGtH  (2048u)
#endif

enum {
    MQTT_PACKET_ERROR_NONE           = 0,
    MQTT_PACKET_ERROR_BAD_ARG        = -1,
    MQTT_PACKET_ERROR_OUT_OF_BUFFER  = -2,
    MQTT_PACKET_ERROR_INVALID_FORMAT = -3, /* eg. Malformed Remaining */
    MQTT_PACKET_ERROR_PACKET_TYPE    = -4,
    MQTT_PACKET_ERROR_PACKET_ID      = -5,
};


enum {
    MQTT_QOS_0 = 0, /* At most once delivery */
    MQTT_QOS_1 = 1, /* At least once delivery */
    MQTT_QOS_2 = 2, /* Exactly once delivery */
};


typedef struct {
    const char *topic_filter;
    uint8_t     qos;
} mqtt_topic_t;


/* Packet types: Located in first byte of packet in bits 4-7 */
#define MQTT_PACKET_TYPE_GET(x)  (((x) >> 4) & 0xF)

enum {
    MQTT_PACKET_TYPE_RESERVED = 0,
    MQTT_PACKET_TYPE_CONNECT = 1,           //!< CONNECT
    MQTT_PACKET_TYPE_CONNECT_ACK = 2,       //!< CONNACK
    MQTT_PACKET_TYPE_PUBLISH = 3,           //!< PUBLISH
    MQTT_PACKET_TYPE_PUBLISH_ACK = 4,       //!< PUBACK
    MQTT_PACKET_TYPE_PUBLISH_REC = 5,       //!< PUBREC
    MQTT_PACKET_TYPE_PUBLISH_REL= 6,        //!< PUBREL
    MQTT_PACKET_TYPE_PUBLISH_COMP = 7,      //!< PUBCOMP
    MQTT_PACKET_TYPE_SUBSCRIBE = 8,         //!< SUBSCRIBE
    MQTT_PACKET_TYPE_SUBSCRIBE_ACK = 9,     //!< SUBACK
    MQTT_PACKET_TYPE_UNSUBSCRIBE = 10,      //!< UNSUBSCRIBE
    MQTT_PACKET_TYPE_UNSUBSCRIBE_ACK = 11,  //!< UNSUBACK
    MQTT_PACKET_TYPE_PING_REQ = 12,         //!< PINGREQ
    MQTT_PACKET_TYPE_PING_RESP = 13,        //!< PINGRESP
    MQTT_PACKET_TYPE_DISCONNECT = 14,       //!< DISCONNECT
    MQTT_PACKET_TYPE_AUTH = 15,             //!< AUTH
};

/* Packet Header: Size is variable 2 - 5 bytes */
#define MQTT_PACKET_MAX_LEN_BYTES   4
#define MQTT_PACKET_LEN_ENCODE_MASK 0x80
typedef struct {
    /* Type = bits 4-7, bits 0-3 are flags */
    uint8_t type_flags;
    uint8_t remaining_length[MQTT_PACKET_MAX_LEN_BYTES];
} mqtt_header_t;

enum {
    MQTT_CONNECT_PROTOCOL_LEVEL_4 = 4, /* for version v3.1.1 */
    MQTT_CONNECT_PROTOCOL_LEVEL_5 = 5, /* for version v5.0 */
};

typedef struct mqtt_publish_t mqtt_publish_t;

typedef struct {
    uint8_t             is_clean_session;
    uint8_t             protocol_level;
    uint16_t            keep_alive_sec;

    const char         *client_id;

    /* (Optional) login */
    const char         *username;
    const char         *password;

    /* (Optional) Last will and testament */
    const mqtt_publish_t   *lwt_msg;
} mqtt_connect_t;


enum {
    MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT = 0x01,
};

enum {
    MQTT_CONNECT_ACK_CODE_ACCEPTED = 0,
    MQTT_CONNECT_ACK_CODE_REFUSED_PROTO = 1,
    MQTT_CONNECT_ACK_CODE_REFUSED_ID = 2,
    MQTT_CONNECT_ACK_CODE_REFUSED_UNAVAIL = 3,
    MQTT_CONNECT_ACK_CODE_REFUSED_BAD_USER_PWD = 4,
    MQTT_CONNECT_ACK_CODE_REFUSED_NOT_AUTH = 5,
};

typedef struct {
    uint8_t         flags;
    uint8_t         return_code;
} mqtt_connack_t;


struct mqtt_publish_t {
    uint8_t         duplicate;
    uint8_t         qos;
    uint8_t         retain;
    uint16_t        packet_id; /* PacketId used only if QoS > 0 */

    uint16_t        topic_name_len;
    uint16_t        payload_len;
    const char     *topic_name;
    char           *payload;
};


/* PUBLISH RESPONSE */
/* This is the struct for PUBACK, PUBREC and PUBCOMP packets */
/* for their packet structures are identical except the "packet type" domain. */
typedef struct {
    uint16_t        packet_id;
} mqtt_publish_resp_t;


typedef struct {
    uint16_t            packet_id;
    
    uint16_t            topic_count;
    mqtt_topic_t       *topics; /* contiguous list of topics to subscribe to. */
} mqtt_subscribe_t;


enum {
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS0 = 0,
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS1 = 1,
    MQTT_SUBSCRIBE_ACK_CODE_SUCCESS_MAX_QOS2 = 2,
    MQTT_SUBSCRIBE_ACK_CODE_FAILURE          = 0x80,
};

typedef struct {
    uint16_t        packet_id;
    
    /* list of return codes corresponding to the topic list in mqtt_subscribe_t. */
    uint16_t        return_codes_count;
    uint8_t        *return_codes;
} mqtt_subscribe_ack_t;


typedef struct {
    uint16_t            packet_id;
    
    uint16_t            topic_count;
    mqtt_topic_t       *topics; /* contiguous list of topics to subscribe to. */
} mqtt_unsubscribe_t;


typedef struct {
    uint16_t    packet_id;
} mqtt_unsubscribe_ack_t;


typedef struct {
    char        PacketBuffer[MQTT_PACKEt_MAX_LENGtH];
    uint8_t     State;
    uint32_t    DataCount;
    uint32_t    WriteIndex;
} mqtt_packet_parser_t;


typedef struct {
    void *Param;
    void (*Fun)(void *param, const char *buf, uint32_t len);
} mqtt_packet_parser_callback_t;




/* Returns positive number of buffer bytes decoded or negative error. "value"
   is the decoded variable byte integer */
extern int mqtt_decode_remaining_length     (const uint8_t *buf, uint32_t *value);


extern int mqtt_serialize_connect           (char *tx_buf, int len, mqtt_connect_t *connect);
extern int mqtt_deserialize_connect_ack     (const char *rx_buf, int len, mqtt_connack_t *connect_ack);

extern int mqtt_serialize_publish           (char *tx_buf, int len, mqtt_publish_t *publish);
extern int mqtt_deserialize_publish         (const char *rx_buf, int len, mqtt_publish_t *publish);
extern int mqtt_serialize_publish_resp      (char *tx_buf, int len, uint8_t type, mqtt_publish_resp_t *publish_resp);
extern int mqtt_deserialize_publish_resp    (const char *rx_buf, int len, uint8_t type, mqtt_publish_resp_t *publish_resp);

extern int mqtt_serialize_subscribe         (char *tx_buf, int len, mqtt_subscribe_t *subscribe);
extern int mqtt_deserialize_subscribe_ack   (const char *rx_buf, int len, mqtt_subscribe_ack_t *subscribe_ack);

extern int mqtt_serialize_unsubscribe       (char *tx_buf, int len, mqtt_unsubscribe_t *unsubscribe);
extern int mqtt_deserialize_unsubscribe_ack (const char *rx_buf, int len, mqtt_unsubscribe_ack_t *unsubscribe_ack);

extern int mqtt_serialize_ping      (char *tx_buf, int len);
extern int mqtt_deserialize_ping    (const char *rx_buf, int len);

extern int mqtt_serialize_disconnect(char *tx_buf, int len);




#endif /* __MQTT_PACKET_H__ */
