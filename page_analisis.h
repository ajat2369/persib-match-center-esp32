#ifndef PAGE_ANALISIS_H
#define PAGE_ANALISIS_H

#include <Adafruit_ILI9341.h>
#include "cache.h"
#include "display_ui.h"

// Progress bar yang aman (tanpa loop)
void drawSafeProgressBar(Adafruit_ILI9341 &tft) {
  tft.fillRect(20, 227, 280, 4, ILI9341_DARKGREY);
  tft.fillRect(140, 227, 40, 4, ILI9341_GREEN); 
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(22, 233);
  tft.print("[Analisis]");
  tft.setCursor(280, 233);
  tft.print("5/7");
}

void drawAnalisisPeluang(Adafruit_ILI9341 &tft) {
  drawTopBar(tft, "PELUANG PERSIB", ILI9341_ORANGE);
  
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 42);
  tft.print("PELUANG JUARA BRI LIGA");

  int peluang = cacheAnalisis.peluangJuara;
  tft.drawRect(10, 55, 300, 20, ILI9341_WHITE);
  int fillWidth = map(peluang, 0, 100, 0, 296);
  tft.fillRect(12, 57, fillWidth, 16, ILI9341_GREEN);
  tft.setTextColor(ILI9341_BLACK, ILI9341_GREEN);
  tft.setCursor(140, 61);
  tft.printf("%d%%", peluang);

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setCursor(10, 80);
  tft.print("Prediksi Pengamat");
  
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 115);
  tft.print("KONTRIBUSI DI ASIA");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setCursor(10, 130);
  tft.print("Peringkat Indonesia: ");
  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  tft.printf("#%d Asia", cacheAnalisis.peringkatIDN);

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setCursor(10, 145);
  tft.print("Total Poin: ");
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.printf("%.3f", cacheAnalisis.totalPoinAsia);
}

void drawTabelKlasemen(Adafruit_ILI9341 &tft, GrupItem* data, int count, const char* judul) {
  drawTopBar(tft, judul, ILI9341_BLUE);
  
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 65);
  tft.print("#   TIM                     MP  SG  PTS");
  tft.drawLine(10, 77, 310, 77, ILI9341_DARKGREY);

  int yPos = 85;
  int maxTampil = (count > 5) ? 5 : count;
  
  for (int i = 0; i < maxTampil; i++) {
    bool isPersib = data[i].tim.indexOf("Persib") >= 0;
    
    if (isPersib) {
      tft.fillRect(5, yPos - 3, 310, 18, ILI9341_NAVY);
      tft.setTextColor(ILI9341_WHITE, ILI9341_NAVY);
    } else {
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }

    tft.setCursor(12, yPos); tft.print(data[i].pos);
    tft.setCursor(30, yPos); tft.print(data[i].tim);
    tft.setCursor(235, yPos); tft.print(data[i].mp);
    tft.setCursor(265, yPos); if (data[i].sg > 0) tft.print("+"); tft.print(data[i].sg);
    tft.setCursor(295, yPos); tft.print(data[i].pts);

    yPos += 20; 
  }
}

void drawPageAnalisis(Adafruit_ILI9341 &tft, int subIndex) {
  tft.fillScreen(ILI9341_BLACK);
  
  if (subIndex == 0) {
    drawAnalisisPeluang(tft);
  } else if (subIndex == 1) {
    drawTabelKlasemen(tft, cacheACL, cacheACLCount, "GRUP E ACL TWO");
  } else if (subIndex == 2) {
    drawTabelKlasemen(tft, cacheACC, cacheACCCount, "SHOPEE CUP");
  }
  
  drawSafeProgressBar(tft);
}

#endif