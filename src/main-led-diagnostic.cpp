#include <Arduino.h>
#include <FastLED.h>

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4
#define DELAY 50

// Define the array of pixels
const uint16_t total_pixels = 360;
CRGB pixels[total_pixels];

// The first pixels are in the nose.
const uint8_t nose_pixels = 16;
const uint16_t chaser_pixels = total_pixels - nose_pixels;
const uint16_t nose_factor = 8;

struct Layer {
  uint16_t time;
  const int16_t step;
  const uint16_t run;
  const uint16_t stride;
  int16_t offset;
  const int16_t advance;
  const uint8_t saturation;
  const uint8_t value;
};

const uint8_t layer_count = 2;
Layer Layers[] = {
  { 0,  17,  1,   1,  0,    0,  192,  128 },
  { 0,  11,  5,  17,  0,    3,  128,  128 },
  { 0, -28,  2,   8,  0,    5,   50,   32 },
  { 0, -93,  1,  22,  0,  -13,  240,  240 }
};

// Counts mod 16
uint16_t mod_counter = 0;

// Counts down mod nose_factor;
uint16_t nose_counter = nose_factor;

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixels, total_pixels);  // GRB ordering is typical
  Serial.begin(115200);

  for (uint8_t i=0;  i<nose_pixels; i++) {
    pixels[i] = CRGB::Black;
  }
}


void loop() {
    // Advance .time by .step
    for (uint8_t l = 0;  l < layer_count;  l++){
      Layers[l].time += Layers[l].step;
    }

    if (--nose_counter == 0) {
      nose_counter = nose_factor;
      
      pixels[mod_counter] = CRGB::Black;
      mod_counter = (mod_counter-1) & 0xf;
      pixels[mod_counter] = CRGB::Red;
      //Serial.println(mod_counter);

      // Advance .offset by .advance
      for (uint8_t l=0;  l < layer_count;  l++) {
        Layers[l].offset += Layers[l].advance;
      }
    }

    for (uint8_t l=0;  l<layer_count;  l++) {
      if (Layers[l].stride == 1) {
        // Background layer: color every pixel
        for (uint16_t i = 0;  i < chaser_pixels;  i++) {
          uint16_t hue = Layers[l].time + (i << 6);
          uint8_t hueb = hue >> 8;
          pixels[i + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
        }
      }
      else {
        // Overlay layer: color .run pixels out of every .stride with offset
        int16_t offset = (Layers[l].offset >> 6) % Layers[l].stride;
        Serial.print(l);
        Serial.print(": ");
        for (uint8_t k = 0;  k < l;  k++) { Serial.print("  "); }
        Serial.println(offset);
        for (uint16_t i = 0;  i < chaser_pixels;  i+=Layers[l].stride) {
          for (uint16_t r = 0;  r < Layers[l].run;  r++) {
            uint16_t hue = Layers[l].time + ((i+r) << 6);
            uint8_t hueb = hue >> 8;
            int16_t pos = i + r + offset;
            if (pos > chaser_pixels) { break; }
            if (l == 1) {
              Serial.print(pos);
              Serial.print(' ');
            }
            pixels[pos + nose_pixels].setHSV(hueb, Layers[l].saturation, Layers[l].value);
          }
        }
        Serial.println();
      }
    }

    FastLED.show();
}
