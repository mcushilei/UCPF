
#include "./mqtt_packet.h"


/* Size of a data length segment in the packet */
#define SIZE_OF_LENGTH      (2u)
#define SIZE_OF_PACKET_ID   (2u)


#define MQTT_PACKET_TYPE_SET(x)  (((x) & 0xF) << 4)


/* Packet flag bit-mask: Located in first uint8_t of packet in bits 0-3 */
#define MQTT_PACKET_FLAGS_GET(x) ((x) & 0xF)
#define MQTT_PACKET_FLAGS_SET(x) (x)

#define MQTT_PACKET_FLAGS_GET_QOS(type_flags)   \
    ( MQTT_PACKET_FLAGS_GET((type_flags)) & MQTT_PACKET_FLAG_QOS_MASK ) >> MQTT_PACKET_FLAG_QOS_SHIFT
#define MQTT_PACKET_FLAGS_SET_QOS(qos)   \
    MQTT_PACKET_FLAGS_SET( ( (qos) << MQTT_PACKET_FLAG_QOS_SHIFT ) & MQTT_PACKET_FLAG_QOS_MASK)

enum {
    MQTT_PACKET_FLAG_QOS_SHIFT  = 0x1,
    MQTT_PACKET_FLAG_QOS_MASK   = 0x6,
    MQTT_PACKET_FLAG_RETAIN     = 0x1,
    MQTT_PACKET_FLAG_DUPLICATE  = 0x8,
};



/* Connect flag bit-mask: Located in uint8_t 8 of the mqtt_connect_t packet */
#define MQTT_CONNECT_FLAG_GET_QOS(flags) \
    ( ( (flags) & MQTT_CONNECT_FLAG_WILL_QOS_MASK ) >> MQTT_CONNECT_FLAG_WILL_QOS_SHIFT )
#define MQTT_CONNECT_FLAG_SET_QOS(qos) \
    ( ( (qos) << MQTT_CONNECT_FLAG_WILL_QOS_SHIFT ) & MQTT_CONNECT_FLAG_WILL_QOS_MASK )
enum {
    MQTT_CONNECT_FLAG_RESERVED      = 0x01,
    MQTT_CONNECT_FLAG_CLEAN_SESSION = 0x02, /* Old v3.1.1 name */
    MQTT_CONNECT_FLAG_CLEAN_START   = 0x02,
    MQTT_CONNECT_FLAG_WILL_FLAG     = 0x04,
    MQTT_CONNECT_FLAG_WILL_QOS_SHIFT = 3,
    MQTT_CONNECT_FLAG_WILL_QOS_MASK = 0x18,
    MQTT_CONNECT_FLAG_WILL_RETAIN   = 0x20,
    MQTT_CONNECT_FLAG_PASSWORD      = 0x40,
    MQTT_CONNECT_FLAG_USERNAME      = 0x80,
};


/* Returns positive number of buffer bytes decoded or negative error. "value"
   is the decoded variable byte integer */
int mqtt_decode_remaining_length(const uint8_t *buf, uint32_t *value)
{
    int rc = 0;
    int multiplier = 1;
    uint8_t encodedByte;

    *value = 0;
    do {
       encodedByte = *(buf++);
       *value += (encodedByte & ~MQTT_PACKET_LEN_ENCODE_MASK) * multiplier;
       if (multiplier > (MQTT_PACKET_LEN_ENCODE_MASK *
                         MQTT_PACKET_LEN_ENCODE_MASK *
                         MQTT_PACKET_LEN_ENCODE_MASK))
       {
          return MQTT_PACKET_ERROR_INVALID_FORMAT;
       }
       multiplier *= MQTT_PACKET_LEN_ENCODE_MASK;
       rc++;
    } while ((encodedByte & MQTT_PACKET_LEN_ENCODE_MASK) != 0);

    return rc;
}

/* Encodes to buf a non-negative integer "x" in a Variable Byte Integer scheme.
   Returns the number of bytes encoded.
   If buf is NULL, return number of bytes that would be encoded. */
