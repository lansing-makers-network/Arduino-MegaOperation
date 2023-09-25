#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include "pinmap.h"

#define DELAY 50

// Define the array of pixels
const uint8_t nose_pixels = 16;
const int16_t chaser_pixels = 321;
const uint16_t total_pixels = nose_pixels + chaser_pixels;
CRGB pixels[total_pixels];

// The first pixels are in the nose.  The "chaser" pixels are
// the one we animate. Pixel positions are computed from 0 -- chaser_pixels - 1,
// and the nose_offset is added when addressing the strip.

const uint8_t slow_factor = 3;

struct Layer {
  uint16_t hue;                 // Color counter
  const int16_t hue_step;       // Color counter advance
  const uint16_t run;
  const uint16_t stride;
  uint16_t offset;
  const int16_t offset_step;
  const uint8_t saturation;
  const uint8_t value;
};

const uint8_t layer_count = 4;
Layer Layers[] = {
  { 0,   17,   1,    1,   0,    0,  140,  128 },
  { 0,   11,   8,   66,   0,    5,  192,  128 },
  { 0,  -35,   3,  130,   0,  -23,  240,  240 },
  { 0,   13,   5,   66,   33,  -5,  240,  240 },
};

// Counts down modulo slow_factor to produce a slower clock
uint8_t slow_clock = slow_factor;

// Counts nose pixels, slowed down by slow_factor
uint8_t nose_counter = 0;


void setup() {
  FastLED.addLeds<WS2812, PIN_PIXEL_DATA, GRB>(pixels, total_pixels);  // GRB ordering is typical
  Serial.begin(115200);

  // Turn off nose.
  for (uint8_t i=0;  i<nose_pixels; i++) {
    pixels[i] = CRGB::Black;
  }
}


void loop() {
    // Advance .hue by .hue_step
    for (uint8_t l = 0;  l < layer_count;  l++){
      Layers[l].hue += Layers[l].hue_step;
    }

    if (--slow_clock == 0) {
      slow_clock = slow_factor;

      // Advance the nose pixel
      pixels[nose_counter] = CRGB::Black;
      nose_counter = (nose_counter-1) & 0xf;
      pixels[nose_counter] = CRGB::Red;
    }

    // Animate each layer
    for (uint8_t l=0;  l<layer_count;  l++) {
      // Advance .offset by .offset_step
      Layers[l].offset += Layers[l].offset_step;

      if (Layers[l].stride == 1) {
        // Background layer: color every pixel
        for (uint16_t i = 0;  i < chaser_pixels;  i++) {
          uint16_t hue = Layers[l].hue + (i << 6);
          uint8_t hueb = hue >> 8;
          pixels[i + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
         // pixels[i + nose_pixels] = CRGB::Black;
        }
      }
      else {
        // Overlay layer: color .run pixels out of every .stride with offset
        uint16_t offset = (Layers[l].offset >> 6) % Layers[l].stride;
        for (uint16_t start = offset;  start < chaser_pixels;  start += Layers[l].stride) {
          for (uint16_t r = 0;  r < Layers[l].run;  r++) {
            uint16_t hue = Layers[l].hue + ((start+r) << 6);
            uint8_t hueb = hue >> 8;
            uint16_t pos = start + r;
            if (pos > chaser_pixels) {
              break;
            }
            pixels[pos + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
          }
        }
      }
    }

    FastLED.show();
}
