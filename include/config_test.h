#ifndef _CONFIG_TEST_H_
#define _CONFIG_TEST_H_

#define TEST_MODE
#define MQTT_LOCAL_TEST
// #define BLE_TEST
// #define BLE_CONN_MODE
#define BLUETOOTH_CLASSIC
const int sd_chipselect = 13;

//event group sets
#include "freertos/event_groups.h"
extern EventGroupHandle_t xEventGroup_display;
#define event_wifi (1<<0)
#define event_bluetooth (1<<1)

#endif