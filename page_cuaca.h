#ifndef PAGE_CUACA_H
#define PAGE_CUACA_H

#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "display_ui.h"

void drawPageCuaca(Adafruit_ILI9341 &tft) {
  // 1. Top Bar
  drawTopBar(tft, "CUACA BANDUNG", ILI9341_ORANGE);
  
  // 2. Cek Koneksi WiFi
  if (WiFi.status() != WL_CONNECTED) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.print("No WiFi!");
    drawProgressBar(tft, 2, "Cuaca");
    return;
  }
  
  // 3. Tampilkan Loading
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 50);
  tft.print("Fetching weather data...");
  
  // 4. Request ke OpenWeatherMap
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" 
               + String(WEATHER_CITY) + "," + String(WEATHER_COUNTRY) 
               + "&APPID=" + String(WEATHER_API_KEY) 
               + "&units=metric"; // Menggunakan Celsius langsung
               
  HTTPClient http;
  http.begin(url);
  http.setTimeout(5000);
  int httpCode = http.GET();
  
  // 5. Cek Response HTTP
  if (httpCode != 200) {
    tft.fillScreen(ILI9341_BLACK);
    drawTopBar(tft, "CUACA BANDUNG", ILI9341_ORANGE);
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.printf("HTTP Error: %d", httpCode);
    http.end();
    drawProgressBar(tft, 2, "Cuaca");
    return;
  }
  
  // 6. Parse JSON
  String payload = http.getString();
  http.end();
  
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);
  
  if (error) {
    tft.fillScreen(ILI9341_BLACK);
    drawTopBar(tft, "CUACA BANDUNG", ILI9341_ORANGE);
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setCursor(10, 50);
    tft.print("JSON Parse Error");
    drawProgressBar(tft, 2, "Cuaca");
    return;
  }
  
  // 7. Ambil Data
  float tempC = doc["main"]["temp"].as<float>();
  float feelsLike = doc["main"]["feels_like"].as<float>();
  int humidity = doc["main"]["humidity"].as<int>();
  const char* weatherMain = doc["weather"][0]["main"];
  const char* weatherDesc = doc["weather"][0]["description"];
  float windSpeed = doc["wind"]["speed"].as<float>();
  
  // 8. Render Tampilan Cuaca
  tft.fillScreen(ILI9341_BLACK);
  drawTopBar(tft, "CUACA BANDUNG", ILI9341_ORANGE);
  
  // Nama Kota
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 45);
  tft.print(WEATHER_CITY);
  
  // Icon Cuaca Sederhana (Matahari/Awan)
  int iconX = 260;
  int iconY = 45;
  if (String(weatherMain) == "Clear") {
    tft.fillCircle(iconX, iconY, 15, ILI9341_YELLOW);
  } else if (String(weatherMain) == "Clouds") {
    tft.fillCircle(iconX - 8, iconY, 12, ILI9341_LIGHTGREY);
    tft.fillCircle(iconX + 8, iconY, 12, ILI9341_LIGHTGREY);
    tft.fillCircle(iconX, iconY - 8, 14, ILI9341_LIGHTGREY);
  } else if (String(weatherMain) == "Rain") {
    tft.fillCircle(iconX, iconY - 5, 15, ILI9341_DARKGREY);
    tft.drawLine(iconX - 10, iconY + 10, iconX - 15, iconY + 20, ILI9341_CYAN);
    tft.drawLine(iconX, iconY + 10, iconX - 5, iconY + 20, ILI9341_CYAN);
    tft.drawLine(iconX + 10, iconY + 10, iconX + 5, iconY + 20, ILI9341_CYAN);
  }
  
  // Suhu Utama (Besar)
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(6);
  tft.setCursor(20, 80);
  tft.printf("%.0f", tempC);
  
  tft.setTextSize(3);
  tft.setCursor(130, 100);
  tft.print("C");
  
  // Detail Cuaca
  tft.drawLine(10, 160, 310, 160, ILI9341_DARKGREY);
  
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 170);
  tft.printf("Kondisi : %s", weatherMain);
  
  tft.setCursor(10, 185);
  tft.printf("Detail  : %s", weatherDesc);
  
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setCursor(10, 205);
  tft.printf("Terasa  : %.1f C   |   Kelembapan : %d%%", feelsLike, humidity);
  
  tft.setCursor(10, 220);
  tft.printf("Angin   : %.1f m/s", windSpeed);
  
  // 9. Progress Bar Navigasi
  drawProgressBar(tft, 2, "Cuaca");
}

#endif