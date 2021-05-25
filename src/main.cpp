
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"
#include "BluetoothLE_Client_test.h"
#include "BluetoothLE_Server_test.h"

#include "config_test.h"

#include "BluetoothSerial.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEUUID.h"

WiFiClient espClient;
PubSubClient mqtt_client_my(espClient);
TaskHandle_t xHandle_mqtt;
TaskHandle_t xHandle_blink;
TaskHandle_t xHandle_BLE;
SemaphoreHandle_t task_semaphore_blink;
SemaphoreHandle_t task_semaphore_mqtt ;
SemaphoreHandle_t retask_semaphore_mqtt ;
SemaphoreHandle_t task_semaphore_BLE;
SemaphoreHandle_t test_semaphore;

void semaphore_test(){
  while (1){
    xSemaphoreTake(test_semaphore, 2000 / portTICK_PERIOD_MS);
    Serial.print("living...");
    delay(500);
  }
}

void mqtt_restart() {
  bool done = false;
  int retry = 0;
  while (!done && retry < 2)
  {
    if (xSemaphoreTake(task_semaphore_mqtt, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      xSemaphoreGive(retask_semaphore_mqtt);
      delay(200);
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
}

void mqtt_terminate(){
  bool terminated = false;
  int retry = 0;
  mqtt_stop();
  while (!terminated && retry < 2)
  {
    if(xSemaphoreTake(task_semaphore_mqtt, 2000 / portTICK_PERIOD_MS)==pdTRUE){
      vTaskDelete(xHandle_mqtt);
      terminated = true;
      xSemaphoreGive(task_semaphore_mqtt);
    }else{
      Serial.println("mqtt_test ternimate failed, retrying...");
      retry++;
      delay(500);
    }
    if(retry >= 2){
      Serial.println("mqtt_test ternimate completely failed, fatal error!!");
    }else{
      Serial.println("mqtt_test terminated");
    }
  }
  

}

void resemaphore_test(){
  if (xSemaphoreGive(retask_semaphore_mqtt)==pdTRUE){
    Serial.println("it can");
  }else{
    Serial.println("it cannot");
  }
}

void setup() {
  Serial.begin(115200);
  task_semaphore_mqtt = xSemaphoreCreateBinary();
  xSemaphoreGive(task_semaphore_mqtt);
  test_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(test_semaphore);
  retask_semaphore_mqtt = xSemaphoreCreateBinary();

  // BLE_Server_test_init();

  wifi_connect();
  mqtt_client_my_init();
  mqtt_restart();
  xTaskCreate (blink, "blink", 4096, NULL, 1, &xHandle_blink);
  xTaskCreate (BLE_Client_TEST_loop, "ble_test", 4096, NULL, 1, &xHandle_BLE);

  resemaphore_test();
}

void loop() {
  Serial.println("loop started......");
  delay(2000);
  mqtt_restart();
  delay(10000);
  mqtt_stop();
  Serial.println("***********************");
  delay(10000);
  mqtt_restart();
  while (1)
  {
    sleep(30);
    
  }
}

