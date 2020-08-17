
#include "./mqtt_packet.h"



/* Returns positive number of buffer bytes decoded or negative error. "value"
   is the decoded variable byte integer */
int mqtt_decode_remaining_length(uint8_t *buf, uint32_t *value)
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
          return MQTT_CODE_ERROR_MALFORMED_DATA;
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


/* Positive return value is header length, zero or negative indicates error */
static int mqtt_serialize_fixed_header(char *tx_buf, int tx_buf_len, int remain_len,
    uint8_t type, uint8_t retain, uint8_t qos, uint8_t duplicate)
{
    int header_len;
    MqttHeader* header = (MqttHeader*)tx_buf;

    /* make sure destination buffer has space for header */
    if (tx_buf_len < MQTT_PACKET_MAX_LEN_BYTES+1) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
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
    header_len = mqtt_encode_remaining_length(header->len, remain_len);
    if (header_len > 0) {
        header_len++; /* Add one for type and flags */
    }

    return header_len;
}

static int mqtt_deserialize_fixed_header(char *rx_buf, int rx_buf_len, int *remain_len,
    uint8_t type, uint8_t *p_qos, uint8_t *p_retain, uint8_t *p_duplicate)
{
    int header_len;
    MqttHeader* header = (MqttHeader*)rx_buf;

    /* Decode the length remaining */
    header_len = mqtt_decode_remaining_length(header->len, (uint32_t*)remain_len);
    if (header_len < 0) {
        return header_len;
    }

    /* Validate packet type */
    if (MQTT_PACKET_TYPE_GET(header->type_flags) != type) {
        return MQTT_CODE_ERROR_PACKET_TYPE;
    }

    /* Extract header flags */
    if (p_qos) {
        *p_qos = (uint8_t)MQTT_PACKET_FLAGS_GET_QOS(header->type_flags);
    }
    if (p_retain) {
        *p_retain = (MQTT_PACKET_FLAGS_GET(header->type_flags) &
            MQTT_PACKET_FLAG_RETAIN) ? 1 : 0;
    }
    if (p_duplicate) {
        *p_duplicate = (MQTT_PACKET_FLAGS_GET(header->type_flags) &
            MQTT_PACKET_FLAG_DUPLICATE) ? 1 : 0;
    }

    header_len += sizeof(header->type_flags); /* Add size of type and flags */

    (void)rx_buf_len;

    return header_len;
}

/* Returns number of buffer bytes decoded */
static int mqtt_deserialize_num(uint8_t* buf, uint16_t *len)
{
    if (len) {
        *len = buf[0];
        *len = (*len << 8) | buf[1];
    }
    return MQTT_SIZE_OF_LENGTH;
}

/* Returns number of buffer bytes encoded
   If buf is NULL, return number of bytes that would be encoded. */
static int mqtt_serialize_num(char *buf, uint16_t len)
{
    if (buf != NULL) {
        buf[0] = len >> 8;
        buf[1] = len & 0xFF;
    }
    return MQTT_SIZE_OF_LENGTH;
}

/* Returns number of buffer bytes decoded */
/* Returns pointer to string (which is not guaranteed to be null terminated) */
static int mqtt_deserialize_string(char *buf, const char **pstr, uint16_t *pstr_len)
{
    int len;
    uint16_t str_len;

    len = mqtt_deserialize_num((uint8_t *)buf, &str_len);
    buf += len;
    if (pstr_len) {
        *pstr_len = str_len;
    }
    if (pstr) {
        *pstr = buf;
    }
    return len + str_len;
}

/* Returns number of buffer bytes encoded
   If buf is NULL, return number of bytes that would be encoded. */
static int mqtt_serialize_string(char *buf, const char *str)
{
    int str_len = strlen(str);
    int len = mqtt_serialize_num(buf, (uint16_t)str_len);

    if (buf != NULL) {
        buf += len;
        memcpy(buf, str, str_len);
    }
    return len + str_len;
}



