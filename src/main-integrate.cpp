#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "innards.h"

// How many leds in your strip?
#define NUM_PIXELS 335

const uint8_t IRQ_PIN = 2;      // IRQ for touch sensor
const uint8_t PIXEL_PIN = 4;    // LED pin

// Define the array of leds
CRGB leds[NUM_PIXELS];

// I2C device addresses
const uint8_t TOUCH_ADDR = 0x5A;

// Touch sensor
Adafruit_MPR121 touch = Adafruit_MPR121();

void fill(Organ org, CRGB::HTMLColorCode color, unsigned long delay_ms) {
  for (int i=org.start; i < org.start+org.len;  i++) {
    leds[i] = color;
    FastLED.show();
    delay(delay_ms);
  }
}

void blink(Organ org, CRGB::HTMLColorCode color, unsigned long delay_ms) {
  for (int i=org.start; i < org.start+org.len;  i++) {
    leds[i] = color;
    FastLED.show();
    delay(delay_ms);
    leds[i] = CRGB::Black;
  }
}

void goround(CRGB::HTMLColorCode color) {

    // Non-nose organs only
    for (int k=1;  k<ORGAN_COUNT;  k++) {
        Serial.print('-');
      blink(BodyMap[k], color, 50);
      fill(BodyMap[k], color, 25);
      fill(BodyMap[k], CRGB::Black, 25);
    }
    Serial.println();
}

void touchEvent()
{
  // Keeps track of the last pins touched
  // so we know when buttons are 'released'
  static uint16_t currtouched = 0;

  // Get the currently touched pads
  currtouched = touch.touched();
  
  for (uint8_t i=0; i<12; i++) {
    if (currtouched & _BV(i)) {
        leds[i] = CRGB::Red;
    }
    else {
        leds[i] = CRGB::Black;
    }
  }

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
    // put your setup code here, to run once:
    FastLED.addLeds<WS2812, PIXEL_PIN, GRB>(leds, NUM_PIXELS);  // GRB ordering is typical

    Serial.begin(115200);
    Serial.println("Mega-Operation"); 

    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    if (!touch.begin(TOUCH_ADDR)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");
    //Serial.end();

    // Make sure the interrupt pin is an input and pulled high
    pinMode(IRQ_PIN, INPUT);
    digitalWrite(IRQ_PIN, HIGH);

    // Interrupt to when touch state changes
    attachInterrupt(digitalPinToInterrupt(IRQ_PIN), touchEvent, FALLING);

    // Once around for the nose.
    blink(BodyMap[0], CRGB::Red, 50);
    fill(BodyMap[0], CRGB::Red, 25);
    fill(BodyMap[0], CRGB::Black, 25);
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
