
#include "./app_cfg.h"
#include "./MQTTClient.h"

THIS_FILE_NAME("MQTTClient");

enum {
	CONNECT = 1,
	CONNACK,
	PUBLISH,
	PUBACK,
	PUBREC,
	PUBREL,
	PUBCOMP,
	SUBSCRIBE,
	SUBACK,
	UNSUBSCRIBE,
	UNSUBACK,
	PINGREQ,
	PINGRESP,
	DISCONNECT
};

enum {
    QOS0 = 0,
    QOS1,
    QOS2,
    SUBFAIL = 0x80,
};


static int getNextPacketId(MQTTClient *client)
{
    /* according to the MQTT specification - do not change! */
    return client->next_packetid = (client->next_packetid == 0) ? 1 : client->next_packetid + 1;
}


// assume topic filter and name is in correct format
// # can only be at end
// + and # can only be next to separator
static char isTopicMatched(const char* topicFilter, const char* topicName, uint16_t topicNameLength)
{
    const char* curf = topicFilter;
    const char* curn = topicName;
    const char* curn_end = curn + topicNameLength;

    while (*curf && curn < curn_end) {
        if (*curn == '/' && *curf != '/') {
            break;
        }
        if (*curf != '+' && *curf != '#' && *curf != *curn) {
            break;
        }
        if (*curf == '+') {   // skip until we meet the next separator, or end of string
            const char* nextpos = curn + 1;
            while (nextpos < curn_end && *nextpos != '/') {
                nextpos = ++curn + 1;
            }
        } else if (*curf == '#') {
            curn = curn_end - 1;    // skip until end of string
        }
        curf++;
        curn++;
    };

    return (curn == curn_end) && (*curf == '\0');
}


static int deliverMessage(MQTTClient *client, MqttPublish* message)
{
    int i;
    int rc = MQTT_FAILURE;

    //! we have to find the right message handler - indexed by topic
    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i) {
        if (client->messageHandlers[i].topicFilter != NULL
        && isTopicMatched(client->messageHandlers[i].topicFilter, message->topic_name, message->topic_name_len))
        {
            if (client->messageHandlers[i].fp != NULL) {
                client->messageHandlers[i].fp(message);
                rc = MQTT_SUCCESS;
            }
        }
    }

    //! handle the message by the default handler registered
    if (rc == MQTT_FAILURE && client->defaultMessageHandler != NULL) {
        client->defaultMessageHandler(message);
        rc = MQTT_SUCCESS;
    }

    return rc;
}


static void closeSession(MQTTClient *client)
{
    int i = 0;

    if (client->iscleansession) {
        for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i) {
            client->messageHandlers[i].topicFilter = NULL;
        }
    }
    client->is_ping_outstanding = 0;
    client->isconnected         = 0;
    
    client->ipstack->mqttdisconnect(client->ipstack);
}


static int sendPacket(MQTTClient *client, int length, MQTTDeadline* timer)
{
    int rc = MQTT_FAILURE;
    int sent = 0;

    while (sent < length && !MQTTDeadlineIsExpired(timer)) {
        rc = client->ipstack->mqttwrite(client->ipstack, &client->writebuf[sent],
                                   length - sent, MQTTDeadlineLeftMS(timer));
        if (rc < 0) {
            break;
        }
        sent += rc;
    }
    if (sent == length) {
        // record the fact that we have successfully sent the packet
        MQTTDeadlineSet(&client->last_sent, client->keepAliveInterval);
        rc = MQTT_SUCCESS;
    }
    return rc;
}


