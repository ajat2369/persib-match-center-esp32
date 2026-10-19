#ifndef PAGE_SHOLAT_H
#define PAGE_SHOLAT_H

#include <Adafruit_ILI9341.h>
#include <time.h>
#include "cache.h"
#include "display_ui.h"

void drawPageSholat(Adafruit_ILI9341 &tft) {
  drawTopBar(tft, "JADWAL SHOLAT", ILI9341_GREEN);
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 42);
    tft.printf("%s, %02d-%02d-%04d",
      getDayName(timeinfo.tm_wday), 
      timeinfo.tm_mday,
      timeinfo.tm_mon + 1, 
      timeinfo.tm_year + 1900);
  }

  if (cacheSholatCount == 0) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 70);
    tft.print("Jadwal Sholat");
    tft.setCursor(10, 100);
    tft.print("Tidak Tersedia");
    drawProgressBar(tft, 5, "Sholat");
    return;
  }

  const char* labels[] = {"Subuh", "Terbit", "Dzuhur", "Ashar", "Maghrib", "Isya"};
  uint16_t colors[] = {ILI9341_CYAN, ILI9341_ORANGE, ILI9341_YELLOW, ILI9341_MAGENTA, ILI9341_RED, ILI9341_BLUE};

  int col1X = 10, col2X = 165; // Sedikit digeser agar lebih lega
  int yPos = 65, rowHeight = 32;

  for (int i = 0; i < 6; i++) {
    int x = (i < 3) ? col1X : col2X;
    int y = yPos + ((i % 3) * rowHeight);
    
    bool isNext = (cacheSholat[i].nama == nextPrayerName);
    uint16_t boxColor = isNext ? ILI9341_GREEN : colors[i];
    
    // 1. Kotak Label (Subuh, Terbit, dll)
    tft.fillRoundRect(x, y, 55, 22, 3, boxColor);
    tft.setTextColor(ILI9341_BLACK, boxColor);
    tft.setTextSize(1);
    tft.setCursor(x + 6, y + 7);
    tft.print(labels[i]);
    
    // 2. Ambil string waktu dan bersihkan
    String rawWaktu = cacheSholat[i].waktu;
    String cleanWaktu = rawWaktu;
    
    // Jika API mengirim format panjang (misal dari Sheets), ambil 5 karakter pertama
    if (rawWaktu.indexOf("T") > 0) {
      // Format 1899-12-30T04:15:00.000Z -> kita ambil setelah 'T'
      int tPos = rawWaktu.indexOf("T");
      cleanWaktu = rawWaktu.substring(tPos + 1, tPos + 6); // Ambil 5 char (04:15)
    } else if (rawWaktu.length() > 5) {
      // Jika kepanjangan tapi tidak ada 'T', paksa potong 5 karakter
      cleanWaktu = rawWaktu.substring(0, 5); 
    }

    // 3. Cetak Waktu
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(x + 65, y + 4);
    tft.print(cleanWaktu);
    
    // 4. Label NEXT
    if (isNext) {
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.setTextSize(1);
      tft.setCursor(x + 130, y + 7); // Geser posisi NEXT
      tft.print("NEXT");
    }
  }

  if (nextPrayerMinutes > 0) {
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 210);
    tft.printf("Berikutnya: %s dalam %d menit", nextPrayerName.c_str(), nextPrayerMinutes);
  }
  
  drawProgressBar(tft, 5, "Sholat");
}

#endif