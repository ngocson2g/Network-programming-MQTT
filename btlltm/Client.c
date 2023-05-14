#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClient"
#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	/*
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    */

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", TOPIC, CLIENTID, QOS);

    MQTTClient_subscribe(client, TOPIC, QOS);

    char message[100];
    int i = 0;
    while(1) {
        sprintf(message, "Message #%d", ++i);
        MQTTClient_deliveryToken token;
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = message;
        pubmsg.payloadlen = strlen(message);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               (int)(TIMEOUT/1000), message, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
        sleep(1);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
