#ifndef PAGE_DASHBOARD_H
#define PAGE_DASHBOARD_H

#include <Adafruit_ILI9341.h>
#include <time.h>
#include "cache.h"
#include "display_ui.h"

// Helper untuk mendapatkan nama hari dalam Bahasa Indonesia
const char* getDayName(int wday) {
  const char* days[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
  return days[wday];
}

// Helper untuk mendapatkan nama bulan dalam Bahasa Indonesia
const char* getMonthName(int mon) {
  const char* months[] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"};
  return months[mon];
}

void drawPageDashboard(Adafruit_ILI9341 &tft) {
  // 1. Top Bar
  drawTopBar(tft, "PERSIB MATCH CENTER", ILI9341_BLUE);
  
  // 2. Tanggal Hari Ini
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 42);
    tft.printf("%s, %02d %s %04d", 
      getDayName(timeinfo.tm_wday), 
      timeinfo.tm_mday, 
      getMonthName(timeinfo.tm_mon), 
      timeinfo.tm_year + 1900
    );
  }
  
  // 3. Empat Kotak Statistik
  // Gunakan buffer char untuk mengubah int ke string
  char bufBRI[5], bufACL[5], bufShopee[5], bufTotal[5];
  sprintf(bufBRI, "%d", cacheDashboard.statBRI);
  sprintf(bufACL, "%d", cacheDashboard.statACL);
  sprintf(bufShopee, "%d", cacheDashboard.statShopee);
  sprintf(bufTotal, "%d", cacheDashboard.statTotal);

  drawStatCard(tft, 10, 60, 70, 45, "BRI", bufBRI, ILI9341_BLUE);
  drawStatCard(tft, 85, 60, 70, 45, "ACL2", bufACL, ILI9341_ORANGE);
  drawStatCard(tft, 160, 60, 70, 45, "SHOPEE", bufShopee, ILI9341_GREEN);
  drawStatCard(tft, 235, 60, 70, 45, "TOTAL", bufTotal, ILI9341_MAGENTA);
  
  // 4. Pertandingan Berikutnya
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 115);
  tft.print("PERTANDINGAN BERIKUTNYA");
  
  // Box Latar Biru Navy
  tft.fillRoundRect(10, 130, 300, 55, 5, ILI9341_NAVY);
  tft.drawRoundRect(10, 130, 300, 55, 5, ILI9341_BLUE); // Border biru
  
  // Kompetisi
  tft.setTextColor(ILI9341_WHITE, ILI9341_NAVY);
  tft.setCursor(15, 135);
  tft.print(cacheDashboard.nextMatch.competition);
  
  // Nama Lawan
  tft.setTextColor(ILI9341_CYAN, ILI9341_NAVY);
  tft.setTextSize(2);
  tft.setCursor(15, 148);
  tft.print(cacheDashboard.nextMatch.opponent);
  
  // Tanggal & Venue
  tft.setTextColor(ILI9341_WHITE, ILI9341_NAVY);
  tft.setTextSize(1);
  tft.setCursor(15, 168);
  // Tampilkan tanggal & waktu
  tft.printf("%s - %s", cacheDashboard.nextMatch.date.c_str(), cacheDashboard.nextMatch.time.c_str());
  
  // 5. Form 5 Laga Terakhir
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 195);
  tft.print("Form 5 Terakhir:");
  
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(90, 195);
  tft.print(cacheDashboard.form);
  
  // 6. Status OTA & Sync API
  tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 213);
  tft.printf("Sync: %s", cacheMeta.updated.c_str());
  
  // 7. Progress Bar Navigasi
  drawProgressBar(tft, 0, "Dashboard");
}

#endif