
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"

WiFiClient espClient;
PubSubClient mqtt_client_my(espClient);
TaskHandle_t xHandle_mqtt;
TaskHandle_t xHandle_blink;
SemaphoreHandle_t test_semaphore;

void semaphore_test(){
  while (1){
    xSemaphoreTake(test_semaphore, 2000 / portTICK_PERIOD_MS);
    Serial.print("living...");
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  wifi_connect();
  mqtt_client_my_init();
  xTaskCreate(test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
  xTaskCreate (blink, "blink", 4096, NULL, 1, &xHandle_blink);
  test_semaphore = xSemaphoreCreateBinary();
}

void loop() {
  Serial.println("loop started......");
  delay(10000);
  mqtt_stop();
  while (1)
  {
    sleep(30);
    
  }
}

