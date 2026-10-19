#ifndef PAGE_HASIL_H
#define PAGE_HASIL_H

#include <Adafruit_ILI9341.h>
#include "cache.h"
#include "display_ui.h"

void drawPageHasil(Adafruit_ILI9341 &tft) {
  // 1. Top Bar
  drawTopBar(tft, "HASIL 5 TERAKHIR", ILI9341_MAGENTA);
  
  // 2. Form Summary
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 42);
  tft.print("Form Terkini:");
  
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  // Ambil string form dari Dashboard cache (misal: "W W L W W")
  tft.print(cacheDashboard.form); 
  
  // 3. Daftar Hasil Pertandingan
  int yPos = 60;
  
  // Pastikan tidak membaca array melebihi kapasitas
  int maxRows = (cacheHasilCount > 5) ? 5 : cacheHasilCount;
  
  for (int i = 0; i < maxRows; i++) {
    // Tentukan warna kotak berdasarkan status (W, D, L)
    uint16_t warnaKotak = ILI9341_DARKGREY; // Default
    String status = cacheHasil[i].status;
    status.toUpperCase(); // Pastikan jadi huruf besar
    
    if (status == "W") {
      warnaKotak = ILI9341_GREEN;
    } else if (status == "D") {
      warnaKotak = ILI9341_YELLOW;
    } else if (status == "L") {
      warnaKotak = ILI9341_RED;
    }

    // a. Kotak Tanggal & Status
    tft.fillRoundRect(10, yPos, 60, 22, 3, warnaKotak);
    tft.setTextColor(ILI9341_BLACK, warnaKotak);
    tft.setTextSize(1);
    tft.setCursor(12, yPos + 7); 
    tft.print(cacheHasil[i].tanggal);
    
    // b. Nama Pertandingan
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setCursor(80, yPos + 3);
    tft.print(cacheHasil[i].pertandingan);
    
    // c. Skor (Rata Kanan)
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.setTextSize(2);
    // Menggunakan setCursor tepat sebelum print untuk meratakan kanan
    // "1-3" biasanya selebar 3 karakter x 12px = 36px
    tft.setCursor(270, yPos + 1); 
    tft.print(cacheHasil[i].skor);
    
    // d. Garis Pemisah
    tft.drawLine(10, yPos + 28, 310, yPos + 28, ILI9341_DARKGREY);
    
    yPos += 33; // Jarak antar baris
  }
  
  // 4. Pesan jika kosong
  if (maxRows == 0) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 80);
    tft.print("Data Hasil Kosong");
  }
  
  // 5. Progress Bar Navigasi
  drawProgressBar(tft, 6, "Hasil");
}

#endif