#ifndef JSONUTILS_H_
#define JSONUTILS_H_

#include "ArduinoJson.h"

DynamicJsonDocument lectureJson (char* data);
char* ecritureJson (DynamicJsonDocument doc);
char* get_test_direct(DynamicJsonDocument doc);
// char* get_test_function(DynamicJsonDocument doc);

#endif