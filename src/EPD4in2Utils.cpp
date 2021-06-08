
#include "EPD4in2Utils.h"
#include "Preferences.h"

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
  epd.DisplayFrame();
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

  pref.begin("status", true);
  if(pref.getBool("wifi_connected", false)){
    paint.DrawStringAt(356, 2, "wifi", &Font16, 1-bgc);
  }
  pref.end();

  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
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