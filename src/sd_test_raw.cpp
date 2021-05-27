#include <Arduino.h>
#include <SPI.h>
#include "SD.h"

#define SD_CLK_PIN 14
#define SD_MOSI_PIN 15
#define SD_MISO_PIN 2
#define SD_CS 13
#define VERBOSE

void sd_initialize(){
  // Initialize SD card
  SPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS);
  if(!SD.begin(SD_CS)) {
    #ifdef VERBOSE
    Serial.println("Card Mount Failed");
    #endif
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    #ifdef VERBOSE
    Serial.println("No SD card attached");
    #endif
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    #ifdef VERBOSE
    Serial.println("ERROR - SD card initialization failed!");
    #endif
    return;    // init failed
  }
  Serial.println("SD card initialized !");
}

