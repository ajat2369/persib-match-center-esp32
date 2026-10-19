#ifndef CONFIG_H
#define CONFIG_H

// WiFi
#define WIFI_SSID       "MASUKKAN_SSID_WIFI_ANDA"
#define WIFI_PASSWORD   "MASUKKAN_PASSWORD_WIFI_ANDA"

// API Endpoint (FIXED - jangan diubah kecuali ganti deployment)
#define API_URL         "https://script.google.com/macros/s/AKfycbzO1jnBhm_yJgWGu9tVX7mCTO5M4EcWWIgXAm8k7GGZtYDBJYTkBMmRfkVf2fhMhD7o/exec"

// OTA
#define OTA_HOSTNAME    "persib-dashboard"
#define OTA_PASSWORD    "MASUKKAN_PASSWORD_OTA_ANDA"

// Pin (ESP32 WROOM)
#define TFT_CS     5
#define TFT_DC     16
#define TFT_RST    17
#define TFT_MOSI   23
#define TFT_SCLK   18
#define BUZZER_PIN 25
#define LED_PIN    2

// Timing
#define SCREEN_INTERVAL      12000   // 12 detik per halaman
#define SUB_SCREEN_INTERVAL   6000   // 6 detik per sub-halaman
#define API_FETCH_INTERVAL  300000   // 5 menit (fallback jika meta tidak ada)

// Cuaca (bisa diubah via Sheets juga)
#define WEATHER_CITY        "Bandung"
#define WEATHER_COUNTRY     "ID"
#define WEATHER_API_KEY     "MASUKKAN_OPENWEATHERMAP_API_KEY"

// Sholat (koordinat Bandung)
#define PRAYER_LAT          -6.9147
#define PRAYER_LON          107.6098

// NTP
#define NTP_SERVER          "pool.ntp.org"
#define NTP_GMT_OFFSET      25200
#define NTP_DST_OFFSET      0

#endif