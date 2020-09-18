#ifndef MQTT_H
#define MQTT_H
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
void setup_mqtt();
bool loop_mqtt();
void send_color(String color);
#endif
