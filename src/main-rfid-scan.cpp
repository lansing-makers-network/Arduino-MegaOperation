// Enumerates all devices found on an I2C bus multiplexed with one or more TCA9548A
// I2C multiplexer.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

uint8_t mux_addr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t mux_count = 0;

typedef struct Device {
    uint8_t mux_addr;       // I2C address of mux
    uint8_t mux_channel;    // Channel of mux (0..7)
    uint8_t device_addr;       // Address of device
} Device;

Device devices[16];
uint8_t device_count = 0;

void printhex(uint8_t val) {
    Serial.print("0x");
    if (val < 16) Serial.print('0');
    Serial.print(val, HEX);
}

void selectDevice(uint8_t index) {
    if (index >= device_count)
        return;
    for (uint8_t mi=0;  mi < mux_count;  mi++) {
        Wire.beginTransmission(mux_addr[mi]);
        Wire.write(0);
        Wire.endTransmission();
    }

    Device d = devices[index];
    Wire.beginTransmission(d.mux_addr);
    Wire.write(1 << d.mux_channel);
    Wire.endTransmission();
}

Adafruit_PN532 reader(-1, -1);


void setup()
{
    Wire.begin();

    Serial.begin(115200);

    // First, check the address range for TCA9548A multiplexers: 0x70 - 0x77
    Serial.println("Looking for I2C multiplexers:");

    for (uint8_t addr = 0x70; addr < 0x78; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        if (error == 0) {
            mux_addr[mux_count++] = addr;
            Serial.print("Multiplexer found at ");
            printhex(addr);
            Serial.println();
        }
        else if (error==4) {
            Serial.print("Unknown error at address");
            printhex(addr);
            Serial.println();
        }    
    }

    Serial.print("Found ");
    Serial.print(mux_count);
    Serial.println(" multiplexers.");


    // Enumerate all busses connected. Select the bus by setting one bit
    // at a time across all multiplexers, then scanning all addresses except those used
    // by the multiplexers.
    
    // Clear all bits
    for (uint8_t mi = 0;  mi<mux_count;  mi++) {
        Wire.beginTransmission(mux_addr[mi]);
        Wire.write(0);
        Wire.endTransmission();
    }

    // For each mux, iterate each channel
    for (uint8_t mi=0;  mi<mux_count;  mi++) {
        for (uint8_t bit=0;  bit<8;  bit++) {
            Wire.beginTransmission(mux_addr[mi]);
            Wire.write(1 << bit);
            Wire.endTransmission();

            // Index of multiplexer for collision test
            uint8_t mci = 0;

            // Scan the bus for each target addres
            for (uint8_t addr=0;  addr<127;  addr++) {

                // Target collide with mux?
                if (mci < mux_count && addr == mux_addr[mci]) {
                    // In use by multiplexer. Skip.
                    // Serial.print("Skpping ");
                    // printhex(addr);
                    // Serial.println();
                    mci++;
                    continue;
                }

                Wire.beginTransmission(addr);
                if (!Wire.endTransmission()) {
                    devices[device_count].mux_addr = mux_addr[mi];
                    devices[device_count].mux_channel = bit;
                    devices[device_count].device_addr = addr;
                    device_count++;

                    Serial.print("Device at ");
                    printhex(mux_addr[mi]);
                    Serial.print(" bit ");
                    Serial.print(bit);
                    Serial.print(" address ");
                    printhex(addr);
                    Serial.println();
                }
            }

            // Turn off all channels in current mux
            Wire.beginTransmission(mux_addr[mi]);
            Wire.write(0);
            Wire.endTransmission();
        }
    }

    if (device_count == 0) {
        Serial.println("No devices. Goodnight.");
        while(1) {}
    }

    // Just use first device for now
    selectDevice(0);
}

const int DELAY_BETWEEN_CARDS = 500;
long timeLastCardRead = 0;
boolean readerDisabled = false;
int irqCurr;
int irqPrev;

void loop() {
    if (!reader.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)) {
        Serial.println("Error starting");
        return;
    }


    uint8_t success = false;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // read the NFC tag's info
    success = reader.readDetectedPassiveTargetID(uid, &uidLength);
    Serial.println(success ? "Read successful" : "Read failed (not a card?)");

    if (success) {
      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("  UID Value: ");
      reader.PrintHex(uid, uidLength);

      if (uidLength == 4)
      {
        // We probably have a Mifare Classic card ...
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];
        cardid <<= 8;
        cardid |= uid[3];
        Serial.print("Seems to be a Mifare Classic card #");
        Serial.println(cardid);
      }
      Serial.println("");

      timeLastCardRead = millis();
    }

    // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
    readerDisabled = true;

}