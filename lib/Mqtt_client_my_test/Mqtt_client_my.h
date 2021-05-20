#ifdef MQTT_CLIENT_MY_H_
#define MQTT_CLIENT_MY_H_
#endif

#include "PubSubClient.h"
// PubSubClient mqtt_client_my_init();
// void callback(char* topic, byte* payload, unsigned int length);
// void reconnect();

// typedef struct
// {
//     PubSubClient client_test;
// }mqtt_test_client;


extern PubSubClient client;
void callback(char* topic, uint8_t* message, unsigned int length);
void setup_wifi();
void reconnect();
void test(void *arg);
