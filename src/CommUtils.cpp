#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"
#include "Bluetooth_classic.h"
#include "BluetoothSerial.h"

extern BluetoothSerial SerialBT;
WiFiClient espClient;
PubSubClient mqtt_client_my(espClient);

TaskHandle_t xHandle_mqtt;
TaskHandle_t xHandle_BLE;
TaskHandle_t xHandle_Bluetooth;

SemaphoreHandle_t task_semaphore_mqtt ;
SemaphoreHandle_t task_semaphore_BLE;
SemaphoreHandle_t task_semaphore_bluetooth;

void Task_init_bluetooth(){
  Bluetooth_init();
  task_semaphore_bluetooth = xSemaphoreCreateBinary();
}

void TaskCreate_bluetooth(){
  xTaskCreate(Bluetooth_loop, "bluetooth loop", 4096, NULL, 1, &xHandle_Bluetooth);
}

bool mqtt_restart() {
  bool done = false;
  int retry = 0;
  while (!done && retry < 2)
  {
    int waitting = 0;
    if (xSemaphoreTake(task_semaphore_mqtt, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      while (eTaskGetState(xHandle_mqtt) != eReady && waitting < 4){
        ++waitting;
        delay(200);
      };
      if (waitting >= 4){
        Serial.println("mqtt_test delete failed...");
        retry++;
        continue;
      }
      xTaskCreate(test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
      done = true;
    }else{
      Serial.println("mqtt_test restart failed, retrying...");
      retry++;
      delay(500);
    }
  }
  if (retry >= 2){
    Serial.println("mqtt_test restart completely failed, fatal error!!");
  }else{
    Serial.println("mqtt_test is running");
  }
  return done;
}

bool mqtt_terminate(){
  bool terminated = false;
  int retry = 0;
  int waitting = 0;
  mqtt_stop();
  while (!terminated && retry < 2)
  {
    if(xSemaphoreTake(task_semaphore_mqtt, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      while (eTaskGetState(xHandle_mqtt) != eReady && waitting < 4){
        ++waitting;
        delay(50);
      }
      if (waitting >= 4){
        Serial.println("mqtt_test delete failed...");
        retry++;
        continue;
      }
      terminated = true;
      xSemaphoreGive(task_semaphore_mqtt);
    }else{
      Serial.println("mqtt_test ternimate failed, retrying...");
      retry++;
      delay(500);
      continue;
    }
  }
  if(!terminated){
    Serial.println("mqtt_test ternimate completely failed, fatal error!!");
  }else{
    Serial.println("mqtt_test terminated");
    while (mqtt_client_my.connected()){
      mqtt_client_my.disconnect();
      delay(100);
    }
    Serial.println("mqtt client disconnected.");
  }
  return terminated;
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
  Serial.println("here");
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