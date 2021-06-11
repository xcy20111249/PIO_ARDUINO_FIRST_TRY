#include <Arduino.h>
#include "Preferences.h"
#include "WiFi.h"
#include "BluetoothSerial.h"
#include "Bluetooth_classic.h"
#include "config_test.h"

static Preferences pref;
static bool running_flag;
// extern EventGroupHandle_t xEventGroup_display;

void UpdateStatus_stop(){
  running_flag = false;
}

void UpdateStatus(void *){
  running_flag = true;
  while (running_flag)
  {
    xSemaphoreTake(display_semaphore_epd, portMAX_DELAY);
    Serial.println("[StatusCheck]status check running");
    pref.begin("status", true);
    // Serial.println("[StatusCheck]checking wifi");
    bool wifi_connection = pref.getBool("wifi_connected", false);
    // Serial.println("[StatusCheck]checking bluetooth");
    bool bluetooth_connection = pref.getBool("BT_connected", false);
    pref.end();
    xSemaphoreGive(display_semaphore_epd);
    // Serial.println("[StatusCheck]sending event");
    // Serial.print("wifi_connection is ");
    // Serial.print(wifi_connection);
    // Serial.println("");
    // Serial.println("before wifi check");
    if (wifi_connection != WiFi.isConnected()){
      wifi_connection = WiFi.isConnected();
      // Serial.println("wifi is connected");
      xEventGroupSetBits(xEventGroup_display, event_wifi);
      // Serial.println("flag on");
      // sleep(15);
    }
    Serial.printf("BT is %d\n", bluetooth_connection);
    Serial.printf("get BT is %d\n", get_BluetoothStatus());
    if (bluetooth_connection != get_BluetoothStatus()){
      xEventGroupSetBits(xEventGroup_display, event_bluetooth);
      Serial.println("bluetooth status changed");
    }
    // Serial.println("[StatusCheck]status check done");
    // int stack_left = uxTaskGetStackHighWaterMark(NULL);
    // Serial.printf("[StatusCheck]stack left is %dB\n", stack_left);
    sleep(2);
  }
  vTaskDelete(NULL);
}
