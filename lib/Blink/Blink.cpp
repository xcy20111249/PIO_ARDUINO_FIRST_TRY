#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "Blink.h"

void blink(void *ledpin){
    while (1)
    {
        for(int i = 0; i<10; ++i){
            digitalWrite ((int)ledpin, HIGH);
            vTaskDelay (200);
            digitalWrite ((int)ledpin, LOW);
            vTaskDelay (200);
        }
        Serial.println("One blink circle done.");
        vTaskDelay(5000);
    }
    vTaskDelete(NULL);
}
