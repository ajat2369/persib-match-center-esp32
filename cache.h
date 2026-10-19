#ifndef CACHE_H
#define CACHE_H

#include <Arduino.h>

struct MetaInfo {
  int version;
  String updated;
  String source;
  int nextReloadSec;
};

struct NextMatch {
  String opponent;
  String date;
  String time;
  String venue;
  String competition;
};

struct DashboardData {
  int statBRI, statACL, statShopee, statTotal;
  NextMatch nextMatch;
  String form;
};

struct JadwalItem {
  String tanggal, lawan, kompetisi;
};

struct KlasemenItem {
  int pos, mp, pts;
  String klub;
};

struct AnalisisData {
  int peluangJuara;
  int peringkatIDN;
  float totalPoinAsia;
  String slotACL;
  float targetPoin;
  int aclMD, aclPoin, aclSG;
};

struct SholatItem {
  String nama, waktu;
};

struct HasilItem {
  String tanggal, pertandingan, skor, status;
};

// Global cache
MetaInfo cacheMeta;
DashboardData cacheDashboard;
JadwalItem cacheJadwal[5];
int cacheJadwalCount = 0;
KlasemenItem cacheKlasemen[8];
int cacheKlasemenCount = 0;
AnalisisData cacheAnalisis;
SholatItem cacheSholat[6];
int cacheSholatCount = 0;
HasilItem cacheHasil[5];
int cacheHasilCount = 0;

// Status
bool apiDataLoaded = false;
unsigned long lastApiFetch = 0;

// Tambahkan di dalam cache.h sebelum #endif
String nextPrayerName = "";
int nextPrayerMinutes = 0;

// Tambahkan di dalam cache.h sebelum #endif
struct GrupItem {
  int pos;
  String tim;
  int mp;
  int sg;
  int pts;
};

GrupItem cacheACL[15];
int cacheACLCount = 0;

GrupItem cacheACC[15];
int cacheACCCount = 0;

#endif