static int readPacket(MQTTClient* client, MQTTDeadline* timer)
{
    int i;
    int rc;
    uint32_t header_len = 0,  remain_len = 0;
    MqttHeader *header = (MqttHeader *)client->readbuf;

    /* 1. Read fix header portion */
    rc = client->ipstack->mqttread(client->ipstack, &client->readbuf[header_len],
                                   1, MQTTDeadlineLeftMS(timer));
    if (rc != 1) {
        return 0;
    }

    header_len++;

    /* 2. Read remaining length portion */
    for (i = 0; i < MQTT_PACKET_MAX_LEN_BYTES; i++) {
        /* Read byte and try decode */
        rc = client->ipstack->mqttread(client->ipstack, &client->readbuf[header_len],
                                       1, MQTTDeadlineLeftMS(timer));
        if (rc != 1) {
            return MQTT_FAILURE;
        }
        
        header_len++;

        /* Check if another byte is needed */
        if ((header->len[i] & MQTT_PACKET_LEN_ENCODE_MASK) == 0) {
            /* Variable byte length can be determined */
            break;
        }
    }
    if (i == MQTT_PACKET_MAX_LEN_BYTES) {
        return MQTT_CODE_ERROR_MALFORMED_DATA;
    }

    /* Try and decode remaining length */
    rc = mqtt_decode_remaining_length(header->len, &remain_len);
    if (rc < 0) {
        /* Indicates error */
        return rc;
    }

    /* Make sure it does not overflow rx_buf */
    if (remain_len > (client->readbuf_size - header_len - 1)) {
        return MQTT_BUFFER_OVERFLOW;
    }

    /* 3. Read remaining */
    if (remain_len > 0) {
        rc = client->ipstack->mqttread(client->ipstack, &client->readbuf[header_len],
                                       remain_len, MQTTDeadlineLeftMS(timer));
        if (rc != remain_len) {
            return MQTT_FAILURE;
        }
    }

    if (client->keepAliveInterval > 0) {
        // record the fact that we have successfully received a packet
        MQTTDeadlineSet(&client->last_received, client->keepAliveInterval);
    }

    /* Return read length */
    return header_len + remain_len;
}


static int keepalive(MQTTClient *client)
{
    int rc = MQTT_SUCCESS;
    int len = 0;
    MQTTDeadline timer;

    if (client->keepAliveInterval == 0) {
        goto exit;
    }

    //if (MQTTDeadlineIsExpired(&client->last_sent) || MQTTDeadlineIsExpired(&client->last_received)) {
    if (MQTTDeadlineIsExpired(&client->last_sent)) {
        if (client->is_ping_outstanding) {
            /* PINGRESP is not received in keepalive interval */
            RTT_LOG("MQTT keepalive timeout");
            rc = MQTT_FAILURE;
            goto exit;
        }
        MQTTDeadlineInit(&timer);
        MQTTDeadlineSetMS(&timer, 1000);
        len = mqtt_serialize_ping(client->writebuf, client->writebuf_size);
        if (len <= 0) {
            rc = MQTT_FAILURE;
            goto exit;
        }
        rc = sendPacket(client, len, &timer);
        if (rc == MQTT_SUCCESS) {
            client->is_ping_outstanding = 1;
            RTT_LOG("MQTT ping send");
        }
    }

exit:
    return rc;
}


static int cycle(MQTTClient *client, MQTTDeadline* timer)
{
    int len = 0;
    int rc = MQTT_SUCCESS;
    int packet_type = 0;

    /* read the socket, see what work is due */
    len = readPacket(client, timer);
    if (len < 0) {
        /* there is something wrong when read the socket. */
        rc = MQTT_FAILURE;
        goto exit;
    }
    if (len == 0) {
        /* there is no packet till timed out. */
        goto exit;
    }
    client->packet_len = len;

    packet_type = MQTT_PACKET_TYPE_GET(client->readbuf[0]);
    switch (packet_type) {
        case DISCONNECT:
        default:
            rc = MQTT_FAILURE;
            goto exit;

        case CONNACK:
        case PUBACK:
        case SUBACK:
        case UNSUBACK:
            break;

        case PUBLISH:
        {
            MqttPublish publish;
            MqttPublishResp pubresp;
            if (mqtt_deserialize_publish(client->readbuf, len, &publish) < 0) {
                goto exit;
            }
            deliverMessage(client, &publish);
            if (publish.qos != QOS0) {
                pubresp.packet_id = publish.packet_id;
                if (publish.qos == QOS1) {
                    len = mqtt_serialize_publish_resp(client->writebuf, client->writebuf_size, PUBACK, &pubresp);
                } else if (publish.qos == QOS2) {
                    len = mqtt_serialize_publish_resp(client->writebuf, client->writebuf_size, PUBREC, &pubresp);
                }
                if (len <= 0) {
                    rc = MQTT_FAILURE;
                } else {
                    rc = sendPacket(client, len, timer);
                }
                
                if (rc != MQTT_SUCCESS) {
                    goto exit;
                }
            }
            break;
        }
        
        case PUBREC:
        case PUBREL:
        {
            MqttPublishResp puback;

            if (mqtt_deserialize_publish_resp(client->readbuf, client->packet_len, packet_type, &puback) < 0) {
                rc = MQTT_FAILURE;
            } else if ((len = mqtt_serialize_publish_resp(client->writebuf, client->writebuf_size, (packet_type == PUBREC) ? PUBREL : PUBCOMP, &puback)) <= 0) {
                rc = MQTT_FAILURE;
            } else if ((rc = sendPacket(client, len, timer)) != MQTT_SUCCESS) {
                rc = MQTT_FAILURE;
            }

            if (rc != MQTT_SUCCESS) {
                goto exit;
            }
            break;
        }

        case PUBCOMP:
            break;

        case PINGRESP:
            client->is_ping_outstanding = 0;
            RTT_LOG("MQTT ping recv");
            break;
    }

exit:
    if (rc == MQTT_SUCCESS) {
        rc = packet_type;
    }
    return rc;
}