int mqtt_encode_remaining_length(uint8_t *buf, uint32_t x)
{
    int rc = 0;
    uint8_t encodedByte;

    do {
       encodedByte = x % MQTT_PACKET_LEN_ENCODE_MASK;
       x /= MQTT_PACKET_LEN_ENCODE_MASK;
       // if there are more data to encode, set the top bit of this uint8_t
       if (x > 0) {
          encodedByte |= MQTT_PACKET_LEN_ENCODE_MASK;
       }
       if (buf != NULL) {
           *(buf++) = encodedByte;
       }
       rc++;
    } while (x > 0);

    return rc;
}


#define PARSER_ON_REMAINING_LENGTH()            \
do {                                            \
    rc = mqtt_decode_remaining_length(header->remaining_length, &remain_len);    \
    if (rc < 0) {                                       \
        obj->State = WAIT_FOR_TYPE;                     \
        return rc;                                      \
    }                                                   \
    if( remain_len > sizeof(obj->PacketBuffer) ) {      \
        obj->State = WAIT_FOR_TYPE;                     \
        return MQTT_PACKET_ERROR_INVALID_FORMAT;        \
    } else if( remain_len == 0u ) {             \
        obj->State = WAIT_FOR_TYPE;             \
        callback->Fun( callback->Param, obj->PacketBuffer, obj->WriteIndex );   \
    } else {                                    \
        obj->State = WAIT_FOR_DATA;             \
        obj->DataCount = remain_len;            \
    }                                           \
} while ( 0 )

static int parser_package( void *parser, const uint8_t *data, uint32_t len, mqtt_packet_parser_callback_t *callback )
{
    enum {
        WAIT_FOR_TYPE = 0,
        WAIT_FOR_LENGTH_0,
        WAIT_FOR_LENGTH_1,
        WAIT_FOR_LENGTH_2,
        WAIT_FOR_LENGTH_3,
        WAIT_FOR_DATA,
    };
    int rc;
    mqtt_packet_parser_t *obj = (mqtt_packet_parser_t *)parser;
    mqtt_header_t *header = (mqtt_header_t *)obj->PacketBuffer;
    uint8_t byte;
    uint32_t remain_len = 0;

    for( uint32_t i = 0; i < len; i++ ) {
        byte = data[i];

        switch( obj->State ) {
        case WAIT_FOR_TYPE:
            obj->WriteIndex = 0u;
            obj->DataCount = 0u;
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            obj->State = WAIT_FOR_LENGTH_0;
            break;

        case WAIT_FOR_LENGTH_0:
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            if ( byte & 0x80 ){
                obj->State = WAIT_FOR_LENGTH_1;
            } else {
                PARSER_ON_REMAINING_LENGTH();
            }
            break;

        case WAIT_FOR_LENGTH_1:
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            if( byte & 0x80 ) {
                obj->State = WAIT_FOR_LENGTH_2;
            } else {
                PARSER_ON_REMAINING_LENGTH();
            }
            break;

        case WAIT_FOR_LENGTH_2:
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            if( byte & 0x80 ) {
                obj->State = WAIT_FOR_LENGTH_3;
            } else {
                PARSER_ON_REMAINING_LENGTH();
            }
            break;

        case WAIT_FOR_LENGTH_3:
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            if( byte & 0x80 ) {
                obj->State = WAIT_FOR_TYPE;
                return MQTT_PACKET_ERROR_INVALID_FORMAT;
            } else {
                PARSER_ON_REMAINING_LENGTH();
            }
            break;

        case WAIT_FOR_DATA:
            obj->PacketBuffer[obj->WriteIndex++] = byte;
            obj->DataCount--;
            if (0 == obj->DataCount) {
                callback->Fun( callback->Param, obj->PacketBuffer, obj->WriteIndex );
                obj->State = WAIT_FOR_TYPE;
            }
            break;
        }
    }

    return 0;
}


