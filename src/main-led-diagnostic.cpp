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
const uint8_t nose_offset = 16;

const uint16_t step_forward = 71;
const uint16_t step_backward = -28;
const uint16_t reverse_factor = 8;

// Time counter for color wash.
uint16_t time_forward = 0;
// Time counter for contrast pixels.
uint16_t time_backward = 0;
// Time counter for nose ring
uint16_t time_nose = 0;
// Index of reverse pixels (counts mod 16)
uint16_t reverse_offset = 0;
uint16_t reverse_counter = reverse_factor;


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(pixels, NumPixels);  // GRB ordering is typical

  Serial.begin(115200);

  for (uint8_t i=0;  i<nose_offset; i++) {
    pixels[i] = CRGB::Black;
  }
}



void loop() {
    time_forward += step_forward;
    time_backward += step_backward;

    if (--reverse_counter == 0) {
      reverse_counter = reverse_factor;
      
      pixels[reverse_offset] = CRGB::Black;
      reverse_offset = (reverse_offset-1) & 0xf;
      pixels[reverse_offset] = CRGB::Red;
      //Serial.println(reverse_offset);
    }

    // Color the wash pixels
    for (int i = nose_offset; i < NumPixels;  i++) {
      uint16_t hue = time_forward + (i << 6);
      uint8_t hueb = hue >> 8;
      pixels[i].setHSV(hueb, 192, 128);
    }

    // Overlay the reverse
    for (int i = reverse_offset; i < NumPixels; i+=16) {
      uint16_t hue = time_backward + (i << 6);
      uint8_t hueb = hue >> 8;
      pixels[i].setHSV(hueb, 255, 255);
    }

    FastLED.show();
}
