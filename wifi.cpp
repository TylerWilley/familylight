#include "wifi.h"
#include "neo.h"

extern ConfigMap config;

WiFiManager wm;
WiFiManagerParameter mqtt_server_param;
WiFiManagerParameter mqtt_port_param;
WiFiManagerParameter mqtt_ssl_param;
WiFiManagerParameter mqtt_fingerprint_param;
WiFiManagerParameter mqtt_username_param;
WiFiManagerParameter mqtt_password_param;
WiFiManagerParameter mqtt_topic_param;

String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM mqtt_server = " + getParam("mqtt_server"));
  Serial.println("PARAM mqtt_port = " + getParam("mqtt_port"));
  Serial.println("PARAM mqtt_ssl = " + getParam("mqtt_ssl"));
  Serial.println("PARAM mqtt_fingerprint = " + getParam("mqtt_fingerprint"));
  Serial.println("PARAM mqtt_username = " + getParam("mqtt_username"));
  Serial.println("PARAM mqtt_password = " + getParam("mqtt_password"));
  Serial.println("PARAM mqtt_topic = " + getParam("mqtt_topic"));

  strncpy(config.szServer, getParam("mqtt_server").c_str(), 32);
  strncpy(config.szTopic, getParam("mqtt_topic").c_str(), 32);
  if(getParam("mqtt_ssl").c_str()[0] == 'T')
    config.bSsl = true;
  else
    config.bSsl = false;
  strncpy(config.szFingerprint, getParam("mqtt_fingerprint").c_str(), 61);
  strncpy(config.szUsername, getParam("mqtt_username").c_str(), 32);
  strncpy(config.szPassword, getParam("mqtt_password").c_str(), 32);
  
  config.port = getParam("mqtt_port").toInt();
  
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

    new (&mqtt_server_param)      WiFiManagerParameter("mqtt_server",      "MQTT Server",      "", 20, "placeholder=\"\"");
    new (&mqtt_port_param)        WiFiManagerParameter("mqtt_port",        "MQTT Port",        "", 20, "placeholder=\"\"");
    new (&mqtt_ssl_param)         WiFiManagerParameter("mqtt_ssl",         "SSL",             "T", 2,  "type=\"checkbox\"");
    new (&mqtt_fingerprint_param) WiFiManagerParameter("mqtt_fingerprint", "MQTT Fingerprint", "", 62, "placeholder=\"\"");
    new (&mqtt_username_param)    WiFiManagerParameter("mqtt_username",    "MQTT Username",    "", 20, "placeholder=\"\"");
    new (&mqtt_password_param)    WiFiManagerParameter("mqtt_password",    "MQTT Password",    "", 20, "placeholder=\"\"");
    new (&mqtt_topic_param)       WiFiManagerParameter("mqtt_topic",       "MQTT Topic",       "", 20, "placeholder=\"\"");
  
    wm.addParameter(&mqtt_server_param);
    wm.addParameter(&mqtt_port_param);
    wm.addParameter(&mqtt_ssl_param);
    wm.addParameter(&mqtt_fingerprint_param);
    wm.addParameter(&mqtt_username_param);
    wm.addParameter(&mqtt_password_param);
    wm.addParameter(&mqtt_topic_param);
    wm.setSaveParamsCallback(saveParamCallback);
    
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