/* Positive return value is header length, zero or negative indicates error */
static int mqtt_serialize_fixed_header(char *tx_buf, int len,
                                       int remain_len,
                                       uint8_t type,
                                       uint8_t retain,
                                       uint8_t qos,
                                       uint8_t duplicate)
{
    int header_len;
    mqtt_header_t* header = (mqtt_header_t*)tx_buf;

    /* make sure destination buffer has space for header */
    if (len < MQTT_PACKET_MAX_LEN_BYTES+1) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }

    /* 1. Encode fixed header */
    header->type_flags = MQTT_PACKET_TYPE_SET(type) | MQTT_PACKET_FLAGS_SET(0);
    if (retain) {
        header->type_flags |= MQTT_PACKET_FLAGS_SET(MQTT_PACKET_FLAG_RETAIN);
    }
    if (qos) {
        header->type_flags |= MQTT_PACKET_FLAGS_SET_QOS(qos);
    }
    if (duplicate) {
        header->type_flags |=
            MQTT_PACKET_FLAGS_SET(MQTT_PACKET_FLAG_DUPLICATE);
    }

    /* Encode the length remaining into the header */
    header_len = mqtt_encode_remaining_length(header->remaining_length, remain_len);
    if (header_len > 0) {
        header_len++; /* Add one for type and flags */
    }

    return header_len;
}

static int mqtt_deserialize_fixed_header(const char *rx_buf, int len, int *remain_len,
    uint8_t type, uint8_t *p_qos, uint8_t *p_retain, uint8_t *p_duplicate)
{
    int header_len;
    const mqtt_header_t *header = (mqtt_header_t *)rx_buf;

    /* Decode the length remaining */
    header_len = mqtt_decode_remaining_length(header->remaining_length, (uint32_t *)remain_len);
    if (header_len < 0) {
        return header_len;
    }

    /* Validate packet type */
    if (MQTT_PACKET_TYPE_GET(header->type_flags) != type) {
        return MQTT_PACKET_ERROR_PACKET_TYPE;
    }

    /* Extract header flags */
    if (p_qos) {
        *p_qos = (uint8_t)MQTT_PACKET_FLAGS_GET_QOS(header->type_flags);
    }
    if (p_retain) {
        *p_retain = (MQTT_PACKET_FLAGS_GET(header->type_flags) & MQTT_PACKET_FLAG_RETAIN) ? 1 : 0;
    }
    if (p_duplicate) {
        *p_duplicate = (MQTT_PACKET_FLAGS_GET(header->type_flags) & MQTT_PACKET_FLAG_DUPLICATE) ? 1 : 0;
    }

    header_len += sizeof(header->type_flags); /* Add size of type and flags */

    (void)len;

    return header_len;
}

/* Returns number of buffer bytes decoded */
static int mqtt_deserialize_num(const uint8_t *buf, uint16_t *len)
{
    if (len) {
        *len = buf[0];
        *len = (*len << 8) | buf[1];
    }
    return SIZE_OF_LENGTH;
}

/* Returns number of buffer bytes encoded
   If buf is NULL, return number of bytes that would be encoded. */
static int mqtt_serialize_num(char *buf, uint16_t len)
{
    if (buf != NULL) {
        buf[0] = len >> 8;
        buf[1] = len & 0xFF;
    }
    return SIZE_OF_LENGTH;
}

/* Returns number of buffer bytes decoded */
/* Returns pointer to string (note: it is NOT guaranteed to be a '\0' terminated string!) */
static int mqtt_deserialize_string(const char *buf, const char **pstr, uint16_t *pstr_len)
{
    uint16_t str_len;
    int len;

    len = mqtt_deserialize_num((uint8_t *)buf, &str_len);
    buf += len;
    if (pstr) {
        *pstr = buf;
    }
    if (pstr_len) {
        *pstr_len = str_len;
    }
    return len + str_len;
}

/* Returns number of buffer bytes encoded
   If buf is NULL, return number of bytes that would be encoded. */
static int mqtt_serialize_string(char *buf, const char *str)
{
    uint16_t str_len = strlen(str);
    int len = mqtt_serialize_num(buf, str_len);

    if (buf != NULL) {
        buf += len;
        memcpy(buf, str, str_len);
    }
    return len + str_len;
}



