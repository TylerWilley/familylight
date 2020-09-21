#ifndef STORAGE_H
#define STORAGE_H
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define RESET_CONFIG 0
#define CONFIG_VERSION 128

struct ConfigMap
{
  uint16_t version;
  char     szServer[32];
  uint16_t port;
  bool     bSsl;
  char     szFingerprint[61];
  char     szUsername[32];
  char     szPassword[32];
  char     szTopic[32];
  char     szColor[32];
  bool     bReset;
};

bool loadConfig();
void saveConfig();
void reset_config();
#endif
