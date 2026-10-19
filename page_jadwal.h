#ifndef PAGE_JADWAL_H
#define PAGE_JADWAL_H

#include <Adafruit_ILI9341.h>
#include "cache.h"
#include "display_ui.h"

void drawPageJadwal(Adafruit_ILI9341 &tft) {
  // 1. Top Bar
  drawTopBar(tft, "JADWAL PERSIB", ILI9341_BLUE);
  
  // 2. Sub-title
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 42);
  tft.print("5 Pertandingan Berikutnya");
  
  // 3. Daftar Jadwal
  int yPos = 60;
  
  // Pastikan kita tidak membaca array melebihi kapasitas
  int maxRows = (cacheJadwalCount > 5) ? 5 : cacheJadwalCount;
  
  for (int i = 0; i < maxRows; i++) {
    // Tentukan warna berdasarkan kompetisi
    uint16_t warnaKotak = ILI9341_GREEN; // Default BRI
    String komp = cacheJadwal[i].kompetisi;
    
    if (komp.indexOf("ACL") >= 0) {
      warnaKotak = ILI9341_ORANGE;
    } else if (komp.indexOf("Shopee") >= 0) {
      warnaKotak = ILI9341_MAGENTA;
    }

    // a. Kotak Tanggal
    tft.fillRoundRect(10, yPos, 55, 22, 3, warnaKotak);
    tft.setTextColor(ILI9341_BLACK, warnaKotak);
    tft.setTextSize(1);
    tft.setCursor(12, yPos + 7); // Sedikit disesuaikan agar center
    tft.print(cacheJadwal[i].tanggal);
    
    // b. Nama Pertandingan
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setCursor(75, yPos + 3);
    tft.print(cacheJadwal[i].lawan);
    
    // c. Nama Kompetisi
    tft.setTextColor(ILI9341_DARKGREY, ILI9341_BLACK);
    tft.setCursor(75, yPos + 15);
    tft.print(cacheJadwal[i].kompetisi);
    
    // d. Garis Pemisah
    tft.drawLine(10, yPos + 28, 310, yPos + 28, ILI9341_DARKGREY);
    
    yPos += 33; // Jarak antar baris
  }
  
  // 4. Tampilkan pesan jika data kosong
  if (maxRows == 0) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 80);
    tft.print("Data Jadwal Kosong");
  }
  
  // 5. Progress Bar Navigasi
  drawProgressBar(tft, 1, "Jadwal");
}

#endif