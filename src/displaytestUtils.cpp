
#include "config_test.h"
#include "Preferences.h"

static Preferences pref;
void displaytest_loop(void*){
  for (int i=0; i<2; i++){
    xEventGroupSetBits(xEventGroup_display, event_test);
    sleep(5);
  }
  xEventGroupSetBits(xEventGroup_display, event_wifi);
  sleep(5);
  while (1)
  {
    pref.begin("status", false);
    bool k = pref.getBool("wifi_connected", false);
    pref.end();
    Serial.printf("k is %d\n", k);
    sleep(5);
  }
  
  vTaskDelete(NULL);
}