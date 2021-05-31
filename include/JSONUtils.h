#ifndef JSONUTILS_H_
#define JSONUTILS_H_

#include "ArduinoJson.h"

DynamicJsonDocument lectureJson (const char* data);
String ecritureJson (DynamicJsonDocument doc);
const char* get_any_member (DynamicJsonDocument doc, char* key);
const char* get_test_direct(DynamicJsonDocument doc);
const char* get_test_function(DynamicJsonDocument doc);

#endif  