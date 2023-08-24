// Communiate over HC-05 Bluetooth module

#include <Arduino.h>
#include "SoftwareSerial.h"

#define PIN_RX 2
#define PIN_TX 3
#define PIN_STATE 4
#define PIN_AT 5
#define PIN_PWR_INV 6
#define PIN_LED 13

const long bitrate_AT = 38400;
const long bitrate_comm = 115200;

SoftwareSerial BT(PIN_RX, PIN_TX);
boolean NL = true;
 
void setup() {
    Serial.begin(115200);

    pinMode(PIN_STATE, INPUT);
    pinMode(PIN_PWR_INV, OUTPUT);
    digitalWrite(PIN_PWR_INV, HIGH); // PWR off
    pinMode(PIN_AT, OUTPUT);
    digitalWrite(PIN_AT, LOW); // AT mode OFF

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    //Serial.println("BTserial waiting");
    BT.begin(38400);
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
                //BT.end();
                digitalWrite(PIN_AT, HIGH);
                //BT.begin(bitrate_AT);
                Serial.println("AT mode ON");
                break;

            case '~':
                // Exit AT mode
                //BT.end();
                digitalWrite(PIN_AT, LOW);
                //BT.begin(bitrate_comm);
                Serial.println("AT mode OFF");
                break;

            case '$':
                // Power on
                digitalWrite(PIN_PWR_INV, LOW);
                Serial.println("Power ON");
                break;

            case '%':
                // Power off
                digitalWrite(PIN_PWR_INV, HIGH);
                Serial.println("Power OFF");
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