int mqtt_serialize_connect(char *tx_buf, int tx_buf_len, MqttConnect *connect)
{
    int header_len, remain_len;
    uint8_t  flags = 0;
    char *tx_payload;

    /* Validate required arguments */
    if (tx_buf == NULL || connect == NULL || connect->client_id == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    if (connect->protocol_level == MQTT_CONNECT_PROTOCOL_LEVEL_4) {
        remain_len = 10;
    } else if (connect->protocol_level == MQTT_CONNECT_PROTOCOL_LEVEL_5) {
        remain_len = 16;
    } else {
        return MQTT_CODE_ERROR_BAD_ARG;
    }
    remain_len += strlen(connect->client_id) + MQTT_SIZE_OF_LENGTH;
    if (connect->lwt_msg != NULL) {
        /* Verify all required fields are present */
        if (connect->lwt_msg->topic_name == NULL ||
            connect->lwt_msg->payload == NULL ||
            connect->lwt_msg->payload_len == 0) {
            return MQTT_CODE_ERROR_BAD_ARG;
        }

        remain_len += strlen(connect->lwt_msg->topic_name);
        remain_len += MQTT_SIZE_OF_LENGTH;
        remain_len += connect->lwt_msg->payload_len;
        remain_len += MQTT_SIZE_OF_LENGTH;
    }
    if (connect->username) {
        remain_len += strlen(connect->username) + MQTT_SIZE_OF_LENGTH;
    }
    if (connect->password) {
        remain_len += strlen(connect->password) + MQTT_SIZE_OF_LENGTH;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        MQTT_PACKET_TYPE_CONNECT, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }
    tx_payload = &tx_buf[header_len];

    /* 2. Encode variable header */
    /* Set connection flags */
    if (connect->clean_session) {
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

    tx_payload += mqtt_serialize_string(tx_payload, "MQTT");
    *tx_payload++ = connect->protocol_level;
    *tx_payload++ = flags;
    tx_payload += mqtt_serialize_num(tx_payload, connect->keep_alive_sec);

    /* 3. Encode payload */
    tx_payload += mqtt_serialize_string(tx_payload, connect->client_id);
    if (connect->lwt_msg != NULL) {
        tx_payload += mqtt_serialize_string(tx_payload,
                                            connect->lwt_msg->topic_name);
        tx_payload += mqtt_serialize_num(tx_payload,
                                         connect->lwt_msg->payload_len);
        memcpy(tx_payload, connect->lwt_msg->payload, connect->lwt_msg->payload_len);
        tx_payload += connect->lwt_msg->payload_len;
    }
    if (connect->username) {
        tx_payload += mqtt_serialize_string(tx_payload, connect->username);
    } else {
        /* A Server MAY allow a Client to supply a ClientID that has a length
         * of zero bytes, however if it does so the Server MUST treat this as a
         * special case and assign a unique ClientID to that Client
         * [MQTT-3.1.3-6]. It MUST then process the CONNECT packet as if the
         * Client had provided that unique ClientID, and MUST return the
         * Assigned Client Identifier in the CONNACK packet [MQTT-3.1.3-7].
         */
        tx_payload += mqtt_serialize_num(tx_payload, (uint16_t)0);
    }
    if (connect->password) {
        tx_payload += mqtt_serialize_string(tx_payload, connect->password);
    }

    (void)tx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_connect_ack(char *rx_buf, int rx_buf_len,
    MqttConnectAck *connect_ack)
{
    int header_len, remain_len;
    char *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len, &remain_len,
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

int mqtt_serialize_publish(char *tx_buf, int tx_buf_len,
    MqttPublish *publish)
{
    int header_len = 0, variable_len = 0, payload_len = 0, remain_len = 0;
    char *tx_payload = tx_buf;

    /* Validate required arguments */
    if (tx_buf == NULL || publish == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    variable_len = strlen(publish->topic_name) + MQTT_SIZE_OF_LENGTH;
    if (publish->qos > MQTT_QOS_0) {
        if (publish->packet_id == 0) {
            return MQTT_CODE_ERROR_PACKET_ID;
        }
        variable_len += MQTT_SIZE_OF_LENGTH; /* For packet_id */
    }

    if ((publish->payload != NULL) && (publish->payload_len > 0)) {
        payload_len = publish->payload_len;
    }

    remain_len = variable_len + payload_len;

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len,
        remain_len, MQTT_PACKET_TYPE_PUBLISH,
        publish->retain, publish->qos, publish->duplicate);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }
    tx_payload = &tx_buf[header_len];

    /* 2. Encode variable header */
    tx_payload += mqtt_serialize_string(tx_payload, publish->topic_name);
    if (publish->qos > MQTT_QOS_0) {
        tx_payload += mqtt_serialize_num(tx_payload, publish->packet_id);
    }

    /* 3. Encode payload */
    if (payload_len > 0) {
        memcpy(tx_payload, publish->payload, payload_len);
    }

    /* Return length of packet placed into tx_buf */
    return header_len + remain_len;
}

int mqtt_deserialize_publish(char *rx_buf, int rx_buf_len, MqttPublish *publish)
{
    int header_len = 0, variable_len = 0, payload_len = 0, remain_len = 0;
    char *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0 || publish == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len,
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
        variable_len += mqtt_deserialize_num((uint8_t *)rx_payload, &publish->packet_id);
        rx_payload += MQTT_SIZE_OF_LENGTH;
    }

    /* Decode Payload */
    payload_len = remain_len - variable_len;
    publish->payload = rx_payload;
    publish->payload_len = payload_len;

    return header_len + remain_len;
}

int mqtt_serialize_publish_resp(char *tx_buf, int tx_buf_len, uint8_t type,
    MqttPublishResp *publish_resp)
{
    int header_len, remain_len;
    char *tx_payload;
    uint8_t qos;

    /* Validate required arguments */
    if (tx_buf == NULL || publish_resp == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = MQTT_SIZE_OF_LENGTH; /* For packet_id */

    /* Determine Qos value */
    qos = (type == MQTT_PACKET_TYPE_PUBLISH_REL) ? MQTT_QOS_1 : MQTT_QOS_0;

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        type, 0, qos, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }
    tx_payload = &tx_buf[header_len];

    /* 2. Encode variable header */
    tx_payload += mqtt_serialize_num(&tx_buf[header_len], publish_resp->packet_id);

    (void)tx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_publish_resp(char *rx_buf, int rx_buf_len, uint8_t type,
    MqttPublishResp *publish_resp)
{
    int header_len, remain_len;
    char *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len, &remain_len,
        type, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (publish_resp) {
        rx_payload += mqtt_deserialize_num((uint8_t *)rx_payload, &publish_resp->packet_id);

    }

    (void)rx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_subscribe(char *tx_buf, int tx_buf_len,
    MqttSubscribe *subscribe)
{
    int header_len, remain_len, i;
    char *tx_payload;
    MqttTopic *topic;

    /* Validate required arguments */
    if (tx_buf == NULL || subscribe == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = MQTT_SIZE_OF_LENGTH; /* For packet_id */
    for (i = 0; i < subscribe->topic_count; i++) {
        topic = &subscribe->topics[i];
        if ((topic != NULL) && (topic->topic_filter != NULL)) {
            remain_len += strlen(topic->topic_filter) + MQTT_SIZE_OF_LENGTH;
            remain_len++; /* For QoS */
        } else {
            return MQTT_CODE_ERROR_BAD_ARG;
        }
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        MQTT_PACKET_TYPE_SUBSCRIBE, 0, MQTT_QOS_1, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }
    tx_payload = &tx_buf[header_len];

    /* 2. Encode variable header */
    tx_payload += mqtt_serialize_num(&tx_buf[header_len], subscribe->packet_id);

    /* 3. Encode payload */
    for (i = 0; i < subscribe->topic_count; i++) {
        topic = &subscribe->topics[i];
        tx_payload += mqtt_serialize_string(tx_payload, topic->topic_filter);
        *tx_payload++ = topic->qos;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_subscribe_ack(char *rx_buf, int rx_buf_len,
    MqttSubscribeAck *subscribe_ack)
{
    int header_len, remain_len;
    char *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0 || subscribe_ack == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len, &remain_len,
        MQTT_PACKET_TYPE_SUBSCRIBE_ACK, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (subscribe_ack) {
        rx_payload += mqtt_deserialize_num((uint8_t *)rx_payload, &subscribe_ack->packet_id);
        subscribe_ack->return_codes = (uint8_t *)rx_payload; /* List of return codes */
        subscribe_ack->return_codes_count = remain_len - MQTT_SIZE_OF_LENGTH;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_unsubscribe(char *tx_buf, int tx_buf_len,
    MqttUnsubscribe *unsubscribe)
{
    int header_len, remain_len, i;
    char *tx_payload;
    MqttTopic *topic;

    /* Validate required arguments */
    if (tx_buf == NULL || unsubscribe == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Determine packet length */
    remain_len = MQTT_SIZE_OF_LENGTH; /* For packet_id */
    for (i = 0; i < unsubscribe->topic_count; i++) {
        topic = &unsubscribe->topics[i];
        if ((topic != NULL) && (topic->topic_filter != NULL)) {
            remain_len += strlen(topic->topic_filter) + MQTT_SIZE_OF_LENGTH;
        } else {
            return MQTT_CODE_ERROR_BAD_ARG;
        }
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        MQTT_PACKET_TYPE_UNSUBSCRIBE, 0, MQTT_QOS_1, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }
    tx_payload = &tx_buf[header_len];

    /* 2. Encode variable header */
    tx_payload += mqtt_serialize_num(&tx_buf[header_len], unsubscribe->packet_id);

    /* 3. Encode payload */
    for (i = 0; i < unsubscribe->topic_count; i++) {
        topic = &unsubscribe->topics[i];
        tx_payload += mqtt_serialize_string(tx_payload, topic->topic_filter);
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_unsubscribe_ack(char *rx_buf, int rx_buf_len,
    MqttUnsubscribeAck *unsubscribe_ack)
{
    int header_len, remain_len;
    char *rx_payload;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0 || unsubscribe_ack == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len, &remain_len,
        MQTT_PACKET_TYPE_UNSUBSCRIBE_ACK, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }
    rx_payload = &rx_buf[header_len];

    /* 2. Decode variable header */
    if (unsubscribe_ack) {
        rx_payload += mqtt_deserialize_num((uint8_t *)rx_payload, &unsubscribe_ack->packet_id);
    }

    (void)rx_payload;

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_ping(char *tx_buf, int tx_buf_len)
{
    int header_len, remain_len = 0;

    /* Validate required arguments */
    if (tx_buf == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        MQTT_PACKET_TYPE_PING_REQ, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_deserialize_ping(char *rx_buf, int rx_buf_len)
{
    int header_len, remain_len;

    /* Validate required arguments */
    if (rx_buf == NULL || rx_buf_len <= 0) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Decode fixed header */
    header_len = mqtt_deserialize_fixed_header(rx_buf, rx_buf_len, &remain_len,
        MQTT_PACKET_TYPE_PING_RESP, NULL, NULL, NULL);
    if (header_len < 0) {
        return header_len;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

int mqtt_serialize_disconnect(char *tx_buf, int tx_buf_len)
{
    int header_len, remain_len = 0;

    /* Validate required arguments */
    if (tx_buf == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* 1. Encode fixed header */
    header_len = mqtt_serialize_fixed_header(tx_buf, tx_buf_len, remain_len,
        MQTT_PACKET_TYPE_DISCONNECT, 0, 0, 0);
    if (header_len < 0) {
        return header_len;
    }
    /* Check for buffer room */
    if (tx_buf_len < header_len + remain_len) {
        return MQTT_CODE_ERROR_OUT_OF_BUFFER;
    }

    /* Return total length of packet */
    return header_len + remain_len;
}

