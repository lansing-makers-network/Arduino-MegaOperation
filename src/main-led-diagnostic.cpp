#include <Arduino.h>
#include <FastLED.h>
#include "innards.h"

// How many leds in your strip?
#define NUM_LEDS 335

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 2
#define DELAY 50

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void fill(Organ org, CRGB::HTMLColorCode color, unsigned long delay_ms) {
  for (int i=org.start; i < org.start+org.len;  i++) {
    if (i==45) continue;
    leds[i] = color;
    FastLED.show();
    delay(delay_ms);
  }
}

void blink(Organ org, CRGB::HTMLColorCode color, unsigned long delay_ms) {
  for (int i=org.start; i < org.start+org.len;  i++) {
    if (i==45) continue;
    leds[i] = color;
    FastLED.show();
    delay(delay_ms);
    leds[i] = CRGB::Black;
  }
}

void goround(CRGB::HTMLColorCode color) {
    for (int k=0;  k<ORGAN_COUNT;  k++) {
      blink(BodyMap[k], color, 50);
      fill(BodyMap[k], color, 25);
      fill(BodyMap[k], CRGB::Black, 25);
    }
}

void loop() {
  goround(CRGB::Red);
  goround(CRGB::Green);
  goround(CRGB::Blue);
  goround(CRGB::Yellow);
  goround(CRGB::Purple);
  goround(CRGB::Turquoise);
  goround(CRGB::White);
}
