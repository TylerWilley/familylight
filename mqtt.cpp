#include "mqtt.h"
#include "storage.h"
#include "neo.h"

WiFiClient espClient;

WiFiClientSecure espClientSsl;

extern ConfigMap config;


PubSubClient *client = NULL;

String clientId = "";
uint32_t next_connect_attempt = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  /*
  Called on MQTT message recieved (color update)
  */
  Serial.print(F("[MQTT] Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  char payload_str[40];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if(i < 40)
      payload_str[i] = (char)payload[i];
  }
  payload_str[39] = 0;
  Serial.println();
  if(strncmp(topic, "familylight/color", 17) == 0)
  {
    Serial.println("[MQTT] Got color update");
    client->publish("familylight/checkin", "recieved");
  }
  else
  {
    Serial.println("[MQTT] Other light acked, blink");
  }
  
  int rgb[3];
  int idx = 0;

  char* command = strtok(payload_str, ",");
  while (command != 0)
  {
    Serial.print("p[MQTT] art:");
    Serial.println(command);
    rgb[idx] = atoi(command);
    idx += 1;
    command = strtok(0, ",");
  }

  Serial.print("[MQTT] Setting color:");
  Serial.print(rgb[0]);
  Serial.print(",");
  Serial.print(rgb[1]);
  Serial.print(",");
  Serial.println(rgb[2]);
  set_neo_color(rgb[0], rgb[1], rgb[2]);
}

void send_color(String color)
{
  /*
  Publish a color update to the neopixel topic
  */
  client->publish("familylight/color", color.c_str());
}

void setup_mqtt() {
  /*
  Initialize MQTT using SSL or non-SSL based on wifi manager settings
  */
  Serial.println(F("[MQTT] Setting up"));
  clientId += String("family_light_");
  clientId += String(random(0xffff), HEX);
  Serial.print(F("[MQTT] Client ID: "));
  Serial.println(clientId);

  if(config.bSsl)
  {
    Serial.print(F("[MQTT] SSL: "));
    if(config.szFingerprint[0] == '\0')
    {
      Serial.println(F("[MQTT] Insecure"));
      espClientSsl.setInsecure(); // No fingerprint, no validation
    }
    else
    {
      Serial.print(F("[MQTT] Fingerprinted:"));
      Serial.println(config.szFingerprint);
      espClientSsl.setFingerprint(config.szFingerprint);
    }
    client = new PubSubClient(espClientSsl);
  }
  else
  {
    Serial.println(F("[MQTT] No-SSL"));
    client = new PubSubClient(espClient);
  }
  Serial.print(F("[MQTT] Connecting: "));
  Serial.print(config.szServer);
  Serial.print(':');
  Serial.println(config.port);
  client->setServer(config.szServer, config.port);
  client->setCallback(callback);
}

bool loop_mqtt() {
  /*
  Reconnect MQTT if disconnected. Attempt every 5s
  If connected, run the client loop
  */
  if (!client->connected() && millis() > next_connect_attempt) {
    set_neo_setup_mode();
    Serial.println(F("[MQTT] Disconnected. Connecting..."));
    bool res = false;
    if(config.szUsername[0] != '\0')
    {
      Serial.print("[MQTT] Connecting with username");
      res = client->connect(clientId.c_str(), config.szUsername, config.szPassword);
    }
    else
    {
      Serial.print("[MQTT] Connecting anonymously");
      res = client->connect(clientId.c_str());
    }
    if (res) {
      Serial.println(F("[MQTT] Connected"));
      client->subscribe("familylight/color");
      //client.subscribe("familylight/checkin");
      set_neo_off();
    } else {
      Serial.print(F("[MQTT] Connection failed, rc="));
      Serial.print(client->state());
      Serial.println(F(" try again in 5 seconds"));
      next_connect_attempt = millis() + 5000;
      return false;
    }
  }
  
  client->loop();
  
  return true;
}
