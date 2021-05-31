#include "JSONUtils.h"
#include <SD.h>
#include <string>
#include <fstream>
#include <sstream>

DynamicJsonDocument lectureJson (const char* data){
    DynamicJsonDocument doc(2048);
    DeserializationError err = deserializeJson(doc, data);
    Serial.println(err.c_str());
    return doc;
}

String ecritureJson (DynamicJsonDocument doc){
    String data;
    serializeJsonPretty (doc, data);
    return data;
}

char* parseDistribJson(DynamicJsonDocument doc){
    std::stringstream data_stream;
    data_stream<<doc["Dist_ID"]<<"//"<<doc["Patient_ID"]<<"//"<<doc["Power"]<<"//"<<doc["Language"]<<"//"
    <<doc["NW_level"]<<"//"<<doc["Tim"]<<"//"<<doc["MNT"]["tx_cpt"]<<"//"<<doc["MNT"]["rx_cpt"];
    char* data; 
    data_stream>>data;
    return data;
}

const char* get_any_member (DynamicJsonDocument doc, char* key){
    JsonVariant test_content = doc.getMember(key );
    if (test_content.isNull()){
        Serial.printf("Error: %s missing", key);
    }
    return test_content;

}

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