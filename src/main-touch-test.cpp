#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_MPR121.h>
#include "pinmap.h"
#include "innards.h"


// I2C devices
const uint8_t TOUCH_ADDR = 0x5A;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 touch = Adafruit_MPR121();

// Pixels
CRGB pixels[PIXEL_COUNT];

// Colors
const CRGB TOUCHED_COLOR = CRGB::Red;
const CRGB UNTOUCHED_COLOR = CRGB::Yellow;

uint16_t touchstate = 0;

void touchEvent()
{
  //  Serial.print("Interupt!");
  // Keeps track of the last pins touched
  // so we know when buttons are 'released'
  static uint16_t lasttouched = 0;
  static uint16_t currtouched = 0;

  // Get the currently touched pads
  touchstate = touch.touched();

  CRGB color = UNTOUCHED_COLOR;
  for (int i=1;  i<ORGAN_COUNT;  i++ ) {
    uint16_t mask = 1 << (i-1);
    if (touchstate & mask) {
      color = TOUCHED_COLOR;
    }

    uint16_t offset = BodyMap[i].start;
    for (int k=0;  k<BodyMap[i].len;  k++) {
      pixels[k+offset] = color;
    }
  }

  FastLED.show();
  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(touch.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(touch.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(touch.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 

  //make sure the interrupt pin is an input and pulled high
  pinMode(PIN_TOUCH_INT, INPUT);
  digitalWrite(PIN_TOUCH_INT, HIGH);

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!touch.begin(TOUCH_ADDR)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  // Create and interrupt to trigger when a button
  // is hit, the IRQ pin goes low, and the function getNumber is run. 
  attachInterrupt(0, touchEvent, LOW);

  FastLED.addLeds<WS2812, PIN_PIXEL_DATA, GRB>(pixels, PIXEL_COUNT);  // GRB ordering is typical


}

void loop() {
}
