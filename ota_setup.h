#ifndef OTA_SETUP_H
#define OTA_SETUP_H

#include <ArduinoOTA.h>
#include <Adafruit_ILI9341.h>
#include "config.h"

// Fungsi ini akan dipanggil di setup()
void setupOTA(Adafruit_ILI9341 &tft, bool &otaInProgress) {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([&tft, &otaInProgress]() {
    otaInProgress = true;
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(3);
    tft.setCursor(40, 60);
    tft.print("OTA UPDATE");
  });

  ArduinoOTA.onEnd([&tft, &otaInProgress]() {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(3);
    tft.setCursor(40, 90);
    tft.print("OTA DONE!");
    delay(1000);
    otaInProgress = false;
  });

  ArduinoOTA.onProgress([&tft](unsigned int progress, unsigned int total) {
    uint8_t pct = (progress / (total / 100));
    tft.drawRect(20, 170, 280, 25, ILI9341_WHITE);
    tft.fillRect(22, 172, map(pct, 0, 100, 0, 276), 21, ILI9341_GREEN);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(140, 175);
    tft.printf("%3u%%", pct);
  });

  ArduinoOTA.onError([&tft](ota_error_t e) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.setCursor(40, 80);
    tft.print("OTA FAILED!");
  });

  ArduinoOTA.begin();
  Serial.println("[OTA] Ready");
}

#endif