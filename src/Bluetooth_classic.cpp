#include <Arduino.h>
#include <sstream>
#include "WiFi.h"
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"
#include "mqtt_func.h"

#include "Wifi_test.h"

#include "config_test.h"

#ifndef BLUETOOTH_CLASSIC 
#error Bluetooth classic is not enabled!
#endif

static const char START_FLAG = '$';
static const char END_FLAG = '#';
static bool running_flag;
BluetoothSerial SerialBT;
extern SemaphoreHandle_t task_semaphore_bluetooth; 

void Bluetooth_init(){
    SerialBT.begin("ESP32TEST_X");
    Serial.println("Device started, try to pair");
    running_flag = true;
}

/*
void Bluetooth_print_loop(void *){
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
    running_flag = true;
    while (running_flag){
        std::string incomingmsg;
        std::stringstream incomingstream;
        while (!SerialBT.connected() && running_flag)
        {
            sleep(1);
        }
        while (!SerialBT.available() && running_flag)
        {
            vTaskDelay(500);
            continue;
        }
        char incomingChar = SerialBT.read();
        int i = 0;
        while (incomingChar != START_FLAG && i<10){
            incomingChar = SerialBT.read();
            ++i;
        }
        if (i>=10){
            Serial.println("Start flag not detected, restart reading..");
            continue;
        }
        incomingChar = SerialBT.read();
        i = 0;
        while (incomingChar != END_FLAG && i<10)
        {
            // Serial.print((int)incomingChar);
            incomingstream<<incomingChar;
            incomingChar = SerialBT.read();
            ++i;
        }
        if(i>=10){
            Serial.printf("Receive extra-long messages: %s\nignored\n", incomingstream.str().c_str());
            continue;
        }
        incomingmsg = incomingstream.str();
        Serial.printf("Incoming message is %s\n", incomingmsg.c_str());
        if (incomingmsg.length() == 1){
            Event_treatment(incomingmsg.c_str()[0]);
        }else{
            SerialBT.printf("Your request %s is unexpected.\n", incomingmsg.c_str());
            Serial.printf("Client asked for %s, unable to achieve.\n", incomingmsg.c_str());
        }
    } 
    xSemaphoreGive(task_semaphore_bluetooth);
    Serial.println("Ending bluetooth comm...");
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
        print_wifi_status();
        Serial.println("");
        break;
    case BLUETOOTH_EVENT_MQTT_START:
        if (!WiFi.isConnected()){
            Serial.println("No network service, please connect to internet...");
            break;
        }
        Task_init_mqtt();
        TaskCreate_mqtt();
    case BLUETOOTH_EVENT_MQTT_END:
        
        break;
    default:
        Serial.println("Unexpected info..");
        break;
    }
}

void Bluetooth_stop(){
    running_flag = false;
}

static void print_wifi_status(){
    switch (WiFi.status())
    {
    case 255:
        Serial.print("WL_NO_SHIELD");
        break;
    case 0:
        Serial.print("WL_IDLE_STATUS");
        break;
    case 1:
        Serial.print("WL_NO_SSID_AVAIL");
        break;
    case 2:
        Serial.print("WL_SCAN_COMPLETED");
        break;
    case 3:
        Serial.print("WL_CONNECTED");
        break;
    case 4:
        Serial.print("WL_CONNECT_FAILED");
        break;
    case 5:
        Serial.print("WL_CONNECTION_LOST");
        break;
    case 6:
        Serial.print("WL_DISCONNECTED");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
}

