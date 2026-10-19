#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Adafruit_ILI9341.h>

void drawTopBar(Adafruit_ILI9341 &tft, const char* title, uint16_t color) {
  tft.fillRect(0, 0, 320, 35, color);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  int w = strlen(title) * 12;
  tft.setCursor((320 - w) / 2, 10);
  tft.print(title);
}

void drawProgressBar(Adafruit_ILI9341 &tft, int current, const char* name) {
  int bw = 280, bx = 20, by = 227;
  tft.fillRect(bx, by, bw, 4, ILI9341_DARKGREY);
  int sw = bw / 7;
  for (int i = 0; i < 7; i++) {
    uint16_t c = (i == current) ? ILI9341_GREEN : ILI9341_DARKGREY;
    tft.fillRect(bx + i * sw, by, sw - 2, 4, c);
  }
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(22, 233);
  tft.printf("[%s]", name);
  tft.setCursor(280, 233);
  tft.printf("%d/7", current + 1);
}

void drawStatCard(Adafruit_ILI9341 &tft, int x, int y, int w, int h,
                  const char* label, const char* value, uint16_t color) {
  tft.fillRect(x, y, w, h, color);
  tft.setTextColor(ILI9341_BLACK, color);
  tft.setTextSize(1);
  tft.setCursor(x + 5, y + 5);
  tft.print(label);
  tft.setTextSize(2);
  tft.setCursor(x + 15, y + 22);
  tft.print(value);
}

// Fungsi Gambar Bintang
void drawStar(Adafruit_ILI9341 &tft, int x, int y, int radius, uint16_t color) {
  int x0 = x,                 y0 = y - radius;
  int x1 = x - radius * 0.95, y1 = y - radius * 0.31;
  int x2 = x - radius * 0.59, y2 = y + radius * 0.81;
  int x3 = x + radius * 0.59, y3 = y + radius * 0.81;
  int x4 = x + radius * 0.95, y4 = y - radius * 0.31;
  int x5 = x - radius * 0.22, y5 = y - radius * 0.31;
  int x6 = x - radius * 0.36, y6 = y + radius * 0.12;
  int x7 = x,                 y7 = y + radius * 0.38;
  int x8 = x + radius * 0.36, y8 = y + radius * 0.12;
  int x9 = x + radius * 0.22, y9 = y - radius * 0.31;

  tft.drawLine(x0, y0, x5, y5, color); tft.drawLine(x5, y5, x1, y1, color);
  tft.drawLine(x1, y1, x6, y6, color); tft.drawLine(x6, y6, x2, y2, color);
  tft.drawLine(x2, y2, x7, y7, color); tft.drawLine(x7, y7, x3, y3, color);
  tft.drawLine(x3, y3, x8, y8, color); tft.drawLine(x8, y8, x4, y4, color);
  tft.drawLine(x4, y4, x9, y9, color); tft.drawLine(x9, y9, x0, y0, color);
}

void drawBootSplash(Adafruit_ILI9341 &tft) {
  tft.fillScreen(ILI9341_NAVY);
  
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(5);
  tft.setCursor(60, 60);
  tft.print("PERSIB");
  
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(75, 120);
  tft.print("MATCH CENTER");
  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(115, 150);
  tft.print("Edisi 2026/2027");
  
  for (int i = 0; i < 5; i++) {
    drawStar(tft, 80 + (i * 40), 30, 10, ILI9341_YELLOW);
  }
}

#endif