#ifndef MQTT_CLIENT_MY_H_
#define MQTT_CLIENT_MY_H_


#include "PubSubClient.h"

#include "config_test.h"

extern PubSubClient mqtt_client_my;
extern SemaphoreHandle_t task_semaphore_mqtt;

#ifdef MQTT_LOCAL_TEST
static const char* mqtt_server = "192.168.1.88";
static String Distributeur_id = "1001";
#endif

static const char* TAG = "Mqtt_client_my";
static String topicSubscribe_all;
static String topicSubscribe_User;
static String topicSubscribe_Product;
static String topicSubscribe_Hw;
static String topicSubscribe_App;
static String topicSubscribe_Mtime;

void mqtt_client_my_init();
static void on_connect_subscribe();
static void callback(char* topic, uint8_t* message, unsigned int length);
static void reconnect();
void mqtt_stop();
void test(void *arg);

#endif