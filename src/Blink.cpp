#include <Arduino.h>
#include "freertos/FreeRTOS.h"

#include "config_test.h"
#include "Blink.h"

static bool running_flag;

void blink(void*){
    pinMode(ledpin, OUTPUT);
    running_flag = true;
    while (running_flag)
    {
        for(int i = 0; i<10; ++i){
            digitalWrite (ledpin, HIGH);
            vTaskDelay (200);
            digitalWrite (ledpin, LOW);
            vTaskDelay (200);
        }
        Serial.println("One blink circle done.");
        delay(2000);
    }
    vTaskDelete(NULL);
}

void blink_stop(){
    running_flag = false;
}
