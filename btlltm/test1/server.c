#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleServer"
#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     1000L

int main()
{
	MQTTClient client;
    	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    	MQTTClient_message pubmsg = MQTTClient_message_initializer;
    	MQTTClient_deliveryToken token;
    	int rc;
    	
    	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    	
    	conn_opts.keepAliveInterval = 20;
    	conn_opts.cleansession = 1;
    	
    	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    	{
    		printf("%d\n", MQTTCLIENT_SUCCESS);
    		
        	printf("Failed to connect, return code %d\n", rc);
        	exit(-1);
    	}
    	
    	printf("Connected to MQTT broker at %s\n", ADDRESS);
    	
	while(1){
    		MQTTClient_subscribe(client, TOPIC, QOS);
    		
    	}
    	
    	MQTTClient_disconnect(client, 10000);
    	MQTTClient_destroy(&client);
    	return 0;
}
