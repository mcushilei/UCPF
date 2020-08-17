
#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__


#include "./app_cfg.h"
#include "../MQTTPacket/mqtt_packet.h"
#include "./MQTTClient_plug.h"


#if !defined(MAX_MESSAGE_HANDLERS)
#define MAX_MESSAGE_HANDLERS 5 /* redefinable - how many subscriptions do you want? */
#endif

/* all failure return codes must be negative */
enum {
    MQTT_TIMEOUT = -3,
    MQTT_BUFFER_OVERFLOW = -2,
    MQTT_FAILURE = -1,
    MQTT_SUCCESS = 0
};

typedef struct MQTTNetwork {
	void *my_socket;
    // return the number of byte has been received.
	int (*mqttread) (struct MQTTNetwork *network, char *buffer, int length, int timeout_ms);
    // return the number of byte has been sent out.
	int (*mqttwrite) (struct MQTTNetwork *network, char *buffer, int length, int timeout_ms);
    int (*mqttconnect) (struct MQTTNetwork *network);
    void (*mqttdisconnect) (struct MQTTNetwork *network);
} MQTTNetwork;


typedef void (*MQTTMessageHandler)(MqttPublish*);

typedef struct MQTTClient
{
    uint8_t     is_ping_outstanding;
    uint8_t     isconnected;
    uint8_t     iscleansession;

    uint16_t    next_packetid;

    uint32_t    command_timeout_ms;
    uint32_t    keepAliveInterval;

    size_t      packet_len;
    size_t      writebuf_size;
    size_t      readbuf_size;
    char       *writebuf;
    char       *readbuf;
    
    /* Message handlers are indexed by subscription topic */
    struct MessageHandlers {
        const char         *topicFilter;
        MQTTMessageHandler  fp;
    } messageHandlers[MAX_MESSAGE_HANDLERS];
    MQTTMessageHandler  defaultMessageHandler;

    MQTTNetwork        *ipstack;

    MQTTDeadline        last_sent;
    MQTTDeadline        last_received;

#if defined(MQTT_TASK)
    MQTTMutex           mutex;
#endif
} MQTTClient;


/** @briefInit an MQTT client object
 *  @param client - the MQTTClient structure
 *  @param network - the MQTTNetwork that this client will use.
 *  @param command_timeout_ms - the maximum time used to transceive a MQTT packet.
 *  @param defaultMessageHandler - the default message handle.
 *  @param writebuf - a MQTT packet will be serialized into this buffer.
 *  @param writebuf_size
 *  @param readbuf - the buffer used to receive a packet from the network.
 *  @param readbuf_size
 */
extern void MQTTClientInit(MQTTClient *client, MQTTNetwork *network,
        unsigned int command_timeout_ms, MQTTMessageHandler defaultMessageHandler,
		char *writebuf, size_t writebuf_size, char *readbuf, size_t readbuf_size);

/** @brief set or remove a per topic message handler
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter set the message handler for
 *  @param messageHandler - pointer to the message handler function or NULL to remove
 *  @return success code
 */
extern int MQTTSetMessageHandler(MQTTClient *client, const char *topicFilter, MQTTMessageHandler messageHandler);

/** @brief send an MQTT connect packet down the network and wait for a Connack
 *  The nework object must be connected to the network endpoint before calling this
 *  @param options - connect options
 *  @ack - the acknowledge returned
 *  @return success code
 */
extern int MQTTConnectWithResults(MQTTClient *client, MqttConnect *options, MqttConnectAck *ack);

/** @brief send an MQTT connect packet down the network and wait for a Connack
 *  The nework object must be connected to the network endpoint before calling this
 *  @param options - connect options
 *  @return success code
 */
extern int MQTTConnect(MQTTClient *client, MqttConnect *options);

/** @brief send an MQTT disconnect packet and close the connection
 *  @param client - the client object to use
 *  @return success code
 */
extern int MQTTDisconnect(MQTTClient *client);

/** @brief
 *  @param client - the client object to use
 *  @return truth value indicating whether the client is connected to the server
 */
extern int MQTTIsConnected(MQTTClient *client);

/** @brief send an MQTT subscribe packet and wait for suback before returning.
 *  @param client - the client object to use
 *  @param subscribe - the topic filter to subscribe to
 *  @param ack - suback granted QoS returned
 *  @return success code
 */
extern int MQTTSubscribeWithResults(MQTTClient *client, MqttSubscribe *subscribe, MqttSubscribeAck *ack);

/** @brief send an MQTT subscribe packet and wait for suback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to subscribe to
 *  @return success code or QoS granted if success.
 */
extern int MQTTSubscribe(MQTTClient *client, const char *topicFilter, uint8_t qos);

/** @brief send an MQTT unsubscribe packet and wait for unsuback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to unsubscribe from
 *  @return success code
 */
extern int MQTTUnsubscribe(MQTTClient *client, const char *topicFilter);

/** @brief send an MQTT publish packet and wait for all acks to complete for all QoSs
 *  @param client - the client object to use
 *  @param publish - the message to send
 *  @return success code
 */
extern int MQTTPublish(MQTTClient *client, MqttPublish *publish);

/** @brief shall be called when there is no ongoing message transfer.
 *  @param client - the client object to use
 *  @param timeout_ms - the time, in milliseconds, to yield for
 *  @return success code
 */
extern int MQTTYield(MQTTClient *client, int timeout_ms);



#endif
