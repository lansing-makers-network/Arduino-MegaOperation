// Communiate over HC-05 Bluetooth module

#include <Arduino.h>
#include "SoftwareSerial.h"

#define PIN_RX 2
#define PIN_TX 3
#define PIN_STATE 4
#define PIN_EN 5
#define PIN_AT_INV 6
#define PIN_LED 13

const long baudrate = 38400;
SoftwareSerial BT(PIN_RX, PIN_TX);
boolean NL = true;
 
void setup() {
    Serial.begin(115200);

    pinMode(PIN_STATE, INPUT);
    pinMode(PIN_EN, OUTPUT);
    digitalWrite(PIN_EN, HIGH);
    pinMode(PIN_AT_INV, OUTPUT);
    digitalWrite(PIN_AT_INV, LOW); // AT mode ON

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

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
        switch (c) {
            case '@':
                // Enter AT mode
                digitalWrite(PIN_AT_INV, LOW);
                Serial.println("AT mode on");
                break;
            case '~':
                // Exit AT mode
                digitalWrite(PIN_AT_INV, HIGH);
                Serial.println("AT mode off");
                break;
            case '$':
                // Enable high
                digitalWrite(PIN_EN, HIGH);
                Serial.println("Enabled");
                break;
            case '%':
                // Enable low
                digitalWrite(PIN_EN, LOW);
                Serial.println("Disabled");
                break;
            default:
                BT.write(c);
 
                // Echo the user input to the main window. The ">" character indicates the user entered text.
                if (NL) { Serial.print(">");  NL = false; }
                Serial.write(c);
                if (c==10) { NL = true; }
        }
    }
}