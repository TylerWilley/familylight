#include "storage.h"

ConfigMap config;

bool loadConfig() {
  /*
  Load config from EEPROM
  */
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

  Serial.print("[Config] Magic:");
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
    Serial.println(" True");
  else
    Serial.println(" False");
  Serial.print("[Config] fingerprint:");
  Serial.println(config.szFingerprint);
  Serial.print("[Config] username:");
  Serial.println(config.szUsername);
  Serial.print("[Config] password:");
  Serial.println(config.szPassword);
  Serial.print("[Config] topic:");
  Serial.println(config.szTopic);

  
  Serial.print("[Config] reset:");
  if(config.bReset)
    Serial.println(" True");
  else
    Serial.println(" False");
}

void saveConfig() {
  /*
  Write config to EEPROM
  */
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
  /*
  Trigger setup mode
  */
  config.bReset = true;
  
  uint16_t addr = 0;
  uint8_t *pData = (uint8_t *)&config;
  for(int i = 0; i < sizeof(ConfigMap); i++, addr++)
  {
    EEPROM.write(addr, pData[i] );
  }
  EEPROM.commit();
  ESP.restart();
}
