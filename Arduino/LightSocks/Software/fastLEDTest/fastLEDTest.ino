#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 4

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 18
#define CLOCK_PIN 17

#define X_PIN 14
#define Y_PIN 15
#define Z_PIN 16

// Define the array of leds
CRGB leds[NUM_LEDS];
int dotLED = 0;

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  	  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

      Serial.begin(9600);
}

int findMinVal(int vals[])
{
  int minVal = 1024;
  for(unsigned int i = 0; i < sizeof(vals)/sizeof(int); i++)
  {
    if(vals[i] < minVal)
    {
      minVal = vals[i];
    }
  }

  return minVal;
}

void loop() { 
  // Turn the LED on, then pause
  /*
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  */

  int xSensor = analogRead(X_PIN);
  int ySensor = analogRead(Y_PIN);
  int zSensor = analogRead(Z_PIN);
  CRGB dotColor;

  int normXsensor = abs(xSensor - 512);
  int normYsensor = abs(ySensor - 512);
  int normZsensor = abs(zSensor - 512);

  /*
  if(xSensor > ySensor && xSensor > zSensor)
  {
    dotColor = CRGB::Red;
  }
  else if(ySensor > xSensor && ySensor > zSensor)
  {
    dotColor = CRGB::Green;
  }
  else if(zSensor > xSensor && zSensor > ySensor)
  {
    dotColor = CRGB::Blue;
  }
  else
  {
    dotColor = CRGB::White;
  }
  */

  int normAvgVal = abs((normXsensor + normYsensor + normZsensor) / 3);
  if(normXsensor < 15 && normYsensor < 15 && normZsensor < 15)
  {
    dotColor = CRGB::Black;
  }
  else if(normAvgVal < 10)
  {
    dotColor = CRGB::Blue;
  }
  else if(normAvgVal < 20)
  {
    dotColor = CRGB::Green;
  }
  else if(normAvgVal < 30)
  {
    dotColor = CRGB::Red;
  }
  else
  {
    dotColor = CRGB::White;
  }

  Serial.printf("x = %d y = %d z = %d\n", xSensor, ySensor, zSensor);

  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(i == dotLED)
    {
      leds[i] = dotColor;
    }
    else
    {
      leds[i] = CRGB::Green;
    }
  }
  if(++dotLED == NUM_LEDS)
  {
    dotLED = 0;
  }
  FastLED.show();
  delay(200);
  // Now turn the LED off, then pause
  /*
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::DarkGreen;
  }
  FastLED.show();
  delay(200);
  */
}