static int waitfor(MQTTClient *client, int packet_type, MQTTDeadline *timer)
{
    int rc = MQTT_FAILURE;

    while (!MQTTDeadlineIsExpired(timer)) {
        rc = cycle(client, timer);
        if (rc < 0) {
            // there is something wrong.
            break;
        }
        if (rc == packet_type) {
            break;
        }
        rc = keepalive(client);
        if (rc < 0) {
            break;
        }
    }

    return rc;
}


int MQTTYield(MQTTClient *client, int timeout_ms)
{
    int rc = MQTT_FAILURE;
    MQTTDeadline timer;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    if (!client->isconnected) {
        goto end;
    }

    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, timeout_ms);

	do {
        rc = cycle(client, &timer);
        if (rc < 0) {
            break;
        }
        rc = keepalive(client);
        if (rc < 0) {
            break;
        }
  	} while (!MQTTDeadlineIsExpired(&timer));

exit:
    if (rc != MQTT_SUCCESS) {
        closeSession(client);
    }
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


int MQTTConnectWithResults(MQTTClient *client, MqttConnect *options, MqttConnectAck *ack)
{
    MQTTDeadline timer;
    int rc = MQTT_FAILURE;
    int len = 0;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    /* don't send connect packet again if we are already connected */
    if (client->isconnected) {
        goto end;
    }
    
    if (MQTT_SUCCESS != client->ipstack->mqttconnect(client->ipstack)) {
        goto end;
    }

    client->keepAliveInterval = options->keep_alive_sec;
    client->iscleansession = options->clean_session;

    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, client->command_timeout_ms);

    len = mqtt_serialize_connect(client->writebuf, client->writebuf_size, options);
    if (len <= 0) {
        goto exit;
    }
    rc = sendPacket(client, len, &timer);
    if (rc != MQTT_SUCCESS) {
        goto exit;
    }

    // this will be a blocking call, wait for the connack
    if (waitfor(client, CONNACK, &timer) != CONNACK) {
        rc = MQTT_FAILURE;
        goto exit;
    }
    if (mqtt_deserialize_connect_ack(client->readbuf, client->packet_len, ack) < 0) {
        rc = MQTT_FAILURE;
        goto exit;
    }
    rc = ack->return_code;

exit:
    if (rc == MQTT_SUCCESS) {
        client->isconnected = 1;
        client->is_ping_outstanding = 0;
    } else {
        client->ipstack->mqttdisconnect(client->ipstack);
    }
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


int MQTTConnect(MQTTClient *client, MqttConnect *options)
{
    MqttConnectAck ack;
    return MQTTConnectWithResults(client, options, &ack);
}


