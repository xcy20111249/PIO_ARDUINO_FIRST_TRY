#include <Arduino.h>
#include <sstream>
#include "WiFi.h"
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"

#include "Wifi_test.h"

#include "config_test.h"

#ifndef BLUETOOTH_CLASSIC 
#error Bluetooth classic is not enabled!
#endif

BluetoothSerial SerialBT;
static const char START_FLAG = '$';
static const char END_FLAG = '#';
static bool running_flag;

void Bluetooth_init(){
    SerialBT.begin("ESP32TEST_X");
    Serial.println("Device started, try to pair");
    running_flag = true;
}

/*
void Bluetooth_loop(void *){
    while (!SerialBT.connected()){
        Serial.print(".");
        sleep(2);
    }
    while(1){
        if (Serial.available()) {
            SerialBT.write(Serial.read());
        }
        if (SerialBT.available()) {
            Serial.write(SerialBT.read());
        }
        delay(20);
    }
}
*/

void Bluetooth_wifi_conn(std::string id_pass[2]){
    std::string incomingmsg;
    std::stringstream incomingstream;
    while (!SerialBT.connected()){
        Serial.print(".");
        sleep(2);
    }
    bool done = false;
    SerialBT.println("Which network do you want to connect?");
    Serial.println("Which network do you want to connect?");
    while(!done){
        if (SerialBT.available()) {
            char incomingChar = SerialBT.read();
            while (incomingChar != START_FLAG)
            {
                incomingChar = SerialBT.read();
            }
            incomingChar = SerialBT.read();
            while (incomingChar != END_FLAG)
            {  
                incomingstream<<incomingChar;
                incomingChar = SerialBT.read();
            }
            incomingmsg = incomingstream.str();
            Serial.printf("ssid is %s\n", incomingmsg.c_str());
            id_pass[0] = incomingmsg;
            done = true;
        }else{
            delay(100);
        }
    }
    incomingmsg = "";
    incomingstream.str("");
    done = false;
    SerialBT.println("What's the password?");
    while (!done){
        if (SerialBT.available()) {
            char incomingChar = SerialBT.read();
            while (incomingChar != START_FLAG)
            {
                incomingChar = SerialBT.read();
            }
            incomingChar = SerialBT.read();
            while (incomingChar != END_FLAG)
            {
                incomingstream<<incomingChar;
                incomingChar = SerialBT.read();
            }
            Serial.print("");
            incomingmsg = incomingstream.str();
            Serial.printf("password is %s\n", incomingmsg.c_str());
            id_pass[1] = incomingmsg;
            done = true;
        }else{
            delay(100);
        }
    }
}

//constantly running while bluetooth is active and treat incoming message
void Bluetooth_loop(void *){
    while (running_flag){
        std::string incomingmsg;
        std::stringstream incomingstream;
        while (!SerialBT.connected())
        {
            sleep(1);
        }
        while (!SerialBT.available())
        {
            vTaskDelay(500);
        }
        char incomingChar = SerialBT.read();
        while (incomingChar != START_FLAG){
            incomingChar = SerialBT.read();
        }
        incomingChar = SerialBT.read();
        while (incomingChar != END_FLAG)
        {
            incomingstream<<incomingChar;
            incomingChar = SerialBT.read();
        }
        incomingmsg = incomingstream.str();
        Serial.printf("Incoming message is %s\n", incomingmsg.c_str());
        if (incomingmsg.length() == 1){
            Event_treatment(incomingmsg.c_str()[0]);
        }else{
            SerialBT.printf("Your request %s is unexpected.\n", incomingmsg.c_str());
            Serial.printf("Client asked for %s, unable to achieve.\n", incomingmsg.c_str());
        }
        sleep(1);
    } 
    vTaskDelete(NULL);  
}

static void Event_treatment(char i){
    switch (i)
    {
    case BLUETOOTH_EVENT_OK:
        Serial.println("Bluetooth event OK.");
        break;
    case BLUETOOTH_EVENT_WIFI_CONN:
        Serial.println("Bluetooth event wifi connection.");
        Bluetooth_wifi_conn(id_pass);
        set_ssid(id_pass[0]);
        set_password(id_pass[1]);
        wifi_connect();
        break;
    case BLUETOOTH_EVENT_WIFI_DISCONN:
        Serial.println("Bluetooth event wifi disconnection.");
        wifi_disconnect();
        break;
    case BLUETOOTH_EVENT_WIFI_STATUS:
        Serial.println("Bluetooth event wifi status.");
        Serial.print("wifi status is ");
        Serial.print(WiFi.status());
        Serial.println("");
        break;
    default:
        Serial.println("Unexpected info..");
        break;
    }
}

void Bluetooth_stop(){
    running_flag = false;
}