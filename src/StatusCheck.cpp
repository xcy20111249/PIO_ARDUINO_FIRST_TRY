#include <Arduino.h>
#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"
#include "config_test.h"

static Preferences pref;
static bool running_flag;
// extern EventGroupHandle_t xEventGroup_display;

/*
void UpdateStatus(void *){
  running_flag = true;
  while (running_flag)
  {
    pref.begin("status", false);
    bool wifi_connection = pref.getBool("wifi_connected", false);
    Serial.print("wifi_connection is ");
    Serial.print(wifi_connection);
    Serial.println("");
    Serial.println("before wifi check");
    if (wifi_connection != WiFi.isConnected()){
      wifi_connection = WiFi.isConnected();
      Serial.println("here");
      pref.putBool("wifi_connected", !wifi_connection);
      Serial.println("here");

      pref.putBool("status_changed", true);
      Serial.println("wifi status changed");
    }
    Serial.println("before bluetooth check");
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
*/

void UpdateStatus_stop(){
  running_flag = false;
}

void UpdateStatus(void *){
  running_flag = true;
  while (running_flag)
  {
    pref.begin("status", true);
    Serial.println("checking wifi");
    bool wifi_connection = pref.getBool("wifi_connected", false);
    Serial.println("checking bluetooth");
    bool bluetooth_connection = pref.getBool("bluetooth_connected", false);
    pref.end();
    Serial.println("here");
    // Serial.print("wifi_connection is ");
    // Serial.print(wifi_connection);
    // Serial.println("");
    // Serial.println("before wifi check");
    if (wifi_connection != WiFi.isConnected()){
      wifi_connection = WiFi.isConnected();
      Serial.println("wifi is connected");
      xEventGroupSetBits(xEventGroup_display, event_wifi);
      Serial.println("flag on");
      sleep(15);
    }
    Serial.println("before bluetooth check");
    if (bluetooth_connection != get_BluetoothStatus()){
      xEventGroupSetBits(xEventGroup_display, event_bluetooth);
      // Serial.println("bluetooth status changed");
    }
    Serial.println("status check running");
    sleep(2);
    
  }
  vTaskDelete(NULL);
}
