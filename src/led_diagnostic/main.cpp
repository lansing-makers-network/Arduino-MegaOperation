#include <Arduino.h>
#include <FastLED.h>
#include "../organs.h"

// How many leds in your strip?
#define NUM_LEDS 335

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3
#define DELAY 50

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop() {
   // Turn the LED on, then pause
   for (int i=0;  i<NUM_LEDS; i++) {
    if (i==45)
      continue;
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(DELAY);
    leds[i] = CRGB::Black;
    FastLED.show();
   }

   for (int i=0;  i<NUM_LEDS; i++) {
    if (i==45)
      continue;
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(DELAY);
    leds[i] = CRGB::Black;
    FastLED.show();
   }

   for (int i=0;  i<NUM_LEDS; i++) {
    if (i==45)
      continue;
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(DELAY);
    leds[i] = CRGB::Black;
    FastLED.show();
   }


}
