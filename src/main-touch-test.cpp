#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

const uint8_t IRQ_PIN = 2;

// I2C devices
const uint8_t TOUCH_ADDR = 0x5A;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 touch = Adafruit_MPR121();

void touchEvent()
{
  //  Serial.print("Interupt!");
  // Keeps track of the last pins touched
  // so we know when buttons are 'released'
  static uint16_t lasttouched = 0;
  static uint16_t currtouched = 0;

  // Get the currently touched pads
  currtouched = touch.touched();
  
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

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
  pinMode(IRQ_PIN, INPUT);
  digitalWrite(IRQ_PIN, HIGH);

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
}

void loop() {
}
