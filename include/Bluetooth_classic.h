#ifndef BLUETOOTH_CLASSIC_H_
#define BLUETOOTH_CLASSIC_H_

// #include "BluetoothSerial.h"

static const char BLUETOOTH_EVENT_OK = '0';
static const char BLUETOOTH_EVENT_WIFI_CONN = '1';
static const char BLUETOOTH_EVENT_WIFI_DISCONN = '2';
static const char BLUETOOTH_EVENT_CONF_LOCAL = '3';
static const char BLUETOOTH_EVENT_CONF_REMOTE = '4';
static const char BLUETOOTH_EVENT_WIFI_STATUS = '5';
static const char BLUETOOTH_EVENT_WIFI_CLEAR = '6';
static const char BLUETOOTH_EVENT_MQTT_START = '7';
static const char BLUETOOTH_EVENT_MQTT_END = '8';

void Bluetooth_init();
void Bluetooth_wifi_conn();
void Bluetooth_loop(void*);
static void Event_treatment(char i);
void Bluetooth_stop();
static void print_wifi_status();
bool get_BluetoothStatus();

#endif