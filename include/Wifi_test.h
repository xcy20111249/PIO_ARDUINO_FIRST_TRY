#ifndef WIFI_TEST_H_
#define WIFI_TEST_H_

#include "config_test.h"

bool wifi_connect();
void wifi_disconnect();

void set_ssid(std::string id);
void set_password(std::string code);
bool wifi_clear();

#endif
