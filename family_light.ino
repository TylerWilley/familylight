#include <ESP8266WiFi.h>
#include "storage.h"
#include "mqtt.h"
#include "wifi.h"
#include "neo.h"
#include "button.h"
#include "state.h"

extern enum State state;
extern ConfigMap config;

void setup() {
  /*
  Initialize everything, if we need to setup
  run wifi setup to connect to wifi and get
  mqtt configuration
  */
  state = NORMAL;
  setup_neo();
  Serial.begin(115200);
  if(!loadConfig() || config.bReset)
  {
    config.bReset = false;
    reset_wifi();
  }
  setup_wifi();
  setup_mqtt();
  setup_button();
}

void loop() {
  /*
  Main loop, handle mqtt, then neo pixel animations, then button handling
  loop functions should not block (e.g. use delay) for smooth animations.
  */
  loop_mqtt();
  loop_neo();
  loop_button();
}
