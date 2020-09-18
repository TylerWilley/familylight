#include <ESP8266WiFi.h>
#include "storage.h"
#include "mqtt.h"
#include "wifi.h"
#include "neo.h"
#include "button.h"
#include "state.h"

extern enum State state;

void setup() {
    state = NORMAL;
    setup_neo();
    Serial.begin(115200);
    if(!loadConfig())
      reset_wifi();
    setup_wifi();
    setup_mqtt();
    setup_button();
}

void loop() {
    loop_mqtt();
    loop_neo();
    loop_button();
}
