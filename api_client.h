#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "cache.h"

void fetchApiData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[API] WiFi not connected");
    return;
  }
  
  Serial.println("[API] Fetching...");
  HTTPClient http;
  http.begin(API_URL);
  http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
  http.setTimeout(10000);
  
  int code = http.GET();
  if (code != 200) {
    Serial.printf("[API] HTTP %d\n", code);
    http.end();
    return;
  }
  
  String payload = http.getString();
  http.end();
  
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("[API] JSON error: %s\n", err.c_str());
    return;
  }
  
  // === Meta ===
  JsonObject m = doc["meta"];
  cacheMeta.version = m["version"] | 1;
  cacheMeta.updated = m["updated"].as<String>();
  cacheMeta.source = m["source"].as<String>();
  cacheMeta.nextReloadSec = m["next_reload_sec"] | 300;
  
  // === Dashboard ===
  JsonObject d = doc["dashboard"];
  cacheDashboard.statBRI = d["stat_bri"] | 0;
  cacheDashboard.statACL = d["stat_acl"] | 0;
  cacheDashboard.statShopee = d["stat_shopee"] | 0;
  cacheDashboard.statTotal = d["stat_total"] | 0;
  JsonObject nm = d["next_match"];
  cacheDashboard.nextMatch.opponent = nm["opponent"].as<String>();
  cacheDashboard.nextMatch.date = nm["date"].as<String>();
  cacheDashboard.nextMatch.time = nm["time"].as<String>();
  cacheDashboard.nextMatch.venue = nm["venue"].as<String>();
  cacheDashboard.nextMatch.competition = nm["competition"].as<String>();
  cacheDashboard.form = d["form"].as<String>();
  
  // === Jadwal ===
  cacheJadwalCount = 0;
  for (JsonObject item : doc["jadwal"].as<JsonArray>()) {
    if (cacheJadwalCount >= 5) break;
    cacheJadwal[cacheJadwalCount].tanggal = item["tanggal"].as<String>();
    cacheJadwal[cacheJadwalCount].lawan = item["lawan"].as<String>();
    cacheJadwal[cacheJadwalCount].kompetisi = item["kompetisi"].as<String>();
    cacheJadwalCount++;
  }
  
  // === Klasemen ===
  cacheKlasemenCount = 0;
  for (JsonObject item : doc["klasemen"].as<JsonArray>()) {
    if (cacheKlasemenCount >= 8) break;
    cacheKlasemen[cacheKlasemenCount].pos = item["pos"].as<String>().toInt();
    cacheKlasemen[cacheKlasemenCount].klub = item["klub"].as<String>();
    // Ubah nilai MP dan PTS secara paksa menjadi integer
    cacheKlasemen[cacheKlasemenCount].mp = item["mp"].as<String>().toInt();
    cacheKlasemen[cacheKlasemenCount].pts = item["pts"].as<String>().toInt();
    cacheKlasemenCount++;
  }
  
  // === Analisis ===
  JsonObject a = doc["analisis"];
  cacheAnalisis.peluangJuara = a["peluang_juara"] | 0;
  cacheAnalisis.peringkatIDN = a["peringkat_idn"] | 0;
  cacheAnalisis.totalPoinAsia = a["total_poin_asia"] | 0.0;
  cacheAnalisis.slotACL = a["slot_acl"].as<String>();
  cacheAnalisis.targetPoin = a["target_poin"] | 0.0;
  cacheAnalisis.aclMD = a["acl_md"] | 0;
  cacheAnalisis.aclPoin = a["acl_poin"] | 0;
  cacheAnalisis.aclSG = a["acl_sg"] | 0;

  // === Parse ACL Klasemen ===
  cacheACLCount = 0;
  for (JsonObject item : doc["acl_klasemen"].as<JsonArray>()) {
    if (cacheACLCount >= 15) break;
    cacheACL[cacheACLCount].pos = item["pos"].as<String>().toInt();
    cacheACL[cacheACLCount].tim = item["tim"].as<String>();
    cacheACL[cacheACLCount].mp = item["mp"].as<String>().toInt();
    cacheACL[cacheACLCount].sg = item["sg"].as<String>().toInt();
    cacheACL[cacheACLCount].pts = item["pts"].as<String>().toInt();
    cacheACLCount++;

  // === Parse ACC Klasemen ===
  cacheACCCount = 0;
  for (JsonObject item : doc["acc_klasemen"].as<JsonArray>()) {
    if (cacheACCCount >= 15) break;
    cacheACC[cacheACCCount].pos = item["pos"].as<String>().toInt();
    cacheACC[cacheACCCount].tim = item["tim"].as<String>();
    cacheACC[cacheACCCount].mp = item["mp"].as<String>().toInt();
    cacheACC[cacheACCCount].sg = item["sg"].as<String>().toInt();
    cacheACC[cacheACCCount].pts = item["pts"].as<String>().toInt();
    cacheACCCount++;
  }

  Serial.printf("[API] Klasemen ACC ter-parse: %d tim\n", cacheACCCount); // Debug log
  }
  
  // === Sholat ===
  cacheSholatCount = 0;
  for (JsonObject item : doc["sholat"].as<JsonArray>()) {
    if (cacheSholatCount >= 6) break;
    cacheSholat[cacheSholatCount].nama = item["nama"].as<String>();
    cacheSholat[cacheSholatCount].waktu = item["waktu"].as<String>();
    cacheSholatCount++;
  }
  
  // === Hasil ===
  cacheHasilCount = 0;
  for (JsonObject item : doc["hasil"].as<JsonArray>()) {
    if (cacheHasilCount >= 5) break;
    cacheHasil[cacheHasilCount].tanggal = item["tanggal"].as<String>();
    cacheHasil[cacheHasilCount].pertandingan = item["pertandingan"].as<String>();
    cacheHasil[cacheHasilCount].skor = item["skor"].as<String>();
    cacheHasil[cacheHasilCount].status = item["status"].as<String>();
    cacheHasilCount++;
  }
  
  apiDataLoaded = true;
  lastApiFetch = millis();
  Serial.printf("[API] OK (v%d, updated %s)\n", cacheMeta.version, cacheMeta.updated.c_str());
}

#endif