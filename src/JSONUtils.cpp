#include "JSONUtils.h"
#include <SD.h>
#include <string>
#include <fstream>
#include <sstream>

DynamicJsonDocument lectureJson (char* data){
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, data);
    return doc;
}

char* ecritureJson (DynamicJsonDocument doc){
    char* data;
    serializeJsonPretty (doc, data);
    return data;
}

// char* parseDistribJson(DynamicJsonDocument doc){

// }

char* get_test_direct(DynamicJsonDocument doc){
    JsonObject root = doc.to<JsonObject>();
    JsonVariant content = root["test_empty_topic"];
    std::string test_content = content.as<std::string>();
    const char* t = test_content.c_str();
    return t;
}

// char* get_test_function(DynamicJsonDocument doc){
//     JsonVariant test_content = doc.getMember("test_topic");
//     if (test_content.isNull()){
//         Serial.println("Error: test_topic missing");
//     }
//     return test_content;
// }