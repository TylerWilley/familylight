#include "neo.h"
#include "state.h"
#include "storage.h"

#define LED_PIN    D2
#define LED_COUNT 7

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
extern enum State state;
extern ConfigMap config;
uint32_t current_select_color = 0;

void setup_neo() {
    strip.begin();
    strip.show();
    strip.setBrightness(20);
}

void set_neo_setup_mode()
{
    for(int i = 0; i < LED_COUNT; i++)
    {
      uint32_t color = strip.gamma32(strip.ColorHSV(9362 * i));
      strip.setPixelColor(i, color);
    }
    strip.show();
}

void set_neo_off()
{
  for(int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, 0, 0, 0);
  strip.show();
}

void set_neo_color(int r, int g, int b)
{
  uint32_t color = strip.Color(r, g, b);
  for(int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, color);
   strip.show();
}

void loop_neo()
{
  if(state == COLOR_SELECT)
  {
    current_select_color += 1;
    uint32_t color = strip.gamma32(strip.ColorHSV(current_select_color));
    for(int i = 0; i < LED_COUNT; i++)
      strip.setPixelColor(i, color);
    strip.show();
  }
}

void start_color_select()
{
  state = COLOR_SELECT;
}

void select_current_color()
{
  uint32_t color = strip.getPixelColor(0);
  state = NORMAL;
  uint8_t r = color >> 16;
  uint8_t g = color >> 8;
  uint8_t b = color;
  Serial.print("Color selected: r");
  Serial.print(r);
  Serial.print(" g");
  Serial.print(g);
  Serial.print(" b");
  Serial.println(b);
  snprintf(config.szColor, 32, "%d,%d,%d", r, g, b);
  saveConfig();
}