// \note the application must close the socket after this.
int MQTTDisconnect(MQTTClient *client)
{
    MQTTDeadline timer;
    int len = 0;

#if defined(MQTT_TASK)
	MQTTMutexLock(&client->mutex);
#endif
    if (!client->isconnected) {
        goto end;
    }
    
    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, client->command_timeout_ms);

    len = mqtt_serialize_disconnect(client->writebuf, client->writebuf_size);
    if (len > 0) {
        sendPacket(client, len, &timer);
    }
    closeSession(client);
    
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return MQTT_SUCCESS;
}


int MQTTIsConnected(MQTTClient *client)
{
    return client->isconnected;
}


int MQTTSubscribeWithResults(MQTTClient *client, MqttSubscribe *subscribe,
       MqttSubscribeAck *ack)
{
    int rc = MQTT_FAILURE;
    MQTTDeadline timer;
    int len = 0;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    if (!client->isconnected) {
        goto end;
    }

    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, client->command_timeout_ms);

    subscribe->packet_id = getNextPacketId(client);
    len = mqtt_serialize_subscribe(client->writebuf, client->writebuf_size, subscribe);
    if (len <= 0) {
        goto exit;
    }
    rc = sendPacket(client, len, &timer);
    if (rc != MQTT_SUCCESS) {
        goto exit;
    }

    if (waitfor(client, SUBACK, &timer) == SUBACK) {
        if (mqtt_deserialize_subscribe_ack(client->readbuf, client->packet_len, ack) < 0) {
            rc = MQTT_FAILURE;
            goto exit;
        }
        if (subscribe->topic_count != ack->return_codes_count) {
            rc = MQTT_FAILURE;
            goto exit;
        }
        for (int i = 0; i < subscribe->topic_count; ++i) {
            subscribe->topics[i].return_code = ack->return_codes[i];
        }
    } else {
        rc = MQTT_FAILURE;
    }

exit:
    if (rc != MQTT_SUCCESS) {
        closeSession(client);
    }
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


/*  \return error code if there is any error or the return code back from the server.
 */
int MQTTSubscribe(MQTTClient *client, const char *topicFilter, uint8_t qos)
{
    int                 rc = MQTT_FAILURE;
    MqttTopic           topic = {0};
    MqttSubscribe       subscribe = {0};
    MqttSubscribeAck    ack = {0};

    topic.qos = qos;
    topic.topic_filter = topicFilter;
    subscribe.topic_count = 1;
    subscribe.topics = &topic;
    rc = MQTTSubscribeWithResults(client, &subscribe, &ack);
    if (rc < 0) {
        return rc;
    }

    return topic.return_code;
}


int MQTTUnsubscribe(MQTTClient *client, const char *topicFilter)
{
    int rc = MQTT_FAILURE;
    MQTTDeadline timer;
    int len = 0;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    if (!client->isconnected) {
        goto end;
    }

    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, client->command_timeout_ms);

    MqttUnsubscribe unscribe;
    MqttTopic topic;
    MqttUnsubscribeAck ack;
    topic.topic_filter = topicFilter;
    unscribe.topics = &topic;
    unscribe.packet_id = getNextPacketId(client);
    unscribe.topic_count = 1;
    ack.packet_id = 0;
    len = mqtt_serialize_unsubscribe(client->writebuf, client->writebuf_size, &unscribe);
    if (len <= 0) {
        goto exit;
    }
    rc = sendPacket(client, len, &timer);
    if (rc != MQTT_SUCCESS) {
        goto exit;
    }

    if (waitfor(client, UNSUBACK, &timer) == UNSUBACK) {
        if (mqtt_deserialize_unsubscribe_ack(client->readbuf, client->readbuf_size, &ack) < 0) {
            rc = MQTT_FAILURE;
            goto exit;
        }
        /* remove the subscription message handler associated with
           this topic, if there is one */
        MQTTSetMessageHandler(client, topicFilter, NULL);
    } else {
        rc = MQTT_FAILURE;
    }

exit:
    if (rc != MQTT_SUCCESS) {
        closeSession(client);
    }
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


