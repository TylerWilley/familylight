#ifndef NEO_H
#define NEO_H
#include <Adafruit_NeoPixel.h>
void setup_neo();
void set_neo_color(int r, int g, int b);
void set_neo_off();
void select_current_color();
void start_color_select();
void loop_neo();
void set_neo_setup_mode();
#endif
