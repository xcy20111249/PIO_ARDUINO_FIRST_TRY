#include <Arduino.h>
#include <Preferences.h>
#include <sstream>

Preferences pref;

void NVS_TEST(){
  pref.begin("test1",false);
  pref.putBool("global", true);
  pref.end();
}