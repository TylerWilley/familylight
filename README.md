# familylight

The Family light is a push button light that syncs it's color over MQTT to other Family lights.

e.g. I press my light and it turns red. Everyone else's light also turns red, so they know that I have pressed it.

## Materials

Not everything is _required_, but you may need to figure it out as you go.

Per light:
- NodeMCU
- 1000uF Capacitor
- Veraboard or circuit board
- Small guage wire for connections
- Female micro USB breakout board
- Some female header pins for the NodeMCU
- A NeoPixel x7 jewel
- An Engerizer tap light

Tools:
- Soldering iron
- Dremel
- Hot Glue Gun

## Construction

### Taplight Prep
- Disassemble the taplight
- Hollow out the inside with the dremel
- Cut a slot in the side of the taplight's ring for the micro USB port

### Circuit board prep
- Solder the NodeMCU headers to the side of your board
- Solder the NeoPixel jewel to the center of your board
- Connect the taplight's switch to pins GND and D5
- Connect the NeoPixel's PWR to the NodeMCU's VIN
- Connect the NeoPixel's GND to the NodeMCU's GND
- Connect the NeoPixel's Data In to the NodeMCU's D2
- Connect the micro USB breakout's VCC to the NodeMCU's VIN
- Connect the micro USB breakout's GND to the NodeMCU's GND

### Mounting the Circuit Board
- Place the Circuit board in the center of the taplight
- Replace the taplight's switch into it's holder
- Align the micro USB port with the opening in the taplight's ring
- Hot glue it all into place

### Completion
- Flash the NodeMCU
- Slot the NodeMCU into the headers
- Reassemble the tap light

## Usage

On startup it will turn rainbow, and open a wifi access point FamilyLightSetup.

Connect to the access point and a captive portal will bring you to the configuration page.

Enter wifi credentials and an MQTT server (with SSL, username/password, etc as needed)

The light will then turn off.

By default the light's color will be "125,125,0" (yellow).

To change your color, double press the light and it will begin rotating through all hues. Single press to select the given color.

If you need to reconfigure the light, pressing 5 times will reset the light and re-enter configuration mode. The light will also automatically re-enter configuration mode if it cannot connect to wifi.

Finally, if you do not want to reconfigure MQTT, you do not need to check the Save MQTT checkbox.