#ifndef PAGE_KLASEMEN_H
#define PAGE_KLASEMEN_H

#include <Adafruit_ILI9341.h>
#include "cache.h"
#include "display_ui.h"

void drawPageKlasemen(Adafruit_ILI9341 &tft) {
  // 1. Top Bar
  drawTopBar(tft, "KLASEMEN BRI LIGA", ILI9341_GREEN);
  
  // 2. Sub-title
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 42);
  tft.print("Top 8 Klasemen Sementara");
  
  // 3. Header Tabel
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setCursor(10, 55);
  tft.print("#  KLUB                    MP   PTS");
  tft.drawLine(10, 67, 310, 67, ILI9341_DARKGREY);
  
  // 4. Daftar Klasemen
  int yPos = 72;
  int maxRows = (cacheKlasemenCount > 8) ? 8 : cacheKlasemenCount;
  
  for (int i = 0; i < maxRows; i++) {
    // Cek apakah ini Persib
    bool isPersib = (cacheKlasemen[i].klub.indexOf("PERSIB") >= 0 || 
                     cacheKlasemen[i].klub.indexOf("Persib") >= 0);
    
    if (isPersib) {
      // Highlight khusus Persib (Latar Biru)
      tft.fillRect(5, yPos - 3, 310, 18, ILI9341_BLUE);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
    } else {
      // Tim lain (Latar Hitam)
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    
    // Posisi
    tft.setCursor(10, yPos);
    tft.print(cacheKlasemen[i].pos);
    
    // Nama Klub
    tft.setCursor(30, yPos);
    tft.print(cacheKlasemen[i].klub);
    
    // Main (MP) & Poin (PTS)
    tft.setCursor(245, yPos);
    tft.print(cacheKlasemen[i].mp);
    
    tft.setCursor(285, yPos);
    tft.print(cacheKlasemen[i].pts);
    
    yPos += 18;
  }
  
  // 5. Pesan jika kosong
  if (maxRows == 0) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 80);
    tft.print("Data Kosong");
  }
  
  // 6. Progress Bar Navigasi
  drawProgressBar(tft, 3, "Klasemen");
}

#endif