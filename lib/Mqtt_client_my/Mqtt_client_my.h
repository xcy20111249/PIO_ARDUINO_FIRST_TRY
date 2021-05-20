#ifdef MQTT_CLIENT_MY_H_
#define MQTT_CLIENT_MY_H_
#endif

#include "PubSubClient.h"
extern PubSubClient mqtt_client;
PubSubClient mqtt_client_my_init();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();