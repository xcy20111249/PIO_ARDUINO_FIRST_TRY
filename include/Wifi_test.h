#ifndef WIFI_TEST_H_
#define WIFI_TEST_H_

#include "config_test.h"

bool wifi_connect();
void wifi_disconnect();

#ifdef BLUETOOTH_CLASSIC
void set_ssid(std::string id);
void set_password(std::string code);
#endif 
#endif
