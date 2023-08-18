#include <Arduino.h>
#include <FastLED.h>

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4
#define DELAY 50

// Define the array of pixels
const int NumPixels = 360;
CRGB pixels[NumPixels];

// Color wash offset -- the first pixels are
// in the nose, and behind a red lens.
const uint8_t offset = 16;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixels, NumPixels);  // GRB ordering is typical
}

const uint16_t step_forward = 17;
const uint16_t step_backward = -13;

// Time counter for color wash.
uint16_t time_forward = 0;
// Time counter for contrast pixels.
uint16_t time_backward = 0;
// Time counter for nose ring
uint16_t time_nose = 0;
// Index of reverse pixels (counts mod 16)
uint16_t reverse_index = 0;

void loop() {
    time_forward += step_forward;
    time_backward += step_backward;

    // Color the wash pixels
    uint16_t start = time_forward;
    for (int i = 0; i < NumPixels;  i++) {
      uint16_t hue = start + (i << 6);
      uint8_t hueb = hue >> 8;
      pixels[i].setHSV(hueb, 192, 128);
    }

    // Overlay the reverse
    start = time_backward;
    for (int i = reverse_index; i < NumPixels; i+=16) {
      uint16_t hue = start + (i << 6);
      uint8_t hueb = hue >> 8;
      pixels[i].setHSV(hueb, 128, 83);
    }

    FastLED.show();
}
