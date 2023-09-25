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

bool ATmode = false;
bool PWRmode = false;

void setup() {
    Serial.begin(115200);

    pinMode(PIN_STATE, INPUT);
    pinMode(PIN_PWR_INV, OUTPUT);
    digitalWrite(PIN_PWR_INV, HIGH); // Power OFF
    pinMode(PIN_AT, OUTPUT);
    digitalWrite(PIN_AT, LOW); // AT mode OFF

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    //Serial.println("BTserial waiting");
    BT.begin(38400);
}

// Sends a command to BT module, and also to monitor. The response is echoed back to the monitor.
void sendAndEcho(String s)
{
    Serial.println();
    Serial.print('>');
    Serial.println(s);

    BT.println(s);
    delay(100);

    while (BT.available()) {
        char c = BT.read();
        Serial.write(c);
        delay(25);
    }
}

void blindReset() {
    sendAndEcho("AT");
    sendAndEcho("AT");
    sendAndEcho("AT");
    sendAndEcho("AT+NAME=FAKE3");
    sendAndEcho("AT+ROLE=0");
    sendAndEcho("AT+CMODE=0");
    sendAndEcho("AT+UART=38400,0,0");
    sendAndEcho("AT+PSWD=1234");
}

void setATmode(bool state) {
    if (state) {
        // Enter AT mode
        digitalWrite(PIN_AT, HIGH);
    }
    else {
        // Exit AT mode
        digitalWrite(PIN_AT, LOW);
    }

    ATmode = state;
}

void setPWRmode(bool state) {
    if (state) {
        // Set power ON
        digitalWrite(PIN_PWR_INV, LOW);
    }
    else {
        // Set power OFF
        digitalWrite(PIN_PWR_INV, HIGH);
    }
    PWRmode = state;
}

void loop() {
    if (BT.available()) {
        char c = BT.read();
        if (c != 0) {
            Serial.write(c);
        }
    }
    
    if (Serial.available()) {
        char c = Serial.read();
        switch (c) {

            case '@':
                if (ATmode) {
                    Serial.println("AT mode is already ON");
                    break;
                }
                setATmode(true);
                Serial.println("AT mode ON");
                break;

            case '~':
                if (!ATmode) {
                    Serial.println("AT mode is already OFF");
                    break;
                }
                // Exit AT mode
                setATmode(false);
                Serial.println("AT mode OFF");
                break;

            case '$':
                if (PWRmode) {
                    Serial.println("Power is already ON");
                    break;
                }
                setPWRmode(true);
                Serial.println("Power ON");
                break;

            case '%':
                if (!PWRmode) {
                    Serial.println("Power is already OFF");
                    break;
                }
                setPWRmode(false);
                Serial.println("Power OFF");
                break;

            case '*':
                if (ATmode && PWRmode) {
                    Serial.println("Blind reset!");
                    blindReset();
                }
                else {
                    Serial.println("Set AT mode and PWR mode first.");
                }
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