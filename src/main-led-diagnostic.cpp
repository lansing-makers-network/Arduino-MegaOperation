#include <Arduino.h>
#include <FastLED.h>

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4
#define DELAY 50

// Define the array of pixels
const uint8_t nose_pixels = 16;
const int16_t chaser_pixels = 344;
const uint16_t total_pixels = nose_pixels + chaser_pixels;
CRGB pixels[total_pixels];

// The first pixels are in the nose.  The "chaser" pixels are
// the one we animate. Pixel positions are computed from 0 -- chaser_pixels - 1,
// and the nose_offset is added when addressing the strip.

const uint8_t slow_factor = 5;

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
  { 0,      17,   1,   1,   0,    0,  100,   96 },
  { 0,      11,   4,  67,   0,    3,  128,  128 },
  { 21845, -28,   8,  33,   0,    5,  192,  128 },
  { 43690, -45,   3,  130,  0,  -23,  240,  240 }
};

// Counts down mod slow_factor to produce a slower clock
uint8_t slow_clock = slow_factor;

// Counts mod 16, slowed down by slow_factor
uint8_t nose_counter = 0;


void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixels, total_pixels);  // GRB ordering is typical
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
      //Serial.println(nose_counter);

      // Advance .offset by .offset_step
      for (uint8_t l=0;  l < layer_count;  l++) {
        Layers[l].offset += Layers[l].offset_step;
      }
    }

    // Animate each layer
    for (uint8_t l=0;  l<layer_count;  l++) {
      if (Layers[l].stride == 1) {
        // Background layer: color every pixel
        for (uint16_t i = 0;  i < chaser_pixels;  i++) {
          uint16_t hue = Layers[l].hue + (i << 6);
          uint8_t hueb = hue >> 8;
          pixels[i + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
        }
      }
      else {
        // Overlay layer: color .run pixels out of every .stride with offset
        uint16_t offset = (Layers[l].offset >> 6) % Layers[l].stride;

        for (uint16_t i = 0;  i < chaser_pixels;  i+=Layers[l].stride) {
          for (uint8_t r = 0;  r < Layers[l].run;  r++) {
            uint16_t hue = Layers[l].hue + ((i+r) << 6);
            uint8_t hueb = hue >> 8;
            uint16_t pos = offset + i + r;
            if (pos > chaser_pixels) { pos -= chaser_pixels; }
            if (pos < 0) {
              Serial.print("pos < 0: ");
              Serial.println(pos);
            }
            pixels[pos + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
          }
        }
        Serial.println();
      }
    }

    FastLED.show();
}
