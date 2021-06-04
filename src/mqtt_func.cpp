#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Mqtt_client_my.h"
#include "WiFiClient.h"
#include "mqtt_func.h"

WiFiClient espClient;
PubSubClient mqtt_client_my(espClient);

TaskHandle_t xHandle_mqtt;
SemaphoreHandle_t task_semaphore_mqtt ;

void mqtt_set_server(char* add, int port){
  mqtt_client_my.setServer(add, port);
}

void Task_init_mqtt(){
  mqtt_client_my_init();
  task_semaphore_mqtt = xSemaphoreCreateBinary();
}

void TaskCreate_mqtt(){
  xTaskCreate(test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
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
