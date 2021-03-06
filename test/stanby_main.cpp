
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"
#include "CommUtils.h"
// #include <SD.h>
#include <WiFi.h>
#include "NVSUtils.h"
#include "Wifi_test.h"

#include "config_test.h"
#include "JSONUtils.h"
#include <Preferences.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "sd_test_raw.h"
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"

#include "StatusCheck.h"

#include "EPD4in2Utils.h"
#include "displaytestUtils.h"

#define COLORED     0
#define UNCOLORED   1

bool running_flag;
long now;
std::stringstream ss;

const char* json_string;
const char* json_string_empty;
const char* json_test;
std::string id_pass[2];

TaskHandle_t xHandle_test;
TaskHandle_t xHandle_blink;
TaskHandle_t xHandle_status;
TaskHandle_t xHandle_epd;
TaskHandle_t xHandle_displaytest;

SemaphoreHandle_t test_semaphore;
SemaphoreHandle_t task_semaphore_blink;
SemaphoreHandle_t display_semaphore_epd;

EventGroupHandle_t xEventGroup_display;

static Preferences pref;

void semaphore_test(){
  while (1){
    xSemaphoreTake(test_semaphore, 2000 / portTICK_PERIOD_MS);
    Serial.print("living...");
    delay(500);
  }
}

/*
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
}

void bluetooth_restart() {
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
}

void bluetooth_terminate(){
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
}
*/

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
  pref.begin("status", false);
  pref.clear();
  pref.end();

  xEventGroup_display = xEventGroupCreate();
  display_semaphore_epd = xSemaphoreCreateBinary();
  xSemaphoreGive(display_semaphore_epd);

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
  // Bluetooth_wifi_conn(id_pass);
  // set_ssid(id_pass[0]);
  // set_password(id_pass[1]);
  // wifi_connected = wifi_connect();

  // wifi and mqtt test setup
  // wifi_connected = wifi_connect();
  // mqtt_client_my_init();
  // xTaskCreate (test, "mqtt_test", 4096, NULL, 1, &xHandle_mqtt);
  // xTaskCreate (blink, "blink", 4096, NULL, 1, &xHandle_blink);
  // task_semaphore_mqtt = xSemaphoreCreateBinary();
  // test_semaphore = xSemaphoreCreateBinary();

  //Wifi conn via bluetooth
  Task_init_bluetooth();
  TaskCreate_bluetooth();

  // epd test
  epd_init();

  // xTaskCreate(displaytest_loop, "displaytest_loop", 4096, NULL, 1, &xHandle_displaytest);
  xTaskCreate(UpdateStatus, "update_status", 4096, NULL, 1, &xHandle_status);
  xTaskCreate(EPD_loop, "epd_loop", 4096, NULL, 1, &xHandle_epd);
  // Sustainable storage test
  // preferences.begin("credentials", false);

  // ss<<preferences.getString("ssid","");
  // ss>>id_pass[0];
  // ss<<preferences.getString("password", "");
  // ss>>id_pass[1];
  // if (id_pass[0] == "" || id_pass[1] == ""){
  //   Serial.println("No values saved for ssid or password");
  // }else{
  //   WiFi.mode(WIFI_STA);
  //   WiFi.begin(id_pass[0].c_str(),id_pass[1].c_str());
  //   Serial.print("Connecting to WiFi ..");
  //   while (WiFi.status() != WL_CONNECTED) {
  //     Serial.print('.');
  //     delay(1000);
  //   }
  //   Serial.println(WiFi.localIP()); 
  // }
  // ESP.restart();

  // Preferences prefs; // ??????Preferences??????
  // prefs.begin("mynamespace"); // ??????????????????mynamespace
  // uint32_t count = prefs.getUInt("count", 0); // ???????????????????????????????????????"count"??????
  //                                             // ???????????????????????????????????????0
  // count++; // ????????????
  // Serial.printf("??????????????? %u ?????????\n", count);

  // if ((int)count>10)
  // {
  //   Serial.println("here");
  //   prefs.clear();
  //   count = 0;
  // }
  

  // prefs.putUInt("count", count); // ???????????????????????????????????????"count"??????
  // prefs.end(); // ????????????????????????

  // delay(5000);
  // ESP.restart(); // ????????????

  // NVS_TEST();
  // nvs_comm_test();

}

void loop() {
  Serial.println("loop started...");
  sleep(2);
  wifi_connect();

  // display task test


  // display test
  // pref.begin("wifi_config", false);
  // while (!WiFi.isConnected())
  // {
  //   sleep(1);
  // }

  // /**
  // * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  // * In this case, a smaller image buffer is allocated and you have to 
  // * update a partial display several times.
  // * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  // */
  // unsigned char image[1500];
  // // unsigned char image[0];
  // Paint paint(image, 400, 28);    //width should be the multiple of 8 

  // EPD_set_header(epd, "test header", milieu);
  // EPD_set_footer(epd, "test footer", milieu);

  // // paint.Clear(UNCOLORED);
  // // paint.DrawStringAt(0, 0, "e-Paper Demo", &Font16, COLORED);
  // // epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 100, 40, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(COLORED);
  // paint.DrawStringAt(100, 2, "hello world", &Font24, UNCOLORED);
  // epd.SetPartialWindow(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());
  
  // paint.SetWidth(64);
  // paint.SetHeight(64);

  // paint.Clear(UNCOLORED);
  // paint.DrawRectangle(0, 0, 40, 50, COLORED);
  // paint.DrawLine(0, 0, 40, 50, COLORED);
  // paint.DrawLine(40, 0, 0, 50, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());
  
  // paint.Clear(UNCOLORED);
  // paint.DrawCircle(25, 25, 25, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

  // epd.DisplayFrame();
  // epd.Sleep();


  //preferences test
  // preferences.begin("test1");
  // Serial.printf("from main global is %d\n", preferences.getBool("global",false));
  // preferences.end();
  // preferences.begin("test2");
  // preferences.clear();
  // Serial.printf("from main test2/global is %d\n", preferences.getBool("global"));
  // preferences.end();

  // sleep(2);
  // bluetooth_print_state();
  // bluetooth_terminate();
  
  // Serial.println("loop started......");
  // delay(2000);
  // mqtt_restart();
  // Serial.println("here");
  // delay(10000);
  // mqtt_stop();
  // Serial.println("***********************");
  // delay(10000);
  // mqtt_restart();
  // delay(5000);
  // mqtt_terminate();
  // delay(2000);
  // mqtt_restart();
  
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