int mqtt_serialize_connect(char *tx_buf, int len, mqtt_connect_t *connect)
{
    int header_len = 0, remain_len = 0;
    uint8_t  flags = 0;
    char *buf_ptr;

    /* Validate required arguments */
    if (tx_buf == NULL || connect == NULL || connect->client_id == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    if (connect->protocol_level == MQTT_CONNECT_PROTOCOL_LEVEL_4) {
        remain_len = 10;
    } else if (connect->protocol_level == MQTT_CONNECT_PROTOCOL_LEVEL_5) {
        remain_len = 16;
    } else {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }
    remain_len += strlen(connect->client_id) + SIZE_OF_LENGTH;
    if (connect->lwt_msg != NULL) {
        /* Verify all required fields are present */
        if (connect->lwt_msg->topic_name == NULL ||
            connect->lwt_msg->payload == NULL ||
            connect->lwt_msg->payload_len == 0) {
            return MQTT_PACKET_ERROR_BAD_ARG;
        }

        remain_len += strlen(connect->lwt_msg->topic_name);
        remain_len += SIZE_OF_LENGTH;
        remain_len += connect->lwt_msg->payload_len;
        remain_len += SIZE_OF_LENGTH;
    }
    if (connect->username) {
        remain_len += strlen(connect->username) + SIZE_OF_LENGTH;
    }
    if (connect->password) {
        remain_len += strlen(connect->password) + SIZE_OF_LENGTH;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        MQTT_PACKET_TYPE_CONNECT, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }
    buf_ptr = &tx_buf[header_len];

    /* 2. Encode variable header */
    /* Set connection flags */
    if (connect->is_clean_session) {
        flags |= MQTT_CONNECT_FLAG_CLEAN_SESSION;
    }
    if (connect->lwt_msg != NULL) {
        flags |= MQTT_CONNECT_FLAG_WILL_FLAG;

        if (connect->lwt_msg->qos) {
            flags |= MQTT_CONNECT_FLAG_SET_QOS(connect->lwt_msg->qos);
        }
        if (connect->lwt_msg->retain) {
            flags |= MQTT_CONNECT_FLAG_WILL_RETAIN;
        }
    }
    if (connect->username) {
        flags |= MQTT_CONNECT_FLAG_USERNAME;
    }
    if (connect->password) {
        flags |= MQTT_CONNECT_FLAG_PASSWORD;
    }

    buf_ptr += mqtt_serialize_string(buf_ptr, "MQTT");
    *buf_ptr++ = connect->protocol_level;
    *buf_ptr++ = flags;
    buf_ptr += mqtt_serialize_num(buf_ptr, connect->keep_alive_sec);

    /* 3. Encode payload */
    buf_ptr += mqtt_serialize_string(buf_ptr, connect->client_id);
    if (connect->lwt_msg != NULL) {
        buf_ptr += mqtt_serialize_string(buf_ptr,
                                            connect->lwt_msg->topic_name);
        buf_ptr += mqtt_serialize_num(buf_ptr,
                                         connect->lwt_msg->payload_len);
        memcpy(buf_ptr, connect->lwt_msg->payload, connect->lwt_msg->payload_len);
        buf_ptr += connect->lwt_msg->payload_len;
    }
    if (connect->username) {
        buf_ptr += mqtt_serialize_string(buf_ptr, connect->username);
    } else {
        /* A Server MAY allow a Client to supply a ClientID that has a length
         * of zero bytes, however if it does so the Server MUST treat this as a
         * special case and assign a unique ClientID to that Client
         * [MQTT-3.1.3-6]. It MUST then process the CONNECT packet as if the
         * Client had provided that unique ClientID, and MUST return the
         * Assigned Client Identifier in the CONNACK packet [MQTT-3.1.3-7].
         */
        buf_ptr += mqtt_serialize_num(buf_ptr, (uint16_t)0);
    }
    if (connect->password) {
        buf_ptr += mqtt_serialize_string(buf_ptr, connect->password);
    }

    (void)buf_ptr;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_connect_ack(const char *rx_buf, int len, mqtt_connack_t *connect_ack)
{
    int header_len = 0, remain_len = 0;
    const uint8_t *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len, &remain_len,
        MQTT_PACKET_TYPE_CONNECT_ACK, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (connect_ack) {
        connect_ack->flags = *rx_payload++;
        connect_ack->return_code = *rx_payload++;
    }

    (void)rx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_publish(char *tx_buf, int len, mqtt_publish_t *publish)
{
    int header_len = 0, remain_len = 0, variable_len = 0, payload_len = 0;
    char *buf_ptr = tx_buf;

    /* Validate required arguments */
    if (tx_buf == NULL || publish == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    variable_len = strlen(publish->topic_name) + SIZE_OF_LENGTH;
    if (publish->qos > MQTT_QOS_0) {
        if (publish->packet_id == 0) {
            return MQTT_PACKET_ERROR_PACKET_ID;
        }
        variable_len += SIZE_OF_LENGTH; /* For packet_id */
    }

    if ((publish->payload != NULL) && (publish->payload_len > 0)) {
        payload_len = publish->payload_len;
    }

    remain_len = variable_len + payload_len;

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len,
        remain_len, MQTT_PACKET_TYPE_PUBLISH,
        publish->retain, publish->qos, publish->duplicate);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }
    buf_ptr = &tx_buf[header_len];

    /* 2. Encode variable header */
    buf_ptr += mqtt_serialize_string(buf_ptr, publish->topic_name);
    if (publish->qos > MQTT_QOS_0) {
        buf_ptr += mqtt_serialize_num(buf_ptr, publish->packet_id);
    }

    /* 3. Encode payload */
    if (payload_len > 0) {
        memcpy(buf_ptr, publish->payload, payload_len);
    }

    /* Return length of packet placed into tx_buf */
    return header_len + remain_len;
}

int mqtt_deserialize_publish(const char *rx_buf, int len, mqtt_publish_t *publish)
{
    int header_len = 0, remain_len = 0, variable_len = 0, payload_len = 0;
    const uint8_t *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0 || publish == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len,
        &remain_len, MQTT_PACKET_TYPE_PUBLISH, &publish->qos,
        &publish->retain, &publish->duplicate);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    variable_len = mqtt_deserialize_string(rx_payload, &publish->topic_name,
        &publish->topic_name_len);
    rx_payload += variable_len;

    /* If QoS > 0 then get packet Id */
    if (publish->qos > MQTT_QOS_0) {
        variable_len += mqtt_deserialize_num(rx_payload, &publish->packet_id);
        rx_payload += SIZE_OF_LENGTH;
    }

    /* Decode Payload */
    payload_len = remain_len - variable_len;
    publish->payload     = (char *)rx_payload;
    publish->payload_len = payload_len;

    return header_len + remain_len;
}

int mqtt_serialize_publish_resp(char *tx_buf, int len, uint8_t type, mqtt_publish_resp_t *publish_resp)
{
    int header_len = 0, remain_len = 0;
    char *buf_ptr;
    uint8_t qos;

    /* Validate required arguments */
    if (tx_buf == NULL || publish_resp == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = SIZE_OF_LENGTH; /* For packet_id */

    /* Determine Qos value */
    qos = (type == MQTT_PACKET_TYPE_PUBLISH_REL) ? MQTT_QOS_1 : MQTT_QOS_0;

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        type, 0, qos, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }
    buf_ptr = &tx_buf[header_len];

    /* 2. Encode variable header */
    buf_ptr += mqtt_serialize_num(&tx_buf[header_len], publish_resp->packet_id);

    (void)buf_ptr;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_publish_resp(const char *rx_buf, int len, uint8_t type, mqtt_publish_resp_t *publish_resp)
{
    int header_len = 0, remain_len = 0;
    const uint8_t *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len, &remain_len,
        type, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (publish_resp) {
        rx_payload += mqtt_deserialize_num(rx_payload, &publish_resp->packet_id);
    }

    (void)rx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_subscribe(char *tx_buf, int len, mqtt_subscribe_t *subscribe)
{
    int header_len = 0, remain_len = 0;
    int i;
    char *buf_ptr;
    mqtt_topic_t *topic;

    /* Validate required arguments */
    if (tx_buf == NULL || subscribe == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = SIZE_OF_LENGTH; /* For packet_id */
    for (i = 0; i < subscribe->topic_count; i++) {
        topic = &subscribe->topics[i];
        if ((topic != NULL) && (topic->topic_filter != NULL)) {
            remain_len += strlen(topic->topic_filter) + SIZE_OF_LENGTH;
            remain_len++; /* For QoS */
        } else {
            return MQTT_PACKET_ERROR_BAD_ARG;
        }
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        MQTT_PACKET_TYPE_SUBSCRIBE, 0, MQTT_QOS_1, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }
    buf_ptr = &tx_buf[header_len];

    /* 2. Encode variable header */
    buf_ptr += mqtt_serialize_num(&tx_buf[header_len], subscribe->packet_id);

    /* 3. Encode payload */
    for (i = 0; i < subscribe->topic_count; i++) {
        topic = &subscribe->topics[i];
        buf_ptr += mqtt_serialize_string(buf_ptr, topic->topic_filter);
        *buf_ptr++ = topic->qos;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_subscribe_ack(const char *rx_buf, int len, mqtt_subscribe_ack_t *subscribe_ack)
{
    int header_len = 0, remain_len = 0;
    int payload_length = 0;
    int return_codes_count = 0;
    const uint8_t *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0 || subscribe_ack == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len, &remain_len,
        MQTT_PACKET_TYPE_SUBSCRIBE_ACK, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];
    
    /* 2. Decode variable header */
    if (subscribe_ack != NULL) {
        header_len += mqtt_deserialize_num(&rx_buf[header_len], &subscribe_ack->packet_id);
        
        if (len > header_len) {
            payload_length = len - header_len;
        }
        
        return_codes_count = payload_length < subscribe_ack->return_codes_count ? payload_length : subscribe_ack->return_codes_count;
        for (int i = 0; i < return_codes_count; i++) {
            subscribe_ack->return_codes[i] = rx_payload[i];
        }
        subscribe_ack->return_codes_count = return_codes_count;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_unsubscribe(char *tx_buf, int len, mqtt_unsubscribe_t *unsubscribe)
{
    int header_len = 0, remain_len = 0;
    int  i;
    char *buf_ptr;
    mqtt_topic_t *topic;

    /* Validate required arguments */
    if (tx_buf == NULL || unsubscribe == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = SIZE_OF_PACKET_ID;
    for (i = 0; i < unsubscribe->topic_count; i++) {
        topic = &unsubscribe->topics[i];
        if ((topic != NULL) && (topic->topic_filter != NULL)) {
            remain_len += strlen(topic->topic_filter) + SIZE_OF_LENGTH;
        } else {
            return MQTT_PACKET_ERROR_BAD_ARG;
        }
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        MQTT_PACKET_TYPE_UNSUBSCRIBE, 0, MQTT_QOS_1, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }
    buf_ptr = &tx_buf[header_len];

    /* 2. Encode variable header */
    buf_ptr += mqtt_serialize_num(&tx_buf[header_len], unsubscribe->packet_id);

    /* 3. Encode payload */
    for (i = 0; i < unsubscribe->topic_count; i++) {
        topic = &unsubscribe->topics[i];
        buf_ptr += mqtt_serialize_string(buf_ptr, topic->topic_filter);
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_unsubscribe_ack(const char *rx_buf, int len, mqtt_unsubscribe_ack_t *unsubscribe_ack)
{
    int header_len = 0, remain_len = 0;
    const uint8_t *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0 || unsubscribe_ack == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len, &remain_len,
        MQTT_PACKET_TYPE_UNSUBSCRIBE_ACK, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (unsubscribe_ack) {
        rx_payload += mqtt_deserialize_num(rx_payload, &unsubscribe_ack->packet_id);
    }

    (void)rx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_ping(char *tx_buf, int len)
{
    int header_len = 0, remain_len = 0;

    /* Validate required arguments */
    if (tx_buf == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        MQTT_PACKET_TYPE_PING_REQ, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_ping(const char *rx_buf, int len)
{
    int header_len = 0, remain_len = 0;

    /* Validate required arguments */
    if (rx_buf == NULL || len <= 0) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, len, &remain_len,
        MQTT_PACKET_TYPE_PING_RESP, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_disconnect(char *tx_buf, int len)
{
    int header_len = 0, remain_len = 0;

    /* Validate required arguments */
    if (tx_buf == NULL) {
        return MQTT_PACKET_ERROR_BAD_ARG;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, len, remain_len,
        MQTT_PACKET_TYPE_DISCONNECT, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (len < header_len + remain_len) {
        return MQTT_PACKET_ERROR_OUT_OF_BUFFER;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

