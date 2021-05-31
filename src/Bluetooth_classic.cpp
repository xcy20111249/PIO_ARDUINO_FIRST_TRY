#include <Arduino.h>
#include <sstream>
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"

#include "config_test.h"

#ifndef BLUETOOTH_CLASSIC 
#error Bluetooth classic is not enabled!
#endif

BluetoothSerial SerialBT;
char START_FLAG = '$';
char END_FLAG = '#';

void Bluetooth_init(){
    SerialBT.begin("ESP32TEST_X");
    Serial.println("Device started, try to pair");
}

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

void Bluetooth_test(std::string id_pass[2]){
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
            Serial.print(".");
        }
    }
    incomingmsg = "";
    incomingstream.str("");
    done = false;
    SerialBT.println("What's the password?");
    while (!done){
        if (SerialBT.available()) {
            char incomingChar = SerialBT.read();
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