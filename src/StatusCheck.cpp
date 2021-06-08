#include <Arduino.h>
#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"

static Preferences pref;
static bool running_flag;

void UpdateStatus(void *){
  running_flag = true;
  while (running_flag)
  {
    pref.begin("status", false);
    bool wifi_connection = pref.getBool("wifi_connected", false);
    if (wifi_connection != WiFi.isConnected()){
      pref.putBool("wifi_connected", !wifi_connection);
      Serial.println("here");

      pref.putBool("status_changed", true);
      Serial.println("wifi status changed");
    }
    bool bluetooth_connection = pref.getBool("bluetooth_connected", false);
    if (bluetooth_connection != get_BluetoothStatus()){
      pref.putBool("bluetooth_connected", get_BluetoothStatus());
      pref.putBool("status_changed", true);
      Serial.println("bluetooth status changed");
    }
    pref.end();
    Serial.println("status check running");
    sleep(2);
  }
  vTaskDelete(NULL);
}

void UpdateStatus_stop(){
  running_flag = false;
}