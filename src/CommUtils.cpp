#include <Arduino.h>
#include "FreeRTOS.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"
#include "Bluetooth_classic.h"
#include "BluetoothSerial.h"
#include <Preferences.h>

extern BluetoothSerial SerialBT;

// TaskHandle_t xHandle_BLE;
TaskHandle_t xHandle_Bluetooth;

// SemaphoreHandle_t task_semaphore_BLE;
SemaphoreHandle_t task_semaphore_bluetooth;

// TaskHandle_t get_task_hanle(){
//   xTaskGet
// }

void bluetooth_print_state(){
  eTaskState taskstate = eTaskGetState(xHandle_Bluetooth);
  switch ((int)taskstate)
  {
  case 0:
    Serial.println("Running");
    break;
  case 1:
    Serial.println("Ready");
    break;
  case 2:
    Serial.println("Block");
    break;
  case 3:
    Serial.println("Suspend");
    break;
  case 4:
    Serial.println("Delete");
    break;
  default:
    Serial.println("Invalid");
    break;
  }
}

void Task_init_bluetooth(){
  Bluetooth_init();
  task_semaphore_bluetooth = xSemaphoreCreateBinary();
}

void TaskCreate_bluetooth(){
  xTaskCreate(Bluetooth_loop, "bluetooth loop", 4096, NULL, 1, &xHandle_Bluetooth);
}

bool bluetooth_restart() {
  bool done = false;
  int retry = 0;
  Serial.println("restarting bluetooth loop.");
  Bluetooth_stop();
  while (!done && retry < 2)
  {
    int waitting = 0;
    if (xSemaphoreTake(task_semaphore_bluetooth, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      while (eTaskGetState(xHandle_Bluetooth) != eReady && waitting < 4){
        ++waitting;
        delay(200);
      };
      if (waitting >= 4){
        Serial.println("bluetooth_loop delete failed...");
        retry++;
        continue;
      }
      xTaskCreate(Bluetooth_loop, "bluetooth loop", 4096, NULL, 1, &xHandle_Bluetooth);
      done = true;
    }else{
      Serial.println("bluetooth loop restart failed, retrying...");
      retry++;
      delay(500);
    }
  }
  if (retry >= 2){
    Serial.println("bluetooth loop restart completely failed, fatal error!!");
  }else{
    Serial.println("bluetooth loop is running");
  }
  return done;
}

bool bluetooth_terminate(){
  bool terminated = false;
  int retry = 0;
  int waitting = 0;
  Bluetooth_stop();
  while (!terminated && retry < 2)
  {
    if(xSemaphoreTake(task_semaphore_bluetooth, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      while (eTaskGetState(xHandle_Bluetooth) != eReady && waitting < 4){
        ++waitting;
        delay(50);
      }
      if (waitting >= 4){
        Serial.println("bluetooth_loop delete failed...");
        retry++;
        continue;
      }
      terminated = true;
      xSemaphoreGive(task_semaphore_bluetooth);
    }else{
      Serial.println("bluetooth_loop ternimate failed, retrying...");
      retry++;
      delay(500);
      continue;
    }
  }
  if(!terminated){
    Serial.println("bluetooth_lopp ternimate completely failed, fatal error!!");
  }else{
    Serial.println("bluetooth_loop terminated");
    while (SerialBT.connected()){
      SerialBT.disconnect();
      delay(100);
    }
    Serial.println("bluetooth disconnected.");
    SerialBT.end();
    Serial.println("bluetooth off.");
  }
  return terminated;
}

void nvs_comm_test(){
  Preferences pref;
  pref.begin("test1", false);
  Serial.printf("global is %d\n",pref.getBool("global", false));
  pref.end();
}