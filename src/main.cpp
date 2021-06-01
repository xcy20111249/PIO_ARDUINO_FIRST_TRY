
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"
#include "Wifi_test.h"
#include "WiFi.h"
#include "Mqtt_client_my.h"
#include "PubSubClient.h"
#include "BluetoothLE_Client_test.h"
#include "BluetoothLE_Server_test.h"
#include "Bluetooth_classic.h"
// #include <SD.h>

#include "config_test.h"
#include "JSONUtils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "sd_test_raw.h"

WiFiClient espClient;
PubSubClient mqtt_client_my(espClient);
TaskHandle_t xHandle_mqtt;
TaskHandle_t xHandle_blink;
TaskHandle_t xHandle_BLE;
TaskHandle_t xHandle_Bluetooth;
TaskHandle_t xHandle_test;

SemaphoreHandle_t task_semaphore_blink;
SemaphoreHandle_t task_semaphore_mqtt ;
SemaphoreHandle_t retask_semaphore_mqtt ;
SemaphoreHandle_t task_semaphore_BLE;
SemaphoreHandle_t test_semaphore;

bool running_flag;
long now;
static bool wifi_connected;

const char* json_string;
const char* json_string_empty;
const char* json_test;
std::string id_pass[2];

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
}

void mqtt_terminate(){
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
    }
    if(retry >= 2){
      Serial.println("mqtt_test ternimate completely failed, fatal error!!");
    }else{
      Serial.println("mqtt_test terminated");
    }
  }
  if (terminated){
    while (mqtt_client_my.connected()){
      mqtt_client_my.disconnect();
      delay(100);
    }
    Serial.println("mqtt client disconnected.");
  }
  

}

void task_test(void *){
  delay(1000);
  long lasttm = 0;
  while (running_flag){
    now = millis();
    if (now-lasttm > 1000){
      lasttm = now;
      Serial.println("living...");
    }
    delay(50);
  }
  Serial.println("ending");
  vTaskDelete(NULL);
}

void task_stop(){
  running_flag = false;
}

void print_state(TaskHandle_t xHandle){
  eTaskState taskstate = eTaskGetState(xHandle);
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

void sd_test(char* path){
  std::fstream json_stream;
  json_stream.open(path, std::ios::in | std::ios::out);
  std::string contest = "writing test";
  json_stream.write(contest.c_str(), contest.length());
  json_stream << "test" << std::endl;
  json_stream.close();
}
void sd_test_read(char* path){
  char data[128];
  std::fstream json_stream;
  json_stream.open(path, std::ios::in);
  json_stream >> data;
  json_stream.close();
  Serial.print(data);

}

void setup() {
  Serial.begin(115200);

  //BLE and WiFi test setup
  /*
  task_semaphore_mqtt = xSemaphoreCreateBinary();
  xSemaphoreGive(task_semaphore_mqtt);
  test_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(test_semaphore);
  retask_semaphore_mqtt = xSemaphoreCreateBinary();

  BLE_Server_test_init();

  wifi_connect();
  mqtt_client_my_init();
  xTaskCreate (test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
  xTaskCreate (blink, "blink", 4096, NULL, 1, &xHandle_blink);
  // xTaskCreate (BLE_Client_TEST_loop, "ble_test", 4096, NULL, 1, &xHandle_BLE);
  running_flag = true;
  Serial.println("setup done");
  */

  //json test setup
  /*
  json_string = "{\"hello\":\"world\"}";
  Serial.println(json_string);
  json_test="{\"ID\": \"xcy\",\"password\": \"12345\",\"things\":[\"thing1\",\"thing2\"],\"else\":{\"anyID\": \"xxx\",\"anypassword\": \"12345\",}}";
  */

  //Bluetooth classic test setup
  // Bluetooth_init();
  // Bluetooth_test(id_pass);
  // set_ssid(id_pass[0]);
  // set_password(id_pass[1]);

  // wifi and mqtt test setup
  wifi_connected = wifi_connect();
  mqtt_client_my_init();
  xTaskCreate (test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
  xTaskCreate (blink, "blink", 4096, NULL, 1, &xHandle_blink);
  task_semaphore_mqtt = xSemaphoreCreateBinary();
  xSemaphoreGive(task_semaphore_mqtt);
  test_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(test_semaphore);

}

void loop() {
  
  Serial.println("loop started......");
  delay(2000);
  mqtt_restart();
  Serial.println("here");
  delay(10000);
  mqtt_stop();
  Serial.println("***********************");
  delay(10000);
  mqtt_restart();
  delay(5000);
  mqtt_terminate();
  delay(2000);
  mqtt_restart();
  
  // xTaskCreate (task_test, "test", 4096, NULL, 1, &xHandle_test);
  // Serial.println("task created");
  // print_state(xHandle_test);
  // delay(5000);
  // print_state(xHandle_test);
  // delay(5000);
  // task_stop();
  // delay(200);
  // print_state(xHandle_test);

  // DynamicJsonDocument test_doc = lectureJson(json_test);
  // const char* test_data = get_any_member(test_doc, (char*)"anyID");
  // Serial.println(test_data);
  // const char* data = get_test_direct(test_doc);
  // Serial.println(data);
  // Serial.println("************************");
  // String changed = ecritureJson(test_doc);
  // Serial.println(changed);
  // test_doc["ID"] = 12345;
  // changed = ecritureJson(test_doc);
  // Serial.println(changed);
  // test_doc["ID"] = 45678;
  // changed = ecritureJson(test_doc);
  // Serial.println(changed);
  // const char* func_data = get_test_function(test_doc);
  // Serial.println(func_data);
  // sleep (3);
  // wifi_disconnect();
  while (1)
  {
    sleep(30); 
  }
}