int MQTTPublish(MQTTClient *client, MqttPublish *message)
{
    int rc = MQTT_FAILURE;
    MQTTDeadline timer;
    int len = 0;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    if (!client->isconnected) {
        goto end;
    }

    MQTTDeadlineInit(&timer);
    MQTTDeadlineSetMS(&timer, client->command_timeout_ms);

    if (message->qos == QOS1 || message->qos == QOS2) {
        message->packet_id = getNextPacketId(client);
    }

    len = mqtt_serialize_publish(client->writebuf, client->writebuf_size, message);
    if (len <= 0) {
        goto exit;
    }
    rc = sendPacket(client, len, &timer);
    if (rc != MQTT_SUCCESS) {
        goto exit;
    }

    if (message->qos == QOS1) {
        if (waitfor(client, PUBACK, &timer) == PUBACK) {
            MqttPublishResp puback;
            if (mqtt_deserialize_publish_resp(client->readbuf, client->packet_len, PUBACK, &puback) < 0) {
                rc = MQTT_FAILURE;
            }
        } else {
            rc = MQTT_FAILURE;
        }
    } else if (message->qos == QOS2) {
        if (waitfor(client, PUBCOMP, &timer) == PUBCOMP) {
            MqttPublishResp rsp = {0};
            if (mqtt_deserialize_publish_resp(client->readbuf, client->packet_len, PUBCOMP, &rsp) < 0) {
                rc = MQTT_FAILURE;
            }
        } else {
            rc = MQTT_FAILURE;
        }
    } else {
        // nothing to do.
    }

exit:
    if (rc != MQTT_SUCCESS) {
        closeSession(client);
    }
end:
#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


/* to remove a handler by setting messageHandler to NULL. */
int MQTTSetMessageHandler(MQTTClient *client, const char *topicFilter, MQTTMessageHandler messageHandler)
{
    int rc = MQTT_FAILURE;
    int i = 0;

#if defined(MQTT_TASK)
    MQTTMutexLock(&client->mutex);
#endif
    
    /* first check for an existing matching slot */
    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i) {
        if (client->messageHandlers[i].topicFilter != NULL && strcmp(client->messageHandlers[i].topicFilter, topicFilter) == 0) {
            if (messageHandler == NULL) {
                /* remove existing */
                client->messageHandlers[i].topicFilter = NULL;
                client->messageHandlers[i].fp = NULL;
            } else {
                /* modify existing */
                client->messageHandlers[i].topicFilter = topicFilter;
                client->messageHandlers[i].fp = messageHandler;
            }
            rc = MQTT_SUCCESS;
            break;
        }
    }

    /* if no existing, look for empty slot (unless we are removing) */
    if (rc != MQTT_SUCCESS) {
        if (messageHandler != NULL) {
            for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i) {
                if (client->messageHandlers[i].topicFilter == NULL) {
                    client->messageHandlers[i].topicFilter = topicFilter;
                    client->messageHandlers[i].fp = messageHandler;
                    rc = MQTT_SUCCESS;
                    break;
                }
            }
        } else {
            rc = MQTT_SUCCESS;
        }
    }

#if defined(MQTT_TASK)
    MQTTMutexUnlock(&client->mutex);
#endif
    return rc;
}


void MQTTClientInit(MQTTClient *client, MQTTNetwork *network,
        unsigned int command_timeout_ms, MQTTMessageHandler defaultMessageHandler,
		char *writebuf, size_t writebuf_size, char *readbuf, size_t readbuf_size)
{
    int i;

    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i) {
        client->messageHandlers[i].topicFilter = NULL;
    }
    client->ipstack             = network;
    client->command_timeout_ms  = command_timeout_ms;
    client->writebuf            = writebuf;
    client->writebuf_size       = writebuf_size;
    client->readbuf             = readbuf;
    client->readbuf_size        = readbuf_size;
    client->isconnected         = 0;
    client->iscleansession      = 0;
    client->is_ping_outstanding = 0;
    client->defaultMessageHandler = defaultMessageHandler;
    client->next_packetid       = 0;
    MQTTDeadlineInit(&client->last_sent);
    MQTTDeadlineInit(&client->last_received);
#if defined(MQTT_TASK)
    MQTTMutexInit(&client->mutex);
#endif
}
