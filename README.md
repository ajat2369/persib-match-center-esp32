# Persib Bandung Match Center (ESP32 Dashboard)

Dashboard informasi seputar Persib Bandung menggunakan ESP32, layar TFT ILI9341 3.2", dan Buzzer. Data ditarik secara dinamis melalui Google Apps Script dari Google Sheets.

## Fitur
- 📊 Dashboard statistik & Laga Berikutnya
- 📅 Jadwal 5 Pertandingan
- 🌤️ Cuaca Real-time Bandung
- 🏆 Klasemen BRI (Highlight Persib)
- 📈 Analisis Peluang & Klasemen ACL
- 🕌 Jadwal Sholat & Countdown
- ⚽ Hasil 5 Pertandingan Terakhir
- 🎵 Boot Screen (5 Bintang & Nada Mario)
- 📡 Update OTA (Over-The-Air)

## Skema Pin (ESP32 WROOM)
| Komponen | Pin |
|---|---|
| TFT CS | 5 |
| TFT DC | 16 |
| TFT RST | 17 |
| TFT MOSI | 23 |
| TFT SCLK | 18 |
| Buzzer | 25 |

## Cara Menjalankan
1. Buka `config.h`, sesuaikan `WIFI_SSID`, `WIFI_PASSWORD`, dan URL `API_URL`.
2. Pastikan library `Adafruit ILI9341`, `Adafruit GFX`, dan `ArduinoJson` sudah terinstal.
3. Upload ke ESP32.

## Lisensi
MIT License
