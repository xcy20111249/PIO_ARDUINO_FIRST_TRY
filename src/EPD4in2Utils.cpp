
#include "EPD4in2Utils.h"
#include "Preferences.h"
#include "image_imeds.h"
#include "imageR6.h"
#include "image_wifi.h"
#include "image_bluetooth.h"

#define COLORED     0
#define UNCOLORED   1

static Preferences pref;
static bool running_flag;
static Epd epd;

void epd_init(){
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrame();
  Serial.println("printing logo");
  epd.DisplayFrame(IMAGE_imeds);
  epd.ClearFrame();
  Serial.printf("length of image is %d\n", strlen((char*)IMAGE_imeds));
}

int EPD_set_header (Epd epd, std::string text, int pos, int bgc){
  unsigned char image[1500];
  Paint paint(image, 400, 20);    //width should be the multiple of 8 
  int indent;
  switch (pos)
  {
  case gauche:
    indent = 0;
    break;
  case milieu:
    indent = (400-11*text.length())/2;
    break;
  case droit:
    indent = 400-11*text.length();
    break;
  default:
    return -1;
  }

  paint.Clear(COLORED);
  paint.DrawStringAt(indent, 2, text.c_str(), &Font16, 1-bgc);
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  xSemaphoreTake(display_semaphore_epd, portMAX_DELAY);
  pref.begin("status", true);
  if(pref.getBool("wifi_connected", false)){
    if(bgc){
      epd.SetPartialWindow(IMAGE_wifi, 384, 2, 16, 16);
    }else{
      epd.SetPartialWindow(IMAGE_wifi_r, 384, 2, 16, 16);
    }
  }
  if(pref.getBool("BT_connected", false)){
    if(bgc){
      epd.SetPartialWindow(IMAGE_bluetooth, 368, 2, 16, 16);
    }else{
      epd.SetPartialWindow(IMAGE_bluetooth_r, 368, 2, 16, 16);
    }
  }
  pref.end();
  xSemaphoreGive(display_semaphore_epd);
  return 0;
}

int EPD_set_header (Epd epd, std::string text, int pos){
  return EPD_set_header (epd, text, pos, COLORED);
}

int EPD_set_footer (Epd epd, std::string text, int pos, int bgc){
  unsigned char image[1500];
  Paint paint(image, 400, 20);    //width should be the multiple of 8 
  int indent;
  switch (pos)
  {
  case gauche:
    indent = 0;
    break;
  case milieu:
    indent = (400-11*text.length())/2;
    break;
  case droit:
    indent = 400-11*text.length();
    break;
  default:
    return -1;
  }

  paint.Clear(COLORED);
  paint.DrawStringAt(indent, 2, text.c_str(), &Font16, 1-bgc);
  epd.SetPartialWindow(paint.GetImage(), 0, 280, paint.GetWidth(), paint.GetHeight());
  return 0;
}

int EPD_set_footer (Epd epd, std::string text, int pos){
  return EPD_set_footer (epd, text, pos, COLORED);
}

/*
void EPD_loop (void *){
  running_flag = true;
  while (running_flag){
    pref.begin("status", false);
    if (pref.getBool("status_changed", false)){
      EPD_set_header (epd, "test_header", milieu);
      EPD_set_footer (epd, "test_footer", milieu);
      epd.DisplayFrame();
      epd.Sleep();
      pref.putBool("status_changed", false);
    }
    pref.end();
    Serial.println("here");
    sleep(2);
  }
  vTaskDelete(NULL);
}
*/

void EPD_loop (void *){
  bool refresh = false;
  running_flag = true;
  EventBits_t eventRet;
  bool temp;
  while (running_flag){
    eventRet = xEventGroupWaitBits(xEventGroup_display, event_wifi|event_bluetooth|event_test, pdTRUE, pdFALSE, 1000/portTICK_PERIOD_MS);
    Serial.print("eventRet is ");
    Serial.println(eventRet);
    pref.begin("status", false);
    if(eventRet & event_wifi){
      temp = pref.getBool("wifi_connected", false);
      pref.putBool("wifi_connected", !temp);
      refresh = true;
    }
    if(eventRet & event_bluetooth){
      temp = pref.getBool("BT_connected", false);
      pref.putBool("BT_connected", !temp);
      refresh = true;
    }
    pref.end();
    if(eventRet & event_test){
      // Serial.println("printing R6...");
      // epd.DisplayFrame(IMAGER6);
      // epd.ClearFrame();
      EPD_set_header (epd, "Welcome!", milieu);
      EPD_set_footer (epd, "test_footer", milieu);
      Serial.println("test event rcvd, printing...");
      epd.DisplayFrame();
      // epd.DisplayFrame(IMAGER6);
    }
    if (refresh){
      epd.ClearFrame();
      EPD_set_header (epd, "test_header", milieu);
      EPD_set_footer (epd, "test_footer", milieu);
      epd.DisplayFrame();
      // epd.Sleep();
      refresh = false;
    }
    // int stack_left = uxTaskGetStackHighWaterMark(NULL);
    // Serial.printf("[EPD4in2Utils]stack left is %dB\n", stack_left);
    Serial.println("[EPD4in2Utils]EPD running");
    sleep(2);
  }
  vTaskDelete(NULL);
}
