#ifndef _CONFIG_TEST_H_
#define _CONFIG_TEST_H_

#define TEST_MODE
#define MQTT_LOCAL_TEST
// #define BLE_TEST
// #define BLE_CONN_MODE
#define BLUETOOTH_CLASSIC
const int sd_chipselect = 13;

#include <Arduino.h>

//event group sets
#include "freertos/event_groups.h"
extern EventGroupHandle_t xEventGroup_display;
#define event_wifi (1<<1)
#define event_bluetooth (1<<2)
#define event_test (1<<3)

extern SemaphoreHandle_t display_semaphore_epd;

#endif