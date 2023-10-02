#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <Adafruit_VS1053.h>
#include <NoseErrors.h>
#include "pinmap.h"
#include "innards.h"

#define DELAY 50

// I2C devices
const uint8_t TOUCH_ADDR = 0x5A;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 touch = Adafruit_MPR121();

// Audio player
Adafruit_VS1053_FilePlayer audioPlayer = Adafruit_VS1053_FilePlayer(PIN_AUDIO_RESET, PIN_AUDIO_MCS, PIN_AUDIO_DCS, PIN_AUDIO_DREQ, PIN_AUDIO_SDCARD_CS);

// Define the array of pixels
const uint8_t nose_pixels = NOSE.len;
const int16_t chaser_pixels = 84; // ORGAN_PIXEL_COUNT;
const uint16_t total_pixels = 100; //TOTAL_PIXEL_COUNT;
CRGB pixels[total_pixels];

// The first pixels are in the nose.  The "chaser" pixels are+
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

// Colors
const CRGB TOUCHED_COLOR = CRGB::Red;

// Updated by touch sensor interrupt handler
volatile uint16_t touchstate = 0;

// Touch sensor interrupt handler
void touchEvent()
{
  //  Serial.print("Interupt!");
  // Keeps track of the last pins touched
  // so we know when buttons are 'released'
  //static uint16_t lasttouched = 0;
  //static uint16_t currtouched = 0;

  // Get the currently touched pads
  touchstate = touch.touched();
  //Serial.print("0x");
  //if (touchstate < 0x100) Serial.print('0');
  //if (touchstate < 0x10) Serial.print('0');
  //Serial.println(touchstate, HEX);

  // Set nose according to touchstate
  for (int i=0;  i<12;  i++) {
    if (touchstate & (1 << i)) {
      pixels[i] = CRGB::Red;
    }
    else {
      pixels[i] = CRGB::Black;
    }
  }
}

void setOrganColor(int organ, CRGB color) {
    uint16_t offset = BodyMap[organ].start;
    for (int k=0;  k<BodyMap[organ].len;  k++) {
      pixels[k+offset] = color;
  }
}

// Indicate a status using the nose. Light up consecutive pixels.
void setNoseStatus(int8_t val, bool solid) {

  for (int8_t i=0;  i<val-1;  i++) {
    if (solid) {
      pixels[i] = CRGB::Red;
    }
    else {
      pixels[i] = CRGB::Black;
    }
  }
  pixels[val-1] = CRGB::Red;

  for (int8_t i=val;  i< 16; i++) {
    pixels[i] = CRGB::Black;
  }
  FastLED.show();
}

void setup() {
  //Serial.begin(115200);
  //Serial.println("Start");
  FastLED.addLeds<WS2812, PIN_PIXEL_DATA, GRB>(pixels, total_pixels);  // GRB ordering is typical

  delay(2000);
  for (int8_t i=0;  i<16;  i++) {
    setNoseStatus(i, true);
    delay(50);
  }

  // Turn off nose.
  setNoseStatus(0, true);

  // Set up touch sensor. Make sure the interrupt pin is an input and pulled high
  pinMode(PIN_TOUCH_INT, INPUT);
  digitalWrite(PIN_TOUCH_INT, HIGH);

  setNoseStatus(1, true);

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!touch.begin(TOUCH_ADDR, &Wire, 16, 8)) {
    while(1);
  }

  setNoseStatus(2, true);
  // Create and interrupt to trigger when a button
  // is hit, the IRQ pin goes low, and the function getNumber is run. 
  //attachInterrupt(0, touchEvent, LOW);

  if (!audioPlayer.begin()) { // initialise the music player
     while(1);
  }
  setNoseStatus(3, true);

  // Set volume for left, right channels. lower numbers == louder volume!
  audioPlayer.setVolume(60,60);
  audioPlayer.useInterrupt(PIN_AUDIO_DREQ);  // DREQ int
  setNoseStatus(4, true);

   if (!SD.begin(PIN_AUDIO_SDCARD_CS)) {
    while (1);  // don't do anything more
  }
  setNoseStatus(5, true);

  audioPlayer.playFullFile("/3beeps.mp3");
  setNoseStatus(6, true);
}

void loop() {
    static uint8_t audioState = 5;
    //if (audioPlayer.stopped()) {
      //audioState = (audioState + 1) & 0x0F;
      //setNoseStatus(audioState, false);
    //  audioPlayer.startPlayingFile("/ic_back.mp3");
    //}

    // Advance .hue by .hue_step
    for (uint8_t l = 0;  l < layer_count;  l++){
      Layers[l].hue += Layers[l].hue_step;
    }

    if (--slow_clock == 0) {
      slow_clock = slow_factor;

      // Advance the nose pixel
      //pixels[nose_counter] = CRGB::Black;
      nose_counter = (nose_counter-1) & 0xf;
      //pixels[nose_counter] = CRGB::Red;
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

    // Overlay touched organs
    touchstate = touch.touched();
    for (int i=1;  i<ORGAN_COUNT;  i++) {
      if (touchstate & (1 << BodyMap[i].sensor)) {
        setOrganColor(i, CRGB::Red);
      }
    }

    FastLED.show();
}
