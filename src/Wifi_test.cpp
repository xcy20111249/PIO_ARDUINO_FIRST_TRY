#include <Arduino.h>
#include "WiFi.h"
#include "esp_event.h"
#include "PubSubClient.h"
#include "Wifi_test.h"

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
char* ssid;
char* password;
void set_ssid(std::string id){
    int len = id.length();
    ssid = new char[len+1];
    strcpy(ssid, id.c_str());
}
void set_password(std::string code){
    int len = code.length();
    password = new char[len+1];
    strcpy(password, code.c_str());
}
#endif

void wifi_connect(){
    WiFi.begin(ssid, password);
    Serial.printf("Connecting to %s\n", ssid);
    while (WiFi.status()!=WL_CONNECTED)
    {
        vTaskDelay(1000);
        Serial.print(".");
    }
    Serial.printf("local IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("host IP: %s\n",WiFi.gatewayIP().toString().c_str());
}