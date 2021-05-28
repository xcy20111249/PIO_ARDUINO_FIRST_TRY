#include "JSONUtils.h"
#include <SD.h>
#include <string>
#include <fstream>
#include <sstream>

// static void deserializeerrorprint(DeserializationError err){
//     switch ((int)err)
//     {
//     case 0:
//         Serial.println("OK");
//         break;
    
//     default:
//         break;
//     }
// }

DynamicJsonDocument lectureJson (const char* data){
    DynamicJsonDocument doc(128);
    DeserializationError err = deserializeJson(doc, data);
    return doc;
}

String ecritureJson (DynamicJsonDocument doc){
    String data;
    serializeJsonPretty (doc, data);
    return data;
}

// char* parseDistribJson(DynamicJsonDocument doc){

// }

const char* get_test_direct(DynamicJsonDocument doc){
    const char* content = doc["test_empty_topic"];
    return content;
}

const char* get_test_function(DynamicJsonDocument doc){
    JsonVariant test_content = doc.getMember("test_topic");
    if (test_content.isNull()){
        Serial.println("Error: test_topic missing");
    }
    return test_content;
}