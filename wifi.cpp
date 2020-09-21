#include "wifi.h"
#include "neo.h"

extern ConfigMap config;

WiFiManager wm;
WiFiManagerParameter mqtt_save_param;
WiFiManagerParameter mqtt_server_param;
WiFiManagerParameter mqtt_port_param;
WiFiManagerParameter mqtt_ssl_param;
WiFiManagerParameter mqtt_fingerprint_param;
WiFiManagerParameter mqtt_username_param;
WiFiManagerParameter mqtt_password_param;
WiFiManagerParameter mqtt_topic_param;

String get_param(String name) {
  /*
  Get a parameter from the WiFiManager
  */

  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void save_param_callback(){
  /*
  Load all the WifiManager params and store them into the EEPROM
  */

  Serial.println("[WIFI] save_param_callback");
  Serial.println("[WIFI] mqtt_save = " + get_param("mqtt_save"));

  if(get_param("mqtt_save").c_str()[0] == 'T')
  {  
    Serial.println("[WIFI] mqtt_server = " + get_param("mqtt_server"));
    Serial.println("[WIFI] mqtt_port = " + get_param("mqtt_port"));
    Serial.println("[WIFI] mqtt_ssl = " + get_param("mqtt_ssl"));
    Serial.println("[WIFI] mqtt_fingerprint = " + get_param("mqtt_fingerprint"));
    Serial.println("[WIFI] mqtt_username = " + get_param("mqtt_username"));
    Serial.println("[WIFI] mqtt_password = " + get_param("mqtt_password"));
    Serial.println("[WIFI] mqtt_topic = " + get_param("mqtt_topic"));

    strncpy(config.szServer, get_param("mqtt_server").c_str(), 32);
    strncpy(config.szTopic, get_param("mqtt_topic").c_str(), 32);
    if(get_param("mqtt_ssl").c_str()[0] == 'T')
      config.bSsl = true;
    else
      config.bSsl = false;
    strncpy(config.szFingerprint, get_param("mqtt_fingerprint").c_str(), 61);
    strncpy(config.szUsername, get_param("mqtt_username").c_str(), 32);
    strncpy(config.szPassword, get_param("mqtt_password").c_str(), 32);
    
    config.port = get_param("mqtt_port").toInt();
  }
  else
  {
    Serial.println("[WIFI] Not updating mqtt");
  }
  
  saveConfig();
}
void reset_wifi()
{
  Serial.println("[WIFI] Resetting");
  wm.resetSettings();
}

void setup_wifi() {
    set_neo_setup_mode();
    WiFi.mode(WIFI_STA);

    new (&mqtt_save_param)        WiFiManagerParameter("mqtt_save",        "Save MQTT Config", "T", 2,  "type=\"checkbox\"");
    new (&mqtt_server_param)      WiFiManagerParameter("mqtt_server",      "MQTT Server",      "", 20, "placeholder=\"\"");
    new (&mqtt_port_param)        WiFiManagerParameter("mqtt_port",        "MQTT Port",        "", 20, "placeholder=\"\"");
    new (&mqtt_ssl_param)         WiFiManagerParameter("mqtt_ssl",         "SSL",             "T", 2,  "type=\"checkbox\"");
    new (&mqtt_fingerprint_param) WiFiManagerParameter("mqtt_fingerprint", "MQTT Fingerprint", "", 62, "placeholder=\"\"");
    new (&mqtt_username_param)    WiFiManagerParameter("mqtt_username",    "MQTT Username",    "", 20, "placeholder=\"\"");
    new (&mqtt_password_param)    WiFiManagerParameter("mqtt_password",    "MQTT Password",    "", 20, "placeholder=\"\"");
    new (&mqtt_topic_param)       WiFiManagerParameter("mqtt_topic",       "MQTT Topic",       "", 20, "placeholder=\"\"");
  
    wm.addParameter(&mqtt_save_param);
    wm.addParameter(&mqtt_server_param);
    wm.addParameter(&mqtt_port_param);
    wm.addParameter(&mqtt_ssl_param);
    wm.addParameter(&mqtt_fingerprint_param);
    wm.addParameter(&mqtt_username_param);
    wm.addParameter(&mqtt_password_param);
    wm.addParameter(&mqtt_topic_param);
    wm.setSaveParamsCallback(save_param_callback);
    
    bool res;
    res = wm.autoConnect("FamilyLightSetup");
    
    if(!res) {
        Serial.println("[WIFI] Failed to connect, rebooting");
        ESP.restart();
    } 
    else {   
        Serial.println("[WIFI] Connected");
    }
    set_neo_off();
}
