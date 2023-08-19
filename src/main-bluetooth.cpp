// Communiate over Bluetooth module

#include <Arduino.h>
#include "SoftwareSerial.h"

#define PIN_RX 2
#define PIN_TX 3
#define PIN_STATE 4
#define PIN_AT 5
#define PIN_EN 6

const long baudrate = 38400;
SoftwareSerial BT(PIN_RX, PIN_TX);
char c=' ';
boolean NL = true;
 
void setup() {
    Serial.begin(115200);

    pinMode(PIN_STATE, INPUT);
    pinMode(PIN_EN, OUTPUT);
    digitalWrite(PIN_EN, HIGH);
    pinMode(PIN_AT, OUTPUT);
    digitalWrite(PIN_AT, HIGH);

    BT.begin(baudrate);
    Serial.print("BTserial started at "); Serial.println(baudrate);
    Serial.println(" ");

}

void loop() {
    if (BT.available()) {
        char c = BT.read();
        Serial.write(c);
    }
    
    if (Serial.available()) {
        char c = Serial.read();
        BT.write(c);
 
        // Echo the user input to the main window. The ">" character indicates the user entered text.
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }

}