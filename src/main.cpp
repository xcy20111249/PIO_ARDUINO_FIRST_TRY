#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"

#define TEST_MODE

#ifdef TEST_MODE
const static int LedPin = 13;
#endif
PubSubClient mqtt_client;
xTaskHandle xHandle;

void setup() {
  Serial.begin(115200);
  Serial.println("starting setup...");
  // put your setup code here, to run once:
  pinMode(LedPin, OUTPUT);
  Serial.println("pin setted");
  vTaskDelay(2000);
  Serial.println("Setup done!");
  wifi_connect();
}

void loop() {
  reconnect();
  mqtt_client.loop();
  // put your main code here, to run repeatedly:
  xTaskCreate (blink, "blink", 4096, (void *)LedPin, 1, &xHandle);
  mqtt_client = mqtt_client_my_init();
  while (1)
  {
    sleep(5);
  }

}

