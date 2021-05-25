/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include"BluetoothLE_Client_test.h"

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
static bool BLE_running;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void BLE_Client_TEST_init() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  BLE_running = true;
}

void BLE_Client_TEST_loop(void* ) {
  // put your main code here, to run repeatedly:
  while (BLE_running){
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done!");
    pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
    delay(2000);
  }
//   Serial.println("self_killing thread BLE now...");
  vTaskDelete(NULL);
}

void BLE_Client_stop(){
    BLE_running = false;
}

bool BLE_Client_isrunning(){
    return BLE_running;
}