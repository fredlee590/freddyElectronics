#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 14

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 4
#define DELTA 1
#define UPPER 255 - DELTA
#define LOWER 0 + DELTA

// Define the array of leds
CRGB leds[NUM_LEDS];
int dir = DELTA;
int hue = LOWER;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  
  FastLED.show();
}

void loop() {
  if(hue > UPPER || hue < LOWER)
  {
    dir = -dir;
  }
  
  hue += dir;

  /*
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue, 255, 255);
  }
  */
  leds[0] = CHSV(hue, 255, 127);
  
  FastLED.show();
  delay(100);
}
