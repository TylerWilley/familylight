#include "button.h"
#include "mqtt.h"
#include "state.h"
#include "neo.h"
#include "storage.h"
#include "wifi.h"

extern enum State state;
extern ConfigMap config;

#define INPUT_PIN D5

int last_button_state = 0;
int press_count = 0;
uint32_t last_button_press = 0;
bool momentary_depressed = false;

const bool momentary_button = false; 


void setup_button()
{
  /*
  Bind the button pin and load it's initial state
  */
  pinMode(INPUT_PIN, INPUT_PULLUP);
  last_button_state = digitalRead(INPUT_PIN);
}

void loop_button()
{
  /*
  Check button state, debounce only allowing changes every 100ms
  This sketch is designed to use a latching button because that's
  what my Energizer tap lights came with. You'll need to modify
  momentary_button to 'true' if your buttons are momentary.
  */
  int button_state = digitalRead(INPUT_PIN);

  /*
  Toggle Button press_count handler
  */ 
  if(!momentary_button && button_state != last_button_state && millis() - last_button_press > 100) {
    Serial.print("[BUTTON] ");
    Serial.print(millis() - last_button_press);
    last_button_press = millis();
    press_count += 1;
    Serial.print(" Button moved from ");
    Serial.print(last_button_state);
    Serial.print(" -> ");
    Serial.print(button_state);
    Serial.print(" current count: ");
    Serial.println(press_count);
    last_button_state = button_state;
  }

  /*
  Momentary button press_count handler 
  */
  else if (momentary_button && !momentary_depressed && button_state != last_button_state && millis() - last_button_press > 100) {
  Serial.print("[BUTTON] ");
    Serial.print(" Button moved from ");
    Serial.print(last_button_state);
    Serial.print(" -> ");
    Serial.print(button_state);
    momentary_depressed = true;
    last_button_state = button_state;
  }
  /*
  Don't need to check if it's been 100 miliseconds, since momentary_depressed can't be true without checking this in the above conditional.
  */
  else if (momentary_button && momentary_depressed && button_state != last_button_state) {
    Serial.print("[BUTTON] ");
    Serial.print(millis() - last_button_press);
    last_button_press = millis();
    press_count += 1;
    Serial.print(" Button moved from ");
    Serial.print(last_button_state);
    Serial.print(" -> ");
    Serial.print(button_state);
    Serial.print(" current count: ");
    Serial.println(press_count);
    momentary_depressed = false;
    last_button_state = button_state;
  }

  if(press_count && millis() - last_button_press > 500)
  {
    switch(press_count) {
      case 0:
        break;
      case 1:
          Serial.println("[BUTTON] Singlepress");
          if(state == NORMAL)
            send_color(config.szColor);
          if(state == COLOR_SELECT)
            select_current_color();
          break;
      case 2:
          Serial.println("[BUTTON] Doublepress");
          start_color_select();
          break;
      case 3:
      case 4:
          break;
      default:
          Serial.println("[BUTTON] 5 or more presses presses");
          reset_config();
        
    }
    press_count = 0;
  }
}
