#include <FastLED.h>
#include <pixeltypes.h>

#define LED_PIN     2

// LEFT: 22 + FRONT: 14 + RIGHT: 21
// Total LEDs: 57
#define NUM_LEDS    57
#define BRIGHTNESS 50
#define UPDATES_PER_SECOND 10

CRGB leds[NUM_LEDS];

String input="";
String currentLight;
bool sameColour;

int i;

CRGB colour;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  i = 0;
  colour = CRGB::Green;
  
  fill_solid(leds, NUM_LEDS, colour);
  currentLight = "GREEN";
  delay(1000);
  
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('.');
    //Serial.println(Serial.readString());
    //Serial.println("HELLO");
    if (input == currentLight) {
      sameColour = true;
    } else {
      sameColour = false;
    }
    
    if (input == "RED" && !sameColour) {
      // RED: Emergency Stop
      
      currentLight = "RED";
      colour = CRGB::Red;      
      
    } else if (input == "GREEN" && !sameColour) {
    // GREEN: Waiting for job
      
      colour = CRGB::Green;
      currentLight = "GREEN";
    
    } else if (input == "BLUE" && !sameColour) {
      // Cyan: Drives to destination
      colour = CRGB::Blue;
      currentLight = "BLUE";
            
    } else if (input == "PURPLE" && !sameColour) {

      colour = CRGB::Purple;
      currentLight = "PURPLE";
      
    } else if (input == "OFF" && !sameColour) {
      currentLight = "OFF";
      colour = CRGB::Black;
      
    } else if (input == "WHITE" && !sameColour) {  
      colour = CRGB::White;
      currentLight = "WHITE";
    } 
    fill_solid(leds, NUM_LEDS, colour);
    FastLED.show();
  }

  // Control of LEDs
  LED_sequence();
  
  FastLED.show();
  delay(1000/UPDATES_PER_SECOND);
}

void LED_sequence(){
  leds[21] = CRGB::Black;
  leds[i] = colour;
  leds[i+1] = colour;
  leds[i+2] = colour;
  leds[56-i] = colour;
  leds[55-i] = colour;
  leds[54-i] = colour;
  if (i != 0){
    leds[i-1] = CRGB::Black;
    leds[57-i] = CRGB::Black;
  }
  if (i == 21){
    i = 0;
  }
  else{
    i++;
  }
}
