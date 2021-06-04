#include <Arduino.h>
#include "WiFi.h"
#include "esp_event.h"
#include "PubSubClient.h"
#include "Wifi_test.h"
#include <Preferences.h>

// #define BEENEITC_LOCAL_MODE
#if defined HOTPOINT_MODE
#define ssid "HLK-L41%0589%CloudClone"
#define password "53026446"
#elif defined BEENEITC_LOCAL_MODE
#define ssid "BEENETIC_FIBRE"
#define password "CairZigWatt2010"
#elif defined BLE_CONN_MODE
char* ssid;
char* password;

void set_ssid(char* id){
    ssid = id;
}
void set_password(char* code){
    password = code;
}
#elif defined BLUETOOTH_CLASSIC
// char* ssid;
// char* password;
// void set_ssid(std::string id){
//     int len = id.length();
//     ssid = new char[len+1];
//     strcpy(ssid, id.c_str());
// }
// void set_password(std::string code){
//     int len = code.length();
//     password = new char[len+1];
//     strcpy(password, code.c_str());
// }
#endif

static Preferences pref;

// try to connect to wifi with given configuration, if unsuccess in 10 sec, return false
bool wifi_connect(){
    pref.begin("wifi_config");
    if (pref.getBool("wifi_setup", false))
    {
        WiFi.begin(pref.getString("ssid").c_str(), pref.getString("password").c_str());
        Serial.printf("Connecting to %s\n", pref.getString("ssid").c_str());
        long now = millis();
        long conn_start_time = now;
        while (WiFi.status()!=WL_CONNECTED)
        {
            vTaskDelay(500);
            Serial.print(".");
            now = millis();
            if(now-conn_start_time>10000){
                Serial.println("WiFi connection time out, please try again later...");
                return false;
            }
        }
        Serial.printf("local IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("host IP: %s\n",WiFi.gatewayIP().toString().c_str());
        return true;
    }else{
        Serial.println("Please setup wifi configuration via bluetooth.");
        return false;
    }
    

}

void wifi_disconnect(){
    Serial.println("Wifi disconnecting...");
    WiFi.disconnect();
    if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("Wifi not even started.");
    }else{
        while (WiFi.status()!=WL_DISCONNECTED)
        {
            vTaskDelay(500);
            Serial.print(".");
        }
        Serial.println("Wifi disconnected.");
    }
}

bool wifi_reconnect(){
    Serial.println("Wifi reconnecting...");
    wifi_disconnect();
    return wifi_connect();
}