
#include "EPD4in2Utils.h"

#define COLORED     0
#define UNCOLORED   1

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
  Serial.println("here");

  paint.Clear(COLORED);
  Serial.println("here");

  paint.DrawStringAt(indent, 2, text.c_str(), &Font16, 1-bgc);
  Serial.println("here");
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  return 0;
}

int EPD_set_header (Epd epd, std::string text, int pos){
  return EPD_set_header (epd, text, pos, COLORED);
}