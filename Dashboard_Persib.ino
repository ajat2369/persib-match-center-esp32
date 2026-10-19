/**
 * ============================================================================
 * PERSIB BANDUNG MATCH CENTER - ESP32 FIRMWARE
 * ============================================================================
 * 
 * Proyek    : Dashboard Informasi Persib Bandung
 * Hardware  : ESP32 WROOM (atau S3) + Display ILI9341 + Buzzer
 * OS        : FreeRTOS (Arduino Framework)
 * Fitur     : 
 *   - 7 Halaman dinamis (Dashboard, Jadwal, Cuaca, Klasemen, Analisis, Sholat, Hasil)
 *   - Sinkronisasi Data via API (Google Apps Script + Google Sheets)
 *   - Sinkronisasi Waktu via NTP
 *   - Cuaca Real-time via OpenWeatherMap
 *   - Update Firmware via WiFi (OTA)
 *   - Boot Animasi & Nada Mario Bros
 * 
 * Author    : [Nama Anda]
 * Repo      : [Link GitHub Anda]
 * Lisensi   : MIT License
 * ============================================================================
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <time.h>
#include <ArduinoJson.h>

// --- Konfigurasi Lokal ---
#include "config.h"
#include "cache.h"
#include "api_client.h"
#include "display_ui.h"

// --- Modul Halaman ---
#include "page_dashboard.h"
#include "page_jadwal.h"
#include "page_cuaca.h"
#include "page_klasemen.h"
#include "page_analisis.h"
#include "page_sholat.h"
#include "page_hasil.h"

// --- Variabel Global Hardware ---
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
bool otaInProgress = false;

// --- Modul OTA (Harus di-include setelah tft & otaInProgress) ---
#include "ota_setup.h" 

// --- Variabel Navigasi ---
int screenIndex = 0;
int subScreenIndex = 0;
unsigned long lastScreenChange = 0;
unsigned long lastSubScreenChange = 0;

// ============================================================
// DEKLARASI FUNGSI LOKAL
// ============================================================
void hitungSholatBerikutnya();
bool waktuSudahSinkron();
void playTone(int frequency, int duration);
void playMarioTheme();

// ============================================================
// SETUP (Inisialisasi Awal)
// ============================================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // 1. Inisialisasi Buzzer
  ledcAttach(BUZZER_PIN, 2000, 8);

  // 2. Inisialisasi Display
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.begin();
  delay(100);
  tft.setRotation(1);
  
  // 3. Animasi Booting (Logo & Suara)
  drawBootSplash(tft); // Fungsi ada di display_ui.h
  playMarioTheme();
  
  delay(500);
  tft.fillScreen(ILI9341_BLACK);
  
  // 4. Koneksi WiFi
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.print("Connecting WiFi...");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  // 5. Jika WiFi Tersambung, Lanjutkan
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    tft.setCursor(10, 80);
    tft.print("Syncing Time...");
    
    // Sinkronisasi Waktu (NTP)
    configTime(25200, 0, "pool.ntp.org", "time.nist.gov", "time.google.com");
    int ntpAttempts = 0;
    while (!waktuSudahSinkron() && ntpAttempts < 40) {
      delay(500);
      ntpAttempts++;
    }

    // Inisialisasi OTA
    setupOTA(tft, otaInProgress);

    // Tarik Data Pertama Kali
    fetchApiData();
    hitungSholatBerikutnya(); 
  }

  // 6. Tampilkan Halaman Pertama
  screenIndex = 0;
  tft.fillScreen(ILI9341_BLACK);
  drawPageDashboard(tft);
  lastScreenChange = millis();
  lastSubScreenChange = millis();
  
  // Nada Sukses
  playTone(1047, 100); delay(50);
  playTone(1047, 100); delay(50);
  playTone(1047, 150);
}

// ============================================================
// LOOP (Berjalan Terus Menerus)
// ============================================================
void loop() {
  // 1. Cek Update OTA
  ArduinoOTA.handle();
  if (otaInProgress) { delay(100); return; }
  
  // 2. Update Hitungan Sholat (Setiap 1 Menit)
  static unsigned long lastSholatCalc = 0;
  if (millis() - lastSholatCalc > 60000) { 
    hitungSholatBerikutnya();
    lastSholatCalc = millis();
  }

  // 3. Fetch API Periodik
  int interval = (cacheMeta.nextReloadSec > 0) ? (cacheMeta.nextReloadSec * 1000UL) : API_FETCH_INTERVAL;
  if (millis() - lastApiFetch > interval) {
    fetchApiData();
    hitungSholatBerikutnya();
    lastSholatCalc = millis();
  }

  // ============================================================
  // 4. Navigasi Sub-Halaman (Khusus Halaman Analisis / Index 4)
  // ============================================================
  if (screenIndex == 4) {
    // Jika sudah 6 detik, ganti sub-halaman
    if (millis() - lastSubScreenChange > SUB_SCREEN_INTERVAL) {
      subScreenIndex++;
      tft.fillScreen(ILI9341_BLACK);
      
      if (subScreenIndex > 2) {
        // Jika sub 0, 1, 2 sudah selesai, baru pindah ke Halaman 6 (Sholat)
        screenIndex = 5;
        drawPageSholat(tft);
        lastScreenChange = millis(); // Reset timer halaman utama
      } else {
        // Gambar sub-halaman berikutnya (1 atau 2)
        drawPageAnalisis(tft, subScreenIndex);
        lastSubScreenChange = millis(); // Reset timer sub-halaman
      }
    }
  } 
  // ============================================================
  // 5. Navigasi Halaman Utama (Kecuali Halaman Analisis)
  // ============================================================
  else if (millis() - lastScreenChange > SCREEN_INTERVAL) {
    // Jika BUKAN di halaman Analisis, timer 12 detik berjalan normal
    screenIndex = (screenIndex + 1) % 7;
    tft.fillScreen(ILI9341_BLACK);
    
    if (screenIndex == 4) {
      // Jika baru masuk ke Analisis, mulai dari Sub 0
      subScreenIndex = 0;
      lastSubScreenChange = millis();
    }

    switch (screenIndex) {
      case 0: drawPageDashboard(tft); break;
      case 1: drawPageJadwal(tft); break;
      case 2: drawPageCuaca(tft); break;
      case 3: drawPageKlasemen(tft); break;
      case 4: drawPageAnalisis(tft, subScreenIndex); break;
      case 5: drawPageSholat(tft); break;
      case 6: drawPageHasil(tft); break;
    }
    lastScreenChange = millis();
  }

  // 6. Indikator LED
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(100);
}

// ============================================================
// FUNGSI LOKAL (Diletakkan di bawah agar file utama tetap bersih)
// ============================================================

void hitungSholatBerikutnya() {
  if (cacheSholatCount == 0) return;
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;
  
  int currentMinutes = timeinfo.tm_hour * 60 + timeinfo.tm_min;
  int minDiff = 24 * 60;
  
  for (int i = 0; i < cacheSholatCount; i++) {
    String waktuStr = cacheSholat[i].waktu;
    int colonPos = waktuStr.indexOf(':');
    if (colonPos > 0) {
      int h = waktuStr.substring(0, colonPos).toInt();
      int m = waktuStr.substring(colonPos + 1, colonPos + 3).toInt();
      int prayerMinutes = h * 60 + m;
      int diff = prayerMinutes - currentMinutes;
      
      if (diff < 0) diff += 24 * 60; 
      
      if (diff < minDiff) {
        minDiff = diff;
        nextPrayerName = cacheSholat[i].nama;
        nextPrayerMinutes = diff;
      }
    }
  }
}

bool waktuSudahSinkron() {
  time_t now = time(nullptr);
  return (now > 1700000000); 
}

void playTone(int frequency, int duration) {
  ledcWriteTone(BUZZER_PIN, frequency);
  delay(duration);
  ledcWriteTone(BUZZER_PIN, 0);
  delay(30);
}

// ============================================================
// DEFINISI NADA (FREKUENSI)
// ============================================================
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047

void playMarioTheme() {
  playTone(NOTE_E5, 150); playTone(NOTE_E5, 150); delay(150);
  playTone(NOTE_E5, 150); delay(150);
  playTone(NOTE_C5, 150); playTone(NOTE_E5, 150); delay(150);
  playTone(NOTE_G5, 200); delay(200);
  playTone(NOTE_G4, 200); delay(200);
  
  playTone(NOTE_C5, 150); delay(150);
  playTone(NOTE_G4, 150); delay(150);
  playTone(NOTE_E4, 150); delay(150);
  playTone(NOTE_A4, 150); delay(150);
  playTone(NOTE_B4, 150); delay(150);
  playTone(NOTE_AS4, 150); playTone(NOTE_A4, 150); delay(150);
  playTone(NOTE_G4, 150); playTone(NOTE_E5, 150);
  playTone(NOTE_G5, 150); playTone(NOTE_A5, 150); delay(150);
  playTone(NOTE_F5, 150); playTone(NOTE_G5, 150);
}