#include "storage.h"

ConfigMap config;

bool loadConfig() {
  memset(&config, 0, sizeof(ConfigMap));
  strncpy(config.szColor, "125,125,0", 32);

  EEPROM.begin(sizeof(ConfigMap));

  uint8_t data[sizeof(ConfigMap)];
  uint16_t *tmp = (uint16_t *)data;

  int addr = 0;
  for(int i = 0; i < sizeof(ConfigMap); i++, addr++)
  {
    data[i] = EEPROM.read( addr );
  }

  Serial.print("Magic:");
  Serial.println(tmp[0]);

  if(tmp[0] != CONFIG_VERSION)
  {
    return false; // mismatch magic
  }
  
  memcpy(&config, data, sizeof(ConfigMap) );
  Serial.print("[Config] version:");
  Serial.println(config.version);
  Serial.print("[Config] server:");
  Serial.println(config.szServer);
  Serial.print("[Config] port:");
  Serial.println(config.port);
  Serial.print("[Config] ssl:");
  if(config.bSsl)
    Serial.println("True");
  else
    Serial.println("False");
  Serial.print("[Config] fingerprint:");
  Serial.println(config.szFingerprint);
  Serial.print("[Config] username:");
  Serial.println(config.szUsername);
  Serial.print("[Config] password:");
  Serial.println(config.szPassword);
  Serial.print("[Config] topic:");
  Serial.println(config.szTopic);
}

void saveConfig() {
  config.version = CONFIG_VERSION;
  
  uint16_t addr = 0;
  uint8_t *pData = (uint8_t *)&config;
  for(int i = 0; i < sizeof(ConfigMap); i++, addr++)
  {
    EEPROM.write(addr, pData[i] );
  }
  EEPROM.commit();
}

void reset_config() {
  config.version = RESET_CONFIG;
  
  uint16_t addr = 0;
  uint8_t *pData = (uint8_t *)&config;
  for(int i = 0; i < sizeof(ConfigMap); i++, addr++)
  {
    EEPROM.write(addr, pData[i] );
  }
  EEPROM.commit();
  ESP.restart();
}
