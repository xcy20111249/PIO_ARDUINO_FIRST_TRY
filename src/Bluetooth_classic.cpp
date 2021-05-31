#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"

#include "config_test.h"

#ifndef BLUETOOTH_CLASSIC 
#error Bluetooth classic is not enabled!
#endif

BluetoothSerial SerialBT;

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
    char* incomingmsg;
    while (!SerialBT.connected()){
        Serial.print(".");
        sleep(2);
    }
    bool done = false;
    while(!done){
        if (SerialBT.available()) {
            char incomingChar = SerialBT.read();
            while (incomingChar != '\n')
            {
                incomingmsg += incomingChar;
                incomingChar = SerialBT.read();
            }
            Serial.printf("ssid is %s", incomingmsg);
        }
        if (SerialBT.available()) {
            Serial.write(SerialBT.read());
        }
        delay(20);
    }
}