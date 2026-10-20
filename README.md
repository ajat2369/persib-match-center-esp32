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

## Dokumentasi
<img width="1215" height="2602" alt="20261020_092652" src="https://github.com/user-attachments/assets/adcd7ec7-0b1a-4063-b21b-951196ae9ce6" />
<img width="1196" height="2561" alt="20261020_092703" src="https://github.com/user-attachments/assets/ec8fc17d-a2c9-4f2b-8c0e-4a299d2081b1" />
<img width="1191" height="2551" alt="20261020_092726" src="https://github.com/user-attachments/assets/fe3e18fb-7930-4c55-9abb-4d79bffeedc8" />
<img width="1195" height="2559" alt="20261020_092743" src="https://github.com/user-attachments/assets/785b1473-6df9-4c30-a007-e6b974400e64" />
<img width="1199" height="2567" alt="20261020_092748" src="https://github.com/user-attachments/assets/3cc8f06f-b2bd-4cf5-87e5-4106b56dab22" />
<img width="1188" height="2545" alt="20261020_092807" src="https://github.com/user-attachments/assets/d95cd0ef-4570-42e8-ae77-252f888e54a9" />





